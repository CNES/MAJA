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
from orchestrator.plugins.common.factory.maja_l1_image_info_provider import L1ImageInformationsProvider
from orchestrator.plugins.common.factory.maja_l2_image_reader_provider import L2ImageReaderProvider
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.common.date_utils import get_julianday_as_int, get_julianday_as_double
import operator

LOGGER = configure_logger(__name__)


def get_input_l1_image_product(dir, tile_id=None):
    LOGGER.info("Searching for L1 product in " + dir )
    list_of_product = get_list_of_input_l1_image_product_sorted(dir,tile_id=tile_id)
    if len(list_of_product) > 1:
        raise MajaDataException("There is more than one L1 image product in the input data.")
    return list_of_product[0]


def get_list_of_input_l1_image_product_sorted(dir,tile_id=None):
    list_of_product = L1ImageInformationsProvider.get_list_of_l1products(dir,tile_id=tile_id)
    LOGGER.info("Nb L1 product found : " + str(list_of_product.__len__()))
    if list_of_product.__len__() == 0:
        raise MajaDataException("No L1 product found")
    dictofproduct = {}
    for pr in list_of_product:
        l1product = L1ImageInformationsProvider.create(pr,tile_id=tile_id)
        l1jdate = get_julianday_as_double(l1product.ProductDate)
        dictofproduct[l1jdate] = l1product
    dictofproduct_sorted = sorted(list(dictofproduct.items()), key=operator.itemgetter(0))
    list_of_product_sorted = list()
    for prs in dictofproduct_sorted:
        list_of_product_sorted.append(prs[1])

    return list_of_product_sorted


def get_input_l2_image_product(dir, app_handler, enable_public_data):
    LOGGER.info("Searching for L2 product in " + dir)
    list_of_product = get_list_of_input_l2_image_product_sorted(dir, app_handler, enable_public_data)
    if len(list_of_product) > 1:
        raise MajaDataException("There is more than one L2 image product in the input data.")
    return list_of_product[0]


def get_list_of_input_l2_image_product_sorted(dir, app_handler, enable_public_data):
    list_of_product = L2ImageReaderProvider.get_list_of_l2_products(dir)
    LOGGER.info("Nb L2 product found : " + str(list_of_product.__len__()))
    if list_of_product.__len__() == 0:
        raise MajaDataException("No L2 product found")
    dictofproduct = {}
    for pr in list_of_product:
        l2product = L2ImageReaderProvider.create(pr, app_handler, enable_public_data)
        l2jdate = get_julianday_as_double(l2product.Date)
        dictofproduct[l2jdate] = l2product

    dictofproduct_sorted = sorted(list(dictofproduct.items()), key=operator.itemgetter(0))
    list_of_product_sorted = list()
    for prs in dictofproduct_sorted:
        list_of_product_sorted.append(prs[1])

    return list_of_product_sorted
