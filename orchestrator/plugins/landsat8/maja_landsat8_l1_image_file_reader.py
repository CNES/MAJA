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
#                        o  o  o o    o      o o    o     |||)<
#                        o     o oooooo      o oooooo      / \
#                        o     o o    o o    o o    o      `-^
#                        o     o o    o  oooo  o    o
#
###################################################################################################

orchestrator.plugins.landsat8.maja_landsat8_l1_image_file_reader -- shortdesc

orchestrator.plugins.landsat8.maja_landsat8_l1_image_file_reader is a description

It defines classes_and_methods

###################################################################################################

:organization: CS SI
:copyright: 2017 CNES. All rights reserved.
:license: see LICENSE file
:created: 12 Jan 2018

###################################################################################################
"""
import os
import math
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.plugins.landsat8.maja_landsat8_plugin import MajaLandsat8Plugin
from orchestrator.common.constants import ReadL1Mode
import orchestrator.common.xml_tools as xml_tools
from orchestrator.cots.otb.algorithms.otb_band_math import band_math
from orchestrator.cots.otb.algorithms.otb_apply_mask import apply_mask
from orchestrator.cots.otb.algorithms.otb_resample import resample
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.cots.otb.algorithms.otb_resample import OtbResampleType
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
from orchestrator.plugins.common.base.maja_l1_image_reader_base import L1ImageReaderBase
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
LOGGER = configure_logger(__name__)


class Landsat8L1ImageFileReader(L1ImageReaderBase):
    def __init__(self):
        super(Landsat8L1ImageFileReader, self).__init__()
        self._Satellite = "LANDSAT_8"
        self._plugin = MajaLandsat8Plugin()
        self._header_handler = None
        self._toascalarlist = []
        self._satmasklist = []
        self._l1toaimagelist = []
        self._l2piximagelist = []
        self._l2toaimagelist = []
        self._l2satmasklist = []
        self._qbmask = ""
        self._edgsubmask = ""
        self._subtoaimage = ""
        self._subsatimage = ""
        self._claimage = ""
        self._meanZenithMap = {}
        self._meanAzimuthMap = {}
        self._sol1image = ""
        self._vieimage = []
        self._l2edgmasklist = []
        self._toasublist = []
        self._satsublist = []
        self._pix_pipeline = OtbPipelineManager()
        self._qb_pipeline = OtbPipelineManager()
        self._toa_pipeline = OtbPipelineManager()
        self._toa_sub_pipeline = OtbPipelineManager()
        self._l2toa_pipeline = OtbPipelineManager()
        self._edg_pipeline = OtbPipelineManager()
        self._sat_pipeline = OtbPipelineManager()
        self._sat_sub_pipeline = OtbPipelineManager()
        self._sol1_pipeline = OtbPipelineManager()
        self._vie_pipeline = OtbPipelineManager()
        self._reall1nodata = -10
        self._reall2nodata = 0
        self._toathresholdminvalue = 0

    def can_read(self, plugin_name):
        return plugin_name == "LANDSAT8"

    def generate_pix_mask(self, working_dir):
        LOGGER.debug("Landsat8L1ImageFileReader::GeneratePIXMask()")
        l_BandsDefinitions = self._plugin.BandsDefinitions
        l_ListOfL1Resolution = l_BandsDefinitions.ListOfL1Resolution  # ListOfStrings

        for l1res in range(0,len(l_ListOfL1Resolution)):
            curL1Res = l_ListOfL1Resolution[l1res]
            dtm = self._dem.ALTList[l1res]
            tmp_constant_filename = os.path.join(working_dir, "PIX_Masks_const_{}.tif".format(curL1Res))
            app = constant_image(dtm, 0, tmp_constant_filename + ":uint8", write_output=False)
            self._l2piximagelist.append(app.getoutput().get("out"))
            self._pix_pipeline.add_otb_app(app)

    def generate_qb_mask(self, qb_filename, working_dir):
        img_binaryqb = os.path.join(working_dir, "NoDataFromQBFilter.tif")
        param_bintovec_qb = {"im": qb_filename,
                             "out": img_binaryqb + ":uint8",
                             "nbcomp": 16
                             }
        app = OtbAppHandler("BinaryToVector", param_bintovec_qb, write_output=False)
        self._toa_pipeline.add_otb_app(app)
        self._qbmask = app.getoutput().get("out")
        self._qb_pipeline.add_otb_app(app)

    def generate_l1_toa_image(self, l2comm, factor, solzenith, L1ResolList, working_dir):
        for l1res in range(len(L1ResolList)):
            curL1Res = L1ResolList[l1res]
            l_ListOfL1BandCode = self._plugin.BandsDefinitions.get_list_of_l1_band_code(curL1Res)

            for band in l_ListOfL1BandCode:
                curMULT = self._header_handler.get_ref_mutl_factor_from_band_code(band)
                curADD = self._header_handler.get_ref_add_factor_from_band_code(band)
                expression_bandmath = "(im2b1 || (im1b1-0.00001)<0)?"+str(self._reall1nodata) + ":"
                if xml_tools.as_bool(l2comm.get_value("CalAdjustOption")):
                    expression_bandmath = expression_bandmath + str(factor) + "*"
                expression_bandmath = expression_bandmath + "(im1b1*" + str(curMULT) + "+" + str(curADD) + ")/cos(" + str(solzenith) + "*" + str(math.pi/180)+")"

                img_toaconverter = os.path.join(working_dir, "TOAConverter_{}.tif".format(band))
                app_bandmath = band_math(
                    [self._header_handler.get_toa_image_filename_from_band_code(band), self._qbmask],
                    expression_bandmath,
                    img_toaconverter,write_output=False
                )
                self._toascalarlist.append(app_bandmath.getoutput().get("out"))
                self._toa_pipeline.add_otb_app(app_bandmath)
            img_concatenate_toa = os.path.join(working_dir, "l1toa_{}.tif".format(l1res))
            param_concatenate_toa = {"il": self._toascalarlist,
                                 "out": img_concatenate_toa}
            app_concatenate_toa = OtbAppHandler("ConcatenateDoubleImages", param_concatenate_toa,write_output=True)
            self._l1toaimagelist.append(app_concatenate_toa.getoutput()["out"])
            self._qb_pipeline.free_otb_app()
            self._toa_pipeline.free_otb_app()

    def generate_toa_sub_image(self, working):
        dtm_coarse = self._dem.ALC
        # For each res of the input L1 product
        for i, toa in enumerate(self._l1toaimagelist):
            # undersampling at L2CoarseResolution
            toa_sub_image = os.path.join(working, "aot_sub_{}.tif".format(i))
            app_resample = resample(toa, dtm_coarse, toa_sub_image, OtbResampleType.LINEAR_WITH_RADIUS,
                           write_output=False)
            self._toa_sub_pipeline.add_otb_app(app_resample)
            self._toasublist.append(app_resample.getoutput().get("out"))
        # end res loop
        #concat
        toa_sub_image = os.path.join(working, "aot_sub.tif")
        param_concatenate = {"il": self._toasublist,
                             "out": toa_sub_image
                             }
        app_concatenate = OtbAppHandler("ConcatenateDoubleImages", param_concatenate, write_output=False)
        self._toa_sub_pipeline.add_otb_app(app_concatenate)
        #apply edg
        #invert EDG for masking
        out_edgsub_threshold = os.path.join(working, "edgsubthreshold.tif")
        param_edgsub_threshold = {"im": self._edgsubmask,
                                  "thresholdvalue": self._toathresholdminvalue,
                                  "equalvalue": 1,
                                  "outsidevalue": 0,
                                  "out": out_edgsub_threshold + ":uint8"
                                  }
        app_edgsub_threshold = OtbAppHandler("OneBandEqualThreshold", param_edgsub_threshold, write_output=False)
        self._toa_sub_pipeline.add_otb_app(app_edgsub_threshold)
        #apply mask to sub toa
        tmp_l2subtoa = os.path.join(working, "toasub.tif")
        app_l2subtoa = apply_mask(app_concatenate.getoutput().get("out"),
                                  app_edgsub_threshold.getoutput().get("out"),
                                  self._reall1nodata, tmp_l2subtoa, write_output=True)
        self._subtoaimage = app_l2subtoa.getoutput().get("out")
        self._toa_sub_pipeline.free_otb_app()

    def generate_l2_toa_images(self, l_ListOfL2Resolution, working_dir):

        for l1res in range(len(l_ListOfL2Resolution)):
            curL1Res = l_ListOfL2Resolution[l1res]
            dtm = self._dem.ALTList[l1res]
            l_ListOfL2BandCode = self._plugin.BandsDefinitions.get_list_of_l2_band_code(curL1Res)
            LOGGER.debug("Read the nb bands" + str(len(l_ListOfL2BandCode)) + " of the TOA images...")
            l_l1bandidx = [self._plugin.BandsDefinitions.get_band_id_in_l1(b) for b in l_ListOfL2BandCode]
            tmp_toa_resample = os.path.join(working_dir, "tmp_toa_resample.tif")
            app_toa_resample = resample(self._l1toaimagelist[l1res], dtm, tmp_toa_resample, OtbResampleType.LINEAR_WITH_RADIUS,
                           write_output=False)
            self._l2toa_pipeline.add_otb_app(app_toa_resample)
            # L2TOA has less band than L1 so extract
            tmp_toa_roi = os.path.join(working_dir, "L2TOAImageList_{}.tif".format(curL1Res))
            l2toa_roi_app = extract_roi(app_toa_resample.getoutput().get("out"),
                                        l_l1bandidx,
                                        tmp_toa_roi, write_output=False)
            self._l2toa_pipeline.add_otb_app(l2toa_roi_app)
            self._l2toaimagelist.append(l2toa_roi_app.getoutput().get("out"))

    def generate_edg_images(self, working_dir):
        dtm_coarse = self._dem.ALC
        # One band equal threshold
        out_edg = os.path.join(working_dir, "tmp_EDG_oneBandEqual.tif")
        param_edg = {"im": self._l1toaimagelist[0],
                     "thresholdvalue": self._reall1nodata,
                     "equalvalue": 255,
                     "outsidevalue": 0,
                     "out": out_edg + ":double"
                     }
        onebandequal_app = OtbAppHandler("OneBandEqualThreshold", param_edg,write_output=False)
        self._edg_pipeline.add_otb_app(onebandequal_app)
        # Resample to coarse
        LOGGER.debug("Start IPEDGSub.")
        tmp_edg_sub_resample = os.path.join(working_dir, "tmp_edg_sub.tif")
        edg_sub_resample_app = resample(onebandequal_app.getoutput().get("out"), dtm_coarse,
                                        tmp_edg_sub_resample,
                                        OtbResampleType.LINEAR_WITH_RADIUS, padradius=4.0,write_output=False)
        # Threshold the output
        out_sub_edg = os.path.join(working_dir, "IPEDGSubOutput.tif")
        param_sub_edg = {"im": edg_sub_resample_app.getoutput().get("out"),
                         "thresholdvalue": 0,
                         "equalvalue": 0,
                         "outsidevalue": 1,
                         "out": out_sub_edg + ":uint8"
                         }
        onebandequal_sub_app = OtbAppHandler("OneBandEqualThreshold", param_sub_edg,write_output=True)
        # Put in internal data
        self._edgsubmask = onebandequal_sub_app.getoutput().get("out")

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
            # ExpandFilterPointer => PadAndResampleImageFilter => app ressampling
            out_ressampling = os.path.join(working_dir, "IPEDGRealL2_{}.tif".format(res_str))
            #threshold to 0.001 has the same result as multiplynig by 1000 and threshold to 1 after
            app = resample(self._edgsubmask, self._dem.ALTList[r], out_ressampling + ":uint8",
                           OtbResampleType.LINEAR,threshold=0.001,write_output=False)
            self._edg_pipeline.add_otb_app(app)
            self._l2edgmasklist.append(app.getoutput().get("out"))

    def generate_l2_sat_images(self, l_ListOfL2Resolution, working_dir):
        for l1res in range(len(l_ListOfL2Resolution)):
            curL1Res = l_ListOfL2Resolution[l1res]
            l_ListOfL2BandCode = self._plugin.BandsDefinitions.get_list_of_l2_band_code(curL1Res)
            dtm = self._dem.ALTList[0]
            tmp_constant_sat_filename = os.path.join(working_dir, "SAT_constant_image_{}.tif".format(l1res))
            app_constant_sat_app = constant_image(dtm, 0, tmp_constant_sat_filename, write_output=False)
            self._sat_pipeline.add_otb_app(app_constant_sat_app)
            # ******************************************************************************************************
            # L2 SAT image pipeline connection
            # ******************************************************************************************************
            for band in l_ListOfL2BandCode:
                self._satmasklist.append(app_constant_sat_app.getoutput().get("out"))

            out_sat_concatenate = os.path.join(working_dir, "SATVectorImageList{}.tif".format(l1res))
            param_concatenate_sat = {"il": self._satmasklist,
                                     "out": out_sat_concatenate + ":uint8"}
            concatenate_sat_app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate_sat, write_output=False)
            self._sat_pipeline.add_otb_app(concatenate_sat_app)
            # L2SAT
            self._l2satmasklist.append(concatenate_sat_app.getoutput().get("out"))

    def generate_sat_sub_image(self, l2comm, working):
        # Sat are not provided on landsat 8 thus no need to resample or threshold anything, just create constant
        # images at coarse resolution
        dtm_coarse = self._dem.ALC

        l_ListOfCoarseBandCode = self._plugin.BandsDefinitions.get_list_of_band_id_in_l2_coarse()
        tmp_constant_sat_filename = os.path.join(working, "SATSub_constant_image.tif")
        app_constant_sat_app = constant_image(dtm_coarse, 0, tmp_constant_sat_filename+ ":uint8", write_output=False)
        self._sat_sub_pipeline.add_otb_app(app_constant_sat_app)
        # ******************************************************************************************************
        # Sub SAT image pipeline connection
        # ******************************************************************************************************
        for band in l_ListOfCoarseBandCode:
            self._satsublist.append(app_constant_sat_app.getoutput().get("out"))
        # end band loop
        #concatenate const image to have the vector image with coarse nb bands
        sat_sub_image = os.path.join(working, "sat_sub.tif")
        param_concatenate = {"il": self._satsublist,
                             "out": sat_sub_image+ ":uint8"
                             }
        app_concatenate = OtbAppHandler("ConcatenateMaskImages", param_concatenate)
        self._subsatimage = app_concatenate.getoutput().get("out")
        self._sat_sub_pipeline.add_otb_app(app_concatenate)
        self._sat_sub_pipeline.free_otb_app()

    def generate_cla_image(self,  working_dir):
        LOGGER.debug("Cloud Altitude (CLA) image doesn't exist. Replaced by constant nodata image. !")
        cla_filename = os.path.join(working_dir, "cla_constant_sub.tif")
        cla_image = constant_image(self._dem.ALC, self._reall1nodata, cla_filename, write_output=True).getoutput()["out"]
        self._claimage = cla_image

    def generate_sol1_image(self, sol_h1, working_dir):
        const_pi_18 = math.pi/180
        sol_filename = os.path.join(working_dir, "sol1.tif")
        mean_solar = self._header_handler.get_mean_solar_angles()
        l_sol1 = sol_h1 * math.tan(const_pi_18 * mean_solar[0]) * math.sin(const_pi_18 * mean_solar[1])
        l_sol2 = sol_h1 * math.tan(const_pi_18 * mean_solar[0]) * math.cos(const_pi_18 * mean_solar[1])
        cla_app_1 = constant_image(self._dem.ALC, l_sol1, sol_filename, write_output=False)
        cla_app_2 = constant_image(self._dem.ALC, l_sol2, sol_filename, write_output=False)
        out_concatenate = os.path.join(working_dir, "sol1_concat.tif")
        param_concatenate = {"il": [cla_app_1.getoutput().get("out"), cla_app_2.getoutput().get("out")],
                             "out": out_concatenate}
        app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate)
        self._sol1image = app.getoutput().get("out")
        self._sol1_pipeline.add_otb_app(app)

    def generate_vie_image(self, view_href, working_dir):
        const_pi_18 = math.pi/180
        vie_filename = os.path.join(working_dir, "vie.tif")
        mean_solar = self._header_handler.get_mean_solar_angles()
        l_vie1 = view_href * math.tan(const_pi_18 * mean_solar[0]) * math.sin(const_pi_18 * mean_solar[1])
        l_vie2 = view_href * math.tan(const_pi_18 * mean_solar[0]) * math.cos(const_pi_18 * mean_solar[1])
        app_1 = constant_image(self._dem.ALC, l_vie1, vie_filename, write_output=False)
        app_2 = constant_image(self._dem.ALC, l_vie2, vie_filename, write_output=False)
        out_concatenate = os.path.join(working_dir, "vie_concat.tif")
        param_concatenate = {"il": [app_1.getoutput().get("out"), app_2.getoutput().get("out")],
                             "out": out_concatenate}
        app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate)
        self._vieimage.append(app.getoutput().get("out"))
        self._vie_pipeline.add_otb_app(app)

    def read(self, product_info, app_handler, l2comm, dem, pReadL1Mode):
        product_filename = product_info.HeaderFilename
        LOGGER.debug("Start Landsat8 L1 ImageFileReader with the filename: " + product_filename)

        self._plugin.initialize(app_handler)
        self._header_handler = product_info.HeaderHandler
        self._GIPPL2COMMHandler = l2comm
        self._dem = dem
        working_dir = app_handler.get_directory_manager().get_temporary_directory("L1Read_",
                                                                                  do_always_remove=True)

        self.ReadL1Mode = pReadL1Mode
        l_BandsDefinitions = self._plugin.BandsDefinitions
        l_factor = xml_tools.as_float_list(l2comm.get_value("CalAdjustFactor"))
        l_ListOfL1Resolution = l_BandsDefinitions.ListOfL1Resolution  # ListOfStrings
        l_ListOfL2Resolution = l_BandsDefinitions.ListOfL2Resolution  # ListOfStrings
        l_SolZenith = self._header_handler.get_sol_zenith()

        # READ THE Inputs
        # *********************************************************************************************************
        # QB Reader connection
        # *********************************************************************************************************
        qb_filename = self._header_handler.get_qbi_image_filename()
        self.generate_qb_mask(qb_filename, working_dir)

        # *********************************************************************************************************
        # TOA Reader connection
        # *********************************************************************************************************
        self.generate_l1_toa_image(l2comm, l_factor, l_SolZenith, l_ListOfL1Resolution, working_dir)

        # *********************************************************************************************************
        # PIX image pipeline connection
        # *********************************************************************************************************
        self.generate_pix_mask(working_dir)

        # *********************************************************************************************************
        #                                     START READ L1 for ALGORITHMS
        # *********************************************************************************************************
        if pReadL1Mode == ReadL1Mode.READ_L1_MODE_FOR_ALGORITHMS:
            # Get information of areas (footprint) of the product (Origin, Spacing and Size for L2 and
            # L2Coarse resolution)
            l_L2Dems = dem.ALTList
            l_CoarseDem = dem.ALC

            # ************************************************************************************************
            # TOA image pipeline connection
            # ************************************************************************************************
            self.generate_l2_toa_images(l_ListOfL2Resolution, working_dir)

            # ************************************************************************************************
            # L2SAT image pipeline connection
            # ************************************************************************************************
            self.generate_l2_sat_images(l_ListOfL2Resolution, working_dir)

            # ************************************************************************************************
            # L2 EDG image pipeline connection
            # ************************************************************************************************
            self.generate_edg_images(working_dir)

            # ************************************************************************************************
            # TOA Sub image pipeline connection
            # ************************************************************************************************
            self.generate_toa_sub_image(working_dir)

            # *********************************.**************************************************************
            # SAT Sub image pipeline connection
            # ************************************************************************************************
            self.generate_sat_sub_image(l2comm, working_dir)

            # ************************************************************************************************
            # CLA image pipeline connectio
            # ************************************************************************************************
            self.generate_cla_image(working_dir)

            # ************************************************************************************************
            # SOL1 image pipeline connectio
            # ************************************************************************************************
            sol_h1 = self._plugin.ConfigUserCamera.get_Algorithms().get_GRID_Reference_Altitudes().get_SOLH1()
            self.generate_sol1_image(sol_h1, working_dir)

            # *************************************************************************************************
            # VIE image pipeline connection
            # *************************************************************************************************
            vie_href = self._plugin.ConfigUserCamera.get_Algorithms().get_GRID_Reference_Altitudes().get_VIEHRef()
            self.generate_vie_image(vie_href, working_dir)


           # Fill the datas
            self.dict_of_vals["IPEDGSubOutput"] = self._edgsubmask
            self.dict_of_vals["L2TOAImageList"] = self._l2toaimagelist
            self.dict_of_vals["L2SATImageList"] = self._l2satmasklist
            self.dict_of_vals["L1PIXImageList"] = self._l2piximagelist
            self.dict_of_vals["L2PIXImageList"] = self._l2piximagelist
            self.dict_of_vals["L2EDGOutputList"] = self._l2edgmasklist
            self.dict_of_vals["IPTOASubOutput"] = self._subtoaimage
            self.dict_of_vals["IPSATSubOutput"] = self._subsatimage
            self.dict_of_vals["CLAImage"] = self._claimage
            self.dict_of_vals["SOL1Image"] = self._sol1image
            self.dict_of_vals["ViewingZenithMeanMap"] = self._meanZenithMap
            self.dict_of_vals["ViewingAzimuthMeanMap"] = self._meanAzimuthMap

            if len(self._vieimage) > 2:
                l_DTMBandCode = l2comm.get_value("DTMViewingDirectionBandCode")
                l_DTMBandIdx = l_BandsDefinitions.get_band_id_in_l2_coarse(l_DTMBandCode)
                LOGGER.info("DTMBandCode= " + l_DTMBandCode)
                self.dict_of_vals[
                    "DTMVIEImage"] = self._vieimage[l_DTMBandIdx]
            else:
                self.dict_of_vals[
                    "DTMVIEImage"] = self._vieimage[0]
            if len(self._vieimage) > 2:
                l_CLDBandCode = l2comm.get_value("CLDViewingDirectionBandCode")
                l_CLDBandIdx = self._plugin.BandsDefinitions.get_band_id_in_l2_coarse(l_CLDBandCode)
                self.dict_of_vals["ShadowVIEImage"] = self._vieimage[l_CLDBandIdx]
            elif len(self._vieimage) > 1:
                self.dict_of_vals["ShadowVIEImage"] = self._vieimage[1]
            else:
                self.dict_of_vals["ShadowVIEImage"] = self._vieimage[0]
            if self._plugin.CirrusMasking:
                l_CirrusBandCode = l2comm.get_value("CirrusBandCode")
                l_CirrusRes = self._plugin.BandsDefinitions.get_l1_resolution_for_band_code(l_CirrusBandCode)
                l_CirrusBandIdx = self._plugin.BandsDefinitions.get_band_id_in_l1(l_CirrusBandCode)
                tmp_toa_roi = os.path.join(working_dir, "L1TOACirrusImage.tif")
                l2toa_roi_app = extract_roi(self._l1toaimagelist[0],
                                            [l_CirrusBandIdx],
                                            tmp_toa_roi, write_output=False)
                self._l2toa_pipeline.add_otb_app(l2toa_roi_app)
                self.dict_of_vals["L1TOACirrusImage"] = l2toa_roi_app.getoutput().get("out")

            self.dict_of_vals["L1TOAImageList"] = self._l1toaimagelist

