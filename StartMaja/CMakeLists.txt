#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES), CS-SI, CESBIO - All Rights Reserved
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
############################################################################################################
#                                                                                                          #
#                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         #
#                             o                                                                            #
#                          o                                                                               #
#                        o                                                                                 #
#                      o                                                                                   #
#                     o       ooooooo       ooooooo    o         o      oo                                 #
#    o              o       o        o     o       o   o         o     o   o                               #
#      o           o       o          o   o         o  o         o    o      o                             #
#        o        o       o           o   o         o  o         o    o        o                           #
#         o      o        o      oooo     o         o  o         o   o           o                         #
#          o    o          o              o         o  o         o   o           o                         #
#           o  o            o             o         o  o o      o   o          o                           #
#            oo              oooooooo    o         o   o  oooooo   o      oooo                             #
#                                                     o                                                    #
#                                                     o                                                    #
#                                                    o                            o                        #
#                                                    o            o      oooo     o   o      oooo          #
#                                                   o             o         o    o    o         o          #
#                                                   o            o       ooo     o   o       ooo           #
#                                                               o       o       o   o          o           #
#                                                               ooooo   oooo    o   ooooo  oooo            #
#                                                                              o                           #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################                                                                                             #
#                                                                                                          #
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2572-CNES : 22 mars 2018 : Nouveau Plugin SENTINEL2_TM               #
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-1483-CNES : 24 mai 2016 : Organisation des schémas                    #
# VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte        #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                       #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id: CMakeLists.txt 4032 2012-04-12 08:41:39Z tfeuvrie $                                                 #
#                                                                                                          #
############################################################################################################


SET(processing "python3.7 $MAJA_INSTALL_DIR/lib/python/StartMaja/Start_maja.py")
CONFIGURE_FILE(${MAJA_SOURCE_DIR}/Code/Core/script.sh.in
  ${MAJA_BINARY_DIR}/bin/startmaja @ONLY)
INSTALL(PROGRAMS ${MAJA_BINARY_DIR}/bin/startmaja DESTINATION ${MAJA_INSTALL_BIN_DIR})


SET(processing "python3.7 $MAJA_INSTALL_DIR/lib/python/StartMaja/prepare_mnt/DTMCreation.py")
CONFIGURE_FILE(${MAJA_SOURCE_DIR}/Code/Core/script.sh.in
  ${MAJA_BINARY_DIR}/bin/dtmcreation @ONLY)
INSTALL(PROGRAMS ${MAJA_BINARY_DIR}/bin/dtmcreation DESTINATION ${MAJA_INSTALL_BIN_DIR})


SET(processing "python3.7 $MAJA_INSTALL_DIR/lib/python/StartMaja/cams_download/download_CAMS_daily.py")
CONFIGURE_FILE(${MAJA_SOURCE_DIR}/Code/Core/script.sh.in
  ${MAJA_BINARY_DIR}/bin/camsdownload @ONLY)
INSTALL(PROGRAMS ${MAJA_BINARY_DIR}/bin/camsdownload DESTINATION ${MAJA_INSTALL_BIN_DIR})




