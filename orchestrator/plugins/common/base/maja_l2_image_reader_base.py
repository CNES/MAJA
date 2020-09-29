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
from orchestrator.common.maja_exceptions import *
from orchestrator.common.earth_explorer.header_image_earth_explorer_xml_file_handler \
    import HeaderImageEarthExplorerXMLFileHandler
from orchestrator.common.earth_explorer.gipp_lut_earch_explorer_xml_file_handler \
    import GippLUTEarthExplorerXMLFileHandler
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
import orchestrator.common.maja_utils as maja_utils
import orchestrator.common.file_utils as file_utils
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
from orchestrator.common.interfaces.maja_xml_app_lutmap import *
import orchestrator.common.date_utils as date_utils
import os
import io

LOGGER = configure_logger(__name__)

L2_READER_KEYS = []


class L2ImageReaderBase(object):

    def __init__(self):
        self.Date = None
        self.Satellite = ""
        self.Acquisition_Date = ""
        self.Validity_Start = ""
        self.Validity_Stop = ""
        self.Nick_Name = ""
        self.Reference_SiteDefinition_Id = ""
        self.Adjacency_Effects_And_Slope_Correction = False
        self.Reflectance_Quantification_Value = ""
        self.Nodata_Value = None
        self.RealL2Nodata_Value = None
        self.VAP_Nodata_Value = None
        self.RealVAP_Nodata_Value = None
        self.VAP_Quantification_Value = None
        self.AOT_Nodata_Value = None
        self.RealAOT_Nodata_Value = None
        self.AOT_Quantification_Value = None
        self.ProjectionRef = ""
        self.List_of_GIPP_Files = []
        self.PluginName = ""
        self.FileClass = ""
        self.LevelType = ""
        self.Prefix = ""
        self.ListOfBandTheoreticalWavelengthOfTheReflectancesComposite = []
        self.ListOfBandTheoreticalWavelengthOfTheLTCComposite = []
        self.STOBandTheoreticalWavelengthIsPresent = False
        self.STOBandTheoreticalWavelengthValue = 0
        self.GlobalProductFilename = None
        self.ProductFilename = ""
        self.STOListOfStringDates = []
        self.L2PrivateImageFilenamesProvider = None
        self.dict_of_vals = {}
        self._coarse_pipeline = OtbPipelineManager()

    def read_info(self, product_filename, enable_public_data):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def can_read(self, product_filename):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def read(self, product_filename, app_handler, enable_public_data):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def detect_l2_products(self, dir, product_list):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def get_value(self, key):
        if key in self.dict_of_vals:
            return self.dict_of_vals.get(key)
        else:
            raise MajaDriverException("No " + key + " available in the reader.")

    def read_public_images(
            self,
            p_L2ImagesFilenamesProvider,
            p_ReflectanceQuantificationValue,
            p_PluginBase,
            working_dir):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def read_private_images(
            self,
            p_L2PrivateImageFilenamesProvider,
            p_ReflectanceQuantificationValue,
            p_PluginBase,
            working_dir):
        l_NumberOfComponentsPerPixelForPWAAndTWA = len(p_PluginBase.BandsDefinitions.L2CoarseBandMap)

        # *************************************************************************************************************
        # Register the Header for the L2 Input image file
        # *************************************************************************************************************
        l_ReflectanceQuantificationValue = p_ReflectanceQuantificationValue
        # Read the  list of wavelength of the RTA/RTC and RCR list of bands in the HDR xml header file
        l_RTAHeaderFilename = p_L2PrivateImageFilenamesProvider.get_rta_header_filename()
        l_RTAHandler = HeaderImageEarthExplorerXMLFileHandler(l_RTAHeaderFilename)
        self.ListOfBandTheoreticalWavelengthOfTheReflectancesComposite = l_RTAHandler.get_list_of_band_theoretical_wavelength()

        # ********************************************************************************************************/
        # #* RTA Reader connection */
        LOGGER.debug("L2ImageFileReaderBase::ReadPrivateImages - RTA image filename: " +
                     p_L2PrivateImageFilenamesProvider.get_rta_image_filename() + ".")
        tmp_rta_scale = os.path.join(working_dir, "tmp_rta_scale.tif")
        param_scaled_rta = {"im": p_L2PrivateImageFilenamesProvider.get_rta_image_filename(),
                            "coef": l_ReflectanceQuantificationValue,
                            "out": tmp_rta_scale
                            }
        rta_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_rta, write_output=True)
        self._coarse_pipeline.add_otb_app(rta_scal_app)
        self.dict_of_vals["RTAImage"] = rta_scal_app.getoutput()["out"]
        # ********************************************************************************************************/
        # * RTC Reader connection */
        # ********************************************************************************************************/
        LOGGER.debug("L2ImageFileReaderBase::ReadPrivateImages - RTC image filename: " +
                     p_L2PrivateImageFilenamesProvider.get_rtc_image_filename() + ".")
        tmp_rtc_scale = os.path.join(working_dir, "tmp_rtc_scale.tif")
        param_scaled_rtc = {"im": p_L2PrivateImageFilenamesProvider.get_rtc_image_filename(),
                            "coef": l_ReflectanceQuantificationValue,
                            "out": tmp_rtc_scale
                            }
        rtc_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_rtc, write_output=True)
        self._coarse_pipeline.add_otb_app(rtc_scal_app)
        self.dict_of_vals["RTCImage"] = rtc_scal_app.getoutput()["out"]
        # ********************************************************************************************************/
        # * RCR Reader connection */
        # ********************************************************************************************************/
        LOGGER.debug("L2ImageFileReaderBase::ReadPrivateImages - RCR image filename: " +
                     p_L2PrivateImageFilenamesProvider.get_rcr_image_filename() + ".")
        tmp_rcr_scale = os.path.join(working_dir, "tmp_rcr_scale.tif")
        param_scaled_rcr = {"im": p_L2PrivateImageFilenamesProvider.get_rcr_image_filename(),
                            "coef": l_ReflectanceQuantificationValue,
                            "out": tmp_rcr_scale
                            }
        rcr_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_rcr, write_output=True)
        self._coarse_pipeline.add_otb_app(rcr_scal_app)
        self.dict_of_vals["RCRImage"] = rcr_scal_app.getoutput()["out"]
        # ********************************************************************************************************/
        # * PXD Reader connection */
        # ********************************************************************************************************/
        LOGGER.debug("L2ImageFileReaderBase::ReadPrivateImages - PXD image filename: " +
                     p_L2PrivateImageFilenamesProvider.get_pxd_image_filename() + ".")
        self.dict_of_vals["PXDImage"] = p_L2PrivateImageFilenamesProvider.get_pxd_image_filename()

        # ********************************************************************************************************/
        # * WAS Reader connection (WAM product) */
        # ********************************************************************************************************/
        tmp_was = os.path.join(working_dir, "tmp_was.tif")
        was_extr_app = extract_roi(
            p_L2PrivateImageFilenamesProvider.get_wam_image_filename(),
            [0],
            tmp_was,
            write_output=True)
        self._coarse_pipeline.add_otb_app(was_extr_app)
        self.dict_of_vals["WASImage"] = was_extr_app.getoutput()["out"]
        # ********************************************************************************************************/
        # * PWA Reader connection (WAM product) */
        # ********************************************************************************************************/
        tmp_pwa = os.path.join(working_dir, "tmp_pwa.tif")
        pwa_image_app = extract_roi(p_L2PrivateImageFilenamesProvider.get_wam_image_filename(),
                                    [1], tmp_pwa + ":uint8", write_output=False)
        self._coarse_pipeline.add_otb_app(pwa_image_app)
        tmp_vecpwa = os.path.join(working_dir, "tmp_pwa_vec.tif")
        param_binpwa = {"im": pwa_image_app.getoutput()["out"],
                        "out": tmp_vecpwa,
                        "nbcomp": l_NumberOfComponentsPerPixelForPWAAndTWA}
        binpwa_app = OtbAppHandler("BinaryToVector", param_binpwa, write_output=True)
        self._coarse_pipeline.add_otb_app(binpwa_app)
        self.dict_of_vals["PWAImage"] = binpwa_app.getoutput()["out"]

        # ********************************************************************************************************/
        # * TWA Reader connection (WAM product) */
        # ********************************************************************************************************/
        tmp_twa = os.path.join(working_dir, "tmp_twa.tif")
        twa_image = extract_roi(
            p_L2PrivateImageFilenamesProvider.get_wam_image_filename(),
            [2],
            tmp_twa + ":uint8",
            write_output=False)
        tmp_vectwa = os.path.join(working_dir, "tmp_twa_vec.tif")
        param_bintwa = {"im": twa_image.getoutput().get("out"),
                        "out": tmp_vectwa,
                        "nbcomp": l_NumberOfComponentsPerPixelForPWAAndTWA}
        bintwa_app = OtbAppHandler("BinaryToVector", param_bintwa, write_output=True)
        self._coarse_pipeline.add_otb_app(bintwa_app)
        self.dict_of_vals["TWAImage"] = bintwa_app.getoutput()["out"]

        # ********************************************************************************************************/
        # * STO Reader connection */
        # ********************************************************************************************************/
        l_STOHeaderFilename = p_L2PrivateImageFilenamesProvider.get_sto_header_filename()
        l_STOHandler = HeaderImageEarthExplorerXMLFileHandler(l_STOHeaderFilename)

        try:
            self.STOBandTheoreticalWavelengthIsPresent = (l_STOHandler.get_theoretical_wavelength() is not None)
            self.STOBandTheoreticalWavelengthValue = l_STOHandler.get_theoretical_wavelength()
        except BaseException:
            LOGGER.debug("Could not retrive STO wavelength !!!")

        # 4.3: Get the lis of date (in Bands nodes)
        self.STOListOfStringDates = l_STOHandler.get_list_of_bands()

        LOGGER.debug("L2ImageFileReaderBase::ReadPrivateImages - STO image filename: " +
                     p_L2PrivateImageFilenamesProvider.get_sto_image_filename() + ".")
        tmp_sto_scale = os.path.join(working_dir, "tmp_sto_scale.tif")
        param_scaled_sto = {"im": p_L2PrivateImageFilenamesProvider.get_sto_image_filename(),
                            "coef": l_ReflectanceQuantificationValue,
                            "out": tmp_sto_scale
                            }
        sto_scal_app = OtbAppHandler("MultiplyByScalar", param_scaled_sto, write_output=True)
        self._coarse_pipeline.add_otb_app(sto_scal_app)
        self.dict_of_vals["STOImage"] = sto_scal_app.getoutput()["out"]
        # ********************************************************************************************************/
        # * NDT Reader connection */
        # ********************************************************************************************************/
        LOGGER.debug("L2ImageFileReaderBase::ReadPrivateImages - NDT image filename: " +
                     p_L2PrivateImageFilenamesProvider.get_ndt_image_filename() + ".")
        self.dict_of_vals["NDTImage"] = p_L2PrivateImageFilenamesProvider.get_ndt_image_filename()

        # Initialize the date. In plugin sub classes.
        # m_Date = ....

# 4-1-0 : HandlingConnectionCLD DM 1039
#            #********************************************************************************************************/
#            #* CLD sub (summary) Reader connection */
#            #********************************************************************************************************/
        LOGGER.debug("L2ImageFileReaderBase::ReadPrivateImages - CLD image filename: " +
                     p_L2PrivateImageFilenamesProvider.get_cld_image_filename() + ".")

        # TODO ATTENTION m_CLDSubOutput est la sortie du Reader (donc pas toute le
        # s 9 bandes algo, ou laors sortie du m_ReorganizeBinaryCLDSub ??!!!
        self.dict_of_vals["CLDSubImage"] = p_L2PrivateImageFilenamesProvider.get_cld_image_filename()

        # MACCS 5.0.0 : DM 982 . Need CLD bands (for example extract Shadow and ShadVar bands in L2CoarseResolution)

        # For GetVectorizedCLDImageList method
        tmp_cld_vec = os.path.join(working_dir, "tmp_cld_vec.tif")
        param_vec_cld = {"im": p_L2PrivateImageFilenamesProvider.get_cld_image_filename(),
                         "out": tmp_cld_vec,
                         "nbcomp": len(p_PluginBase.CLDCoreAlgorithmsMapBand)}
        cld_vec_app = OtbAppHandler("BinaryToVector", param_vec_cld, write_output=False)
        self._coarse_pipeline.add_otb_app(cld_vec_app)
        cld_vec_image = cld_vec_app.getoutput()["out"]
        # In this case some cld bits are not available in the data
        tmp_cld_zero = os.path.join(working_dir, "tmp_cld_zero.tif")
        cld_const_zero_app = None
        if len(p_PluginBase.CLDCoreAlgorithmsMapBand) > len(p_PluginBase.CLDDataBandsSelected):
            cld_const_zero_app = constant_image(
                cld_vec_image, 0, os.path.join(
                    working_dir, "tmp_zero_cld.tif"), write_output=False)
            self._coarse_pipeline.add_otb_app(cld_const_zero_app)
        self.dict_of_vals["VectorizedCLDSubOutput"] = {}
        for b in list(p_PluginBase.CLDCoreAlgorithmsMapBand.keys()):
            if b in p_PluginBase.CLDDataBandsSelected:
                tmp_cld_chan = os.path.join(working_dir, "tmp_" + b + ".tif")
                chan = p_PluginBase.CLDDataBandsSelected.index(b)
                app_extract_cld = extract_roi(cld_vec_image, [chan], tmp_cld_chan, write_output=False)
                self._coarse_pipeline.add_otb_app(app_extract_cld)
                self.dict_of_vals["VectorizedCLDSubOutput"][b] = app_extract_cld.getoutput()["out"]
            else:
                self.dict_of_vals["VectorizedCLDSubOutput"][b] = cld_const_zero_app.getoutput()["out"]

        # *************************************************************************************************************
        #  Generate a map of LUT containing all TOCR miniLUTs of the L2 product at D-1 (L2inTOCR)
        # *************************************************************************************************************
        if p_L2PrivateImageFilenamesProvider.get_dealing_ltc():
            LOGGER.debug("L2ImageFileReaderBase::ReadPrivateImages - Start reading LTC ...")
            l_Filename = p_L2PrivateImageFilenamesProvider.get_ltc_header_filename()
            # --------------------------------------------
            # Load the XML file
            LOGGER.debug("L2ImageFileReaderBase::Start loading the LTC header filename <" + l_Filename + "> ....")
            l_LUTXMLHandler = GippLUTEarthExplorerXMLFileHandler(l_Filename)

            self.ListOfBandTheoreticalWavelengthOfTheLTCComposite = \
                l_LUTXMLHandler.get_list_of_band_theoretical_wavelength()

            # Read the files
            l_L2P_LTC_ListOfFilenames = l_LUTXMLHandler.get_list_of_packaged_dbl_files(True, True)

            # convert the lut indexes
            new_lut_indexes = IndexesType()
            for ind, vals in list(l_LUTXMLHandler.get_lut_indexes().items()):
                if ind == "Solar_Zenith_Angle_Indexes":
                    new_lut_indexes.set_Solar_Zenith_Angle_Indexes(vals)
                elif ind == "Relative_Azimuth_Angle_Indexes":
                    new_lut_indexes.set_Relative_Azimuth_Angle_Indexes(vals)
                elif ind == "Altitude_Indexes":
                    new_lut_indexes.set_Altitude_Indexes(vals)
                elif ind == "AOT_Indexes":
                    new_lut_indexes.set_AOT_Indexes(vals)
                elif ind == "TOA_Reflectance_Indexes":
                    new_lut_indexes.set_TOA_Reflectance_Indexes(vals)
                elif ind == "Zenithal_Angle_Indexes":
                    new_lut_indexes.set_Zenithal_Angle_Indexes(vals)
                elif ind == "View_Zenith_Angle_Indexes":
                    new_lut_indexes.set_View_Zenith_Angle_Indexes(vals)
                elif ind == "Reflectance_Ratio_Indexes":
                    new_lut_indexes.set_Reflectance_Ratio_Indexes(vals)
                else:
                    raise MajaDataException("Unhandled lut index type : " + ind)

            # Initialize the LookUpTable reader
            # --------------------------------------------
            # Set the list of minilut filenames to the mapReader
            l_lutmap = LUTMap()
            l_JdayRef = date_utils.get_julianday_as_int(
                date_utils.get_datetime_from_yyyymmdd(p_PluginBase.ReferenceDate))

            for f in l_L2P_LTC_ListOfFilenames:
                # Generate the LUT Map
                # --------------------------------------------
                l_jday = maja_utils.get_integer_jday_from_filename(f)
                if l_jday < l_JdayRef:
                    raise MajaDataException(
                        "Cannot fill the LookUpTable with filename <" +
                        f +
                        "> detected at Julian Day " +
                        l_jday +
                        " as it's prior to reference date " +
                        p_PluginBase.ReferenceDate +
                        " (" +
                        l_JdayRef +
                        ").")
                l_jday = int(l_jday - l_JdayRef)
                LOGGER.debug("l_jday " + str(l_jday) + " from filename " + f)
                LOGGER.debug("l_JDayRef : "+str(l_JdayRef))
                # Add cr lut to map
                l_listoffile = List_Of_FilesType()
                l_listoffile.add_Relative_File_Path("LTC_Lut_" + str(l_jday) + ".mha")
                file_utils.copy_file(f, os.path.join(working_dir, "LTC_Lut_" + str(l_jday) + ".mha"))
                l_lut = LUT(index=str(l_jday), Indexes=new_lut_indexes, List_Of_Files=l_listoffile)
                l_lutmap.add_LUT(l_lut)
            # Write down the lut map
            output = io.StringIO()
            output.write('<?xml version="1.0" ?>\n')

            l_lutmap.export(output, 0, name_='LUTMap', namespacedef_='', pretty_print=True)
            l_lutmap_filename = os.path.join(working_dir, "LTC_LutMap.xml")
            with open(l_lutmap_filename, "w") as fh:
                fh.write(output.getvalue().replace("    ", "  "))
                LOGGER.info("Writed new gipp lutmap to " + l_lutmap_filename)

            output.close()
            self.dict_of_vals["LTCImage"] = l_lutmap_filename
