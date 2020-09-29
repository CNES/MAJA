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
                        o  o  o o    o      o o    o     |||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################

:copyright: 2019 CNES. All rights reserved.
:license: license

###################################################################################################
"""
import os
import orchestrator.common.maja_utils as maja_utils
import orchestrator.common.maja_common as maja_common


class EarthExplorerUtilities:
    CONST_GIPP_FILE_LOCATION_NV = "NIL=OTHER:EXTERNAL"

    # Return the SatelliteClass:  VE, F2, L_, S2A, S2_, ...
    @staticmethod
    def get_satellite_class_from_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        # Split with '_'
        l_listofstr = maja_utils.split_string(l_parse_filename, '_', False)
        l_value = l_listofstr[0]
        # Find consecutive '_'
        cpt = 1
        while cpt < len(l_listofstr) and len(l_listofstr[cpt]) == 0:
            l_value = l_value + "_"
            cpt = cpt + 1
        return l_value

    # Return the FileClass:  TEST, OPER, ...  4characters
    @staticmethod
    def get_fileclass_from_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        # Find first '_' because SENTINEL2 startswith 3 characters
        l_satclass = EarthExplorerUtilities.get_satellite_class_from_filename(l_parse_filename)
        return l_parse_filename[len(l_satclass) + 1: len(l_satclass) + 1 + 4]

    # Return the FileClass:  VSC, LSC, FSC, SSC, GIP... 3 characters
    @staticmethod
    def get_file_type_short_from_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        l_satclass = EarthExplorerUtilities.get_satellite_class_from_filename(l_parse_filename)
        return l_parse_filename[len(l_satclass) + 6: len(l_satclass) + 6 + 3]

    # Return the LevelClass: # For Global HDR: L1VALD, L2VALD, L2NOTV,..
    # For Annex:     PDTANX, PDTIMG, ...
    #  For GIPP:      L2TOCR, L2COMM, ...
    @staticmethod
    def get_level_class_from_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        l_satclass = EarthExplorerUtilities.get_satellite_class_from_filename(l_parse_filename)
        return l_parse_filename[len(l_satclass) + 6 + 3 + 1: len(l_satclass) + 6 + 3 + 1 + 6]

    # Return the FileClass:  VSC_L1VALD, LSC_L2VALD, FSC_L2NOTV, SSC...
    # For Global HDR: VSC_L1VALD, LSC_L2VALD, FSC_L2NOTV, SSC...
    #  For Annex:     VSC_PDTANX, LSC_PDTIMG, ...
    # For Gipp: GIP_L2TOCR
    @staticmethod
    def get_file_type_from_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        l_satclass = EarthExplorerUtilities.get_satellite_class_from_filename(l_parse_filename)
        return l_parse_filename[len(l_satclass) + 6: len(l_satclass) + 6 + 3 + 1 + 6]

    # 012345678901234567890123456789
    # case 1: VE_VM01_VSC_L1VALD_PERU_____20120101.HDR
    # case 2: L7_TEST_LSC_L2VALD_EU93066200A00B_20100315.HDR
    @staticmethod
    def get_nickname_from_global_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        l_satclass = EarthExplorerUtilities.get_satellite_class_from_filename(l_parse_filename)
        return EarthExplorerUtilities.get_internal_nickname_from_filename(l_parse_filename, len(l_satclass) + 17)

    @staticmethod
    def get_nickname_from_annex_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        l_satclass = EarthExplorerUtilities.get_satellite_class_from_filename(l_parse_filename)
        return EarthExplorerUtilities.get_internal_nickname_from_filename(l_parse_filename, len(l_satclass) + 17 + 7)

    @staticmethod
    def get_nickname_from_gipp_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        l_satclass = EarthExplorerUtilities.get_satellite_class_from_filename(l_parse_filename)
        return EarthExplorerUtilities.get_internal_nickname_from_filename(l_parse_filename, len(l_satclass) + 17 + 2)

    @staticmethod
    def remove_trailing_underscores(nickname):
        if nickname.find("_") != -1:
            return nickname[0: nickname.find("_")]
        else:
            return nickname

    @staticmethod
    def get_acquisition_from_global_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        return l_parse_filename[l_parse_filename.rfind("_") + 1::]

    # For Annex File, return Nature
    @staticmethod
    def get_nature_from_filename(filename):
        l_parse_filename = EarthExplorerUtilities.parse_filename(filename)
        return EarthExplorerUtilities.get_file_type_from_filename(l_parse_filename)

    @staticmethod
    def parse_filename(filename):
        l_reffilename = os.path.basename(filename)
        l_reffilename = os.path.splitext(l_reffilename)[0]
        return l_reffilename

    @staticmethod
    def get_internal_nickname_from_filename(filename, index_start):
        # case 1: VE_VM01_VSC_L1VALD_PERU_____20120101.HDR
        # case 2: L7_TEST_LSC_L2VALD_EU93066200A00B_20100315.HDR
        if filename[index_start + 8: index_start + 9] == "_":
            # case 1
            return filename[index_start: index_start + 8]
        else:
            # case 2
            # get EU93066200A00B_20100315.HDR
            l_tmp = filename[index_start::]
            return l_tmp[:: l_tmp.find("_")]

    #         Return the gipp with nickname is a subset of the key
    # For example S4_TEST_GIP_L2TOCR_L_CONTINEN_00001_00000000_99999999.HDR
    # will be returned if the key to search is CONTINENTAL
    @staticmethod
    def find_gipp_with_sub_set_of_value(p_gipps, p_tosearch):
        l_norms = p_tosearch.lower()
        for g in p_gipps:
            l_tmp = maja_utils.remove_trailing_underscores(EarthExplorerUtilities.get_nickname_from_gipp_filename(g))
            l_tmp = l_tmp.lower()
            if l_tmp in l_norms:
                return g
        return None

    # ConvertGIPPFilenameToFileType */
    @staticmethod
    def convert_gipp_filename_to_file_type(pfilename, file_location):
        l_filenamewithoutextension = os.path.splitext(pfilename)[0]
        lgippfile = maja_common.GIPPFile()
        lgippfile.Nature = EarthExplorerUtilities.get_nature_from_filename(pfilename)
        lgippfile.FileLocation = file_location
        lgippfile.LogicalName = l_filenamewithoutextension
        return lgippfile

    # ConvertGIPPFilenameToFileType */
    @staticmethod
    def convert_one_gipp_filename_to_file_type(pfilename):
        l_filenamewithoutextension = os.path.splitext(pfilename)[0]
        lgippfile = maja_common.GIPPFile()
        lgippfile.Nature = EarthExplorerUtilities.get_nature_from_filename(pfilename)
        lgippfile.FileLocation = EarthExplorerUtilities.CONST_GIPP_FILE_LOCATION_NV
        lgippfile.LogicalName = l_filenamewithoutextension
        return lgippfile

    # ConvertGIPPFilenameToFileType */
    @staticmethod
    def convert_gipp_filenames_to_file_types(pfilenames):
        llistoffiles = []
        for f in pfilenames:
            llistoffiles.append(EarthExplorerUtilities.convert_one_gipp_filename_to_file_type(f))
        return llistoffiles

    @staticmethod
    def get_regular_file_from_file(filename):
        subdir = os.path.basename(os.path.dirname(filename))
        filenamewithoutextension = os.path.splitext(filename)[0]

        regfile = maja_common.RegularFile()
        regfile.Nature = EarthExplorerUtilities.get_nature_from_filename(filename)
        regfile.FileLocation = "./" + subdir + "/" + os.path.basename(filename)
        regfile.LogicalName = filenamewithoutextension
        return regfile
