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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 24 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1769-CNES : 6 juillet 2016 : Implémentation écart type checktools     *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 25 avril 2016 : Ajout macro vnsIsZeroDoubleMacro et       *
 *                                                                                  vnsIsNotZeroDoubleMacro *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 17 juin 2010 : Creation                                             *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMath_h
#define __vnsMath_h

/**
 * otbMath.h defines standard math macros, constants, and other
 * parameters.
 * All mathematics constans, functions (used in the OTB code) are the vcl version (vcl..., CONST_PI, ).
 */

#include "otbMath.h"

namespace vns{

/** Define constants if not define in the "vcl_cmath.h" */

/* Some useful constants.  */
const double CONST_EPSILON =    1e-6; /* devide by zero */
const double CONST_EPSILON_20 = 1e-20; /* devide by zero ; for example, used for AOT Gap Flilling */
const double CONST_EPSILON_4 =  1e-4; /* for NoData testing value */

/** Macros for math operations */

#define vnsEqualsDoubleMacro( x, ref ) (vcl_abs(x - ref) < CONST_EPSILON)
#define vnsEqualsDoubleWithEpsilonMacro( x, ref , epsilon ) (vcl_abs(x - ref) < epsilon)

#define vnsIsZeroDoubleMacro( x ) (vcl_abs(x) < CONST_EPSILON)
#define vnsIsNotZeroDoubleMacro( x ) (vcl_abs(x) > CONST_EPSILON)

#define vnsDifferentDoubleMacro( x, ref ) (vcl_abs((x) - (ref)) > CONST_EPSILON)

#define vnsStrictlySupDoubleMacro( x, y, epsilon ) ( (vnsEqualsDoubleWithEpsilonMacro(x, y,epsilon)?false:(x > y) ) )
#define vnsStrictlyInfDoubleMacro( x, y, epsilon ) ( (vnsEqualsDoubleWithEpsilonMacro(x, y,epsilon)?false:(x < y) ) )

#define vnsSupOrEqualDoubleMacro( x, y, epsilon ) ( (vnsEqualsDoubleWithEpsilonMacro(x, y,epsilon)?true:(x > y) ) )
#define vnsInfOrEqualDoubleMacro( x, y, epsilon ) ( (vnsEqualsDoubleWithEpsilonMacro(x, y,epsilon)?true:(x < y) ) )

#define vnsIsZeroDoubleWithEpsilonMacro( x, epsilon ) (vcl_abs(x) < epsilon)

#define vnsDifferentDoubleWithEpsilonMacro( x, ref, epsilon ) (vcl_abs((x) - (ref)) > epsilon)

#define vnsDifferentTab2DimsDoubleWithEpsilonMacro( x, ref, epsilon ) ((vcl_abs((x)[0] - (ref)[0]) > (epsilon)) || (vcl_abs((x)[1] - (ref)[1]) > (epsilon)))


/** Macros for math operations */
#define vnsIsNoDataMacro( x, ref ) (vcl_abs((x) - (ref)) < CONST_EPSILON_4)

#define vnsIsNotNoDataMacro( x, ref ) (vcl_abs((x) - (ref)) > CONST_EPSILON_4)

}

#endif
