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

orchestrator.plugins.sentinel2.maja_sentinel2_l1_granule_xml_reader --

orchestrator.plugins.sentinel2.maja_sentinel2_l1_granule_xml_reader is a description

It defines classes_and_methods

###################################################################################################
"""


import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
from orchestrator.plugins.sentinel2.maja_sentinel2_constants import LIST_OF_L1BAND_CHARID
from .maja_sentinel2_l1_angles import MajaL1Angles
import orchestrator.common.date_utils as date_utils
from orchestrator.common.maja_common import *
from orchestrator.common.maja_exceptions import *
import os
LOGGER = configure_logger(__name__)

XPATH_TILE_S2 = {"sensing_time": "//SENSING_TIME",
                 "datastrip_id": "//DATASTRIP_ID",
                 "reference_code": "//HORIZONTAL_CS_CODE",
                 "horizontal_cs_name": "//HORIZONTAL_CS_NAME"
                 }


class MajaSentinel2L1GranuleXmlReader(EarthExplorerXMLFileHandler):
    """
    classdocs
    """

    def __init__(self, granule_xml_file):
        """
        Constructor
        """
        super(MajaSentinel2L1GranuleXmlReader, self).__init__(granule_xml_file)
        self.granule_xml_file = granule_xml_file
        self.TileId = ""
        self.angles = MajaL1Angles(self.root)
        self.sensing_time = xml_tools.get_only_value(self.root, XPATH_TILE_S2.get("sensing_time")).text
        self.datastrip_id = xml_tools.get_only_value(self.root, XPATH_TILE_S2.get("datastrip_id")).text
        self.reference_system_code = xml_tools.get_only_value(self.root, XPATH_TILE_S2.get("reference_code")).text
        self.reference_system_name = xml_tools.get_only_value(self.root, XPATH_TILE_S2.get("horizontal_cs_name")).text

        #*******************************************************************************/
        #** Set the list of Detector foot print filename */
        #/*******************************************************************************/
        # TEMPORARY
        l_DetList = self.get_list_of_mask("MSK_DETFOO")

        # ---------------------------------------------------------------------------------------------
        # PSD version detection:
        # Diff PSD = 14 and PSD >= 14
        # ATTENTION : for PSD 14 product, the SchemaLoaction value is always PSD 12
        #  PSD < 14  : <MASK_FILENAME type="MSK_DEFECT">S2A_OPER_MSK_DEFECT_MPS__20150428T084140_A000065_T15SVD_B01_MSIL1C.gml</MASK_FILENAME>
        #  PSD < 14  : <MASK_FILENAME bandId="0" type="MSK_DEFECT">S2A_OPER_MSK_DEFECT_MPS__20160914T113643_A006424_T36JTT_B01_MSIL1C.gml</MASK_FILENAME>
        #  PSD >= 14 :<MASK_FILENAME bandId="0" type="MSK_DEFECT">GRANULE/L1C_T36JTT_A006424_20160914T081456/QI_DATA/MSK_DEFECT_B01.gml</MASK_FILENAME>
        # -> Automatic detection: find GRANULE/ in the path. If is present, PSD >=14 ? yes
        # PSD >=14 : PATH from ROOT
        l_FirstFilename = l_DetList[0]
        l_PsdNewVersion = True
        # If not find, PSD < 14
        if not "GRANULE/" in l_FirstFilename:
            l_PsdNewVersion = False
            LOGGER.debug("The Sentinel2 XML Tile <"+granule_xml_file+"> has been detected OLD PSD version (PSD<14).")
        productPath = os.path.join(os.path.dirname(granule_xml_file),"QI_DATA")
        if l_PsdNewVersion:
            # return to the ROOT path
            productPath = os.path.join(os.path.dirname(granule_xml_file), ".." + os.path.sep + "..")
        LOGGER.debug("productPath : "+productPath)

        self.ListOfDetFootPrintHeaderFileName = [os.path.join(
            productPath, m) for m in self.get_list_of_mask("MSK_DETFOO")]
        self.ListOfSaturatedPixelsHeaderFileName = [os.path.join(
            productPath, m) for m in self.get_list_of_mask("MSK_SATURA")]
        self.ListOfDefectivPixelsHeaderFileName = [os.path.join(
            productPath, m) for m in self.get_list_of_mask("MSK_DEFECT")]
        self.ListOfNoDataPixelsHeaderFileName = [os.path.join(
            productPath, m) for m in self.get_list_of_mask("MSK_NODATA")]

    def get_list_of_mask(self, masktype):
        masks = []
        for band in LIST_OF_L1BAND_CHARID:
            path = "//MASK_FILENAME[@type='" + masktype + "' and contains(.,'" + band + "')]"
            masks.append(xml_tools.get_only_value(self.root, path).text)
            LOGGER.debug(
                "Adding " +
                xml_tools.get_only_value(
                    self.root,
                    path).text +
                " to list of masks for type " +
                masktype)
        return masks

    def get_geoposition_upperleftcorner(self, res):
        # ---------------------------------------------------------------------------------------------
        path = ("//Tile_Geocoding/Geoposition[@resolution='" + str(res) + "']")

        # ---------------------------------------------------------------------------------------------
        # The values are set in double in the xml header but they can be converted in integer because
        # the decimal part corresponds to 0.01 pixel
        ulx = int(xml_tools.get_only_value(self.root, path + "/ULX").text)
        uly = int(xml_tools.get_only_value(self.root, path + "/ULY").text)

        # ---------------------------------------------------------------------------------------------
        # Checks if null
        if (ulx == 0) or (uly == 0):
            raise MajaDataException(
                "Impossible to read the ULX and ULY coordinates for the Tile id '" +
                self.TileId +
                "' from the node Tile_Description/Geoposition.")

        l_Point = PointXY()
        l_Point.x = ulx
        l_Point.y = uly

        return l_Point

    def get_size(self, res):
        # ---------------------------------------------------------------------------------------------
        path = "//Tile_Geocoding/Size[@resolution='" + str(res) + "']"

        # ---------------------------------------------------------------------------------------------
        # Reads the sizes
        ncols = int(xml_tools.get_only_value(self.root, path + "/NCOLS").text)
        nrows = int(xml_tools.get_only_value(self.root, path + "/NROWS").text)
        # ---------------------------------------------------------------------------------------------
        # Checks if null
        if (ncols == 0) or (nrows == 0):
            raise MajaDataException(
                "Impossible to read the NROWS and NCOLS coordinates for the Tile id '" +
                self.TileId +
                "' from the node Tile_Description/Size.")

        # ---------------------------------------------------------------------------------------------
        # Set the point
        l_Point = PointXY()
        l_Point.x = ncols
        l_Point.y = nrows

        return l_Point

    def get_geoposition_dims(self, res):
        # ---------------------------------------------------------------------------------------------
        path = "//Tile_Geocoding/Geoposition[@resolution='" + str(res) + "']"

        # ---------------------------------------------------------------------------------------------
        # Reads the sizes
        xdim = int(xml_tools.get_only_value(self.root, path + "/XDIM").text)
        ydim = int(xml_tools.get_only_value(self.root, path + "/YDIM").text)

        # ---------------------------------------------------------------------------------------------
        # Checks if null
        if (xdim == 0) or (ydim == 0):
            raise MajaDataException(
                "Impossible to read the ULX and ULY coordinates for the Tile id '" +
                self.TileId +
                "' from the node Tile_Description/Geoposition.")

        # ---------------------------------------------------------------------------------------------
        # Set the point
        l_Point = PointXY()
        l_Point.x = xdim
        l_Point.y = ydim

        return l_Point

    def get_geoposition_boundingbox(self, res):
        l_Box = BoundingBox()
        ul = self.get_geoposition_upperleftcorner(res)
        n = self.get_size(res)
        dim = self.get_geoposition_dims(res)
        # ---------------------------------------------------------------------------------------------
        # Checks if null
        # ATTENTION: For coordinates, cf. FD 73233
        l_Box.xmin = ul.x
        l_Box.ymin = ul.y - abs(dim.y) * n.y
        l_Box.xmax = ul.x + abs(dim.x) * n.x
        l_Box.ymax = ul.y

        return l_Box

    def get_list_of_zones(self, bandIdx):
        list_of_zone = []
        # Convert the band index integer value in string
        path = "//Viewing_Incidence_Angles_Grids[@bandId='" + str(bandIdx) + "']"
        xnodes = xml_tools.get_all_values(self.root, path)
        # Two indices : the detector index and the feature index
        detId = 0

        # For each feature
        for i in range(len(xnodes)):
            LOGGER.debug((list(xnodes[i].items())))
            list_of_zone.append(xnodes[i].get("detectorId"))

        return list_of_zone

    def get_date_formated_yyyymmdd(self):
        ident = self.sensing_time
        element = ident.split('T')
        date = element[0]
        return date.replace("-", "")

    def get_date_formated_utc(self):
        ident = self.sensing_time
        element = ident.split('.')
        date = element[0]
        # Return date formated 'YYYY-MM-DD HH:MM:SS'
        return "UTC=" + date

    # Get the validity start and stop dates with the YYYYMMDDTHHMMSS format
    def get_validity_start_date(self):
        # ex : S2A_OPER_MSI_L1C_DS_MPS__20140915T120000_S20130707T171925_N01.01
        dataset = xml_tools.get_xml_string_value(self.root, "//DATASTRIP_ID")
        element = dataset.split('_')
        # start time : 20140915T120000
        date = element[-3]
        LOGGER.debug("GetValidityStartDate: " + date)
        l_tm = date_utils.get_datetime_from_yyyymmddthhmmss(date)
        return date_utils.get_utc_from_datetime(l_tm)

    def get_validity_stop_date(self):
        # ex : S2A_OPER_MSI_L1C_DS_MPS__20140915T120000_S20130707T171925_N01.01
        dataset = xml_tools.get_xml_string_value(self.root, "//DATASTRIP_ID")
        element = dataset.split('_')
        date = element[-2]
        # Remove S from the stop time : S20130707T171925
        date = date[1:]
        LOGGER.debug("GetValidityStopDate: " + date)
        l_tm = date_utils.get_datetime_from_yyyymmddthhmmss(date)
        return date_utils.get_utc_from_datetime(l_tm)
