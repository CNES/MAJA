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
import orchestrator.common.xml_tools as xml_tools
import os
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaDataException
from lxml import etree as et
LOGGER = configure_logger(__name__)


EARTH_EXPLORER_HANDLER_XPATH = {
    "Mission": "//Earth_Explorer_Header/Fixed_Header/Mission",
    "FileName": "//Earth_Explorer_Header/Fixed_Header/File_Name",
    "DBLFilenames": "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/DBL_Organization/" +
                    "List_of_Packaged_DBL_Files/Packaged_DBL_File/Relative_File_Path",
    "ListOfBands": "//List_of_Bands",
    "band_list_theor": "//List_of_Band_Theoretical_Wavelength",
    "band_theorical_wavelength_sub": "//Band_Theoretical_Wavelength",
    "ValidityStart": "//Fixed_Header/Validity_Period/Validity_Start",
    "ValidityStop": "//Fixed_Header/Validity_Period/Validity_Stop",
    "SignificantBits": "//Number_of_Significant_Bits",
    "File_Type": "//Fixed_Header/File_Type",
    "Creator": "//Creator",
    "CreatorVersion": "//Creator_Version",
    "OrbitNumber": "//SENSING_ORBIT_NUMBER",
    "ReferenceSystemCode": "//HORIZONTAL_CS_CODE",
    "ReferenceSystemShortDescription": "//HORIZONTAL_CS_NAME",
    "GenerationTime": "//GENERATION_TIME",
    "ProductStartTime" : "//PRODUCT_START_TIME",
    "ProductURI" : "//PRODUCT_URI",
    "SamplingFactor" : "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Annex_Information/Subsampling_Factor/By_Line"
}


class EarthExplorerXMLFileHandler(object):

    EE_SCHEMA_DECIMAL_9_FORMAT = "%.9f"

    XML_FORMAT_DATE = "UTC=%Y-%m-%dT%H:%M:%S"
    CONST_CIRRUS_DETECTED_NV = "CIRRUS_DETECTED"
    CONST_RAIN_DETECTED_NV = "RAIN_DETECTED"
    CONST_SUNGLINT_DETECTED_NV = "SUN_GLINT_DETECTED"
    CONST_HOTSPOT_DETECTED_NV = "HOT_SPOT_DETECTED"

    # QualityIndexType
    class QualityIndex:
        def __init__(self):
            self.Code = ""
            self.Value = ""
            self.BandCode = None

    class ImageCenterType(object):
        def __init__(self):
            self.azimuth = ""
            self.zenith = ""

    class LTCType(object):
        def __init__(self):
            self.date = ""
            self.solarangle = EarthExplorerXMLFileHandler.ImageCenterType()
            self.viewingangle = EarthExplorerXMLFileHandler.ImageCenterType()

    class PackagedDBLFileType:
        def __init__(self):
            self.relativefilepath = ""
            self.filedefinition = ""

    def __init__(self, main_xml_file, validate=False, schema_path=None):
        """
        Constructor
        """
        LOGGER.debug("Loading " + main_xml_file)
        self.main_xml_file = main_xml_file
        self.root = xml_tools.get_root_xml(self.main_xml_file, deannotate=True)
        self.orig_root = xml_tools.get_root_xml(self.main_xml_file, deannotate=False)
        self.nss = self.root.nsmap
        if validate and schema_path is not None:
            xml_tools.check_xml(main_xml_file, schema_path)

    def save_to_file(self, output_filename):
        self.orig_root[:] = self.root[:]
        xml_tools.save_to_xml_file(self.orig_root, output_filename)
        os.system(
            'XMLLINT_INDENT="    "  xmllint --format ' + str(output_filename) + " --output " + str(output_filename))

    def get_string_value_of(self, key):
        return xml_tools.get_only_value(self.root, EARTH_EXPLORER_HANDLER_XPATH[key], namespaces=self.nss).text

    def get_string_value(self, xpath):
        return xml_tools.get_only_value(self.root, xpath, namespaces=self.nss).text

    def replace_node(self, node_src, xpath, check_if_present=False):
        xml_tools.replace_node(node_src, xpath, self.root, xpath, check_if_present)

    def set_string_value(self, xpath, value, check_if_present=True):
        xml_tools.set_value(self.root, xpath, value, check=check_if_present)

    def get_list_of_packaged_dbl_files(self, absolute, check):
        nodes = xml_tools.get_all_values(self.root, EARTH_EXPLORER_HANDLER_XPATH["DBLFilenames"])
        list_of_files = [nd.text for nd in nodes]
        if absolute:
            l_rootbasepath = os.path.dirname(self.main_xml_file)
            list_of_files = [os.path.join(l_rootbasepath, nd.text) for nd in nodes]
        if check:
            for fi in list_of_files:
                if not os.path.exists(fi):
                    raise MajaDataException("File " + fi + " does not exists")
        return list_of_files

    def get_list_of_bands(self):
        return xml_tools.get_list_value_sorted_by_attribut(
            self.root, EARTH_EXPLORER_HANDLER_XPATH["ListOfBands"], "/Band", "", "count", "sn", 1)

    def get_list_of_band_theoretical_wavelength(self):
        """
        GetListOfBandTheoreticalWavelength
        TODO: check this one
        :return:
        """
        return [
            int(f) for f in xml_tools.get_list_value_sorted_by_attribut(
                self.root,
                EARTH_EXPLORER_HANDLER_XPATH["band_list_theor"],
                EARTH_EXPLORER_HANDLER_XPATH["band_theorical_wavelength_sub"],
                "",
                "count",
                "sn",
                1)]

    def update(self, p_validity_start, p_validity_stop, p_file_version, output_filename, p_creator, p_creator_version,
               p_notes,
               p_system, p_fileclass, p_creationdate, p_referenceproductheaderid):
        # ---------------------------------------------------------------------------------------------
        # Get the filename witout extension
        lee_file_name = os.path.basename(os.path.splitext(output_filename)[0])
        # ---------------------------------------------------------------------------------------------
        # Set all members
        xml_tools.set_value(self.root, "//File_Version", p_file_version)
        # ---------------------------------------------------------------------------------------------
        xml_tools.set_value(self.root, "//File_Name", lee_file_name)
        # ---------------------------------------------------------------------------------------------
        xml_tools.set_value(self.root, "//Creator", p_creator)
        xml_tools.set_value(self.root, "//Creator_Version", p_creator_version)
        # ---------------------------------------------------------------------------------------------
        xml_tools.set_value(self.root, "//Notes", p_notes)
        xml_tools.set_value(self.root, "//System", p_system)
        # ---------------------------------------------------------------------------------------------
        xml_tools.set_value(self.root, "//File_Class", p_fileclass)
        xml_tools.set_value(self.root, "//Fixed_Header/Source/Creation_Date", p_creationdate)
        # ---------------------------------------------------------------------------------------------
        xml_tools.set_value(self.root, "//Validity_Period/Validity_Start", p_validity_start)
        xml_tools.set_value(self.root, "//Validity_Period/Validity_Stop", p_validity_stop)
        # ---------------------------------------------------------------------------------------------
        xml_tools.set_value(self.root, "//Variable_Header/Specific_Product_Header/Reference_Product_Header_Id",
                            p_referenceproductheaderid)

    def update_pdtanx(self, p_mission, p_productimagesizex, p_productimagesizey, p_referenceproductinstance,
                      p_productisvalid):
        xml_tools.set_value(self.root, "//Mission", p_mission)
        xml_tools.set_int_value(self.root, "//Lines", p_productimagesizey)
        xml_tools.set_int_value(self.root, "//Columns", p_productimagesizex)
        # Instance_Id node
        xml_tools.set_value(self.root, "//Reference_Product_Instance", p_referenceproductinstance)
        if p_productisvalid:
            xml_tools.set_value(self.root, "//Reference_Product_Semantic", "L2VALD")
        else:
            xml_tools.set_value(self.root, "//Reference_Product_Semantic", "L2NOTV")

    def write_list_of_gipp_files(
            self,
            listofgipps,
            write_nature_node=True,
            p_xpath="//List_of_GIPP_Files",
            p_subxpath="GIPP_File"):
        """
         <List_of_GIPP_Files count="11">
         <GIPP_File sn="1">
         <Nature>GIP_CKEXTL</Nature>
         <File_Location>NIL=OTHER:EXTERNAL</File_Location>
         <Logical_Name>VE_TEST_GIP_CKEXTL_S_CAMARGUE_00001_00000000_99999999</Logical_Name>
         </GIPP_File>

         """
        # ", "GIPP_File
        try:
            xnode = xml_tools.get_only_value(self.root, p_xpath)
        except BaseException:
            raise MajaDataException("Error while reading the xml node '" + p_xpath + "' in the xml file! Details: ")

        l_count = len(listofgipps)
        xnode.set("count", str(l_count))
        for i in range(l_count):
            f = listofgipps[i]
            node = et.Element(p_subxpath)
            node.set("sn", str(i + 1))
            if write_nature_node:
                # Nature
                node2 = et.Element("Nature")
                node2.text = f.Nature
                node.append(node2)
            # File_Location
            node3 = et.Element("File_Location")
            node3.text = f.FileLocation
            node.append(node3)
            # Logical_Name
            node4 = et.Element("Logical_Name")
            node4.text = f.LogicalName
            node.append(node4)
            xnode.append(node)

    def write_list_of_quality_indexes(self, listofquality):
        try:
            xnode = xml_tools.get_only_value(self.root, "//List_of_Quality_Indexes")
        except BaseException:
            raise MajaDataException(
                "Error while reading the xml node '" +
                "//List_of_Quality_Indexes" +
                "' in the xml file! Details: ")

        l_count = len(listofquality)
        xnode.set("count", str(l_count))
        for i in range(l_count):
            f = listofquality[i]
            node = et.Element("Quality_Index")
            node.set("sn", str(i + 1))
            # Code
            node1 = et.Element("Code")
            node1.text = f.Code
            node.append(node1)
            # Value
            node2 = et.Element("Value")
            node2.text = f.Value
            node.append(node2)
            # band code if any
            if f.BandCode is not None:
                node3 = et.Element("Band_Code")
                node3.text = f.BandCode
                node.append(node3)
            xnode.append(node)



    def set_list_of_used_products_ids(self, list_of_products):
        xml_tools.set_string_list_value_sorted_by_attribute(self.root, list_of_products, "//List_of_Used_Product_Ids",
                                                            "Used_Product_Id", "count", "sn", 1)

    def set_list_of_bands(self, list_of_bands):
        xml_tools.set_string_list_value_sorted_by_attribute(
            self.root, list_of_bands, "//List_of_Bands", "Band", "count", "sn", 1)

    def write_list_of_annex_files(self, listofannex):
        """
          <List_of_Annex_Files count="6">
         <Annex_File sk="CLD">
         <Nature>VSC_PDTANX</Nature>
         <File_Location>./VE_TEST_VSC_L2VALD_CAMARGUE_20120101.DBL.DIR/VE_TEST_VSC_PDTANX_L2VALD_CAMARGUE_20120101_CLD.HDR</File_Location>
         <Logical_Name>VE_TEST_VSC_PDTANX_L2VALD_CAMARGUE_20120101_CLD</Logical_Name>
         </Annex_File>
         """
        try:
            xnode = xml_tools.get_only_value(self.root, "//Product_Organization/List_of_Annex_Files")
        except BaseException:
            raise MajaDataException(
                "Error while reading the xml node '" +
                "//Product_Organization/List_of_Annex_Files" +
                "' in the xml file! Details: ")

        l_count = len(listofannex)
        xnode.set("count", str(l_count))
        for i in range(l_count):
            f = listofannex[i]
            node = et.Element("Annex_File")
            node.set("sk", f.sk)
            # Nature
            node2 = et.Element("Nature")
            node2.text = f.Nature
            node.append(node2)
            # File_Location
            node3 = et.Element("File_Location")
            node3.text = f.FileLocation
            node.append(node3)
            # Logical_Name
            node4 = et.Element("Logical_Name")
            node4.text = f.LogicalName
            node.append(node4)
            xnode.append(node)

    def write_private_ltc_header(self, list_of_ltc):
        """
                 <LTC sn="1">
                 <Date>UTC=2012-01-01T10:51:48</Date>
                 <Solar_Angles>
                 <Image_Center>
                 <Azimuth unit="deg">166.636131956</Azimuth>
                 <Zenith unit="deg">67.602640760</Zenith>
                 </Image_Center>
                 </Solar_Angles>
                 <Viewing_Angles>
                 <Image_Center>
                 <Azimuth unit="deg">290.804966617</Azimuth>
                 <Zenith unit="deg">14.406687968</Zenith>
                 </Image_Center>
                 </Viewing_Angles>
                 </LTC>
                 """
        LOGGER.debug("Start writing the list of LTC node ...")
        try:
            xnode = xml_tools.get_only_value(self.root, "//List_of_LTC")
        except BaseException:
            raise MajaDataException(
                "Error while reading the xml node '" + "//List_of_LTC" + "' in the xml file! Details: ")
        l_count = len(list_of_ltc)
        xnode.set("count", str(l_count))
        for i in range(l_count):
            f = list_of_ltc[i]
            node = et.Element("LTC")
            node.set("sn", str(i + 1))
            # Date
            node2 = et.Element("Date")
            node2.text = f.Date
            node.append(node2)
            # SolarAngle
            node_angle = et.Element("Solar_Angles")
            node_im_center = et.Element("Image_Center")
            node_azimuth = et.Element("Azimuth")
            node_azimuth.set("unit", "deg")
            node_azimuth.text = f.solarangle.azimuth
            node_zenith = et.Element("Zenith")
            node_zenith.set("unit", "deg")
            node_zenith.text = f.solarangle.zenith
            node_im_center.append(node_azimuth)
            node_im_center.append(node_zenith)
            node_angle.append(node_im_center)
            node.append(node_angle)
            # Viewing angle
            node_v_angle = et.Element("Viewing_Angles")
            node_v_im_center = et.Element("Image_Center")
            node_v_azimuth = et.Element("Azimuth")
            node_v_azimuth.set("unit", "deg")
            node_v_azimuth.text = f.viewingangle.azimuth
            node_v_zenith = et.Element("Zenith")
            node_v_zenith.set("unit", "deg")
            node_v_zenith.text = f.viewingangle.zenith
            node_v_im_center.append(node_v_azimuth)
            node_v_im_center.append(node_v_zenith)
            node_v_angle.append(node_v_im_center)
            node.append(node_v_angle)
            # Insert to doc
            xnode.append(node)

        LOGGER.debug("Writing the list of LTC node DONES")

    def write_private_ltc_header_list_of_dbl(self, list_of_dbl):
        """
              <DBL_Organization>
           <List_of_Packaged_DBL_Files count="2">
           <Packaged_DBL_File sn="1">
           <Relative_File_Path>VE_TEST_VSC_LUTANX_L2VALD_CAMARGUE_20120103_LTC.DBL.DIR/VE_TEST_VSC_LUTANX_L2VALD_CAMARGUE_20120101_LTC.DBL.mha</Relative_File_Path>
           <File_Definition>VSC_LUTANX</File_Definition>
           </Packaged_DBL_File>
           <Packaged_DBL_File sn="2">
           <Relative_File_Path>VE_TEST_VSC_LUTANX_L2VALD_CAMARGUE_20120103_LTC.DBL.DIR/VE_TEST_VSC_LUTANX_L2VALD_CAMARGUE_20120103_LTC.DBL.mha</Relative_File_Path>
           <File_Definition>VSC_LUTANX</File_Definition>
           </Packaged_DBL_File>
           </List_of_Packaged_DBL_Files>
           </DBL_Organization>
              """
        LOGGER.debug("Start writing the list of LTC DBL node ...")
        try:
            xnode = xml_tools.get_only_value(self.root, "//DBL_Organization/List_of_Packaged_DBL_Files")
        except BaseException:
            raise MajaDataException(
                "Error while reading the xml node '" +
                "//DBL_Organization/List_of_Packaged_DBL_Files" +
                "' in the xml file! Details: ")

        l_count = len(list_of_dbl)
        xnode.set("count", str(l_count))
        for i in range(l_count):
            f = list_of_dbl[i]
            node = et.Element("Packaged_DBL_File")
            node.set("sn", str(i + 1))
            # Relative file path
            node2 = et.Element("Relative_File_Path")
            node2.text = f.relativefilepath
            node.append(node2)
            # File definition
            node3 = et.Element("File_Definition")
            node3.text = f.filedefinition
            node.append(node3)
            # Insert to doc
            xnode.append(node)

        LOGGER.debug("Writing the list of LTC DBL node DONE")

    def get_l2_product_ltc_information(self, p_destinationrelativedir, listofltc, listofpft):
        # --------------------------------------
        # Get the list of the LTC item
        l_count = int(xml_tools.get_attribute(self.root, "//List_of_LTC", "count"))
        # --------------------------------------
        # Loop under LTC
        for i in range(l_count):
            ltc = EarthExplorerXMLFileHandler.LTCType()
            ltc.Date = xml_tools.get_xml_string_value(self.root, "//List_of_LTC/LTC[@sn=" + str(i + 1) + "]/Date")
            # Get the solar angles
            ltc.solarangle.azimuth = xml_tools.get_xml_string_value(
                self.root, "//List_of_LTC/LTC[@sn=" + str(i + 1) + "]/Solar_Angles/Image_Center/Azimuth")
            ltc.solarangle.zenith = xml_tools.get_xml_string_value(
                self.root, "//List_of_LTC/LTC[@sn=" + str(i + 1) + "]/Solar_Angles/Image_Center/Zenith")

            # --------------------------------------
            # Get the viiewing angles
            ltc.viewingangle.azimuth = xml_tools.get_xml_string_value(
                self.root, "//List_of_LTC/LTC[@sn=" + str(i + 1) + "]/Viewing_Angles/Image_Center/Azimuth")
            ltc.viewingangle.zenith = xml_tools.get_xml_string_value(
                self.root, "//List_of_LTC/LTC[@sn=" + str(i + 1) + "]/Viewing_Angles/Image_Center/Zenith")
            # --------------------------------------
            # Push in the list
            listofltc.append(ltc)

        # --------------------------------------
        # Get the DBL packages
        l_count = int(xml_tools.get_attribute(self.root, "//DBL_Organization/List_of_Packaged_DBL_Files", "count"))
        s_count = str(l_count)

        # --------------------------------------
        # Loop under DBL
        for i in range(l_count):
            pft = EarthExplorerXMLFileHandler.PackagedDBLFileType()

            l_string = str(i + 1)
            l_relativefilepath = xml_tools.get_xml_string_value(
                self.root,
                "//DBL_Organization/List_of_Packaged_DBL_Files[@count=" +
                s_count +
                "]/Packaged_DBL_File[@sn=" +
                l_string +
                "]/Relative_File_Path")
            # --------------------------------------
            pft.relativefilepath = os.path.join(p_destinationrelativedir,
                                                os.path.basename(l_relativefilepath))
            pft.filedefinition = xml_tools.get_xml_string_value(
                self.root,
                "//DBL_Organization/List_of_Packaged_DBL_Files[@count=" +
                s_count +
                "]/Packaged_DBL_File[@sn=" +
                l_string +
                "]/File_Definition")

            # --------------------------------------
            # Push in the list
            listofpft.append(pft)

    def get_sampling_factor(self, ):
        return xml_tools.get_xml_int_value(self.root,EARTH_EXPLORER_HANDLER_XPATH["SamplingFactor"] )
