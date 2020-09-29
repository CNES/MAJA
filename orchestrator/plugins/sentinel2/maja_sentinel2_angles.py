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

orchestrator.plugins.sentinel2.maja_sentinel2_angles -- shortdesc

orchestrator.plugins.sentinel2.maja_sentinel2_angles is a description

It defines classes_and_methods

###################################################################################################
"""


import numpy as np
import os
from orchestrator.common.xml_tools import get_only_value
from orchestrator.common.maja_exceptions import MajaNotYetImplemented
from orchestrator.common.interfaces.maja_xml_input_angles import MajaInternalXmlInputAngles
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)

MEAN_SUN_ANGLES = "Mean_Sun_Angle"
MEAN_INCIDENCE_ANGLE = "Mean_Viewing_Incidence_Angle_List/Mean_Viewing_Incidence_Angle"
BAND_ID = "./@bandId"
DETECTOR_ID = "./@detectorId"
GRID_ZENITH = "Zenith/Values_List/VALUES/text()"
GRID_AZIMUTH = "Azimuth/Values_List/VALUES/text()"
ZENITH_ANGLE = "ZENITH_ANGLE/text()"
AZIMUTH_ANGLE = "AZIMUTH_ANGLE/text()"
SUN_ANGLE_GRID_NODE_NAME = "Sun_Angles_Grid"
SUN_ANGLES_GRID_ZENITH = SUN_ANGLE_GRID_NODE_NAME + "/" + GRID_ZENITH
SUN_ANGLES_GRID_AZIMUTH = SUN_ANGLE_GRID_NODE_NAME + "/" + GRID_AZIMUTH
VIEWING_INCIDENCE_ANGLE_GRIDS_NODE = "Viewing_Incidence_Angles_Grids"
COL_STEP = "*/COL_STEP/text()"
ROW_STEP = "*/ROW_STEP/text()"
STEP_UNIT = "*/COL_STEP/@unit"

# TODO: write a mother class to put write method and col_step and row_step values


class MajaSunAngles():

    def __init__(self, dom, sun_angle_node_name):
        self.col_step = None
        self.row_step = None
        self.step_unit = None
        self._read(dom, sun_angle_node_name)

    def _read(self, tile_angles_node, sun_angle_node_name):
        """
        Read the following information from the given xml node

            - mean of sun zenith and sun azimuth angle
            - the grid of values
            - the step in x and y

        """
        LOGGER.debug(tile_angles_node)
        sun_angles_mean_node = get_only_value(tile_angles_node, MEAN_SUN_ANGLES)
        LOGGER.debug(sun_angles_mean_node)
        self.sun_angles_mean = {"sun_zenith_angle": get_only_value(sun_angles_mean_node,
                                                                   ZENITH_ANGLE),
                                "sun_azimuth_angle": get_only_value(sun_angles_mean_node,
                                                                    AZIMUTH_ANGLE)}
        LOGGER.debug("Sun angles : %s", self.sun_angles_mean)

        SUN_ANGLES_GRID_ZENITH = sun_angle_node_name + "/" + GRID_ZENITH
        SUN_ANGLES_GRID_AZIMUTH = sun_angle_node_name + "/" + GRID_AZIMUTH

        self.zenith_angle_list_string_values = tile_angles_node.xpath(SUN_ANGLES_GRID_ZENITH)
        self.azimuth_angle_list_string_values = tile_angles_node.xpath(SUN_ANGLES_GRID_AZIMUTH)

        _sun_angles_grid_node = get_only_value(tile_angles_node, SUN_ANGLE_GRID_NODE_NAME, check=True)
        if _sun_angles_grid_node is not None:
            all_col_step = _sun_angles_grid_node.xpath(COL_STEP)
            all_row_step = _sun_angles_grid_node.xpath(ROW_STEP)
            all_step_unit = _sun_angles_grid_node.xpath(STEP_UNIT)
            LOGGER.debug("%s, %s", all_col_step, all_row_step)
            LOGGER.debug(set(all_row_step))

            if not len(set(all_col_step)) == 1:
                raise MajaNotYetImplemented("COL_STEP is different for Zenith than Azimuth %s" %
                                            all_col_step)

            if not len(set(all_row_step)) == 1:
                raise MajaNotYetImplemented("ROW_STEP is different for Zenith than Azimuth %s" %
                                            all_row_step)

            if not len(set(all_step_unit)) == 1:
                raise MajaNotYetImplemented("Step units are different for Zenith than Azimuth %s" %
                                            all_step_unit)

            self.col_step = all_col_step[0]
            self.row_step = all_row_step[0]
            self.step_unit = all_step_unit[0]

    def get_as_numpy_array(self):
        """
        Return the grid on angles value as a 2D numpy array
        """
        # compose a numpy array with all zenith angle list node
        list_of_zenith_angle_values = [x.split(" ") for x in self.zenith_angle_list_string_values]
        list_of_azimuth_angle_values = [x.split(" ") for x in self.azimuth_angle_list_string_values]

        return np.array(list_of_zenith_angle_values), np.array(list_of_azimuth_angle_values)

    def write(self, output_directory):
        """
        Write in a internal xml format file
        """
        output_filename = os.path.join(output_directory, "sun_angles.xml")
        LOGGER.debug("Angles output_filename : %s", output_filename)
        writer = MajaInternalXmlInputAngles(self.zenith_angle_list_string_values,
                                            self.azimuth_angle_list_string_values,
                                            self.col_step, self.row_step, output_filename)
        writer.write()
        return output_filename


class MajaViewingIncidenceAnglesByBandAndDetector():
    """
    Structure of a list of values for a couple (band, detector)
    """

    def __init__(self, node):
        self.band_id = str(get_only_value(node, BAND_ID))
        self.detector_id = str(get_only_value(node, DETECTOR_ID))
        LOGGER.debug("self.band id : %s:%s", self.band_id, type(self.band_id))
        LOGGER.debug("self.detectorid %s:%s", self.detector_id, type(self.detector_id))
        self.zenith_values = node.xpath(GRID_ZENITH)
        self.azimuth_values = node.xpath(GRID_AZIMUTH)

        # Get ROW_STEP and COL_STEP
        all_col_step = node.xpath(COL_STEP)
        all_row_step = node.xpath(ROW_STEP)
        all_step_unit = node.xpath(STEP_UNIT)
        LOGGER.debug("%s, %s", all_col_step, all_row_step)
        LOGGER.debug(set(all_row_step))

        if not len(set(all_col_step)) == 1:
            raise MajaNotYetImplemented("COL_STEP is different for Zenith than Azimuth %s" %
                                        all_col_step)

        if not len(set(all_row_step)) == 1:
            raise MajaNotYetImplemented("ROW_STEP is different for Zenith than Azimuth %s" %
                                        all_row_step)

        if not len(set(all_step_unit)) == 1:
            raise MajaNotYetImplemented("Step units are different for Zenith than Azimuth %s" %
                                        all_step_unit)

        self.col_step = all_col_step[0]
        self.row_step = all_row_step[0]
        self.step_unit = all_step_unit[0]

    def __repr__(self):
        return "Values for band id %s and detector %s" % (self.band_id, self.detector_id)

    def write(self, output_directory):
        """
        Write in a internal xml format file
        """
        output_filename = os.path.join(output_directory, "viewing_incidence_angle_b%s_d%s_" %
                                           (self.band_id, self.detector_id) + ".xml")
        LOGGER.debug("Angles output_filename : %s", output_filename)
        writer = MajaInternalXmlInputAngles(self.zenith_values, self.azimuth_values,
                                            self.col_step, self.row_step, output_filename)
        writer.write()
        return output_filename


class MajaViewingIncidenceAngles():

    def __init__(self, dom, list_viewing_angles_node=None):
        self.list_of_viewing_angles_grid = []
        self._extract_info(dom, list_viewing_angles_node)

    def _extract_info(self, tile_angles_node, list_viewing_angles_node=None):
        # If L2 level, tile_angles_node to search mean and list are not on the same level
        if list_viewing_angles_node is None:
            list_viewing_angles_node = tile_angles_node

        incidence_angle_list_node = tile_angles_node.xpath(MEAN_INCIDENCE_ANGLE)
        LOGGER.debug("incidence_angle_list_node %s", incidence_angle_list_node)

        self.incidence_angles_mean = \
            {get_only_value(x, BAND_ID):
             {"incidence_zenith_angle": get_only_value(x, ZENITH_ANGLE),
              "incidence_azimuth_angle": get_only_value(x, AZIMUTH_ANGLE)}
             for x in incidence_angle_list_node}
        LOGGER.debug("incidence_angles %s", self.incidence_angles_mean)

        self.list_of_viewing_angles_grid = []
        viewing_incidence_angles_grids_nodes = list_viewing_angles_node.xpath(VIEWING_INCIDENCE_ANGLE_GRIDS_NODE)
        LOGGER.debug(viewing_incidence_angles_grids_nodes)

        for node in viewing_incidence_angles_grids_nodes:
            self.list_of_viewing_angles_grid.append(MajaViewingIncidenceAnglesByBandAndDetector(node))
        LOGGER.debug("self.list_of_viewing_angles_grid %s", self.list_of_viewing_angles_grid)

    def get_viewing_angles(self, band):
        output = []
        for ang in self.list_of_viewing_angles_grid:
            if ang.band_id == str(band):
                output.append(ang)
        return output

    def get_viewing_angles_for_band_det(self, band, det):
        output = None
        for ang in self.list_of_viewing_angles_grid:
            if ang.band_id == str(band) and ang.detector_id == str(det):
                output = ang
                break
        return output

    def write(self, output_directory=None):
        """
        Write all internal xml files and return a dictionary with all output filepaths
        {(band_id, detector_id):output_filepath}

        """
        output_filenames = {}
        for grid in self.list_of_viewing_angles_grid:
            output_filename = grid.write(output_directory)
            output_filenames[(grid.band_id, grid.detector_id)] = output_filename
        assert len(output_filenames) == len(self.list_of_viewing_angles_grid)
        return output_filenames
