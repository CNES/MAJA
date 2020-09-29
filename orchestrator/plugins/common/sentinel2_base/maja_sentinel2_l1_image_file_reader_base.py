# -*- coding: utf-8 -*-
#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""
###################################################################################################
#
#                        o     o
#                        oo   oo   oo        o   oo        ,-.
#                        o o o o  o  o       o  o  o       \_/
#                        o  o  o o    o      o o    o     {|||)<
#                        o     o oooooo      o oooooo      / \
#                        o     o o    o o    o o    o      `-^
#                        o     o o    o  oooo  o    o
#
###################################################################################################

orchestrator.processor.base_processor -- shortdesc

orchestrator.processor.base_processor is the base of all processors

It defines method mandatory for a processor

###################################################################################################
"""
import os
import math
import operator

from orchestrator.cots.otb.algorithms.otb_binary_threshold import binary_threshold
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.common.maja_utils import is_croco_on
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.cots.otb.algorithms.otb_band_math import band_math_or
from orchestrator.cots.otb.algorithms.otb_multiply_by_scalar import multiply_by_scalar
from orchestrator.common.xml_tools import get_root_xml
from orchestrator.common.xml_tools import get_all_values
from orchestrator.cots.gdal.gdal_rasterize import GdalRasterize
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
from orchestrator.cots.otb.algorithms.otb_angle_list_to_image import angle_list_to_image
from orchestrator.cots.otb.algorithms.otb_resample import resample
from orchestrator.cots.otb.algorithms.otb_resample import OtbResampleType
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.plugins.common.base.maja_l1_image_reader_base import L1ImageReaderBase
from orchestrator.algorithms.grid_to_angle import grid_to_angle


LOGGER = configure_logger(__name__)


class Sentinel2L1ImageFileReaderBase(L1ImageReaderBase):

    def __init__(self):
        super(Sentinel2L1ImageFileReaderBase, self).__init__()
        self._toa_scalar_list = []
        self._toa_sub_list = []
        self._l2toaimagelist = []
        self._sol1image = ""
        self._sub_toa = ""
        self._l2satmasklist = []
        self._l2satimagelist = []
        self._subsatimage = ""
        self._l2zonemasklist = []
        self._l2defectmasklist = []
        self._l2edgmasklist = []
        self._l2piximagelist = []
        self._l2dfpimagelist = []
        self._l2zoneimagelist = []
        self._satmasksublist = []
        self._zonemasksublist = []
        self._nodatamasksublist = []
        self._edgsubmask = ""
        self._claimage = ""
        self._vieimagelist = []
        self._meanZenithMap = {}
        self._meanAzimuthMap = {}

        # Store the apps
        self._pipeline = OtbPipelineManager()
        self._subedg_pipeline = OtbPipelineManager()

    def get_l1_toa_cirrus_image(self):
        """
        Get the L1 TOA reflectance image for Cirrus Band
        :return:
        """
        LOGGER.debug("Start L1 reading for L1 Cirrus band ...")
        # TODO: GetCheckXMLFilesWithSchema TBD ConfigUserSystem.GetInstance()
        l_BandsDefinitions = self._plugin.BandsDefinitions  # BandsDefinitions

        l_CirrusBandCode = self._GIPPL2COMMHandler.get_cirrus_band_code()  # string
        LOGGER.debug("Cirrus Viewing band code: %s", l_CirrusBandCode)
        # ---------------------------------------------------------------
        # get the index of the cirrus band in the list of the L1 bands
        # curRes = "R2", bd = 1 <=> "B6" => i = 5
        cirrusBandId = l_BandsDefinitions.L1BandMap.get(l_CirrusBandCode, None)  # int
        LOGGER.debug(
            "Cirrus BandId index: %s with nb index = %s %s )", cirrusBandId, len(
                self._toa_scalar_list), len(
                self._toa_scalar_list))
        return self._toa_scalar_list[cirrusBandId]

    def get_dtm_vie_image(self):
        """
        Get DTMVIE Image
        :return:
        """
        l_DTMBandCode = self._GIPPL2COMMHandler.l2_comm_values.get("DTMViewingDirectionBandCode", None)[0]
        LOGGER.debug("DTM Viewing band code: %s", l_DTMBandCode)
        # TODO; check GetL2CoarseResolution grep L2CoarseResolution
        l_DTMBandId = self._plugin.BandsDefinitions.get_band_id_in_l2_coarse(l_DTMBandCode)
        return self._vieimagelist[l_DTMBandId]

    def get_shadow_vie_image(self):
        """
        Get ShadowVIE Image
        :return:
        """
        l_CLDBandCode = self._GIPPL2COMMHandler.get_cld_viewing_direction_band_code()
        l_CLDBandId = self._plugin.BandsDefinitions.get_band_id_in_l2_coarse(l_CLDBandCode)
        return self._vieimagelist[l_CLDBandId]

    def generate_toa_caching(self, listOfTOAImageFileNames, reflectanceMultiplicationValues, working):
        """

        :param listOfTOAImageFileNames: ListOfStrings
        :param reflectanceMultiplicationValues: ListOfDoubles
        :return:
        """
        if len(reflectanceMultiplicationValues) != len(listOfTOAImageFileNames):
            raise MajaDataException("Not the same number of band images and coefficients")

        # ---------------------------------------------------------------
        # Get the number of band with the number of TOA image files set in the input product directory
        l_NbBand = len(listOfTOAImageFileNames)  # int

        # Convert the input jp2 images in tif
        for i in range(l_NbBand):
            toaFilename = listOfTOAImageFileNames[i]
            LOGGER.debug("Caching the <%s> image filename...", toaFilename)
            LOGGER.debug("Reflectance quantification value: %s", reflectanceMultiplicationValues[i])
            # Initialize the TOA reader
            l_ImageFilename = os.path.join(working, "toaconverted_" + str(i) + ".tif")
            # extract_roi(toaFilename,[0],l_ImageFilename,working)
            app = multiply_by_scalar(
                toaFilename,
                reflectanceMultiplicationValues[i],
                output_image=l_ImageFilename,
                write_output=True)
            self._pipeline.add_otb_app(app)
            self._toa_scalar_list.append(app.getoutput()["out"])

    def generate_toa_sub_images(self, working):
        dtm_coarse = self._dem.ALC

        # For each band of the input product
        tmp_sub_toa_pipe = OtbPipelineManager()
        for i, toa in enumerate(self._toa_scalar_list):
            # undersampling at L2CoarseResolution
            toa_sub_image = os.path.join(working, "aot_sub_{}.tif".format(i))
            app = resample(toa, dtm_coarse, toa_sub_image, OtbResampleType.LINEAR_WITH_RADIUS, write_output=False)
            self._toa_sub_list.append(app.getoutput()["out"])
            tmp_sub_toa_pipe.add_otb_app(app)
        # end band loop

        # *******************************************************************************************************
        # TOA Sub image pipeline connection
        # *******************************************************************************************************
        toa_sub_image = os.path.join(working, "aot_sub.tif")
        param_concatenate = {"il": self._toa_sub_list,
                             "out": toa_sub_image
                             }
        OtbAppHandler("ConcatenateDoubleImages", param_concatenate)
        self._sub_toa = toa_sub_image
        tmp_sub_toa_pipe.free_otb_app()

    def rasterize_gml_masks(self, l1Resolution, l2res, l2Area, projectionRef, l1Band, satFilename, defectFilename,
                          zoneFilename, boundingBox, gdalRasterizeMaskCmd, gdalRasterizeDetCmd,
                            tmp_constant_image, working):
        """

        :param l1Resolution: string
        :param l2res: int
        :param l2Area: AreaType
        :param projectionRef: string
        :param l1Band: int
        :param satFilename: string
        :param defectFilename: string
        :param zoneFilename: string
        :param boundingBox: BoundingBoxType
        :param gdalRasterizeMaskCmd: string
        :param gdalRasterizeDetCmd: string
        :return:
        """

        # convert the band index in string
        l1BandStr = str(l1Band)

        LOGGER.debug("RasterizeGmlMasks mask for band: %s\n"
                     " - for l1Resolution  = %s"
                     "\n - l2res             = %s \n \n - l2Area    = %s"
                     "\n - satFilename       = %s \n - defectFilename    = %s"
                     "\n - zoneFilename      = %s \n", l1Band, l1Resolution, l2res, l2Area,
                     satFilename, defectFilename, zoneFilename)


        # TODO: la generation d'une image constante cots.otb.otb_constant_image
        # ------------------------------------------------------------

        # -----------------------------------------------------------------------------------
        # Rasterize gml masks at L2 resolution with gdal_rasterize system command
        # -----------------------------------------------------------------------------------

        # -----------------------------------------------------------------------------------
        # Detectors FootPrint
        # check if the gml mask contains features
        count_zone = self.get_mask_feature_count(zoneFilename)
        LOGGER.debug("MaskFeatureCount for " + zoneFilename + ": " + str(count_zone))

        if count_zone > 0:
            # Get the shift between the detector index and the feature index
            shift_res = self.get_detfoo_index_shift(zoneFilename)

            if shift_res[0]:
                sqlRequest = gdalRasterizeDetCmd
                sqlRequest = sqlRequest + """-a fid2 -sql \'select fid + """ + \
                    str(shift_res[1]) + """ as fid2, * from MaskFeature \'"""
                l_image_raster_filename = os.path.join(
                    working,
                    "L2ZoneMaskReaders_Resol_" +
                    l1Resolution +
                    "_band_id_" +
                    l1BandStr +
                    ".tif")
                GdalRasterize().internal_rasterize_gml_macro(zoneFilename, boundingBox.xmin, boundingBox.ymin,
                                                          boundingBox.xmax, boundingBox.ymax,
                                                          l2Area.size[0], l2Area.size[1], projectionRef,
                                                          l_image_raster_filename,
                                                          sqlRequest)
                self._l2zonemasklist[l2res].append(l_image_raster_filename)
            else:
                LOGGER.debug("Bad Order DETFOO detected")
                sqlRequest = gdalRasterizeDetCmd
                sqlRequest = sqlRequest + """-a fid2 -sql \'select fid + 1 as fid2, * from MaskFeature \'"""
                l_image_raster_filename = os.path.join(
                    working,
                    "L2ZoneMaskReaders_Resol_" +
                    l1Resolution +
                    "_band_id_" +
                    l1BandStr +
                    ".tif")
                GdalRasterize().internal_rasterize_gml_macro(zoneFilename, boundingBox.xmin, boundingBox.ymin,
                                                             boundingBox.xmax, boundingBox.ymax,
                                                             l2Area.size[0], l2Area.size[1], projectionRef,
                                                             l_image_raster_filename,
                                                             sqlRequest)
                l_fid_dets = self.get_detfoo_index_detector_list(zoneFilename, 1)
                l_image_changed_filename = os.path.join(
                    working,
                    "L2ZoneMaskReadersChanged_Resol_" +
                    l1Resolution +
                    "_band_id_" +
                    l1BandStr +
                    ".tif")
                change_values_param = { "im" : l_image_raster_filename,
                                        "out" : l_image_changed_filename,
                                        "invals" : [str(a) for a in l_fid_dets.keys()],
                                        "outvals": [str(a) for a in l_fid_dets.values()]
                                        }
                change_values_app = OtbAppHandler("ChangeValues", change_values_param)
                self._l2zonemasklist[l2res].append(change_values_app.getoutput().get("out"))

        else:
            self._l2zonemasklist[l2res].append(tmp_constant_image)

        # -----------------------------------------------------------------------------------
        # Saturated SAT
        # check if the gml mask contains features
        count_sat = self.get_mask_feature_count(satFilename)
        LOGGER.debug("MaskFeatureCount for " + satFilename + ": " + str(count_sat))
        if count_sat > 0:
            l_image_raster_filename = os.path.join(
                working,
                "L2SatPixReaders_Resol_" +
                l1Resolution +
                "_band_id_" +
                l1BandStr +
                ".tif")
            GdalRasterize().internal_rasterize_gml_macro(satFilename, boundingBox.xmin, boundingBox.ymin,
                                                      boundingBox.xmax, boundingBox.ymax,
                                                      l2Area.size[0], l2Area.size[1], projectionRef,
                                                      l_image_raster_filename,
                                                      gdalRasterizeMaskCmd)
            self._l2satimagelist[l2res].append(l_image_raster_filename)
        else:

            self._l2satimagelist[l2res].append(tmp_constant_image)

        # -----------------------------------------------------------------------------------
        # Defectiv PIX
        # check if the gml mask contains features
        count_pix = self.get_mask_feature_count(defectFilename)
        LOGGER.debug("MaskFeatureCount for " + defectFilename + ": " + str(count_pix))
        if count_pix > 0:
            l_image_raster_filename = os.path.join(
                working,
                "L2DefectivPixReaders_Resol_" +
                l1Resolution +
                "_band_id_" +
                l1BandStr +
                ".tif")
            GdalRasterize().internal_rasterize_gml_macro(defectFilename, boundingBox.xmin, boundingBox.ymin,
                                                      boundingBox.xmax, boundingBox.ymax,
                                                      l2Area.size[0], l2Area.size[1], projectionRef,
                                                      l_image_raster_filename,
                                                      gdalRasterizeMaskCmd)
            self._l2defectmasklist[l2res].append(l_image_raster_filename)
        else:
            self._l2defectmasklist[l2res].append(tmp_constant_image)

        return count_zone, count_sat, count_pix

    def generate_mask_rasters_gml(self, boundingBoxMap, l2Areas, projectionRef, satPixFileNames, defectivPixFileNames,
                            zoneMaskFileNames, gdalRasterizeMaskCmd, gdalRasterizeDetCmd, working):
        """

        :param boundingBoxMap: BoundingBoxMapType
        :param projectionRef: string
        :param satPixFileNames: ListOfStrings
        :param defectivPixFileNames: ListOfStrings
        :param zoneMaskFileNames: ListOfStrings
        :param gdalRasterizeMaskCmd: string
        :param gdalRasterizeDetCmd: string
        :param working: string
        :return:
        """

        # *******************************************************************************************************
        # Generate mask rasters by rasterizing the gml mask per L2 resolution per band
        # *******************************************************************************************************
        LOGGER.debug("Start GML mask rasterization ...")
        l_BandsDefinitions = self._plugin.BandsDefinitions
        l2Area = None
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution  # ListOfStrings
        l_NbL2Res = len(l_ListOfL2Resolution)

        for l2res in range(l_NbL2Res):
            self._l2defectmasklist.append([])
            self._l2zonemasklist.append([])
            self._l2satimagelist.append([])

        for l2res in range(l_NbL2Res):
            # Current resolution: "R1" or "R2"
            curRes = l_ListOfL2Resolution[l2res]

            # Get the list of band of the current resolution
            listOfL2Bands = l_BandsDefinitions.get_list_of_l2_band_code(curRes)
            nbBand = len(listOfL2Bands)
            l2Area = l2Areas[l2res]
            counts = (0, 0, 0)
            # Generate a constant image that will be used if the gml masks are empty (no feature)
            tmp_constant_filename = os.path.join(working, "const_{}.tif:uint8".format(l2res))
            tmp_constant_image_app = constant_image(self._dem.ALTList[l2res], 0, tmp_constant_filename,
                                                    write_output=True)
            tmp_constant_image = tmp_constant_image_app.getoutput()["out"]
            # For each band of the current resolution
            for l_StrBandIdL2 in listOfL2Bands:
                # Get the L1 band index associated to the L2 band code
                l1BandIdx = l_BandsDefinitions.get_band_id_in_l1(l_StrBandIdL2)
                LOGGER.debug("Sentinel2L1ImageFileReaderBase::GenerateMaskRasters: CurrentResol = %s, reading the "
                             "BandId L1 (associated) <%s> with index <%s>.", curRes, l_StrBandIdL2,
                             l1BandIdx)

                # Read the bounding box for the resolution  "l_L1Resolution"
                l_BoundingBox = boundingBoxMap.get(curRes)

                # =======> RASTERIZE GML MASKS
                new_counts = self.rasterize_gml_masks(curRes, l2res, l2Area, projectionRef, l1BandIdx,
                                                    satPixFileNames[l1BandIdx],
                                                    defectivPixFileNames[l1BandIdx],
                                                    zoneMaskFileNames[l1BandIdx],
                                                    l_BoundingBox, gdalRasterizeMaskCmd,
                                                    gdalRasterizeDetCmd, tmp_constant_image, working)
                counts = tuple(map(operator.add, counts, new_counts))

            # band loop

            # *******************************************************************************************************
            # L2 Zone mask pipeline connection
            # *******************************************************************************************************
            if counts[0] != 0:
                zone_mask = os.path.join(working, "Masks_Zone_{}.tif".format(l2res))
                param_concatenate = {"il": self._l2zonemasklist[l2res],
                                     "out": zone_mask+":uint8"
                                     }
                l2zoneimage_app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate,
                                                write_output=(False or is_croco_on("sentinel2.l1reader.l2zone")))
                self._l2zoneimagelist.append(l2zoneimage_app.getoutput().get("out"))
                self._pipeline.add_otb_app(l2zoneimage_app)
            else:
                self._l2zoneimagelist.append(self._l2zonemasklist[l2res][0])
            # *******************************************************************************************************
            # PIX image pipeline connection (DEFECTIV PIX)
            # *******************************************************************************************************
            if counts[2] != 0:
                # Compress (concatenate the vector image to binary image)
                pix_vector_mask = os.path.join(working, "Masks_Defect_Vector_{}.tif".format(l2res))
                param_concatenate = {"il": self._l2defectmasklist[l2res],
                                     "out": pix_vector_mask + ":uint8"
                                     }
                pix_vector = OtbAppHandler("ConcatenateDoubleImages", param_concatenate)
                pix_mask = os.path.join(working, "Masks_Defect_{}.tif".format(l2res))
                param_binconcatenate = {"im": pix_vector.getoutput().get("out"),
                                        "out": pix_mask + ":uint16"
                                        }
                pix = OtbAppHandler("BinaryConcatenate", param_binconcatenate,
                                    write_output=(False or is_croco_on("sentinel2.l1reader.l2pix")))
                self._pipeline.add_otb_app(pix)
                self._l2piximagelist.append(pix.getoutput().get("out"))
            else:
                self._l2piximagelist.append(self._l2defectmasklist[l2res][0])
            # end res loop
        LOGGER.debug("End GML mask rasterization ...")

    def get_solar_grids(self, dtm, solarAnglesFile, solH1, working):

        solar_grid_filename = os.path.join(working, "solar_grid.tif")

        # angle_list_to_image()
        solarangle_grid_app = angle_list_to_image(dtm, solarAnglesFile, solar_grid_filename, write_output=False)
        # Multiply by the solar reference altitude
        solar_grid_mult_filename = os.path.join(working, "solar_grid_mult.tif")
        param_scaled_solar = {"im": solarangle_grid_app.getoutput().get("out"),
                              "coef": float(solH1),
                              "out": solar_grid_mult_filename
                              }
        rta_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_solar, write_output=False)
        # Expand at L2Coarse.
        solar_grid_resamp_filename = os.path.join(working, "solar_grid_resamp.tif")
        resample(rta_scal_app.getoutput().get("out"), dtm, solar_grid_resamp_filename, OtbResampleType.LINEAR)
        self._sol1image = solar_grid_resamp_filename

    def get_viewing_grids(self, band, l2CoarseArea, projectionRef, vieHRef, satFilename, zoneFilename, nodataFilename,
                        listOfZone, boundingBox, viewing_angles, gdalRasterizeMaskCmd,
                        gdalRasterizeDetCmd, gdalRasterizeNdtCmd, nbCoarseBands, working):
        """

        :param band: int
        :param l2CoarseArea: AreaType
        :param projectionRef: string
        :param vieHRef: double
        :param satFilename: string
        :param zoneFilename: string
        :param nodataFilename: string
        :param listOfZone: ListOfUIntegers
        :param boundingBox: BoundingBoxType
        :param viewing_angles:
        :param gdalRasterizeMaskCmd: string
        :param gdalRasterizeDetCmd: string
        :param gdalRasterizeNdtCmd: string
        :return:
        """
        dtm_coarse = self._dem.ALC

        # Set a constant image if the gml masks are empty
        tmp_constant_filename = os.path.join(working, "Masks_sat_const.tif")
        constant_image(dtm_coarse, 0, tmp_constant_filename + ":uint8", write_output=True)

        # -----------------------------------------------------------------------------------
        # Rasterize all the gml maks at L2 coarse resolution
        # to generate the viewing grids
        # -----------------------------------------------------------------------------------

        # -----------------------------------------------------------------------------------
        # Detectors FootPrint
        # check if the gml mask contains features
        count = self.get_mask_feature_count(zoneFilename)

        if count > 0:
            # Get the shift between the detector index and the feature index
            shift_res = self.get_detfoo_index_shift(zoneFilename)

            if shift_res[0]:
                l_image_raster_filename = os.path.join(working,
                                                       "SubZoneMask_band_id_{}.tif".format(band))
                # Additional parameters of the gdal_rasterize system command
                sqlRequest = gdalRasterizeDetCmd + \
                    "-a fid2 -sql 'select fid + {} as fid2, * from MaskFeature'".format(shift_res[1])

                GdalRasterize().internal_rasterize_gml_macro(
                    zoneFilename,
                    boundingBox.xmin,
                    boundingBox.ymin,
                    boundingBox.xmax,
                    boundingBox.ymax,
                    l2CoarseArea.size[0],
                    l2CoarseArea.size[1],
                    projectionRef,
                    l_image_raster_filename,
                    sqlRequest)

                submask_resamp_filename = os.path.join(working,
                                                       "SubZoneMask_resampled_band_id_{}.tif".format(band))
                resample(l_image_raster_filename, dtm_coarse,
                         submask_resamp_filename, OtbResampleType.LINEAR)

                self._zonemasksublist.append(submask_resamp_filename)
            else:
                LOGGER.debug("Bad Order DETFOO detected")
                l_image_raster_filename = os.path.join(working,
                                                       "SubZoneMask_band_id_{}.tif".format(band))
                # Additional parameters of the gdal_rasterize system command
                sqlRequest = gdalRasterizeDetCmd + \
                             "-a fid2 -sql 'select fid + 1 as fid2, * from MaskFeature'"

                GdalRasterize().internal_rasterize_gml_macro(
                    zoneFilename,
                    boundingBox.xmin,
                    boundingBox.ymin,
                    boundingBox.xmax,
                    boundingBox.ymax,
                    l2CoarseArea.size[0],
                    l2CoarseArea.size[1],
                    projectionRef,
                    l_image_raster_filename,
                    sqlRequest)
                l_fid_dets = self.get_detfoo_index_detector_list(zoneFilename, 1)
                l_image_changed_filename = os.path.join(
                    working,
                    "SubZoneMaskChanged_band_id_{}.tif".format(band))
                change_values_param = {"im": l_image_raster_filename,
                                       "out": l_image_changed_filename + ":uint8",
                                       "invals": [str(a) for a in l_fid_dets.keys()],
                                       "outvals": [str(a) for a in l_fid_dets.values()]
                                       }
                change_values_app = OtbAppHandler("ChangeValues", change_values_param)

                submask_resamp_filename = os.path.join(working,
                                                       "SubZoneMask_resampled_band_id_{}.tif".format(band))
                resample(change_values_app.getoutput().get("out"), dtm_coarse,
                         submask_resamp_filename, OtbResampleType.LINEAR)

                self._zonemasksublist.append(submask_resamp_filename)

        else:
            submask_resamp_filename = os.path.join(working,
                                                   "SubZoneMask_resampled_band_id_{}.tif".format(band))
            resample(tmp_constant_filename, dtm_coarse,
                     submask_resamp_filename, OtbResampleType.LINEAR)
            self._zonemasksublist.append(submask_resamp_filename)

        # -----------------------------------------------------------------------------------
        # Saturated pixel mask at L2 coarse
        count = self.get_mask_feature_count(satFilename)

        if count > 0:
            l_image_raster_filename = os.path.join(working,
                                                   "SubSatMask_band_id_{}.tif".format(band))
            GdalRasterize().internal_rasterize_gml_macro(satFilename, boundingBox.xmin, boundingBox.ymin, boundingBox.xmax,
                                                      boundingBox.ymax,
                                                      l2CoarseArea.size[0], l2CoarseArea.size[1], projectionRef,
                                                      l_image_raster_filename,
                                                      gdalRasterizeMaskCmd)
            self._satmasksublist.append(l_image_raster_filename)
        else:
            self._satmasksublist.append(tmp_constant_filename)

        # -----------------------------------------------------------------------------------
        # No_data mask at L2 coarse
        count = self.get_mask_feature_count(nodataFilename, "NODATA")

        if count > 0:
            l_image_raster_filename = os.path.join(working,
                                                   "SubNoDataMask_band_id_{}.tif".format(band))
            GdalRasterize().internal_rasterize_gml_macro(nodataFilename, boundingBox.xmin, boundingBox.ymin,
                                                      boundingBox.xmax, boundingBox.ymax,
                                                      l2CoarseArea.size[0], l2CoarseArea.size[1], projectionRef,
                                                      l_image_raster_filename,
                                                      gdalRasterizeNdtCmd)
            self._nodatamasksublist.append(l_image_raster_filename)
        else:
            self._nodatamasksublist.append(tmp_constant_filename)

        l_VieAnglesGridList = []
        l_nbDetectors = len(viewing_angles)

        # Detector loop
        LOGGER.debug("For each detectors (nb=%s) ...", l_nbDetectors)
        for angle in viewing_angles:
            l_vieAngleFile = angle.write(working)
            # ---------------------------------------------------------------------------------
            # Generate an image with the list of viewing angle values set in the header file
            # ---------------------------------------------------------------------------------
            viewing_grid_filename = os.path.join(working, "viewing_grid_{}_{}.tif".format(angle.detector_id, band))

            # angle_list_to_image()
            viewing_angle_app = angle_list_to_image(
                dtm_coarse, l_vieAngleFile, viewing_grid_filename, write_output=False, extrapolation=True)

            # Expand at L2Coarse.
            viewing_grid_resamp_filename = os.path.join(
                working, "viewing_grid_resamp_{}_{}.tif".format(
                    angle.detector_id, band))
            resample(viewing_angle_app.getoutput().get("out"), dtm_coarse,
                     viewing_grid_resamp_filename, OtbResampleType.LINEAR)

            # add images in a list
            l_VieAnglesGridList.append(viewing_grid_resamp_filename)

        # end detector loop
        LOGGER.debug("Start ConcatenatePerZoneVectorImageFilter for band id [%s]...", band)

        # -----------------------------------------------------------------------------------
        # Generate the angle images using the zone (detector) mask
        # -----------------------------------------------------------------------------------
        # Concatenate all the detectors
        viewing_concat_filename = os.path.join(working, "viewing_concat_{}.tif".format(band))
        param_concat_perzone = {"mask": self._zonemasksublist[-1],
                                "il": l_VieAnglesGridList,
                                "zonelist": listOfZone,
                                "out": viewing_concat_filename
                                }
        concat_perzone = OtbAppHandler("ConcatenatePerZone", param_concat_perzone)
        # Multiply by reference altitude
        viewing_grid_mult_filename = os.path.join(working, "viewing_grid_mult_{}.tif".format(band))
        param_scaled_solar = {"im": viewing_concat_filename,
                              "coef": float(vieHRef),
                              "out": viewing_grid_mult_filename
                              }
        view_scale_app = OtbAppHandler("MultiplyByScalar", param_scaled_solar, write_output=True)
        self._vieimagelist.append(view_scale_app.getoutput().get("out"))

        l_nbZones = len(listOfZone)
        LOGGER.debug("Start Loop for Zone (nb=" + str(l_nbZones) + ")...")
        for d in range(l_nbZones):
            l_zone = listOfZone[d]
            # -----------------------------------------------------------------------------------
            # Compute average values of zenithal and azimuthal angles grid per zone (detector in level1B)
            # -----------------------------------------------------------------------------------
            # VAP Reader connection (from ATB)
            tmp_azi = os.path.join(working, "tmp_azi_{}_{}.tif".format(band, l_zone))
            tmp_azi_image = extract_roi(l_VieAnglesGridList[d], [1], tmp_azi)
            param_stats = {"im": tmp_azi,
                           "exclude": 1,
                           "mask": self._zonemasksublist[band],
                           "maskforeground": int(l_zone)
                           }
            l2_stat = OtbAppHandler("Stats", param_stats)

            azi_mean = l2_stat.getoutput().get("mean")
            tmp_zen = os.path.join(working, "tmp_zen_{}_{}.tif".format(band, l_zone))
            tmp_zen_image = extract_roi(l_VieAnglesGridList[d], [0], tmp_zen)
            param_stats = {"im": tmp_zen,
                           "exclude": 1,
                           "mask": self._zonemasksublist[band],
                           "maskforeground": int(l_zone)
                           }
            l2_stat = OtbAppHandler("Stats", param_stats)
            zen_mean = l2_stat.getoutput().get("mean")
            tmp_mean = (zen_mean, azi_mean)
            l_ViewingAngleMean = grid_to_angle(tmp_mean)
            l_ViewingAngleMeanDeg = (l_ViewingAngleMean[0] * 180.0 / math.pi, l_ViewingAngleMean[1] * 180.0 / math.pi)
            # Add a vector to mean maps
            if l_zone not in self._meanZenithMap:
                self._meanZenithMap[listOfZone[d]] = ["0"] * nbCoarseBands
            if l_zone not in self._meanAzimuthMap:
                self._meanAzimuthMap[listOfZone[d]] = ["0"] * nbCoarseBands
            self._meanZenithMap[listOfZone[d]][band] = str(l_ViewingAngleMeanDeg[0])
            self._meanAzimuthMap[listOfZone[d]][band] = str(l_ViewingAngleMeanDeg[1])
            LOGGER.debug(" For BandId[" +
                         str(band) +
                         "], zone [" +
                         str(listOfZone[d]) +
                         "]  . Mean 'GRID View angles'=" +
                         str(tmp_mean) +
                         " . Mean 'View angles'=" +
                         str(l_ViewingAngleMean[0]) +
                         ":" +
                         str(l_ViewingAngleMean[1]) +
                         " rad. ou  " +
                         str(l_ViewingAngleMeanDeg[0]) +
                         ":" +
                         str(l_ViewingAngleMeanDeg[1]) +
                         " deg.")
        LOGGER.debug("Start Loop for Zone done.")

    def generate_edg_images(self, working_dir):
        """

        :param working_dir: String
        :return:
        """
        dtm_coarse = self._dem.ALC

        list_of_binary_threshold = []
        m_OrFilter1List = []
        m_OrFilter2List = []

        # *******************************************************************************************************
        # IPEDGSub pipeline connection
        # *******************************************************************************************************
        LOGGER.debug("Start IPEDGSub ...")
        # At L2 coarse resolution

        # Set to 0 the pixel that are not edges in the zone (detector) mask image raster
        out_thresh_0 = os.path.join(working_dir, "IPEDGOrFilter1SubMask_0.tif")
        out_binary_threshold_ipedg_image_b1 = binary_threshold(input_image=self._zonemasksublist[0],
                                                               inside_value=0,
                                                               outside_value=1,
                                                               output_image=out_thresh_0 + ":uint8",
                                                               lower_threshold=1,
                                                               write_output=False)

        # Concatenate the zone image and the nodata one
        m_OrFilter2List.append(out_binary_threshold_ipedg_image_b1.getoutput()["out"])
        m_OrFilter2List.append(self._nodatamasksublist[0])
        l_NbBand = len(self._zonemasksublist)
        # band loop
        for l_band in range(l_NbBand - 1):
            out_thresh_1 = os.path.join(working_dir, "IPEDGThreshFilter1SubMask_{}.tif".format(l_band + 1))
            out_binary_threshold_ipedg_image = binary_threshold(input_image=self._zonemasksublist[l_band + 1],
                                                                lower_threshold=1,
                                                                inside_value=0,
                                                                outside_value=1,
                                                                output_image=out_thresh_1 + ":uint8",
                                                                write_output=False)
            self._subedg_pipeline.add_otb_app(out_binary_threshold_ipedg_image)
            m_OrFilter2List.append(out_binary_threshold_ipedg_image.getoutput().get("out"))
            m_OrFilter2List.append(self._nodatamasksublist[l_band+1])

        #OR filter
        out_edg = os.path.join(working_dir, "IPEDGubMask.tif")
        or_edg_sub_app = band_math_or(m_OrFilter2List,out_edg + ":uint8", write_output=True)
        # Allocation of 7 Go in S2 MUSCATE case
        self._edgsubmask = or_edg_sub_app.getoutput().get("out")
        self._subedg_pipeline.free_otb_app()
        LOGGER.debug("Start IPEDGSub done.")

        # *******************************************************************************************************
        # L2EDG pipeline connection
        # *******************************************************************************************************
        LOGGER.debug("Start L2EDG ...")

        l_BandsDefinitions = self._plugin.BandsDefinitions
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution  # ListOfStrings
        # At L2 resolution
        l_NbL2Res = len(l_ListOfL2Resolution)

        for r in range(l_NbL2Res):
            res_str = l_ListOfL2Resolution[r]
            # Set 1000 to edge pixels to identify the pixel contaminated by an edge pixel after resampling
            out_thresh = os.path.join(working_dir, "EDGThreshL2_{}.tif".format(res_str))
            #m_L2EDGThreshold_app = binary_threshold(self._edgsubmask,
            #                                         lower_threshold=0,
            #                                         inside_value=0,
            #                                         outside_value=1000,
            #                                         output_image=out_thresh + ":uint8",
            #                                         write_output=False)
            #self._pipeline.add_otb_app(m_L2EDGThreshold_app)
            # ExpandFilterPointer => PadAndResampleImageFilter => app ressampling
            out_ressampling = os.path.join(working_dir, "IPEDGRealL2_{}.tif".format(res_str))
            resamp_app = resample(self._edgsubmask, self._dem.ALTList[r],
                                  out_ressampling + ":uint8",
                                  OtbResampleType.LINEAR,
                                  threshold=0.000001,
                                  write_output=False)
            self._pipeline.add_otb_app(resamp_app)

            # Set Threshold value to one because the expand filter interpolates values set to 0
            # or 1000 in the first threshold and adds systematically CONST_EPSILON to the output value.
            #m_L2EDGThresholdImage2_out = os.path.join(working_dir, "IPEDGMaskL2_{}.tif".format(res_str))
            #m_L2EDGThresholdImageApp2 = binary_threshold(
            #    resamp_app.getoutput().get("out"),
            #    lower_threshold=0.,
            #    inside_value=0,
            #    outside_value=1,
            #    output_image=m_L2EDGThresholdImage2_out + ":uint8",
            #    write_output=False)
            #self._pipeline.add_otb_app(m_L2EDGThresholdImageApp2)
            self._l2edgmasklist.append(resamp_app.getoutput().get("out"))

    def generate_l2_toa_images(self, working_dir):
        """
        :param working_dir:
        :return:
        """
        l_BandsDefinitions = self._plugin.BandsDefinitions
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution  # ListOfStrings
        l_NbL2Res = len(l_ListOfL2Resolution)

        for l2res in range(l_NbL2Res):
            # Current resolution: "R1" or "R2"
            curRes = l_ListOfL2Resolution[l2res]

            # Get the list of band of the current resolution
            listOfL2Bands = l_BandsDefinitions.get_list_of_l2_band_code(curRes)
            nbBand = len(listOfL2Bands)
            list_of_image = []
            # For each band of the current resolution
            for l_StrBandIdL2 in listOfL2Bands:
                # Get the L1 band index associated to the L2 band code
                l1BandIdx = l_BandsDefinitions.get_band_id_in_l1(l_StrBandIdL2)
                # Generate the list of L2 TOA images per resolution
                list_of_image.append(self._toa_scalar_list[l1BandIdx])
            out_concatenate = os.path.join(working_dir, "L2TOAImageListVector_" + curRes + ".tif")
            param_concatenate = {"il": list_of_image,
                                 "out": out_concatenate}
            l2toa_concat_app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate,
                                             write_output=(False or is_croco_on("sentinel2.l1reader.l2toa")))
            self._pipeline.add_otb_app(l2toa_concat_app)
            self._l2toaimagelist.append(l2toa_concat_app.getoutput().get("out"))

    def generate_sat_images(self, working_dir):
        """

        :return:
        """

        # *******************************************************************************************************
        # IPSAT Sub image pipeline connection
        # *******************************************************************************************************
        out_concatenate = os.path.join(working_dir, "SubSatVector.tif")
        param_concatenate = {"il": self._satmasksublist,
                             "out": out_concatenate + ":uint8"
                             }
        concat_app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate)
        self._subsatimage = concat_app.getoutput().get("out")
        # *******************************************************************************************************
        # L2SAT image pipeline connection
        # *******************************************************************************************************
        l_BandsDefinitions = self._plugin.BandsDefinitions
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution
        # At L2 resolution
        l_NbL2Res = len(l_ListOfL2Resolution)
        for i in range(l_NbL2Res):
            str_res = l_ListOfL2Resolution[i]
            out_concatenate = os.path.join(working_dir, "L2SATVector_{}.tif".format(str_res))
            param_concatenate = {"il": self._l2satimagelist[i],
                                 "out": out_concatenate + ":uint8"
                                 }
            sat_image = OtbAppHandler("ConcatenateDoubleImages", param_concatenate, write_output=False)
            self._pipeline.add_otb_app(sat_image)
            self._l2satmasklist.append(sat_image.getoutput().get("out"))

    def generate_cla_images(self, realL1Nodata, working):
        cla_filename = os.path.join(working, "cla_constant_sub.tif")
        cla_image = constant_image(self._dem.ALC, realL1Nodata, cla_filename, write_output=True).getoutput()["out"]
        self._claimage = cla_image

    @staticmethod
    def get_mask_feature_count(maskFilename, regex=None):
        """
        Get the number of layers in a gml file
        :param maskFilename:
        :return:
        """
        maskHandler = get_root_xml(maskFilename, deannotate=True)
        xnodes = get_all_values(maskHandler, "//*[local-name(.) = 'MaskFeature']")
        if xnodes is not None:
            if regex is None:
                return len(xnodes)
            else:
                nb = 0
                for x in xnodes:
                    for a in list(x.items()):
                        if regex in a[1]:
                            nb = nb + 1
                return nb
        else:
            return 0

    @staticmethod
    def get_detfoo_index_shift(maskFilename):
        """
        Get the shift between the detector index and the feature index in the MSK_DETFOO mask

        :param maskFilename: string
        :return:
        """
        shift = 0

        # Load the MSK_DETFOO gml file
        maskHandler = get_root_xml(maskFilename, deannotate=True)

        # Get the mask feature nodes
        # as : <eop:MaskFeature gml:id="detector_footprint-B02-10-2">
        xnodes = get_all_values(maskHandler, "//*[local-name(.) = 'MaskFeature']")
        # Two indices : the detector index and the feature index
        detId = 0

        # For each feature
        for i in range(len(xnodes)):
            feat = xnodes[i]
            prevShift = shift
            prevDetId = detId
            # "detector_footprint-B02-10-2"
            attribute = list(feat.items())[0][1]
            element = attribute.split('-')

            # Get the detector index and the feature index
            detId = int(element[-2])
            featId = int(element[-1])

            # The shift between those two indices must be constant
            # because it should not have a gap of detector between to consecutive features
            shift = detId - featId
            # Check the index values of the next features
            if (i > 0) and ((shift != prevShift) or (detId != prevDetId + 1)):
                LOGGER.warn(
                    "Sentinel2L1ImageFileReaderBase::get_detfoo_index_shift: The detector indices are not in ascending order "
                    "or do not have a constant shift with the feature indices in the MSK_DETFOO " + maskFilename + " !")
                return False, shift

        return True,shift

    # Get the shift between the detector index and the feature index in the MSK_DETFOO mask
    @staticmethod
    def get_detfoo_index_detector_list(maskFilename, shift):

        l_result = {}

        # Load the MSK_DETFOO gml file
        maskHandler = get_root_xml(maskFilename, deannotate=True)

        # Get the mask feature nodes
        # as : <eop:MaskFeature gml:id="detector_footprint-B02-10-2">
        xnodes = get_all_values(maskHandler, "//*[local-name(.) = 'MaskFeature']")

        # Two indices : the detector index and the feature index
        detId = 0

        # For each feature
        for i in range(len(xnodes)):
            feat = xnodes[i]
            # Get the attribute of the feature node
            # "detector_footprint-B02-10-2"
            attribute = list(feat.items())[0][1]
            element = attribute.split('-')

            # Get the detector index and the feature index
            detId = int(element[-2])
            featId = int(element[-1])

            l_result[featId+shift] = detId
        return l_result
