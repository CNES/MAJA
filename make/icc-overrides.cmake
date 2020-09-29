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
# CMake propose quatre modes de compilation par défaut, dont la sélection se fait grace au paramètre CMake "CMAKE_BUILD_TYPE".
# Les modes sont:
#  1. "Debug"           : mode pour une compilation sans optimisation, pour un mode de debug
#  2. "MinSizeRel"      : mode pour compiler des binaires les plus petits en taille
#  3. "Release"         : mode pour compiler le code en optimisé
#  4. "RelWithDeInfo"   : mode pour activer le debug mais en compilant le code legerement optimisé
#
# Dans le script de compilation de MACCS (launch-generation-maccs-package) lorsque l'option --release est activée, c'est le mode "Release" de CMake 
# qui est utilisé, utilisant alors les parametres de définition des FLAGS ci-dessous nommés CMAKE_C_FLAGS_RELEASE_INIT et CMAKE_CXX_FLAGS_RELEASE_INIT. 
# Dans ce cas, ce sont ces paramètres qu'il faut modifier pour changer les options de compilation.
#
# Si le mode "Debug" est activé, c'est le mode "RelWithDeInfo" de CMake qui est utilisé,  utilisant alors les parametres de définition des FLAGS ci-dessous 
# nommés CMAKE_C_FLAGS_RELWITHDEBINFO_INIT et CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT. Dans ce cas, ce sont ces paramètres qu'il faut modifier pour changer les options de compilation.
#
# Les flags communs a tous les modes de compilations sont CMAKE_C_FLAGS_INIT et CMAKE_CXX_FLAGS_INIT.

#set(CMAKE_C_FLAGS_INIT                  "-Wall -Wextra -pthread -std=c99")
set(CMAKE_C_FLAGS_INIT                  "-Wall -Wextra -pthread -Wall -Wunused  -Wunused-function  -Wunused-parameter -Wunused-variable -Wsign-compare -Wparentheses -Wunknown-pragmas -Wswitch -Wno-deprecated")
set(CMAKE_C_FLAGS_DEBUG_INIT            "-g")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT       "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE_INIT          "-xHost -ip -O3 -no-prec-div -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT   "-O2 -g")

set(CMAKE_CXX_FLAGS_INIT                "-Wall -Wextra -pthread -Wall -Wunused  -Wunused-function -Wunused-parameter -Wunused-variable -Wsign-compare -Wparentheses -Wunknown-pragmas -Wswitch -Wno-deprecated")
set(CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT        "-xHost -ip -O3 -no-prec-div -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

# "-Wl,--no-as-needed": temporary fix related to bug of GCC 4.8 on Ubuntu 13.10:
# https://bugs.launchpad.net/ubuntu/+source/gcc-defaults/+bug/1228201
set(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,--no-as-needed")
