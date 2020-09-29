#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2572-CNES : 22 mars 2018 : Nouveau Plugin SENTINEL2_TM               #
# VERSION : 2.0.0 : DM : LAIG-DM-MAJA-981-CNES : 2 mars 2017 : ReducedLutComputation extracted as new algo #
# VERSION : 4-3-0 : DM : LAIG-DM-MAC-972-CNES : 7 avril 2015 : Creation du nouveau plugin MUSCATE          #        
# VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     #
# VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 28 octobre 2013 : Creation plugin LANDSAT                  #
# VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Modification pour la pris en compte du        #
#                                                            MACCS generique (PM LAIG-NT-MAC-0239-CNES)    #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte        #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 avril 2010 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(MAJA_INCLUDE_DIRS_SYSTEM "")

#-----------------------------------------------------------------------------
# Include directories from the build tree.
# These directories are always needed.
SET(MAJA_INCLUDE_DIRS_BUILD_TREE
${PROJECT_BINARY_DIR}
${PROJECT_SOURCE_DIR}/Code/Core/Applications
${PROJECT_SOURCE_DIR}/Code/Core/Chain
${PROJECT_SOURCE_DIR}/Plugins/Common/Code/Base
${PROJECT_BINARY_DIR}/Plugins/Common/Code/Base
${PROJECT_SOURCE_DIR}/Code/Algorithms/Common
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/Subsampling
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/DTMProcessing
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/AtmosphericAbsorptionCorrection
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/RayleighCorrection
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/GeometricFlags
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/CirrusFlag
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/CloudMasking
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/WaterMasking
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/RainDetection
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/AOTEstimation
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/SnowMasking
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/CirrusMask
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/CirrusCorrection
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/ScatteringCorrection
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/CompositeImage
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/ReduceLutComputation
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/EnvironmentCorrection
${PROJECT_SOURCE_DIR}/Code/Algorithms/L2/SlopeCorrection 
${PROJECT_SOURCE_DIR}/Code/Core/Data/Common
${PROJECT_SOURCE_DIR}/Code/Core/Data/EarthExplorer
${PROJECT_SOURCE_DIR}/Code/Core/Data/IO
${PROJECT_BINARY_DIR}/Code/Core/Data/Common
${PROJECT_SOURCE_DIR}/Utilities/I3D

)

if(BUILD_TESTING)
  SET(MAJA_INCLUDE_DIRS_BUILD_TREE
    ${MAJA_INCLUDE_DIRS_BUILD_TREE}
    ${PROJECT_SOURCE_DIR}/Code/Core/TestHelperLibrary
    )
endif()


#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in MAJA.  These include
# directories will NOT be available to user projects.
SET(MAJA_INCLUDE_DIRS_BUILD_TREE_CXX)

SET(MAJA_INCLUDE_DIRS_BUILD_TREE_CXX ${MAJA_INCLUDE_DIRS_BUILD_TREE_CXX} )


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Include directories from the install tree.
# Alls install dirs must been RELATIVE PATH, except for the external libraries, specify by the ABSOLUE PATH
SET(MAJA_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}${MAJA_INSTALL_INCLUDE_DIR}")

SET(MAJA_INCLUDE_RELATIVE_DIRS 
        Chain
        AlgorithmsCommon
        AlgorithmsL2
        DataIO
        DataCommon
        DataEarthExplorer
        UtilitiesI3D
        TestHelperLibrary
)

SET(MAJA_INCLUDE_ABSOLUE_DIRS 
    # Include OTB dirs
  	${OTB_INCLUDE_DIRS} 
)

