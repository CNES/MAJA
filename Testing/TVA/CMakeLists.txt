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
#                        o     o                                                                           #
#                        oo   oo   oo        o   oo        ,-.                                             #
#                        o o o o  o  o       o  o  o       \_/                                             #
#                        o  o  o o    o      o o    o     {|||D                                            #
#                        o     o oooooo      o oooooo      / \                                             #
#                        o     o o    o o    o o    o      `-^                                             #
#                        o     o o    o  oooo  o    o                                                      #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 mars 2019 : Creation                                           #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
#                                                                                                          #
############################################################################################################


if(ENABLE_TVA)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TVA)
endif()

SET(MAJADATA_TVA_INPUT ${MAJADATA_TVA}/Input)
SET(MAJADATA_TVA_BASELINE ${MAJADATA_TVA}/Baseline)



ADD_SUBDIRECTORY(Common)
ADD_SUBDIRECTORY(AOTEstimation)
ADD_SUBDIRECTORY(AthmosphericAbsorption)
ADD_SUBDIRECTORY(CirrusCorrection)
ADD_SUBDIRECTORY(CirrusFlag)
ADD_SUBDIRECTORY(CirrusMask)
ADD_SUBDIRECTORY(Composite)
ADD_SUBDIRECTORY(DTMProcessing)
ADD_SUBDIRECTORY(EnvCorrection)
ADD_SUBDIRECTORY(CloudMasking)
ADD_SUBDIRECTORY(RayleighCorrection)
ADD_SUBDIRECTORY(ReduceLutComputation)
ADD_SUBDIRECTORY(ScatteringCorrection)
ADD_SUBDIRECTORY(SlopeCorrection)
ADD_SUBDIRECTORY(SnowMask)
ADD_SUBDIRECTORY(WaterMask)
ADD_SUBDIRECTORY(WaterVaporPostProcessing)
ADD_SUBDIRECTORY(OTBApplications)



