#!/bin/bash 
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
#
############################################################################################################
#                                                                                                          #
#                                         __  __    __    ___  ___  ___                                    #
#                                        (  \/  )  /__\  / __)/ __)/ __)                                   #
#                                         )    (  /(__)\( (__( (__ \__ \                                   #
#                                        (_/\/\_)(__)(__)\___)\___)(___/                                   #
#                                                                                                          #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 4-2-0 : DM : LAIG-DM-MAC-1129-CNES : 04 mars 2015 : Creation pour compilation icc              #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################

#-------------------------------------------------------------
# Name of the builder (used to print compiler in launcher-generation-maccs-cots-package.sh)
export COMPILER_NAME="gcc"

#-------------------------------------------------------------
# Set compiler executables
export CC=$(which gcc)
export CXX=$(which g++)
