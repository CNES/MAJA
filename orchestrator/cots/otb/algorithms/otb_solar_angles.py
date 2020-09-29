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

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################
orchestrator.cots.otb.otb_solar_angles -- shortdesc

orchestrator.cots.otb.otb_solar_angles is a description

It defines classes_and_methods


###################################################################################################
"""

from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaNotYetImplemented

LOGGER = configure_logger(__name__)


def solar_angle(solar_angle_image, sol1_height, sol_href, output_image, sol2_in=None, sol2_height=None,
                write_output=True):
    # test about input and raise MajaNotYetImplemented is case of sol2 case
    if sol2_in is not None or sol2_height is not None:
        raise MajaNotYetImplemented()

    parameters = {"sol1.in": solar_angle_image,
                  "sol1.h": sol1_height,
                  "solhref": sol_href,
                  "sol": output_image}

    app = OtbAppHandler("SolarAngle", parameters, write_output)

    return app
