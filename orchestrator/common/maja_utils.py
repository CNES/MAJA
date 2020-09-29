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
                        o  o  o o    o      o o    o     |||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################
"""
import os
import orchestrator.common.date_utils as date_utils
from orchestrator.common.maja_exceptions import MajaDataException


def split_string(str_str, sep, remove_empty=True):
    ret = str_str.split(sep)
    if not remove_empty:
        return ret
    else:
        return [f for f in ret if len(f) != 0]


def split_filename(str_str, sep, without_extension=True):
    l_reffilename = os.path.basename(str_str)
    if without_extension:
        l_reffilename = os.path.splitext(l_reffilename)[0]
    return split_string(l_reffilename, sep, True)


def format_to_string(value, format_str):
    return format_str % value


def remove_trailing_underscores(nickname):
    if "_" in nickname:
        return nickname[: nickname.find("_") - 1]
    else:
        return nickname


def get_formated_site(site):
    site_final = "________"
    if len(site) < 8:
        site_final = site + site_final[len(site):]
        return site_final
    else:
        return site


def get_integer_jday_from_filename(filename):
    # ---------------------------------------------------------------------------------------------
    # 4.3 find automatically the date in the filename. Find 'number' and without character
    # to be sur, remove the extention for working with this example: .._TOTO_20150414.mha
    # revert loop and finds number
    # Used to detect the date for :
    # L2 EarthExplorer product              : ..../S4_TEST_S4C_L2VALD_AFAngola00D00B_20130223.HDR -> detects 20130223
    # L2 Private EarthExplorer LTC mha file :
    # ...LTC.DBL.DIR/S4_TEST_S4C_LUTANX_L2VALD_CVERSAIL00D00B_20130402_LTC.DBL.mha
    # -> detects 20130402
    filenamekey = split_string(os.path.splitext(os.path.basename(filename))[0], "_", True)
    number_list = [f for f in filenamekey if f.isdigit()]
    if len(number_list) == 0:
        raise MajaDataException("Filename " + filename + " does not contains a date")
    indate = date_utils.get_datetime_from_yyyymmdd(number_list[-1])
    return date_utils.get_julianday_as_int(indate)


def get_test_mode():
    if "MAJA_ORCH_TEST_MODE" in os.environ:
        return True
    else:
        return False


def is_croco_on(module=None):
    if "MAJA_CROCO_MODE" in os.environ:
        if module is None:
            return "GLOBAL_ON" in os.environ["MAJA_CROCO_MODE"]
        else:
            return module+"_ON" in os.environ["MAJA_CROCO_MODE"]
    else:
        return False


def is_croco_off(module=None):
    if "MAJA_CROCO_MODE" in os.environ:
        if module is None:
            return "GLOBAL_OFF" in os.environ["MAJA_CROCO_MODE"]
        else:
            return module+"_OFF" in os.environ["MAJA_CROCO_MODE"]
    else:
        return False
