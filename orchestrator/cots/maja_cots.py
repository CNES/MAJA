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

orchestrator.cots.maja_cots -- shortdesc

orchestrator.cots.maja_cots is a description

It defines classes_and_methods

###################################################################################################
"""


from orchestrator.common.system_utils import launch_command
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaProcessingError

LOGGER = configure_logger(__name__)


class MajaCots(object):
    """
    classdocs
    """

    def __init__(self, working_directory):
        """
        Constructor
        """
        self.working_directory = working_directory
        self.command_line = []
        self.env = {}
        self.status = 0
        self.output_std = ""
        self.output_err = ""

    def run(self):
        """
        Run self.command_line
        """
        LOGGER.info("Running %s", self.command_line)

        # TODO: TBC use append env or manage entire env for each COTS ?
        self.status = launch_command(self.command_line)

        # TODO: TBC MOve status to post ?
        # TODO: see status management by system command executor
        if self.status != 0:
            raise MajaProcessingError("Error running {}. Exit code {}.".format(self.command_line, self.status))
