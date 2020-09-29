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
orchestrator.common.file_utils -- shortdesc

orchestrator.common.file_utils is a description

It defines classes_and_methods

###################################################################################################
"""

from orchestrator.common.maja_utils import get_test_mode
import os
import shutil
from orchestrator.cots.otb.algorithms.otb_write_images import write_images
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaIOError

LOGGER = configure_logger(__name__)


# Copy a file to the directory
def otb_copy_file_to_directory(sourceFilename, destinationDir):
    destinationFilename = os.path.join(destinationDir, os.path.basename(sourceFilename))
    if not get_test_mode():
        if type(sourceFilename).__name__ == "SwigPyObject":
            write_images([sourceFilename], [destinationFilename])
        else:
            shutil.copyfile(sourceFilename, destinationFilename)
    else:
        with open(destinationFilename, 'a'):
            os.utime(destinationFilename, None)
    return destinationFilename


def otb_copy_image_to_file(source, dest, raise_exceptions=True):
    if not get_test_mode():
        if type(source).__name__ == "SwigPyObject":
            write_images([source], [dest])
        else:
            LOGGER.debug("source : " + source + " , dest : " + dest)
            try:
                if not os.path.exists(dest) or not os.path.samefile(source, dest):
                    shutil.copyfile(source, dest)
            except IOError as err:
                if raise_exceptions:
                    raise MajaIOError(err)
                else:
                    LOGGER.warn("Copy failed !!!")

    else:
        with open(dest, 'a'):
            os.utime(dest, None)


def otb_is_swig_pointer(im):
    return type(im).__name__ == "SwigPyObject"
