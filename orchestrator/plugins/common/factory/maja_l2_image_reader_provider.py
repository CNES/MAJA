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
from orchestrator.common.maja_exceptions import MajaBusinessException
from orchestrator.common.logger.maja_logging import configure_logger

LOGGER = configure_logger(__name__)


class L2ImageReaderProvider(object):

    @staticmethod
    def create(product_filename, app_handler, enable_reading_public):
        creators = FactoryBase.get_registered("L2ImageReaderBase")
        for crea in creators:
            LOGGER.info("Trying : " + crea.__name__)
            if crea().can_read(product_filename):
                l2reader = crea()
                l2reader.read_info(product_filename, enable_reading_public)
                l2reader.read(product_filename, app_handler, enable_reading_public)
                return l2reader
        raise MajaBusinessException("No factory to handle " + product_filename)

    @staticmethod
    def get_list_of_l2_products(directory):
        creators = FactoryBase.get_registered("L2ImageReaderBase")
        product_list = list()
        for crea in creators:
            LOGGER.info("Trying : " + crea.__name__)
            crea().detect_l2_products(directory, product_list)
        if product_list.__len__() == 0:
            raise MajaBusinessException("No input L2 product available")
        return product_list
