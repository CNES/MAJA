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

from orchestrator.common.earth_explorer.gipp_smac_earch_explorer_xml_file_handler import \
    GippSMACEarthExplorerXMLFileHandler
import os
import io

from orchestrator.common.interfaces.maja_xml_app_smac import *
from orchestrator.common.logger.maja_logging import configure_logger

LOGGER = configure_logger(__name__)


class SmacConverter(object):

    def __init__(self, working_dir, gipp_filename, validate_schema=False, schema_path=None):
        self.smac_handler = GippSMACEarthExplorerXMLFileHandler(gipp_filename, validate_schema=validate_schema,
                                                                schema_path=schema_path)
        self.original_gipp_filename = gipp_filename
        self.NumberOfBands = self.smac_handler.NumberOfBands
        self.Coefficients = self.smac_handler.Coefficients
        LOGGER.debug(("SMAC Coeff : " + self.Coefficients))
        self.new_gipp_filename = os.path.join(working_dir, os.path.basename(gipp_filename))
        # build root node
        new_smac = SMAC()
        new_smac.set_SMAC_Coefficients(self.Coefficients)
        new_smac.set_SMAC_NumberOfBands(self.NumberOfBands)
        # build file
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        new_smac.export(output, 0, name_='SMAC', namespacedef_='', pretty_print=True)
        with open(self.new_gipp_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
            LOGGER.info("Writed new gipp smac to " + self.new_gipp_filename)
