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

orchestrator.plugins.sentinel2.maja_sentinel2_l1_image_file_reader -- shortdesc

orchestrator.plugins.maja_sentinel2_l1_image_file_reader is a description

It defines classes_and_methods


###################################################################################################
"""
import os
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.cots.otb.algorithms.otb_resample import resample
from orchestrator.cots.otb.algorithms.otb_resample import OtbResampleType
from orchestrator.cots.otb.algorithms.otb_binary_threshold import binary_threshold
from orchestrator.plugins.landsat8_muscate.maja_landsat8_muscate_plugin import MajaLandsat8MuscatePlugin
from orchestrator.plugins.common.muscate.maja_muscate_l1_image_file_reader import MuscateL1ImageFileReaderBase

LOGGER = configure_logger(__name__)


class Landsat8MuscateL1ImageFileReader(MuscateL1ImageFileReaderBase):

    def __init__(self):
        super(Landsat8MuscateL1ImageFileReader, self).__init__()
        self._Satellite = "LANDSAT8"
        self._plugin = MajaLandsat8MuscatePlugin()

    def can_read(self, plugin_name):
        return plugin_name == "LANDSAT8_MUSCATE"

    def generate_edg_images(self, working_dir):
        """
        :param working_dir: Path
        :return:
        """
        dtm_coarse = self._dem.ALC

        # Get the mask filename
        edg_mask_filename = self._header_handler.get_designated_fill_mask_filename()

        # Resample to coarse
        LOGGER.debug("Start IPEDGSub.")
        tmp_edg_sub_resample = os.path.join(working_dir, "tmp_edg_sub.tif")
        edg_sub_resample_app = resample(edg_mask_filename, dtm_coarse, tmp_edg_sub_resample,
                                        OtbResampleType.LINEAR_WITH_RADIUS, padradius=4.0,write_output=False)
        # Threshold the output
        out_sub_edg = os.path.join(working_dir, "tmp_edg_sub_oneBandEqual.tif")
        param_sub_edg = {"im": edg_sub_resample_app.getoutput().get("out"),
                         "thresholdvalue": 0,
                         "equalvalue": 0,
                         "outsidevalue": 1,
                         "out": out_sub_edg + ":uint8"
                         }
        onebandequal_sub_app = OtbAppHandler("OneBandEqualThreshold", param_sub_edg)
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
        # Set 1000 to edge pixels to identify the pixel contaminated by an edge pixel after resampling
        #out_thresh = os.path.join(working_dir, "EDGThreshL2.tif")
        #m_L2EDGThresholdApp = binary_threshold(self._edgsubmask,
        #                                         lower_threshold=0,
        #                                         inside_value=10000,
        #                                         outside_value=0,
        #                                         output_image=out_thresh + ":double",
        #                                         write_output=False)  # //l_ThresholdImageFilter
        #self._l2edg_pipeline.add_otb_app(m_L2EDGThresholdApp)
        for r in range(l_NbL2Res):
            res_str = l_ListOfL2Resolution[r]
            # ExpandFilterPointer => PadAndResampleImageFilter => app ressampling
            out_ressampling = os.path.join(working_dir, "IPEDGRealL2_{}.tif".format(res_str))
            l2_edg_resamp_app = resample(self._edgsubmask,
                                         self._dem.ALTList[r], out_ressampling, OtbResampleType.LINEAR,threshold=0.0001)
            self._l2edg_pipeline.add_otb_app(l2_edg_resamp_app)
            # Set Threshold value to one because the expand filter interpolates values set to 0
            # or 1000 in the first threshold and adds systematically CONST_EPSILON to the output value.
            #m_L2EDGThresholdImage2_out = os.path.join(working_dir, "IPEDGMaskL2_{}.tif".format(res_str))
            #m_L2EDGThresholdApp2 = binary_threshold(l2_edg_resamp_app.getoutput().get("out"),
            #                                          lower_threshold=1.,
            #                                          inside_value=1,
            #                                          outside_value=0,
            #                                          output_image=m_L2EDGThresholdImage2_out + ":uint8",
            #                                          write_output=False)
            #self._l2edg_pipeline.add_otb_app(m_L2EDGThresholdApp2)
            self._l2edgmasklist.append(l2_edg_resamp_app.getoutput().get("out"))
