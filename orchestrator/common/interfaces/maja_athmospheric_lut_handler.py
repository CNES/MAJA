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

from orchestrator.common.interfaces.maja_lut_converter import LutConverter
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.common.earth_explorer.earth_explorer_utilities import EarthExplorerUtilities
from orchestrator.common.earth_explorer.gipp_lut_earch_explorer_xml_file_handler import \
    GippLUTEarthExplorerXMLFileHandler
import orchestrator.common.gipp_utils as gipp_utils
import orchestrator.common.file_utils as file_utils
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.interfaces.maja_xml_app_lut import *
from orchestrator.algorithms.raw_multiply.raw_multiply import multiply_and_add
import os
import io

LOGGER = configure_logger(__name__)


class AthmosphericLutHandler(object):

    def __init__(self):
        self.GIP_L2DIRT_LookUpTableConverter = {}
        self.GIP_L2DIFT_LookUpTableConverter = {}
        self.GIP_L2ALBD_LookUpTableConverter = {}
        self.GIP_L2TOCR_LookUpTableConverter = {}
        self._proportions = {}
        self._synthetized = False
        self._new_gipp_filename_tocr = ""
        self._new_gipp_filename_dift = ""
        self._new_gipp_filename_dirt = ""
        self._new_gipp_filename_albd = ""

    def add_lut_for_model(self, p_indir, p_workdir, p_sat, p_model, p_validate_schemas, p_plugin_schema_dir,
                          p_userh=False, p_rh=0):

        LOGGER.debug("Loading LUT for model " + p_model + " and satellite " + p_sat + " (use rh : " + str(p_userh))
        # *************************************************************************************************************
        # Read the GIPP file "GIP_L2DIRT"
        # *************************************************************************************************************
        LOGGER.info("Starting GIP_L2DIRT reading ...")
        # Get the filename of the GIPP
        l_gippfilenames = gipp_utils.get_list_of_gipp_filenames_with_mission(p_indir, "GIP_L2DIRT", p_sat)
        l_gippfilename = EarthExplorerUtilities.find_gipp_with_sub_set_of_value(l_gippfilenames, p_model)
        if l_gippfilename is None:
            raise MajaDataException("Missing input L2DIRT gipp for model " + p_model)
        else:
            LOGGER.debug("L2DIRT filename : " + l_gippfilename)
            lgipp_dbl = os.path.splitext(l_gippfilename)[0] + ".DBL"
            gipp_utils.uncompress_dbl_product(lgipp_dbl)
            dirt_converter = LutConverter(p_workdir, l_gippfilename,
                                          validate_schema=p_validate_schemas, schema_path=p_plugin_schema_dir,
                                          use_rh=p_userh, rh=p_rh)
            self.GIP_L2DIRT_LookUpTableConverter[p_model] = dirt_converter

        # *************************************************************************************************************
        # Read the GIPP file "GIP_L2DIFT"
        # *************************************************************************************************************
        LOGGER.info("Starting GIP_L2DIFT reading ...")
        # --------------------------------------------
        # Get the filename of the GIPP
        l_gippfilenames = gipp_utils.get_list_of_gipp_filenames_with_mission(p_indir, "GIP_L2DIFT", p_sat)
        l_gippfilename = EarthExplorerUtilities.find_gipp_with_sub_set_of_value(l_gippfilenames, p_model)
        if l_gippfilename is None:
            raise MajaDataException("Missing input L2DIFT gipp for model " + p_model)
        else:
            LOGGER.debug("L2DIFT filename : " + l_gippfilename)
            lgipp_dbl = os.path.splitext(l_gippfilename)[0] + ".DBL"
            gipp_utils.uncompress_dbl_product(lgipp_dbl)
            dift_converter = LutConverter(p_workdir, l_gippfilename,
                                          validate_schema=p_validate_schemas, schema_path=p_plugin_schema_dir,
                                          use_rh=p_userh, rh=p_rh)
            self.GIP_L2DIFT_LookUpTableConverter[p_model] = dift_converter

        # *************************************************************************************************************
        # Read the GIPP file "GIP_L2ALBD"
        # *************************************************************************************************************
        LOGGER.info("Starting GIP_L2ALBD reading ...")
        # --------------------------------------------
        # Get the filename of the GIPP
        l_gippfilenames = gipp_utils.get_list_of_gipp_filenames_with_mission(p_indir, "GIP_L2ALBD", p_sat)
        l_gippfilename = EarthExplorerUtilities.find_gipp_with_sub_set_of_value(l_gippfilenames, p_model)
        if l_gippfilename is None:
            raise MajaDataException("Missing input L2ALBD gipp for model " + p_model)
        else:
            LOGGER.debug("L2ALBD filename : " + l_gippfilename)
            lgipp_dbl = os.path.splitext(l_gippfilename)[0] + ".DBL"
            gipp_utils.uncompress_dbl_product(lgipp_dbl)
            albd_converter = LutConverter(p_workdir, l_gippfilename,
                                          validate_schema=p_validate_schemas, schema_path=p_plugin_schema_dir,
                                          use_rh=p_userh, rh=p_rh)
            self.GIP_L2ALBD_LookUpTableConverter[p_model] = albd_converter

        # *************************************************************************************************************
        # Read the GIPP file "GIP_L2TOCR"
        # *************************************************************************************************************
        LOGGER.info("Starting GIP_L2TOCR reading ...")
        l_gippfilenames = gipp_utils.get_list_of_gipp_filenames_with_mission(p_indir, "GIP_L2TOCR", p_sat)
        l_gippfilename = EarthExplorerUtilities.find_gipp_with_sub_set_of_value(l_gippfilenames, p_model)
        if l_gippfilename is None:
            raise MajaDataException("Missing input L2TOCR gipp for model " + p_model)
        else:
            LOGGER.debug("L2TOCR filename : " + l_gippfilename)
            lgipp_dbl = os.path.splitext(l_gippfilename)[0] + ".DBL"
            gipp_utils.uncompress_dbl_product(lgipp_dbl)
            tocr_converter = LutConverter(p_workdir, l_gippfilename,
                                          validate_schema=p_validate_schemas, schema_path=p_plugin_schema_dir,
                                          use_rh=p_userh, rh=p_rh)
            self.GIP_L2TOCR_LookUpTableConverter[p_model] = tocr_converter

    def set_proportions(self, props):
        self._proportions = dict(props)

    def get_albd_for_model(self, model):
        if model in list(self.GIP_L2ALBD_LookUpTableConverter.keys()):
            return self.GIP_L2ALBD_LookUpTableConverter[model].new_gipp_filename
        else:
            raise MajaDataException("No " + model + " in list of available ALBD luts")

    def get_dift_for_model(self, model):
        if model in list(self.GIP_L2DIFT_LookUpTableConverter.keys()):
            return self.GIP_L2DIFT_LookUpTableConverter[model].new_gipp_filename
        else:
            raise MajaDataException("No " + model + " in list of available DIFT luts")

    def get_dirt_for_model(self, model):
        if model in list(self.GIP_L2DIRT_LookUpTableConverter.keys()):
            return self.GIP_L2DIRT_LookUpTableConverter[model].new_gipp_filename
        else:
            raise MajaDataException("No " + model + " in list of available DIRT luts")

    def get_tocr_for_model(self, model):
        if model in list(self.GIP_L2TOCR_LookUpTableConverter.keys()):
            return self.GIP_L2TOCR_LookUpTableConverter[model].new_gipp_filename
        else:
            raise MajaDataException("No " + model + " in list of available TOCR luts")

    def get_synth_albd(self):
        if not self._synthetized:
            raise MajaDataException("Luts have not been synthetised yet")
        return self._new_gipp_filename_albd

    def get_synth_dift(self):
        if not self._synthetized:
            raise MajaDataException("Luts have not been synthetised yet")
        return self._new_gipp_filename_dift

    def get_synth_dirt(self):
        if not self._synthetized:
            raise MajaDataException("Luts have not been synthetised yet")
        return self._new_gipp_filename_dirt

    def get_synth_tocr(self):
        if not self._synthetized:
            raise MajaDataException("Luts have not been synthetised yet")
        return self._new_gipp_filename_tocr

    def gen_synthetised_luts(self, p_workdir):
        # Generate luts based on proportions of each model
        if self._proportions is None:
            raise MajaDataException("No proportions given in athmospheric lut synthetize")
        l_nbfiles = 0
        l_listoflistoffiles_tocr = []
        l_listoflistoffiles_dirt = []
        l_listoflistoffiles_dift = []
        l_listoflistoffiles_albd = []
        for m in list(self._proportions.keys()):
            LOGGER.debug("Treating model : "+m)
            if m not in self.GIP_L2TOCR_LookUpTableConverter or m not in self.GIP_L2DIFT_LookUpTableConverter \
                    or m not in self.GIP_L2DIRT_LookUpTableConverter or m not in self.GIP_L2ALBD_LookUpTableConverter:
                raise MajaDataException("No " + m + " in luts while asked in proportions")
            else:
                if l_nbfiles == 0:
                    l_nbfiles = len(self.GIP_L2TOCR_LookUpTableConverter[m].new_list_of_files)
                    for f in range(l_nbfiles):
                        l_listoflistoffiles_tocr.append([(self._proportions[m],
                                                          os.path.join(os.path.dirname(
                                                              self.GIP_L2TOCR_LookUpTableConverter[
                                                                  m].new_gipp_filename),
                                                              self.GIP_L2TOCR_LookUpTableConverter[m].new_list_of_files[
                                                                  f]))]
                                                        )
                        l_listoflistoffiles_dift.append([(self._proportions[m],
                                                         os.path.join(os.path.dirname(
                                                             self.GIP_L2DIFT_LookUpTableConverter[m].new_gipp_filename),
                            self.GIP_L2DIFT_LookUpTableConverter[
                                                             m].new_list_of_files[f]))]
                                                        )
                        l_listoflistoffiles_dirt.append([(self._proportions[m],
                                                         os.path.join(os.path.dirname(
                                                             self.GIP_L2DIRT_LookUpTableConverter[m].new_gipp_filename),
                            self.GIP_L2DIRT_LookUpTableConverter[
                                                             m].new_list_of_files[f]))]
                                                        )
                        l_listoflistoffiles_albd.append([(self._proportions[m],
                                                         os.path.join(os.path.dirname(
                                                             self.GIP_L2ALBD_LookUpTableConverter[m].new_gipp_filename),
                            self.GIP_L2ALBD_LookUpTableConverter[
                                                             m].new_list_of_files[f]))]
                                                        )

                elif l_nbfiles != len(self.GIP_L2TOCR_LookUpTableConverter[m].new_list_of_files):
                    raise MajaDataException("Not the same number of subfiles in luts for model : "+m)
                else:
                    for f in range(l_nbfiles):
                        l_listoflistoffiles_tocr[f].append(
                            (self._proportions[m],
                             os.path.join(
                                os.path.dirname(
                                    self.GIP_L2TOCR_LookUpTableConverter[m].new_gipp_filename),
                                self.GIP_L2TOCR_LookUpTableConverter[m].new_list_of_files[f])))
                        l_listoflistoffiles_dift[f].append((self._proportions[m],
                                                           os.path.join(os.path.dirname(
                                                               self.GIP_L2DIFT_LookUpTableConverter[
                                                                   m].new_gipp_filename),
                                                               self.GIP_L2DIFT_LookUpTableConverter[
                                                                   m].new_list_of_files[f])))
                        l_listoflistoffiles_dirt[f].append((self._proportions[m],
                                                           os.path.join(os.path.dirname(
                                                               self.GIP_L2DIRT_LookUpTableConverter[
                                                                   m].new_gipp_filename),
                                                               self.GIP_L2DIRT_LookUpTableConverter[
                                                                   m].new_list_of_files[f])))
                        l_listoflistoffiles_albd[f].append((self._proportions[m],
                                                           os.path.join(os.path.dirname(
                                                               self.GIP_L2ALBD_LookUpTableConverter[
                                                                   m].new_gipp_filename),
                                                               self.GIP_L2ALBD_LookUpTableConverter[
                                                                   m].new_list_of_files[f])))
        # build files
        new_working = p_workdir
        # TOCR
        new_gipp_tocr_filename = os.path.join(new_working, "Tmp_TOCR.HDR")
        new_dbldir_tocr_rel = os.path.splitext(os.path.basename(new_gipp_tocr_filename))[0] + ".DBL.DIR"
        new_dbldir_tocr = os.path.join(new_working, new_dbldir_tocr_rel)
        file_utils.create_directory(new_dbldir_tocr)
        new_dbl_tocr = List_Of_FilesType()
        # DIFT
        new_gipp_dift_filename = os.path.join(new_working, "Tmp_DIFT.HDR")
        new_dbldir_dift_rel = os.path.splitext(os.path.basename(new_gipp_dift_filename))[0] + ".DBL.DIR"
        new_dbldir_dift = os.path.join(new_working, new_dbldir_dift_rel)
        file_utils.create_directory(new_dbldir_dift)
        new_dbl_dift = List_Of_FilesType()
        # DIRT
        new_gipp_dirt_filename = os.path.join(new_working, "Tmp_DIRT.HDR")
        new_dbldir_dirt_rel = os.path.splitext(os.path.basename(new_gipp_dirt_filename))[0] + ".DBL.DIR"
        new_dbldir_dirt = os.path.join(new_working, new_dbldir_dirt_rel)
        file_utils.create_directory(new_dbldir_dirt)
        new_dbl_dirt = List_Of_FilesType()
        # ALBD
        new_gipp_albd_filename = os.path.join(new_working, "Tmp_ALBD.HDR")
        new_dbldir_albd_rel = os.path.splitext(os.path.basename(new_gipp_albd_filename))[0] + ".DBL.DIR"
        new_dbldir_albd = os.path.join(new_working, new_dbldir_albd_rel)
        file_utils.create_directory(new_dbldir_albd)
        new_dbl_albd = List_Of_FilesType()

        # Multiply and write bin files
        # TOCR
        for m in l_listoflistoffiles_tocr:
            l_newraw_tocr = os.path.join(new_dbldir_tocr, os.path.basename(m[0][1]))
            l_newraw_tocr_rel = os.path.join(new_dbldir_tocr_rel, os.path.basename(m[0][1]))
            for f in m:
                LOGGER.debug("Multiply " + f[1] + " by " + str(f[0]))
                multiply_and_add(f[1], l_newraw_tocr, f[0])
            new_dbl_tocr.add_Relative_File_Path(l_newraw_tocr_rel)
        # DIFT
        for m in l_listoflistoffiles_dift:
            l_newraw_dift = os.path.join(new_dbldir_dift, os.path.basename(m[0][1]))
            l_newraw_dift_rel = os.path.join(new_dbldir_dift_rel, os.path.basename(m[0][1]))
            for f in m:
                LOGGER.debug("Multiply " + f[1] + " by " + str(f[0]))
                multiply_and_add(f[1], l_newraw_dift, f[0])
            new_dbl_dift.add_Relative_File_Path(l_newraw_dift_rel)
        # DIRT
        for m in l_listoflistoffiles_dirt:
            l_newraw_dirt = os.path.join(new_dbldir_dirt, os.path.basename(m[0][1]))
            l_newraw_dirt_rel = os.path.join(new_dbldir_dirt_rel, os.path.basename(m[0][1]))
            for f in m:
                LOGGER.debug("Multiply " + f[1] + " by " + str(f[0]))
                multiply_and_add(f[1], l_newraw_dirt, f[0])
            new_dbl_dirt.add_Relative_File_Path(l_newraw_dirt_rel)
        # ALBD
        for m in l_listoflistoffiles_albd:
            l_newraw_albd = os.path.join(new_dbldir_albd, os.path.basename(m[0][1]))
            l_newraw_albd_rel = os.path.join(new_dbldir_albd_rel, os.path.basename(m[0][1]))
            for f in m:
                LOGGER.debug("Multiply " + f[1] + " by " + str(f[0]))
                multiply_and_add(f[1], l_newraw_albd, f[0])
            new_dbl_albd.add_Relative_File_Path(l_newraw_albd_rel)

        # TOCR
        # build root node
        new_lut_tocr = LUT(self.GIP_L2TOCR_LookUpTableConverter[list(self._proportions.keys())[0]].new_lut_indexes, new_dbl_tocr)
        # build file
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        new_lut_tocr.export(output, 0, name_='LUT', namespacedef_='', pretty_print=True)
        with open(new_gipp_tocr_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
            LOGGER.info("Writed new gipp lut to " + new_gipp_tocr_filename)
        # DIFT
        # build root node
        new_lut_dift = LUT(self.GIP_L2DIFT_LookUpTableConverter[list(self._proportions.keys())[0]].new_lut_indexes, new_dbl_dift)
        # build file
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        new_lut_dift.export(output, 0, name_='LUT', namespacedef_='', pretty_print=True)
        with open(new_gipp_dift_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
            LOGGER.info("Writed new gipp lut to " + new_gipp_dift_filename)
        # DIRT
        # build root node
        new_lut_dirt = LUT(self.GIP_L2DIRT_LookUpTableConverter[list(self._proportions.keys())[0]].new_lut_indexes, new_dbl_dirt)
        # build file
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        new_lut_dirt.export(output, 0, name_='LUT', namespacedef_='', pretty_print=True)
        with open(new_gipp_dirt_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
            LOGGER.info("Writed new gipp lut to " + new_gipp_dirt_filename)
        # ALBD
        # build root node
        new_lut_albd = LUT(self.GIP_L2ALBD_LookUpTableConverter[list(self._proportions.keys())[0]].new_lut_indexes, new_dbl_albd)
        # build file
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        new_lut_albd.export(output, 0, name_='LUT', namespacedef_='', pretty_print=True)
        with open(new_gipp_albd_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
            LOGGER.info("Writed new gipp lut to " + new_gipp_albd_filename)

        # Write in instance
        self._synthetized = True
        self._new_gipp_filename_tocr = new_gipp_tocr_filename
        self._new_gipp_filename_dift = new_gipp_dift_filename
        self._new_gipp_filename_dirt = new_gipp_dirt_filename
        self._new_gipp_filename_albd = new_gipp_albd_filename

    @staticmethod
    def extract_cams_info(p_indir, p_sat, p_model):
        # *************************************************************************************************************
        # Read the GIPP file "GIP_L2TOCR"
        # *************************************************************************************************************
        LOGGER.info("Starting GIP_L2TOCR reading ...")
        l_gippfilenames = gipp_utils.get_list_of_gipp_filenames_with_mission(p_indir, "GIP_L2TOCR", p_sat)
        l_gippfilename = EarthExplorerUtilities.find_gipp_with_sub_set_of_value(l_gippfilenames, p_model)
        if l_gippfilename is None:
            raise MajaDataException("Missing input L2TOCR gipp for model " + p_model)

        # CAMS informations read requested
        l_lut_xml_handler = GippLUTEarthExplorerXMLFileHandler(l_gippfilename)
        if l_lut_xml_handler.has_cams_info():
            return l_lut_xml_handler.get_cams_info()
        else:
            LOGGER.debug("GIPP doesn't have CAMS info available")
            return None
