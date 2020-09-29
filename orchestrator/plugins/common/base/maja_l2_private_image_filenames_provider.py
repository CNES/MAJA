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
from orchestrator.common.maja_exceptions import MajaDataException
import os

LOGGER = configure_logger(__name__)


class L2PrivateImageFilenamesProvider():

    def __init__(self):
        """

        """
        # ---------------------------------------------------------------------------------------------------
        # Default values. Use by MUSCATE L2 filename product
        self.m_PrivateDirectory = "PRIVATE"
        # ---------------------------------------------------------------------------------------------------
        self.m_RTAImageFileName = "RTA.tif"
        self.m_RTAHeaderFileName = "RTA.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_RCRImageFileName = "RCR.tif"
        self.m_RCRHeaderFileName = "RCR.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_RTCImageFileName = "RTC.tif"
        self.m_RTCHeaderFileName = "RTC.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_STOImageFileName = "STO.tif"
        self.m_STOHeaderFileName = "STO.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_PXDImageFileName = "PXD.tif"
        self.m_PXDHeaderFileName = "PXD.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_NDTImageFileName = "NDT.tif"
        self.m_NDTHeaderFileName = "NDT.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_CLDImageFileName = "CLD.tif"
        self.m_CLDHeaderFileName = "CLD.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_CLAImageFileName = "CLA.tif"
        self.m_CLAHeaderFileName = "CLA.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_WAMImageFileName = "WAM.tif"
        self.m_WAMHeaderFileName = "WAM.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_LTCImageDirFileName = "LTC"
        self.m_LTCImageFileName = "LTC.mha"
        self.m_LTCHeaderFileName = "LTC.xml"
        # ---------------------------------------------------------------------------------------------------
        self.m_HDRPrivateFileName = "HDR.xml"
        self.m_HDRPrivateRegularFile = None
        # ---------------------------------------------------------------------------------------------------
        self.m_TECPrivateFileName = "TEC.EEF"

        self.m_DealingLTC = True

    def get_private_directory(self):
        return self.m_PrivateDirectory

    def get_dealing_ltc(self):
        return self.m_DealingLTC

    def set_dealing_ltc(self, dealltc):
        self.m_DealingLTC = dealltc

    def get_ltc_image_dir_filename(self):
        return self.m_LTCImageDirFileName

    def get_ltc_header_filename(self):
        return self.m_LTCHeaderFileName

    def get_rta_image_filename(self):
        return self.m_RTAImageFileName

    def get_rta_header_filename(self):
        return self.m_RTAHeaderFileName

    def get_rtc_image_filename(self):
        return self.m_RTCImageFileName

    def get_rtc_header_filename(self):
        return self.m_RTCHeaderFileName

    def get_ndt_image_filename(self):
        return self.m_NDTImageFileName

    def get_ndt_header_filename(self):
        return self.m_NDTHeaderFileName

    def get_cld_image_filename(self):
        return self.m_CLDImageFileName

    def get_cld_header_filename(self):
        return self.m_CLDHeaderFileName

    def get_rcr_image_filename(self):
        return self.m_RCRImageFileName

    def get_rcr_header_filename(self):
        return self.m_RCRHeaderFileName

    def get_sto_image_filename(self):
        return self.m_STOImageFileName

    def get_sto_header_filename(self):
        return self.m_STOHeaderFileName

    def get_pxd_image_filename(self):
        return self.m_PXDImageFileName

    def get_pxd_header_filename(self):
        return self.m_PXDHeaderFileName

    def get_cla_image_filename(self):
        return self.m_CLAImageFileName

    def get_cla_header_filename(self):
        return self.m_CLAHeaderFileName

    def get_wam_image_filename(self):
        return self.m_WAMImageFileName

    def get_wam_header_filename(self):
        return self.m_WAMHeaderFileName

    def get_ltc_image_filename(self):
        return self.m_LTCImageFileName

    def get_tec_image_filename(self):
        return self.m_TECPrivateFileName

    def get_hdr_private_filename(self):
        return self.m_HDRPrivateFileName

    def get_hdr_regular_file(self):
        return self.m_HDRPrivateRegularFile

    def initialize_with_root_dir(self, root_dir, product_date):
        # ------------------------------------------------------------------------
        # Generate the filenames of the composite product(private part of the L2 product)
        # header and image filenames
        self.m_PrivateDirectory = root_dir
        # ---------------------------------------------------------------------------------------------------
        self.m_HDRPrivateFileName = self.m_PrivateDirectory + os.path.sep + self.m_HDRPrivateFileName
        self.m_RTAHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_RTAHeaderFileName
        self.m_RTAImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_RTAImageFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_RCRHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_RCRHeaderFileName
        self.m_RCRImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_RCRImageFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_RTCHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_RTCHeaderFileName
        self.m_RTCImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_RTCImageFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_STOHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_STOHeaderFileName
        self.m_STOImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_STOImageFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_PXDHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_PXDHeaderFileName
        self.m_PXDImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_PXDImageFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_NDTHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_NDTHeaderFileName
        self.m_NDTImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_NDTImageFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_CLDHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_CLDHeaderFileName
        self.m_CLDImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_CLDImageFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_CLAHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_CLAHeaderFileName
        self.m_CLAImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_CLAImageFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_WAMHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_WAMHeaderFileName
        self.m_WAMImageFileName = self.m_PrivateDirectory + os.path.sep + self.m_WAMImageFileName

        # ---------------------------------------------------------------------------------------------------
        self.m_LTCImageDirFileName = self.m_PrivateDirectory + os.path.sep + self.m_LTCImageDirFileName
        self.m_LTCHeaderFileName = self.m_PrivateDirectory + os.path.sep + self.m_LTCHeaderFileName
        # ---------------------------------------------------------------------------------------------------
        self.m_LTCImageFileName = self.m_LTCImageDirFileName + os.path.sep + product_date + ".mha"
        # ---------------------------------------------------------------------------------------------------
        self.m_TECPrivateFileName = self.m_PrivateDirectory + os.path.sep + self.m_TECPrivateFileName

    def initialize_for_reading(self, root_dir):
        # Parse the directory
        listoffilenames = [
            os.path.join(
                root_dir,
                f) for f in os.listdir(root_dir) if os.path.isfile(
                os.path.join(
                    root_dir,
                    f))]
        l_NbFiles = len(listoffilenames)
        LOGGER.debug(
            "L2PrivateImageFilenamesProvider::InitializeForReadingWithThisThisRootDirectory : Nb files in the sub directory: " +
            str(l_NbFiles))
        # Loop for all files
        for i in range(0, l_NbFiles):
            l_Filename = listoffilenames[i]
            # Get the short filename(without full path)
            l_ShortFilename = os.path.basename(l_Filename)
            l_ShortExt = os.path.splitext(l_ShortFilename)[1]
            # Set the STO filename
            if "STO" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_STOHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_STOImageFileName = l_Filename
            # Set the CLA filename
            elif "CLA" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_CLAHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_CLAImageFileName = l_Filename
            # Set the RTA filename
            elif "RTA" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_RTAHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_RTAImageFileName = l_Filename
            # Set the RTC filename
            elif "RTC" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_RTCHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_RTCImageFileName = l_Filename
            # Set the RCR filename
            elif "RCR" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_RCRHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_RCRImageFileName = l_Filename
            # Set the PXD filename
            elif "PXD" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_PXDHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_PXDImageFileName = l_Filename
            # Set the NDT filename
            elif "NDT" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_NDTHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_NDTImageFileName = l_Filename
            # Set the CLD filename
            elif "CLD" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_CLDHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_CLDImageFileName = l_Filename
            # Set the WAM filename
            elif "WAM" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_WAMHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_WAMImageFileName = l_Filename
            # Set the LTC filename
            elif "LTC" in l_ShortFilename:
                if ".xml" in l_ShortExt or ".HDR" in l_ShortExt:
                    self.m_LTCHeaderFileName = l_Filename
                elif ".tif" in l_ShortExt or ".TIF" in l_ShortExt:
                    self.m_LTCImageFileName = l_Filename
            # Set the TEC filename
            elif "TEC" in l_ShortFilename:
                self.m_TECPrivateFileName = l_Filename

        # Dealing the LTC data
        self.m_DealingLTC = os.path.exists(self.m_LTCHeaderFileName)
        LOGGER.debug("The LTC file exist <" + self.m_LTCHeaderFileName + "> : " + str(self.m_DealingLTC))

        # If Dealing the LTC data, initialize the LTC Directory
        if self.m_DealingLTC:
            # List of all subdirectories
            listoffilenames = [
                os.path.join(
                    root_dir,
                    f) for f in os.listdir(root_dir) if os.path.isdir(
                    os.path.join(
                        root_dir,
                        f))]
            l_NbFiles = len(listoffilenames)
            LTCDirFound = False
            LOGGER.debug(
                "L2PrivateImageFilenamesProvider::InitializeForReadingWithThisThisRootDirectory : Nb files in the sub directory: " +
                str(l_NbFiles))
            # Find the LTC subdir name
            l_ltc = [f for f in listoffilenames if "LTC" in os.path.basename(f) and os.path.isdir(f)]
            LTCDirFound = len(l_ltc) != 0
            # If not found, throw an exception !
            if not LTCDirFound:
                raise MajaDataException(
                    "Impossible to detect the LTC sub-directory in the directory <" + root_dir + "> !")
            else:
                self.m_LTCImageDirFileName = l_ltc[-1]
