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

orchestrator.plugins.sentinel2.maja_sentinel2_l1_main_xml_reader -- shortdesc

orchestrator.plugins.sentinel2.maja_sentinel2_l1_main_xml_reader is a description

It defines classes_and_methods

###################################################################################################
"""

import os
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
import orchestrator.common.xml_tools as xml_tools
import orchestrator.common.file_utils as file_utils
from orchestrator.common.maja_exceptions import MajaDriverException,MajaExceptionPluginSentinel2
from orchestrator.plugins.sentinel2.maja_sentinel2_constants import LIST_OF_BAND_CODES, LIST_OF_L1BAND_CHARID

from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)

XPATH_MAIN_S2 = {"general_info": "//General_Info",
                 "granule_identifier": "//Product_Info/Product_Organisation/Granule_List/Granule/@granuleIdentifier",
                 "spacecraft_name": "//Product_Info/Datatake/SPACECRAFT_NAME",
                 "special_values": "//Product_Image_Characteristics/Special_Values",
                 "quantification_value": "//QUANTIFICATION_VALUE",
                 "file_class": "",
                 "generation_time": "//GENERATION_TIME",
                 "product_start_time": "//PRODUCT_START_TIME"}


class MajaSentinel2L1MainXmlReader(EarthExplorerXMLFileHandler):
    """
    classdocs
    """

    def __init__(self, main_xml_file, validate=False, schema_path=None, tile_id=None):
        """
        Constructor
        """
        super(MajaSentinel2L1MainXmlReader, self).__init__(main_xml_file, validate, schema_path)
        # ---------------------------------------------------------------------------------------------
        # Test if the input product is a SENTINEL one
        # Get the satellite name in the product header (as SENTINEL-2A)
        satRef ="SENTINEL"
        self.satellite_name = xml_tools.get_only_value(self.root, XPATH_MAIN_S2.get("spacecraft_name")).text
        LOGGER.debug("Satellite detected in metadata : "+self.satellite_name)
        element = str(self.satellite_name).upper().split('-')

        if element[0] != satRef:
            raise MajaExceptionPluginSentinel2("Sentinel2XMLFileHandler: The Header file "+main_xml_file +" is not a SENTINEL2 product !")
        self.product_path = os.path.dirname(main_xml_file)
        # Read the SchemaLocation and detect the PSD Version
        l_PsdIntegerVersion = self.get_psd_integer_version()
        LOGGER.debug("The PSD version '"+str(l_PsdIntegerVersion)+"' has been detected in the L1C product <"+main_xml_file+"'.")

        if l_PsdIntegerVersion < 14:
            if tile_id is None:
                raise MajaExceptionPluginSentinel2("Tile Id is mandatory with PSD < 14 products")
            self.TileId = tile_id

            # Get the tile directory (with the tile id)
            self.granule_id = xml_tools.get_attribute(self.root, "//Granules[contains(@granuleIdentifier,'" + tile_id + "')]","granuleIdentifier")
            LOGGER.debug("SENTINEL2 XML Handler : Granule ID = "+self.granule_id)

            # Set FileClass : TEST or OPER ...
            l_SplitGranuleId = self.granule_id.split('_')
            self.FileClass = l_SplitGranuleId[1]
            # Set AcquisitionPlatform : S2A or S2B ...
            self.AcquisitionPlatform = l_SplitGranuleId[0]
            self.quantification_value = float(
                xml_tools.get_only_value(
                    self.root,
                    XPATH_MAIN_S2.get("quantification_value"),
                    ).text)
            LOGGER.debug("Quantification : "+str(self.quantification_value))
            granulePath = self.product_path + "/GRANULE/" + self.granule_id

            # ---------------------------------------------------------------------------------------------
            # Get the xml tile file in the granuleId directory as "S2A_OPER_MTD_L1C_TL_MPS__20150507T132817_A000065_T14SLH.xml"
            l_listoffiles = [ os.path.join(granulePath,f) for f in os.listdir(granulePath) if os.path.isfile(os.path.join(granulePath,f))]
            nbFiles = len(l_listoffiles)
            isFound = False

            for f in l_listoffiles:
                # Detect the file type of the xml file only for user product SAFE formatted
                if "_MTD_L1C_TL_" in f and "xml" in os.path.splitext(f)[1]:
                    self.XmlTileFileName = f
                    isFound = True
            # ---------------------------------------------------------------------------------------------
            # If not found, throws an error
            if not isFound:
                raise MajaExceptionPluginSentinel2("Sentinel2XMLFileHandler: No xml header file was found for the tile <"+tile_id+">")

            # ---------------------------------------------------------------------------------------------
            # Get the list of image filename for all the tiles per band
            imagePath = granulePath + "/IMG_DATA/"
            imageName = ""
            # ---------------------------------------------------------------------------------------------
            # For all the bands, get image filename
            self._list_of_toa_images = []
            image = ""
            for band in LIST_OF_L1BAND_CHARID:
                path = "//Granule_List/Granules[@granuleIdentifier='" + self.granule_id + "']/IMAGE_ID[contains(.,'" + band + "')]"
                image = xml_tools.get_only_value(self.root, path, namespaces=self.nss).text
                self._list_of_toa_images.append(os.path.join(imagePath, image + ".jp2"))
                LOGGER.debug("Adding " + image + " to list of images")
        else:
            general_info_node = xml_tools.get_only_value(self.root, XPATH_MAIN_S2.get("general_info"), namespaces=self.nss)
            self.granule_id = xml_tools.get_only_value(
                general_info_node,
                XPATH_MAIN_S2.get("granule_identifier"),
                namespaces=self.nss)
            LOGGER.debug("GranuleId: "+self.granule_id)

            self.quantification_value = float(
                xml_tools.get_only_value(
                    general_info_node,
                    XPATH_MAIN_S2.get("quantification_value"),
                    namespaces=self.nss).text)

            LOGGER.debug("Quantification : " + str(self.quantification_value))

            self.product_path = os.path.dirname(main_xml_file)
            self._list_of_toa_images = []
            image = ""
            for band in LIST_OF_L1BAND_CHARID:
                path = "//Granule_List/Granule/IMAGE_FILE[contains(.,'" + band + "')]"
                image = xml_tools.get_only_value(self.root, path, namespaces=self.nss).text
                self._list_of_toa_images.append(os.path.join(self.product_path, image + ".jp2"))
                LOGGER.debug("Adding " + image + " to list of images")
            self.TileId = image.replace("GRANULE/L1C_", "").split("_")[0][1:]

            root_path = self.product_path + os.path.sep + "GRANULE" + os.path.sep
            list_of_subdir = [os.path.join(root_path, dir) for dir in os.listdir(root_path)]
            if len(list_of_subdir) != 1:
                raise MajaExceptionPluginSentinel2("Sentinel2XMLFileHandler: The GRANULE directory must contain only on" +
                                        " sub-directory (one tile)!")

            tilePath = list_of_subdir[0]
            list_of_files = [
                os.path.join(
                    tilePath,
                    f) for f in os.listdir(tilePath) if os.path.isfile(
                    os.path.join(
                        tilePath,
                        f))]
            xmlTileFilenames = [f for f in list_of_files if "MTD_TL" in f and os.path.splitext(f)[1] == ".xml"]
            if len(xmlTileFilenames) == 0:
                raise MajaExceptionPluginSentinel2("XMLTile filename not found")
            self.XmlTileFileName = xmlTileFilenames[0]

    @property
    def granule_xml_file(self):
        """

        :return: the path to the granule xml file
        :rtype: str
        """
        product_dir = os.path.abspath(os.path.dirname(self.main_xml_file))
        LOGGER.debug("produft dir %s", product_dir)
#         granule_dir = os.path.join(product_dir, "GRANULE", self.granule_id)
        # should work

        granule_dir_list = [os.path.join(product_dir,
                                         "GRANULE", x) for x in os.listdir(os.path.join(product_dir,
                                                                                        "GRANULE"))]

        # get granule dir
        LOGGER.debug('granule dir list %s', granule_dir_list)
        if len(granule_dir_list) == 1:
            self.granule_dir = granule_dir_list[0]
        elif len(granule_dir_list) == 0:
            raise MajaDriverException("No granule found in {}".format(product_dir))
        else:
            raise MajaDriverException("Several granules found in {}. The version of the product is "
                                      "correct ?".format(product_dir))
        LOGGER.debug(granule_dir_list)

        # get all xml in the granule folder
        granule_xml_list_result = [os.path.join(self.granule_dir, x) for x in
                                   os.listdir(self.granule_dir)
                                   if os.path.isfile(os.path.join(self.granule_dir, x))]
        LOGGER.debug(granule_xml_list_result)

        # get xml file
        if len(granule_xml_list_result) == 1:
            self.granule_xml = granule_xml_list_result[0]
        elif len(granule_xml_list_result) == 0:
            raise MajaDriverException("No granule found in {}".format(product_dir))
        else:
            raise MajaDriverException("Several xml file found "
                                      "in granule{}.".format(self.granule_dir))
        LOGGER.debug("self.granule_xml %s", self.granule_xml)

        return self.granule_xml

    def get_no_data(self):
        nodata = 0
        special_values = xml_tools.get_all_values(self.root, XPATH_MAIN_S2.get("special_values"))
        for values in special_values:
            if values.findtext("SPECIAL_VALUE_TEXT") == "NODATA":
                nodata = int(values.findtext("SPECIAL_VALUE_INDEX"))
                return nodata
        raise MajaDriverException("NODATA not found in the L1 Header file")

    def get_list_of_toa_images(self):
        return self._list_of_toa_images

    @staticmethod
    def get_list_of_band_codes():
        return LIST_OF_BAND_CODES

    def get_mission(self):
        return self.satellite_name.upper()

    def get_psd_integer_version(self):
        # Retrieve Schema
        l_SchemaLocation = xml_tools.get_schema_location_attrib(self.root)
        # SchemaLocation nomenclature :
        #   PSD 12 or 13: https://psd-12.sentinel2.eo.esa.int/PSD/User_Product_Level-1C.xsd S2_User_Product_Level-1C_Metadata.xsd
        #   PSD 14 :      https://psd-14.sentinel2.eo.esa.int/PSD/User_Product_Level-1C.xsd
        # find the numeric PSD version: 12 or 14 in these examples
        # declare references
        prefix = "https://psd-"
        postfix = ".sentinel2.eo.esa.int"
        # Insure we can find "prefix" in schemaLocation
        if prefix not in l_SchemaLocation:
            # else raise an exception
            raise MajaExceptionPluginSentinel2("The Sentinel2 xml file "+self.main_xml_file+" schemaLocation attribute is not conform (must be contain '"+prefix+"' value) !")
        # Insure we can find "postfix" in schemaLocation
        if not postfix in l_SchemaLocation:
            # else raise an exception
            raise MajaExceptionPluginSentinel2("The Sentinel2 xml file "+self.main_xml_file+" schemaLocation attribut is not conform (must be contain '"+postfix+"' value) !")
        # Then parse schemaLocation
        l_Tempo = l_SchemaLocation
        l_Tempo2 = str(l_Tempo).replace(prefix, "")
        l_ListOfStrings = str(l_Tempo2).split('.')
        # And find the version number
        try:
            int(l_ListOfStrings[0])
        except ValueError:# else raise an exception
            raise MajaExceptionPluginSentinel2("The Sentinel2 xml file "+self.main_xml_file+" schemaLocation attribute is not conform (impossible to detect the PSD version !")
        return int(l_ListOfStrings[0])

