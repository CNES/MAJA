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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsSmacCoefContainer.h"
#include "vnsLoggers.h"


int
vnsSmacCoefContainerTest(int /*argc*/, char * /*argv*/[])
{
  typedef vns::SmacCoefContainer ContainerType;
  
  ContainerType cont;

  // Test the setters
  cont.SetAH2O(1);
  cont.SetNH2O(2);
  cont.SetAO3(3);
  cont.SetNO3(4);
  cont.SetAO2(5);
  cont.SetNO2(6);
  cont.SetPO2(7);
  cont.SetACO2(8);
  cont.SetNCO2(9);
  cont.SetPCO2(10);
  cont.SetACH4(11);
  cont.SetNCH4(12);
  cont.SetPCH4(13);
  cont.SetANO2(14);
  cont.SetNNO2(15);
  cont.SetPNO2(16);
  cont.SetACO(17);
  cont.SetNCO(18);
  cont.SetPCO(19);
  cont.SetA0S(20);
  cont.SetA1S(21);
  cont.SetA2S(22);
  cont.SetA3S(23);
  cont.SetA0T(24);
  cont.SetA1T(25);
  cont.SetA2T(26);
  cont.SetA3T(27);
  cont.SetTauR(28);
  cont.SetSR(29);
  cont.SetA0TauP(30);
  cont.SetA1TauP(31);
  cont.SetWO(32);
  cont.SetGC(33);
  cont.SetA0P(34);
  cont.SetA1P(35);
  cont.SetA2P(36);
  cont.SetA3P(37);
  cont.SetA4P(38);
  cont.SetResT1(39);
  cont.SetResT2(40);
  cont.SetResT3(41);
  cont.SetResT4(42);
  cont.SetResR1(43);
  cont.SetResR2(44);
  cont.SetResR3(45);
  cont.SetResa1(46);
  cont.SetResa2(47);
  cont.SetResa3(48);
  cont.SetResa4(49);

  // Test vector copy
  std::vector<double> coefList(49,0.);
  for(unsigned int i=0; i<coefList.size(); i++)
    coefList.at(i) = 0.5*static_cast<double>(i+1);
  
  ContainerType cont2;
  cont2.SetCoefficients( coefList );


  // Test the Print method
  cont2.Print(std::cout);

  return EXIT_SUCCESS;
}
