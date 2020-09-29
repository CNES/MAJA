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
orchestrator.common.maja_exceptions -- shortdesc

orchestrator.common.maja_exceptions is a description

It defines classes_and_methods


###################################################################################################
"""


class MajaException(Exception):

    def __init__(self, msg):
        super(MajaException, self).__init__(msg)


class MajaDriverException(MajaException):
    """
    Exception raised by an error reading a product with maja drivers
    """
    pass


class MajaDataException(MajaException):
    """
    Exception raised when there is a problem to read or write a file
    """
    pass


class MajaModuleException(MajaException):
    """
    Exception raised when there is a problem to read or write a file
    """
    pass


class MajaNotYetImplemented(NotImplementedError):
    """
    Exception raised when entering a not yet implemented functionality
    """
    pass


class MajaProcessingError(MajaException):
    """
    Exception raised when process crashed during run
    """
    pass


class MajaIOError(IOError):
    """
    Exception raised when there is a problem to read or write a file
    """
    pass


class MajaExceptionAlgorithm(ArithmeticError):
    """
    Exception raised when there is a problem to read or write a file
    """
    pass



class MajaExceptionPluginBase(MajaException):
    """

    """
    pass


class MajaExceptionPluginSentinel2(MajaExceptionPluginBase):
    """

    """
    pass


class MajaExceptionPluginSentinel2Muscate(MajaExceptionPluginBase):
    """

    """
    pass


class MajaExceptionPluginVenus(MajaExceptionPluginBase):
    """

    """
    pass



class MajaExceptionPluginMuscate(MajaExceptionPluginBase):
    """

    """
    pass


class MajaBusinessException(MajaException):
    """

    """
    pass


class MajaChainException(MajaException):
    """

    """
    pass


class MajaOgrException(MajaException):
    """

    """
    pass
