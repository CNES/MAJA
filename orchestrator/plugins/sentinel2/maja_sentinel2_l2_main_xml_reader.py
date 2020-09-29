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

orchestrator.plugins.sentinel2.maja_sentinel2_l2_main_xml_reader -- shortdesc

orchestrator.plugins.sentinel2.maja_sentinel2_l2_main_xml_reader is a description

It defines classes_and_methods

###################################################################################################
"""

from orchestrator.common.xml_tools import get_root_xml
from .maja_sentinel2_l2_angles import MajaL2Angles


class MajaSentinel2L2MainXmlReader(object):
    """
    classdocs
    """

    def __init__(self, main_xml_file):
        """
        Constructor
        """
        self.root = get_root_xml(main_xml_file, deannotate=True)
        self.angles = MajaL2Angles(self.root)
