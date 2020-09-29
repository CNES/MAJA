#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (C) 2016-2020 Centre National d'Etudes Spatiales (CNES), CSSI, CESBIO  All Rights Reserved

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

from __future__ import print_function
import os
import shutil
import logging


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
        logger.debug("Cannot remove file %s" % filename)
    return


def remove_directory(directory):
    """
    Removes Directory of given path
    :param directory: Path to Directory
    """
    try:
        shutil.rmtree(directory)
    except OSError:
        logger.debug("Cannot remove directory {0}".format(directory))


def find(pattern, path, case_sensitive=False, depth=None, ftype="all"):
    """
    Find a file or dir in a directory-tree of given depth.

    :param pattern: The filename to be searched for
    :param path: The path to the root directory
    :param case_sensitive: Do a case sensitive comparison. Default is False.
    :param depth: Search only up to a specified depth. Default is None, signifying a maximum limit of 20.
    :param ftype: Can be "file", "folder" or "all".
    :return: The file/directory if found. AssertionError if not.
    """
    import re
    result = []

    reg_to_find = pattern.replace("*", ".*")

    if not case_sensitive:
        reg_to_find = reg_to_find.lower()

    path = os.path.abspath(path)
    if not depth:
        depth = 20  # Limit depth in case it is not specified.
    for root, dirs, files in os.walk(path):
        if root[len(path):].count(os.sep) < depth:
            if ftype == "all":
                names = files + dirs
            elif ftype == "file":
                names = files
            elif ftype == "folder":
                names = dirs
            else:
                raise ValueError("Unknown type %s" % ftype)
            for name in names:
                if re.search(reg_to_find, name if case_sensitive else name.lower()):
                    result.append(os.path.join(root, name))
    if not result:
        raise ValueError("Cannot find %s in %s" % (pattern, path))
    return result


def find_single(pattern, path, case_sensitive=False, depth=None, ftype="all"):
    """
    Find a single file or dir in a directory-tree.

    :param pattern: The filename to be searched for
    :param path: The path to the root directory
    :param case_sensitive: Do a case sensitive comparison.
    :param depth: Search only up to a specified depth. Default is None, signifying a maximum limit of 20.
    :param ftype: Can be "file", "folder" or "all".
    :return: The file/directory if found. ValueError if not.
    """
    return find(pattern, path, case_sensitive=case_sensitive, depth=depth, ftype=ftype)[0]


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
        logger.error("Cannot create symlink for %s at %s."
                     "Does your plaform support symlinks?" % (src, dst))
        shutil.copy(src, dst)


def __get_return_code(proc, log_level):
    """
    Read the stdout of a subprocess while also processing its return code if available
    :param proc: The subprocess
    :param log_level: The log level for the messages displayed.
    :return: The return code of the app
    """
    full_log = []
    while proc.poll() is None:
        line = proc.stdout.readline()  # This blocks until it receives a newline.
        if log_level == logging.DEBUG:
            log_line = line.decode('utf-8')
            print(log_line, end="")
            full_log.append(log_line.rstrip())
    proc.stdout.close()
    return proc.wait(), full_log


def run_external_app(name, args, log_level=logging.DEBUG, logfile=None, skip_error=False):
    """
    Run an external application using the subprocess module

    :param name: the Name of the application
    :param args: The list of arguments to run the app with
    :param log_level: The log level for the messages displayed.
    :param logfile: Save all logs of the subprocess to this file.
    :param skip_error: Do not raise error if command fails. Default is False.
    :return: The return code of the App with logfile written to disk if desired.
    """
    from timeit import default_timer as timer
    import subprocess
    full_args = [name] + args
    cmd = " ".join(str(a) for a in full_args)
    # Bug in conda: Windows path prepended in linux versions
    env = os.environ.copy()
    if os.name != "nt" and ";" in env["PATH"]:
        env["PATH"] = env["PATH"].split(";")[1]
    logger.debug("Executing cmd: %s" % cmd)
    start = timer()
    try:
        with subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env) as proc:
            return_code, full_log = __get_return_code(proc, log_level=log_level)
    except AttributeError:
        # For Python 2.7, popen has no context manager:
        proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env)
        return_code, full_log = __get_return_code(proc, log_level=log_level)
    if return_code and not skip_error:
        raise subprocess.CalledProcessError(return_code, cmd)
    end = timer()
    if logfile and not os.path.isdir(logfile):
        # Filter empty lines:
        filter_empty_log = list(filter(None, full_log))
        # Add command and total runtime to log:
        full_log = ["Running command", cmd] + filter_empty_log +\
                   ["{0} took {1:.2f}s".format(os.path.basename(name), end - start)]
        # Write to file
        with open(logfile, 'w') as f:
            for item in full_log:
                f.write("%s\n" % item)
    # Show total execution time for the App:
    logger.debug("{0} took {1:.2f}s".format(os.path.basename(name), end - start))
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


if __name__ == "__main__":
    pass
else:
    logger = logging.getLogger("root")
