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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

SET(INPUT_MAJADATATU_IMAGES_SURF ${INPUT_MAJADATATU_IMAGES}/surf)
SET(INPUT_MAJADATATU_IMAGES_SURF_COMP  ${INPUT_MAJADATATU_IMAGES}/surf_comp)
SET(INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP  ${INPUT_MAJADATATU_IMAGES}/surf_ray_comp)
SET(INPUT_MAJADATATU_IMAGES_PILE  ${INPUT_MAJADATATU_IMAGES}/pile_surf)
SET(INPUT_MAJADATATU_IMAGES_BORD  ${INPUT_MAJADATATU_IMAGES}/bord)
SET(INPUT_MAJADATATU_IMAGES_DATE  ${INPUT_MAJADATATU_IMAGES}/date)
SET(INPUT_MAJADATATU_IMAGES_SRTMWAT  ${INPUT_MAJADATATU_IMAGES}/srtm_wat)
SET(INPUT_MAJADATATU_IMAGES_WATER  ${INPUT_MAJADATATU_IMAGES}/water)
SET(INPUT_MAJADATATU_IMAGES_MSQ  ${INPUT_MAJADATATU_IMAGES}/msq)
SET(INPUT_MAJADATATU_IMAGES_OMBRE  ${INPUT_MAJADATATU_IMAGES}/ombre)
SET(INPUT_MAJADATATU_IMAGES_CAST_SHW  ${INPUT_MAJADATATU_IMAGES}/cast_shw)
SET(INPUT_MAJADATATU_IMAGES_SNW ${INPUT_MAJADATATU_IMAGES}/snow)
SET(INPUT_MAJADATATU_IMAGES_TOA  ${INPUT_MAJADATATU_IMAGES}/toa)
SET(INPUT_MAJADATATU_IMAGES_TOA_ABS  ${INPUT_MAJADATATU_IMAGES}/toa_abs)
SET(INPUT_MAJADATATU_IMAGES_TOA_COMP  ${INPUT_MAJADATATU_IMAGES}/toa_comp)
SET(INPUT_MAJADATATU_IMAGES_SAT ${INPUT_MAJADATATU_IMAGES}/sat)
SET(INPUT_MAJADATATU_IMAGES_TAU2 ${INPUT_MAJADATATU_IMAGES}/tau2)
SET(INPUT_MAJADATATU_IMAGES_TAU2F ${INPUT_MAJADATATU_IMAGES}/tau2f)
SET(INPUT_MAJADATATU_IMAGES_TAU2S ${INPUT_MAJADATATU_IMAGES}/tau2s)
SET(INPUT_MAJADATATU_IMAGES_ORTHO_SURF ${INPUT_MAJADATATU_IMAGES}/ORTHO_SURF)
SET(INPUT_MAJADATATU_IMAGES_ORTHO_SURF_COR_ENV ${INPUT_MAJADATATU_IMAGES}/ORTHO_SURF_COR_ENV)
SET(INPUT_MAJADATATU_IMAGES_ORTHO_RHO_ENV ${INPUT_MAJADATATU_IMAGES}/ORTHO_RHO_ENV)
SET(INPUT_MAJADATATU_IMAGES_ORTHO_SURF_COR_PENTE ${INPUT_MAJADATATU_IMAGES}/ORTHO_SURF_COR_PENTE)
SET(INPUT_MAJADATATU_IMAGES_TAU_2 ${INPUT_MAJADATATU_IMAGES}/TAU_2)
SET(INPUT_MAJADATATU_IMAGES_MSQL2 ${INPUT_MAJADATATU_IMAGES}/MSQL2)
SET(INPUT_MAJADATATU_IMAGES_NUAL2 ${INPUT_MAJADATATU_IMAGES}/NUAL2)
SET(INPUT_MAJADATATU_IMAGES_GRID ${INPUT_MAJADATATU_IMAGES}/grid )
SET(INPUT_MAJADATATU_IMAGES_HIGHAOT ${INPUT_MAJADATATU_IMAGES}/highAOT)
SET(INPUT_MAJADATATU_IMAGES_RESAMPLING_REFERENCES ${INPUT_MAJADATATU_IMAGES}/resampling-references)


SET(INPUT_MAJADATATU_IMAGES_L1_FORMOSAT ${INPUT_MAJADATATU_IMAGES}/L1/FORMOSAT)
#SET(INPUT_MAJADATATU_IMAGES_L1_VENUS ${INPUT_MAJADATATU_IMAGES}/L1/VENUS)

#SENTINEL2
SET(S2_DEM_L2RESOLUTION_R1              ${INPUT_MAJADATA_IMAGES_DTM_SENTINEL2_GPP}/S2__TEST_AUX_REFDE2_15SVD____0001.DBL.DIR/S2__TEST_AUX_REFDE2_15SVD____0001_ALT_R1.TIF)
SET(S2_DEM_L2RESOLUTION_R2              ${INPUT_MAJADATA_IMAGES_DTM_SENTINEL2_GPP}/S2__TEST_AUX_REFDE2_15SVD____0001.DBL.DIR/S2__TEST_AUX_REFDE2_15SVD____0001_ALT_R2.TIF)
SET(S2_DEM_L2COARSERESOLUTION           ${INPUT_MAJADATA_IMAGES_DTM_SENTINEL2_GPP}/S2__TEST_AUX_REFDE2_15SVD____0001.DBL.DIR/S2__TEST_AUX_REFDE2_15SVD____0001_ALC.TIF)
SET(S2_TOA_L2RESOLUTION_R1              ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-mono-band-uint16-10980x10980.tif)
SET(S2_TOA_L2COARSERESOLUTION           ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-mono-band-uint16-458x458.tif)
SET(S2_TOA_L2RESOLUTION_SIX_BANDS_R1    ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-six-bands-uint16-10980x10980.tif)
SET(S2_TOA_L2COARSERESOLUTION_SIX_BANDS ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-six-bands-uint16-458x458.tif)
SET(S2_TOA_L2COARSERESOLUTION_16_BANDS ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-16-bands-uint16-458x458.tif)

#VENUS
#SET(INPUT_MAJADATA_IMAGES_DTM_VENUS ${INPUT_MAJADATATU_IMAGES_DTM}/Venus)
#SET(VE_DEM_L2RESOLUTION                 ${INPUT_MAJADATATU_IMAGES_L1_VENUS}/VE_TEST_AUX_REFDE2_CAMARGUE_0001.DBL.DIR/VE_TEST_AUX_REFDE2_CAMARGUE_0001_ALT.TIF)
#SET(VE_DEM_L2COARSERESOLUTION           ${INPUT_MAJADATATU_IMAGES_L1_VENUS}/VE_TEST_AUX_REFDE2_CAMARGUE_0001.DBL.DIR/VE_TEST_AUX_REFDE2_CAMARGUE_0001_ALC.TIF)
#SET(VE_TOA_L1RESOLUTION                 ${INPUT_MAJADATATU_IMAGES}/STUB-L1-PRODUCTS/VE_TEST_VSC_L1VALD_CAMARGUE_20120101.DBL.DIR/VE_TEST_VSC_PDTIMG_L1VALD_CAMARGUE_20120101.DBL.TIF)
#SET(VE_TOA_L2RESOLUTION                 ${INPUT_MAJADATATU_IMAGES}/STUB-L2-PRODUCTS/VE_TEST_VSC_L2VALD_CAMARGUE_20120101.DBL.DIR/VE_TEST_VSC_PDTIMG_L2VALD_CAMARGUE_20120101_FRE.DBL.TIF)
#SET(VE_TOA_L2COARSERESOLUTION           ${INPUT_MAJADATATU_IMAGES}/STUB-L2-PRODUCTS/VE_TEST_VSC_L2VALD_CAMARGUE_20120101.DBL.DIR/PRIVATE/VE_TEST_VSC_PDTANX_L2VALD_CAMARGUE_20120101_RTC.DBL.TIF)



#-----------------------------------------------------------------------------
# Add libraries subdirectories 
ADD_SUBDIRECTORY(Common)
ADD_SUBDIRECTORY(L2)
