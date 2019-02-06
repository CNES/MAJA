# -*- coding: utf-8 -*-

import os
import shutil

import logging
log = logging.getLogger(__name__)

def createDirectory(path):
    """
    @brief Creates a Directory with the given path. Throws are warning if it's already existing and an error
           if a file with the same name already exists
    @param path The full path to the new directory
    """
    if os.path.exists(path) and not os.path.isdir(path):
        raise OSError("Cannot create the output directory because there is a file with the same name. Remove: {0}".format(path))

    else:
        try:
            os.makedirs(path)
        except:
            log.debug("Cannot create directory {0}".format(path))
            return 1
    return 0

def removeFile(filename):
    """
    @brief Removes File of given path
    @param file Path to File
    @return 0 if suceeded, 1 if not
    """
    try:
        os.remove(filename)
    except:
        log.debug("Cannot remove file {0}".format(filename))
        return 1
    return 0

def removeDirectory(directory):
    """
    @brief Removes Directory of given path
    @param file Path to Directory
    @return 0 if suceeded, 1 if not
    """
    try:
        shutil.rmtree(directory)
    except:
        log.debug("Cannot remove directory {0}".format(directory))
        return 1
    return 0
