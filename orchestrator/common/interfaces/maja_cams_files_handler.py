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

orchestrator.common.interfaces.maja_xml_input -- shortdesc

orchestrator.common.interfaces.maja_xml_input is a description

It defines classes_and_methods

###################################################################################################
"""
from orchestrator.common.logger.maja_logging import configure_logger
import orchestrator.common.date_utils as date_utils
import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.maja_exceptions import *
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.common.earth_explorer.gipp_cams_earch_explorer_xml_file_handler import GippCAMSEarthExplorerXMLFileHandler
from orchestrator.common.interfaces.maja_xml_app_cams import *
from orchestrator.common.maja_utils import get_test_mode

import os
import io
import math
LOGGER = configure_logger(__name__)


class CAMSFileHandler(object):

    def __init__(self, limaod, rhsampling, models, time_window, working_dir):
        self._limaod = limaod
        self._rhsampling = rhsampling
        self._rhtab = []
        self._models = models
        self._time_window = time_window
        self._working_dir = working_dir
        self._list_of_cams_description_file = []
        self._extinction_map = {}
        # internal
        self._beforefilefound = False
        self._afterfilefound = False
        self._beforeFile = None
        self._afterFile = None
        self.valid = False
        self.proportions = {}
        self.out_rh_sampling = 0
        self.out_sum_of_aot = None
        self.__camsapp = None
        # 2019/07/10 00:00:00 in JD:
        self._cams_46r1_switch = 2458674.5

    def set_rh_tab(self, levels):
        self._rhtab = levels

    def set_extinction_map(self, ext_map):
        self._extinction_map = ext_map

    def get_extinction_map(self):
        return self._extinction_map

    def initializelistofcamsfiledescription(self, list_of_cam_files):
        # ---------------------------------------------------------------------------------------------
        self._list_of_cams_description_file = []
        # Loops on the cams list
        for cam in list_of_cam_files:
            # Normaly, the prodcut is uncompress and XML file checked in the PreProcessing method
            l_hdrcamsfilename = os.path.splitext(cam)[0] + ".HDR"
            l_camsfiledescription = {}

            # Load the EarthExplorer XML file
            l_handler = GippCAMSEarthExplorerXMLFileHandler(l_hdrcamsfilename)
            # Converts these dates in Julian day
            l_camsfiledescription["date_utc"] = l_handler.acquisition_date_time
            l_camsfiledescription["date_jd"] = date_utils.get_julianday_as_double(
                date_utils.get_datetime_from_utc(l_handler.acquisition_date_time))
            l_camsfiledescription["filename"] = l_hdrcamsfilename
            # Read the files to get the various CAMS part
            l_list_of_date_filenames = l_handler.get_list_of_packaged_dbl_files(True, True)
            # For each file, search the type
            for f in l_list_of_date_filenames:
                l_filenamename = os.path.basename(f)
                # AOT File ?
                if "CAMS_AOT" in l_filenamename:
                    l_camsfiledescription["aot_file"] = f
                # MR File ?
                if "CAMS_MR" in l_filenamename:
                    l_camsfiledescription["mr_file"] = f
                # RH File ?
                if "CAMS_RH" in l_filenamename:
                    l_camsfiledescription["rh_file"] = f
            if "aot_file" not in l_camsfiledescription or "mr_file" not in l_camsfiledescription \
                    or "aot_file" not in l_camsfiledescription:
                LOGGER.info(l_camsfiledescription)
                raise MajaIOError("Missing one CAMS file")

            if xml_tools.get_only_value(l_handler.root, "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/NumberOfNonInterpolableValues", check=True) is not None:
                l_camsfiledescription["NbNonInterpolate"] = xml_tools.get_xml_int_value(l_handler.root, "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/NumberOfNonInterpolableValues")

            LOGGER.debug(
                "Add CAMS file for date [" +
                l_camsfiledescription["date_utc"] +
                ";" + str(l_camsfiledescription["date_jd"]) +
                "] -> file " +
                l_camsfiledescription["filename"])
            l_camsfiledescription["model_levels"] = [str(f) for f in l_handler.model_levels]
            self._list_of_cams_description_file.append(l_camsfiledescription)

    def searchvalidcamsfilenames(self, p_image_time_jd):
        self._afterfilefound = False
        self._beforefilefound = False
        # Log the product date
        LOGGER.debug("Searching CAMS for product julian date : " + str(p_image_time_jd))

        # ---------------------------------------------------------------------------------------------
        # Loops on the cam files
        for cam in self._list_of_cams_description_file:
            l_cur_cam_date_jd = cam["date_jd"]
            LOGGER.debug("CAM date : " + str(l_cur_cam_date_jd))
            # Is the CAMS less than a day before or after
            if math.fabs(p_image_time_jd - l_cur_cam_date_jd) <= (self._time_window / 24.0):
                # CAMS is before product
                if p_image_time_jd > l_cur_cam_date_jd:
                    if not self._beforefilefound:
                        self._beforeFile = cam
                        self._beforefilefound = True
                        # Log on current
                        LOGGER.debug("Before Cams: " + cam["filename"] + " with date " + cam["date_utc"])
                    else:
                        # Incoming file is after the stored
                        if self._beforeFile["date_jd"] <= l_cur_cam_date_jd:
                            self._beforeFile = cam
                            LOGGER.debug("Before Cams: " + cam["filename"] + " with date " + cam["date_utc"])
                else:
                    # CAMS is after product
                    if not self._afterfilefound:
                        self._afterFile = cam
                        self._afterfilefound = True
                        LOGGER.debug("After Cams: " + cam["filename"] + " with date " + cam["date_utc"])
                    else:
                        # Incoming file is before the stored
                        if self._afterFile["date_jd"] > l_cur_cam_date_jd:
                            self._afterFile = cam
                            LOGGER.debug("After Cams: " + cam["filename"] + " with date " + cam["date_utc"])

        #Do not mix new and old cams, if one has
        if self._afterfilefound and self._beforefilefound:
            if "NbNonInterpolate" in self._afterFile.keys() and not "NbNonInterpolate" in self._beforeFile.keys():
                self._beforefilefound = False
                self._beforeFile = None
            if not "NbNonInterpolate" in self._afterFile.keys() and "NbNonInterpolate" in self._beforeFile.keys():
                self._afterfilefound = False
                self._afterFile = None

        return self._afterfilefound or self._beforefilefound

    def has_cams_data(self, p_imageproductaquisitiontimejd):
        # Get the acquisition date in julian day
        self.searchvalidcamsfilenames(p_imageproductaquisitiontimejd)
        if not self._afterfilefound and not self._beforefilefound:
            LOGGER.info("No CAM file found for JD date " + str(p_imageproductaquisitiontimejd))
            return False
        else:
            return True

    def extract_cams_datas(self, corner_lat, corner_lon, p_imageproductaquisitiontimejd):
        self.valid = False
        if not self.searchvalidcamsfilenames(p_imageproductaquisitiontimejd):
            LOGGER.info("No CAM found for JD Date " + str(p_imageproductaquisitiontimejd))
            return
        
        if p_imageproductaquisitiontimejd < self._cams_46r1_switch:
            try:
                self._models.remove("AMMONIUM")
                self._models.remove("NITRATE")
                LOGGER.debug("Removed 46r1 models.")
            except ValueError:
                LOGGER.warning("Cannot remove 46r1 models. CAMS models present: %s" % self._models)

        cams_app_param = {"models": self._models,
                          "rhsampling": self._rhsampling,
                          "limaod": self._limaod,
                          "lat": corner_lat,
                          "lon": corner_lon,
                          "datejulian": p_imageproductaquisitiontimejd,
                          "sumofaot": os.path.join(self._working_dir, "tmpaot.tiff")
                          }

        # Create the xml instance of cams app configuration file
        xml_model_list = ModelListType()
        for model, mapex in list(self._extinction_map.items()):
            xml_model = ModelType()
            xml_extinction_coeff_list = Extinction_Coefs_ListType()
            count = 1
            for ext in mapex["extinction_coeffs"]:
                xml_extinction_coeff = Extinction_CoefType()
                xml_extinction_coeff.set_Name(ext["Name"])
                xml_extinction_coeff.set_Values(ext["Values"])
                xml_extinction_coeff.set_Description(ext["Description"])
                xml_extinction_coeff.set_n(count)
                count = count + 1
                xml_extinction_coeff_list.add_Extinction_Coef(xml_extinction_coeff)
            xml_extinction_coeff_list.set_count(len(model))
            xml_model.set_RhDep(bool(mapex["rh_dep"]))
            xml_model.set_Name(model)
            xml_model.set_Extinction_Coefs_List(xml_extinction_coeff_list)
            xml_model_list.add_Model(xml_model)
        xml_cams = CAMS()
        xml_cams.set_ModelList(xml_model_list)
        xml_cams.set_RhTab(self._rhtab)

        # build file
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        xml_cams.export(output, 0, name_='CAMS', namespacedef_='', pretty_print=True)
        xml_filename = os.path.join(self._working_dir, "cams_description.xml")
        with open(xml_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
            LOGGER.info("Writed cams description to " + xml_filename)

        cams_app_param["camsdescription"] = xml_filename

        if self._beforefilefound:
            cams_app_param["before.dateutc"] = self._beforeFile["date_utc"]
            cams_app_param["before.aotfile"] = self._beforeFile["aot_file"]
            cams_app_param["before.mrfile"] = self._beforeFile["mr_file"]
            cams_app_param["before.rhfile"] = self._beforeFile["rh_file"]
            cams_app_param["before.modellevels"] = self._beforeFile["model_levels"]
            if "NbNonInterpolate" in  self._beforeFile.keys():
                cams_app_param["before.nbnointerpol"] = self._beforeFile["NbNonInterpolate"]

        if self._afterfilefound:
            cams_app_param["after.dateutc"] = self._afterFile["date_utc"]
            cams_app_param["after.aotfile"] = self._afterFile["aot_file"]
            cams_app_param["after.mrfile"] = self._afterFile["mr_file"]
            cams_app_param["after.rhfile"] = self._afterFile["rh_file"]
            cams_app_param["after.modellevels"] = self._afterFile["model_levels"]
            if "NbNonInterpolate" in  self._beforeFile.keys():
                cams_app_param["after.nbnointerpol"] = self._afterFile["NbNonInterpolate"]

        self.__camsapp = OtbAppHandler("CAMSComputation", cams_app_param, write_output=True)

        l_app_proportions = self.__camsapp.getoutput().get("proportions")
        if get_test_mode():
            for m in xml_model_list.get_Model():
                self.proportions[m.get_Name()] = 1.0 / len(xml_model_list.get_Model())
        else:
            for p in l_app_proportions.split(";"):
                if len(p) != 0:
                    l_model = p.split("=")[0]
                    l_value = p.split("=")[1]
                    self.proportions[l_model] = l_value
        self.out_rh_sampling = self.__camsapp.getoutput().get("rh")
        self.valid = True
        self.out_sum_of_aot = self.__camsapp.getoutput().get("sumofaot")
