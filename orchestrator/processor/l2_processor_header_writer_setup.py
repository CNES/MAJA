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

orchestrator.processor.base_processor -- shortdesc

orchestrator.processor.base_processor is the base of all processors

It defines method mandatory for a processor


###################################################################################################
"""

from orchestrator.common.gipp_utils import *
from orchestrator.plugins.common.factory.maja_l2_image_reader_provider import L2ImageReaderProvider
from orchestrator.plugins.common.factory.maja_l1_image_info_provider import L1ImageInformationsProvider
import os
LOGGER = configure_logger(__name__)


def setup_l2_header_writer(l_L2HeaderFileWriter, l_CurrentPluginBase, p_OutputL2ImageFileWriter, in_global_dict, out_global_dict,
                           p_ProductIsValid, p_WriteL2Products, p_CamsStatus, l_IgnoreCurrentLTC):
    param_dic = in_global_dict["Params"]
    l2comm = in_global_dict["L2COMM"]
    # Set the L2 image filenames provider
    # --------------------------------------------------
    l_L2HeaderFileWriter.l1imageinformationsproviderbase = in_global_dict["L1Info"]
    # Set the L2 input image filenames provider
    # --------------------------------------------------
    if in_global_dict["L2Reader"] is not None:
        l_L2HeaderFileWriter.inputl2imagefilereader = in_global_dict["L2Reader"]
    # Set the File_Class
    # --------------------------------------------------
    l_L2HeaderFileWriter.outputfileclass = in_global_dict["L1Info"].FileClass
    l_L2HeaderFileWriter.outputdirectory = param_dic["L2OutputDirectory"]
    l_L2HeaderFileWriter.outputl2productfilename = p_OutputL2ImageFileWriter.get_global_product_filename()
    l_L2HeaderFileWriter.outputl2privateimagefilenamesprovider = p_OutputL2ImageFileWriter.get_private_filenames_provider()
    l_L2HeaderFileWriter.camsstatus = p_CamsStatus
    l_L2HeaderFileWriter.l1_plugin = l_CurrentPluginBase
    # Set the L2 images size for each resolution
    # --------------------------------------------------
    l_L2HeaderFileWriter.reflectancequantificationvalue = in_global_dict["L1Info"].ReflectanceQuantification
    # Set the dem
    l_L2HeaderFileWriter.dem = in_global_dict["DEM"]
    # Set the list of Gipps filenames
    # --------------------------------------------------
    l_L2HeaderFileWriter.listofgippsfilenames = get_listoffilenames_filetype_sorted(
        in_global_dict["AppHandler"].get_input_directory(), "GIP_")
    # Set the list of Gipps filenames
    # --------------------------------------------------
    l_L2HeaderFileWriter.creator = in_global_dict["AppHandler"]._processorName
    l_L2HeaderFileWriter.system = in_global_dict["AppHandler"].get_admin_conf().get_System()
    l_L2HeaderFileWriter.checkxmlfileswithschema = in_global_dict["AppHandler"].get_user_conf(
    ).get_Processing().get_CheckXMLFilesWithSchema()
    # Set the Notes
    # --------------------------------------------------
    l_L2HeaderFileWriter.notes = in_global_dict["AppHandler"].get_user_conf().get_Notes().get_L2Note()
    # Set the list of Used Products Ids
    # --------------------------------------------------
    l_listOfFilename = L1ImageInformationsProvider.get_list_of_l1products(
        in_global_dict["AppHandler"].get_input_directory(), tile_id=in_global_dict["AppHandler"].get_tile_id())

    # TODO
    if not param_dic["InitMode"] and not param_dic["BackWardMode"]:
        l_listOfFilename2 = L2ImageReaderProvider.get_list_of_l2_products(
            in_global_dict["AppHandler"].get_input_directory())
        # Add to the l_listOfFilename
        for i in l_listOfFilename2:
            l_listOfFilename.append(i)

    l_L2HeaderFileWriter.listofusedproductsids = l_listOfFilename
    l_L2HeaderFileWriter.listofbandtheoreticalwavelengthforreflectancescomposite = param_dic[
        "ListOfBandTheoreticalWavelengthForReflectancesComposite"]
    l_L2HeaderFileWriter.listofbandtheoreticalwavelengthforltccomposite = param_dic[
        "ListOfBandTheoreticalWavelengthForLTCComposite"]
    l_L2HeaderFileWriter.correlbandtheoreticalwavelengthforstocomposite = param_dic["CorrelTheoreticalWavelength_DateD"]
    l_L2HeaderFileWriter.nodatavalue = l2comm.get_value("NoData")
    l_L2HeaderFileWriter.vapnodatavalue = l2comm.get_value("VAPNodataValue")
    l_L2HeaderFileWriter.vapquantificationvalue = l2comm.get_value("VAPQuantificationValue")
    l_L2HeaderFileWriter.aotnodatavalue = l2comm.get_value("AOTNodataValue")
    l_L2HeaderFileWriter.aotquantificationvalue = l2comm.get_value("AOTQuantificationValue")
    l_L2HeaderFileWriter.quicklookredbandcode = l2comm.get_value("QuicklookRedBandCode")
    l_L2HeaderFileWriter.quicklookgreenbandcode = l2comm.get_value("QuicklookGreenBandCode")
    l_L2HeaderFileWriter.quicklookbluebandcode = l2comm.get_value("QuicklookBlueBandCode")
    # Set the L2 images flags
    # --------------------------------------------------

    if out_global_dict.get("CirrusFlag") is not None:
        l_L2HeaderFileWriter.cirrusflag = out_global_dict["CirrusFlag"]

    if out_global_dict["CloudRate"] is not None:
        l_L2HeaderFileWriter.cloudrate = out_global_dict["CloudRate"]

    if out_global_dict.get("SnowRate") is not None:
        l_L2HeaderFileWriter.snowrate = out_global_dict.get("SnowRate")

    if out_global_dict.get("RainFlag") is not None:
        l_L2HeaderFileWriter.rainflag = out_global_dict.get("RainFlag")
    else:
        l_L2HeaderFileWriter.rainflag = False
    l_L2HeaderFileWriter.hotspotflag = out_global_dict["HotSpotFlag"]
    l_L2HeaderFileWriter.sunglintflag = out_global_dict["SunglintFlag"]
    l_L2HeaderFileWriter.adjacencyeffectsandslopecorrection = param_dic.get("EnvCorOption")
    if out_global_dict.get("CirrusCorrected") is not None:
        l_L2HeaderFileWriter.cirruscorrection = out_global_dict.get("CirrusCorrected")
    else:
        l_L2HeaderFileWriter.cirruscorrection = False
    l_L2HeaderFileWriter.initmode = param_dic.get("InitMode")
    l_L2HeaderFileWriter.backwardmode = param_dic.get("BackWardMode")
    # m_ProductIsValid is declared in the L2Processor class
    l_L2HeaderFileWriter.productisvalid = p_ProductIsValid
    l_L2HeaderFileWriter.writel2products = p_WriteL2Products
    l_L2HeaderFileWriter.writepublicproduct = param_dic["WriteL2ProductToL2Resolution"]
    l_L2HeaderFileWriter.numberofstackimages = len(in_global_dict.get("Params").get("STOListOfStringDates"))
    l_L2HeaderFileWriter.stolistofstringdates = in_global_dict.get("Params").get("STOListOfStringDates")
    l_L2HeaderFileWriter.enablewriteearthexplorerheaders = param_dic["WriteHeaders"]
    l_L2HeaderFileWriter.writeltc = in_global_dict["Params"]["DealingLTC"]
    l_L2HeaderFileWriter.ignorecurrentltc = l_IgnoreCurrentLTC

    # *************************************************************************************************************
    # Write the EarthExplorer XML files
    # *************************************************************************************************************
    LOGGER.info("Writing L2 EarthExplorer headers files into the output L2 product")
    LOGGER.debug("itou: " +
          str((l_L2HeaderFileWriter.outputl2productfilename)) +
          " : " +
          str(os.path.exists(l_L2HeaderFileWriter.outputl2productfilename)) +
          " : " +
          str(os.path.isdir(l_L2HeaderFileWriter.outputl2productfilename)))

    l_L2HeaderFileWriter.write()

    # Set the L2 header filename and the PMC report filename
    m_HDRFileNameOfProductImage = p_OutputL2ImageFileWriter.get_global_product_filename()
    m_ProductionReportFilename = p_OutputL2ImageFileWriter.get_production_report_filename()

    LOGGER.debug(
        "L2Processor::ScientificSingleProductProcessing m_HDRFileNameOfProductImage: <" +
        m_HDRFileNameOfProductImage +
        ">")
    LOGGER.debug(
        "L2Processor::ScientificSingleProductProcessing m_ProductionReportFilename:  <" +
        m_ProductionReportFilename +
        ">")

    # *************************************************************************************************************
    # *********                  E N D     W R I T E S    H E A D E R S    P R O C E S S I N G            *********
    # *************************************************************************************************************
