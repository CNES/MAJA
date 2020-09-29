#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#
# -*- coding: utf-8 -*-
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

orchestrator.plugins.venus.maja_venus_l1_image_file_reader -- shortdesc

orchestrator.plugins.venus.maja_venus_l1_image_file_reader is a description

It defines classes_and_methods

###################################################################################################

:organization: CS SI
:copyright: 2017 CNES. All rights reserved.
:license: see LICENSE file
:created: 12 Jan 2018

###################################################################################################
"""
import os
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.plugins.venus.maja_venus_plugin import MajaVenusPlugin
from orchestrator.common.constants import ReadL1Mode
import orchestrator.common.xml_tools as xml_tools
from orchestrator.cots.otb.algorithms.otb_resample import resample
from orchestrator.cots.otb.algorithms.otb_apply_mask import apply_mask
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.cots.otb.algorithms.otb_resample import OtbResampleType
from orchestrator.cots.otb.algorithms.otb_binary_threshold import binary_threshold
from orchestrator.plugins.common.base.maja_l1_image_reader_base import L1ImageReaderBase
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.common.earth_explorer.header_image_earth_explorer_xml_file_handler import HeaderImageEarthExplorerXMLFileHandler
from orchestrator.common.maja_exceptions import MajaExceptionPluginVenus
from orchestrator.common.maja_common import Area
from orchestrator.cots.gdal.gdal_dataset_info import GdalDatasetInfo, update_projection
LOGGER = configure_logger(__name__)
from orchestrator.cots.otb.algorithms.otb_multiply_by_scalar import multiply_by_scalar
from orchestrator.cots.otb.algorithms.otb_write_images import write_images
from orchestrator.plugins.venus.maja_venus_l1_image_filenames import VenusL1ImageFilenames


class VenusL1ImageFileReader(L1ImageReaderBase):

    TOAFirstChannel = 1
    TOALastChannel = 12
    PIXChannel = 14
    SATChannel = 13
    PIXNumberOfComponentsPerPixel = 12
    SATNumberOfComponentsPerPixel = 12
    SOL1ChannelB1 = "?&sdataidx=0"
    SOL1ChannelB2 = "?&sdataidx=1"
    SOL2ChannelB1 = "?&sdataidx=2"
    SOL2ChannelB2 = "?&sdataidx=3"
    VIEB5ChannelB1 = "?&sdataidx=0"
    VIEB5ChannelB2 = "?&sdataidx=1"
    VIEB6ChannelB1 = "?&sdataidx=6"
    VIEB6ChannelB2 = "?&sdataidx=7"



    def __init__(self):
        super(VenusL1ImageFileReader, self).__init__()
        self._plugin = MajaVenusPlugin()
        self._Satellite = "VEnUS"
        self.headerHandler = None
        self._l1toa_pipeline = OtbPipelineManager()
        self._l1pix_pipeline = OtbPipelineManager()
        self._l2pix_pipeline = OtbPipelineManager()
        self._edg_pipeline = OtbPipelineManager()
        self._l2toa_pipeline = OtbPipelineManager()
        self._l2edg_pipeline = OtbPipelineManager()
        self._sat_pipeline = OtbPipelineManager()
        self._sol_pipeline = OtbPipelineManager()
        self._dtmvie_pipeline = OtbPipelineManager()
        self._shadowvie_pipeline = OtbPipelineManager()
        self._toascalar = ""
        self._l1pix = ""
        self._l2pix = ""
        self._edgsub = ""
        self._toasub = ""
        self._l2toa = ""
        self._l2edg = ""
        self._l2sat = ""
        self._satsub = ""
        self._cla = ""
        self._sol1 = ""
        self._sol2 = ""
        self._dtmvie = ""
        self._shadowvie = ""
        self._reall1nodata = -10
        self._meanZenithMap = {}
        self._meanAzimuthMap = {}

    def can_read(self, plugin_name):
        return plugin_name == "VENUS"



    def read(self, product_info, app_handler, l2comm, dem, pReadL1Mode):
        """product_info,plugin, l2comm,mode

        :param product_info: L1ImageInformationsBase
        :param pReadL1Mode: ReadL1ModeType
        :return:
        """
        LOGGER.debug("Start Venus L1 ImageFileReader ...")
        product_filename = product_info.HeaderFilename
        LOGGER.debug("Start Venus L1 ImageFileReader with the filename: " + product_filename)
        self._plugin.initialize(app_handler)
        self.headerHandler = product_info.HeaderHandler
        self._dem = dem
        # working_dir = get_working_directory("L1Read_", app_handler.get_working_directory())
        working_dir = app_handler.get_directory_manager().get_temporary_directory("L1Read_",
                                                                                  do_always_remove=True)
        self.ReadL1Mode = pReadL1Mode
        self._GIPPL2COMMHandler = l2comm
        l_BandsDefinitions = self._plugin.BandsDefinitions
        # --------------------------------------
        # Initialize the Image filename provider
        l_FilenameProvider = VenusL1ImageFilenames()
            #product_info.FilenamesProvider
        IsValidSatellite = (l_FilenameProvider.initialize(product_filename) is not False)
        if not IsValidSatellite:
            raise MajaExceptionPluginVenus(
                "The file <{}> is not a valid Venus L1 product.".format(product_filename))

        # *********************************************************************************************************
        # Register the Header o the Input image file
        # *********************************************************************************************************
        LOGGER.debug("Load the Venus L1 Header file : '" + product_filename + "'")
        l_L1XMLHandler = HeaderImageEarthExplorerXMLFileHandler(product_filename)

        # *********************************************************************************************************
        # TOA Reader connection
        # *********************************************************************************************************
        l_L1NoData = l_L1XMLHandler.get_no_data_value_as_double()
        l_ReflectanceQuantificationValue = l_L1XMLHandler.get_reflectance_quantification_value()
        l_reflectanceMultiplicationValues = []

        if xml_tools.as_bool(l2comm.get_value("CalAdjustOption")):
            l_factor = xml_tools.as_float_list(l2comm.get_value("CalAdjustFactor"))
            if len(l_factor) != (VenusL1ImageFileReader.TOALastChannel-VenusL1ImageFileReader.TOAFirstChannel+1):
                raise MajaExceptionPluginVenus("Not the same number of Calibration coeffs than L1 bands")
            for i in range(0,len(l_factor)):
                l_reflectanceMultiplicationValues.append(l_ReflectanceQuantificationValue * l_factor[i])
        else:
            for i in range(VenusL1ImageFileReader.TOALastChannel-VenusL1ImageFileReader.TOAFirstChannel+1):
                l_reflectanceMultiplicationValues.append(l_ReflectanceQuantificationValue)

        l_RealL1NoData = l_L1NoData * l_ReflectanceQuantificationValue

        # *********************************************************************************************************
        # L1 TOA image pipeline connection
        # *********************************************************************************************************
        if not l_FilenameProvider.m_TOAImageFileName:
            raise MajaExceptionPluginVenus("VenusL1ImageFileReader : The TOA image does not exist !")

        tmp_l1toa_roi = os.path.join(working_dir, "l1toa_roi.tif")
        app_l1_toa_roi = extract_roi(l_FilenameProvider.m_TOAImageFileName,
                                    [channel for channel in range(VenusL1ImageFileReader.TOALastChannel - VenusL1ImageFileReader.TOAFirstChannel+1)],
                                    tmp_l1toa_roi, write_output=False)
        self._l1toa_pipeline.add_otb_app(app_l1_toa_roi)

        tmp_sat_roi = os.path.join(working_dir, "sat_roi.tif")
        app_sat_roi = extract_roi(l_FilenameProvider.m_TOAImageFileName,
                                  [VenusL1ImageFileReader.SATChannel - 1],
                                  tmp_sat_roi, write_output=False)
        self._sat_pipeline.add_otb_app(app_sat_roi)
        #Multiply scalar by quantif
        tmp_l1toa_mbs = os.path.join(working_dir, "l1toa.tif")
        app_l1toa_mbs= multiply_by_scalar(app_l1_toa_roi.getoutput().get("out"),
                                          l_ReflectanceQuantificationValue,
                                          output_image=tmp_l1toa_mbs, write_output=False)
        self._l1toa_pipeline.add_otb_app(app_l1toa_mbs)
        # update all extract ROI in once
        write_images([app_l1toa_mbs.getoutput().get("out"), app_sat_roi.getoutput().get("out")],
                     [tmp_l1toa_mbs, tmp_sat_roi])

        self._toascalar = tmp_l1toa_mbs

        # *********************************************************************************************************
        # L1 PIX image pipeline connection
        # *********************************************************************************************************
        tmp_l1pix_roi = os.path.join(working_dir, "l1pix.tif")
        app_l1_pix_roi = extract_roi(l_FilenameProvider.m_TOAImageFileName,
                                     [VenusL1ImageFileReader.PIXChannel-1],
                                     tmp_l1pix_roi + ":uint16", write_output=False)
        self._l1pix_pipeline.add_otb_app(app_l1_pix_roi)
        self._l1pix = app_l1_pix_roi.getoutput().get("out")

        # *********************************************************************************************************
        #                                     START READ L1 for ALGORITHMS
        # *********************************************************************************************************
        if pReadL1Mode == ReadL1Mode.READ_L1_MODE_FOR_ALGORITHMS:
            # *********************************************************************************************************
            # L2 PIX image pipeline connection
            # *********************************************************************************************************
            # LAIG-FA-MAC-131720-CS : New for 4.2
            # Before resample, binarytovector -> resample -> vectortobinary
            tmp_l2pix_bin2vec = os.path.join(working_dir, "l2pix_bin2vec.tif")
            param_l2pix_bin2vec = {"im": app_l1_pix_roi.getoutput().get("out"),
                                 "out": tmp_l2pix_bin2vec + ":uint8",
                                 "nbcomp": VenusL1ImageFileReader.PIXNumberOfComponentsPerPixel
                                 }
            app_l2pix_bin2vec = OtbAppHandler("BinaryToVector", param_l2pix_bin2vec, write_output=False)
            self._l2pix_pipeline.add_otb_app(app_l2pix_bin2vec)

            tmp_l2pix_resample = os.path.join(working_dir, "l2pix_resample.tif")
            app_l2pix_resample = resample(app_l2pix_bin2vec.getoutput().get("out"),
                                          self._dem.ALTList[0],
                                          tmp_l2pix_resample,
                                          OtbResampleType.LINEAR_WITH_RADIUS, padradius=4.0, threshold=0.0,
                                          write_output=False)
            self._l2pix_pipeline.add_otb_app(app_l2pix_resample)
            #L2 PIX is concatenate
            tmp_l2pix_binconcat = os.path.join(working_dir, "l2pix.tif")
            param_l2pix_binconcat = {"im": app_l2pix_resample.getoutput().get("out"),
                                   "out": tmp_l2pix_binconcat + ":uint16"}
            app_l2pix_binconcat = OtbAppHandler("BinaryConcatenate", param_l2pix_binconcat, write_output=False)
            self._l2pix = app_l2pix_binconcat.getoutput().get("out")
            self._l2pix_pipeline.add_otb_app(app_l2pix_binconcat)

            # *********************************************************************************************************
            # L2 EDG image pipeline connection
            # *********************************************************************************************************
            tmp_edg_thresholder = os.path.join(working_dir, "edg_thresholder1.tif")
            param_edg_thresholder1 = {"im": self._toascalar,
                         "thresholdvalue": l_RealL1NoData,
                         "equalvalue": 255,
                         "outsidevalue": 0,
                         "out": tmp_edg_thresholder + ":uint8"
                         }
            app_edg_thresholder1 = OtbAppHandler("OneBandEqualThreshold", param_edg_thresholder1, write_output=True)
            self._edg_pipeline.add_otb_app(app_edg_thresholder1)

            tmp_edg_resample = os.path.join(working_dir, "edg_resample.tif")
            app_edg_resample = resample(app_edg_thresholder1.getoutput().get("out"), self._dem.ALTList[0],
                                        tmp_edg_resample,
                                        OtbResampleType.BCO, padradius=4.0,  write_output=True)
            self._edg_pipeline.add_otb_app(app_edg_resample)

            # Threshold the output
            out_sub_edg = os.path.join(working_dir, "edg_thresholder2.tif")
            param_edg_thresholder2 = {"im": app_edg_resample.getoutput().get("out"),
                             "thresholdvalue": 0,
                             "equalvalue": 1,
                             "outsidevalue": 0,
                             "out": out_sub_edg + ":uint8"
                             }
            app_edg_thresholder2 = OtbAppHandler("OneBandEqualThreshold", param_edg_thresholder2, write_output=True)
            self._edg_pipeline.add_otb_app(app_edg_thresholder2)

            # *********************************************************************************************************
            # IPEDGSub image pipeline connection
            # *********************************************************************************************************
            tmp_edgsub_resample = os.path.join(working_dir, "edgsub_resample.tif")
            app_edgsub_resample = resample(app_edg_thresholder1.getoutput().get("out"), self._dem.ALC,
                                        tmp_edgsub_resample,
                                        OtbResampleType.LINEAR_WITH_RADIUS, padradius=12.0, write_output=True)
            self._edg_pipeline.add_otb_app(app_edgsub_resample)
            # Threshold the output
            out_sub_edgsub = os.path.join(working_dir, "edgsub.tif")
            param_edgsub_thresholder2 = {"im": app_edgsub_resample.getoutput().get("out"),
                                      "thresholdvalue": 0,
                                      "equalvalue": 0,
                                      "outsidevalue": 1,
                                      "out": out_sub_edgsub + ":uint8"
                                      }
            app_edgsub_thresholder2 = OtbAppHandler("OneBandEqualThreshold", param_edgsub_thresholder2, write_output=True)
            self._edgsub = app_edgsub_thresholder2.getoutput().get("out")
            self._edg_pipeline.add_otb_app(app_edgsub_thresholder2)


            # *********************************************************************************************************
            # L2 TOA image pipeline connection
            # *********************************************************************************************************
            tmp_l2toa_resample = os.path.join(working_dir, "l2toa_resample.tif")
            app_l2toa_resample = resample(self._toascalar, self._dem.ALTList[0],
                                        tmp_l2toa_resample,
                                        OtbResampleType.BCO, padradius=4.0,write_output=False)
            self._l2toa_pipeline.add_otb_app(app_l2toa_resample)
            l2toa_list = []
            l_toathresholdminvalue = 0
            l_toathresholvalue = -10

            #Apply EDG mask on l2toa resampled
            tmp_l2toa = os.path.join(working_dir, "l2toa.tif")
            app_l2toa = apply_mask(app_l2toa_resample.getoutput().get("out"),
                                          app_edg_thresholder2.getoutput().get("out"),
                                          l_toathresholvalue, tmp_l2toa,write_output=False)
            self._l2toa = app_l2toa.getoutput().get("out")
            self._l2toa_pipeline.add_otb_app(app_l2toa)

            # *********************************************************************************************************
            # TOA Sub image pipeline connection
            # *********************************************************************************************************
            tmp_toasub_resample = os.path.join(working_dir, "toasub_resample.tif")
            app_toasub_resample = resample(self._toascalar,
                                           self._dem.ALC,
                                          tmp_toasub_resample,
                                          OtbResampleType.LINEAR_WITH_RADIUS, padradius=4.0, write_output=True)
            self._l2toa_pipeline.add_otb_app(app_toasub_resample)

            # Threshold the output
            out_edgsub_threshold = os.path.join(working_dir, "edgsubthreshold.tif")
            param_edgsub_threshold = {"im": app_edgsub_resample.getoutput().get("out"),
                                      "thresholdvalue": 0,
                                      "equalvalue": 1,
                                      "outsidevalue": 0,
                                      "out": out_edgsub_threshold + ":uint8"
                                      }
            app_edgsub_threshold = OtbAppHandler("OneBandEqualThreshold", param_edgsub_threshold, write_output=True)
            self._edg_pipeline.add_otb_app(app_edgsub_threshold)


            tmp_l2subtoa = os.path.join(working_dir, "toasub.tif")
            app_l2subtoa = apply_mask(app_toasub_resample.getoutput().get("out"),
                                   app_edgsub_threshold.getoutput().get("out"),
                                   l_toathresholvalue, tmp_l2subtoa, write_output=True)
            self._toasub = app_l2subtoa.getoutput().get("out")
            self._l2toa_pipeline.add_otb_app(app_l2subtoa)

            # *********************************************************************************************************
            # L2EDG - Actualization of the L2 edge mask
            # *********************************************************************************************************
            #tmp_l2edg_threshold = os.path.join(working_dir, "l2edg_threshold.tif")
            #app_l2edg_threshold = binary_threshold(self._edgsub,
            #                                         lower_threshold=0,
            #                                         inside_value=1000,
            #                                         outside_value=0,
            #                                         output_image=tmp_l2edg_threshold + ":uint8",
            #                                         write_output=True)
            #self._l2edg_pipeline.add_otb_app(app_l2edg_threshold)
            tmp_l2edg_resample = os.path.join(working_dir, "l2edg.tif")
            app_l2edg_resample = resample(self._edgsub,
                                          self._dem.ALTList[0],
                                          tmp_l2edg_resample + ":uint8",
                                          OtbResampleType.LINEAR, padradius=4.0, threshold=0.001,write_output=True)
            self._l2edg = app_l2edg_resample.getoutput().get("out")
            self._l2edg_pipeline.add_otb_app(app_l2edg_resample)

            # *********************************************************************************************************
            # SAT image pipeline connection
            # *********************************************************************************************************
            tmp_sat_bin2vec = os.path.join(working_dir, "sat_bin2vec.tif")
            param_sat_bin2vec = {"im": tmp_sat_roi,
                                 "out": tmp_sat_bin2vec + ":uint8",
                                 "nbcomp": VenusL1ImageFileReader.SATNumberOfComponentsPerPixel
                                 }
            app_sat_bin2vec = OtbAppHandler("BinaryToVector", param_sat_bin2vec, write_output=False)
            self._sat_pipeline.add_otb_app(app_sat_bin2vec)

            l_l2sat_thresholdvalue = l2comm.get_value_f("SaturationThreshold")

            tmp_sat_resample = os.path.join(working_dir, "l2sat.tif")
            app_sat_resample = resample(app_sat_bin2vec.getoutput().get("out"),
                                        self._dem.ALTList[0],
                                        tmp_sat_resample + ":uint8",
                                        OtbResampleType.BCO, padradius=4.0,
                                        threshold=l_l2sat_thresholdvalue,write_output=False)
            self._l2sat = app_sat_resample.getoutput().get("out")
            self._sat_pipeline.add_otb_app(app_sat_resample)

            # *********************************************************************************************************
            # IPSAT Sub image pipeline connection
            # *********************************************************************************************************
            l_sat_subthresholdvalue = l2comm.get_value_f("SaturationThresholdSub")
            tmp_satsub_resample = os.path.join(working_dir, "satsub.tif")
            app_satsub_resample = resample(app_sat_bin2vec.getoutput().get("out"),
                                        self._dem.ALC,
                                        tmp_satsub_resample + ":uint8",
                                        OtbResampleType.LINEAR_WITH_RADIUS, padradius=4.0,
                                        threshold=l_sat_subthresholdvalue)
            self._satsub = app_satsub_resample.getoutput().get("out")
            self._sat_pipeline.add_otb_app(app_satsub_resample)

            # *********************************************************************************************************
            # CLA image pipeline connection
            # *********************************************************************************************************

            LOGGER.debug("VenusL1ImageFileReader::Initialize - CLA image filename: '" + l_FilenameProvider.m_CLAImageFileName + "'")
            if not l_FilenameProvider.m_CLAImageFileName:
                raise MajaExceptionPluginVenus(
                    "The CLA image does not exist !! ")
            self._cla = l_FilenameProvider.m_CLAImageFileName

            # *********************************************************************************************************
            # SOL1 image pipeline connection
            # *********************************************************************************************************
            LOGGER.debug(
                "VenusL1ImageFileReader::Initialize - SOL image filename: '" + l_FilenameProvider.m_SOLImageFileName + "'")
            if not l_FilenameProvider.m_SOLImageFileName:
                raise MajaExceptionPluginVenus(
                    "The SOL image does not exist !! ")
            mtdat = GdalDatasetInfo(l_FilenameProvider.m_TOAImageFileName)
            toaarea = Area()
            toaarea.size = mtdat.size
            toaarea.origin = mtdat.origin
            toaarea.spacing = mtdat.pixel_size
            l_SOLHeaderHandler = HeaderImageEarthExplorerXMLFileHandler(l_FilenameProvider.m_SOLHeaderFileName)
            l_L1SOLSubsamplingFactor = l_SOLHeaderHandler.get_sampling_factor()
            LOGGER.debug(l_L1SOLSubsamplingFactor)
            # SOL1
            tmp_sol1_b1 = os.path.join(working_dir, "sol1_B1.tif")
            app_sol1_b1 = multiply_by_scalar(
                l_FilenameProvider.m_SOLImageFileName+VenusL1ImageFileReader.SOL1ChannelB1, toaarea.spacing[0],
                tmp_sol1_b1,write_output=False)
            self._sol_pipeline.add_otb_app(app_sol1_b1)

            tmp_sol1_b2 = os.path.join(working_dir, "sol1_B2.tif")
            app_sol1_b2 = multiply_by_scalar(
                l_FilenameProvider.m_SOLImageFileName + VenusL1ImageFileReader.SOL1ChannelB2, (-1)*toaarea.spacing[1],
                tmp_sol1_b2,write_output=False)
            self._sol_pipeline.add_otb_app(app_sol1_b2)
            tmp_sol1_concat = os.path.join(working_dir, "sol1_concat.tif")
            param_sol1_concat = {"il": [app_sol1_b1.getoutput().get("out"), app_sol1_b2.getoutput().get("out")],
                                 "out": tmp_sol1_concat}
            app_sol1_concat = OtbAppHandler("ConcatenateDoubleImages", param_sol1_concat)
            update_projection(l_FilenameProvider.m_TOAImageFileName, app_sol1_concat.getoutput().get("out"),
                                   l_L1SOLSubsamplingFactor)

            self._sol_pipeline.add_otb_app(app_sol1_concat)

            tmp_sol1_resample = os.path.join(working_dir, "sol1.tif")
            app_sol1_resample = resample(app_sol1_concat.getoutput().get("out"), self._dem.ALC, tmp_sol1_resample,
                                        OtbResampleType.LINEAR, padradius=4.0)
            self._sol1 = app_sol1_resample.getoutput().get("out")
            self._sol_pipeline.add_otb_app(app_sol1_resample)

            # SOL2
            tmp_sol2_b1 = os.path.join(working_dir, "sol2_B1.tif")
            app_sol2_b1 = multiply_by_scalar(
                l_FilenameProvider.m_SOLImageFileName + VenusL1ImageFileReader.SOL2ChannelB1, toaarea.spacing[0],
                tmp_sol2_b1,write_output=False)
            self._sol_pipeline.add_otb_app(app_sol2_b1)
            tmp_sol2_b2 = os.path.join(working_dir, "sol2_B2.tif")
            app_sol2_b2 = multiply_by_scalar(
                l_FilenameProvider.m_SOLImageFileName + VenusL1ImageFileReader.SOL2ChannelB2, (-1)*toaarea.spacing[1],
                tmp_sol2_b2,write_output=False)
            self._sol_pipeline.add_otb_app(app_sol2_b2)

            tmp_sol2_concat = os.path.join(working_dir, "sol2_concat.tif")
            param_sol2_concat = {"il": [app_sol2_b1.getoutput().get("out"), app_sol2_b2.getoutput().get("out")],
                                 "out": tmp_sol2_concat}
            app_sol2_concat = OtbAppHandler("ConcatenateDoubleImages", param_sol2_concat)
            update_projection(l_FilenameProvider.m_TOAImageFileName, app_sol2_concat.getoutput().get("out"),
                              l_L1SOLSubsamplingFactor)
            self._sol_pipeline.add_otb_app(app_sol2_concat)

            tmp_sol2_resample = os.path.join(working_dir, "sol2.tif")
            app_sol2_resample = resample(app_sol2_concat.getoutput().get("out"), self._dem.ALC,
                                         tmp_sol2_resample, OtbResampleType.LINEAR, padradius=4.0)
            self._sol2 = app_sol2_resample.getoutput().get("out")
            self._sol_pipeline.add_otb_app(app_sol2_resample)


            # *********************************************************************************************************
            # DTMVIE image pipeline connection
            # *********************************************************************************************************
            LOGGER.debug(
                "VenusL1ImageFileReader::Initialize - VIE image filename: '" + l_FilenameProvider.m_VIEImageFileName + "'")

            l_VIEHeaderHandler = HeaderImageEarthExplorerXMLFileHandler(l_FilenameProvider.m_VIEHeaderFileName)
            l_L1VIESubsamplingFactor = l_VIEHeaderHandler.get_sampling_factor()
            LOGGER.debug(l_L1VIESubsamplingFactor)

            tmp_vieb5b1_mult = os.path.join(working_dir, "vie5b1_mult.tif")
            app_vieb5b1_mult = multiply_by_scalar(l_FilenameProvider.m_VIEImageFileName + VenusL1ImageFileReader.VIEB5ChannelB1,
                                                  toaarea.spacing[0], tmp_vieb5b1_mult,write_output=False)
            self._dtmvie_pipeline.add_otb_app(app_vieb5b1_mult)

            tmp_vieb5b2_mult = os.path.join(working_dir, "vie5b2_mult.tif")
            app_vieb5b2_mult = multiply_by_scalar(
                l_FilenameProvider.m_VIEImageFileName + VenusL1ImageFileReader.VIEB5ChannelB2,
                (-1) * toaarea.spacing[1], tmp_vieb5b2_mult,write_output=False)
            self._dtmvie_pipeline.add_otb_app(app_vieb5b2_mult)

            tmp_vieb6b1_mult = os.path.join(working_dir, "vie6b1_mult.tif")
            app_vieb6b1_mult = multiply_by_scalar(
                l_FilenameProvider.m_VIEImageFileName + VenusL1ImageFileReader.VIEB6ChannelB1,
                toaarea.spacing[0], tmp_vieb6b1_mult,write_output=False)
            self._dtmvie_pipeline.add_otb_app(app_vieb6b1_mult)

            tmp_vieb6b2_mult = os.path.join(working_dir, "vie6b2_mult.tif")
            app_vieb6b2_mult = multiply_by_scalar(
                l_FilenameProvider.m_VIEImageFileName + VenusL1ImageFileReader.VIEB6ChannelB2,
                (-1) * toaarea.spacing[1], tmp_vieb6b2_mult,write_output=False)
            self._dtmvie_pipeline.add_otb_app(app_vieb6b2_mult)

            tmp_dtmvie_concat = os.path.join(working_dir, "dtmvie_concat.tif")
            param_dtmvie_concat = {"il": [app_vieb5b1_mult.getoutput().get("out"),
                                          app_vieb5b2_mult.getoutput().get("out"),
                                          app_vieb6b1_mult.getoutput().get("out"),
                                          app_vieb6b2_mult.getoutput().get("out")],
                                 "out": tmp_dtmvie_concat}
            app_dtmvie_concat = OtbAppHandler("ConcatenateDoubleImages", param_dtmvie_concat)
            update_projection(l_FilenameProvider.m_TOAImageFileName, app_dtmvie_concat.getoutput().get("out"),
                              l_L1VIESubsamplingFactor)
            self._dtmvie_pipeline.add_otb_app(app_dtmvie_concat)

            tmp_dtmvie_resample = os.path.join(working_dir, "dtmvie.tif")
            app_dtmvie_resample = resample(app_dtmvie_concat.getoutput().get("out"),
                                         self._dem.ALC,
                                         tmp_dtmvie_resample,
                                         OtbResampleType.LINEAR, padradius=4.0)
            self._dtmvie = app_dtmvie_resample.getoutput().get("out")
            self._dtmvie_pipeline.add_otb_app(app_dtmvie_resample)


            # *********************************************************************************************************
            # VIE image pipeline connection
            # *********************************************************************************************************
            tmp_shadowvie_concat = os.path.join(working_dir, "shadowvie_concat.tif")
            param_shadowvie_concat = {"il": [app_vieb5b1_mult.getoutput().get("out"),
                                          app_vieb5b2_mult.getoutput().get("out")],
                                   "out": tmp_shadowvie_concat}
            app_shadowvie_concat = OtbAppHandler("ConcatenateDoubleImages", param_shadowvie_concat)
            self._shadowvie_pipeline.add_otb_app(app_shadowvie_concat)
            update_projection(l_FilenameProvider.m_TOAImageFileName, app_shadowvie_concat.getoutput().get("out"),
                              l_L1VIESubsamplingFactor)
            tmp_shadowvie_resample = os.path.join(working_dir, "shadowvie.tif")
            app_shadowvie_resample = resample(app_shadowvie_concat.getoutput().get("out"),
                                           self._dem.ALC,
                                              tmp_shadowvie_resample,
                                           OtbResampleType.LINEAR, padradius=4.0)
            self._shadowvie = app_shadowvie_resample.getoutput().get("out")
            self._shadowvie_pipeline.add_otb_app(app_shadowvie_resample)

            # Fill the datas
            self.dict_of_vals[
                "IPEDGSubOutput"] = self._edgsub
            self.dict_of_vals[
                "SOL1Image"] = self._sol1
            self.dict_of_vals[
                "SOL2Image"] = self._sol2
            self.dict_of_vals[
                    "DTMVIEImage"] = self._dtmvie
            self.dict_of_vals[
                "IPTOASubOutput"] = self._toasub

            self.dict_of_vals[
                "L2TOAImageList"] = [self._l2toa]
            self.dict_of_vals["ViewingZenithMeanMap"] = self._meanZenithMap
            self.dict_of_vals["ViewingAzimuthMeanMap"] = self._meanAzimuthMap
            self.dict_of_vals["CLAImage"] = self._cla
            self.dict_of_vals["IPSATSubOutput"] = self._satsub
            self.dict_of_vals["ShadowVIEImage"] = self._shadowvie
            if self._plugin.CirrusMasking:
                l_CirrusBandCode = l2comm.get_value("CirrusBandCode")
                l_CirrusBandIdx = self._plugin.BandsDefinitions.get_band_id_in_l2_coarse(l_CirrusBandCode)
                tmp = os.path.join(working_dir, "l1toacirrus.tif")
                app = extract_roi(
                    self._toascalar,
                    [l_CirrusBandIdx-1],
                    tmp
                )
                self.dict_of_vals["L1TOACirrusImage"] = app.getoutput().get("out")
            self.dict_of_vals["L2EDGOutputList"] = [self._l2edg]
            self.dict_of_vals["L2SATImageList"] = [self._l2sat]
            self.dict_of_vals["L2PIXImageList"] = [self._l2pix]
            self.dict_of_vals["L1PIXImageList"] = [self._l1pix]
            self.dict_of_vals["L1TOAImageList"] = [self._toascalar]
