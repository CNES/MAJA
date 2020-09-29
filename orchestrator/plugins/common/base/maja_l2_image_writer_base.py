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
#                        o  o  o o    o      o o    o     |||)<
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
from orchestrator.common.maja_exceptions import MajaNotYetImplemented
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.cots.otb.algorithms.otb_multiply_by_scalar import multiply_by_scalar
from orchestrator.cots.otb.algorithms.otb_rescale import rescale_intensity
from orchestrator.cots.otb.algorithms.otb_resample import resample
import orchestrator.common.file_utils as file_utils
from orchestrator.plugins.common.base.maja_plugin_base import PluginBase
import orchestrator.cots.otb.otb_file_utils as otb_file_utils
from orchestrator.cots.otb.algorithms.otb_resample import OtbResampleType
import os
LOGGER = configure_logger(__name__)


class L2ImageWriterBase(object):

    def __init__(self):
        self._l1_image_info = None
        self._l2_image_reader = None
        self._plugin = None
        self._copyprivatefroml2 = False
        self._l2imagereader = None
        self._dtm = None
        self._write_l2_products = None
        self._l2_output_directory = None
        self._real_l2_no_data = None
        self._reflectancequantification = None
        self._aotquantificationvalue = None
        self._vapquantificationvalue = None
        self._vapnodatavalue = None
        self._aotnodatavalue = None
        self._quicklookminreflredband = None
        self._quicklookminreflgreenband = None
        self._quicklookminreflblueband = None
        self._quicklookmaxreflredband = None
        self._quicklookmaxreflgreenband = None
        self._quicklookmaxreflblueband = None
        self._quicklookredbandcode = None
        self._quicklookgreenbandcode = None
        self._quicklookbluebandcode = None
        self._projectionref = None
        self._writepublicproduct = None
        self._envcoroption = None
        self._sre_list = None
        self._fre_list = None
        self._stl_list = None
        self._tgs_list = None
        self._l2aotlist = None
        self._l2aotmasklist = None
        self._l2vapimagelist = None
        self._l2vapmasklist = None
        self._l2iwcmasklist = None
        self._l2taomasklist = None
        self._dtm_hid = None
        self._dtm_shd = None
        self._cld_snow = None
        self._l2satimagelist = None
        self._l2piximagelist = None
        self._l2edgimagelist = None
        self._l2dfpimagelist = None
        self._writeonlyclacld = False
        self._cld = None
        self._l2cldlist = None
        self._cld_l2cla = None
        self._wasimage = None
        self._rta = None
        self._rtc = None
        self._rcr = None
        self._sto = None
        self._pxd = None
        self._ndt = None
        self._possiblewatermask = None
        self._testedwatermask = None
        self._dealingltc = None
        self._ltc_image = None
        self._global_product_filename = ""
        self._production_report_filename = ""
        self._l2privateimagefilenamesprovider = None

    def set_dtm(self, dtm):
        self._dtm = dtm

    def set_current_plugin_base(self, plugin):
        self._plugin = plugin

    def set_copy_private_from_l2_input_to_l2_output(self, l_copyprivatefroml2inputtol2output):
        self._copyprivatefroml2 = l_copyprivatefroml2inputtol2output

    def set_write_l2_products(self, l_writel2products):
        self._write_l2_products = l_writel2products

    def set_output_directory(self, l2outputdirectory):
        self._l2_output_directory = l2outputdirectory

    def set_l2_image_file_reader(self, l2reader):
        self._l2_image_reader = l2reader

    def set_l1_image_informations_provider(self, l1info):
        self._l1_image_info = l1info

    def set_real_l2_nodata(self, reall2nodata):
        self._real_l2_no_data = reall2nodata

    def set_reflectance_quantification_value(self, reflectancequantification):
        self._reflectancequantification = reflectancequantification

    def set_aot_quantification_value(self, aotquantificationvalue):
        self._aotquantificationvalue = aotquantificationvalue

    def set_vap_quantification_value(self, vapquantificationvalue):
        self._vapquantificationvalue = vapquantificationvalue

    def set_vap_nodata_value(self, vapnodatavalue):
        self._vapnodatavalue = vapnodatavalue

    def set_aot_nodata_value(self, aotnodatavalue):
        self._aotnodatavalue = aotnodatavalue

    def set_quicklook_min_refl_redband(self, quicklookminreflredband):
        if quicklookminreflredband[1] == '0':
            self._quicklookminreflredband = quicklookminreflredband[0:1] + quicklookminreflredband[2:len(quicklookminreflredband)]
        else:
            self._quicklookminreflredband = quicklookminreflredband

    def set_quicklook_min_refl_greenband(self, quicklookminreflgreenband):
        if quicklookminreflgreenband[1] == '0':
            self._quicklookminreflgreenband = quicklookminreflgreenband[0:1] + quicklookminreflgreenband[2:len(quicklookminreflgreenband)]
        else:
            self._quicklookminreflgreenband = quicklookminreflgreenband

    def set_quicklook_min_refl_blueband(self, quicklookminreflblueband):
        if quicklookminreflblueband[1] == '0':
            self._quicklookminreflblueband = quicklookminreflblueband[0:1] + quicklookminreflblueband[2:len(quicklookminreflblueband)]
        else:
            self._quicklookminreflblueband = quicklookminreflblueband

    def set_quicklook_max_refl_redband(self, quicklookmaxreflredband):
        if quicklookmaxreflredband[1] == '0':
            quicklookmaxreflredband = quicklookmaxreflredband[0:1] + quicklookmaxreflredband[2:len(quicklookmaxreflredband)]
        self._quicklookmaxreflredband = quicklookmaxreflredband

    def set_quicklook_max_refl_greenband(self, quicklookmaxreflgreenband):
        if quicklookmaxreflgreenband[1] == '0':
            self._quicklookmaxreflgreenband = quicklookmaxreflgreenband[0:1] + quicklookmaxreflgreenband[2:len(quicklookmaxreflgreenband)]
        else:
            self._quicklookmaxreflgreenband = quicklookmaxreflgreenband

    def set_quicklook_max_refl_blueband(self, quicklookmaxreflblueband):
        if quicklookmaxreflblueband[1] == '0':
            self._quicklookmaxreflblueband = quicklookmaxreflblueband[0:1] + quicklookmaxreflblueband[2:len(quicklookmaxreflblueband)]
        else:
            self._quicklookmaxreflblueband = quicklookmaxreflblueband

    def set_quicklook_red_band_code(self, quicklookredbandcode):
        if quicklookredbandcode[1] == '0':
            self._quicklookredbandcode = quicklookredbandcode[0:1] + quicklookredbandcode[2:len(quicklookredbandcode)]
        else:
            self._quicklookredbandcode = quicklookredbandcode

    def set_quicklook_green_band_code(self, quicklookgreenbandcode):
        if quicklookgreenbandcode[1] == '0':
            self._quicklookgreenbandcode = quicklookgreenbandcode[0:1] + quicklookgreenbandcode[2:len(quicklookgreenbandcode)]
        else:
            self._quicklookgreenbandcode = quicklookgreenbandcode

    def set_quicklook_blue_band_code(self, quicklookbluebandcode):
        if quicklookbluebandcode[1] == '0':
            self._quicklookbluebandcode = quicklookbluebandcode[0:1] + quicklookbluebandcode[2:len(quicklookbluebandcode)]
        else:
            self._quicklookbluebandcode = quicklookbluebandcode

    def set_projection_ref(self, projectionref):
        self._projectionref = projectionref

    def set_write_public_product(self, l_writel2producttol2resolution):
        self._writepublicproduct = l_writel2producttol2resolution

    def set_env_cor_option(self, l_envcoroption):
        self._envcoroption = l_envcoroption

    def set_sre_image_list(self, sre_list):
        self._sre_list = sre_list

    def set_fre_image_list(self, fre_list):
        self._fre_list = fre_list

    def set_stl_image_list(self, stl_list):
        self._stl_list = stl_list

    def set_tgs_image_list(self, tgs_list):
        self._tgs_list = tgs_list

    def set_aot_image_list(self, l2aotlist):
        self._l2aotlist = l2aotlist

    def set_tao_image_list(self, l2taomasklist):
        self._l2taomasklist = l2taomasklist

    def set_vap_image_list(self, l2vapimagelist):
        self._l2vapimagelist = l2vapimagelist

    def set_iwc_image_list(self, l2iwcmasklist):
        self._l2iwcmasklist = l2iwcmasklist

    def set_hid_image(self, dtm_hid):
        self._dtm_hid = dtm_hid

    def set_shd_image(self, dtm_shd):
        self._dtm_shd = dtm_shd

    def set_snw_image(self, cld_snow):
        self._cld_snow = cld_snow

    def set_sat_image_list(self, l2satimagelist):
        self._l2satimagelist = l2satimagelist

    def set_pix_image_list(self, l2piximagelist):
        self._l2piximagelist = l2piximagelist

    def set_edg_image_list(self, l2edgimagelist):
        self._l2edgimagelist = l2edgimagelist

    def set_dfp_image_list(self, l2dfpimagelist):
        self._l2dfpimagelist = l2dfpimagelist

    def set_write_only_cla_cld(self, writeonlyclacld):
        self._writeonlyclacld = writeonlyclacld

    def set_cld_image(self, cld_vec):
        self._cld = cld_vec

    def set_l2_cld_image_list(self, l2cldlist):
        self._l2cldlist = l2cldlist

    def set_cla_image(self, cld_l2cla):
        self._cld_l2cla = cld_l2cla

    def set_was_image(self, wasimage):
        self._wasimage = wasimage

    def set_rta_image(self, composite_rta):
        self._rta = composite_rta

    def set_rtc_image(self, composite_rtc):
        self._rtc = composite_rtc

    def set_rcr_image(self, composite_rcr):
        self._rcr = composite_rcr

    def set_sto_image(self, composite_sto):
        self._sto = composite_sto

    def set_pxd_image(self, composite_pxd):
        self._pxd = composite_pxd

    def set_ndt_image(self, composite_ndt):
        self._ndt = composite_ndt

    def set_pwa_image(self, possiblewatermask):
        self._possiblewatermask = possiblewatermask

    def set_twa_image(self, testedwatermask):
        self._testedwatermask = testedwatermask

    def set_write_ltc(self, dealingltc):
        self._dealingltc = dealingltc

    def set_ltc_image(self, ltc_image):
        self._ltc_image = ltc_image

    def get_global_product_filename(self):
        return self._global_product_filename

    def get_production_report_filename(self):
        return self._production_report_filename

    def get_private_filenames_provider(self):
        return self._l2privateimagefilenamesprovider

    def can_write(self, plugin_name):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def initialize(self, l2privateimagefilenames):
        if self._copyprivatefroml2:
            LOGGER.debug("Copy Input L2 private to output private data...")
            # If backward mode, if the cloud rate is too high an output L2 product must be generated
            # A copy of the L2 input product(D - 1) is done to continue the iteration
            l_l2privateimagefilenamesprovider = self._l2_image_reader.L2PrivateImageFilenamesProvider

            #create directory
            file_utils.create_directory(os.path.dirname(l2privateimagefilenames.get_rta_image_filename()))

            # Copy all data from private directory
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_rta_image_filename(),
                l2privateimagefilenames.get_rta_image_filename())
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_rtc_image_filename(),
                l2privateimagefilenames.get_rtc_image_filename())
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_rcr_image_filename(),
                l2privateimagefilenames.get_rcr_image_filename())
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_sto_image_filename(),
                l2privateimagefilenames.get_sto_image_filename())
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_pxd_image_filename(),
                l2privateimagefilenames.get_pxd_image_filename())
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_ndt_image_filename(),
                l2privateimagefilenames.get_ndt_image_filename())
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_cld_image_filename(),
                l2privateimagefilenames.get_cld_image_filename())
            #CLA can be optional
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_cla_image_filename(),
                l2privateimagefilenames.get_cla_image_filename(),raise_exceptions=False)
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_wam_image_filename(),
                l2privateimagefilenames.get_wam_image_filename())
            #LTC writing can be optional
            otb_file_utils.otb_copy_image_to_file(
                l_l2privateimagefilenamesprovider.get_ltc_image_filename(),
                l2privateimagefilenames.get_ltc_image_filename(), raise_exceptions=False)

    def initialize_product(self):
        LOGGER.debug("vnsL2ImageFileWriterBase::InitializeProduct()")

    def write(self, working_dir):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def write_qlt_product(self, p_res, p_qlt_image_filename, working_dir):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def write_cld_image(self, p_cldimages, p_clddatabandsselected, cld_image_filename, use_filenames=False):
        il = []
        for c in p_clddatabandsselected:
            band = self._plugin.get_cld_algoindex_from_bandname(c)
            il.append(p_cldimages[band])

        param_concat = {
                        "il": il,
                        "out": "tmp.tif:uint8"
                        }
        if not use_filenames:
            concat_app = OtbAppHandler("ConcatenateMaskImages", param_concat,write_output=False)
        else:
            concat_app = OtbAppHandler("ConcatenateImages", param_concat, write_output=False)
        param_bin_concat = {"im": concat_app.getoutput().get("out"),
                            "out": cld_image_filename + ":uint8"+file_utils.get_extended_filename_write_image_file_standard()
                            }
        bin_concat_app = OtbAppHandler("BinaryConcatenate", param_bin_concat, write_output=True)

    def write_private_images(
            self,
            p_L2PrivateImageFilenamesProvider,
            p_ReflectanceQuantificationValue,
            p_CLDDataBandsSelected,
            p_CLDCoreAlgorithmsMapBand,
            p_WriteOnlyCLACLD,
            p_WriteLTC,
            working_dir):
        LOGGER.debug("Write LTC: " + str(p_WriteLTC))
        LOGGER.debug("Write WriteOnlyCLACLD: " + str(p_WriteOnlyCLACLD))
        LOGGER.debug("p_L2PrivateImageFilenamesProvider.GetLTCImageDirFileName(): %s",
                     p_L2PrivateImageFilenamesProvider.get_ltc_image_dir_filename())

        if not p_WriteOnlyCLACLD:
            # ** ** PRIVATE DATA ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **

            # START WRITING PXD Image file DATA
            # Create the writer
            # Initialize the writer filter
            otb_file_utils.otb_copy_image_to_file(
                self._pxd,
                p_L2PrivateImageFilenamesProvider.get_pxd_image_filename() +
                file_utils.get_extended_filename_write_image_file_standard())

            # START WRITING WAM Image file DATA
            # Create the concatenate imagefilter
            # Create the writer UnsignedIntegerVectorImageFileWriterPointer

            # Convert Binary PWA and TWA images
            tmp_pwa_concat = os.path.join(working_dir, "tmp_pwa_concat.tif")
            param_pwa_concat = {"im": self._possiblewatermask,
                                "out": tmp_pwa_concat
                                }
            pwa_concat_app = OtbAppHandler("BinaryConcatenate", param_pwa_concat, write_output=False)

            tmp_twa_concat = os.path.join(working_dir, "tmp_twa_concat.tif")
            param_twa_concat = {"im": self._testedwatermask,
                                "out": tmp_twa_concat
                                }
            twa_concat_app = OtbAppHandler("BinaryConcatenate", param_twa_concat, write_output=False)

            # Concatenate into one file
            param_wam_concat = {
                "il": [
                    self._wasimage,
                    pwa_concat_app.getoutput().get("out"),
                    twa_concat_app.getoutput().get("out")],
                "out": p_L2PrivateImageFilenamesProvider.get_wam_image_filename() +
                ":uint16"+  file_utils.get_extended_filename_write_image_file_standard()}

            OtbAppHandler("ConcatenateImages", param_wam_concat)

            # START WRITING STO Image file DATA
            # Scalar filter
            multiply_by_scalar(self._sto,p_ReflectanceQuantificationValue,
                               p_L2PrivateImageFilenamesProvider.get_sto_image_filename() +
                               file_utils.get_extended_filename_write_image_file_standard() + ":int16")


            # START WRITING NDT Image file DATA
            # Create the writer
            # Evol 4 - 1: NDT ecris sur 8 bits(et non 16  bits car seuilement un masque)
            otb_file_utils.otb_copy_image_to_file(
                self._ndt,
                p_L2PrivateImageFilenamesProvider.get_ndt_image_filename() +
                file_utils.get_extended_filename_write_image_file_standard())

            # START WRITING LTC Image file DATA
            # Create the image list
            if p_WriteLTC:
                otb_file_utils.otb_copy_image_to_file(self._ltc_image,
                                             p_L2PrivateImageFilenamesProvider.get_ltc_image_filename())

            # START WRITING RTA Image file DATA
            multiply_by_scalar(self._rta, p_ReflectanceQuantificationValue,
                               p_L2PrivateImageFilenamesProvider.get_rta_image_filename() +
                               file_utils.get_extended_filename_write_image_file_standard()+":int16")

            # START WRITING RTC Image file DATA
            multiply_by_scalar(self._rtc, p_ReflectanceQuantificationValue,
                               p_L2PrivateImageFilenamesProvider.get_rtc_image_filename() +
                               file_utils.get_extended_filename_write_image_file_standard()+":int16")

            # START WRITING RCR Image file DATA
            multiply_by_scalar(self._rcr, p_ReflectanceQuantificationValue,
                               p_L2PrivateImageFilenamesProvider.get_rcr_image_filename() +
                               file_utils.get_extended_filename_write_image_file_standard()+":int16")


        # End od condition
        self.write_cld_image(
            self._cld,
            p_CLDDataBandsSelected,
            p_L2PrivateImageFilenamesProvider.get_cld_image_filename(), use_filenames=True)

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # START WRITING CLA Sub Image file DATA
        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # LAIG - DM - MAC - 1691 - CNES: Write CLA only is available(only for VENUS - stereo)
        if self._cld_l2cla is not None:

            otb_file_utils.otb_copy_image_to_file(
                self._cld_l2cla,
                p_L2PrivateImageFilenamesProvider.get_cla_image_filename() +
                file_utils.get_extended_filename_write_image_file_standard())

    @staticmethod
    def write_quicklook_image_from_files(
            filename,
            p_WritePublicProduct,
            p_QuicklookRedBandImage,
            p_QuicklookGreenBandImage,
            p_QuicklookBlueBandImage,
            p_QuicklookMinReflRedBand,
            p_QuicklookMaxReflRedBand,
            p_QuicklookMinReflGreenBand,
            p_QuicklookMaxReflGreenBand,
            p_QuicklookMinReflBlueBand,
            p_QuicklookMaxReflBlueBand,
            p_RealL2NoData,
            dtm,
            working_dir):

        # ====================================================================
        # IF PUBLIC PART OF L2 PRODUCT IS WRITTEN
        # ====================================================================
        if p_WritePublicProduct:
            LOGGER.debug("Start QLK wiring...")

            # Replace NoData value by min
            # TODO
            # Generate he quicklook
            tmp_qlk_red_sub = os.path.join(working_dir, "tmp_qck_red_sub.tif")
            tmp_qlk_green_sub = os.path.join(working_dir, "tmp_qck_green_sub.tif")
            tmp_qlk_blue_sub = os.path.join(working_dir, "tmp_qck_blue_sub.tif")

            # Call the resampling app
            resampling_app_red = resample(p_QuicklookRedBandImage, dtm, tmp_qlk_red_sub,
                                          OtbResampleType.LINEAR_WITH_RADIUS,
                                          padradius=4, outarea=[1000,1000], write_output=False)
            resampling_app_green = resample(p_QuicklookGreenBandImage, dtm, tmp_qlk_green_sub,
                                            OtbResampleType.LINEAR_WITH_RADIUS,
                                            padradius=4, outarea=[1000,1000], write_output=False)
            resampling_app_blue = resample(p_QuicklookBlueBandImage, dtm, tmp_qlk_blue_sub,
                                           OtbResampleType.LINEAR_WITH_RADIUS,
                                           padradius=4, outarea=[1000,1000], write_output=False)

            # Concatenate images
            tmp_qlk = os.path.join(working_dir, "tmp_qck_sub.tif")
            param_concat = {
                "il": [
                    resampling_app_red.getoutput().get("out"),
                    resampling_app_blue.getoutput().get("out"),
                    resampling_app_green.getoutput().get("out")],
                "out": tmp_qlk,
                "ram": str(OtbAppHandler.ram_to_use / 3)
                }
            concat_app = OtbAppHandler("ConcatenateDoubleImages", param_concat,write_output=True)

            # Rescale between 0 and 255
            rescale_intensity(
                concat_app.getoutput().get("out"),
                0,
                255,
                filename +
                file_utils.get_extended_filename_write_image_file_standard()+":uint8",
                inmin=[str(p_QuicklookMinReflRedBand), str(p_QuicklookMinReflGreenBand), str(p_QuicklookMinReflBlueBand)],
                inmax=[str(p_QuicklookMaxReflRedBand), str(p_QuicklookMaxReflGreenBand), str(p_QuicklookMaxReflBlueBand)]
            )
            if (os.path.exists(filename + '.aux.xml')):
                LOGGER.debug("Removing " + filename + '.aux.xml file')
                os.remove(filename + '.aux.xml')

    @staticmethod
    def write_quicklook_image(
            filename,
            pInputImage,
            p_WritePublicProduct,
            p_QuicklookRedBandId,
            p_QuicklookGreenBandId,
            p_QuicklookBlueBandId,
            p_QuicklookMinReflRedBand,
            p_QuicklookMaxReflRedBand,
            p_QuicklookMinReflGreenBand,
            p_QuicklookMaxReflGreenBand,
            p_QuicklookMinReflBlueBand,
            p_QuicklookMaxReflBlueBand,
            p_RealL2NoData,
            dtm,
            working_dir):

        # ====================================================================
        # IF PUBLIC PART OF L2 PRODUCT IS WRITTEN
        # ====================================================================
        if p_WritePublicProduct:
            LOGGER.debug("Start QLK wiring...")
            # Extract min and max reflectance value



            # RealVectorPixelType inputMin;
            #    RealVectorPixelType inputMax;
            #    inputMin.SetSize(3);
            #    inputMax.SetSize(3);
            #    inputMin[0] = static_cast<InternalRealVectorPixelType>(p_QuicklookMinReflRedBand);
            #    #DMGetDouble("QuicklookMinReflRedBand"));
            #    inputMin[1] = static_cast<InternalRealVectorPixelType>(p_QuicklookMinReflGreenBand);
            #    #DMGetDouble("QuicklookMinReflGreenBand"));
            #    inputMin[2] = static_cast<InternalRealVectorPixelType>(p_QuicklookMinReflBlueBand);
            #    #DMGetDouble("QuicklookMinReflBlueBand"));
            #    inputMax[0] = static_cast<InternalRealVectorPixelType>(p_QuicklookMaxReflRedBand);
            #    #DMGetDouble("QuicklookMaxReflRedBand"));
            #    inputMax[1] = static_cast<InternalRealVectorPixelType>(p_QuicklookMaxReflGreenBand);
            #    #DMGetDouble("QuicklookMaxReflGreenBand"));
            # inputMax[2] =
            # static_cast<InternalRealVectorPixelType>(p_QuicklookMaxReflBlueBand);
            #DMGetDouble("QuicklookMaxReflBlueBand"));

            # Select the RGB Band for the QuickLook
            tmp_qlk_red = os.path.join(working_dir, "tmp_qck_red.tif")
            tmp_qlk_green = os.path.join(working_dir, "tmp_qck_green.tif")
            tmp_qlk_blue = os.path.join(working_dir, "tmp_qck_blue.tif")
            tmp_qlk_red_app = extract_roi(pInputImage, [p_QuicklookRedBandId], tmp_qlk_red,write_output=False)
            tmp_qlk_green_app = extract_roi(pInputImage, [p_QuicklookGreenBandId], tmp_qlk_green,write_output=False)
            tmp_qlk_blue_app = extract_roi(pInputImage, [p_QuicklookBlueBandId], tmp_qlk_blue,write_output=False)

            L2ImageWriterBase.write_quicklook_image_from_files(filename,
                                                               p_WritePublicProduct,
                                                               tmp_qlk_red_app.getoutput().get("out"),
                                                               tmp_qlk_green_app.getoutput().get("out"),
                                                               tmp_qlk_blue_app.getoutput().get("out"),
                                                               p_QuicklookMinReflRedBand,
                                                               p_QuicklookMaxReflRedBand,
                                                               p_QuicklookMinReflGreenBand,
                                                               p_QuicklookMaxReflGreenBand,
                                                               p_QuicklookMinReflBlueBand,
                                                               p_QuicklookMaxReflBlueBand,
                                                               p_RealL2NoData,
                                                               dtm,
                                                               working_dir)
