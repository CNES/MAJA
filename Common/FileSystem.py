#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES, CS-SI, CESBIO - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>,
Project:        Start-MAJA, CNES
"""

from __future__ import print_function
import os
import shutil
import logging

log = logging.getLogger(__name__)


def create_directory(path):
    """
    Creates a Directory with the given path. Throws a warning if it's already existing and an error if
    a file with the same name already exists.
    :param path: The full path to the new directory
    """
    if os.path.exists(path):
        if not os.path.isdir(path):
            raise IOError("Cannot create the output directory. There is a file with the same name: %s" % path)
        else:
            logging.debug("Directory already existing: %s" % path)
    else:
        try:
            os.makedirs(path)
        except OSError:
            raise IOError("Cannot create directory %s" % path)
    return 0


def remove_file(filename):
    """
    Removes File of given path
    :param filename: Path to File
    """
    try:
        os.remove(filename)
    except OSError:
        log.debug("Cannot remove file %s" % filename)
    return


def remove_directory(directory):
    """
    Removes Directory of given path
    :param directory: Path to Directory
    """
    try:
        shutil.rmtree(directory)
    except OSError:
        log.debug("Cannot remove directory {0}".format(directory))


def __get_item(path, reg):
    """
    Find a specific file/folder within a directory
    :param path: The full path to the directory
    :param reg: The regex to be searched for
    :return: The full path to the file/folder
    """
    import re
    import os
    available_dirs = [f for f in os.listdir(path) if re.search(reg.lower(), f.lower())]
    if not available_dirs:
        raise IOError("Cannot find %s in %s" % (reg, path))
    return os.path.abspath(os.path.join(path, available_dirs[0]))


def find(pattern, path, case_sensitive=False):
    """
    Find a file or dir in a directory-tree.
    :param pattern: The filename to be searched for
    :param path: The path to the root directory
    :param case_sensitive: Do a case sensitive comparison.
    :return: The file/directory if found. AssertionError if not.
    """
    import re
    result = []
    parameter = pattern.replace("*", ".*")
    if not case_sensitive:
        parameter = parameter.lower()
    for root, dirs, files in os.walk(path):
        for name in files + dirs:
            if re.search(parameter.lower(), name if case_sensitive else name.lower()):
                result.append(os.path.join(root, name))
    if not result:
        raise ValueError("Cannot find %s in %s" % (parameter, path))
    return result


def find_single(pattern, path, case_sensitive=False):
    """
    Find a file or dir in a directory-tree.
    :param pattern: The filename to be searched for
    :param path: The path to the root directory
    :param case_sensitive: Do a case sensitive comparison.
    :return: The file/directory if found. AssertionError if not.
    """
    return find(pattern, path, case_sensitive)[0]


def get_file(**kwargs):
    """
    Get a single file from inside the root directory by glob or regex.
    The necessary arguments:
    - root: The root folder to start the search from.
    The file can have one or multiple of the following characteristics:
    - folders: Inside a (sub-)folder
    - filename: Filename with specific pattern
    :param kwargs: The folders and filename arguments
    :return: The full path to the file if found or OSError if not.
    """
    import os

    search_folder = kwargs["root"]
    # The function supports globbing, so replace the globs for regex-like ones
    folders = kwargs.get("folders", ".")
    parameter = os.path.normpath(folders).replace("*", ".*")
    subdirs = parameter.split(os.sep)
    # Recursively update the search folder for each sub folder
    for sub in subdirs:
        if sub == ".":
            continue
        if sub == "..":
            search_folder = os.path.dirname(search_folder)
            continue
        search_folder = __get_item(search_folder, sub)
    # Now that we are in the right directory, search for the file:
    try:
        filename = kwargs["filename"]
    except KeyError:
        return search_folder
    parameter = os.path.normpath(filename).replace("*", ".*")
    return __get_item(search_folder, parameter)


def symlink(src, dst):
    """
    Create symlink from src to dst and raise Exception if it didnt work
    :param src: The full path to the source file or folder
    :param dst: The destination folder
    :return: None. OSError if symlink cannot be created.
    """
    if os.path.exists(dst) and os.path.islink(dst):
        logging.debug("File already existing: %s" % dst)
        return

    try:
        os.symlink(src, dst)
    except OSError:
        raise OSError("Cannot create symlink for %s at %s."
                      "Does your plaform support symlinks?" % (src, dst))


def __get_return_code(proc, log_level):
    """
    Read the stdout of a subprocess while also processing its return code if available
    :param proc: The subprocess
    :param log_level: The log level for the messages displayed.
    :return: The return code of the app
    """
    while proc.poll() is None:
        line = proc.stdout.readline()  # This blocks until it receives a newline.
        if log_level == logging.DEBUG:
            print(line.decode('utf-8'), end="")
    proc.stdout.close()
    return proc.wait()


def run_external_app(name, args, log_level=logging.DEBUG):
    """
    Run an external application using the subprocess module
    :param name: the Name of the application
    :param args: The list of arguments to run the app with
    :param log_level: The log level for the messages displayed.
    :return: The return code of the App
    """
    from timeit import default_timer as timer
    import subprocess
    full_args = [name] + args
    cmd = " ".join(str(a) for a in full_args)
    # Bug in conda: Windows path prepended in linux versions
    env = os.environ.copy()
    if os.name != "nt" and ";" in env["PATH"]:
        env["PATH"] = env["PATH"].split(";")[1]
    log.log(log_level, "Executing cmd: " + cmd)
    start = timer()
    try:
        with subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env) as proc:
            return_code = __get_return_code(proc, log_level=log_level)
    except AttributeError:
        # For Python 2.7, popen has no context manager:
        proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env)
        return_code = __get_return_code(proc, log_level=log_level)
    if return_code:
        raise subprocess.CalledProcessError(return_code, cmd)
    end = timer()
    # Show total execution time for the App:
    log.log(log_level, "{0} took {1:.2f}s".format(os.path.basename(name), end - start))
    return return_code


def download_file(url, filepath, log_level=logging.DEBUG):
    """
    Download a single file using wget
    :param filepath: The file name to be written to
    :param url: The url to download
    :param log_level: The log level for the messages displayed.
    :return:
    """
    import tempfile
    import shutil
    tmp_file = tempfile.mktemp()
    args = ["--retry-connrefused", "--waitretry=1",
            "--read-timeout=20", "--timeout=15",
            "-O", tmp_file, url]
    if log_level != logging.DEBUG:
        args.append("-nv")
    ret_code = run_external_app("wget", args, log_level=log_level)
    shutil.move(tmp_file, filepath)
    return ret_code


def unzip(archive, dest):
    """
    Unzip a file to the given destination
    :param archive: The archive name
    :param dest: The destination dir
    :return:
    """
    # TODO Unittest
    import zipfile
    assert os.path.isfile(archive)
    with zipfile.ZipFile(archive, 'r') as zip_ref:
        zip_ref.extractall(dest)


def find_in_file(filename, pattern):
    """
    Find a pattern inside a file
    :param filename: The filename
    :param pattern: The pattern to be searched for
    :return: The pattern (if found). None if not.
    """
    # TODO Unittest
    import re
    assert os.path.isfile(filename)
    with open(filename, "r") as f:
        content = "".join(f.read().splitlines())
        lut_url = re.search(pattern, content)
    if lut_url:
        return lut_url.group()
    return None
