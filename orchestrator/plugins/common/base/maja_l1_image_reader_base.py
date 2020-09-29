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
#                        o  o  o o    o      o o    o     |||)<
#                        o     o oooooo      o oooooo      / \
#                        o     o o    o o    o o    o      `-^
#                        o     o o    o  oooo  o    o
#
###################################################################################################

orchestrator.plugins.maja_dataset -- shortdesc

orchestrator.plugins.maja_dataset is a description

It defines classes_and_methods


###################################################################################################
"""
from orchestrator.common.maja_exceptions import MajaNotYetImplemented
from orchestrator.common.maja_exceptions import MajaDriverException

L1_READER_KEYS = ["L1TOAImageList", "L1TOACirrusImage", "L2TOAImageList", "IPTOASubOutput",
                  "L2EDGOutputList", "IPEDGSubOutput", "IPSATSubOutput", "L2SATImageList",
                  "L2DFPImageList", "L1PIXImageList", "L2PIXImageList", "L1CLDImageList",
                  "CLAImage", "SOL1Image", "SOL2Image", "DTMVIEImage", "ShadowVIEImage",
                  "ViewingZenithMeanMap", "ViewingAzimuthMeanMap", "AngleZoneMaskList",
                  "ListOfResolution", "Satellite"]


class L1ImageReaderBase(object):

    def __init__(self):
        self.ReadL1Mode = ""
        self._Satellite = ""
        self._ListOfResolution = ""
        self._GIPPL2COMMHandler = None
        self._plugin = None
        self._dem = None
        self.dict_of_vals = {}

    """ Can
        read
        method: return true if the
        key is valid and initialize
        the
        filter """

    def can_read(self, plugin_name):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def read(self, product_info, app_handler, l2comm, dem, pReadL1Mode):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def get_value(self, key):
        if key in self.dict_of_vals:
            return self.dict_of_vals.get(key)
        else:
            raise MajaDriverException("No " + key + " available in the reader.")

    def has_value(self, key):
        return key in self.dict_of_vals

    def get_list_of_resolution(self):
        return self._ListOfResolution

        """ Get
        the
        name
        of
        the
        satellite
        of
        the
        L1
        Product """

    def get_satellite(self):
        return self._Satellite

    def get_plugin(self):
        return self._plugin

    def get_l2comm(self):
        return self._GIPPL2COMMHandler
