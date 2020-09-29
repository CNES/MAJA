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

orchestrator.common.interfaces.maja_xml_input_angles -- shortdesc

orchestrator.common.interfaces.maja_xml_input_angles is a description

It defines classes_and_methods

###################################################################################################
"""

from lxml import etree as ET

from .maja_xml_input import MajaInternalXmlInput


class MajaInternalXmlInputAngles(MajaInternalXmlInput):
    """
    classdocs
    """

    def __init__(self, list_of_zenital_angles, list_of_azimuth_angles,
                 col_step, row_step, output_filename=""):
        super(MajaInternalXmlInputAngles, self).__init__(output_filename)

        self.list_of_zenital_angles = list_of_zenital_angles
        self.list_of_azimuth_angles = list_of_azimuth_angles
        self.col_step = col_step
        self.row_step = row_step

    def write(self):
        """
        Generates a xml for angles in an internal maja format
        """

        root = ET.Element("ANGLE")

        root.append(self.zenital_angle_node(self.list_of_zenital_angles))
        root.append(self.azimuth_angle_node(self.list_of_azimuth_angles))

        # TODO: the nodes COL_STEP and ROW_STEP does not contain the unit contrary to
        # the granule xml or given interface xml
        if self.col_step is not None:
            subelt = ET.SubElement(root, 'COL_STEP')
            subelt.text = str(self.col_step)

        if self.row_step is not None:
            subelt = ET.SubElement(root, 'ROW_STEP')
            subelt.text = str(self.row_step)

        super(MajaInternalXmlInputAngles, self).write(root)

    def zenital_angle_node(self, list_of_zenital_angles):
        """
        Add zenital angle node
        <Zenithal_Angle_List>
            <VALUES>5.33861 5.71312 6.09205 NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN</VALUES>
            <VALUES>5.41446 5.79039 NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN</VALUES>
            <VALUES>5.4907 5.86668 NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN</VALUES>
            <VALUES>5.56822 5.94363 NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN</VALUES>
            ...
        </Zenithal_Angle_List>
        """

        zenital_angle_root = ET.Element('Zenithal_Angle_List')

        for line in list_of_zenital_angles:
            subelt = ET.SubElement(zenital_angle_root, 'VALUES')
            subelt.text = str(line)
        return zenital_angle_root

    def azimuth_angle_node(self, list_of_azimuth_angles):
        """
        Add azimuth angle node
        <Azimuth_Angle_List>
            <VALUES>44.3323 44.2683 44.2042 44.14 44.0758 44.0114 43.947 43.8825 43.8179 43.7533 43.6885 43.6237 43.5588 43.4938 43.4287 43.3635 43.2983 43.2329 43.1675 43.102 43.0364 42.9707 42.905</VALUES>
            <VALUES>44.2871 44.2231 44.159 44.0948 44.0305 43.9661 43.9017 43.8372 43.7726 43.7079 43.6431 43.5782 43.5133 43.4483 43.3832 43.318 43.2527 43.1873 43.1219 43.0564 42.9907 42.925 42.8593</VALUES>
            <VALUES>44.242 44.178 44.1138 44.0496 43.9853 43.9209 43.8565 43.7919 43.7273 43.6626 43.5978 43.5329 43.4679 43.4029 43.3377 43.2725 43.2072 43.1418 43.0763 43.0108 42.9452 42.8794 42.8136</VALUES>
            <VALUES>44.197 44.133 44.0688 44.0046 43.9402 43.8758 43.8113 43.7467 43.6821 43.6173 43.5525 43.4876 43.4226 43.3575 43.2924 43.2271 43.1618 43.0964 43.0309 42.9653 42.8997 42.8339 42.7681</VALUES>
            ...
        </Azimuth_Angle_List>
        """

        azimuth_angle_root = ET.Element('Azimuth_Angle_List')

        for line in list_of_azimuth_angles:
            subelt = ET.SubElement(azimuth_angle_root, 'VALUES')
            subelt.text = str(line)

        return azimuth_angle_root
