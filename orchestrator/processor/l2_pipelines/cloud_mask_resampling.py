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
from orchestrator.cots.otb.algorithms.otb_resample import resample
from orchestrator.common.maja_utils import is_croco_on
from orchestrator.cots.otb.algorithms.otb_resample import OtbResampleType
from orchestrator.common.maja_exceptions import *
from orchestrator.common.constants import *
from orchestrator.modules.maja_module import MajaModule
import os
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
LOGGER = configure_logger(__name__)


class MajaMaskResampling(MajaModule):
    """
    classdocs
    """
    NAME = "CloudMaskResampling"

    def __init__(self):
        """
        Constructor
        """
        super(MajaMaskResampling, self).__init__()
        self.in_keys_to_check = ["AppHandler", "Plugin", "DEM"]
        self.out_keys_to_check = []
        self.out_keys_provided = ["L2CLDList"]
        self._apps =  OtbPipelineManager()

    def cleanup(self):
        LOGGER.debug("Cleanup of module : "+MajaMaskResampling.NAME)
        self._apps.free_otb_app()

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Cloud mask resampling start")
        p_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("CloudMaskResampling_",
                                            do_always_remove=True)
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        do_write = is_croco_on("cloudmaskresampling")
        l2_cld_list = []
        for p_res in range(0, len(bands_definition.ListOfL2Resolution)):
            l_res = bands_definition.ListOfL2Resolution[p_res]
            # ---------- Resample All to resolution ---------------
            cld_all_resampled = os.path.join(p_working, "cloud_all_" + l_res + ".tif")
            cld_all_app = resample(dict_of_output[CLOUD_MASK_ALL], dict_of_input.get("DEM").ALTList[p_res],
                     cld_all_resampled, OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_all_app)
            dict_of_output[CLOUD_MASK_ALL + "_" + l_res] = cld_all_app.getoutput().get("out")
            # ---------- Resample All cloud to resolution ---------------
            cld_allclouds_resampled = os.path.join(p_working, "cloud_allclouds_" + l_res + ".tif")
            cld_allclouds_app = resample(dict_of_output[CLOUD_MASK_ALL_CLOUDS], dict_of_input.get("DEM").ALTList[p_res],
                     cld_allclouds_resampled, OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_allclouds_app)
            dict_of_output[CLOUD_MASK_ALL_CLOUDS + "_" + l_res] = cld_allclouds_app.getoutput().get("out")
            # ---------- Resample shadow to resolution ---------------
            cld_shadows_resampled = os.path.join(p_working, "cloud_shadows_" + l_res + ".tif")
            cld_shadows_app = resample(dict_of_output[CLOUD_MASK_SHADOWS], dict_of_input.get("DEM").ALTList[p_res],
                     cld_shadows_resampled, OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_shadows_app)
            dict_of_output[CLOUD_MASK_SHADOWS + "_" + l_res] = cld_shadows_app.getoutput().get("out")
            # ---------- Resample shadvar to resolution ---------------
            cld_shadvar_resampled = os.path.join(p_working, "cloud_shadvar_" + l_res + ".tif")
            cld_shadvar_app = resample(dict_of_output[CLOUD_MASK_SHADVAR], dict_of_input.get("DEM").ALTList[p_res],
                     cld_shadvar_resampled, OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_shadvar_app)
            dict_of_output[CLOUD_MASK_SHADVAR + "_" + l_res] = cld_shadvar_app.getoutput().get("out")
            # ---------- Resample Refl cloud to resolution ---------------
            cld_refl_resampled = os.path.join(p_working, "cloud_refl_" + l_res + ".tif")
            cld_refl_app = resample(dict_of_output[CLOUD_MASK_REFL], dict_of_input.get("DEM").ALTList[p_res], cld_refl_resampled,
                     OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_refl_app)
            dict_of_output[CLOUD_MASK_REFL + "_" + l_res] = cld_refl_app.getoutput().get("out")
            # ---------- Resample ReflVar cloud to resolution ---------------
            cld_reflvar_resampled = os.path.join(p_working, "cloud_reflvar_" + l_res + ".tif")
            cld_reflvar_app = resample(dict_of_output[CLOUD_MASK_REFL_VAR], dict_of_input.get("DEM").ALTList[p_res],
                     cld_reflvar_resampled,
                     OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_reflvar_app)
            dict_of_output[CLOUD_MASK_REFL_VAR + "_" + l_res] = cld_reflvar_app.getoutput().get("out")
            # ---------- Resample Extension cloud to resolution ---------------
            cld_ext_resampled = os.path.join(p_working, "cloud_ext_" + l_res + ".tif")
            cld_ext_app = resample(dict_of_output[CLOUD_MASK_EXTENSION], dict_of_input.get("DEM").ALTList[p_res], cld_ext_resampled,
                     OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_ext_app)
            dict_of_output[CLOUD_MASK_EXTENSION + "_" + l_res] = cld_ext_app.getoutput().get("out")
            # ---------- Resample Alt to resolution ---------------
            cld_alt_resampled = os.path.join(p_working, "cloud_alt_" + l_res + ".tif")
            cld_alt_app = resample(dict_of_output[CLOUD_MASK_ALT], dict_of_input.get("DEM").ALTList[p_res], cld_alt_resampled,
                     OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_alt_app)
            dict_of_output[CLOUD_MASK_ALT + "_" + l_res] = cld_alt_app.getoutput().get("out")
            # ---------- Resample Cirrus cloud to resolution ---------------
            cld_cirrus_resampled = os.path.join(p_working, "cloud_cirrus_" + l_res + ".tif")
            cld_cirrus_app = resample(dict_of_output[CLOUD_MASK_CIRRUS], dict_of_input.get("DEM").ALTList[p_res], cld_cirrus_resampled,
                     OtbResampleType.LINEAR,threshold=0.25,write_output=do_write)
            self._apps.add_otb_app(cld_cirrus_app)
            dict_of_output[CLOUD_MASK_CIRRUS + "_" + l_res] = cld_cirrus_app.getoutput().get("out")

            # APRES MEMORY IN CHAIN CORE ALGORITHMS : V 4-1-0
            #                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud, extension and shadow masks
            #                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks and extension
            #                    Bit 3 - Cloud_Mask.shadows : shadows mask from clouds within image
            #                    Bit 4 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
            #                    Bit 5 - Cloud_Mask.refl : reflectance threshold
            #                    Bit 6 - Cloud_Mask.refl_var : reflectance variation threshold
            #                    Bit 7 - Cloud_Mask.extension : extension of the cloud mask
            #                        Bit 8 - Cloud_Mask.alt : stereoscopic mask  =>  VENUS et vide pour les autres
            #                    Bit 9 - Cirrus : Pour  L8 et S2 et vide pour les autres

            # FORMAT DISK
            #                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud and shadow masks
            #                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks
            #                    Bit 3 - Cloud_Mask.shadows : shadows mask from clouds within image
            #                    Bit 4 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
            #                    Bit 5 - Cloud_Mask.refl : reflectance threshold
            #                    Bit 6 - Cloud_Mask.refl_var : reflectance variation threshold
            #                    Bit 7 - Cloud_Mask.extension : extension of the cloud mask
            #                    Bit 8 - VENUS : Cloud_Mask.alt : stereoscopic mask
            #                    Bit 8 - L8 et S2 : Bit 9 - Cirrus
            #                    Bit 8 - Vide pour les autres
            cld_list = []
            cld_list.append(dict_of_output[CLOUD_MASK_ALL + "_" + l_res])
            cld_list.append(dict_of_output[CLOUD_MASK_ALL_CLOUDS + "_" + l_res])
            cld_list.append(dict_of_output[CLOUD_MASK_SHADOWS + "_" + l_res])
            cld_list.append(dict_of_output[CLOUD_MASK_SHADVAR + "_" + l_res])
            cld_list.append(dict_of_output[CLOUD_MASK_REFL + "_" + l_res])
            cld_list.append(dict_of_output[CLOUD_MASK_REFL_VAR + "_" + l_res])
            cld_list.append(dict_of_output[CLOUD_MASK_EXTENSION + "_" + l_res])
            cld_list.append(dict_of_output[CLOUD_MASK_ALT + "_" + l_res])
            cld_list.append(dict_of_output[CLOUD_MASK_CIRRUS + "_" + l_res])
            l2_cld_list.append(cld_list)


        dict_of_output["L2CLDList"] = l2_cld_list
