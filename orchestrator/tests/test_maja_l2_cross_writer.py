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
from orchestrator.plugins.common.factory.maja_l2_header_writer_provider import L2HeaderWriterProvider
from orchestrator.plugins.common.base.maja_l2_private_image_filenames_provider import L2PrivateImageFilenamesProvider
from orchestrator.processor.l2_processor_bands_setup import prepare_bands
from orchestrator.common.constants import AOTEstimation
LOGGER = configure_logger(__name__)
from orchestrator.plugins.common.factory.factory_base import FactoryBase
import glob, os, sys, shutil
import os.path as op
from orchestrator.tests.plugins import test_maja_l2_writer_setup
from orchestrator.common.constants import CAMSStatus

# Only reading/writing metadata
#   Read L1 product (native or muscate)
#   Write L2 Header file (MUSCATE)

def test(argv):
    l1_input_dir = argv[1]
    outdir = argv[2]
    l_cloudrate_value = 0
    l_snowrate_value = 0

    register_all()
    app_handler = AppHandler()
    app_handler._workingDirectory = outdir
    app_handler._inputDirectory = l1_input_dir
    app_handler._nbThreads = 1

    global_output_dict = {}

    os.makedirs(outdir, exist_ok=True)

    #~ test_maja_l2_writer_setup.set_input_l2_writer(l2_input_dir, global_output_dict)

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

    l_L2BaseFilename = l1product.get_l2_product_id()
    l_PublicDirectory = os.path.join(app_handler._workingDirectory, l_L2BaseFilename)
    l_DATADirectory = os.path.join(l_PublicDirectory, "DATA")
    # Store filename
    l_L2XMLFilename = os.path.join(l_PublicDirectory, l_L2BaseFilename + "_MTD_ALL.xml")

    l_PrivateFilenamesProvider = L2PrivateImageFilenamesProvider()
    l_PrivateFilenamesProvider.initialize_with_root_dir(
        op.join(l_DATADirectory, l_L2BaseFilename + "_PVD_ALL"),
        l1product.ProductDateStr)
    
    # Clean and Create Directories
    if op.exists(l_PublicDirectory):
        shutil.rmtree(l_PublicDirectory)
    os.makedirs(l_DATADirectory)
    os.makedirs(l_PrivateFilenamesProvider.get_private_directory())
    os.makedirs(l_PrivateFilenamesProvider.get_ltc_image_dir_filename())

    # TODO: create fake mha ?

    outputPlugin = l1product.PluginName
    if not outputPlugin.endswith("_MUSCATE"):
        outputPlugin += "_MUSCATE"
    # Set L2 Header
    l_L2HeaderFileWriter = L2HeaderWriterProvider.create(outputPlugin,
                                                         app_handler)
    l_L2HeaderFileWriter.l1imageinformationsproviderbase = l1product
    l_L2HeaderFileWriter.outputfileclass = l1product.FileClass
    l_L2HeaderFileWriter.outputdirectory = app_handler._workingDirectory
    l_L2HeaderFileWriter.outputl2productfilename = l_L2XMLFilename
    l_L2HeaderFileWriter.outputl2privateimagefilenamesprovider = l_PrivateFilenamesProvider
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

    l_L2HeaderFileWriter.writeltc = True
    l_L2HeaderFileWriter.ignorecurrentltc = False

    l_L2HeaderFileWriter.camsstatus = CAMSStatus.ACTIVATED_NOAVAILABLEDATA

    LOGGER.info("Writing L2 EarthExplorer headers files into the output L2 product")
    l_L2HeaderFileWriter.write()


if __name__ == "__main__":
    test(sys.argv)
