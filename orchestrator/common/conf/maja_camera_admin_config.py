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
                        o  o  o o    o      o o    o     |||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################
"""
import orchestrator.common.conf.maja_xml_camera_admin_config as camera_admin_conf
from orchestrator.common.conf.maja_xml_camera_admin_config import *
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.common.logger.maja_logging import configure_logger

LOGGER = configure_logger(__name__)


class ConfigAdminCamera(object):

    def __init__(self, filename):

        self.ListOfBandsTheoreticalWavelength = {}
        self.conf = None
        try:
            self.conf = camera_admin_conf.parse(filename, True)
            LOGGER.debug("Start read the 'List_of_Band_Theoretical_Wavelength', 'count' attribute...")
            # Set the list of theoretical wavelenght
            l_Count = self.conf.get_Composite().get_List_of_Band_Theoretical_Wavelength().get_count()
            # Loop under the sk and read the value of the node
            list_of_bands = self.conf.get_Composite().get_List_of_Band_Theoretical_Wavelength().get_Band_Theoretical_Wavelength()
            l_nbvalues = len(list_of_bands)
            LOGGER.debug("Nb Sk values: " + str(l_nbvalues))
            # Check if the number of attribute conforms with the number of nodes
            if l_Count != l_nbvalues:
                MajaDataException(
                    "In the Configuration file <" +
                    filename +
                    ">, the 'count' attribute value (" +
                    str(l_Count) +
                    ") is different than the number of sub-node found '" +
                    str(l_nbvalues) +
                    "'!")

            # Get the list  of theoretical wavelength defined in the configuration file
            for wave in list_of_bands:
                self.ListOfBandsTheoreticalWavelength[wave.get_sk()] = wave.get_valueOf_()
                LOGGER.debug("Adding " + wave.get_sk() + " = " + str(wave.get_valueOf_()))
            LOGGER.debug("Load the ConfigAdminCamera file done.")

        except Exception as e:
            raise MajaDataException(
                "Cannot create ConfigAdminCamera Instance with the following input filename <" +
                filename +
                "> !: " +
                e.message)

    def get_theoretical_wavelength_from_band_code_id(self, band_code):
        return self.ListOfBandsTheoreticalWavelength.get(band_code)

    def get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(self, band_code, bandwavedm1):
        # ---------------------------------------------------------------------------------------------
        l_theoreticalwavelength_dated = self.ListOfBandsTheoreticalWavelength.get(band_code)

        # ---------------------------------------------------------------------------------------------
        # Get the list  of         the Band(TheoreticalWavelength) in the L2 input  composite image
        l_nblistofbandtheoreticalwavelengthdatedm1 = len(bandwavedm1)
        # ---------------------------------------------------------------------------------------------
        # Find the Index in the Date Dm1 list
        for i in range(0, l_nblistofbandtheoreticalwavelengthdatedm1):
            if (int(bandwavedm1[i]) == int(l_theoreticalwavelength_dated)):
                return i

        # ---------------------------------------------------------------------------------------------
        # If not found, throw an  exception
        msg = "Error while checking the corresponding band from the current date D to the previous date D-1.\n" + \
              "The band code '" + band_code + "' (with the theoretical wavelength '" + str(l_theoreticalwavelength_dated) + \
              "') is not found in the composite of the L2 input product. The theoretical wavelength of the L2 input composite are (" + \
              str(l_nblistofbandtheoreticalwavelengthdatedm1) + "): "
        # ---------------------------------------------------------------------------------------------
        for band in bandwavedm1:
            msg = msg + band + " "
        # ---------------------------------------------------------------------------------------------
        raise MajaDataException(msg)
