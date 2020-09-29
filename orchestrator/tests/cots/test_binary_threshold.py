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

orchestrator.tests.algorithms.test_binary_threshold -- shortdesc

orchestrator.tests.algorithms.test_binary_threshold is a description

It defines classes_and_methods


###################################################################################################
"""

import unittest

from ..settings import QB_1_ORTHO_EXTRACT
from ..maja_test_case import MajaTestCase
from orchestrator.cots.otb.algorithms.otb_binary_threshold import binary_threshold
from orchestrator.common.directory_manager import get_working_file


class TestBinaryThreshold(MajaTestCase):
    """Test class for ClassUndertestName"""

    def setUp(self):
        self.image = QB_1_ORTHO_EXTRACT.get("image_fullpath", None)

    def test_algo(self):
        """
        Test the initialization of the object
        """
        binary_threshold(input_image=self.image,
                         lower_threshold=25,
                         upper_threshold=100,
                         inside_value=1,
                         outside_value=0,
                         output_image=get_working_file("TestAlgoBinaryThreshold",
                                                       working_directory=self.working_test_directory))


if __name__ == "__main__":
    unittest.main()
