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

orchestrator.common.directory_manager -- shortdesc

orchestrator.common.directory_manager is a description

It defines classes_and_methods


###################################################################################################
"""

from orchestrator.common.maja_exceptions import MajaBusinessException


class ReadL1Mode:
    READ_L1_MODE_FOR_CHECKTOOL = 0
    READ_L1_MODE_FOR_ALGORITHMS = 1
    INVALID_READ_L1_MODE = 2


class AOTEstimation:
    MULTITEMPORAL = 0
    MULTISPECTRAL = 1
    SPECTROTEMPORAL = 2
    AOT_NOT_INITIALIZED = 3

    @staticmethod
    def get_aot_method(str_name):
        if str_name == "MULTITEMPORAL":
            method = AOTEstimation.MULTITEMPORAL
        elif str_name == "MULTISPECTRAL":
            method = AOTEstimation.MULTISPECTRAL
        elif str_name == "SPECTROTEMPORAL":
            method = AOTEstimation.SPECTROTEMPORAL
        else:
            raise MajaBusinessException("Utilities:GetAOTMethod: The AOT method " + str_name + "does not exist.")
        return method


class CAMSStatus:
    ACTIVATED_OK = 0
    ACTIVATED_NOAVAILABLEDATA = 1
    ACTIVATED_PARTIALLYAVAILABLEDATA = 2
    DEACTIVATED = 3

    @staticmethod
    def get_cams_status_string(status):
        if status == CAMSStatus.ACTIVATED_OK:
            str = "Activated_OK"
        elif status == CAMSStatus.ACTIVATED_NOAVAILABLEDATA:
            str = "Activated_NoAvailableData"
        elif status == CAMSStatus.ACTIVATED_PARTIALLYAVAILABLEDATA:
            str = "Activated_PartiallyAvailableData"
        elif status == CAMSStatus.DEACTIVATED:
            str = "Deactivated"
        else:
            raise MajaDataException("The Cams status " + status + "does not exist.")
        return str


CLOUD_MASK_ALL = "Cloud_Mask_all"
CLOUD_MASK_ALL_CLOUDS = "Cloud_Mask_all_clouds"
CLOUD_MASK_SHADOWS = "Cloud_Mask_shadows"
CLOUD_MASK_SHADVAR = "Cloud_Mask_shadvar"
CLOUD_MASK_REFL = "Cloud_Mask_refl"
CLOUD_MASK_REFL_VAR = "Cloud_Mask_refl_var"
CLOUD_MASK_EXTENSION = "Cloud_Mask_extension"
CLOUD_MASK_ALT = "Cloud_Mask_alt"
CLOUD_MASK_CIRRUS = "Cloud_Mask_cirrus"
