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
orchestrator.common.file_utils -- shortdesc

orchestrator.common.file_utils is a description

It defines classes_and_methods

###################################################################################################
"""
import os
from orchestrator.common.file_utils import uncompress_file
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler

LOGGER = configure_logger(__name__)


def get_gipp_filename_with_mission(dir_name, file_type, mission):
    list_of_gipp = get_list_of_gipp_filenames_with_mission(dir_name, file_type, mission)
    if len(list_of_gipp) > 1:
        raise MajaDataException("More than one GIPP of type " + file_type + " has been detected for the Mission <" +mission+">  in the input directory!")
    if len(list_of_gipp) < 1:
        raise MajaDataException("No GIPP of type " + file_type + " has been detected for the Mission <" +mission+">  in the input directory!")
    return list_of_gipp[0]


def get_gipp_filename(dir_name, file_type):
    list_of_gipp = get_list_of_gipp_filenames(dir_name, file_type)
    if len(list_of_gipp) > 1:
        raise MajaDataException("More than one GIPP of type " + file_type + " has been detected in the input directory")
    if len(list_of_gipp) < 1:
        raise MajaDataException("No GIPP of type " + file_type + " has been detected in the input directory!")
    return list_of_gipp[0]


def get_list_of_gipp_filenames_with_mission(dir_name, file_type, mission):
    list_of_gipp = get_list_of_gipp_filenames(dir_name, file_type)
    list_of_gipp_mission = []
    for gip in list_of_gipp:
        gip_xml = gip
        if not os.path.splitext(gip)[1] == ".EEF":
            gip_xml = os.path.splitext(gip)[0] + ".HDR"
        handler = EarthExplorerXMLFileHandler(gip_xml)
        gip_mis = handler.get_string_value_of("Mission")
        if gip_mis == mission:
            list_of_gipp_mission.append(gip)
    return list_of_gipp_mission


def get_list_of_gipp_filenames(dir_name, file_type, sort=False):
    list_of_file = [os.path.join(dir_name, file) for file in os.listdir(dir_name)]
    if sort:
        list_of_file.sort()
    return get_ref_hdr_or_eef_with_file_type(file_type, list_of_file)


def get_ref_hdr_or_eef_with_file_type(file_type, files):
    list_of_gipp = []
    for f in files:
        if (".EEF" in os.path.splitext(f) or ".HDR" in os.path.splitext(f)) and os.path.exists(f) \
                and file_type in f:
            list_of_gipp.append(f)
    return list_of_gipp


def uncompress_dbl_product(dbl_filename):
    no_ext = os.path.splitext(dbl_filename)[0]
    dbl_dir = os.path.realpath(no_ext + ".DBL.DIR")
    dbl_bin = os.path.realpath(no_ext + ".DBL.BIN")
    if os.path.isdir(dbl_dir):
        LOGGER.debug("The DBL product <" + dbl_dir + "> is already uncompressed (.DBL.DIR directory already exists).")
    elif os.path.isfile(dbl_bin):
        LOGGER.debug("The DBL product <" + dbl_bin + "> is already uncompressed (.DBL.BIN file already exists).")
    else:
        uncompress_file(dbl_filename, os.path.dirname(dbl_filename))


def get_listoffilenames_filetype_sorted(dir_name, file_type):
    list_of_gipp = get_list_of_gipp_filenames(dir_name, file_type)
    list_of_gipp_file_type = []
    for gip in list_of_gipp:
        gip_xml = gip
        if not os.path.splitext(gip)[1] == ".EEF":
            gip_xml = os.path.splitext(gip)[0] + ".HDR"
        handler = EarthExplorerXMLFileHandler(gip_xml)
        gip_type = handler.get_string_value_of("File_Type")
        if file_type in gip_type:
            list_of_gipp_file_type.append(gip)
    return list_of_gipp_file_type
