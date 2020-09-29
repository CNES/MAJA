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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 26 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsRegionForSubsample.h"

int vnsRegionForSubsampleNew(int /*argc*/, char * /*argv*/ [])
{

    const unsigned int Dimension = 2;
    typedef vns::RegionForSubsample<Dimension> RegionType;

    RegionType l_Region;

    RegionType::IndexType l_Index;
    RegionType::IndexType l_Start;

    l_Index[0] = 6;
    l_Index[1] = 6;

    l_Start[0] = 5;
    l_Start[1] = 5;

    RegionType::SizeType  l_Size;
    unsigned int  l_Radius = 3;

    l_Size[0]  = 15;  // size along X
    l_Size[1]  = 15;  // size along Y

    l_Region.SetIndex(l_Start);
    l_Region.SetSize(l_Size);
    l_Region.SetLargestPossibleRegion(l_Region.GetLargestPossibleRegion());

    std::cout<<"Region 1 : "<<l_Region<<std::endl;

    l_Region.SetSubsampleStep(1);
    l_Region.ChangeCurrentRegion(l_Index, l_Radius);

    std::cout<<"Region 2 : "<<l_Region<<std::endl;

    l_Radius = 2;

    l_Region.ChangeCurrentRegionRadius(l_Radius);

    std::cout<<"Region 3 : "<<l_Region<<std::endl;

    return EXIT_SUCCESS;
}
