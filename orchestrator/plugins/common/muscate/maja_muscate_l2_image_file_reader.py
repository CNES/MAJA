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
orchestrator.plugins.common.earth_explorer.maja_earth_explorer_l2_image_file_reader -- shortdesc

orchestrator.plugins.common.earth_explorer.maja_earth_explorer_l2_image_file_reader is a description

It defines classes_and_methods


###################################################################################################
"""

import os
from orchestrator.plugins.common.base.maja_l2_image_reader_base import L2ImageReaderBase
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.plugins.common.base.maja_l2_private_image_filenames_provider import L2PrivateImageFilenamesProvider
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
import orchestrator.common.file_utils as file_utils
from orchestrator.common.muscate.muscate_xml_file_handler import MuscateXMLFileHandler
from orchestrator.cots.otb.algorithms.otb_band_math import *
import orchestrator.common.date_utils as date_utils

LOGGER = configure_logger(__name__)


class MuscateL2ImageFileReader(L2ImageReaderBase):

    def __init__(self):
        super(MuscateL2ImageFileReader, self).__init__()
        self.dict_of_vals = {}

    @staticmethod
    def is_a_muscate_l2_product_by_checking_the_filename(p_filename):
        # p_filename: ex:
        # /../../../LANDSAT5-TM-XS_20100118-103000-000_L2A_EU93066200A00B_C_V1-0/LANDSAT5-TM-XS_20100118-103000-000_L2A_EU93066200A00B_C_V1-0_MTD_ALL.xml
        l_ShortFilename = os.path.basename(p_filename)
        LOGGER.debug(
            "MuscateL2ImageFileReader::IsAMuscateL2ProductByCheckingTheFilename l_ShortFilename: " +
            l_ShortFilename)
        l_ShortDirectory = os.path.basename(os.path.dirname(p_filename))
        LOGGER.debug(
            "MuscateL2ImageFileReader::IsAMuscateL2ProductByCheckingTheFilename l_ShortDirectory: " +
            l_ShortDirectory)
        # Detect the L1C files : LANDSAT5-TM-XS_20100118-103000-000_L2A_EU93066200A00B_C_V1-0_MTD_ALL.xml
        if "_L2A_" in l_ShortDirectory:
            # Look for the MTD_ALL keys and the xml extension
            if "_L2A_" in l_ShortFilename and "_MTD_ALL" in l_ShortFilename and os.path.splitext(l_ShortFilename)[
                    1] == ".xml":
                return True
        return False

    @staticmethod
    def is_a_muscate_l2_product_by_checking_the_satellite(p_filename, plugin_base):
        LOGGER.debug("File detected with the filenaming <" + p_filename +
                     ">. Check with the field 'Platform' in the xml file...")
        lXMLHandler = MuscateXMLFileHandler(p_filename)
        LOGGER.debug(
            "Is the filename " +
            p_filename +
            " a 'muscate' L1 product compatible with the plugin '" +
            plugin_base.PluginName +
            "' ??")
        if plugin_base.is_valid_with_l2_satellite(lXMLHandler.get_string_value_of("Platform")):
            LOGGER.debug(
                "The filename " +
                p_filename +
                " is a 'muscate' L1 product compatible with the plugin '" +
                plugin_base.PluginName +
                "'!")
            return True
        return False

    @staticmethod
    def can_read_with_plugin_base(p_filename, p_plugin_base):
        """
        Return in the product is readable with the plugin Earth Explorer
        :return:
        """
        return MuscateL2ImageFileReader.is_a_muscate_l2_product_by_checking_the_filename(
            p_filename) and MuscateL2ImageFileReader.is_a_muscate_l2_product_by_checking_the_satellite(p_filename,
                                                                                                       p_plugin_base)

    @staticmethod
    def muscate_detect_l2_products(input_directory, p_ListOfL2Products, p_PluginBase):
        #        if ("F2_" in nomFichier) and ("_L1TILE_" in nomFichier) and (os.path.isdir(os.path.join(input_directory,dir))): # ATTENTION, ne pas prendre le "toa.msq.hdr"
        #        /L5_TEST_LSC_PDTIMG_L1TILE_TM_____EU93066200A00B_20091217/L5_TEST_LSC_PDTIMG_L1TILE_TM_____EU93066200A00B_20091217.XML
        l_ListOfSubDirectories = file_utils.maja_get_subdirectories(input_directory)
        # Note: in the list, there are only File, the directories have been excluded of the list
        l_NbFiles = len(l_ListOfSubDirectories)
        LOGGER.debug("MuscateL1ImageInformationsProvider::MuscateDetectL1Products: nb sub directories: " + str(l_NbFiles))
        for dir in l_ListOfSubDirectories:
            LOGGER.debug("Parse the directory: " + dir)
            # Detect the L1C file
            l_ShortDirectoryFilename = os.path.basename(dir)
            LOGGER.debug("l_ShortDirectoryFilename: " + l_ShortDirectoryFilename)
            if "_L2A_" in l_ShortDirectoryFilename:
                l_ListOfFilenames = [os.path.join(dir, f) for f in os.listdir(dir)]
                l_NbSubFiles = len(l_ListOfFilenames)
                LOGGER.debug("Nb files in the sub directory: " + str(l_NbSubFiles))
                for filename in l_ListOfFilenames:
                    if MuscateL2ImageFileReader.is_a_muscate_l2_product_by_checking_the_filename(filename):
                        LOGGER.debug("Is a muscate filename : " + filename)
                        if MuscateL2ImageFileReader.is_a_muscate_l2_product_by_checking_the_satellite(
                                filename, p_PluginBase):
                            LOGGER.debug("Is a muscate satelite : " + filename)
                            LOGGER.debug("Add the 'muscate' L2 product " + filename + " in the list of products!")
                            p_ListOfL2Products.append(filename)

    def read_info_with_plugin_base(self, p_filename, p_EnableReadingPublicData, p_PluginBase):
        # *************************************************************************************************************
        # ****  PRELIMINARY  DATA      ********************************************************************************
        l_L2XMLHandler = MuscateXMLFileHandler(p_filename)
        # Set Attributes
        self.ProductFilename = p_filename
        self.GlobalProductFilename = p_filename
        self.Satellite = l_L2XMLHandler.get_string_value_of("Platform")
        self.Acquisition_Date = l_L2XMLHandler.get_acquisition_date_formated_yyyymmdd()
        self.Validity_Start = l_L2XMLHandler.get_date_pdv_formated_utc()
        self.Validity_Stop = l_L2XMLHandler.get_date_pdv_formated_utc()
        self.Nick_Name = l_L2XMLHandler.get_string_value_of("ZoneGeo")
        self.Reference_SiteDefinition_Id = "UNKNOWN"
        self.Reflectance_Quantification_Value = l_L2XMLHandler.get_string_value_of("QuantificationValue")
        self.Nodata_Value = float(l_L2XMLHandler.get_string_value_of("Nodata_Value"))
        self.RealL2Nodata_Value = float(l_L2XMLHandler.get_string_value_of(
            "Nodata_Value")) * float(l_L2XMLHandler.get_string_value_of("QuantificationValue"))
        self.VAP_Quantification_Value = float(
            l_L2XMLHandler.get_string_value_of("WaterVaporContentQuantificationValue"))
        self.VAP_Nodata_Value = float(l_L2XMLHandler.get_string_value_of("VAP_Nodata_Value"))
        self.AOT_Quantification_Value = float(
            l_L2XMLHandler.get_string_value_of("AerosolOpticalThicknessQuantificationValue"))
        self.AOT_Nodata_Value = float(l_L2XMLHandler.get_string_value_of("AOT_Nodata_Value"))
        self.RealVAP_Nodata_Value = self.VAP_Nodata_Value * 1. / self.VAP_Quantification_Value
        self.RealAOT_Nodata_Value = self.AOT_Nodata_Value * 1. / self.AOT_Quantification_Value
        # Init private part to get image filename for projRef
        l_PublicDirectory = os.path.dirname(p_filename)
        l_PrivateDirectory = os.path.join(l_PublicDirectory,
                                          l_L2XMLHandler.get_string_value_of("PrivateDirectoryFilename"))
        # Read the Private images
        self.L2PrivateImageFilenamesProvider = L2PrivateImageFilenamesProvider()
        self.L2PrivateImageFilenamesProvider.initialize_for_reading(l_PrivateDirectory)
        # If public reading enable
        if p_EnableReadingPublicData:
            l_JpiFilename = os.path.join(l_PublicDirectory,
                                         l_L2XMLHandler.get_string_value_of("JobProcessingInformationsFileFilename"))
            l_JPIL2XMLHandler = MuscateXMLFileHandler(l_JpiFilename)
            self.Adjacency_Effects_And_Slope_Correction = l_JPIL2XMLHandler.get_string_value_of(
                "ProcessingFlagsAndModesProcessingAdjacencyEffectsAndSlopeCorrection")

        # UTC=2010-01-18T12:00:00
        self.Date = date_utils.get_datetime_from_yyyymmdd(l_L2XMLHandler.get_acquisition_date_formated_yyyymmdd())
        self.PluginName = p_PluginBase.PluginName
        self.FileClass = "TEST"
        self.LevelType = "L2VALD"
        # -------------------------------------------------------------------------
        # Get the projection ref
        # this->m_ProjectionRef =
        # vnsGetProjectionRefMacro(this->m_L2PrivateImageFilenamesProvider.GetRTAImageFileName());
        self.Prefix = p_PluginBase.QCK_PREFIX_MAP[l_L2XMLHandler.get_string_value_of("Platform")]

    def read_with_plugin_base(self, p_filename, p_EnableReadingPublicData, app_handler, p_PluginBase):
        """

        :param p_filename:
        :param p_EnableReadingPublicData: bool
        :param p_PluginBase: PluginBase::Pointer
        :return:
        """
        working_dir = app_handler.get_directory_manager().get_temporary_directory("L2Read_",
                                            do_always_remove=True)
        l_PublicDirectory = os.path.dirname(p_filename)
        # Init xml handler
        l_L2XMLHandler = MuscateXMLFileHandler(p_filename)
        # Do caching ?
        caching = app_handler.get_user_conf().get_Computing().get_Caching().get_EnableCachingConvertReflectanceData()
        # ex: l_L2XMLHandler->GetQuantificationValue() : 1000
        l_ReflectanceQuantificationValue = 1. / float(l_L2XMLHandler.get_string_value_of("QuantificationValue"))
        l_JpiFilename = os.path.join(l_PublicDirectory,
                                     l_L2XMLHandler.get_string_value_of("JobProcessingInformationsFileFilename"))
        l_JPIL2XMLHandler = MuscateXMLFileHandler(l_JpiFilename)
        self._m_WriteFRE = bool(l_JPIL2XMLHandler.get_string_value_of(
            "ProcessingFlagsAndModesProcessingAdjacencyEffectsAndSlopeCorrection"))
        LOGGER.debug("m_WriteFRE= " + str(self._m_WriteFRE))
        # ****  PUBLIC  DATA      *************************************************************************************
        if p_EnableReadingPublicData:
            self.muscate_read_public_images(l_L2XMLHandler, p_PluginBase, working_dir)
        # ****  PRIVATE  DATA     *************************************************************************************
        self.read_private_images(
            self.L2PrivateImageFilenamesProvider,
            l_ReflectanceQuantificationValue,
            p_PluginBase,
            working_dir)

    def muscate_read_public_images(self, p_L2XMLHandler, p_PluginBase, working_dir):
        l_NumberOfResolutions = len(p_PluginBase.ListOfL2Resolutions)
        l_bands_definition = p_PluginBase.BandsDefinitions
        # ****  PUBLIC  DATA      *************************************************************************************
        l_ReflectanceQuantificationValue = 1. / float(p_L2XMLHandler.get_string_value_of("QuantificationValue"))
        # Loop on all resolution products
        l_SREList = []
        l_FREList = []
        l_ATBList = []
        l_VAPList = []
        l_AOTList = []
        l_QLTList = []
        l_SATList = []
        l_EDGList = []
        l_CLDList = []
        l_MSKList = []
        l_WATList = []
        l_SNOWList = []
        for resol in range(0, l_NumberOfResolutions):
            l_sres_mtd = "XS"
            if len(p_PluginBase.BandsDefinitions.ListOfL2Resolutions) > 1:
                l_sres_mtd = p_PluginBase.BandsDefinitions.ListOfL2Resolutions[resol]
            l_sres = p_PluginBase.BandsDefinitions.ListOfL2Resolutions[resol]
            # Get the list of band of the current resolution
            listOfL2Bands = p_PluginBase.BandsDefinitions.get_list_of_l2_band_code(l_sres)
            # For each band of the current resolution
            l_firstBandIdx = p_PluginBase.BandsDefinitions.get_band_id_in_l2(listOfL2Bands[0])
            l_NumberOfComponentsPerPixel = len(p_PluginBase.BandsDefinitions.get_list_of_l2_band_code(l_sres))

            # Surface Reflectance reading
            LOGGER.debug("Start reading the Surface reflectance image filenames ...")
            l_ListOfL2SurfaceReflectanceFilenames = p_L2XMLHandler.get_list_of_l2_surface_reflectance_filenames(
                l_sres_mtd)
            l_NumberOfL2SurfaceReflectanceFilenames = len(l_ListOfL2SurfaceReflectanceFilenames)
            LOGGER.debug("l_NumberOfL2SurfaceReflectanceFilenames: " + str(l_NumberOfL2SurfaceReflectanceFilenames))

            # Concatenate bands
            tmp_refl_concat = os.path.join(working_dir, "tmp_sre_concat_" + l_sres + ".tif")
            param_reflectance_concat = {
                "il": l_ListOfL2SurfaceReflectanceFilenames,
                "out": tmp_refl_concat
            }
            sre_concat_app = OtbAppHandler("ConcatenateDoubleImages", param_reflectance_concat)
            # Multiply by quantification value
            tmp_sre_scale = os.path.join(working_dir, "tmp_sre_scale_" + l_sres + ".tif")
            param_scaled_sre = {"im": sre_concat_app.getoutput().get("out"),
                                "coef": l_ReflectanceQuantificationValue,
                                "out": tmp_sre_scale
                                }
            sre_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_sre)
            l_SREList.append(sre_scal_app.getoutput().get("out"))

            # Read the FRE Flat reflectance
            if self._m_WriteFRE:
                # Flat Reflectance reading
                LOGGER.debug("Start reading the Flat reflectance image filenames ...")
                l_ListOfL2FlatReflectanceFilenames = []
                l_NumberOfL2FlatReflectanceFilenames = 0
                if self._m_WriteFRE:
                    l_ListOfL2FlatReflectanceFilenames = p_L2XMLHandler.get_list_of_l2_flat_reflectance_filenames(
                        l_sres_mtd)
                    l_NumberOfL2FlatReflectanceFilenames = len(l_ListOfL2FlatReflectanceFilenames)
                # Concatenate bands
                tmp_flat_concat = os.path.join(working_dir, "tmp_fre_concat_" + l_sres + ".tif")
                param_flat_concat = {
                    "il": l_ListOfL2FlatReflectanceFilenames,
                    "out": tmp_flat_concat
                }
                fre_concat_app = OtbAppHandler("ConcatenateDoubleImages", param_flat_concat)
                # Multiply by quantification value
                tmp_fre_scale = os.path.join(working_dir, "tmp_fre_scale_" + l_sres + ".tif")
                param_scaled_fre = {"im": fre_concat_app.getoutput().get("out"),
                                    "coef": l_ReflectanceQuantificationValue,
                                    "out": tmp_fre_scale
                                    }
                fre_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_fre)
                l_FREList.append(fre_scal_app.getoutput().get("out"))

            # Read the ATB
            LOGGER.debug("L2ImageFileReaderBase::Initialize - ATB image filename: %s.",
                         p_L2XMLHandler.get_l2_atb_filename())
            l_ATBList.append(p_L2XMLHandler.get_l2_atb_filename())
            # Read the VAP
            l_VAPQuantificationValue = 1. / \
                float(p_L2XMLHandler.get_string_value_of("WaterVaporContentQuantificationValue"))
            tmp_vap_roi = os.path.join(working_dir, "tmp_vap_roi_" + l_sres + ".tif")
            tmp_vap_roi_app = extract_roi(
                p_L2XMLHandler.get_l2_atb_filename(l_sres_mtd), [
                    p_L2XMLHandler.get_l2_vap_index()], tmp_vap_roi, write_output=True)
            tmp_vap_scale = os.path.join(working_dir, "tmp_vap_scale_" + l_sres + ".tif")
            param_scaled_vap = {"im": tmp_vap_roi_app.getoutput()["out"],
                                "coef": l_VAPQuantificationValue,
                                "out": tmp_vap_scale
                                }
            vap_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_vap)
            l_VAPList.append(vap_scal_app.getoutput().get("out"))

            # Read the AOT
            l_AOTQuantificationValue = 1. / \
                float(p_L2XMLHandler.get_string_value_of("AerosolOpticalThicknessQuantificationValue"))

            tmp_aot_roi = os.path.join(working_dir, "tmp_aot_roi_" + l_sres + ".tif")
            tmp_aot_roi_app = extract_roi(p_L2XMLHandler.get_l2_atb_filename(), [p_L2XMLHandler.get_l2_aot_index()],
                                          tmp_aot_roi, write_output=True)
            tmp_aot_scale = os.path.join(working_dir, "tmp_aot_scale_" + l_sres + ".tif")
            param_scaled_aot = {"im": tmp_aot_roi_app.getoutput()["out"],
                                "coef": l_AOTQuantificationValue,
                                "out": tmp_aot_scale
                                }
            aot_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_aot)
            l_AOTList.append(aot_scal_app.getoutput().get("out"))

            # Read the SAT
            l_ListOfL2SATFilenames = p_L2XMLHandler.get_list_of_l2_sat_image_filenames(l_sres_mtd)
            isSameSatForBands = True
            l_strSatFileNameRef = l_ListOfL2SATFilenames[l_firstBandIdx]
            l_isSatSameFilesForBands = True

            for l_BandIdxL2 in range(len(listOfL2Bands)):
                # Get the L1 band index associated to the L2 band code
                l_StrBandIdL2 = listOfL2Bands[l_BandIdxL2]
                l2BandIdx = p_L2XMLHandler.get_index_of_band_code(l_StrBandIdL2)
                # Filenameverify
                if l_strSatFileNameRef != l_ListOfL2SATFilenames[l2BandIdx]:
                    l_isSatSameFilesForBands = False
            LOGGER.debug("l_isL2SatSameFilesForBands = " + str(l_isSatSameFilesForBands))

            if l_isSatSameFilesForBands:
                l_StrBandIdL2 = listOfL2Bands[0]
                l2BandIdx = p_L2XMLHandler.get_index_of_band_code(l_StrBandIdL2)
                l_SATFFilename = l_ListOfL2SATFilenames[l2BandIdx]
                sat_mask = os.path.join(working_dir, "L2_SAT_Masks_{}.tif".format(l_sres))
                param_bintovec_sat = {"im": l_SATFFilename,
                                      "out": sat_mask + ":uint8",
                                      "nbcomp": len(listOfL2Bands)
                                      }
                sat_mask_app = OtbAppHandler("BinaryToVector", param_bintovec_sat, write_output=True)
                l_SATList.append(sat_mask_app.getoutput().get("out"))
            else:
                raise MajaDataException(
                    "Product format not supported : not the same file for band on SAT in L2 product")

            # Read EDG
            LOGGER.debug("Start reading the EDG image")
            l_EDGList.append(p_L2XMLHandler.get_l2_edg_filename(l_sres_mtd))

            # Read MG2
            LOGGER.debug(
                "Start reading the MG2 <" +
                p_L2XMLHandler.get_l2_mg2_filename(l_sres_mtd) +
                "> mask filename...")
            tmp_vec_mg2 = os.path.join(working_dir, "tmp_mg2_vec_" + l_sres + ".tif")
            param_bin_mg2 = {"im": p_L2XMLHandler.get_l2_mg2_filename(l_sres_mtd),
                             "out": tmp_vec_mg2,
                             "nbcomp": 8}
            bin_mg2_app = OtbAppHandler("BinaryToVector", param_bin_mg2, write_output=True)
            # Read WAT from MG2
            LOGGER.debug("Start reading the WAT image")
            tmp_wat = os.path.join(working_dir, "tmp_wat_" + l_sres + ".tif")
            tmp_wat_app = extract_roi(bin_mg2_app.getoutput()["out"], [p_L2XMLHandler.get_l2_wat_index()], tmp_wat)
            l_WATList.append(tmp_wat_app.getoutput().get("out"))

            # Read SNW
            if p_PluginBase.SnowMasking:
                LOGGER.debug("Start reading the SNW image")
                tmp_snow = os.path.join(working_dir, "tmp_snow_" + l_sres + ".tif")
                tmp_snow_image = extract_roi(tmp_wat_app.getoutput()["out"], [p_L2XMLHandler.get_l2_snow_index()],
                                             tmp_snow)
                l_SNOWList.append(tmp_snow_image.getoutput().get("out"))

            # Read CLD
            LOGGER.debug("Start reading the CLD image")
            tmp_cld_vec = os.path.join(working_dir, "tmp_cld_vec_" + l_sres + ".tif")
            param_vec_cld = {"im": p_L2XMLHandler.get_l2_cld_filename(l_sres_mtd),
                             "out": tmp_cld_vec + ":double",
                             "nbcomp": len(p_PluginBase.CLDCoreAlgorithmsMapBand)}
            cld_vec_app = OtbAppHandler("BinaryToVector", param_vec_cld, write_output=False)
            # In this case some cld bits are not available in the data
            tmp_cld_zero = os.path.join(working_dir, "tmp_cld_zero_" + l_sres + ".tif")
            cld_const_zero_app = None
            if len(p_PluginBase.CLDCoreAlgorithmsMapBand) > len(p_PluginBase.CLDDataBandsSelected):
                cld_const_zero_app = constant_image(cld_vec_app.getoutput()["out"], "0",
                                               os.path.join(working_dir, "tmp_zero_cld_" + l_sres + ".tif"))
            l_dict_of_cld = dict()
            for b in list(p_PluginBase.CLDCoreAlgorithmsMapBand.keys()):
                if b in p_PluginBase.CLDDataBandsSelected:
                    tmp_cld_chan = os.path.join(working_dir, "tmp_" + b + "_" + l_sres + ".tif")
                    chan = p_PluginBase.CLDDataBandsSelected.index(b)
                    l_dict_of_cld[b] = extract_roi(tmp_cld_vec, [chan], tmp_cld_chan + ":uint8", working_dir).getoutput()["out"]
                else:
                    l_dict_of_cld[b] = cld_const_zero_app.getoutput()["out"]
            # For GetVectorizedCLDImageList method
            l_CLDList.append(l_dict_of_cld)
