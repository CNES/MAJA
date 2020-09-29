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
orchestrator.plugins.common.base.maja_l2_image_filenames_provider -- shortdesc

orchestrator.plugins.common.base.maja_l2_image_filenames_provider is a description

It defines

/** \class  L2ImageFilenamesProvider
 * \brief This class provide the image filenames for the L2 products.
 *
 * You specify the reference of the filename with the SetReferenceBaseFileName method. The differents methods like xxxFileName() return the correct filename.
 * For example, for the reference of the filename (L1 level) ".../VE_TEST_VSC_L1VALD_CRAUCAME_20110104.DBL", the TOAFileName() method return
 *  ".../VE_TEST_VSC_L2VALD_CRAUCAME_20110104.DBL.DIR/VE_TEST_VSC_PDTIMG_L2VALD_CRAUCAME_20110104.DBL.TIF".

###################################################################################################
"""
from orchestrator.common.logger.maja_logging import configure_logger
from .maja_l_2_3_image_filenames_provider_base import L23ImageFilenamesProviderBase
from orchestrator.common.earth_explorer.earth_explorer_utilities import EarthExplorerUtilities
from orchestrator.plugins.common.base.maja_l2_private_image_filenames_provider import L2PrivateImageFilenamesProvider
from orchestrator.common.file_utils import fully_resolve
import os

LOGGER = configure_logger(__name__)


class L2ImageFilenamesProvider(L23ImageFilenamesProviderBase):

    def __init__(self):

        super(L2ImageFilenamesProvider, self).__init__()
        """

        """
        # ATB image filename (public data)
        self._m_ATBImageFileName = []
        # ATB header filename (public data)
        self._m_ATBHeaderFileName = []

        # CLD image filename (public data)
        self._m_CLDImageFileName = []
        # CLD header filename (public data)
        self._m_CLDHeaderFileName = []

        # SOL image filename (public data)
        self._m_SOLImageFileName = None
        # SOL header filename (public data)
        self._m_SOLHeaderFileName = None

        # VIE image filename (public data)
        self._m_VIEImageFileName = None
        # VIE header filename (public data)
        self._m_VIEHeaderFileName = None

        # Private data
        self._m_PrivateFileNames = L2PrivateImageFilenamesProvider()
        self._m_PrivateFileNames.m_DealingLTC = True

    def get_private_filenames(self):
        """
        GetPrivateFileNames
        :return:
        """
        return self._m_PrivateFileNames

    def get_atb_header_filenames(self):
        """
        GetATBHeaderFileName
        :return:
        """
        return self._m_ATBHeaderFileName

    def get_atb_image_filename(self):
        """
        GetATBHeaderFileName
        :return:
        """
        return self._m_ATBImageFileName

    def get_cld_image_filename(self):
        """
        GetCLDImageFileName
        :return:
        """
        return self._m_CLDImageFileName

    def get_cld_header_filenames(self):
        """
        GetCLDImageFileName
        :return:
        """
        return self._m_CLDHeaderFileName

    def get_hdr_filename(self):
        return self.m_HDRFileName

    def get_pmc_filename(self):
        return self.m_PMCFileName

    def reset(self):
        super(L2ImageFilenamesProvider, self).reset()
        self._m_ATBImageFileName = []
        self._m_ATBHeaderFileName = []
        self._m_CLDImageFileName = []
        self._m_CLDHeaderFileName = []

    def build_filenames(self, directory, prefix, postfix, check_file_exists, reading=None, extention="", file_semantic=""):
        """
        :param directory:
        :param prefix:
        :param postfix:
        :param check_file_exists:
        :param reading: True for reading, False for writing
        :param extention:
        :param file_semantic:
        :return:
        """

        LOGGER.debug("L2ImageFilenamesProvider: prefix='{}' postfix='{}'.".format(prefix, postfix))

        super(L2ImageFilenamesProvider, self).build_filenames(directory, prefix, postfix, check_file_exists,
                                                              reading=reading, extention=extention,
                                                              file_semantic=file_semantic)
        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # ** PUBLIC PART
        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        refFileNameDirDbl = self.m_PublicDirectory
        l_NbResolutionsPrefix = len(self.m_ListOfResolutionsPrefix)

        for i in range(0, l_NbResolutionsPrefix):
            l_ResolutionPrefix = self.m_ListOfResolutionsPrefix[i]
            # ** Set ATB image / header filenames * /
            l_ATBHeaderFileName = self.get_filename(refFileNameDirDbl, prefix, "PDTANX", postfix,
                                                    "_ATB" + l_ResolutionPrefix + ".HDR", check_file_exists)
            self._m_ATBHeaderFileName.append(l_ATBHeaderFileName)

            self._m_ATBImageFileName.append(
                self.get_filename(refFileNameDirDbl, prefix, "PDTANX",
                                  postfix, "_ATB" + l_ResolutionPrefix + ".DBL.TIF",
                                  check_file_exists))
            # * Add the file to the list of he AnnexFile
            self.add_file_to_list_of_annex(l_ATBHeaderFileName, "ATB")

            # * Set CLD image / header filenames * /
            l_CLDHeaderFileName = self.get_filename(refFileNameDirDbl, prefix, "PDTANX", postfix,
                                                    "_CLD" + l_ResolutionPrefix + ".HDR", check_file_exists)
            self._m_CLDHeaderFileName.append(l_CLDHeaderFileName)
            self._m_CLDImageFileName.append(
                self.get_filename(
                    refFileNameDirDbl,
                    prefix,
                    "PDTANX",
                    postfix,
                    "_CLD" +
                    l_ResolutionPrefix +
                    ".DBL.TIF",
                    check_file_exists))
            self.add_file_to_list_of_annex(l_CLDHeaderFileName, "CLD")

        # * Set SOL image / header filenames * /
        self._m_SOLHeaderFileName = self.get_filename(refFileNameDirDbl, prefix, "PDTANX", postfix, "_SOL.HDR",
                                                      check_file_exists)
        self._m_SOLImageFileName = self.get_filename(refFileNameDirDbl, prefix, "PDTANX", postfix, "_SOL.DBL.HDF",
                                                     check_file_exists)
        # Since 4.0 .0: not added in the ListOfAnnexFiles, because only used in the L2HeaderFileWriterProver and only
        # VENUS has this SOL anf VIE files(no for S2, S4, L5 / 7, F2 and L8).
        # * Set VIE image / header filenames * /
        self._m_VIEHeaderFileName = self.get_filename(refFileNameDirDbl, prefix, "PDTANX", postfix, "_VIE.HDR",
                                                      check_file_exists)
        self._m_VIEImageFileName = self.get_filename(refFileNameDirDbl, prefix, "PDTANX", postfix, "_VIE.DBL.HDF",
                                                     check_file_exists)
        # * ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** * /
        # * PRIVATE PART * /
        # * ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** * /
        self._m_PrivateFileNames.m_PrivateDirectory = self.m_PublicDirectory + "/PRIVATE"

        # For PRIVATE PART, the product have VE_TEST_TEC
        refFileNameDirDblPrivate = self._m_PrivateFileNames.m_PrivateDirectory

        # HDRPrivateFileName is build by using m_HDRFileName file and replace
        # extension.HDR by.EEF and replace "VSC" by "TEC".
        l_refFileName = os.path.basename(self.m_HDRFileName)

        DetectFileCategory = EarthExplorerUtilities.get_file_type_short_from_filename(l_refFileName)
        BaseName = l_refFileName
        LOGGER.debug("DetectFileCategory: '" + DetectFileCategory + "'...")
        LOGGER.debug("BaseName: '" + BaseName + "'...")
        BaseName = BaseName.replace(DetectFileCategory, "TEC")

        LOGGER.debug("BaseName: '" + BaseName + "'")

        BaseName2 = os.path.join(refFileNameDirDblPrivate, BaseName)
        self._m_PrivateFileNames.m_HDRPrivateFileName = os.path.splitext(BaseName2)[0] + ".EEF"

        LOGGER.debug("HDRPrivateFileName: '" + self._m_PrivateFileNames.m_HDRPrivateFileName + "'...")

        # * Set the Regular file for the SRE filename * /
        self._m_PrivateFileNames.m_HDRPrivateRegularFile = EarthExplorerUtilities.get_regular_file_from_file(
            self._m_PrivateFileNames.m_HDRPrivateFileName)

        # * Set RTA image / header filenames * /
        self._m_PrivateFileNames.m_RTAHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_RTA.HDR", check_file_exists)
        self._m_PrivateFileNames.m_RTAImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_RTA.DBL.TIF", check_file_exists)
        # * Set RTC image / header filenames * /
        self._m_PrivateFileNames.m_RTCHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_RTC.HDR", check_file_exists)
        self._m_PrivateFileNames.m_RTCImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_RTC.DBL.TIF", check_file_exists)
        # * Set RCR image / header filenames * /
        self._m_PrivateFileNames.m_RCRHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_RCR.HDR", check_file_exists)
        self._m_PrivateFileNames.m_RCRImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_RCR.DBL.TIF", check_file_exists)
        # * Set STO image / header filenames * /
        self._m_PrivateFileNames.m_STOHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_STO.HDR", check_file_exists)
        self._m_PrivateFileNames.m_STOImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_STO.DBL.TIF", check_file_exists)
        # * Set PXD image / header filenames * /
        self._m_PrivateFileNames.m_PXDHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_PXD.HDR", check_file_exists)
        self._m_PrivateFileNames.m_PXDImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_PXD.DBL.TIF", check_file_exists)
        # * Set NDT image / header filenames * /
        self._m_PrivateFileNames.m_NDTHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_NDT.HDR", check_file_exists)
        self._m_PrivateFileNames.m_NDTImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_NDT.DBL.TIF", check_file_exists)
        # * Set CLD image / header filenames * /
        self._m_PrivateFileNames.m_CLDHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_CLD.HDR", check_file_exists)
        self._m_PrivateFileNames.m_CLDImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_CLD.DBL.TIF", check_file_exists)
        # * Set CLA image / header filenames * /
        self._m_PrivateFileNames.m_CLAHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_CLA.HDR", check_file_exists)
        self._m_PrivateFileNames.m_CLAImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_CLA.DBL.TIF", check_file_exists)
        # * Set WAM image / header filenames * /
        self._m_PrivateFileNames.m_WAMHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_WAM.HDR", check_file_exists)
        self._m_PrivateFileNames.m_WAMImageFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "PDTANX", postfix, "_WAM.DBL.TIF", check_file_exists)

        # * Set LTC image / header filenames * /
        self._m_PrivateFileNames.m_LTCHeaderFileName = self.get_filename(
            refFileNameDirDblPrivate, prefix, "LUTANX", postfix, "_LTC.HDR", False)
        self._m_PrivateFileNames.m_LTCImageDirFileName = self.get_filename(refFileNameDirDblPrivate, prefix, "LUTANX",
                                                                           postfix, "_LTC.DBL.DIR", False)
        self._m_PrivateFileNames.m_LTCImageFileName = self.get_filename(
            self._m_PrivateFileNames.m_LTCImageDirFileName, prefix, "LUTANX", postfix, "_LTC.DBL.mha", False)
        # LTC is optional: if AOT = MULTI-SPETRAL, then the LTC product have not been created
        self._m_PrivateFileNames.m_DealingLTC = True
        if reading:
            # Set if the  LTC product is available
            self._m_PrivateFileNames.m_DealingLTC = os.path.exists(self._m_PrivateFileNames.m_LTCHeaderFileName)

        LOGGER.debug("L2ImageFilenamesProvider: prefix='" + prefix + "' postfix='" + postfix + "' done.")

    def initialize(self, l1_info, resolutions, out_dir, check_if_exists):
        self.m_FileClass = l1_info.FileClass
        # Reset all variable(list)
        self.reset()
        self.initialize_resolutions_infos(resolutions)
        # Expand the path * /
        refFileNameDir = fully_resolve(out_dir)
        LOGGER.debug("refFileName: '" + refFileNameDir + "'...")
        preprefix = ""
        prefix = ""
        postfix = ""
        l_site = l1_info.Site
        preprefix = l1_info.Prefix + "_" + self.m_FileClass
        # For FORMOSAT_MUSCATE_PROTO, the date is 2008 - 01 - 24
        prefix = preprefix + "_" + l1_info.FileCategory
        postfix = "L2VALD_" + l_site + "_" + l1_info.ProductDateStr
        self.m_ReferenceProductInstance = l_site + "_" + l1_info.ProductDateStr
        self.m_ReferenceProductHeaderId = prefix + "_" + postfix

        l_FileName = os.path.join(refFileNameDir, prefix + "_" + postfix + ".HDR")
        self.m_HDRFileName = l_FileName
        refFileNameDirDbl = os.path.join(refFileNameDir, prefix + "_" + postfix + ".DBL.DIR")
        LOGGER.debug("refFileNameDirDbl: '" + refFileNameDirDbl + "' ****************************************")

        # PMC report filename
        l_PMCFileName = ""
        pmcPrefix = preprefix + "_PMC"
        pmcPostfix = "L2REPT_" + l_site + "_" + l1_info.ProductDateStr
        # Correction sur V 4 - 1 - 0: PMC.HDR en.EEF
        l_PMCFileName = os.path.join(refFileNameDir, pmcPrefix + "_" + pmcPostfix + ".EEF")
        LOGGER.debug("l_PMCFileName: '" + l_PMCFileName + "'")
        self.m_PMCFileName = l_PMCFileName
        # Set the date(build with the image file name)
        self.m_Date = l1_info.ProductDate
        # Build the names
        self.build_filenames(refFileNameDirDbl, prefix, postfix, check_if_exists, False)

    def invalidate_the_filenames(self):
        pass
