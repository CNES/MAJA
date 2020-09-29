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
orchestrator.plugins.common.base.maja_l_2_3_image_filenames_provider_base -- shortdesc

orchestrator.plugins.common.base.maja_l_2_3_image_filenames_provider_base is a description

It defines


###################################################################################################
"""
import os
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.common.earth_explorer.earth_explorer_utilities import EarthExplorerUtilities
from orchestrator.common.earth_explorer.header_image_earth_explorer_xml_file_handler \
    import HeaderImageEarthExplorerXMLFileHandler
from orchestrator.common.date_utils import get_datetime_from_yyyymmdd
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.gipp_utils import uncompress_dbl_product
import orchestrator.common.maja_common as maja_common
import orchestrator.common.xml_tools as xml_tools

LOGGER = configure_logger(__name__)


class L23ImageFilenamesProviderBase(object):
    """

    """

    def __init__(self):
        self.m_HDRFileName = ""
        self.m_ListOfAnnexFiles = []
        self.m_ListOfResolutionsPrefix = []
        self.m_SREImageFileName = []
        self.m_SREHeaderFileName = []
        self.m_SRERegularFile = []
        self.m_FREImageFileName = []
        self.m_FREHeaderFileName = []
        self.m_FRERegularFile = []
        self.m_MSKImageFileName = []
        self.m_MSKHeaderFileName = []
        self.m_QLTImageFileName = []
        self.m_QLTHeaderFileName = []
        self.m_QLKImageFileName = ""
        self.m_QLKHeaderFileName = ""
        self.m_QLKRegularFile = []
        self.m_ListOfResolutions = []
        self.m_PublicDirectory = ""
        self.m_RootDirectory = ""
        self.m_DBLFileName = ""
        self.m_ReferenceProductHeaderId = ""
        self.m_Date = None
        self.m_FileClass = ""
        self.m_ReferenceProductInstance = ""
        self.m_ReferenceProductHeaderId = ""
        self.m_PMCFileName = ""

    def get_public_directory(self):
        return self.m_PublicDirectory

    def get_sre_filenames(self):
        return self.m_SREImageFileName

    def get_sre_headers(self):
        return self.m_SREHeaderFileName

    def get_sre_regular_filenames(self):
        return self.m_SRERegularFile

    def get_fre_filenames(self):
        return self.m_FREImageFileName

    def get_fre_headers(self):
        return self.m_FREHeaderFileName

    def get_fre_regular_filenames(self):
        return self.m_FRERegularFile

    def get_msk_filename(self):
        return self.m_MSKImageFileName

    def get_msk_header_filenames(self):
        return self.m_MSKHeaderFileName

    def get_qlt_filenames(self):
        return self.m_QLTImageFileName

    def get_qlt_header_filenames(self):
        return self.m_QLTHeaderFileName

    def get_qlk_filename(self):
        return self.m_QLKImageFileName

    def get_qlk_header_filename(self):
        return self.m_QLKHeaderFileName

    def get_qlk_regular_file(self):
        return self.m_QLKRegularFile

    def get_fileclass(self):
        return self.m_FileClass

    def get_dbl_filename(self):
        return self.m_DBLFileName

    def get_list_of_annex_file(self):
        return self.m_ListOfAnnexFiles

    def get_reference_product_header_id(self):
        return self.m_ReferenceProductHeaderId

    def get_reference_product_instance(self):
        return self.m_ReferenceProductInstance

    def reset(self):
        # ---------------------------------------------------------------------------------------------
        # Clean all tabs
        # Use for initialize members for
        # reading data
        self.m_ListOfAnnexFiles = []
        self.m_ListOfResolutionsPrefix = []
        self.m_SREImageFileName = []
        self.m_SREHeaderFileName = []
        self.m_SRERegularFile = []
        self.m_FREImageFileName = []
        self.m_FREHeaderFileName = []
        self.m_FRERegularFile = []
        self.m_MSKImageFileName = []
        self.m_MSKHeaderFileName = []
        self.m_QLTImageFileName = []
        self.m_QLTHeaderFileName = []
        self.m_ListOfResolutions = []

    def get_filename(self, directory, prefix, file_semantic, postfix, extention, check_file_exists):
        """
        const std::string & directory, const std::string & prefix,
        const std::string & file_semantic, const std::string & postfix,
        const std::string & extention, const bool check_file_exists
        TODO: see to remove this part from build_filenames (two different features for one function)

        :param directory:
        :param prefix:
        :param file_semantic:
        :param postfix:
        :param extention:
        :param check_file_exists:
        :return:
        """
        # Build the relative filename
        l_ImageFileName = prefix + "_" + file_semantic + "_" + postfix + extention

        # Build the absolute filename
        filename = os.path.join(directory, l_ImageFileName)

        # Check the existant files
        if check_file_exists and not os.path.exists(filename):
            raise MajaDataException("L2ImageFilenames Provider: The L2 '{}' filename {} doesn't exist.\n"
                                    "The <{}> product is NOT valid !".format(file_semantic, filename, directory))
        return filename

    def build_filenames(self, directory, prefix, postfix, check_file_exists, reading=None,
                        extention="", file_semantic=""):
        """
        Initialize the filename and Build a filename with the file semantic, extention,, etc...
        :param directory:
        :param prefix:
        :param postfix:
        :param check_file_exists:
        :param reading: 'true' for reading, 'false' for writing
        :param extention:
        :param file_semantic:
        :return:
        """
        if file_semantic:
            # Build the relative filename
            l_ImageFileName = prefix + "_" + file_semantic + "_" + postfix + extention

            # Build the absolute filename
            filename = os.path.join(directory, l_ImageFileName)

            # Check the existant files
            if check_file_exists and not os.path.exists(filename):
                raise MajaDataException("L2ImageFilenames Provider: The L2 '{}' filename {} doesn't exist.\n"
                                        "The <{}> product is NOT valid !".format(file_semantic, filename, directory))
            return filename

        # PUBLIC PART
        LOGGER.debug("L23ImageFilenamesProviderBase: prefix='{}' postfix='{}'.".format(prefix, postfix))
        # Set the general directory
        self.m_PublicDirectory = directory
        self.m_RootDirectory = os.path.abspath(self.m_PublicDirectory)

        # Set the DBL package filename
        self.m_DBLFileName = self.m_HDRFileName + ".DBL"

        l_NbResolutionsPrefix = len(self.m_ListOfResolutionsPrefix)
        if l_NbResolutionsPrefix == 0:
            raise MajaDataException("Internal error : the ListOfResolutionsPrefix is not initialized !!")

        LOGGER.debug("L23ImageFilenamesProviderBase: l_NbResolutionsPrefix={}.".format(l_NbResolutionsPrefix))

        for i in range(l_NbResolutionsPrefix):
            l_ResolutionPrefix = self.m_ListOfResolutionsPrefix[i]

            # Set SRE image/header filenames
            l_SREHeaderFileName = self.get_filename(directory, prefix, "PDTIMG", postfix,
                                                    "_SRE" + l_ResolutionPrefix + ".HDR",
                                                    check_file_exists)

            self.m_SREHeaderFileName.append(l_SREHeaderFileName)
            self.m_SREImageFileName.append(
                self.get_filename(
                    directory,
                    prefix,
                    "PDTIMG",
                    postfix,
                    "_SRE" +
                    l_ResolutionPrefix +
                    ".DBL.TIF",
                    check_file_exists))
            # Set the Regular file for the SRE filename
            # self.m_SRERegularFile.append(EarthExplorerXMLFileHandler::GetRegularFileFromFile(l_SREHeaderFileName))
            self.m_SRERegularFile.append(EarthExplorerUtilities.get_regular_file_from_file(l_SREHeaderFileName))

            # Set FRE image/header filenames
            l_FREHeaderFileName = self.get_filename(
                directory,
                prefix,
                "PDTIMG",
                postfix,
                "_FRE" +
                l_ResolutionPrefix +
                ".HDR",
                check_file_exists)
            self.m_FREHeaderFileName.append(l_FREHeaderFileName)
            self.m_FREImageFileName.append(
                self.get_filename(
                    directory,
                    prefix,
                    "PDTIMG",
                    postfix,
                    "_FRE" +
                    l_ResolutionPrefix +
                    ".DBL.TIF",
                    check_file_exists))
            # Set the Regular file for the FRE filename
            # self.m_FRERegularFile.append(EarthExplorerXMLFileHandler::GetRegularFileFromFile(l_FREHeaderFileName))
            # TODO see if regular file needed ?
            self.m_FRERegularFile.append(EarthExplorerUtilities.get_regular_file_from_file(l_FREHeaderFileName))

            # Set MSK image/header filenames
            l_MSKHeaderFileName = self.get_filename(
                directory,
                prefix,
                "PDTANX",
                postfix,
                "_MSK" +
                l_ResolutionPrefix +
                ".HDR",
                check_file_exists)
            self.m_MSKHeaderFileName.append(l_MSKHeaderFileName)
            self.m_MSKImageFileName.append(
                self.get_filename(
                    directory,
                    prefix,
                    "PDTANX",
                    postfix,
                    "_MSK" +
                    l_ResolutionPrefix +
                    ".DBL.TIF",
                    check_file_exists))
            self.add_file_to_list_of_annex(l_MSKHeaderFileName, "MSK")

            # Set QLT image/header filenames
            l_QLTHeaderFileName = self.get_filename(
                directory,
                prefix,
                "PDTANX",
                postfix,
                "_QLT" +
                l_ResolutionPrefix +
                ".HDR",
                check_file_exists)
            self.m_QLTHeaderFileName.append(l_QLTHeaderFileName)
            self.m_QLTImageFileName.append(
                self.get_filename(
                    directory,
                    prefix,
                    "PDTANX",
                    postfix,
                    "_QLT" +
                    l_ResolutionPrefix +
                    ".DBL.TIF",
                    check_file_exists))
            self.add_file_to_list_of_annex(l_QLTHeaderFileName, "QLT")

        # Set QLK image/header filenames
        self.m_QLKHeaderFileName = self.get_filename(directory, prefix, "PDTQLK", postfix, ".HDR", check_file_exists)
        self.m_QLKImageFileName = self.get_filename(directory, prefix, "PDTQLK", postfix, ".DBL.JPG", check_file_exists)
        # Set the Regularfile for the QLK filename
        self.m_QLKRegularFile.append(EarthExplorerUtilities.get_regular_file_from_file(self.m_QLKHeaderFileName))
        # m_QLKRegularFile = EarthExplorerXMLFileHandler::GetRegularFileFromFile(m_QLKHeaderFileName)
        LOGGER.debug("L23ImageFilenamesProviderBase::BuildFilenames done.")

    def add_file_to_list_of_annex(self, filename, sk_name):
        self.m_ListOfAnnexFiles.append(self.get_annex_file(filename, sk_name))

    @staticmethod
    def get_annex_file(filename, sk_name):
        # If filename is VE_VM0x_VSC_PDTANX_L2VALD_PARIS____20120101_ATB_R1.HDR
        # VE_VM0x_VSC_PDTANX_L2VALD__P_IS____20120101_ATB_R1.HDR
        # L5_TEST_LSC_PDTANX_L2VALD_EU93066200A00B_20100118_ATB.HDR
        #    890123456789012345678901234567890123456789
        # find ATB.Attention site is short name, not formated in 8 characters.
        l_refFileName = os.path.basename(filename)
        pathPart = os.path.dirname(filename)
        pathFilePart = os.path.basename(pathPart)

        annexfile = maja_common.AnnexFile
        annexfile.sk = sk_name
        annexfile.Nature = EarthExplorerUtilities.get_nature_from_filename(filename)
        annexfile.FileLocation = "./" + pathFilePart + "/" + l_refFileName
        annexfile.LogicalName = os.path.splitext(l_refFileName)[0]
        return annexfile

    def initialize_for_reading(self, infilename, plugin, check_exists):
        l_CanInitializeForReading = self.can_initialize_for_reading(infilename, plugin, check_exists)
        if not l_CanInitializeForReading:
            raise MajaDataException("Error while reading the product <" + infilename + ">!")

    def can_initialize_for_reading(self, inputfilename, plugin, check_exists):
        # Reset all variable (list)
        self.reset()

        # Expand the path
        l_refFileName = os.path.splitext(inputfilename)[0] + ".HDR"
        self.m_HDRFileName = l_refFileName
        LOGGER.debug("L23ImageFilenamesProviderBase::CanInitializeForReading: l_refFileName: '" + l_refFileName + "'.")

        l_CanLoad = xml_tools.can_load_file(l_refFileName)
        if not l_CanLoad:
            return False

        l_XMLHandler = HeaderImageEarthExplorerXMLFileHandler(l_refFileName)

        # Muscate products don't have always a site name over 8 characters
        site = l_XMLHandler.get_nick_name()
        acquisition_date = EarthExplorerUtilities.get_acquisition_from_global_filename(l_refFileName)

        m_FileClass = EarthExplorerUtilities.get_fileclass_from_filename(l_refFileName)
        prefix = EarthExplorerUtilities.get_satellite_class_from_filename(
            l_refFileName) + "_" + m_FileClass + "_" + \
            EarthExplorerUtilities.get_file_type_short_from_filename(l_refFileName)
        postfix = EarthExplorerUtilities.get_level_class_from_filename(l_refFileName) +\
                  "_" + site + "_" + acquisition_date

        LOGGER.debug(
            "L23ImageFilenamesProviderBase::CanInitializeForReading: prefix: '" +
            prefix +
            "', postfix: '" +
            postfix +
            "'.")
        # BUG Integration CHAINE chez VEGA
        self.m_ReferenceProductHeaderId = prefix + "_" + postfix

        # Set the date (build with the image file name)
        self.m_Date = get_datetime_from_yyyymmdd(acquisition_date)

        refFileNameDbl = os.path.splitext(l_refFileName)[0] + ".DBL"
        refFileNameDirDbl = os.path.splitext(l_refFileName)[0] + ".DBL.DIR"

        if os.path.exists(refFileNameDbl) == False and os.path.isdir(refFileNameDirDbl) == False:
            LOGGER.debug(
                "The input L2 product file .DBL " +
                refFileNameDbl +
                " doesn't exist or the directory <" +
                refFileNameDirDbl +
                "> doesn't exist !")
            return False

        uncompress_dbl_product(refFileNameDbl)

        # Detect the number of resolution.
        # test if file exists
        # Case 1: S2A_TEST_SSC_PDTIMG_L2VALD_15SVD____20091211_SRE_R1.HDR
        # Case 2: S2A_TEST_SSC_PDTIMG_L2VALD_15SVD____20091211_SRE.HDR
        # If Case 1, test the numer of file S2A_TEST_SSC_PDTIMG_L2VALD_15SVD____20091211_SRE_Rx.HDR
        listOfResolutions = plugin.ListOfL2Resolutions

        # Read the list of resolution from the Header file
        self.initialize_resolutions_infos(listOfResolutions)

        self.build_filenames(refFileNameDirDbl, prefix, postfix, check_exists, True)
        return True

    def initialize_resolutions_infos(self, listofres):
        self.m_ListOfResolutions = listofres
        self.m_ListOfResolutionsPrefix = []
        if len(listofres) == 1:
            self.m_ListOfResolutionsPrefix.append("")
        else:
            self.m_ListOfResolutionsPrefix = [str("_" + r) for r in listofres]

    # /** Initialize the filename */
    # virtual void
    # BuildFilenames(const std::string & directory, const std::string & prefix, const std::string & postfix,
    #                 const bool check_file_exists);
    # /** Initialize the filename */
    # virtual void
    # BuildFilenames(const std::string & directory, const std::string & prefix, const std::string & postfix,
    #                 const bool check_file_exists, const bool reading /*  */ );
    # /** Build a filename with the file semantic, extention,, etc... */
    # std::string
    # BuildFilename(const std::string & directory, const std::string & prefix, const std::string & file_semantic,
    #         const std::string & postfix, const std::string & extention, const bool check_file_exists);
    #
    # /** Add the filename to the list of Annex files */
    # virtual void
    # AddFileToListOfAnnexFiles(const std::string & filename, const std::string & sk_name /*ex: ATB, MSK, CLD, ...*/);
    #
    # virtual void
    # InvalidateTheFilenamesString();
    #
    # virtual void
    # InvalidateTheFilenamesOnDisk();
