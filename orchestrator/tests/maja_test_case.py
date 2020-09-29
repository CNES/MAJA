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

orchestrator.tests.maja_test_case -- shortdesc

orchestrator.tests.maja_test_case is a description

It defines classes_and_methods

###################################################################################################
"""


import tempfile
import unittest


class MajaTestCase(unittest.TestCase):
    """Set working directory and epsilon for numeric comparison"""

    EPSILON = 1e-6

    def __init__(self, *args, **kwargs):

        super(MajaTestCase, self).__init__(*args, **kwargs)
        self.working_test_directory = tempfile.mkdtemp(prefix="maja_test")


if __name__ == "__main__":
    unittest.main()
