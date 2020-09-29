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
#                                        __  __    __     ____   __                                        #
#                                       (  \/  )  /__\   (_  _) /__\                                       #
#                                        )    (  /(__)\ .-_)(  /(__)\                                      #
#                                       (_/\/\_)(__)(__)\____)(__)(__)                                     #
#                                                                                                          #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Modification pour la prise en compte de la DM#
#                                                            du MACCS generique (PM LAIG-NT-MAC-0239-CNES) #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(Dart)
set(CTEST_PROJECT_NAME "MAJA-NG")
set(CTEST_NIGHTLY_START_TIME "20:00:00 CEST")

#set(CTEST_NIGHTLY_IDENT_LOCATION "http://www.orfeo-toolbox.org/nightly/MonteverdiNightlyNumber")

set(CTEST_DROP_METHOD "http")
#set(CTEST_DROP_SITE "ipf.si.c-s.fr/cdash")
# @IP srv IPF: 172.26.46.217
#set(CTEST_DROP_SITE "srv-ipf1.cloud-espace.si.c-s.fr/cdash")
set(CTEST_DROP_SITE "172.26.46.217/cdash")
set(CTEST_DROP_LOCATION "/submit.php?project=MAJA-NG")
set(CTEST_DROP_SITE_CDASH TRUE)
