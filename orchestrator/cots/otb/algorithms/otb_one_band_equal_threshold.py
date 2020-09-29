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
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler


def one_band_equal_value(image, output_image, threshold, equal_value=1, outside_value=0, write_output=True):
    """
    :param image:
    :param output_image:
    :param threshold:
    :param equal_value:
    :param outside_value
    :param write_output:
    :return:
    """
    parameters = {"im": image,
                  "thresholdvalue": threshold,
                  "equalvalue": equal_value,
                  "outsidevalue": outside_value,
                  "out": output_image}

    app = OtbAppHandler("OneBandEqualThreshold", parameters, write_output)

    return app
