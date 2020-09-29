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

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################
"""


from orchestrator.common.earth_explorer.earth_explorer_utilities import EarthExplorerUtilities
from orchestrator.tests.maja_test_case import MajaTestCase


class TestMajaEarthExplorerUtilities(MajaTestCase):
    """Test class for ClassUndertestName"""

    def setUp(self):
        pass

    def test_object(self):
        """
        Assert the initialization of the object do not fail
        """
        self.maja_oject = EarthExplorerUtilities()
        self.assertIsNotNone(self.maja_oject)

    def test_some_values(self):
        self.maja_oject = EarthExplorerUtilities()
        """
        Assertion on the extracted angles
        """
        l_Filename = "S2__TEST_GIP_L2SITE_S_31TJF____00001_20150703_21000101.EEF"
        LOGGER.debug(("------ Test the filename '" + l_Filename + "' :"))
        self.assertEqual(EarthExplorerUtilities.get_satellite_class_from_filename(l_Filename), "S2_")
        self.assertEqual(EarthExplorerUtilities.get_fileclass_from_filename(l_Filename), "TEST")
        self.assertEqual(EarthExplorerUtilities.get_file_type_short_from_filename(l_Filename), "GIP")
        self.assertEqual(EarthExplorerUtilities.get_level_class_from_filename(l_Filename), "L2SITE")
        self.assertEqual(EarthExplorerUtilities.get_file_type_from_filename(l_Filename), "GIP_L2SITE")
        self.assertEqual(EarthExplorerUtilities.get_nature_from_filename(l_Filename), "GIP_L2SITE")
        self.assertEqual(EarthExplorerUtilities.get_nickname_from_gipp_filename(l_Filename), "31TJF___")

        l_Filename = "L__VM0x_VSC_PDTANX_L2VALD__P_IS____20120101_ATB_R1.HDR"
        LOGGER.debug(("------ Test the filename '" + l_Filename + "' :"))
        self.assertEqual(EarthExplorerUtilities.get_satellite_class_from_filename(l_Filename), "L_")
        self.assertEqual(EarthExplorerUtilities.get_fileclass_from_filename(l_Filename), "VM0x")
        self.assertEqual(EarthExplorerUtilities.get_file_type_short_from_filename(l_Filename), "VSC")
        self.assertEqual(EarthExplorerUtilities.get_level_class_from_filename(l_Filename), "PDTANX")
        self.assertEqual(EarthExplorerUtilities.get_file_type_from_filename(l_Filename), "VSC_PDTANX")
        self.assertEqual(EarthExplorerUtilities.get_nature_from_filename(l_Filename), "VSC_PDTANX")
        self.assertEqual(EarthExplorerUtilities.get_nickname_from_annex_filename(l_Filename), "_P_IS___")

        l_Filename = "/toto/titi/S2A_TEST_SSC_L1VALD__PE_R_U__20120101.HDR"
        LOGGER.debug(("------ Test the filename '" + l_Filename + "' :"))
        self.assertEqual(EarthExplorerUtilities.get_satellite_class_from_filename(l_Filename), "S2A")
        self.assertEqual(EarthExplorerUtilities.get_fileclass_from_filename(l_Filename), "TEST")
        self.assertEqual(EarthExplorerUtilities.get_file_type_short_from_filename(l_Filename), "SSC")
        self.assertEqual(EarthExplorerUtilities.get_level_class_from_filename(l_Filename), "L1VALD")
        self.assertEqual(EarthExplorerUtilities.get_file_type_from_filename(l_Filename), "SSC_L1VALD")
        self.assertEqual(EarthExplorerUtilities.get_nature_from_filename(l_Filename), "SSC_L1VALD")
        self.assertEqual(EarthExplorerUtilities.get_nickname_from_global_filename(l_Filename), "_PE_R_U_")
        self.assertEqual(EarthExplorerUtilities.get_acquisition_from_global_filename(l_Filename), "20120101")
