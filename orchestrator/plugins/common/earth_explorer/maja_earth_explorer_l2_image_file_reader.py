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
import re

from orchestrator.common.maja_exceptions import MajaExceptionPluginBase
from orchestrator.plugins.common.base.maja_l2_image_reader_base import L2ImageReaderBase
from orchestrator.common.earth_explorer.header_image_earth_explorer_xml_file_handler\
    import HeaderImageEarthExplorerXMLFileHandler
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
from orchestrator.common.earth_explorer.earth_explorer_utilities import EarthExplorerUtilities
from orchestrator.plugins.common.base.maja_l2_image_filenames_provider import L2ImageFilenamesProvider
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.cots.otb.algorithms.otb_band_math import *
import orchestrator.common.date_utils as date_utils

LOGGER = configure_logger(__name__)


class EarthExplorerL2ImageFileReader(L2ImageReaderBase):

    def __init__(self):
        super(EarthExplorerL2ImageFileReader, self).__init__()
        self.dict_of_vals = {}

    @staticmethod
    def is_an_earth_explorer_l2_product_by_checking_the_filename(p_filename, plugin_base):
        """
        IsAnEarthExplorerL2ProductByCheckingTheFilename
        :return:
        """
        # Get the filename without the path
        # l_ShortFilename: ex: VE_TEST_VSC_L2VALD_CAMARGUE_20120101.DBL or VE_TEST_VSC_L2VALD_CAMARGUE_20120101.HDR
        l_ShortFilename = os.path.basename(p_filename)
        l_Extension = os.path.splitext(l_ShortFilename)[-1]

        # Look for the L2 product in the list of files with the keys "L2VALD" and ".HDR"
        # --------------------------------------------------------------------------------
        # FT OGM 1245 - FA 132484 : QCK_L2VALD is not a L2 EarthExplorer product
        # => Remove the QCK product from the list of L2 products
        if (re.search("_L2VALD_", l_ShortFilename) is not None and l_Extension == ".HDR" and
                re.search("_QCK_", l_ShortFilename) is None):
            # Initialize the filenames of the different plans contained in a L2 product
            l_FilenameProvider = L2ImageFilenamesProvider()
            result = l_FilenameProvider.can_initialize_for_reading(p_filename, plugin_base, False)

            if not result:
                LOGGER.debug("The product %s is not a L2 EarthExplorer product"
                             "(exception error throw while initializing the L2ImageFilenamesProvider) !", p_filename)
                return False
            else:
                return True

    @staticmethod
    def is_an_earth_explorer_l2_product_by_checking_the_satellite(p_filename, plugin_base):
        """
        IsAnEarthExplorerL2ProductByCheckingTheSatellite
        :return:
        """
        LOGGER.debug("File detected with the file naming <" + p_filename +
                     ">. Check with the field 'Platform' in the xml file...")
        l_Filename = os.path.splitext(p_filename)[0] + ".HDR"
        xml_handler = EarthExplorerXMLFileHandler(l_Filename)
        if plugin_base.is_valid_with_satellite(xml_handler.get_string_value_of("Mission")):
            LOGGER.debug("The filename " + l_Filename + " is a 'EarthExplorer' L2 product compatible with the plugin '")
            return True
        return False

    def earth_explorer_detect_l2_products(self, input_directory, list_of_product, p_plugin_base):
        list_of_file = [os.path.join(input_directory, f) for f in os.listdir(input_directory)
                        if os.path.isfile(os.path.join(input_directory, f))]
        for f in list_of_file:
            if self.is_an_earth_explorer_l2_product_by_checking_the_filename(f, p_plugin_base):
                if self.is_an_earth_explorer_l2_product_by_checking_the_satellite(f, p_plugin_base):
                    LOGGER.debug("Add the 'earth explorer' L2 product " + f + " in the list of products!")
                    list_of_product.append(f)

    def can_read_with_plugin_base(self, p_filename, p_plugin_base):
        """
        Return in the product is readable with the plugin Earth Explorer
        :return:
        """
        if self.is_an_earth_explorer_l2_product_by_checking_the_filename(p_filename, p_plugin_base):
            return self.is_an_earth_explorer_l2_product_by_checking_the_satellite(p_filename, p_plugin_base)
        return False

    def read_info_with_plugin_base(self, p_filename, p_EnableReadingPublicData, p_PluginBase):
        # ****  PRELIMINARY  DATA      ************************************************************
        l_L2ImagesFilenamesProvider = L2ImageFilenamesProvider()
        l_L2ImagesFilenamesProvider.initialize_for_reading(p_filename, p_PluginBase, False)
        # Set the ImageFilenamesProvider
        l_L2Handler = HeaderImageEarthExplorerXMLFileHandler(l_L2ImagesFilenamesProvider.get_hdr_filename())

        # Set attribute
        self.ProductFilename = p_filename
        self.GlobalProductFilename = l_L2ImagesFilenamesProvider.get_hdr_filename()
        self.Satellite = l_L2Handler.get_string_value_of("Mission")
        self.Acquisition_Date = l_L2Handler.get_acquisition_date()
        self.Validity_Start = l_L2Handler.get_string_value_of("ValidityStart")
        self.Validity_Stop = l_L2Handler.get_string_value_of("ValidityStop")
        self.Nick_Name = l_L2Handler.get_nick_name()
        self.Reference_SiteDefinition_Id = l_L2Handler.get_reference_site_definition_id()
        self.Date = date_utils.get_datetime_from_yyyymmdd(l_L2Handler.get_acquisition_date())
        self.Adjacency_Effects_And_Slope_Correction = l_L2Handler.get_adjacency_effects_and_slope_correction()
        self.Reflectance_Quantification_Value = l_L2Handler.get_reflectance_quantification_value()
        self.Nodata_Value = l_L2Handler.get_no_data_value_as_double()
        self.RealL2Nodata_Value = l_L2Handler.get_no_data_value_as_double() * l_L2Handler.get_reflectance_quantification_value()

        self.List_of_GIPP_Files = l_L2Handler.get_list_of_gipp_files()

        if p_EnableReadingPublicData:
            l_ATB_XMLHandler = HeaderImageEarthExplorerXMLFileHandler(
                l_L2ImagesFilenamesProvider.get_atb_header_filenames().at(0))
            self.VAP_Nodata_Value = l_ATB_XMLHandler.get_vap_no_data_value()
            self.RealVAP_Nodata_Value = l_ATB_XMLHandler.get_vap_no_data_value() * l_ATB_XMLHandler.get_vap_quantification_value()
            self.VAP_Quantification_Value = l_ATB_XMLHandler.get_vap_quantification_value()
            self.AOT_Nodata_Value = l_ATB_XMLHandler.get_aot_no_data_value()
            self.RealAOT_Nodata_Value = l_ATB_XMLHandler.get_aot_no_data_value() * l_ATB_XMLHandler.get_aot_quantification_value()
            self.AOT_Quantification_Value = l_ATB_XMLHandler.get_aot_quantification_value()

        self.PluginName = p_PluginBase.PluginName

        self.FileClass = l_L2ImagesFilenamesProvider.get_fileclass()
        self.LevelType = "L2VALD"

        # -------------------------------------------------------------------------
        # Get the projection ref
        # TODO cf avec alexia
        # self.ProjectionRef = l_L2ImagesFilenamesProvider->GetPrivateFileNames().GetRTAImageFileName())

        # Expand the path
        self.Prefix = EarthExplorerUtilities.get_satellite_class_from_filename(self.GlobalProductFilename)

    def read_with_plugin_base(self, p_filename, p_EnableReadingPublicData, app_handler, p_PluginBase):
        """

        :param p_filename:
        :param p_EnableReadingPublicData: bool
        :param p_PluginBase: PluginBase::Pointer
        :return:
        """
        working_dir = app_handler.get_directory_manager().get_temporary_directory("L2Read_",
                                            do_always_remove=True)

        # Set the ImageFilenamesProvider
        l_L2ImagesFilenamesProvider = L2ImageFilenamesProvider()
        l_L2ImagesFilenamesProvider.initialize_for_reading(p_filename, p_PluginBase, False)
        self.L2PrivateImageFilenamesProvider = l_L2ImagesFilenamesProvider.get_private_filenames()

        # Do caching ?
        caching = app_handler.get_user_conf().get_Computing().get_Caching().get_EnableCachingConvertReflectanceData()

        # Register the Header for the L2 Input image file
        l_L2Handler = HeaderImageEarthExplorerXMLFileHandler(l_L2ImagesFilenamesProvider.get_hdr_filename())
        l_ReflectanceQuantificationValue = l_L2Handler.get_reflectance_quantification_value()

        # Get the number of Resolution, by reading the number of SRE filename
        l_NumberOfResolutions = len(l_L2ImagesFilenamesProvider.get_fre_headers())
        LOGGER.debug("L2ImageFileReaderBase::Initialize Number of resolutions: %s.", l_NumberOfResolutions)

        self._m_WriteFRE = l_L2Handler.get_adjacency_effects_and_slope_correction()

        # ****  PUBLIC  DATA      *************************************************************************************
        LOGGER.debug("L2ImageFileReaderBase::Initialize - EnableReadingPublicData: %s.", p_EnableReadingPublicData)

        if p_EnableReadingPublicData:
            self.read_public_images(
                l_L2ImagesFilenamesProvider,
                l_ReflectanceQuantificationValue,
                p_PluginBase,
                working_dir)
        else:
            # ****  PRIVATE  DATA     ************************************************
            self.read_private_images(
                l_L2ImagesFilenamesProvider.get_private_filenames(),
                l_ReflectanceQuantificationValue,
                p_PluginBase,
                working_dir)

    def read_public_images(
            self,
            p_L2ImagesFilenamesProvider,
            p_ReflectanceQuantificationValue,
            p_PluginBase,
            working_dir):
        """
        ReadPublicImages
        :param p_L2ImagesFilenamesProvider:
        :param p_ReflectanceQuantificationValue:
        :param p_PluginBase:
        :return:
        """
        l_NumberOfResolutions = len(p_PluginBase.ListOfL2Resolutions)

        # ****  PUBLIC  DATA      ******

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
            l_sres = p_PluginBase.BandsDefinitions.ListOfL2Resolutions[resol]
            l_NumberOfComponentsPerPixel = len(p_PluginBase.BandsDefinitions.GetListOfL2BandCode(l_sres))

            # Register the ATB header for reading VAPQuantificationValue and AOTQuantificationValue
            l_ATB_XMLHandler = HeaderImageEarthExplorerXMLFileHandler(
                p_L2ImagesFilenamesProvider.get_atb_header_filename()[resol])
            l_VAP_QuantificationValue = l_ATB_XMLHandler.get_vap_quantification_value()
            l_AOT_QuantificationValue = l_ATB_XMLHandler.get_aot_quantification_value()

            # ** SRE Reader connection */
            LOGGER.debug("L2ImageFileReaderBase::Initialize - "
                         "SRE image filename: %s.", p_L2ImagesFilenamesProvider.get_sre_filenames()[resol])
            tmp_sre_scale = os.path.join(working_dir, "tmp_sre_scale_" + l_sres + ".tif")


            param_scaled_sre = {"im": p_L2ImagesFilenamesProvider.get_sre_filenames()[resol],
                                "coef": p_ReflectanceQuantificationValue,
                                "out": tmp_sre_scale
                                }
            sre_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_sre)
            l_SREList.append(tmp_sre_scale)

            # If the product contains FRE image
            if self._m_WriteFRE:
                tmp_fre_scale = os.path.join(working_dir, "tmp_fre_scale_" + l_sres + ".tif")
                param_scaled_fre = {"im": p_L2ImagesFilenamesProvider.get_fre_filenames()[resol],
                                    "coef": p_ReflectanceQuantificationValue,
                                    "out": tmp_fre_scale
                                    }
                fre_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_fre)
                l_FREList.append(tmp_fre_scale)

            # ATB Reader connection
            LOGGER.debug("L2ImageFileReaderBase::Initialize - ATB image filename: %s.",
                         p_L2ImagesFilenamesProvider.get_atb_image_filename()[resol])
            l_ATBList.append(p_L2ImagesFilenamesProvider.get_atb_image_filename()[resol])

            # VAP Reader connection (from ATB)
            tmp_vap = os.path.join(working_dir, "tmp_vap_" + l_sres + ".tif")
            tmp_vap_image_app = extract_roi(p_L2ImagesFilenamesProvider.get_atb_image_file_name()[resol], ["Channel1"],
                                            tmp_vap, write_output=False)
            tmp_vap_scale = os.path.join(working_dir, "tmp_vap_scale_" + l_sres + ".tif")
            param_scaled_vap = {"im": tmp_vap_image_app.getoutput()["out"],
                                "coef": l_VAP_QuantificationValue,
                                "out": tmp_vap_scale
                                }
            vap_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_vap)
            l_VAPList.append(tmp_vap_scale)

            # AOT Reader connection (from ATB)
            tmp_aot = os.path.join(working_dir, "tmp_aot_" + l_sres + ".tif")
            tmp_aot_image_app = extract_roi(p_L2ImagesFilenamesProvider.get_atb_image_file_name()[resol], ["Channel2"],
                                            tmp_aot, write_output=False)
            tmp_aot_scale = os.path.join(working_dir, "tmp_aot_scale_" + l_sres + ".tif")
            param_scaled_aot = {"im": tmp_aot_image_app.getoutput()["out"],
                                "coef": l_AOT_QuantificationValue,
                                "out": tmp_aot_scale
                                }
            aot_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_aot)
            l_AOTList.append(tmp_aot_scale)

            # QLT Reader connection */
            LOGGER.debug("L2ImageFileReaderBase::Initialize - QLT image filename: %s.",
                         p_L2ImagesFilenamesProvider.get_qlt_filenames()[resol])
            l_QLTList.append(p_L2ImagesFilenamesProvider.get_qlt_filenames()[resol])

            # SAT Reader connection (from QLT) */
            tmp_sat = os.path.join(working_dir, "tmp_sat_" + l_sres + ".tif")
            tmp_sat_image_app = extract_roi(p_L2ImagesFilenamesProvider.get_qlt_filenames()[resol], [0],
                                            tmp_sat, write_output=False)
            tmp_vec_sat = os.path.join(working_dir, "tmp_sat_vec_" + l_sres + ".tif")
            param_bin_sat = {"im": tmp_sat_image_app.getoutput()["out"],
                             "out": tmp_vec_sat,
                             "nbcomp": l_NumberOfComponentsPerPixel}
            bin_sat_app = OtbAppHandler("BinaryToVector", param_bin_sat)
            l_SATList.append(tmp_vec_sat)

            # EDG Reader connection (from QLT) */
            tmp_qoth = os.path.join(working_dir, "tmp_qoth_" + l_sres + ".tif")
            tmp_qoth_image = extract_roi(p_L2ImagesFilenamesProvider.get_qlt_filenames()[resol], [2],
                                         tmp_qoth, write_output=False)
            tmp_vec_qoth = os.path.join(working_dir, "tmp_qoth_vec_" + l_sres + ".tif")
            param_bin_qoth = {"im": tmp_qoth_image.getoutput()["out"],
                              "out": tmp_vec_qoth,
                              "nbcomp": 2}
            bin_qoth_app = OtbAppHandler("BinaryToVector", param_bin_qoth)
            tmp_edg = os.path.join(working_dir, "tmp_edg_" + l_sres + ".tif")
            tmp_edg_image = extract_roi(bin_qoth_app.getoutput()["out"], [0], tmp_edg, working_dir)
            l_EDGList.append(tmp_edg)

            # CLD Reader connection */
            tmp_cld_vec = os.path.join(working_dir, "tmp_cld_vec_" + l_sres + ".tif")
            param_vec_cld = {"im": p_L2ImagesFilenamesProvider.get_cld_image_filename(),
                             "out": tmp_cld_vec,
                             "nbcomp": len(p_PluginBase.CLDCoreAlgorithmsMapBand)}
            cld_vec_app = OtbAppHandler("BinaryToVector", param_vec_cld, write_output=False)
            # In this case some cld bits are not available in the data
            tmp_cld_zero = os.path.join(working_dir, "tmp_cld_zero_" + l_sres + ".tif")
            cld_const_zero_app = None
            if len(p_PluginBase.CLDCoreAlgorithmsMapBand) > len(p_PluginBase.CLDDataBandsSelected):
                cld_const_zero_app = band_math([cld_vec_app.getoutput()["out"]], "0",
                                               os.path.join(working_dir, "tmp_zero_cld_" + l_sres + ".tif"))
            l_dict_of_cld = {}
            for b in list(p_PluginBase.CLDCoreAlgorithmsMapBand.keys()):
                if b in p_PluginBase.CLDDataBandsSelected:
                    tmp_cld_chan = os.path.join(working_dir, "tmp_" + b + "_" + l_sres + ".tif")
                    chan = p_PluginBase.CLDDataBandsSelected.index(b)
                    l_dict_of_cld[b] = extract_roi(tmp_cld_vec, [chan], tmp_cld_chan, working_dir).getoutput()["out"]
                else:
                    l_dict_of_cld[b] = cld_const_zero_app.getoutput()["out"]
            # For GetVectorizedCLDImageList method
            l_CLDList.append(l_dict_of_cld)

            # MSK Reader connection */
            LOGGER.debug("L2ImageFileReaderBase::Initialize - MSK image filename: %s.",
                         p_L2ImagesFilenamesProvider.get_msk_filename()[resol])
            l_XMLHandler = EarthExplorerXMLFileHandler(p_L2ImagesFilenamesProvider.get_msk_header_file_name()[resol])
            # For Nominal case Spectral camera MSK contains the WAT, HID, SHD, STL and TGS masks
            # If Snow masking is available (For ex for Sentinel2 and Landsat, S4, L8),
            # MSK contains the WAT, HID, SHD, STL, TGS and SNW masks
            l_MSKHDRFilename = p_L2ImagesFilenamesProvider.get_msk_header_file_name()[resol]
            l_MSKNumberOfComponentsPerPixel = int(l_XMLHandler.get_string_value_of("SignificantBits"))
            # l_MSKNumberOfComponentsPerPixel = 5 or 6 if SNW
            LOGGER.debug(
                "The MSK header file <%s> have '%s' number of significant bits = NumberOfComponentsPerPixel.",
                l_MSKHDRFilename,
                l_MSKNumberOfComponentsPerPixel)
            tmp_vec_moth = os.path.join(working_dir, "tmp_moth_vec_" + l_sres + ".tif")
            param_bin_moth = {"im": p_L2ImagesFilenamesProvider.get_msk_filename()[resol],
                              "out": tmp_vec_moth,
                              "nbcomp": l_MSKNumberOfComponentsPerPixel}
            bin_moth_app = OtbAppHandler("BinaryToVector", param_bin_moth, write_output=False)

            # Extract WAT from moth
            tmp_wat = os.path.join(working_dir, "tmp_wat_" + l_sres + ".tif")
            tmp_wat_image = extract_roi(bin_moth_app.getoutput()["out"], [0], tmp_wat)
            l_WATList.append(tmp_wat_image)

            if p_PluginBase.SnowMasking:
                if l_MSKNumberOfComponentsPerPixel < 6:
                    raise MajaExceptionPluginBase(
                        "Error while reading the SNOW band in the MSK product. The SNOW band number is '6', but the MSK have only '{}' bands!".format(l_MSKNumberOfComponentsPerPixel))
                # TODO: const unsigned int l_IndexSNWChannel(6)
                tmp_snow = os.path.join(working_dir, "tmp_snow_" + l_sres + ".tif")
                tmp_snow_image = extract_roi(tmp_vec_moth, [5], tmp_snow)
                l_SNOWList.append(tmp_snow_image)
