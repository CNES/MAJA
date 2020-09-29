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

orchestrator.tests.plugins.test_maja_sentinel2_l2_angles -- shortdesc

orchestrator.tests.plugins.test_maja_sentinel2_l2_angles is a description

It defines classes_and_methods


###################################################################################################
"""


import os
import unittest

from orchestrator.common.xml_tools import get_root_xml
from orchestrator.tests.settings import A_PRODUCT_S2_L2
from orchestrator.plugins.sentinel2.maja_sentinel2_l2_angles import MajaL2Angles
from ..maja_test_case import MajaTestCase


class TestMajaSentinel2L2Angles(MajaTestCase):
    """Test class for ClassUndertestName"""

    def setUp(self):
        self.main_xml = A_PRODUCT_S2_L2.get("main_xml_fullpath")
        self.root = get_root_xml(self.main_xml, deannotate=True)
        self.assertTrue(os.path.isfile(self.main_xml))

    def test_object(self):
        """
        Assert the initialization of the object do not fail
        """
        self.maja_oject = MajaL2Angles(self.root)
        self.assertIsNotNone(self.maja_oject)

    def test_sun_angles(self):
        """
        Assertion on the extracted angles
        """
        self.maja_oject = MajaL2Angles(self.root)
        sun_angles_object = self.maja_oject.sun_angles

        self.assertEqual(sun_angles_object.sun_angles_mean.get("sun_zenith_angle"),
                         A_PRODUCT_S2_L2.get("product_solar_angles").get("sun_zenith_angle"))
        self.assertEqual(sun_angles_object.sun_angles_mean.get("sun_azimuth_angle"),
                         A_PRODUCT_S2_L2.get("product_solar_angles").get("sun_azimuth_angle"))

        self.assertEqual(len(sun_angles_object.zenith_angle_list_string_values), 23)

#     def test_sun_angles_as_np_array(self):
#         self.assertEqual(len(sun_angles_object.sun_angles_zenith[0]), 23)

    def test_viewing_angles(self):
        """
        Assertion on the extracted angles
        """
        self.maja_oject = MajaL2Angles(self.root)
        viewing_angles_object = self.maja_oject.viewing_incidence_angle

        maja_incidence_angles_mean = viewing_angles_object.incidence_angles_mean

        self.assertEqual(maja_incidence_angles_mean.get("1").get("incidence_zenith_angle"),
                         A_PRODUCT_S2_L2.get("product_incidence_angle_zenith_band_1"))
        self.assertEqual(maja_incidence_angles_mean.get("7").get("incidence_azimuth_angle"),
                         A_PRODUCT_S2_L2.get("product_incidence_angle_azimuth_band_7"))

        self.assertEqual(len(viewing_angles_object.list_of_viewing_angles_grid), 52)

    def test_sun_angles_write(self):
        """
        Test of the write method for sun angles
        """
        self.maja_oject = MajaL2Angles(self.root)
        sun_angles_object = self.maja_oject.sun_angles

        internal_xml = sun_angles_object.write(self.working_test_directory)

        self.assertTrue(os.path.exists(internal_xml))

    def test_viewing_azimuth_angles_write_one_grid(self):
        """
        Test of the write method for one grid of viewing angles
        """
        self.maja_oject = MajaL2Angles(self.root)
        viewing_angles_object = self.maja_oject.viewing_incidence_angle

        self.assertTrue(len(viewing_angles_object.list_of_viewing_angles_grid) > 0)
        one_band_detector = viewing_angles_object.list_of_viewing_angles_grid[0]

        internal_xml = one_band_detector.write(self.working_test_directory)

        self.assertTrue(os.path.exists(internal_xml))

    def test_viewing_azimuth_angles_write(self):
        """
        Test of the write of all viewing angles
        """
        self.maja_oject = MajaL2Angles(self.root)
        viewing_angles_object = self.maja_oject.viewing_incidence_angle

        internal_xmls = viewing_angles_object.write(self.working_test_directory)
        LOGGER.debug(internal_xmls)

        for xml_file in list(internal_xmls.values()):
            self.assertTrue(os.path.exists(os.path.realpath(xml_file)),
                            "%s does not exist" % xml_file)


if __name__ == "__main__":
    unittest.main()
