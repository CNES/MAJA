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


import os
from orchestrator.common.interfaces.maja_xml_input_angles import MajaInternalXmlInputAngles
from orchestrator.tests.settings import A_PRODUCT_S2_MUSCATE_L1
from orchestrator.common.muscate.muscate_xml_file_handler import MuscateXMLFileHandler
from orchestrator.common.muscate.muscate_xml_file_handler import MUSCATE_HANDLER_XPATH
from orchestrator.tests.maja_test_case import MajaTestCase


class TestMajaMuscateXmlFileHandler(MajaTestCase):
    """Test class for ClassUndertestName"""

    def setUp(self):
        self.xml_filename = A_PRODUCT_S2_MUSCATE_L1.get("main_xml_fullpath")
        self.assertTrue(os.path.isfile(self.xml_filename))
        self.output_filename = "/tmp/internal_angles_test.xml"

    def test_object(self):
        """
        Assert the initialization of the object do not fail
        """
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        self.assertIsNotNone(self.maja_object)

    def test_save_to_file(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        # self.maja_object.save_to_file(output_filename)

    def test_save_sun_angles(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        sun_angle_col_step = int(self.maja_object.get_string_value_of("SunAngleColStep"))
        sun_angle_row_step = -1 * int(self.maja_object.get_string_value_of("SunAngleRowStep"))
        # ATTENTION : * -1 for compatibility negative spacing

        l_zenithSolarAngles = self.maja_object.get_sun_zenithal_angles()
        l_azimuthSolarAngles = self.maja_object.get_sun_azimuthal_angles()
        LOGGER.debug(("ZenithSolarAngles nb values: " + str(len(l_zenithSolarAngles))))
        LOGGER.debug(("AzimuthSolarAngles nb values: " + str(len(l_azimuthSolarAngles))))
        LOGGER.debug(("Angles output_filename : %s", self.output_filename))
        writer = MajaInternalXmlInputAngles(l_zenithSolarAngles,
                                            l_azimuthSolarAngles,
                                            sun_angle_col_step, sun_angle_row_step, self.output_filename)
        writer.write()
        self.assertTrue(os.path.exists(self.output_filename), "File does not exist")

    def test_save_view_angles(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        view_angle_col_step = int(self.maja_object.get_string_value_of("SunAngleColStep"))
        view_angle_row_step = -1 * int(self.maja_object.get_string_value_of("SunAngleRowStep"))
        # ATTENTION : * -1 for compatibility negative spacing

        l_ListOfTOABandCode = self.maja_object.get_list_of_band_code()  # ListOfStrings
        for l_band in l_ListOfTOABandCode:
            # Get the list of detectors (index)
            l_ListOfZone = self.maja_object.get_list_of_zones(l_band)
            LOGGER.debug("List of zone : ")
            LOGGER.debug(l_ListOfZone)
            # Get the list of viewing angles in the header file of the tile
            l_zenithVieAngles = self.maja_object.get_viewing_zenithal_angles(l_band)
            l_azimuthVieAngles = self.maja_object.get_viewing_azimuthal_angles(l_band)

            if len(l_zenithVieAngles) != len(l_azimuthVieAngles):
                raise Exception(
                    "The number of detector for viewing zenithal angles and viewing azimuthal angles is different or null !")

            for detId in range(len(l_ListOfZone)):
                det = l_ListOfZone[detId]
                LOGGER.debug(("ZenithViewingAngles for band " + l_band + " and det " +
                       det + " ,  nb values: " + str(len(l_zenithVieAngles[detId]))))
                LOGGER.debug(l_zenithVieAngles[detId])
                LOGGER.debug(("AzimuthViewingAngles " + l_band + " and det " + det +
                       " nb values: " + str(len(l_azimuthVieAngles[detId]))))
                LOGGER.debug(l_azimuthVieAngles[detId])
                output_filename = "/tmp/internal_angles_test_{}_{}.xml".format(l_band, det)
                LOGGER.debug(("Angles output_filename : %s", output_filename))
                writer = MajaInternalXmlInputAngles(l_zenithVieAngles[detId],
                                                    l_azimuthVieAngles[detId],
                                                    view_angle_col_step, view_angle_row_step, output_filename)
                writer.write()
                self.assertTrue(os.path.exists(output_filename), "File does not exist " + output_filename)

            # writer = MajaInternalXmlInputAngles(l_zenithVieAngles,
            #                                    l_azimuthVieAngles,
            #                                    view_angle_col_step, view_angle_row_step, output_filename)
            # writer.write()
            #self.assertTrue(os.path.exists(output_filename), "File does not exist")

    def test_get_string_value_of_L1C_product(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        list_of_key_missing_in_L1C = [
            'SunTooLowMaskFilename',
            'DEMInformationMaximum',
            'ProcessingFlagsAndModesCAMSProcessing',
            'DEMInformationAverage',
            'DEMInformationMinimum',
            'AOT_Nodata_Value',
            'PrivateDirectoryFilename',
            'AerosolOpticalThicknessQuantificationValue',
            'VAP_Nodata_Value',
            'JobProcessingInformationsFileFilename',
            'AerosolOpticalThicknessQuantificationValueAsString',
            'ProcessingFlagsAndModesFileType',
            'WaterVaporContentQuantificationValueAsString',
            'DEMInformationStandardDeviation',
            'TopographyShadowMaskFilename',
            'WaterMaskFilename',
            'AOTInterpolationMaskFilename',
            'SnowMaskFilename',
            'Instrument',
            'EdgeMaskFilename',
            'ProcessingFlagsAndModesProcessingAdjacencyEffectsAndSlopeCorrection',
            'ProcessingFlagsAndModesProcessingMode',
            'HiddenSurfaceMaskFilename',
            'WaterVaporContentQuantificationValue',
            'ProcessingFlagsAndModesQualityCheck',
            'TangentSunMaskFilename',
            'ProcessingFlagsAndModesValidityFlag',
            'ProcessingFlagsAndModesCirrusCorrection']
        for k in list(MUSCATE_HANDLER_XPATH.keys()):
            if k not in list_of_key_missing_in_L1C:
                LOGGER.debug(self.maja_object.get_string_value_of(k))

    # Get the upper left corner in latitude / longitude coordinate system

    def test_get_upper_left_corner(self):
        # upperLeftCorner[0] = Long upperLeftCorner[1] = Lat
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_upper_left_corner())

    # Get the upper right corner in latitude / longitude coordinate system
    def test_get_upper_right_corner(self):
        # upperRightCorner[0] = Long upperRightCorner[1] = Lat
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_upper_right_corner())

    # Get the lower left corner in latitude / longitude coordinate system
    def test_get_lower_left_corner(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_lower_left_corner())

    # Get the lower right corner in latitude / longitude coordinate system
    def test_get_lower_right_corner(self):
        # lowerRightCorner[0] = Long lowerRightCorner[1] = Lat
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_lower_right_corner())

    # Get the center in latitude / longitude coordinate system
    def test_get_center(self):
        # lowerRightCorner[0] = Long lowerRightCorner[1] = Lat
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_center())

    # Get the mean solar angles
    def test_get_mean_solar_angles(self):
        # solAngles[0] = zenith solAngles[1] = azimuth
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_mean_solar_angles())

    # Get the acquisition date with the YYYYMMDD format
    def test_get_acquisition_date_formated_YYYYMMDD(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_acquisition_date_formated_yyyymmdd())

    # Get the acquisition date in UTC format

    def test_get_date_pdv_formated_UTC(self):
        # Date formated:
        # Case1: <PRODUCTION_DATE>2014-12-17T09:36:57.0</PRODUCTION_DATE>
        # or
        # Case2: <PRODUCTION_DATE>2014-12-17T09:36:57</PRODUCTION_DATE>
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_date_pdv_formated_utc())

    def test_get_mean_viewing_zenithal_angles(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_mean_viewing_zenithal_angles())

    def test_get_mean_viewing_azimuthal_angles(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_mean_viewing_azimuthal_angles())

    # Get the mean viewing angles
    # def test_get_mean_viewing_angles(self):
    #     # vieAngles[0] = zenith vieAngles[1] = azimuth
    #     self.maja_object = MuscateXMLFileHandler(self.xml_filename)
    #     print self.maja_object.get_mean_viewing_angles()


    # get the sun zenithal angles
    def test_get_sun_zenithal_angles(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_sun_zenithal_angles())

    # get the sun azimuthal angles
    def test_get_sun_azimuthal_angles(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_sun_azimuthal_angles())

    # get the viewing zenithal angles
    def test_get_viewing_zenithal_angles(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_viewing_zenithal_angles("B1"))

    # get the viewing azimuthal angles
    def test_get_viewing_azimuthal_angles(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_viewing_azimuthal_angles("B1"))

    # Get the list of bands
    def test_get_list_of_bands(self):
        # Get the number of bands
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_list_of_bands())

    # Get the saturated mask filename
    def test_get_list_of_L1_SAT_image_filenames(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_list_of_l1_sat_image_filenames())

    # Get the saturated mask index within its file
    def test_get_L1_SAT_image_index(self):
        # Get the list of bands
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_l1_sat_image_index(1))

    # Get the saturated mask filename
    def test_get_list_of_L1_NDT_image_filenames(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_list_of_l1_ndt_image_filenames())

    # Get the saturated mask index within its file
    def test_get_L1_NDT_image_index(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_l1_ndt_image_index(1))

    # Get the zone mask filename for the band
    def test_get_list_of_zones(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_list_of_zones("B1"))

    # Get the zone image index
    def test_get_L1_DTF_image_index(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_l1_dtf_image_index(1, "10"))

    # Get the list of name of the L1 TOA image
    def test_get_list_of_TOA_image_filenames(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_list_of_toa_image_filenames())

    # Get the list of name of the L1 TOA image
    def test_get_list_of_defective_pixel_image_filenames(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_list_of_defective_pixel_image_filenames())

    # Get the list of name of the L1 TOA image
    def test_get_map_list_of_detector_footprint_image_filenames(self):
        self.maja_object = MuscateXMLFileHandler(self.xml_filename)
        LOGGER.debug(self.maja_object.get_map_list_of_detector_footprint_image_filenames())
