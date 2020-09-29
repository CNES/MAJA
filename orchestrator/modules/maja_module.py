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

orchestrator.modules.maja_module -- shortdesc

orchestrator.modules.maja_module is a description

It defines classes_and_methods

###################################################################################################
"""

from orchestrator.common.maja_exceptions import *
from orchestrator.plugins.common.factory.factory_base import FactoryBase


class MajaModule(object):
    """
    classdocs
    """

    def __init__(self):
        """
        Constructor
        """
        self.in_keys_to_check = list()
        self.out_keys_to_check = list()
        self.out_keys_provided = list()

    def __del__(self):
        """
        Cleanup at destuction
        """
        self.cleanup()


    def launch(self, dict_of_input, dict_of_output):
        if self._check_conditions(dict_of_input, dict_of_output):
            self._check_requirements(dict_of_input, dict_of_output)
            self.run(dict_of_input, dict_of_output)
            self.post(dict_of_input, dict_of_output)
            self._check_post_conditions(dict_of_input, dict_of_output)

    def _check_requirements(self, dict_of_input, dict_of_output):
        # check input keys to verify
        for key in self.in_keys_to_check:
            if not MajaModule._check_presence_in_dict(dict_of_input, key):
                raise MajaModuleException("Key : "+key+" is not available in input dict for module : " + type(self).NAME)
        # check output dict keys to verify
        for key in self.out_keys_to_check:
            if not MajaModule._check_presence_in_dict(dict_of_output, key):
                raise MajaModuleException("Key : " + key + " is not available in output dict for module : " + type(self).NAME)
        return True

    def _check_conditions(self, dict_of_input, dict_of_output):
        # Default behaviour will be not to have any conditions
        return True

    def _check_post_conditions(self, dict_of_input, dict_of_output):
        for key in self.out_keys_provided:
            if not MajaModule._check_presence_in_dict(dict_of_output, key):
                raise MajaModuleException("Key : " + key + " is not provided in output dict by module : " + type(self).NAME)
        return True

    def run(self, dict_of_input, dict_of_output):
        """
        Check the requirements on input and input dict
        :param dict_of_input:
        :param dict_of_output:
        :return:
        """
        return

    def post(self, dict_of_input, dict_of_output):
        """
        check that the module correctly gave the output
        :param dict_of_input:
        :param dict_of_output:
        :return:
        """
        return

    def cleanup(self):
        """
        Cleanup the object at destruction
        :return:
        """
        return

    @staticmethod
    def create(module_name):
        creators = FactoryBase.get_registered(module_name)
        if len(creators) == 0:
            raise MajaModuleException("No factory to create module : " + module_name)

        if len(creators) > 1:
            raise MajaModuleException("More than one factory to provide module : " + module_name)
        return creators[0]()

    @staticmethod
    def _check_presence_in_dict(dict_to_check, key):
        l_splitted_key = str(key).split(".")
        if len(l_splitted_key) > 1:
            if l_splitted_key[0] in dict_to_check:
                return MajaModule._check_presence_in_dict(dict_to_check[l_splitted_key[0]], key[key.find(".")+1:])
            else:
                return False
        else:
            return key in dict_to_check
