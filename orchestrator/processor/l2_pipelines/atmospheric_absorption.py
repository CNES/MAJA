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
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.common.xml_tools import as_bool
from orchestrator.common.maja_exceptions import *
import os
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.directory_manager import DirectoryManager
LOGGER = configure_logger(__name__)


class MajaAthmosphericAbsorption(MajaModule):

    """
    classdocs

    """
    NAME = "AthmosphericAbsorption"

    def __init__(self):
        """
        Constructor
        """
        super(MajaAthmosphericAbsorption, self).__init__()
        self._coarse_pipeline = OtbPipelineManager()
        self._l2_pipeline = OtbPipelineManager()
        self.in_keys_to_check = ["Params.WriteL2ProductToL2Resolution", "Params.RealL2NoData",
                                 "Params.OzoneAmount",
                                 "AppHandler", "Plugin", "L1Reader", "L1Info" ,
                                 "L2COMM", "DEM"]
        self.out_keys_to_check = []
        self.out_keys_provided = ["AtmoAbsIPTOAC"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("AthmosphericAbsorption start")
        atm_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("AtmoProc_",
                                            do_always_remove=True)
        l_writeL2 = dict_of_input.get("Params").get("WriteL2ProductToL2Resolution")
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        # Get some datas

        pressure_image_sub = os.path.join(atm_working, "atmo_pressure_sub.tif")
        param_pressure = {"dtm": dict_of_input.get("DEM").ALC,
                          "pressure": pressure_image_sub}
        pressure_app_sub = OtbAppHandler("PressureFilter", param_pressure,
                                         write_output=True)
        self._coarse_pipeline.add_otb_app(pressure_app_sub)
        viewing_zenith = dict_of_input.get("L1Info").ListOfViewingZenithAnglesPerBandAtL2CoarseResolution
        water_amount_image_sub = os.path.join(atm_working, "atmo_wateramount_sub.tif")
        if dict_of_input.get("Plugin").WaterVapourDetermination and not as_bool(
           dict_of_input.get("L2COMM").get_value("GIP_L2COMM_UseDefaultConstantWaterAmount")):
            watervaporreferencebandid = bands_definition.get_band_id_in_l2_coarse(
                dict_of_input.get("L2COMM").get_value("WaterVaporReferenceBandCode"))
            watervaporbandid = bands_definition.get_band_id_in_l2_coarse(
                dict_of_input.get("L2COMM").get_value("WaterVaporBandCode"))

            viewing_zenith_vapor_band = float(viewing_zenith[watervaporbandid])
            param_watervapour = {"toa": dict_of_input.get("L1Reader").get_value("IPTOASubOutput"),
                                 "watervaporreferencebandid": watervaporreferencebandid,
                                 "watervaporbandid": watervaporbandid,
                                 "vapnodatavalue": float(dict_of_input.get("L2COMM").get_value("VAPNodataValue")),
                                 "watervapormaxthreshold": float(
                                     dict_of_input.get("L2COMM").get_value("WaterVaporMaxThreshold")),
                                 "solarzenith": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                                 "viewingzenith": viewing_zenith_vapor_band,
                                 "wateramount": water_amount_image_sub,
                                 "gipwatv": dict_of_input.get("L2WATV").new_gipp_filename}
            water_vapour_app_sub = OtbAppHandler("WaterAmountGeneratorFilter", param_watervapour,
                                                 write_output=False)
            self._coarse_pipeline.add_otb_app(water_vapour_app_sub)
            dict_of_output["VAP_Sub"] = water_vapour_app_sub.getoutput()["wateramount"]

        toac_image_sub = os.path.join(atm_working, "atmo_toac_sub.tif")
        param_toa_correction = {"toa": dict_of_input.get("L1Reader").get_value("IPTOASubOutput"),
                                "pressure": pressure_app_sub.getoutput()["pressure"],
                                "smac": dict_of_input.get("L2SMAC").new_gipp_filename,
                                "wateramountdefault": float(
                                    dict_of_input.get("L2COMM").get_value("WaterAmountDefaultValue")),
                                "nodata": dict_of_input.get("Params").get("RealL2NoData"),
                                "ozoneamount": dict_of_input.get("Params").get("OzoneAmount"),
                                "thetas": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                                "thetav": viewing_zenith,
                                "bandlist": bands_definition.get_list_of_band_id_in_l2_coarse(),
                                "toac": toac_image_sub}
        if (dict_of_input.get("Plugin").WaterVapourDetermination and
                dict_of_input.get("L2COMM").get_value("GIP_L2COMM_UseDefaultConstantWaterAmount")):
            param_toa_correction["wateramount"] = dict_of_output["VAP_Sub"]

        toa_correction_app_sub = OtbAppHandler("TOACorrection", param_toa_correction, write_output=l_writeL2)
        if not l_writeL2:
            self._coarse_pipeline.add_otb_app(toa_correction_app_sub)

        dict_of_output["AtmoAbsIPTOAC"] = toa_correction_app_sub.getoutput()["toac"]

        if dict_of_input.get("Params").get("WriteL2ProductToL2Resolution"):
            l_nbRes = len(bands_definition.ListOfL2Resolution)
            for r in range(0, l_nbRes):
                l_res = bands_definition.ListOfL2Resolution[r]
                pressure_image = os.path.join(atm_working, "atmo_pressure_" + l_res + ".tif")
                param_pressure["dtm"] = dict_of_input.get("DEM").ALTList[r]
                param_pressure["pressure"] = pressure_image
                pressure_app = OtbAppHandler("PressureFilter", param_pressure,write_output=False)
                self._l2_pipeline.add_otb_app(pressure_app)
                water_image = os.path.join(atm_working, "atmo_wateramount_" + l_res + ".tif")
                if (dict_of_input.get("Plugin").WaterVapourDetermination and
                        dict_of_input.get("L2COMM").get_value("GIP_L2COMM_UseDefaultConstantWaterAmount")):
                    param_resamp = {"dtm": dict_of_input.get("DEM").ALTList[r],
                                    "im": dict_of_output["VAP_Sub"],
                                    "interp": "linear",
                                    "padradius": 4.0,
                                    "out": water_image}
                    water_app = OtbAppHandler("Resampling", param_resamp, write_output=False)
                    self._l2_pipeline.add_otb_app(water_app)
                    param_toa_correction["wateramount"] = water_app.getoutput().get("out")
                toac_image = os.path.join(atm_working, "atmo_toa_" + l_res + ".tif")
                param_toa_correction["toa"] = dict_of_input.get("L1Reader").get_value("L2TOAImageList")[r]
                param_toa_correction["pressure"] = pressure_app.getoutput().get("pressure")
                band_list = bands_definition.get_list_of_l2_band_id(l_res)
                LOGGER.debug("Athmo band_list")
                LOGGER.debug(band_list)

                viewing_zenith_l2 = []
                band_list_l2 = []
                for i in band_list:
                    viewing_zenith_l2.append(viewing_zenith[i])
                    band_list_l2.append(str(i))
                param_toa_correction["bandlist"] = band_list_l2
                param_toa_correction["thetav"] = viewing_zenith_l2
                param_toa_correction["toac"] = toac_image
                toa_app = OtbAppHandler("TOACorrection", param_toa_correction,write_output=False)
                self._l2_pipeline.add_otb_app(toa_app)
                dict_of_output["AtmoAbsIPTOA_" + l_res] = toa_app.getoutput().get("toac")
                dict_of_output["L2TOA_" + l_res] = toa_app.getoutput().get("toac")

