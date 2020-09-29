/*
* Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/
/************************************************************************************************************
 *                                                                                                          *
 *                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         *
 *                             o                                                                            *
 *                          o                                                                               *
 *                        o                                                                                 *
 *                      o                                                                                   *
 *                     o       ooooooo       ooooooo    o         o      oo                                 *
 *    o              o       o        o     o       o   o         o     o   o                               *
 *      o           o       o          o   o         o  o         o    o      o                             *
 *        o        o       o           o   o         o  o         o    o        o                           *
 *         o      o        o      oooo     o         o  o         o   o           o                         *
 *          o    o          o              o         o  o         o   o           o                         *
 *           o  o            o             o         o  o o      o   o          o                           *
 *            oo              oooooooo    o         o   o  oooooo   o      oooo                             *
 *                                                     o                                                    *
 *                                                     o                                                    *
 *                                                    o                            o                        *
 *                                                    o            o      oooo     o   o      oooo          *
 *                                                   o             o         o    o    o         o          *
 *                                                   o            o       ooo     o   o       ooo           *
 *                                                               o       o       o   o          o           *
 *                                                               ooooo   oooo    o   ooooo  oooo            *
 *                                                                              o                           *
 *                                                                                                          *
 ************************************************************************************************************
 *                                                                                                          *
 * Author: CS Systemes d'Information  (France)                                                              * 
 *                                                                                                          * 
 ************************************************************************************************************ 
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 avril 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
// this file defines the vnsBusinessCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//
#include "vnsTestMain.h"

void
RegisterTests()
{
    REGISTER_TEST(vnsReflectanceThresholdFunctorTest);
    REGISTER_TEST(vnsReflectanceVariationCorrelationImageFilterNew);
    REGISTER_TEST(vnsReflectanceVariationCorrelationImageFilterTest);
    REGISTER_TEST(vnsCrossCorrelationFunctorCompileTest);
    REGISTER_TEST(vnsTernaryLogicalOrFunctorTest);
    REGISTER_TEST(vnsStereoCorrectionImageFilterNew);
    REGISTER_TEST(vnsStereoCorrectionImageFilterTest);
    REGISTER_TEST(vnsStereoAltitudeCorrectionImageFilterNew);
    REGISTER_TEST(vnsShadowVariationThresholdImageFilterNew);
    REGISTER_TEST(vnsShadowVariationThresholdImageFilterTest);
    REGISTER_TEST(vnsConditionalDifferenceImageFilterNew);
    REGISTER_TEST(vnsConditionalDifferenceImageFilterTest);
    REGISTER_TEST(vnsShadowProjectionImageFilterNew);
    REGISTER_TEST(vnsShadowProjectionImageFilterTest);
    REGISTER_TEST(vnsShadowMaskProjectionNew);
    REGISTER_TEST(vnsShadowMaskProjectionTest);
    REGISTER_TEST(vnsSnowMaskValidationByLabelImageFilterNew);
    REGISTER_TEST(vnsSnowMaskValidationByLabelImageFilterTest);
    REGISTER_TEST(vnsSnowMaskCorrectionByLabelImageFilterNew);
    REGISTER_TEST(vnsSnowMaskCorrectionImageFilterNew);
    REGISTER_TEST(vnsSnowMaskCorrectionImageFilterTest);
    REGISTER_TEST(vnsNoDataShadowImageFilterNew);
    REGISTER_TEST(vnsNoDataShadowImageFilterTest);
    REGISTER_TEST(vnsCloudLabelUpdatingImageFilterTest);
    REGISTER_TEST(vnsCloudLabelRemovalImageFilterTest);
}
