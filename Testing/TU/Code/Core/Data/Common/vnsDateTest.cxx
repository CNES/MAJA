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
 * VERSION : 1-0-3 : DM : 310 : 13 septembre 2011 : Creation                                                *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsUtilitiesUTCDate.cxx 3205 2011-09-14 17:16:24Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsUtilities.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

#include "vnsDate.h"
#include "vnsUtilities.h"

int
vnsDateTest(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage   : " << argv[0] << " Output file" << std::endl;
        return EXIT_FAILURE;
    }
    const char * l_OutputFile = argv[1];

    std::ofstream flux(l_OutputFile);
    flux << std::setprecision(15);

    vns::Tm l_tm;

    flux << "--------------------------------------------------------------------"<<std::endl;
    std::string l_String = "20120824";
    l_tm = vns::Date::GetTmFromDateYYYYMMDD(l_String);
    double l_julian = vns::Date::GetJulianDayAsDouble(l_tm);
    flux << l_String << ": "<<std::endl;
    l_tm.Print(flux);
    flux << " -> in UTC                             : "<< vns::Date::GetDateUTCFromTm(l_tm)<<std::endl;
    flux << " -> in Julian Day (double)             : "<< l_julian<<std::endl;
    flux << " -> in Julian Day (int)                : "<< vns::Date::GetJulianDayAsInteger(l_tm)<<std::endl;
    flux << " -> Back to date                       : "<< vns::Date::GetDateUTCFromTm(vns::Date::GetTmFromJulianDay(l_julian));
//    flux << " -> in Julian Day avec ossim (double)  : "<< vns::Utilities::GetDoubleDate(l_String)<<std::endl;
    flux << "--------------------------------------------------------------------"<<std::endl;
    l_String = "20120824142833";
    l_tm = vns::Date::GetTmFromDateYYYYMMDDHHMMSS(l_String);
    l_julian = vns::Date::GetJulianDayAsDouble(l_tm);
    flux << l_String << ": "<<std::endl;
    l_tm.Print(flux);
    flux << " -> in UTC                             : "<< vns::Date::GetDateUTCFromTm(l_tm)<<std::endl;
    flux << " -> in Julian Day (double)             : "<< l_julian<<std::endl;
    flux << " -> in Julian Day (int)                : "<< vns::Date::GetJulianDayAsInteger(l_tm)<<std::endl;
    flux << " -> Back to date                       : "<< vns::Date::GetDateUTCFromTm(vns::Date::GetTmFromJulianDay(l_julian));
//    flux << " -> in Julian Day avec ossim (double)  : "<< vns::Utilities::GetDoubleDate(l_String)<<std::endl;

    flux << "--------------------------------------------------------------------"<<std::endl;
    l_String = "20141217";
    std::string l_String2 = "073015";
    l_tm = vns::Date::GetTmFromDateYYYYMMDD_And_HHMMSS(l_String, l_String2);
    l_julian = vns::Date::GetJulianDayAsDouble(l_tm);
    flux << l_String << " and " <<l_String2 <<": "<<std::endl;
    l_tm.Print(flux);
    flux << " -> in UTC                             : "<< vns::Date::GetDateUTCFromTm(l_tm)<<std::endl;
    flux << " -> in Julian Day (double)             : "<< l_julian<<std::endl;
    flux << " -> in Julian Day (int)                : "<< vns::Date::GetJulianDayAsInteger(l_tm)<<std::endl;
    flux << " -> Back to date                       : "<< vns::Date::GetDateUTCFromTm(vns::Date::GetTmFromJulianDay(l_julian));
//    flux << " -> in Julian Day avec ossim (double)  : "<< vns::Utilities::GetDoubleDate(l_String)<<std::endl;

    flux << "--------------------------------------------------------------------"<<std::endl;
    l_String = "19800101";
    l_String2 = "12:45:35.123456";
    l_tm = vns::Date::GetTmFromDateYYYYMMDD_And_HHMMSS(l_String, l_String2);
    l_julian = vns::Date::GetJulianDayAsDouble(l_tm);
    flux << l_String << " and " <<l_String2 <<": "<<std::endl;
    l_tm.Print(flux);
    flux << " -> in UTC                             : "<< vns::Date::GetDateUTCFromTm(l_tm)<<std::endl;
    flux << " -> in Julian Day (double)             : "<< l_julian<<std::endl;
    flux << " -> in Julian Day (int)                : "<< vns::Date::GetJulianDayAsInteger(l_tm)<<std::endl;
    flux << " -> Back to date                       : "<< vns::Date::GetDateUTCFromTm(vns::Date::GetTmFromJulianDay(l_julian));
//    flux << " -> in Julian Day avec ossim (double)  : "<< vns::Utilities::GetDoubleDate(l_String)<<std::endl;

    flux << "--------------------------------------------------------------------"<<std::endl;
    l_String = "UTC=2012-01-01T10:51:44";
    l_tm = vns::Date::GetTmFromDateUTC(l_String);
    l_julian = vns::Date::GetJulianDayAsDouble(l_tm);
    flux << l_String << ": "<<std::endl;
    l_tm.Print(flux);
    flux << " -> in UTC                             : "<< vns::Date::GetDateUTCFromTm(l_tm)<<std::endl;
    flux << " -> in Julian Day (double)             : "<< l_julian <<std::endl;
    flux << " -> in Julian Day (int)                : "<< vns::Date::GetJulianDayAsInteger(l_tm)<<std::endl;
    flux << " -> Back to date                       : "<< vns::Date::GetDateUTCFromTm(vns::Date::GetTmFromJulianDay(l_julian));
//    flux << " -> in Julian Day avec ossim (double)  : "<< vns::Utilities::GetDoubleDate("20120101")<<std::endl;

    flux.close();

    return EXIT_SUCCESS;
}
