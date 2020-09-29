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
from orchestrator.common.maja_exceptions import MajaBusinessException
from orchestrator.common.logger.maja_logging import configure_logger

LOGGER = configure_logger(__name__)


def compute_origin(in_origin, in_spacing, out_spacing, mode):
    """

    :param in_origin: VNSOriginType
    :param in_spacing: VNSSpacingType
    :param out_spacing: VNSSpacingType
    :param mode: ResamplerHelper
    :return:
    """
    outOrigin = None
    factor = None

    # --------------------------------------------------------------------------------------
    # Resampling factor in line and column
    factor[0] = float(in_spacing[0]) / float(out_spacing[0])
    factor[1] = float(in_spacing[1]) / float(out_spacing[1])

    # TODO:
    # LOGGER.debug("Resampling CheckComputeOrigin     - pixel mode:             %s (with CENTERED="<<vns::ResamplerHelper::RB_MODE_CENTERED_PIXEL<<" and UPPER_LEFT_CORNER="<<vns::ResamplerHelper::RB_MODE_UPPER_LEFT_CORNER_PIXEL<<")        ", mode)
    # LOGGER.debug("                                  - Input image   origin:    ["<<inOrigin[0]<<";"<<inOrigin[1]<<"]        ")
    # LOGGER.debug("                                  -              spacing:    ["<<inSpacing[0]<<";"<<inSpacing[1]<<"]        ")
    # LOGGER.debug("                                  - Output image spacing:    ["<<outSpacing[0]<<";"<<outSpacing[1]<<"]        ")
    # LOGGER.debug("                                  - Factor:                  ["<<factor[0]<<";"<<factor[1]<<"]        ")

    # Check the sign of the input and output spacing
    if ((in_spacing[0] > 0) and (out_spacing[0] < 0)) or ((in_spacing[0] < 0) and (out_spacing[0] > 0)) \
            or ((in_spacing[1] > 0) and (out_spacing[1] < 0)) or ((in_spacing[1] < 0) and (out_spacing[1] > 0)):
        raise MajaBusinessException(
            "For resampling, input and output spacing must have the same sign for each dimension!")

    # --------------------------------------------------------------------------------------
    # Two convention are used to resample images depending on the satellite
    if mode == "RB_MODE_CENTERED_PIXEL":
        # In centered case, origin does not move
        outOrigin = in_origin
        # --------------------------------------------------------------------------------------
    elif mode == "RB_MODE_UPPER_LEFT_CORNER_PIXEL":
        # In this case, the origin has to be shifted to preserve
        # footprint:
        # Formule CNES (J.Michel) et validee sur papier par CS
        outOrigin[0] = in_origin[0] - (factor[0] - 1) * in_spacing[0] / (2 * factor[0])
        outOrigin[1] = in_origin[1] - (factor[1] - 1) * in_spacing[1] / (2 * factor[1])
        # --------------------------------------------------------------------------------------
    elif mode == "NOT_MODE_INITIALIZED":
        # --------------------------------------------------------------------------------------
        pass
    else:
        raise MajaBusinessException("Unknown ResamplerHelper mode.")
    LOGGER.debug("                          - Output image origin:    [%s;%s]", outOrigin[0], outOrigin[1])
    return outOrigin
