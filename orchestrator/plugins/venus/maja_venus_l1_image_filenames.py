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
from orchestrator.common.maja_exceptions import MajaExceptionPluginBase
from orchestrator.plugins.venus.maja_venus_plugin import MajaVenusPlugin
from orchestrator.common.earth_explorer.earth_explorer_utilities import EarthExplorerUtilities

LOGGER = configure_logger(__name__)


class VenusL1ImageFilenames(object):

    def __init__(self):
        self.plugin = MajaVenusPlugin()
        self.m_hdrfilename = ""
        self.m_TOAImageFileName = ""
        self.m_TOAHeaderFileName = ""
        self.m_CLAImageFileName = ""
        self.m_CLAHeaderFileName = ""
        self.m_SOLImageFileName = ""
        self.m_SOLHeaderFileName = ""
        self.m_VIEImageFileName = ""
        self.m_VIEHeaderFileName = ""


    def initialize(self, reference_base_filename, validate=False, schema_path=None):
        LOGGER.debug("Start Venus L1 VenusL1ImageFilenamesProvider ...")
        LOGGER.debug("Analyse of the reference base filename: '" + reference_base_filename + "'")

        # Set GLOBAL HDR image filename
        l_dblfilename = os.path.splitext(reference_base_filename)[0] + ".DBL"
        l_FileNameDBL_DIR = os.path.splitext(reference_base_filename)[0] + ".DBL.DIR"

        if not (os.path.exists(l_dblfilename) or os.path.exists(l_FileNameDBL_DIR)):
            raise MajaExceptionPluginBase(
                "VenusL1ImageFilenames Provider: The GLOBAL Product file .DBL " +
                l_dblfilename +
                "' doesn't exist !! The " +
                  reference_base_filename +
                " product is NOT valid !"
            )

        # Expand the path
        l_refFileName = os.path.splitext(reference_base_filename)[0] + ".HDR"
        LOGGER.debug("m_hdrfilename: '" + l_refFileName + "'" )

        if not os.path.exists(l_refFileName):
            raise MajaExceptionPluginBase(
                "VenusL1ImageFilenames Provider: The GLOBAL Header file .HDR " +
                l_refFileName +
                " The" +
                reference_base_filename +
                " product is NOT valid !"
            )
        self.m_hdrfilename = l_refFileName

        if schema_path:
            LOGGER.debug("Root shemas install directory '" + schema_path + "'")
            lSchemaLocationDirectory = schema_path + self.plugin.MAJA_INSTALL_SCHEMAS_DIR

        # Check XML file
        if validate:
            xml_tools.check_xml(self.m_hdrfilename, lSchemaLocationDirectory)

        # Load HDR file
        l_CanLoad = xml_tools.can_load_file(self.m_hdrfilename)
        if not l_CanLoad:
            return False

        # Filename naming convention:
        # VE_TEST_VSC_L1VALD_CAMARGUE_20120101.HDR
        # VE_VM01_VSC_L1VALD__PE_R_U__20120101.HDR
        # XML information
        #        <File_Class>TEST</File_Class>
        #        <File_Type>VSC_L2VALD</File_Type>
        #        <Nick_Name>CAMARGUE</Nick_Name>
        #		  <Acquisition_Date>20120103</Acquisition_Date>
        # Split FileType to extract VSC and L1VALD strings ( from "VSC_L2VALD")

        m_FileClass = EarthExplorerUtilities.get_fileclass_from_filename(l_refFileName)


        prefix1 = EarthExplorerUtilities.get_satellite_class_from_filename(l_refFileName) + "_" + \
                  m_FileClass + "_" + \
                  EarthExplorerUtilities.get_file_type_short_from_filename(l_refFileName) + "_"

        prefix2 = "_" + EarthExplorerUtilities.get_level_class_from_filename(l_refFileName) + "_" + \
                  EarthExplorerUtilities.get_nickname_from_global_filename(l_refFileName) + "_" + \
                  EarthExplorerUtilities.get_acquisition_from_global_filename(l_refFileName)

        # *******************************************************************************************************
        # Set TOA image filename
        # *******************************************************************************************************
        # Build the relative filename
        l_FileName = prefix1 + "PDTIMG" + prefix2 + ".DBL.TIF"
        self.m_TOAImageFileName = os.path.join(l_FileNameDBL_DIR, l_FileName)

        LOGGER.debug(self.m_TOAImageFileName)

        if not os.path.exists(self.m_TOAImageFileName):
            raise MajaExceptionPluginBase("VenusL1ImageFilenames Provider: The L1 TOA filename " +
                                          str(self.m_TOAImageFileName) + " doesn't exist. The " +
                                          str(reference_base_filename) + "product is NOT valid !")
        # *******************************************************************************************************
        # Set TOA header filename
        # *******************************************************************************************************
        l_FileName = prefix1 + "PDTIMG" + prefix2 + ".HDR"
        self.m_TOAHeaderFileName = os.path.join(l_FileNameDBL_DIR, l_FileName)
        if not os.path.exists(self.m_TOAHeaderFileName):
            raise MajaExceptionPluginBase("VenusL1ImageFilenames Provider: The L1 TOA filename " +
                                          str(self.m_TOAHeaderFileName) + " doesn't exist. The " +
                                          str(reference_base_filename) + "product is NOT valid !")
        # Check XML file
        if validate:
            xml_tools.check_xml(self.m_TOAHeaderFileName, lSchemaLocationDirectory)

        # *******************************************************************************************************
        # Set CLA image filename
        # *******************************************************************************************************
        l_FileName = prefix1 + "PDTANX" + prefix2 + "_CLA.DBL.TIF"
        self.m_CLAImageFileName = os.path.join(l_FileNameDBL_DIR, l_FileName)
        if not os.path.exists(self.m_CLAImageFileName):
            raise MajaExceptionPluginBase("VenusL1ImageFilenames Provider: The L1 CLA filename " +
                                          str(self.m_CLAImageFileName) + " doesn't exist. The " +
                                          str(reference_base_filename) + "product is NOT valid !")
        # *******************************************************************************************************
        # Set CLA header filename
        # *******************************************************************************************************
        l_FileName = prefix1 + "PDTANX" + prefix2 + "_CLA.HDR"
        self.m_CLAHeaderFileName = os.path.join(l_FileNameDBL_DIR, l_FileName)
        if not os.path.exists(self.m_CLAHeaderFileName):
            raise MajaExceptionPluginBase("VenusL1ImageFilenames Provider: The L1 CLA filename " +
                                          str(self.m_CLAHeaderFileName) + " doesn't exist. The " +
                                          str(reference_base_filename) + "product is NOT valid !")
        # Check XML file
        if validate:
            xml_tools.check_xml(self.m_CLAHeaderFileName, lSchemaLocationDirectory)

        # *******************************************************************************************************
        # Set SOL image filename
        # *******************************************************************************************************
        l_FileName = prefix1 + "PDTANX" + prefix2 + "_SOL.DBL.HDF"
        self.m_SOLImageFileName = os.path.join(l_FileNameDBL_DIR, l_FileName)
        if not os.path.exists(self.m_SOLImageFileName):
            raise MajaExceptionPluginBase("VenusL1ImageFilenames Provider: The L1 SOL filename " +
                                              str(self.m_SOLImageFileName) + " doesn't exist. The " +
                                              str(reference_base_filename) + "product is NOT valid !")

        # *******************************************************************************************************
        # Set SOL header filename
        # *******************************************************************************************************
        l_FileName = prefix1 + "PDTANX" + prefix2 + "_SOL.HDR"
        self.m_SOLHeaderFileName = os.path.join(l_FileNameDBL_DIR, l_FileName)
        if not os.path.exists(self.m_SOLHeaderFileName):
            raise MajaExceptionPluginBase("VenusL1ImageFilenames Provider: The L1 SOL filename " +
                                              str(self.m_SOLHeaderFileName) + " doesn't exist. The " +
                                              str(reference_base_filename) + "product is NOT valid !")
        # Check XML file
        if validate:
            xml_tools.check_xml(self.m_SOLHeaderFileName, lSchemaLocationDirectory)


        # *******************************************************************************************************
        # Set VIE image filename
        # *******************************************************************************************************
        l_FileName = prefix1 + "PDTANX" + prefix2 + "_VIE.DBL.HDF"
        self.m_VIEImageFileName = os.path.join(l_FileNameDBL_DIR, l_FileName)
        if not os.path.exists(self.m_VIEImageFileName):
            raise MajaExceptionPluginBase("VenusL1ImageFilenames Provider: The L1 VIE filename " +
                                          str(self.m_VIEImageFileName) + " doesn't exist. The " +
                                          str(reference_base_filename) + "product is NOT valid !")
        # *******************************************************************************************************
        # Set VIE header filename
        # *******************************************************************************************************
        l_FileName = prefix1 + "PDTANX" + prefix2 + "_VIE.HDR"
        self.m_VIEHeaderFileName = os.path.join(l_FileNameDBL_DIR, l_FileName)
        if not os.path.exists(self.m_VIEHeaderFileName):
            raise MajaExceptionPluginBase("VenusL1ImageFilenames Provider: The L1 VIE filename " +
                                          str(self.m_VIEHeaderFileName) + " doesn't exist. The " +
                                          str(reference_base_filename) + "product is NOT valid !")
        # Check XML file
        if validate:
            xml_tools.check_xml(self.m_VIEHeaderFileName, lSchemaLocationDirectory)

        return True






