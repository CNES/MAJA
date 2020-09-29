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

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.interfaces.maja_xml_input -- shortdesc

orchestrator.common.interfaces.maja_xml_input is a description

It defines classes_and_methods

###################################################################################################
"""

from orchestrator.common.earth_explorer.gipp_lut_earch_explorer_xml_file_handler import \
    GippLUTEarthExplorerXMLFileHandler
import orchestrator.common.file_utils as file_utils
from orchestrator.common.maja_exceptions import MajaDataException
import os
import io

from orchestrator.common.interfaces.maja_xml_app_lut import *
from orchestrator.common.logger.maja_logging import configure_logger

LOGGER = configure_logger(__name__)


class LutConverter(object):

    def __init__(self, working_dir, gipp_filename, validate_schema=False, schema_path=None, use_rh=False, rh=None):
        self.lut_handler = GippLUTEarthExplorerXMLFileHandler(gipp_filename, validate=validate_schema,
                                                              schema_path=schema_path)
        self.original_gipp_filename = gipp_filename
        self.lut_indexes = self.lut_handler.get_lut_indexes()
        self.list_of_files = self.lut_handler.get_list_of_packaged_dbl_files(True, True)
        self.new_list_of_files = []
        self.new_gipp_filename = os.path.join(working_dir, os.path.basename(gipp_filename))
        new_dbldir_rel = os.path.splitext(os.path.basename(gipp_filename))[0] + ".DBL.DIR"
        self.new_dbldir = os.path.join(working_dir, new_dbldir_rel)
        file_utils.create_directory(self.new_dbldir)
        for f in self.list_of_files:
            if (use_rh and "RH" + str(int(rh)) in f) or not use_rh:
                LOGGER.debug("Copying " + f + " to " + self.new_dbldir)
                file_utils.copy_file_to_directory(f, self.new_dbldir)
                self.new_list_of_files.append(new_dbldir_rel + os.path.sep + os.path.basename(f))
                LOGGER.debug(self.new_list_of_files[-1])
        # Build indexes
        self.new_lut_indexes = IndexesType()
        for ind, vals in list(self.lut_indexes.items()):
            if ind == "Solar_Zenith_Angle_Indexes":
                self.new_lut_indexes.set_Solar_Zenith_Angle_Indexes(vals)
            elif ind == "Relative_Azimuth_Angle_Indexes":
                self.new_lut_indexes.set_Relative_Azimuth_Angle_Indexes(vals)
            elif ind == "Altitude_Indexes":
                self.new_lut_indexes.set_Altitude_Indexes(vals)
            elif ind == "AOT_Indexes":
                self.new_lut_indexes.set_AOT_Indexes(vals)
            elif ind == "TOA_Reflectance_Indexes":
                self.new_lut_indexes.set_TOA_Reflectance_Indexes(vals)
            elif ind == "Zenithal_Angle_Indexes":
                self.new_lut_indexes.set_Zenithal_Angle_Indexes(vals)
            elif ind == "View_Zenith_Angle_Indexes":
                self.new_lut_indexes.set_View_Zenith_Angle_Indexes(vals)
            elif ind == "Reflectance_Ratio_Indexes":
                self.new_lut_indexes.set_Reflectance_Ratio_Indexes(vals)
            else:
                raise MajaDataException("Unhandled lut index type : " + ind)
        # build files
        new_dbl = List_Of_FilesType()
        for f in self.new_list_of_files:
            new_dbl.add_Relative_File_Path(f)

        # build root node
        new_lut = LUT(self.new_lut_indexes, new_dbl)
        # build file
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        new_lut.export(output, 0, name_='LUT', namespacedef_='', pretty_print=True)
        with open(self.new_gipp_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
            LOGGER.info("Writed new gipp lut to " + self.new_gipp_filename)
