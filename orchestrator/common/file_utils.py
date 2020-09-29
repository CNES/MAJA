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

from orchestrator.common.maja_exceptions import MajaProcessingError
from orchestrator.common.maja_utils import get_test_mode
from orchestrator.common.system_utils import launch_command
import glob
import os
import errno
import shutil

from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaDataException

LOGGER = configure_logger(__name__)


def maja_glob_one(search_directory, search_pattern):
    """
    Search in search directory the file with search_pattern and return it.
    If several files are found, a warning is printed and the first element is returned

    TODO: use logging

    :param search_directory:
    :type search_directory:
    :param search_pattern:
    :type search_pattern:
    """
    list_glob_result = glob.glob(os.path.join(search_directory, search_pattern))
    if len(list_glob_result) == 1:
        return list_glob_result[0]
    elif len(list_glob_result) == 0:
        return None
    else:
        LOGGER.warning("More than one result found. Taking the first")
        return list_glob_result[0]


def maja_get_subdirectories(search_directory):
    """
    Search in search directory all the subdirs

    TODO: use logging

    :param search_directory:
    :type search_directory:
     """

    return [os.path.join(search_directory, name) for name in os.listdir(search_directory)
            if os.path.isdir(os.path.join(search_directory, name))]


# Resolve a path with env var
def fully_resolve(a_path, check_existence=False):
    resolved = os.path.expanduser(os.path.expandvars(os.path.normpath(a_path)))
    if "$" in resolved:
        raise MajaProcessingError("Environment variable not resolved in %s" % resolved)
    if check_existence:
        if not os.path.exists(resolved):
            raise MajaDataException("File not found %s" % resolved)
    return resolved


# Copy a file to the directory
def copy_file_to_directory(sourcefilename, destinationdir, notestmode=False):
    destinationfilename = os.path.join(destinationdir, os.path.basename(sourcefilename))
    if not get_test_mode() or notestmode:
        shutil.copyfile(sourcefilename, destinationfilename)
    else:
        with open(destinationfilename, 'a'):
            os.utime(destinationfilename, None)
    return destinationfilename


def copy_file(source, dest, notestmode=False):
    if not get_test_mode() or notestmode:
        shutil.copyfile(source, dest)
    else:
        with open(dest, 'a'):
            os.utime(dest, None)


def copy_tree(src, dst):
    files = os.listdir(src)
    if not os.path.isdir(dst):
        os.makedirs(dst)

    for f in files:
        src_filename = os.path.join(src, f)
        dst_filename = os.path.join(dst, f)
        if os.path.isdir(src_filename):
            copy_tree(src_filename, dst_filename)
        else:
            copy_file(src_filename, dst_filename)


def touch_file(filename):
    with open(filename, 'a'):
        os.utime(filename, None)


def remove_directory(dir_name):
    if os.path.exists(dir_name) and os.path.isdir(dir_name):
        try:
            os.rmdir(dir_name)
        except OSError:
            LOGGER.warn("Could not delete " + dir_name)


def remove_file(filepath):
    if os.path.exists(filepath) and os.path.isfile(filepath):
        try:
            os.remove(filepath)
        except OSError:
            LOGGER.warn("Could not delete " + filepath)


def uncompress_file(src, dst):
    command_line = "tar -xjf " + src + " -C " + dst

    LOGGER.info("Uncompress %s", command_line)

    # TODO: TBC use append env or manage entire env for each COTS ?
    status = launch_command(command_line)

    # TODO: TBC MOve status to post ?
    # TODO: see status management by system command executor
    if status != 0:
        raise MajaDataException("Error running {}. Exit code {}".format(command_line, status))


def create_directory(dir_name):
    try:
        LOGGER.debug("Creating directory: " + dir_name)
        os.makedirs(dir_name)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise MajaProcessingError("Error while creatind directory : "+dir_name)


def provide_filename(directory, basefilename, extentionfilename):
    """
    :param directory:
    :param basefilename:
    :param extentionfilename:
    :return:
    """
    # Set the number max of filename
    nbmaxfilenames = 10000
    cptfilenames = 0
    provide = False
    filename = ""
    # Loops and find a valid filename
    while not provide and cptfilenames < nbmaxfilenames:
        filename = os.path.join(directory, "{}{}.{}".format(basefilename, cptfilenames, extentionfilename))
        # Checks if the filename already exists
        if not os.path.exists(filename):
            provide = True
        cptfilenames += 1
    # 'nbMaxFilenames' file already exists.
    # Error: user must be clear it working dir
    if not provide:
        raise MajaDataException("Utilities::ProvideFilename: Impossible to find a available filename in "
                                "the {} caching directory ! The maximum number of filename is {}. Clean the "
                                "directory.".format(directory, nbmaxfilenames))
    return filename


def provide_caching_filename(directory, basefilename="", extentionfilename=""):
    """

    :param directory:
    :param basefilename:
    :param extentionfilename:
    :return:
    """
    # ---------------------------------------------------------------------------------------------
    # Returns a valid caching filename
    return provide_filename(directory, "vns_caching_" + basefilename, extentionfilename)


def get_extended_filename_write_image_file_standard():
    return ""


def get_extended_filename_write_mask_file_muscate():
    return ""


def get_extended_filename_write_image_file_jpeg():
    return ""



def convert_to_unix_slash(path):
    path = path.replace("\\", "/")
    path = path.replace("//", "/")
    return path
