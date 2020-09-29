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

orchestrator.cots.otb.otb_band_math -- shortdesc

orchestrator.cots.otb.otb_band_math is a description

It defines classes_and_methods

###################################################################################################

:copyright: 2019 CNES. All rights reserved.
:license: license

###################################################################################################
"""


from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.common.logger.maja_logging import configure_logger

LOGGER = configure_logger(__name__)


def clean_pipe(input_file_path):

    parameters = {"in": input_file_path,
                "startx": 0,
                "starty": 0,
                "sizex": 1,
                "sizey": 1,
                "out": "tmp.tif"}
    otb_app = OtbAppHandler("ExtractROI",parameters,write_output=False)
    parameters2 = {"im": otb_app.getoutput().get("out")}
    otb_app2 = OtbAppHandler("Stats", parameters2, write_output=True)
    LOGGER.debug("Stats on clean: "+str(otb_app2.getoutput().get("mean")))
    del otb_app2
    del otb_app
