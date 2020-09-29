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
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.interfaces.maja_xml_app_lutmap import *
import orchestrator.common.interfaces.maja_xml_app_lut as maja_xml_app_lut
import os
import io
LOGGER = configure_logger(__name__)


class MajaReduceLutComputation(MajaModule):
    """
    classdocs
    """

    NAME = "ReduceLut"

    def __init__(self):
        """
        Constructor
        """
        super(MajaReduceLutComputation, self).__init__()
        self.in_keys_to_check = ["Params.Caching", "AppHandler", "Plugin", "L1Reader","L2COMM",
                                 "L1Info","L2TOCR", ]
        self.out_keys_to_check = []
        self.out_keys_provided = ["cr_lut"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Lut computation start")
        l_writeL2 = dict_of_input.get("Params").get("WriteL2ProductToL2Resolution")
        viewing_zenith = dict_of_input.get("L1Info").ListOfViewingZenithAnglesPerBandAtL2CoarseResolution
        viewing_azimuth = dict_of_input.get("L1Info").ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution
        # CR Lut generation
        rlc_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("ReduceLutProc_",
                                            do_always_remove=True)
        cr_lut_file = os.path.join(rlc_working, "CR_Lut.mha")
        param_cr_lut = {"lut": dict_of_input.get("L2TOCR"),
                        "solar.zenith": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                        "solar.azimuth": float(dict_of_input.get("L1Info").SolarAngle["sun_azimuth_angle"]),
                        "viewing.zenith": viewing_zenith,
                        "viewing.azimuth": viewing_azimuth,
                        "reducelut": cr_lut_file
                        }
        if dict_of_input.get("Plugin").DirectionalCorrection and dict_of_input.get("L2COMM").get_value(
                "DirCorrOption"):
            param_cr_lut["coeffsr"] = xml_tools.as_string_list(dict_of_input.get("L2COMM").get_value("CoefsR"))
            param_cr_lut["coeffsv"] = xml_tools.as_string_list(dict_of_input.get("L2COMM").get_value("CoefsV"))
            param_cr_lut["refzenith"] = float(dict_of_input.get("L2COMM").get_value("ZenithRef"))
            param_cr_lut["refazimuth"] = float(dict_of_input.get("L2COMM").get_value("AzimuthRef"))
            param_cr_lut["dircorr"] = True

        OtbAppHandler("ReduceLut", param_cr_lut)
        dict_of_output["cr_lut"] = cr_lut_file
        # Read the input lut header to get indexes
        old_lut = maja_xml_app_lut.parse(dict_of_input.get("L2TOCR"),True)
        # HR lut if wide field
        l_lutmap = LUTMap()
        # Add cr lut to map
        l_listoffile = List_Of_FilesType()
        l_listoffile.add_Relative_File_Path(os.path.basename(cr_lut_file))
        l_lut = LUT(index=0, Indexes=old_lut.get_Indexes(), List_Of_Files=l_listoffile)
        l_lutmap.add_LUT(l_lut)
        if dict_of_input.get("Plugin").WideFieldSensor and l_writeL2:
            m_ViewingZenithAnglesMap = dict_of_input.get("L1Reader").get_value("ViewingZenithMeanMap")
            m_ViewingAzimuthAnglesMap = dict_of_input.get("L1Reader").get_value("ViewingAzimuthMeanMap")
            for key in list(m_ViewingZenithAnglesMap.keys()):
                if key not in m_ViewingAzimuthAnglesMap:
                    raise MajaDataException("Internal ERROR: The Viewing Zenith angles Map per zone are incorrect !!")
                l_listoffile = List_Of_FilesType()
                l_listoffile.add_Relative_File_Path("HR_Lut_" + key + ".mha")
                l_lut = LUT(index=key, Indexes=old_lut.get_Indexes(), List_Of_Files=l_listoffile)
                l_ViewingZenithAngle = m_ViewingZenithAnglesMap.get(key)
                l_ViewingAzimuthAngle = m_ViewingAzimuthAnglesMap.get(key)
                if len(l_ViewingZenithAngle) != len(dict_of_input.get("Plugin").BandsDefinitions.L2CoarseBandMap):
                    raise MajaDataException("Error on input L1 product: missing Viewing angles for some l2 coarse band on detector "+key+" , all l2 coarse bands are needed for HR_Lut generation")

                hr_lut_file = os.path.join(rlc_working, "HR_Lut_" + key + ".mha")
                param_hr_lut = {"lut": dict_of_input.get("L2TOCR"),
                                "solar.zenith": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                                "solar.azimuth": float(dict_of_input.get("L1Info").SolarAngle["sun_azimuth_angle"]),
                                "viewing.zenith": l_ViewingZenithAngle,
                                "viewing.azimuth": l_ViewingAzimuthAngle,
                                "reducelut": hr_lut_file
                                }
                OtbAppHandler("ReduceLut", param_hr_lut)
                dict_of_output["hr_lut_" + key] = hr_lut_file
                l_lutmap.add_LUT(l_lut)
        # Write down the lut map
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        l_lutmap.export(output, 0, name_='LUTMap', namespacedef_='', pretty_print=True)
        l_lutmap_filename = os.path.join(rlc_working, "HR_LutMap.xml")
        with open(l_lutmap_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
            LOGGER.info("Writed new gipp lutmap to " + l_lutmap_filename)
        output.close()
        dict_of_output["hr_lutmap"] = l_lutmap_filename

