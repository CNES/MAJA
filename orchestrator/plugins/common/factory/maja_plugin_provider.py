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

orchestrator.plugins.maja_dataset -- shortdesc

orchestrator.plugins.maja_dataset is a description

It defines classes_and_methods

###################################################################################################
"""
from orchestrator.plugins.common.factory.factory_base import FactoryBase
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaExceptionPluginBase

LOGGER = configure_logger(__name__)


class MAJAPluginProvider(object):

    @staticmethod
    def create(plugin_name, app_handler):
        creators = FactoryBase.get_registered("PluginBase")
        for crea in creators:
            LOGGER.info("Trying : " + crea.__name__ + " for plugin " + plugin_name)
            if crea().is_valid(plugin_name):
                plugin = crea()
                plugin.initialize(app_handler)
                return plugin
        raise MajaExceptionPluginBase("No factory to create plugin for " + plugin_name)

    @staticmethod
    def create_with_unique_sat(sat_name, app_handler):
        creators = FactoryBase.get_registered("PluginBase")
        for crea in creators:
            LOGGER.info("Trying : " + crea.__name__ + " for satellite " + sat_name)
            if crea().is_valid_with_unique_satellite(sat_name):
                plugin = crea()
                plugin.initialize(app_handler)
                return plugin
        raise MajaExceptionPluginBase("No factory to create plugin for sat " + sat_name)

    @staticmethod
    def auto_tm(plugin_name):
        if not "MUSCATE" in plugin_name:
            return plugin_name + "_MUSCATE"
        return plugin_name
