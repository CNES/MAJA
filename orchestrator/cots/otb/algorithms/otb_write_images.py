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

from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaBusinessException,MajaIOError
import shutil,os
LOGGER = configure_logger(__name__)


def write_images(img_list, img_filenames):
    # TODO : Remove fixed streaming lines

    if len(img_list) != len(img_filenames):
        raise MajaBusinessException("Not the same number of image and filename given to write_images")

    tmp_img_list_clean = []
    tmp_filename_list_clean = []
    for f in range(len(img_list)):
        if type(img_list[f]).__name__ != "str":
            tmp_img_list_clean.append(img_list[f])
            tmp_filename_list_clean.append(img_filenames[f])
        else:
            LOGGER.debug("source : " + img_list[f] + " , dest : " + img_filenames[f])
            try:
                if not os.path.exists(img_filenames[f]) or not os.path.samefile(img_list[f], img_filenames[f]):
                    shutil.copyfile(img_list[f], img_filenames[f])
            except IOError as err:
                raise MajaIOError(err)

    if len(tmp_img_list_clean) == 0:
        return

    parameters = {"il": tmp_img_list_clean,
                  "filenames": tmp_filename_list_clean
                  }

    app = OtbAppHandler("ImageListWriter", parameters)
    del app
