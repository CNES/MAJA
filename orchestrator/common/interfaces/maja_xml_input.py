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


from lxml import etree as ET

from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)


class MajaInternalXmlInput(object):
    """
    classdocs
    """

    def __init__(self, output_filename):
        """
        Constructor
        """
        if output_filename == "":
            self.only_print = True
        else:
            self.only_print = False
        self.output_filename = output_filename

    def write(self, root):
        """
        write method common to all internal interfaces
        """
        tree = ET.ElementTree(root)

        if self.only_print:
            LOGGER.info(ET.tostring(tree, pretty_print=True, xml_declaration=True, standalone='No',
                                    encoding="UTF-8").decode("utf-8"))
            return
        f = open(self.output_filename, "w")
        f.write(ET.tostring(tree, pretty_print=True, xml_declaration=True, standalone='No',
                            encoding="UTF-8").decode("utf-8"))
        f.close()
