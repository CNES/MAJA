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

orchestrator.cots.gdal.gdal_rasterize -- shortdesc

orchestrator.cots.gdal.gdal_rasterize is a description

It defines classes_and_methods

###################################################################################################
"""


import os
import tempfile
import time

from ..maja_cots import MajaCots
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_utils import get_test_mode

LOGGER = configure_logger(__name__)


def rasterize_det_foo(input_file, working_directory, option_user={}, output_image=None):
    """
    Rasterization for detector footprint gml mask
    """
    options = {"-ot": "Byte",
               "-init": 0,
               "-a_nodata": 0,
               "-at": None,
               "-burn": 1
               }
    options.update(option_user)
    output_image = rasterize(input_file, options, working_directory, output_image)
    return output_image


def rasterize_no_data(input_file, working_directory, option_user={}, output_image=None):
    """
    Rasterize for no data
    """
    options = {"-ot": "Byte",
               "-init": 0,
               "-a_nodata": 0,
               "-at": None,
               "-burn": 1,
               "-sql": 'select fid, maskType, * from MaskFeature',
               "-where": 'maskType="QT_NODATA_PIXELS"' }
    options.update(option_user)
    output_image = rasterize(input_file, options, working_directory, output_image)
    return output_image


def rasterize(input_file, options, working_directory, output_image=None):
    """
    Run the cots rasterization
    :param input_file:
    :param options:
    :param working_directory:
    :param output_image:
    :return:
    """

    if output_image is None:
        sub_working_directory = tempfile.mkdtemp(prefix="GDALRasterize_",
                                                 dir=working_directory)
        output_image = "gdal_rasterize.tif"
    else:
        sub_working_directory = working_directory

    c1 = GdalRasterize(sub_working_directory, output_image)
    c1.pre(input_file, options)

    c1.run()
    c1.post()

    return c1.output_image


class GdalRasterize(MajaCots):

    def __init__(self, working_directory=None, output_image="gdal_rasterize.tif"):
        super(GdalRasterize, self).__init__(working_directory)
        # TODO: TBC
        if working_directory is not None:
            self.output_image = os.path.join(self.working_directory,
                                             output_image)

    def pre(self, input_file, options={}):
        """
        Prepares the command line to be launched in run()
        """
        list_option = []
        for key, value in options.items():
            if key != "":
                list_option.append(key)
            if value is not None:
                list_option.append(value)

        self.command_line = ["gdal_rasterize"] + list_option + [input_file, self.output_image]

    def post(self):
        pass

    def run_rasterize(self, inputfilename, xmin, ymin, xmax, ymax, size_x, size_y, projection,
                      outputfilename, gdaladditionalcommandlineparameters):
        """

        :param inputfilename: string
        :param xmin: int
        :param ymin: int
        :param xmax: int
        :param ymax: int
        :param size_x: int
        :param size_y: int
        :param projection: string
        :param outputfilename: string
        :param gdaladditionalcommandlineparameters: string
        :return:
        """
        # Call the gdal_rasterize command system with the specific parameters
        self.command_line = (
            """gdal_rasterize """ +
            gdaladditionalcommandlineparameters +
            """ -te """ +
            str(xmin) +
            """ """ +
            str(ymin) +
            """ """ +
            str(xmax) +
            """ """ +
            str(ymax) +
            """ -ts """ +
            str(size_x) +
            """ """ +
            str(size_y) +
            """ -a_srs '""" +
            projection +
            """ ' """ +
            inputfilename +
            """ """ +
            outputfilename)
        LOGGER.debug(self.command_line)
        if not get_test_mode():
            self.run()
        return self.status

    # TODO: split ?
    def internal_rasterize_gml_macro(self, pinputfilename, pxmin, pymin, pxmax, pymax, psize_x, psize_y,
                                    pprojection, image_filename, pgdaladditionalcommandlineparameters):
        """

        :param pinputfilename: string
        :param pxmin: int
        :param pymin: int
        :param pxmax: int
        :param pymax: int
        :param psize_x: int
        :param psize_y: int
        :param pprojection: string
        :param image_filename: string
        :param ppostfix_filenamepostfix: string
        :param pgdaladditionalcommandlineparameters: string
        :return:
        """
        pass

        # Read the additionnal parameters
        # TODO: add -of tif in command eline paramters
        l_shortfilename = os.path.basename(image_filename)
        LOGGER.debug("vnsCachingRasterizeGmlMacro the gml file '%s' to the image file name '%s' with the "
                     "following parameters: %s -te %s %s %s %s -ts %s %s.",
                     pinputfilename, l_shortfilename, pgdaladditionalcommandlineparameters, pxmin, pymin, pxmax, pymax,
                     psize_x, psize_y)

        start_time = time.time()
        # Call program
        result = self.run_rasterize(pinputfilename, pxmin, pymin, pxmax, pymax, psize_x, psize_y, pprojection,
                                    image_filename, pgdaladditionalcommandlineparameters)
        stop_time = time.time()
        duration = stop_time - start_time

        LOGGER.debug(" => Rasterize the file name %s run in %s.", l_shortfilename, duration)

        return image_filename
