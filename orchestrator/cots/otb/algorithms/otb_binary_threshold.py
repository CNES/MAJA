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
from orchestrator.cots.otb.algorithms.otb_band_math import band_math
from orchestrator.common.maja_exceptions import MajaProcessingError


def binary_threshold(input_image, inside_value, outside_value,
                     output_image, lower_threshold=None, upper_threshold=None,
                     write_output=True):
    """

    :param input_image:
    :param lower_threshold:
    :param upper_threshold:
    :param inside_value:
    :param outside_value:
    :param output_image:
    :param write_output:
    :return:
    """
    if lower_threshold is None and upper_threshold is None:
        raise MajaProcessingError("Error, no threshold given")

    if lower_threshold is None:
        expression = "im1b1<{up}?{in_v}:{out_v}".format(up=upper_threshold, in_v=inside_value, out_v=outside_value)
    elif upper_threshold is None:
        expression = "(im1b1>{low})?{in_v}:{out_v}".format(low=lower_threshold, in_v=inside_value, out_v=outside_value)
    else:
        expression = "(im1b1>{low}&&im1b1<{up})?{in_v}:{out_v}".format(low=lower_threshold, up=upper_threshold,
                                                                       in_v=inside_value, out_v=outside_value)

    out = band_math([input_image], expression, output_image=output_image, write_output=write_output)

    return out
