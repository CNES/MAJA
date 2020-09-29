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
orchestrator.common.logger.maja_logging -- shortdesc

orchestrator.common.logger.maja_logging is a description

It defines classes_and_methods

###################################################################################################
"""

import logging.config
import logging
import sys
from orchestrator.common import version


def addLoggingLevel(levelName, levelNum, methodName=None):
    """
    Comprehensively adds a new logging level to the `logging` module and the
    currently configured logging class.

    `levelName` becomes an attribute of the `logging` module with the value
    `levelNum`. `methodName` becomes a convenience method for both `logging`
    itself and the class returned by `logging.getLoggerClass()` (usually just
    `logging.Logger`). If `methodName` is not specified, `levelName.lower()` is
    used.

    To avoid accidental clobberings of existing attributes, this method will
    raise an `AttributeError` if the level name is already an attribute of the
    `logging` module or if the method name is already present

    Example
    -------
    >>> addLoggingLevel('TRACE', logging.DEBUG - 5)
    >>> logging.getLogger(__name__).setLevel("TRACE")
    >>> logging.getLogger(__name__).trace('that worked')
    >>> logging.trace('so did this')
    >>> logging.TRACE
    5

    """
    if not methodName:
        methodName = levelName.lower()

    if hasattr(logging, levelName):
       raise AttributeError('{} already defined in logging module'.format(levelName))
    if hasattr(logging, methodName):
       raise AttributeError('{} already defined in logging module'.format(methodName))
    if hasattr(logging.getLoggerClass(), methodName):
       raise AttributeError('{} already defined in logger class'.format(methodName))

    # This method was inspired by the answers to Stack Overflow post
    # http://stackoverflow.com/q/2183233/2988730, especially
    # http://stackoverflow.com/a/13638084/2988730
    def logForLevel(self, message, *args, **kwargs):
        if self.isEnabledFor(levelNum):
            self._log(levelNum, message, args, **kwargs)
    def logToRoot(message, *args, **kwargs):
        logging.log(levelNum, message, *args, **kwargs)

    logging.addLevelName(levelNum, levelName)
    setattr(logging, levelName, levelNum)
    setattr(logging.getLoggerClass(), methodName, logForLevel)
    setattr(logging, methodName, logToRoot)


def configure_logger(name, std=sys.stdout, level=logging.DEBUG):
    """
    """
    if not hasattr(logging, 'PROGRESS'):
        addLoggingLevel('PROGRESS', logging.INFO + 5)

    log = logging.getLogger('MAJA')
    if len(log.handlers) == 0:
        log.setLevel(level)
        handler = logging.StreamHandler(std)
        handler.setLevel(level)
        handler.setFormatter(
            logging.Formatter('%(asctime)s [%(process)d] %(levelname)s %(name)s ' + version.MAJA_VERSION_LOGGER_STRING +
                              ' %(funcName)s:%(lineno)d %(message)s',"%Y-%m-%d %H:%M:%S"))
        log.addHandler(handler)
    """
    Stdout and err redirection
    """
    """stdout_logger = logging.getLogger('STDOUT')
    stream_logger_out = StreamToLogger(stdout_logger, log_level=logging.INFO)
    sys.stdout = stream_logger_out
    stderr_logger = logging.getLogger('STDERR')
    stream_logger_err = StreamToLogger(stderr_logger, log_level=logging.ERROR)
    sys.stderr = stream_logger_err"""
    return log


class StreamToLogger(object):
    """Fake file-like stream object that redirects writes to a logger instance.

    Code from
    `electricmonk <http://www.electricmonk.nl/log/2011/08/14/redirect-\
    stdout-and-stderr-to-a-logger-in-python/>`_
    """

    def __init__(self, logger, log_level=logging.INFO):
        self.logger = logger
        self.log_level = log_level
        self.linebuf = ''

    def write(self, buf):
        """
        :param buf: The buffer to write
        :type buf: [str]
        """
        for line in buf.splitlines():
            line_stripped = line.rstrip()
            if len(line_stripped) > 0:  # Log only non empty lines
                self.logger.log(self.log_level, line_stripped.decode('UTF-8'))
