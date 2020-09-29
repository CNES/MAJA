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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 02 avril 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "AlgorithmsL2AOT")


SET(TESTS1 ${CXX_TEST_PATH}/vnsL2AOTTests1)
SET(TESTS2 ${CXX_TEST_PATH}/vnsL2AOTTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  vnsL2AOTTests1  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    XMLFileHolder class with .xml file test

# -------    vnsChangeValueWithMaskFunctorTest.cxx
ADD_TEST(${PREFIX}ChangeValueWithMaskFunctorTest1 ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}ChangeValueWithMaskFunctorTest1.tif
    ${TEMP}/${PREFIX}ChangeValueWithMaskFunctorTest1.tif
    vnsChangeValueWithMaskFunctorTest
    ${INPUT_MAJADATATU_IMAGES_TAU2}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.tau2
    ${INPUT_MAJADATATU_IMAGES_TAU2}/mask_126_112.tif
    ${TEMP}/${PREFIX}ChangeValueWithMaskFunctorTest1.tif
    0
    -10
    )
   
ADD_TEST(${PREFIX}ChangeValueWithMaskFunctorTest2 ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}ChangeValueWithMaskFunctorTest2.tif
    ${TEMP}/${PREFIX}ChangeValueWithMaskFunctorTest2.tif
    vnsChangeValueWithMaskFunctorTest
    ${INPUT_MAJADATATU_IMAGES_TAU2}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.tau2
    ${INPUT_MAJADATATU_IMAGES_TAU2}/mask_126_112.tif
    ${TEMP}/${PREFIX}ChangeValueWithMaskFunctorTest2.tif
    255
    -10
    )


# -------    vnsRunLevenbergMarquardtOptimizationNew.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationNew ${TESTS1}
    vnsRunLevenbergMarquardtOptimizationNew
    )        
        
# -------    vnsRunLevenbergMarquardtOptimizationTest.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationTest ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationTest.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationTest.txt
    vnsRunLevenbergMarquardtOptimizationTest
    ${INPUT_MAJADATATU_IMAGES_TOA_ABS}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.tif # IPTOAC
    ${INPUT_MAJADATATU_IMAGES_TOA_COMP}/Sudouest_20060810_MS_fmsat_ortho_toa_comp_96m.tif # L2inRTASub 
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060810_MS_fmsat_ortho_surf_ray_comp_96m.tif #L2inRCRSub
    ${INPUT_MAJADATATU_IMAGES_DTM}/Sudouest_96m.mnt # DTM image
    ${INPUT_MAJADATATU_FILES}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.minilut #miniLUT
    ${INPUT_MAJADATATU_FILES}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.minilut #miniLUT
    0.05   # AOTDark
    10  # Weq1
    2  # Weq2
    10  # Wdark
    1e9  # Wbounds
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationTest.txt
    )        
        
# -------    vnsRunLevenbergMarquardtOptimizationValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationValid1 ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationValid1.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid1.txt
    vnsRunLevenbergMarquardtOptimizationValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa1.20091016 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa2.20091016 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/rsurf_prior.20091016 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/h_multit.20091016 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_1.20091016 # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_2.20091016 # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/nbpix.20091016
    ${INPUT_MAJADATATU_FILES}/Levenberg/Kpond_multit.20091016
    ${INPUT_MAJADATATU_FILES}/Levenberg/Tau_max.20091016
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau1.20091016
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau2.20091016
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    10  # Wdark, 0 to correspond to the immitated test
    1e9  # Wbounds
    40 #size1
    1 #size2
    1e-10; # ftol Function value tolerance
    1e-10; # xtol Gradient magnitude tolerance
    1e-5; # gtol Search space tolerance
    1e-11; # Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid1.txt
    ) 
    
    
    
# -------    vnsRunLevenbergMarquardtOptimizationValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationValid2 ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationValid2.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid2.txt
    vnsRunLevenbergMarquardtOptimizationValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa1.20090603 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa2.20090603 # IPTOACSub 
    ${INPUT_MAJADATATU_FILES}/Levenberg/rsurf_prior.20090603 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/h_multit.20090603 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_1.20090603 # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_2.20090603 # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/nbpix.20090603
    ${INPUT_MAJADATATU_FILES}/Levenberg/Kpond_multit.20090603
    ${INPUT_MAJADATATU_FILES}/Levenberg/Tau_max.20090603
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau1.20090603
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau2.20090603
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    46 #size1 = nb of pixel
    1 #size2
    0.0000000001 #1e-10; // ftol Function value tolerance
    0.00001 #1e-5; // xtol Gradient magnitude tolerance
    0.00000001 #1e-8; // gtol Search space tolerance
    0.00000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid2.txt
    )
        

# -------    vnsRunLevenbergMarquardtOptimizationValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationValid3 ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationValid3.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid3.txt
    vnsRunLevenbergMarquardtOptimizationValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa1.20070223 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa2.20070223 # IPTOACSub 
    ${INPUT_MAJADATATU_FILES}/Levenberg/rsurf_prior.20070223 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/h_multit.20070223 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_1.20070223 # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_2.20070223 # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/nbpix.20070223
    ${INPUT_MAJADATATU_FILES}/Levenberg/Kpond_multit.20070223
    ${INPUT_MAJADATATU_FILES}/Levenberg/Tau_max.20070223
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau1.20070223
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau2.20070223
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    48 #size1
    1 #size2
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid3.txt
    )
    
# -------    vnsRunLevenbergMarquardtOptimizationValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationValid4 ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationValid4.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid4.txt
    vnsRunLevenbergMarquardtOptimizationValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa1.20060527 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa2.20060527 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/rsurf_prior.20060527 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/h_multit.20060527 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_1.20060527 # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_2.20060527 # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/nbpix.20060527
    ${INPUT_MAJADATATU_FILES}/Levenberg/Kpond_multit.20060527
    ${INPUT_MAJADATATU_FILES}/Levenberg/Tau_max.20060527
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau1.20060527
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau2.20060527
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    29 #size1
    1 #size2
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid4.txt
    )
    
    
# -------    vnsRunLevenbergMarquardtOptimizationValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationValid5 ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationValid5.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid5.txt
    vnsRunLevenbergMarquardtOptimizationValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa1.20060608 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa2.20060608 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/rsurf_prior.20060608 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/h_multit.20060608 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_1.20060608 # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_2.20060608 # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/nbpix.20060608
    ${INPUT_MAJADATATU_FILES}/Levenberg/Kpond_multit.20060608
    ${INPUT_MAJADATATU_FILES}/Levenberg/Tau_max.20060608
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau1.20060608 #  date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau2.20060608 #  date D
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    49 #size1
    1 #size2
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid5.txt
    )
    
# -------    vnsRunLevenbergMarquardtOptimizationValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationValid6 ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationValid6.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid6.txt
    vnsRunLevenbergMarquardtOptimizationValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa1.20060909 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/rtoa2.20060909 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/rsurf_prior.20060909 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/h_multit.20060909 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_1.20060909 # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/lut_inv_2.20060909 # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/nbpix.20060909
    ${INPUT_MAJADATATU_FILES}/Levenberg/Kpond_multit.20060909
    ${INPUT_MAJADATATU_FILES}/Levenberg/Tau_max.20060909
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau1.20060909
    ${INPUT_MAJADATATU_FILES}/Levenberg/tau2.20060909
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    49 #size1
    1 #size2
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationValid6.txt
    )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  vnsL2AOTTests2  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
# -------    vnsGapFillingImageFilterNew.cxx   
ADD_TEST(${PREFIX}GapFillingImageFilterNew ${TESTS2}
    vnsGapFillingImageFilterNew
    )


# -------    vnsGapFillingImageFilterTest.cxx   
ADD_TEST(${PREFIX}GapFillingImageFilterTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}ImageAOTFilled.tif
    ${TEMP}/${PREFIX}ImageAOTFilled.tif   
    vnsGapFillingImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_TAU2}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.tau2 # IPAOTSubSub brut
    ${TEMP}/${PREFIX}ImageAOTFilled.tif # output : OutputFilledImage
    ${TEMP}/${PREFIX}MaskAOTFilled.tif # outputMaskFileName
    0.052 # AOT glob / mean value 
    0.2 # default AOT
    1 # true for AOT Has Valid Pixel
    2 # init window radius    
    16 # max window radius,[1,2,4,8...]
    -0.1 # NoDataIn output value
    )

# -------    case Mean == NoData   
ADD_TEST(${PREFIX}GapFillingImageFilterTest2 ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}DefaultImageAOTFilled.tif
    ${TEMP}/${PREFIX}DefaultImageAOTFilled.tif 
    #${INPUT_MAJADATATU_IMAGES_TAU2}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.tau2f # IPAOTFilled
    vnsGapFillingImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_TAU2}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.tau2 # IPAOTSubSub brut
    ${TEMP}/${PREFIX}DefaultImageAOTFilled.tif # output : OutputFilledImage
    ${TEMP}/${PREFIX}DefaultMaskAOTFilled.tif # outputMaskFileName
    -0.1 # AOT glob / mean value 
    0.2 # default AOT
    1 # true for AOT Has Valid Pixel
    2 # init window radius    
    16 # max window radius,[1,2,4,8...]
    -0.1 # NoDataIn output value
    )

# -------    vnsGapFillingImageFilterTest.cxx   
ADD_TEST(${PREFIX}GapFillingImageFilterTest3 ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}ImageAOTFilled3.tif
        ${TEMP}/${PREFIX}ImageAOTFilled3.tif   
    vnsGapFillingImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_TAU2}/Sudouest_20060206_MS_fmsat_ortho_toa_96m.tau2 # IPAOTSubSub brut
    ${TEMP}/${PREFIX}ImageAOTFilled3.tif # output : OutputFilledImage
    ${TEMP}/${PREFIX}MaskAOTFilled3.tif # outputMaskFileName
    0.155 # AOT glob / mean value 
    0.2 # default AOT
    1 # true for AOT Has Valid Pixel
    2 # init window radius    
    16 # max window radius,[1,2,4,8...]
    -0.1 # NoDataIn output value
    )
    
   

 
# -------    vnsRunLevenbergMarquardtOptimizationMSNew.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationMSNew ${TESTS2}
    vnsRunLevenbergMarquardtOptimizationMSNew
    )       


# -------    vnsRunLevenbergMarquardtOptimizationMSValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020111 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020111.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020111.txt
    vnsRunLevenbergMarquardtOptimizationMSValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/rtoa2_ddv.20020111 # IPTOACSub 
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/h_ddv.20020111 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/lut_inv_2_ddv.20020111_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/nbpix_ddv.20020111
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/Tau_max.20020111
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/tau2.20020111
    0.01  # AOTmin
    0 # blue band
    1 # red band
    0.3 # W_MS
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    0.610 # Slope
    0.004 # YIntercept
    0 # NDVI slope
    1 # NDVI YIntercept
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020111.txt
    )

# -------    vnsRunLevenbergMarquardtOptimizationMSValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020212 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020212.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020212.txt
    vnsRunLevenbergMarquardtOptimizationMSValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/rtoa2_ddv.20020212 # IPTOACSub 
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/h_ddv.20020212 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/lut_inv_2_ddv.20020212_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/nbpix_ddv.20020212
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/Tau_max.20020212
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/tau2.20020212
    0.01  # AOTmin
    0 # blue band
    1 # red band
    0.3 # W_MS
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    0.610 # Slope
    0.004 # YIntercept
    0 # NDVI slope
    1 # NDVI YIntercept
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020212.txt
    )

# -------    vnsRunLevenbergMarquardtOptimizationMSValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020325 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020325.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020325.txt
    vnsRunLevenbergMarquardtOptimizationMSValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/rtoa2_ddv.20020325 # IPTOACSub 
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/h_ddv.20020325 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/lut_inv_2_ddv.20020325_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/nbpix_ddv.20020325
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/Tau_max.20020325
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/tau2.20020325
    0.01  # AOTmin
    0 # blue band
    1 # red band
    0.3 # W_MS
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    0.610 # Slope
    0.004 # YIntercept
    0 # NDVI slope
    1 # NDVI YIntercept    
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20020325.txt
    )

# -------    vnsRunLevenbergMarquardtOptimizationMSValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationMSValid20030802 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20030802.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20030802.txt
    vnsRunLevenbergMarquardtOptimizationMSValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/rtoa2_ddv.20030802 # IPTOACSub 
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/h_ddv.20030802 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/lut_inv_2_ddv.20030802_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/nbpix_ddv.20030802
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/Tau_max.20030802
    ${INPUT_MAJADATATU_FILES}/Levenberg/MS/tau2.20030802
    0.01  # AOTmin
    0 # blue band
    1 # red band
    0.3 # W_MS
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    0.713 # Slope
    -0.005 # YIntercept
    0 # NDVI slope
    1 # NDVI YIntercept    
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationMSValid20030802.txt
    )

# -------    vnsRunLevenbergMarquardtOptimizationSTNew.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationSTNew ${TESTS2}
    vnsRunLevenbergMarquardtOptimizationSTNew
    ) 
    
# -------    vnsRunLevenbergMarquardtOptimizationSTValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020111 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020111.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020111.txt
    vnsRunLevenbergMarquardtOptimizationSTValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa1.20020111 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa2_mt.20020111 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa2_ddv.20020111 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rsurf_prior.20020111 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/h_mt.20020111 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/h_ddv.20020111 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_1.20020111_cs # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_2_mt.20020111_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_2_ddv.20020111_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/nbpix_mt.20020111
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/nbpix_ddv.20020111
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Kpond_multit.20020111
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Kpond_ddv.20020111
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Tau_max.20020111
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/tau1.20020111
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/tau2.20020111
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    2  # NbOfMSUsedBands 1 to 4
    0 # blue band
    1 # red band
    0 # NDVI1_aero
    1 # NDVI2_aero
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    0.610 # Slope
    0.004 # YIntercept
    0 # NDVI slope
    1 # NDVI YIntercept    
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020111.txt
    ) 
    
    
# -------    vnsRunLevenbergMarquardtOptimizationSTValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020212 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020212.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020212.txt
    vnsRunLevenbergMarquardtOptimizationSTValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa1.20020212 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa2_mt.20020212 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa2_ddv.20020212 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rsurf_prior.20020212 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/h_mt.20020212 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/h_ddv.20020212 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_1.20020212_cs # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_2_mt.20020212_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_2_ddv.20020212_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/nbpix_mt.20020212
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/nbpix_ddv.20020212
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Kpond_multit.20020212
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Kpond_ddv.20020212
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Tau_max.20020212
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/tau1.20020212
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/tau2.20020212
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    2  # NbOfMSUsedBands 1 to 4
    0 # blue band
    1 # red band
    2 # NDVI1_aero
    3 # NDVI2_aero
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    0.610 # Slope
    0.004 # YIntercept
    0 # NDVI slope
    1 # NDVI YIntercept    
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020212.txt
    )     

# -------    vnsRunLevenbergMarquardtOptimizationSTValid.cxx   
ADD_TEST(${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020325 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020325.txt
        ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020325.txt
    vnsRunLevenbergMarquardtOptimizationSTValid
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa1.20020325 # L2inRTA
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa2_mt.20020325 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rtoa2_ddv.20020325 # IPTOACSub
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/rsurf_prior.20020325 #L2inRCR
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/h_mt.20020325 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/h_ddv.20020325 # DTM image
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_1.20020325_cs # miniLUT "composite" date D-1
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_2_mt.20020325_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/lut_inv_2_ddv.20020325_cs # miniLUT date D
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/nbpix_mt.20020325
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/nbpix_ddv.20020325
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Kpond_multit.20020325
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Kpond_ddv.20020325
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/Tau_max.20020325
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/tau1.20020325
    ${INPUT_MAJADATATU_FILES}/Levenberg/ST/tau2.20020325
    0.01  # AOTmin
    1  # NbOfUsedBands 1 to 4
    2  # NbOfMSUsedBands 1 to 4    
    0 # blue band
    1 # red band
    2 # NDVI1_aero
    3 # NDVI2_aero
    10  # Wdark, 0 to correspond to the immitated test 
    1e9  # Wbounds
    0.610 # Slope
    0.004 # YIntercept
    0 # NDVI slope
    1 # NDVI YIntercept    
    0.000000001 #1e-10; // ftol Function value tolerance
    0.0000001 #1e-5; // xtol Gradient magnitude tolerance
    0.000001 #1e-5; // gtol Search space tolerance
    0.000000001 #1e-11; // Step / epislon
    200 # Maximum number of iterations
    0.2 #AOTInitialD
    0.2 #AOTinitialDm1
    ${TEMP}/${PREFIX}RunLevenbergMarquardtOptimizationSTValid20020325.txt
    ) 

# -------    vnsMultiTemporalAOTMethodFunctionsNew.cxx   
ADD_TEST(${PREFIX}MultiTemporalAOTMethodFunctionsNew ${TESTS2}
    vnsMultiTemporalAOTMethodFunctionsNew
    ) 

# -------    vnsMultiSpectralAOTMethodFunctionsNew.cxx   
ADD_TEST(${PREFIX}MultiSpectralAOTMethodFunctionsNew ${TESTS2}
    vnsMultiSpectralAOTMethodFunctionsNew
    )     


# -------   vnsMultiTemporalAOTMethodThresholdsGeneratorTest.cxx   
ADD_TEST(${PREFIX}MultiTemporalAOTMethodThresholdsGeneratorTest ${TESTS2}
    --compare-n-ascii ${EPSILON_6} 1
    ${BASELINEDATATU_FILES}/${PREFIX}ReflectanceThresholds.txt
    ${TEMP}/${PREFIX}ReflectanceThresholds.txt
    vnsMultiTemporalAOTMethodThresholdsGeneratorTest
    ${INPUT_MAJADATATU_FILES}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.minilut #miniLUT
    0
    1
    0.      # First AOT
    1.12    # Second AOT
    0.      # TOA refl min
    1.      # TOA refl max
    0.01    # TOA refl step
    0.01    # min diff threshold
    220.    # altitude mean
    ${TEMP}/${PREFIX}ReflectanceThresholds.txt
    )    

    
# Src1 sources files
SET(SRCS1
vnsL2AOTTests1.cxx
vnsChangeValueWithMaskFunctorTest.cxx
vnsRunLevenbergMarquardtOptimizationNew.cxx
vnsRunLevenbergMarquardtOptimizationTest.cxx
vnsRunLevenbergMarquardtOptimizationValid.cxx
)


SET(SRCS2
vnsL2AOTTests2.cxx
vnsGapFillingImageFilterNew.cxx
vnsGapFillingImageFilterTest.cxx
vnsRunLevenbergMarquardtOptimizationMSNew.cxx
vnsRunLevenbergMarquardtOptimizationMSValid.cxx
vnsRunLevenbergMarquardtOptimizationSTNew.cxx
vnsRunLevenbergMarquardtOptimizationSTValid.cxx
vnsMultiTemporalAOTMethodThresholdsGeneratorTest.cxx
vnsMultiSpectralAOTMethodFunctionsNew.cxx
vnsMultiTemporalAOTMethodFunctionsNew.cxx
)

ADD_EXECUTABLE(vnsL2AOTTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsL2AOTTests1 ${OTB_LIBRARIES} 
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )

ADD_EXECUTABLE(vnsL2AOTTests2 ${SRCS2})
TARGET_LINK_LIBRARIES(vnsL2AOTTests2 ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )


ENDIF( NOT MAJA_DISABLE_CXX_TESTING )

