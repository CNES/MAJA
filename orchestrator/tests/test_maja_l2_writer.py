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

orchestrator.tests.plugins.test_maja_sentinel2_l1_pimage_file_reader -- shortdesc

orchestrator.tests.plugins.test_maja_sentinel2_l1_pimage_file_reader is a description

It defines classes_and_methods

###################################################################################################
"""

from __future__ import absolute_import
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.plugins.common.factory.plugin_activate import register_all
from orchestrator.launcher.app_handler import AppHandler
import orchestrator.plugins.common.factory.product_utils as product_utils
import orchestrator.common.constants as constants
from orchestrator.common.dem.dem_base import DEMBase
from orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler import GippL2COMMEarthExplorerXMLFileHandler
import orchestrator.common.gipp_utils as gipp_utils
from orchestrator.common.gipp_utils import *
from orchestrator.plugins.common.factory.maja_l2_image_writer_provider import L2ImageWriterProvider
from orchestrator.plugins.common.factory.maja_l2_header_writer_provider import L2HeaderWriterProvider
from orchestrator.processor.l2_processor_bands_setup import prepare_bands
from orchestrator.common.constants import AOTEstimation
LOGGER = configure_logger(__name__)
from orchestrator.plugins.common.factory.factory_base import FactoryBase
import glob
from orchestrator.tests.plugins import test_maja_l2_writer_setup
from orchestrator.common.constants import CAMSStatus



def test(argv):
    l1_input_dir = argv[1]
    l2_input_dir = argv[2]
    outdir = argv[3]
    l_cloudrate_value = int(argv[5])
    l_snowrate_value = int(argv[6])

    register_all()
    app_handler = AppHandler()
    app_handler._workingDirectory = outdir
    app_handler._inputDirectory = l1_input_dir
    app_handler._nbThreads = int(argv[4])

    global_output_dict = {}

    test_maja_l2_writer_setup.set_input_l2_writer(l2_input_dir, global_output_dict)


    # Set L1 product and extrat plugin name
    l1product = product_utils.get_input_l1_image_product(l1_input_dir)
    creators = FactoryBase.get_registered("L1ImageReaderBase")
    for crea in creators:
        LOGGER.info("Trying : " + crea.__name__)
        if crea().can_read(l1product.PluginName):
            l1reader = crea()
            l_CurrentPluginBase = l1reader.get_plugin()

    # Load L2COMM
    gipp_l2comm_filename = gipp_utils.get_gipp_filename_with_mission(l1_input_dir, "GIP_L2COMM",
                                                                     l1product.Satellite)
    LOGGER.debug("l_GIPPL2COMMHandler filename : " + gipp_l2comm_filename)

    l_GIPPL2COMMHandler = GippL2COMMEarthExplorerXMLFileHandler(gipp_l2comm_filename)

    # Load DTM
    dem_filename = gipp_utils.get_gipp_filename_with_mission(l1_input_dir, "AUX_REFDE2",
                                                             l1product.UniqueSatellite)
    dem = DEMBase()
    dem.initialize(dem_filename, app_handler._workingDirectory, False)

    # Extract product informations
    l_L2NoData = float(l_GIPPL2COMMHandler.get_value("NoData"))
    l_ReflectanceQuantificationValue = float(l1product.ReflectanceQuantification)
    l_RealL2NoData = l_L2NoData * l_ReflectanceQuantificationValue
    l_CurrentPluginBase.initialize(app_handler)
    l_BandDefinitons = l_CurrentPluginBase.BandsDefinitions
    l_AOTMethodString = l_GIPPL2COMMHandler.get_value("AOTMethod")
    l_AOTMethod = AOTEstimation.get_aot_method(l_AOTMethodString)
    l_CurrentConfigAdminCamera = l_CurrentPluginBase.ConfigAdminCamera

    param_dic = {}
    prepare_bands(l_BandDefinitons, l_BandDefinitons, l_GIPPL2COMMHandler, l_AOTMethod,
                                   l_CurrentConfigAdminCamera, True, False,
                                   param_dic)
    l_EnvCorOption = l_GIPPL2COMMHandler.get_value("EnvCorrOption")

    # Set L2 image Writer
    l2_image_writer = L2ImageWriterProvider.create(l1product.PluginName)
    l2_image_writer.set_dtm(dem)
    l2_image_writer.set_current_plugin_base(l_CurrentPluginBase)
    l2_image_writer.set_copy_private_from_l2_input_to_l2_output(False)
    l2_image_writer.set_write_l2_products(True)
    l2_image_writer.set_output_directory(app_handler._workingDirectory)
    l2_image_writer.set_l2_image_file_reader(None)
    l2_image_writer.set_l1_image_informations_provider(l1product)
    l2_image_writer.set_real_l2_nodata(l_RealL2NoData)

    l2_image_writer.set_reflectance_quantification_value(l1product.ReflectanceQuantification)
    l2_image_writer.set_aot_quantification_value(l_GIPPL2COMMHandler.get_value_f("AOTQuantificationValue"))
    l2_image_writer.set_vap_quantification_value(l_GIPPL2COMMHandler.get_value_f("VAPQuantificationValue"))
    l2_image_writer.set_vap_nodata_value(l_GIPPL2COMMHandler.get_value_f("VAPNodataValue"))
    l2_image_writer.set_aot_nodata_value(l_GIPPL2COMMHandler.get_value_f("AOTNodataValue"))
    l2_image_writer.set_quicklook_min_refl_redband(l_GIPPL2COMMHandler.get_value("QuicklookMinReflRedBand"))
    l2_image_writer.set_quicklook_min_refl_greenband(
        l_GIPPL2COMMHandler.get_value("QuicklookMinReflGreenBand"))
    l2_image_writer.set_quicklook_min_refl_blueband(l_GIPPL2COMMHandler.get_value("QuicklookMinReflBlueBand"))
    l2_image_writer.set_quicklook_max_refl_redband(l_GIPPL2COMMHandler.get_value("QuicklookMaxReflRedBand"))
    l2_image_writer.set_quicklook_max_refl_greenband(
        l_GIPPL2COMMHandler.get_value("QuicklookMaxReflGreenBand"))
    l2_image_writer.set_quicklook_max_refl_blueband(l_GIPPL2COMMHandler.get_value("QuicklookMaxReflBlueBand"))
    l2_image_writer.set_quicklook_red_band_code(l_GIPPL2COMMHandler.get_value("QuicklookRedBandCode"))
    l2_image_writer.set_quicklook_green_band_code(l_GIPPL2COMMHandler.get_value("QuicklookGreenBandCode"))
    l2_image_writer.set_quicklook_blue_band_code(l_GIPPL2COMMHandler.get_value("QuicklookBlueBandCode"))

    l2_image_writer.set_projection_ref(l1product.ProjectionRef)
    l2_image_writer.set_write_public_product(True)
    l2_image_writer.set_env_cor_option(l_EnvCorOption)
    l2_image_writer.set_sre_image_list(global_output_dict["SRE_List"])
    l2_image_writer.set_fre_image_list(global_output_dict["FRE_List"])
    l2_image_writer.set_aot_image_list(global_output_dict["L2AOT_List"])
    l2_image_writer.set_tao_image_list(global_output_dict["L2TAO_List"])
    l2_image_writer.set_vap_image_list(global_output_dict["L2VAP_List"])
    l2_image_writer.set_iwc_image_list(global_output_dict["L2IWC_List"])
    l2_image_writer.set_hid_image(global_output_dict["dtm_hid"])
    l2_image_writer.set_shd_image(global_output_dict["dtm_shd"])
    l2_image_writer.set_sat_image_list(global_output_dict["L2SAT_List"])
    l2_image_writer.set_pix_image_list(global_output_dict["L2PIX_List"])
    l2_image_writer.set_edg_image_list(global_output_dict["L2EDG_List"])
    l2_image_writer.set_stl_image_list(global_output_dict["L2STL_List"])
    l2_image_writer.set_tgs_image_list(global_output_dict["L2TGS_List"])

    if global_output_dict["L2DFP_List"]:
        l2_image_writer.set_dfp_image_list(global_output_dict["L2DFP_List"])

    if global_output_dict["snow"]:
        l2_image_writer.set_snw_image(global_output_dict["snow"])



    if global_output_dict["cla"]:
        l2_image_writer.set_cla_image(global_output_dict["cla"])


    l2_image_writer.set_rta_image(global_output_dict["rta_composite"])
    l2_image_writer.set_rtc_image(global_output_dict["rtc_composite"])
    l2_image_writer.set_rcr_image(global_output_dict["rcr_composite"])
    l2_image_writer.set_sto_image(global_output_dict["sto_composite"])

    l2_image_writer.set_pxd_image(global_output_dict["pxd_composite"])
    l2_image_writer.set_ndt_image(global_output_dict["ndt_composite"])

    l2_image_writer.set_cld_image(global_output_dict["CLD_List"])
    l2_image_writer.set_l2_cld_image_list(global_output_dict["L2CLD_List"])

    l2_image_writer.set_was_image(global_output_dict["was"])
    l2_image_writer.set_pwa_image(global_output_dict["pwa"])
    l2_image_writer.set_twa_image(global_output_dict["twa"])



    if global_output_dict["ltc"]:
        l_write_ltc = True
        l2_image_writer.set_write_ltc(l_write_ltc)
        LOGGER.debug(global_output_dict["ltc"])
        l2_image_writer.set_ltc_image(global_output_dict["ltc"])
    else:
        l_write_ltc = False
        l2_image_writer.set_write_ltc(l_write_ltc)


    l2_image_writer.initialize_product()
    l2_image_writer.write(app_handler._workingDirectory)

    # Set L2 Header
    l_L2HeaderFileWriter = L2HeaderWriterProvider.create(l1product.PluginName,
                                                         app_handler)
    l_L2HeaderFileWriter.l1imageinformationsproviderbase = l1product
    l_L2HeaderFileWriter.outputfileclass = l1product.FileClass
    l_L2HeaderFileWriter.outputdirectory = app_handler._workingDirectory
    l_L2HeaderFileWriter.outputl2productfilename = l2_image_writer.get_global_product_filename()
    l_L2HeaderFileWriter.outputl2privateimagefilenamesprovider = l2_image_writer.get_private_filenames_provider()
    l_L2HeaderFileWriter.reflectancequantificationvalue = l1product.ReflectanceQuantification
    l_L2HeaderFileWriter.dem = dem

    l_L2HeaderFileWriter.listofgippsfilenames = get_listoffilenames_filetype_sorted(
        app_handler.get_input_directory(), "GIP_")

    l_L2HeaderFileWriter.creator = 'MAJA_L2_INIT_CHAIN'
    l_L2HeaderFileWriter.system = 'MAJA'
    l_L2HeaderFileWriter.checkxmlfileswithschema = True
    l_L2HeaderFileWriter.notes = 'L2 note'

    l_L2HeaderFileWriter.listofbandtheoreticalwavelengthforreflectancescomposite = param_dic[
        "ListOfBandTheoreticalWavelengthForReflectancesComposite"]
    l_L2HeaderFileWriter.listofbandtheoreticalwavelengthforltccomposite = param_dic[
        "ListOfBandTheoreticalWavelengthForLTCComposite"]
    l_L2HeaderFileWriter.correlbandtheoreticalwavelengthforstocomposite = param_dic[
        "CorrelTheoreticalWavelength_DateD"]

    l_L2HeaderFileWriter.nodatavalue = l_GIPPL2COMMHandler.get_value("NoData")
    l_L2HeaderFileWriter.vapnodatavalue = l_GIPPL2COMMHandler.get_value("VAPNodataValue")
    l_L2HeaderFileWriter.vapquantificationvalue = l_GIPPL2COMMHandler.get_value("VAPQuantificationValue")
    l_L2HeaderFileWriter.aotnodatavalue = l_GIPPL2COMMHandler.get_value("AOTNodataValue")
    l_L2HeaderFileWriter.aotquantificationvalue = l_GIPPL2COMMHandler.get_value("AOTQuantificationValue")

    l_L2HeaderFileWriter.quicklookredbandcode = l_GIPPL2COMMHandler.get_value("QuicklookRedBandCode")
    l_L2HeaderFileWriter.quicklookgreenbandcode = l_GIPPL2COMMHandler.get_value("QuicklookGreenBandCode")
    l_L2HeaderFileWriter.quicklookbluebandcode = l_GIPPL2COMMHandler.get_value("QuicklookBlueBandCode")

    # Set the L2 images flags
    l_L2HeaderFileWriter.cirrusflag = False
    l_L2HeaderFileWriter.cloudrate = l_cloudrate_value
    l_L2HeaderFileWriter.snowrate = l_snowrate_value
    l_L2HeaderFileWriter.hotspotflag = False
    l_L2HeaderFileWriter.sunglintflag = False
    l_L2HeaderFileWriter.adjacencyeffectsandslopecorrection = l_EnvCorOption
    l_L2HeaderFileWriter.cirruscorrection = False
    l_L2HeaderFileWriter.initmode = True
    l_L2HeaderFileWriter.backwardmode = False
    l_STOListOfStringDates = []
    l_STOListOfStringDates.insert(0, l1product.ProductDateStr)

    l_L2HeaderFileWriter.productisvalid = True
    l_L2HeaderFileWriter.writel2products = True
    l_L2HeaderFileWriter.writepublicproduct = True
    l_L2HeaderFileWriter.numberofstackimages = len(l_STOListOfStringDates)
    l_L2HeaderFileWriter.stolistofstringdates = l_STOListOfStringDates
    l_L2HeaderFileWriter.enablewriteearthexplorerheaders = True

    l_L2HeaderFileWriter.writeltc = l_write_ltc
    l_L2HeaderFileWriter.ignorecurrentltc = False

    l_L2HeaderFileWriter.camsstatus = CAMSStatus.ACTIVATED_NOAVAILABLEDATA

    LOGGER.info("Writing L2 EarthExplorer headers files into the output L2 product")
    l_L2HeaderFileWriter.write()

    # Set the L2 header filename and the PMC report filename
    m_HDRFileNameOfProductImage = l2_image_writer.get_global_product_filename()
    m_ProductionReportFilename = l2_image_writer.get_production_report_filename()

    LOGGER.debug(
        "HDRFileNameOfProductImage: <" + m_HDRFileNameOfProductImage + ">")
    LOGGER.debug(
        "ProductionReportFilename:  <" + m_ProductionReportFilename + ">")

    LOGGER.debug("THREADS = ")

    LOGGER.debug(app_handler._nbThreads)

