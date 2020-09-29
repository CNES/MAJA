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
from orchestrator.cots.otb.algorithms.otb_binary_threshold import binary_threshold
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaDataException, MajaExceptionPluginMuscate
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.common.constants import ReadL1Mode
from orchestrator.cots.otb.algorithms.otb_multiply_by_scalar import multiply_by_scalar
import orchestrator.common.xml_tools as xml_tools
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
from orchestrator.cots.otb.algorithms.otb_resample import resample
from orchestrator.cots.otb.algorithms.otb_resample import OtbResampleType
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.plugins.common.base.maja_l1_image_reader_base import L1ImageReaderBase

LOGGER = configure_logger(__name__)


class MuscateL1ImageFileReaderBase(L1ImageReaderBase):

    def __init__(self):
        super(MuscateL1ImageFileReaderBase, self).__init__()
        self._toa_scalar_list = []
        self._toa_sub_list = []
        self._l1toaimagelist = []
        self._l2toaimagelist = []
        self._sub_toa = ""
        self._l2satmasklist = []
        self._subsatimage = ""
        self._l2zonemasklist = []
        self._l2defectmasklist = []
        self._l2dfpimagelist = []
        self._l2edgmasklist = []
        self._l2piximagelist = []
        self._l2zoneimagelist = []
        self._satmasksublist = []
        self._zonemasksublist = []
        self._nodatamasksublist = []
        self._edgsubmask = ""
        self._claimage = ""
        self._vieimagelist = []
        self._meanZenithMap = {}
        self._meanAzimuthMap = {}
        self._sol1image = ""
        self._sol2image = ""
        # Store the apps
        self._toa_pipeline = OtbPipelineManager()
        self._l2_dfp_pipeline = OtbPipelineManager()
        self._l2_sat_pipeline = OtbPipelineManager()
        self._l2_detf_pipeline = OtbPipelineManager()
        self._l2_zone_pipeline = OtbPipelineManager()
        self._l1toa_pipeline = OtbPipelineManager()
        self._l2edg_pipeline = OtbPipelineManager()
        self._satmasksub_pipeline = OtbPipelineManager()
        self._satmask_pipeline = OtbPipelineManager()
        self._l2pix_pipeline = OtbPipelineManager()
        self._header_handler = None

    def generate_cla_image(self, realL1Nodata, working_dir):
        LOGGER.debug("MuscateL1ImageFileReader::GenerateCLAMask()")
        # CLA image pipeline connection
        # test if a CLA image file is available
        cla_node = xml_tools.get_only_value(
            self._header_handler.root,
            "//Data_List/Data[Data_Properties/NATURE='Cloud_Altitude_Grid']/Data_File_List/DATA_FILE",
            check=True)
        if cla_node is not None:
            self._claimage = self._header_handler.get_cla_image_filename()
        else:
            cla_filename = os.path.join(working_dir, "cla_constant_sub.tif")
            cla_image = constant_image(self._dem.ALC, realL1Nodata, cla_filename, write_output=True).getoutput()["out"]
            self._claimage = cla_image

    def generate_sol1_image(self, sol_h1, working_dir):
        LOGGER.debug("MuscateL1ImageFileReader::GenerateSOL1Image()")
        const_pi_18 = 0.01745329251994329577
        # SOL1 image pipeline connection
        # *********************************************************************************************************
        sol_filename = os.path.join(working_dir, "sol1.tif")
        mean_solar = self._header_handler.get_mean_solar_angles()
        l_sol1 = sol_h1 * math.tan(const_pi_18 * mean_solar[0]) * math.sin(const_pi_18 * mean_solar[1])
        l_sol2 = sol_h1 * math.tan(const_pi_18 * mean_solar[0]) * math.cos(const_pi_18 * mean_solar[1])
        LOGGER.debug("SOL1 parameters : ")
        LOGGER.debug(mean_solar)
        LOGGER.debug(l_sol1)
        LOGGER.debug(l_sol2)
        cla_app_1 = constant_image(self._dem.ALC, l_sol1, sol_filename, write_output=False)
        cla_app_2 = constant_image(self._dem.ALC, l_sol2, sol_filename, write_output=False)
        out_concatenate = os.path.join(working_dir, "sol1_concat.tif")
        param_concatenate = {"il": [cla_app_1.getoutput().get("out"), cla_app_2.getoutput().get("out")],
                             "out": out_concatenate}
        OtbAppHandler("ConcatenateDoubleImages", param_concatenate)
        self._sol1image = out_concatenate

    def generate_sol2_image(self, sol_h2, working_dir):
        raise MajaExceptionPluginMuscate(
            "MuscateL1ImageFileReader::GenerateSOL2Image. Internal error: the GenerateSOL2Image() method must be define in the sub class")

    def generate_vie_image(self, view_href, working_dir):
        # *********************************************************************************************************
        # VIE image pipeline connection
        # *********************************************************************************************************
        sol_filename = os.path.join(working_dir, "cla_constant_tmp.tif")
        mean_view = self._header_handler.get_mean_viewing_angles()
        l_vie1 = view_href * math.tan(mean_view[0]) * math.sin(mean_view[1])
        l_vie2 = view_href * math.tan(mean_view[0]) * math.cos(mean_view[1])
        vie_app_1 = constant_image(self._dem.ALC, l_vie1, sol_filename, write_output=False)
        vie_app_2 = constant_image(self._dem.ALC, l_vie2, sol_filename, write_output=False)
        out_concatenate = os.path.join(working_dir, "cla_constant.tif")
        param_concatenate = {"il": [vie_app_1.getoutput().get("out"), vie_app_2.getoutput().get("out")],
                             "out": out_concatenate}
        OtbAppHandler("ConcatenateDoubleImages", param_concatenate)
        l_BandsDefinitions = self._plugin.BandsDefinitions
        self._vieimagelist.append(out_concatenate)

    def generate_toa(self, listOfTOAImageFileNames, reflectanceMultiplicationValues, working):
        """
        :param listOfTOAImageFileNames: ListOfStrings
        :param reflectanceMultiplicationValues: ListOfDoubles
        :return:
        :rtype: string
        """
        if len(reflectanceMultiplicationValues) != len(listOfTOAImageFileNames):
            raise MajaDataException("Not the same number of band images and coefficients : " + str(
                len(reflectanceMultiplicationValues)) + " : " + str(len(listOfTOAImageFileNames)))
        # Init the projRef
        l_ProjectionRef = ""
        # =======> GENERATE TOA CACHING
        # ---------------------------------------------------------------
        # Get the number of band with the number of TOA image files set in the input product directory
        l_NbBand = len(listOfTOAImageFileNames)  # int

        # Convert the input jp2 images in tif
        for i in range(l_NbBand):
            toaFilename = listOfTOAImageFileNames[i]
            LOGGER.debug("Caching the <%s> image filename...", toaFilename)
            LOGGER.debug("Reflectance quantification value: %s", reflectanceMultiplicationValues[i])
            # initialize the TOA reader
            l_ImageFilename = os.path.join(working, "toaconverted_" + str(i) + ".tif")
            # extract_roi(toaFilename,[0],l_ImageFilename,working)
            app = multiply_by_scalar(
                toaFilename,
                reflectanceMultiplicationValues[i],
                output_image=l_ImageFilename,
                write_output=True)
            self._toa_scalar_list.append(app.getoutput()["out"])

        LOGGER.debug("Caching TOA images done ...")
        return l_ProjectionRef

    def generate_l2_toa_images(self, working_dir):
        """

        :return:
        """
        l_BandsDefinitions = self._plugin.BandsDefinitions
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution  # ListOfStrings
        l_NbL2Res = len(l_ListOfL2Resolution)

        for l2res in range(l_NbL2Res):
            # Current resolution: "R1" or "R2"
            curRes = l_ListOfL2Resolution[l2res]
            dtm = self._dem.ALTList[l2res]
            # Get the list of band of the current resolution
            listOfL2Bands = l_BandsDefinitions.get_list_of_l2_band_code(curRes)
            nbBand = len(listOfL2Bands)
            list_of_image = []
            # For each band of the current resolution
            for l_StrBandIdL2 in listOfL2Bands:
                # Get the L1 band index associated to the L2 band code
                l1BandIdx = l_BandsDefinitions.get_band_id_in_l1(l_StrBandIdL2)
                # Generate the list of L2 TOA images per resolution
                toa_l2_image = os.path.join(working_dir, "aot_l2_{}.tif".format(l_StrBandIdL2))
                app = resample(self._toa_scalar_list[l1BandIdx], dtm, toa_l2_image, OtbResampleType.LINEAR_WITH_RADIUS,
                               write_output=False)
                self._toa_pipeline.add_otb_app(app)
                list_of_image.append(app.getoutput()["out"])
            out_concatenate = os.path.join(working_dir, "L2TOAImageListVector_" + curRes + ".tif")
            param_concatenate = {"il": list_of_image,
                                 "out": out_concatenate}
            concat_app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate,write_output=False)
            self._toa_pipeline.add_otb_app(concat_app)
            self._l2toaimagelist.append(concat_app.getoutput().get("out"))

    def generate_l1_toa_images(self, working_dir):
        """

        :return:
        """

        out_concatenate = os.path.join(working_dir, "L1TOAVector.tif")
        param_concatenate = {"il": self._toa_scalar_list,
                             "out": out_concatenate}
        app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate, write_output=False)
        self._l1toa_pipeline.add_otb_app(app)
        self._l1toaimagelist.append(app.getoutput()["out"])

    def generate_toa_sub_images(self, working):
        dtm_coarse = self._dem.ALC

        toa_sub_mini_pipe = OtbPipelineManager()
        # For each band of the input product
        for i, toa in enumerate(self._toa_scalar_list):
            # undersampling at L2CoarseResolution
            toa_sub_image = os.path.join(working, "aot_sub_{}.tif".format(i))
            app = resample(toa, dtm_coarse, toa_sub_image, OtbResampleType.LINEAR_WITH_RADIUS,
                           write_output=False)
            self._toa_sub_list.append(app.getoutput()["out"])
            toa_sub_mini_pipe.add_otb_app(app)
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
        toa_sub_mini_pipe.free_otb_app()

    def generate_mask_rasters(self, p_ListOfTOABandCode, working):
        # Generate mask rasters by rasterizing the gml mask per L2 resolution per band
        LOGGER.info("Starting Muscate GenerateMaskRasters")

        l_BandsDefinitions = self._plugin.BandsDefinitions
        l2Area = None
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution  # ListOfStrings
        l_NbL2Res = len(l_ListOfL2Resolution)
        l_ListOfL1Resolution = l_BandsDefinitions.ListOfL1Resolution  # ListOfStrings
        l_NbL1Res = len(l_ListOfL1Resolution)
        dtm_coarse = self._dem.ALC

        # Set a constant image if the gml masks are empty
        tmp_constant_filename = os.path.join(working, "Masks_const.tif")

        # initialize the L2 Elements
        for l2res in range(l_NbL2Res):
            self._l2defectmasklist.append([])
            self._l2dfpimagelist.append(None)

        # Init the coarse elements
        for coarseband in p_ListOfTOABandCode:
            self._l2satmasklist.append(None)
        for l1res in range(l_NbL1Res):
            self._satmasksublist.append(None)
        # Test if the plugin has PIX and SAT ?
        has_pix_masks = False
        defectivPixFileNames = []
        pix_node = xml_tools.get_only_value(
            self._header_handler.root,
            "//Mask_List/Mask/Mask_Properties/NATURE[.='Aberrant_Pixels']",
            check=True)
        if pix_node is not None:
            has_pix_masks = True
            defectivPixFileNames = self._header_handler.get_list_of_pix_mask_filenames()
        has_sat_masks = False
        satPixFileNames = []
        sat_node = xml_tools.get_only_value(
            self._header_handler.root,
            "//Mask_List/Mask/Mask_Properties/NATURE[.='Saturation']",
            check=True)
        if sat_node is not None:
            has_sat_masks = True
            satPixFileNames = self._header_handler.get_list_of_l1_sat_image_filenames()

        # Loop on L1Res
        for l1res in range(l_NbL1Res):
            # Current resolution: "R1" or "R2" or "R3"
            curL1Res = l_ListOfL1Resolution[l1res]
            # Create constant mask if it will be necessary at a time
            tmp_constant_coarse_app = None
            tmp_constant_L2_app = None
            if not has_sat_masks or not has_pix_masks:
                tmp_constant_coarse_filename = os.path.join(working, "Masks_const_.tif")
                tmp_constant_coarse_app = constant_image(
                    dtm_coarse, 0, tmp_constant_coarse_filename, write_output=False)
                if curL1Res in l_ListOfL2Resolution:
                    tmp_constant_L2_filename = os.path.join(working, "Masks_const_.tif")
                    tmp_constant_L2_app = constant_image(self._dem.ALTList[l1res], 0, tmp_constant_L2_filename + ":uint8",
                                                         write_output=False)
                    self._l2_sat_pipeline.add_otb_app(tmp_constant_L2_app)

            # Get the list of band of the current resolution
            listOfL1Bands = l_BandsDefinitions.get_list_of_l1_band_code(curL1Res)
            nbL1Bands = len(listOfL1Bands)
            # For each band of the current resolution
            l_l2zone_pipeline = OtbPipelineManager()
            firstBandIdx = l_BandsDefinitions.get_band_id_in_l1(listOfL1Bands[0])

            if has_sat_masks:
                l_strSatFileNameRef = satPixFileNames[firstBandIdx]
                l_isSatSameFilesForBands = True

                for l_BandIdxL1 in range(len(listOfL1Bands)):
                    # Get the L1 band index associated to the L2 band code
                    l_StrBandIdL1 = listOfL1Bands[l_BandIdxL1]
                    l1BandIdx = self._header_handler.get_index_of_band_code(l_StrBandIdL1)
                    # Filenameverify
                    LOGGER.debug(l_StrBandIdL1 + " : " + str(l1BandIdx) + " : " + satPixFileNames[l1BandIdx])
                    if l_strSatFileNameRef != satPixFileNames[l1BandIdx]:
                        l_isSatSameFilesForBands = False
                LOGGER.debug("l_isSatSameFilesForBands = " + str(l_isSatSameFilesForBands))

                # SAT Mask generation
                if l_isSatSameFilesForBands:
                    # Sat Masks generation
                    l_StrBandIdL1 = listOfL1Bands[0]
                    l1BandIdx = self._header_handler.get_index_of_band_code(l_StrBandIdL1)
                    l_SATFFilename = satPixFileNames[l1BandIdx]
                    sat_mask = os.path.join(working, "L1_SAT_Masks_{}.tif".format(curL1Res))
                    param_bintovec_dfp = {"im": l_SATFFilename,
                                          "out": sat_mask,
                                          "nbcomp": nbL1Bands
                                          }
                    sat_mask_app = OtbAppHandler("BinaryToVector", param_bintovec_dfp, write_output=False)
                    self._satmask_pipeline.add_otb_app(sat_mask_app)

                    if curL1Res in l_ListOfL2Resolution:
                        tmp_sat_l2_resample = os.path.join(working, "tmp_sat_resample_{}.tif".format(l1res))

                        if l_BandsDefinitions.get_list_of_l1_band_code(curL1Res) != \
                                l_BandsDefinitions.get_list_of_l2_band_code(curL1Res):
                            tmp_l2_band_idx_list = []
                            #Create the list of index to build the current res sat mask
                            for l2band in l_BandsDefinitions.get_list_of_l2_band_code(curL1Res):
                                l2BandIdxInHeader = self._header_handler.get_index_of_band_code(l2band)
                                tmp_l2_band_idx_list.append(self._header_handler.get_l1_sat_image_index(l2BandIdxInHeader) - 1)
                            tmp_satmask_roi = os.path.join(working, "tmp_sat_extract_{}.tif".format(l1res))
                            tmp_extract_sat_toi_app = extract_roi(
                                 sat_mask_app.getoutput()["out"], tmp_l2_band_idx_list,
                                    tmp_satmask_roi, write_output=False)
                            self._satmask_pipeline.add_otb_app(tmp_extract_sat_toi_app)
                            resamp_l2_app = resample(
                                tmp_extract_sat_toi_app.getoutput()["out"],
                                self._dem.ALTList[l1res],
                                tmp_sat_l2_resample,
                                OtbResampleType.LINEAR_WITH_RADIUS,
                                threshold=0.5,
                                padradius=4,write_output=False)
                            self._satmask_pipeline.add_otb_app(resamp_l2_app)
                            self._l2satmasklist[l_ListOfL2Resolution.index(
                                curL1Res)] = resamp_l2_app.getoutput().get("out")
                        else:
                            resamp_l2_app = resample(sat_mask_app.getoutput()["out"], self._dem.ALTList[l1res],
                                                     tmp_sat_l2_resample+ ":uint8",
                                                     OtbResampleType.LINEAR_WITH_RADIUS,
                                                     threshold=0.25, padradius=4,write_output=False)
                            self._satmask_pipeline.add_otb_app(resamp_l2_app)
                            self._l2satmasklist[l_ListOfL2Resolution.index(curL1Res)] = resamp_l2_app.getoutput()["out"]

                    tmp_l1_band_idx_list = []
                    for l1band in listOfL1Bands:
                        l1BandIdxInHeader = self._header_handler.get_index_of_band_code(l1band)
                        tmp_l1_band_idx_list.append(self._header_handler.get_l1_sat_image_index(l1BandIdxInHeader) - 1)
                    tmp_sat_roi = os.path.join(working, "tmp_l1_extract_roi_sat_{}.tif".format(l1res))
                    tmp_sat_roi_app = extract_roi(
                        sat_mask_app.getoutput()["out"], tmp_l1_band_idx_list, tmp_sat_roi, write_output=False)
                    l_sat_subthresholdvalue = self._GIPPL2COMMHandler.get_value_f("SaturationThresholdSub")
                    tmp_sat_resample = os.path.join(working, "tmp_extract_roi_sat_resample_{}.tif".format(l1res))
                    app_resample = resample(tmp_sat_roi_app.getoutput().get("out"), dtm_coarse, tmp_sat_resample,
                                            OtbResampleType.LINEAR_WITH_RADIUS, threshold=l_sat_subthresholdvalue,
                                            padradius=4,write_output=True)
                    self._satmasksublist[l1res] = app_resample.getoutput()["out"]

                else:
                    raise MajaExceptionPluginMuscate("Product format not supported : not the same file for band on SAT")
            else:
                # No sat available then put constants masks in outout
                if curL1Res in l_ListOfL2Resolution:
                    self._l2satmasklist[l_ListOfL2Resolution.index(
                        curL1Res)] = tmp_constant_L2_app.getoutput().get("out")
                for l1band in listOfL1Bands:
                    l1BandIdx = l_BandsDefinitions.get_band_id_in_l1(l1band)
                    self._satmasksublist[l1BandIdx] = tmp_constant_coarse_app.getoutput().get("out")

            # Verify if we can optimize the work if its the same file for all bands of
            # the resolution and if the bands are in correct bit order
            if has_pix_masks:
                l_strDefectFileNameRef = defectivPixFileNames[firstBandIdx]
                l_isDefectSameFilesForBands = True
                for l_BandIdxL1 in range(len(listOfL1Bands)):
                    # Get the L1 band index associated to the L2 band code
                    l_StrBandIdL1 = listOfL1Bands[l_BandIdxL1]
                    l1BandIdx = self._header_handler.get_index_of_band_code(l_StrBandIdL1)
                    # Filenameverify
                    if l_strDefectFileNameRef != defectivPixFileNames[l1BandIdx]:
                        l_isDefectSameFilesForBands = False
                LOGGER.debug("l_isDefectSameFilesForBands = " + str(l_isDefectSameFilesForBands))

                # Defective PIX (only in L2 resolution)
                if l_isDefectSameFilesForBands:
                    if curL1Res in l_ListOfL2Resolution:
                        l_StrBandIdL1 = listOfL1Bands[0]
                        l1BandIdx = self._header_handler.get_index_of_band_code(l_StrBandIdL1)

                        self._l2piximagelist.append(defectivPixFileNames[l1BandIdx])

                        dfp_mask = os.path.join(working, "L1_DFP_Masks_{}.tif".format(curL1Res))
                        param_bintovec_dfp = {"im": defectivPixFileNames[l1BandIdx],
                                              "out": dfp_mask,
                                              "nbcomp": nbL1Bands
                                              }

                        dfp_mask_app = OtbAppHandler("BinaryToVector", param_bintovec_dfp, write_output=False)
                        self._l2_dfp_pipeline.add_otb_app(dfp_mask_app)
                        tmp_dfp_l2_resample = os.path.join(working, "tmp_dfp_resample_{}.tif".format(l1res))
                        resamp_l2_dfp_app = resample(dfp_mask_app.getoutput().get("out"), self._dem.ALTList[l1res],
                                                     tmp_dfp_l2_resample + ":uint8",
                                                     OtbResampleType.LINEAR_WITH_RADIUS, threshold=0.25, padradius=4,
                                                     write_output=False)
                        self._l2dfpimagelist[l_ListOfL2Resolution.index(
                            curL1Res)] = resamp_l2_dfp_app.getoutput().get("out")
                        self._l2_dfp_pipeline.add_otb_app(resamp_l2_dfp_app)
                else:
                    raise MajaExceptionPluginMuscate("Product format not supported : not the same file for band on PIX")
            else:
                if curL1Res in l_ListOfL2Resolution:
                    self._l2dfpimagelist[l_ListOfL2Resolution.index(
                        curL1Res)] = tmp_constant_L2_app.getoutput().get("out")
                    self._l2piximagelist.append(tmp_constant_L2_app.getoutput().get("out"))
        # end res loop

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

    def generate_edg_images(self, working_dir):
        dtm_coarse = self._dem.ALC
        # One band equal threshold
        out_edg = os.path.join(working_dir, "tmp_EDG_oneBandEqual.tif")
        param_edg = {"im": self._l1toaimagelist[0],
                     "thresholdvalue": 0,
                     "equalvalue": 255,
                     "outsidevalue": 0,
                     "out": out_edg + ":uint8"
                     }
        onebandequal_app = OtbAppHandler("OneBandEqualThreshold", param_edg,write_output=False)

        # Resample to coarse
        LOGGER.debug("Start IPEDGSub.")
        tmp_edg_sub_resample = os.path.join(working_dir, "tmp_edg_sub.tif")
        edg_sub_resample_app = resample(onebandequal_app.getoutput().get("out"), dtm_coarse, tmp_edg_sub_resample,
                                        OtbResampleType.LINEAR_WITH_RADIUS, threshold=0.0, padradius=4.0,write_output=True)
        # Threshold the output
        #out_sub_edg = os.path.join(working_dir, "tmp_edg_sub_oneBandEqual.tif")
        #param_sub_edg = {"im": edg_sub_resample_app.getoutput().get("out"),
        #                 "thresholdvalue": 0,
        #                 "equalvalue": 0,
        #                 "outsidevalue": 1,
        #                 "out": out_sub_edg + ":uint8"
        #                 }
        #onebandequal_sub_app = OtbAppHandler("OneBandEqualThreshold", param_sub_edg)
        # Put in internal data
        self._edgsubmask = edg_sub_resample_app.getoutput().get("out")
        del onebandequal_app
        del edg_sub_resample_app
        LOGGER.debug("Start IPEDGSub done.")

        # *******************************************************************************************************
        # L2EDG pipeline connection
        # *******************************************************************************************************
        LOGGER.debug("Start L2EDG ...")

        l_BandsDefinitions = self._plugin.BandsDefinitions
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution  # ListOfStrings
        # At L2 resolution
        l_NbL2Res = len(l_ListOfL2Resolution)
        # Set 1000 to edge pixels to identify the pixel contaminated by an edge pixel after resampling
        #out_thresh = os.path.join(working_dir, "EDGThreshL2.tif")
        #m_L2EDGThresholdApp = binary_threshold(self._edgsubmask,
        #                                         lower_threshold=0,
        #                                         inside_value=1000,
        #                                         outside_value=0,
        #                                         output_image=out_thresh + ":uint8",
        #                                         write_output=False)# //l_ThresholdImageFilter
        #self._l2edg_pipeline.add_otb_app(m_L2EDGThresholdApp)
        for r in range(l_NbL2Res):
            res_str = l_ListOfL2Resolution[r]
            # ExpandFilterPointer => PadAndResampleImageFilter => app ressampling
            out_roi = os.path.join(working_dir, "IPEDGRealL2_{}.tif".format(res_str))
            roi_app = extract_roi(self._edgsubmask, [0], out_roi,write_output=False)
            self._l2edg_pipeline.add_otb_app(roi_app)
            out_ressampling = os.path.join(working_dir, "IPEDGRealL2_{}.tif".format(res_str))
            resamp_app = resample(roi_app.getoutput().get("out"),
                                  self._dem.ALTList[r], out_ressampling,
                                  OtbResampleType.LINEAR,threshold=0.001,write_output=False)
            self._l2edg_pipeline.add_otb_app(resamp_app)
            # Set Threshold value to one because the expand filter interpolates values set to 0
            # or 1000 in the first threshold and adds systematically CONST_EPSILON to the output value.
            #m_L2EDGThresholdImage2_out = os.path.join(working_dir, "IPEDGMaskL2_{}.tif".format(res_str))
            #m_L2EDGThresholdApp2 = binary_threshold(resamp_app.getoutput().get("out"),
            #                                          lower_threshold=1,
            #                                          inside_value=1,
            #                                          outside_value=0,
            #                                          output_image=m_L2EDGThresholdImage2_out + ":uint8",
            #                                        write_output=False)
            self._l2edgmasklist.append(resamp_app.getoutput().get("out"))
            #self._l2edg_pipeline.add_otb_app(m_L2EDGThresholdApp2)

    def read(self, product_info, app_handler, l2comm, dem, pReadL1Mode):
        """product_info,plugin, l2comm,mode

        :param product_info: L1ImageInformationsBase
        :param pReadL1Mode: ReadL1ModeType
        :return:
        """
        LOGGER.info("Start Muscate L1 ImageFileReader ...")
        product_filename = product_info.HeaderFilename
        LOGGER.info("Start Muscate L1 ImageFileReader with filename : " + product_filename)
        working_dir = app_handler.get_directory_manager().get_temporary_directory("L1Read_",
                                                                                  do_always_remove=True)

        self._plugin.initialize(app_handler)
        self._GIPPL2COMMHandler = l2comm
        LOGGER.info("Start Muscate L1 ImageFileReader ...")
        self._ReadL1Mode = pReadL1Mode
        self._dem = dem
        self._header_handler = product_info.HeaderHandler
        # --------------------------------------
        # Check the extension of th efile. For Muscate, must be .XML (in upper case)
        IsValidSatellite = (self._header_handler is not None)
        if not IsValidSatellite:
            raise MajaExceptionPluginMuscate(
                "The file <{}> is not a valid Muscate L1 product.".format(product_filename))
        # Get satellite name
        self.m_Satellite = product_info.Satellite

        # --------------------------------------
        # Load the header tile file in the xml tile handler to read tile information
        xmlTileFilename = product_info.xmlTileFilename
        l_BandsDefinitions = self._plugin.BandsDefinitions  # BandsDefinitions

        # TOA & masks Reader connection
        l_ListOfTOAImageFileNamesInHeader = self._header_handler.get_list_of_toa_image_filenames()
        l_ListOfTOAImageFileNames = []
        l_ListOfTOABandCode = l_BandsDefinitions.get_list_of_band_code_in_l2_coarse_sorted_by_index()  # ListOfStrings
        for band in l_ListOfTOABandCode:
            LOGGER.debug(band)
            l_ListOfTOAImageFileNames.append(
                l_ListOfTOAImageFileNamesInHeader[self._header_handler.get_index_of_band_code(band)])
        l_ListOfL1Resolution = l_BandsDefinitions.ListOfL1Resolution  # ListOfStrings
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution  # ListOfStrings

        l_L1NoData = product_info.L1NoData
        l_ReflectanceQuantificationValue = product_info.ReflectanceQuantification
        l_RealL1NoData = l_L1NoData * l_ReflectanceQuantificationValue  # RealNoDataType

        # =======> GENERATE TOA
        # Get calibration coeffs if activated
        l_reflectanceMultiplicationValues = []  # ListOfDoubles
        if xml_tools.as_bool(l2comm.get_value("CalAdjustOption")):
            l_factors = xml_tools.as_float_list(l2comm.get_value("CalAdjustFactor"))
            if len(l_factors) != len(l_ListOfTOABandCode):
                raise MajaDataException(
                    "Not the same number of Calibration coeffs than L1 bands ( {}, {} )".format(len(l_factors), len(
                        l_ListOfTOABandCode)))
            for i in range(len(l_ListOfTOABandCode)):
                l_reflectanceMultiplicationValues.append(l_ReflectanceQuantificationValue * l_factors[i])
        else:
            for i in range(len(l_ListOfTOABandCode)):
                l_reflectanceMultiplicationValues.append(l_ReflectanceQuantificationValue)

        l_ProjectionRef = self._dem.ProjRef
        self.generate_toa(l_ListOfTOAImageFileNames, l_reflectanceMultiplicationValues, working_dir)  # string
        if pReadL1Mode == ReadL1Mode.READ_L1_MODE_FOR_ALGORITHMS:
            # --------------------------------------
            # Get information of areas (footprint) of the product (Origin, Spacing and Size for L2 and L2Coarse resolution)
            l_L2Dems = dem.ALTList
            l_CoarseDem = dem.ALC

            # =======> GENERATE TOA SUB IMAGES AT L2 COARSE RESOLUTION
            LOGGER.debug("Start SubSampling ...")
            self.generate_toa_sub_images(working_dir)
            LOGGER.debug("Start SubSampling done.")
            #  the gml mask per L2 resolution per band
            self.generate_mask_rasters(l_ListOfTOABandCode, working_dir)
            # IPEDGSub and L2EDG pipeline connection
            # L2 TOA image pipeline connection
            # =======> GENERATE L2 TOA IMAGES
            self.generate_l2_toa_images(working_dir)
            # =======> GENERATE L1 TOA IMAGES
            self.generate_l1_toa_images(working_dir)
            # =======> GENERATE EDG IMAGES
            self.generate_edg_images(working_dir)
            # IPSAT Sub and L2SAT image pipeline connection
            # =======> GENERATE SAT IMAGES
            self.generate_sat_images(working_dir)
            # CLA image pipeline connection
            # *********************************************************************************************************
            self.generate_cla_image(l_RealL1NoData, working_dir)
            # SOL1 image pipeline connection
            # *********************************************************************************************************
            sol_h1 = self._plugin.ConfigUserCamera.get_Algorithms().get_GRID_Reference_Altitudes().get_SOLH1()
            LOGGER.debug("sol_H1 : ")
            LOGGER.debug(sol_h1)
            self.generate_sol1_image(sol_h1, working_dir)
            # SOL2 image pipeline connection
            # *********************************************************************************************************
            if self._plugin.GRIDReferenceAltitudesSOL2GridAvailable:
                sol_h2 = self._plugin.ConfigUserCamera.get_Algorithms().get_GRID_Reference_Altitudes().get_SOLH2()
                self.generate_sol2_image(sol_h2, working_dir)
            # VIE image pipeline connection
            # *********************************************************************************************************
            vie_href = self._plugin.ConfigUserCamera.get_Algorithms().get_GRID_Reference_Altitudes().get_VIEHRef()
            self.generate_vie_image(vie_href, working_dir)

            # Fill the datas
            self.dict_of_vals[
                "IPEDGSubOutput"] = self._edgsubmask
            self.dict_of_vals[
                "SOL1Image"] = self._sol1image
            self.dict_of_vals[
                "SOL2Image"] = self._sol2image

            if len(self._vieimagelist) > 2:
                l_DTMBandCode = xml_tools.as_string_list(l2comm.get_value("DTMViewingDirectionBandCode"))[0]
                l_DTMBandIdx = self._plugin.BandsDefinitions.get_band_id_in_l2_coarse(l_DTMBandCode)
                LOGGER.info("DTMBandCode= " + l_DTMBandCode)

                self.dict_of_vals[
                    "DTMVIEImage"] = self._vieimagelist[l_DTMBandIdx]
            else:
                self.dict_of_vals[
                    "DTMVIEImage"] = self._vieimagelist[0]
            self.dict_of_vals[
                "IPTOASubOutput"] = self._sub_toa
            self.dict_of_vals[
                "L2TOAImageList"] = self._l2toaimagelist
            self.dict_of_vals["ViewingZenithMeanMap"] = self._meanZenithMap
            self.dict_of_vals["ViewingAzimuthMeanMap"] = self._meanAzimuthMap
            self.dict_of_vals["CLAImage"] = self._claimage
            self.dict_of_vals["IPSATSubOutput"] = self._subsatimage
            if len(self._vieimagelist) > 2:
                l_CLDBandCode = l2comm.get_value("CLDViewingDirectionBandCode")
                l_CLDBandIdx = self._plugin.BandsDefinitions.get_band_id_in_l2_coarse(l_CLDBandCode)
                self.dict_of_vals["ShadowVIEImage"] = self._vieimagelist[l_CLDBandIdx]
            elif len(self._vieimagelist) > 1:
                self.dict_of_vals["ShadowVIEImage"] = self._vieimagelist[1]
            else:
                self.dict_of_vals["ShadowVIEImage"] = self._vieimagelist[0]
            if self._plugin.CirrusMasking:
                l_CirrusBandCode = l2comm.get_value("CirrusBandCode")
                l_CirrusBandIdx = self._plugin.BandsDefinitions.get_band_id_in_l2_coarse(l_CirrusBandCode)
                self.dict_of_vals["L1TOACirrusImage"] = self._toa_scalar_list[l_CirrusBandIdx]
            self.dict_of_vals["L2EDGOutputList"] = self._l2edgmasklist
            self.dict_of_vals["L2SATImageList"] = self._l2satmasklist
            self.dict_of_vals["L2PIXImageList"] = self._l2dfpimagelist
