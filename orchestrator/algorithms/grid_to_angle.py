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
import os
import math
from orchestrator.common.maja_exceptions import MajaProcessingError


def grid_to_angle(grid_point):
    """
    :param input_image:
    :param lower_threshold:
    :param upper_threshold:
    :param inside_value:
    :param outside_value:
    :return:
    """
    out1 = 0.
    out2 = 0.
    # Computes the dv values
    dv_x = grid_point[0]
    dv_y = grid_point[1]

    # Declares and init angles phi and theta
    phi = 0.
    theta = 0.

    # If dv_y "zero"
    if dv_y != 0.0:
        # Computes phi
        phi = math.atan(dv_x / dv_y)
        if dv_y < 0:
            phi = phi + math.pi
        if phi < 0.:
            phi = phi + 2 * math.pi
        # Computes theta
        theta = math.atan(dv_y / math.cos(phi))
        # Set the output pixel value
        out1 = theta
        out2 = phi
    # dv_y non zero
    else:
        if dv_x != 0.0:
            # Computes phi
            if dv_x > 0.:
                phi = math.pi * 2.0
            else:
                phi = -2.0 * math.pi
            # To keep phi between [0; 360]
            if phi < 0.:
                phi = phi + math.pi
            # Computes theta
            theta = math.atan(math.fabs(dv_x))
            # Set the output pixel value
            out1 = theta
            out2 = phi
        else:
            # Set the output pixel value to default value
            out1 = 0.
            out2 = 0.

    return (out1, out2)
