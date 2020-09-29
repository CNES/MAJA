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
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 12 septembre 2010 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

# Install in the BINARIES package (no in DEVEL package) 
IF(NOT MAJA_INSTALL_NO_LIBRARIES)

    #--------------------------------------------------------------------------------
    # Fichier a ne jamais modifier, meme par Admin MACCS dans 
    # /share/config
    #--------------------------------------------------------------------------------
    
    #--------------------------------------------------------------------------------
    # Fichier modifiable admim dans 
    # /etc/config/admin
    #--------------------------------------------------------------------------------
    # Install the MAJAAdminConfigSystem.xml generated by cmake 
    file(COPY
      ${CMAKE_CURRENT_SOURCE_DIR}/MAJAAdminConfigSystem.xml
      DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_CONF_ADMIN_DIR} )

    #--------------------------------------------------------------------------------
    # Fichier modifiable user dans 
    # /etc/config/user
    #--------------------------------------------------------------------------------

    # Install MAJAUserConfigSystem.xml in CONFIG USER DIR
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/MAJAUserConfigSystem.xml 
            DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_CONF_USER_DIR} )

    #--------------------------------------------------------------------------------
    # Install Templates dir dans admin:
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Templates 
      DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_ADMIN_CONFIG_DIR})
    #            DESTINATION ${MAJA_INSTALL_ADMIN_CONFIG_DIR}/Interfaces/Config/Common

    #--------------------------------------------------------------------------------
    # Install Fonts dir dans admin:
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Fonts 
      DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_ADMIN_CONFIG_DIR}
      )
ENDIF() #NOT MAJA_INSTALL_NO_LIBRARIES
   
