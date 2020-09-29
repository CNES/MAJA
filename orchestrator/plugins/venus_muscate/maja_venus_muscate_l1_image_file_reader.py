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
from orchestrator.cots.otb.algorithms.otb_band_math import band_math
from orchestrator.plugins.venus_muscate.maja_venus_muscate_plugin import MajaVenusMuscatePlugin
from orchestrator.cots.otb.algorithms.otb_multiply_by_scalar import multiply_by_scalar
from orchestrator.plugins.common.muscate.maja_muscate_l1_image_file_reader import MuscateL1ImageFileReaderBase

LOGGER = configure_logger(__name__)


class VenusMuscateL1ImageFileReader(MuscateL1ImageFileReaderBase):

    def __init__(self):
        super(VenusMuscateL1ImageFileReader, self).__init__()
        self._Satellite = "VENUS"
        self._plugin = MajaVenusMuscatePlugin()

    def can_read(self, plugin_name):
        return plugin_name == "VENUS_MUSCATE"

    def generate_sol1_image(self, sol_h1, working_dir):
        LOGGER.debug("VenusMuscateL1ImageFileReader::GenerateSOL1Image()")
        # SOL1 image pipeline connection
        # *********************************************************************************************************

        LOGGER.debug("MuscateL1ImageFileReader::GenerateSOL1Image()")
        l_SOL1_DX_ImageFileName = self._header_handler.get_sol_image_filename(sol_h1, "DX")
        LOGGER.debug("l_SOL1_DX_ImageFileName :" + l_SOL1_DX_ImageFileName)
        l_SOL1_DY_ImageFileName = self._header_handler.get_sol_image_filename(sol_h1, "DY")
        LOGGER.debug("l_SOL1_DY_ImageFileName :" + l_SOL1_DY_ImageFileName)

        # SOL1 image pipeline connection
        # *********************************************************************************************************
        # Read the vector images containing the SOL DX and DY
        # Extract the corresponding channels since it is vector images
        m_SOL1ExtractDXChannel = self._header_handler.get_sol_image_index(sol_h1, "DX")[0]
        # --------------------------------------
        # Transform the image coordinates in ground coordinates by multiplying by the L1 product spacing (spec : ImageToGround)
        # All viewing and solar angles are transformed in ground coordinates
        sol1_mult_DX_filename = os.path.join(working_dir, "sol1_mult_DX.tif")
        sol1_mult_DX_app = band_math([l_SOL1_DX_ImageFileName],
                                     "im1b" + str(m_SOL1ExtractDXChannel) + "*" + str(self._dem.L2Areas[0].spacing[0]),
                                     sol1_mult_DX_filename + ":double", write_output=False)

        # DY Stuff
        m_SOL1ExtractDYChannel = self._header_handler.get_sol_image_index(sol_h1, "DY")[0]
        # --------------------------------------
        # Transform the image coordinates in ground coordinates by multiplying by the L1 product spacing (spec : ImageToGround)
        # All viewing and solar angles are transformed in ground coordinates
        # x(-1) if the spacing Y is negative
        sol1_mult_DY_filename = os.path.join(working_dir, "sol1_mult_DY.tif")
        sol1_mult_DY_app = band_math([l_SOL1_DY_ImageFileName], "im1b" +
                                     str(m_SOL1ExtractDYChannel) +
                                     "* -1.0 *" +
                                     str(self._dem.L2Areas[0].spacing[1]), sol1_mult_DY_filename + ":double", write_output=False)
        # Concatenate DX and DY
        out_concatenate = os.path.join(working_dir, "sol1_concat.tif")
        param_concatenate = {"il": [sol1_mult_DX_app.getoutput().get("out"), sol1_mult_DY_app.getoutput().get("out")],
                             "out": out_concatenate}
        concat_app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate, write_output=False)
        # Resample the SOL images
        out_resample = os.path.join(working_dir, "sol1_concat_resamp.tif")
        resamp_app = resample(concat_app.getoutput().get("out"), self._dem.ALC, out_resample, OtbResampleType.LINEAR,
                              padradius=4, write_output=True)
        # , 0 /* edgePaddingValue */)
        # V 4.2 - ITK4                m_SOL1Expand->SetEdgePaddingValue(0)
        # Par defaut le filtre initialise le pixel a zero
        self._sol1image = resamp_app.getoutput().get("out")

    def generate_sol2_image(self, sol_h2, working_dir):
        LOGGER.debug("VenusMuscateL1ImageFileReader::GenerateSOL2Image()")
        # SOL2 image pipeline connection
        # *********************************************************************************************************
        l_SOL2_DX_ImageFileName = self._header_handler.get_sol_image_filename(sol_h2, "DX")
        LOGGER.debug("l_SOL2_DX_ImageFileName :" + l_SOL2_DX_ImageFileName)
        l_SOL2_DY_ImageFileName = self._header_handler.get_sol_image_filename(sol_h2, "DY")
        LOGGER.debug("l_SOL2_DY_ImageFileName :" + l_SOL2_DY_ImageFileName)

        # SOL2 image pipeline connection
        # *********************************************************************************************************
        # Read the vector images containing the SOL DX and DY
        # Extract the corresponding channels since it is vector images
        m_SOL2ExtractDXChannel = self._header_handler.get_sol_image_index(sol_h2, "DX")[0]
        # --------------------------------------
        # Transform the image coordinates in ground coordinates by multiplying by the L1 product spacing (spec : ImageToGround)
        # All viewing and solar angles are transformed in ground coordinates
        sol2_mult_DX_filename = os.path.join(working_dir, "sol2_mult_DX.tif")
        sol2_mult_DX_app = band_math([l_SOL2_DX_ImageFileName],
                                     "im1b" + str(m_SOL2ExtractDXChannel) + "*" + str(self._dem.L2Areas[0].spacing[0]),
                                     sol2_mult_DX_filename + ":double", write_output=False)

        # DY Stuff
        m_SOL2ExtractDYChannel = self._header_handler.get_sol_image_index(sol_h2, "DY")[0]
        # --------------------------------------
        # Transform the image coordinates in ground coordinates by multiplying by the L1 product spacing (spec : ImageToGround)
        # All viewing and solar angles are transformed in ground coordinates
        # x(-1) if the spacing Y is negative
        sol2_mult_DY_filename = os.path.join(working_dir, "sol2_mult_DY.tif")
        sol2_mult_DY_app = band_math([l_SOL2_DY_ImageFileName], "im1b" +
                                     str(m_SOL2ExtractDYChannel) +
                                     "* -1.0 *" +
                                     str(self._dem.L2Areas[0].spacing[1]), sol2_mult_DY_filename + ":double", write_output=False)
        # Concatenate DX and DY
        out_concatenate = os.path.join(working_dir, "sol2_concat.tif")
        param_concatenate = {"il": [sol2_mult_DX_app.getoutput().get("out"), sol2_mult_DY_app.getoutput().get("out")],
                             "out": out_concatenate}
        concat_app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate, write_output=False)
        # Resample the SOL images
        out_resample = os.path.join(working_dir, "sol2_concat_resamp.tif")
        resamp_app = resample(concat_app.getoutput().get("out"), self._dem.ALC, out_resample,
                              OtbResampleType.LINEAR, padradius=4, write_output=True)
        # , 0 /* edgePaddingValue */)
        # V 4.2 - ITK4                m_SOL1Expand->SetEdgePaddingValue(0)
        # Par defaut le filtre initialise le pixel a zero
        self._sol2image = resamp_app.getoutput().get("out")

    def generate_vie_image(self, view_href, working_dir):
        LOGGER.debug("VenusMuscateL1ImageFileReader::GenerateVIEImage()")
        # *********************************************************************************************************
        # VIE image pipeline connection
        # Get the VIE filename from the muscate xml
        l_D01_DX_ImageFileName = self._header_handler.get_vie_image_filename("01", "DX")
        LOGGER.debug(l_D01_DX_ImageFileName)
        l_D01_DY_ImageFileName = self._header_handler.get_vie_image_filename("01", "DY")
        LOGGER.debug(l_D01_DY_ImageFileName)

        l_D04_DX_ImageFileName = self._header_handler.get_vie_image_filename("04", "DX")
        LOGGER.debug(l_D04_DX_ImageFileName)
        l_D04_DY_ImageFileName = self._header_handler.get_vie_image_filename("04", "DY")
        LOGGER.debug(l_D04_DY_ImageFileName)

        # --------------------------------------
        # VIE Expand image to the L2 Coarse resolution
        # Multiply by spacing since it is in m/m
        vie5b1_mult_filename = os.path.join(working_dir, "vie5b1_mult.tif")
        vie5b1_mult_app = multiply_by_scalar(l_D01_DX_ImageFileName,
                                             self._dem.L2Areas[0].spacing[0], vie5b1_mult_filename, write_output=False)

        vie5b2_mult_filename = os.path.join(working_dir, "vie5b1_mult_DX.tif")
        vie5b2_mult_app = multiply_by_scalar(l_D01_DY_ImageFileName,
                                             -1 * self._dem.L2Areas[0].spacing[1], vie5b2_mult_filename, write_output=False)

        vie6b1_mult_filename = os.path.join(working_dir, "vie6b1_mult.tif")
        vie6b1_mult_app = multiply_by_scalar(l_D04_DX_ImageFileName,
                                             self._dem.L2Areas[0].spacing[0], vie6b1_mult_filename, write_output=False)

        vie6b2_mult_filename = os.path.join(working_dir, "vie6b1_mult_DX.tif")
        vie6b2_mult_app = multiply_by_scalar(l_D04_DY_ImageFileName,
                                             -1 * self._dem.L2Areas[0].spacing[1], vie6b2_mult_filename, write_output=False)

        # Concatenate to get the final VIE
        out_concatenate = os.path.join(working_dir, "vie_concat.tif")
        param_concatenate = {"il": [vie5b1_mult_app.getoutput().get("out"), vie5b2_mult_app.getoutput().get("out"),
                                    vie6b1_mult_app.getoutput().get("out"), vie6b2_mult_app.getoutput().get("out")],
                             "out": out_concatenate}
        concat_app = OtbAppHandler("ConcatenateDoubleImages", param_concatenate, write_output=False)

        LOGGER.debug("Start DTMVIEExpand ...")
        out_resample = os.path.join(working_dir, "vie_concat_resamp.tif")
        resamp_app = resample(concat_app.getoutput().get("out"), self._dem.ALC, out_resample,
                              OtbResampleType.LINEAR, padradius=4,
                              write_output=True)
        LOGGER.debug("DTMVIEExpand Done")
        self._vieimagelist.append(resamp_app.getoutput().get("out"))
        # *********************************************************************************************************
        # VIE SHAD image pipeline connection
        # *********************************************************************************************************
        out_shad_concatenate = os.path.join(working_dir, "vie_concat.tif")
        param_shad_concatenate = {"il": [l_D01_DX_ImageFileName, l_D01_DY_ImageFileName],
                                  "out": out_shad_concatenate}
        concat_shad_app = OtbAppHandler("ConcatenateDoubleImages", param_shad_concatenate, write_output=False)

        LOGGER.debug("Start DTMSHADExpand ...")
        out_shad_resample = os.path.join(working_dir, "vieshadconcat_resamp.tif")
        resamp_shad_app = resample(concat_shad_app.getoutput().get("out"), self._dem.ALC, out_shad_resample,
                                   OtbResampleType.LINEAR,
                                   padradius=4,
                                   write_output=True)
        LOGGER.debug("DTMSHADExpand Done")
        self._vieimagelist.append(resamp_shad_app.getoutput().get("out"))
