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
orchestrator.cots.otb.otb_cots_manager -- shortdesc

orchestrator.cots.otb.otb_cots_manager is a description

It defines classes_and_methods

###################################################################################################
"""

from orchestrator.common.logger.maja_logging import configure_logger
import gc
LOGGER = configure_logger(__name__)


class OtbPipelineManager(object):
    """
    This class allow to manage launched apps in a pipeline.

    """

    def __init__(self):
        """
        Init to have constant values
        """
        self.otb_app_coarse_resolution = []
        
    def __del__(self):
        self.free_otb_app()

    def add_otb_app(self, otb_app):
        self.otb_app_coarse_resolution.append(otb_app)

    def free_otb_app(self):
        for otb_app in self.otb_app_coarse_resolution:
            if otb_app.get_app_name() is not None :
                LOGGER.debug("Removing "+otb_app.get_app_name())
            else:
                LOGGER.debug("Removing anonymous")
            del otb_app
        self.otb_app_coarse_resolution = []
        gc.collect()

    def get_last_app(self):
        return self.otb_app_coarse_resolution[-1]
