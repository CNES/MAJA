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
#
#                        o     o
#                        oo   oo   oo        o   oo        ,-.
#                        o o o o  o  o       o  o  o       \_/
#                        o  o  o o    o      o o    o     {|||)<
#                        o     o oooooo      o oooooo      / \
#                        o     o o    o o    o o    o      `-^
#                        o     o o    o  oooo  o    o
#
###################################################################################################

orchestrator.plugins.maja_dataset -- shortdesc

orchestrator.plugins.maja_dataset is a description

It defines classes_and_methods

###################################################################################################
"""
from orchestrator.common.maja_exceptions import MajaDriverException
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)


class SingleResolutionImageInfo:

    def __init__(self):
        self.Resolution = 0
        self.ListOfBandCode = []


class BandsDefinitions(object):

    def __init__(self):
        self.L1ListOfBandsMap = {}
        self.L1BandMap = {}
        self.L2CoarseBandMap = {}
        self.ListOfL1Resolution = []
        self.ListOfL2Resolution = []
        self.ReferenceResolution = ""
        self.ImagesInformationMap = {}
        self.DetectorMap = []

    def get_band_id_in_l2_coarse(self, bandcode):
        if bandcode in self.L2CoarseBandMap:
            return self.L2CoarseBandMap[bandcode]
        else:
            raise MajaDriverException("The code '" + bandcode + "' is not present in L2Coarse !")

    def get_band_id_in_l1(self, bandcode):
        if bandcode in self.L1BandMap:
            return self.L1BandMap[bandcode]
        else:
            raise MajaDriverException("The code '" + bandcode + "' is not present in L1 Bands !")

    def get_list_of_band_id_in_l2_coarse(self):
        # ---------------------------------------------------------------------------------------------
        l_listofl2coarsebandid = []
        # ---------------------------------------------------------------------------------------------
        # Loops on the bands
        for b in list(self.L2CoarseBandMap.values()):
            l_listofl2coarsebandid.append(b)
        return [str(e) for e in sorted(l_listofl2coarsebandid)]

    def get_l1_resolution_for_band_code(self, bandcode):
        for res in list(self.ImagesInformationMap.keys()):
            l_listofband = self.get_list_of_l1_band_code(res)
            if bandcode in l_listofband:
                return res
        raise MajaDriverException("Band " + bandcode + " Not found in internal map")

    def get_list_of_band_code_in_l2_coarse_sorted_by_index(self):
        listofkey = []
        l_numberofband = len(self.L2CoarseBandMap)
        # Loops on the bands
        for i in range(0, l_numberofband):
            found = False
            # Loops on the band map at L2Coarse
            for first, second in list(self.L2CoarseBandMap.items()):
                if second == i:
                    found = True
                    listofkey.append(first)
            # If not found, throws an exception
            if not found:
                raise MajaDriverException(
                    "Internal error in the GetListOfBandCodeInL2CoarseSortedByIndex method: the index '" +
                    str(id) +
                    "' has not be found !")
        # CHeck the size of the list created
        if len(listofkey) != l_numberofband:
            raise MajaDriverException("Internal error in the GetListOfBandCodeInL2CoarseSortedByIndex method !")
        return listofkey

    def get_list_of_l2_band_code(self, res):
        if res in self.ImagesInformationMap:
            return self.ImagesInformationMap.get(res).ListOfBandCode
        else:
            raise MajaDriverException("The resolution '" + res + "' is not defined !")

    def get_list_of_l1_band_code(self, res):
        if res in self.L1ListOfBandsMap:
            return self.L1ListOfBandsMap.get(res)
        else:
            raise MajaDriverException("The resolution " + res + " is not defined in L1BandMap")

    def get_l1_resolution(self, res):
        if res in self.ImagesInformationMap:
            return self.ImagesInformationMap.get(res).Resolution
        else:
            raise MajaDriverException("The resolution '" + res + "' is not defined !")

    def get_list_of_l2_coarse_band_id_associated_to_l2_band_code(self, listBandCode):
        result = []
        for band in listBandCode:
            result.append(self.get_band_id_in_l2_coarse(band))
        return result

    def get_list_of_l2_band_id(self, res):
        band_code_list = self.get_list_of_l2_band_code(res)
        return self.get_list_of_l2_coarse_band_id_associated_to_l2_band_code(band_code_list)

    def get_l2_information_for_quicklook_band_code(self, redBandCode, greenBandCode, blueBandCode):
        # --------------------------------------
        # Get the number of resolution
        l_NbResolutions = len(self.ListOfL2Resolution)
        # --------------------------------------
        # Declares RGB variables
        l_RedResolution = 0
        l_GreenResolution = 0
        l_BlueResolution = 0

        # --------------------------------------
        # Declares and init boolean variables
        l_RedBandFound = False
        l_GreenBandFound = False
        l_BlueBandFound = False
        redBandId = 0
        greenBandId = 0
        blueBandId = 0
        # --------------------------------------
        # Loop under resolutions
        for l_resol in range(0, l_NbResolutions):
            # --------------------------------------
            # Get the number of band code
            l_ListOfBandCode = self.get_list_of_l2_band_code(self.ListOfL2Resolution[l_resol])
            LOGGER.debug(l_ListOfBandCode)
            l_NbListOfBandCode = len(l_ListOfBandCode)
            # --------------------------------------
            # Loop under bands codes
            for l_band in range(0, l_NbListOfBandCode):
                # --------------------------------------
                # Init the red resolution and index
                if redBandCode == l_ListOfBandCode[l_band]:
                    l_RedResolution = l_resol
                    redBandId = l_band
                    l_RedBandFound = True
                    LOGGER.debug(redBandId)
                # --------------------------------------
                # Init the green resolution and index
                if greenBandCode == l_ListOfBandCode[l_band]:
                    l_GreenResolution = l_resol
                    greenBandId = l_band
                    LOGGER.debug(greenBandId)
                    l_GreenBandFound = True
                # --------------------------------------
                # Init the blue resolution and index
                if blueBandCode == l_ListOfBandCode[l_band]:
                    l_BlueResolution = l_resol
                    blueBandId = l_band
                    LOGGER.debug(blueBandId)
                    l_BlueBandFound = True

        # --------------------------------------
        # Check the validity of the red band
        if not l_RedBandFound:
            raise MajaDriverException("The Red band value '" + redBandCode + "' is incorrect !")
        # --------------------------------------
        # Check the validity of the green band
        if not l_GreenBandFound:
            raise MajaDriverException("The Green band value '" + greenBandCode + "' is incorrect !")
        # --------------------------------------
        # Check the validity of the blue band
        if not l_BlueBandFound:
            raise MajaDriverException("The Blue band value '" + blueBandCode + "' is incorrect !")
        # --------------------------------------
        # Check the compatibility of the resolution
        if l_RedResolution != l_GreenResolution or l_RedResolution != l_BlueResolution:
            raise MajaDriverException(
                "The resolutions for the three Quicklook bands [" +
                redBandCode +
                "," +
                greenBandCode +
                "," +
                blueBandCode +
                "] are not identical !")
        strResolution = self.ListOfL2Resolution[l_RedResolution]
        IdResolution = l_RedResolution

        return redBandId, greenBandId, blueBandId
