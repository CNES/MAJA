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

from orchestrator.plugins.common.factory.maja_l1_image_reader_provider import L1ImageReaderProvider
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.plugins.common.factory.plugin_activate import register_all

from orchestrator.launcher.app_handler import AppHandler
import orchestrator.plugins.common.factory.product_utils as product_utils
import orchestrator.common.constants as constants
from orchestrator.cots.otb.otb_file_utils import otb_copy_image_to_file
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
import orchestrator.common.file_utils as file_utils
from orchestrator.common.dem.dem_base import DEMBase
from orchestrator.common.directory_manager import DirectoryManager
from orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler import \
    GippL2COMMEarthExplorerXMLFileHandler
import orchestrator.common.gipp_utils as gipp_utils
import argparse, sys, os
LOGGER = configure_logger(__name__)


argParser = argparse.ArgumentParser()
required_arguments = argParser.add_argument_group('required arguments')
required_arguments.add_argument('-i', '--input_dir', required=True,
                                    help='Path to input folder')
required_arguments.add_argument('-o', '--output_dir', required=True,
                                    help='Path to output directory')
required_arguments.add_argument('-t', '--tile_id', required=False,
                                    help='Tile id of the L1 product')
args = argParser.parse_args(sys.argv[1:])

register_all()
app_handler = AppHandler()
app_handler._workingDirectory = args.output_dir
app_handler._directory_manager = DirectoryManager(working_dir=args.output_dir)
OtbAppHandler.set_ram_to_use(8048)

# Get the thread value
os.environ["ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS"] = str(app_handler.get_nb_threads())

l1product = product_utils.get_input_l1_image_product(args.input_dir, args.tile_id)
LOGGER.debug(l1product.Satellite)

# Read and load L2COM
gip_l2comm_filename = gipp_utils.get_gipp_filename_with_mission(args.input_dir, "GIP_L2COMM", l1product.Satellite)
LOGGER.debug("L2COMM filename : " + gip_l2comm_filename)
l_gippl2commhandler = GippL2COMMEarthExplorerXMLFileHandler(gip_l2comm_filename)

# Read and initialize DEM
dem_filename = gipp_utils.get_gipp_filename_with_mission(args.input_dir, "AUX_REFDE2", l1product.UniqueSatellite)
LOGGER.debug("DEM filename : " + dem_filename)
dem = DEMBase()
dem_working = app_handler.get_directory_manager().get_temporary_directory("DTMRead_",do_always_remove=False)
dem.initialize(dem_filename, dem_working, True)


# Read L1 product
l1_reader = L1ImageReaderProvider.create(l1product.PluginName, l1product, app_handler, l_gippl2commhandler, dem,
                             constants.ReadL1Mode.READ_L1_MODE_FOR_ALGORITHMS)

LOGGER.info(app_handler.get_system_infos())
print(app_handler.get_working_directory())


LOGGER.info("L1 Reader Done ...")
#Copy files to the output directory
if l1_reader.has_value("IPEDGSubOutput") and l1_reader.get_value("IPEDGSubOutput"):
    otb_copy_image_to_file(l1_reader.get_value("IPEDGSubOutput"),os.path.join(args.output_dir, "IPEDGSubOutput.tif"))

if l1_reader.has_value("SOL1Image") and l1_reader.get_value("SOL1Image"):
    otb_copy_image_to_file(l1_reader.get_value("SOL1Image"), os.path.join(args.output_dir,"SOL1Image.tif"))

if l1_reader.has_value("SOL2Image") and l1_reader.get_value("SOL2Image"):
    otb_copy_image_to_file(l1_reader.get_value("SOL2Image"), os.path.join(args.output_dir,"SOL2Image.tif"))

if l1_reader.has_value("DTMVIEImage") and l1_reader.get_value("DTMVIEImage"):
    otb_copy_image_to_file(l1_reader.get_value("DTMVIEImage"), os.path.join(args.output_dir,"DTMVieImage.tif"))

if l1_reader.has_value("IPTOASubOutput") and l1_reader.get_value("IPTOASubOutput"):
    otb_copy_image_to_file(l1_reader.get_value("IPTOASubOutput"), os.path.join(args.output_dir,"IPTOASubOutput.tif"))

if l1_reader.has_value("IPSATSubOutput") and l1_reader.get_value("IPSATSubOutput"):
    otb_copy_image_to_file(l1_reader.get_value("IPSATSubOutput"), os.path.join(args.output_dir,"IPSATSubOutput.tif"))

if l1_reader.has_value("L1TOACirrusImage") and l1_reader.get_value("L1TOACirrusImage"):
    otb_copy_image_to_file(l1_reader.get_value("L1TOACirrusImage"), os.path.join(args.output_dir, "L1TOACirrusImage.tif"))

if l1_reader.has_value("CLAImage") and l1_reader.get_value("CLAImage"):
    otb_copy_image_to_file(l1_reader.get_value("CLAImage"), os.path.join(args.output_dir,"CLAImage.tif"))

if l1_reader.has_value("ShadowVIEImage") and l1_reader.get_value("ShadowVIEImage"):
    otb_copy_image_to_file(l1_reader.get_value("ShadowVIEImage"), os.path.join(args.output_dir,"ShadowVIEImage.tif"))

LOGGER.info("L1 PIX Begin ...")
if l1_reader.has_value("L1PIXImageList") and l1_reader.get_value("L1PIXImageList"):
    for i in range(0,len(l1_reader.get_value("L1PIXImageList"))):
        otb_copy_image_to_file(l1_reader.get_value("L1PIXImageList")[i],
                               os.path.join(args.output_dir,"L1PIXImageList_R"+str(i+1)+".tif"))

LOGGER.info("L1 PIX Done ...")

if l1_reader.has_value("L2TOAImageList") and l1_reader.get_value("L2TOAImageList"):
    for i in range(0,len(l1_reader.get_value("L2TOAImageList"))):
        otb_copy_image_to_file(l1_reader.get_value("L2TOAImageList")[i],
                               os.path.join(args.output_dir,"L2TOAImageList_R"+str(i+1)+".tif"))

if l1_reader.has_value("L2EDGOutputList") and l1_reader.get_value("L2EDGOutputList"):
    for i in range(0,len(l1_reader.get_value("L2EDGOutputList"))):
        otb_copy_image_to_file(l1_reader.get_value("L2EDGOutputList")[i],
                               os.path.join(args.output_dir,"L2EDGOutputList_R"+str(i+1)+".tif"))

if l1_reader.has_value("L2SATImageList") and l1_reader.get_value("L2SATImageList"):
    LOGGER.debug(l1_reader.get_value("L2SATImageList"))
    for i in range(0,len(l1_reader.get_value("L2SATImageList"))):
        if l1_reader.get_value("L2SATImageList")[i]:
            otb_copy_image_to_file(l1_reader.get_value("L2SATImageList")[i],
                                   os.path.join(args.output_dir,"L2SATMaskList_R"+str(i+1)+".tif"))

if l1_reader.has_value("L2PIXImageList") and l1_reader.get_value("L2PIXImageList"):
    for i in range(0,len(l1_reader.get_value("L2PIXImageList"))):
        otb_copy_image_to_file(l1_reader.get_value("L2PIXImageList")[i],
                               os.path.join(args.output_dir,"L2PIXImageList_R"+str(i+1)+".tif"))

if l1_reader.has_value("L2DFPImageList") and l1_reader.get_value("L2DFPImageList"):
    for i in range(0,len(l1_reader.get_value("L2DFPImageList"))):
        otb_copy_image_to_file(l1_reader.get_value("L2DFPImageList")[i],
                               os.path.join(args.output_dir,"L2DFPImageList_R"+str(i+1)+".tif"))

if l1_reader.has_value("L1CLDImageList") and l1_reader.get_value("L1CLDImageList"):
    for i in range(0,len(l1_reader.get_value("L1CLDImageList"))):
        otb_copy_image_to_file(l1_reader.get_value("L1CLDImageList")[i],
                               os.path.join(args.output_dir,"L1CLDImageList_R"+str(i+1)+".tif"))

if l1_reader.has_value("AngleZoneMaskList") and l1_reader.get_value("AngleZoneMaskList"):
    for i in range(0, len(l1_reader.get_value("AngleZoneMaskList"))):
        otb_copy_image_to_file(l1_reader.get_value("AngleZoneMaskList")[i],
                               os.path.join(args.output_dir,"AngleZoneMaskList_R"+str(i+1)+".tif"))

if l1_reader.has_value("L1TOAImageList") and l1_reader.get_value("L1TOAImageList"):
    for i in range(0, len(l1_reader.get_value("L1TOAImageList"))):
        LOGGER.debug("toa image list test: " + str(l1_reader.get_value("L1TOAImageList")[i]) )
        otb_copy_image_to_file(l1_reader.get_value("L1TOAImageList")[i],
                               os.path.join(args.output_dir,"L1TOAImageList_R"+str(i+1)+".tif"))
# Log system infos

#app_handler.get_directory_manager().clean(True)
