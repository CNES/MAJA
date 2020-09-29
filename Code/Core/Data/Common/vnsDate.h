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
 * VERSION : 3.2.1 : FA : LAIG-FA-MAJA-2866-CNES : 26 sept 2018 : Added date with T parsing for S2 NATIF    *
 * VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2572-CNES : 22 mars 2018 : Nouveau Plugin SENTINEL2_TM               *
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-2326-CNES : 26 juin 2017 : Modification date central produit         *
 * VERSION : 4-2-0 : DM : LAIG-DM-MAC-971-CNES : 17 decembre 2014 : Suite montee de version OTB             *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsDate.h 8849 2014-10-15 10:14:35Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef vns_Date
#define vns_Date

#include <ctime>
#include <iostream>
#include <string>

namespace vns
{
    /** \class Date
     *   \brief Date management class.
     *
     */
    class Tm : public std::tm
    {
        public :
        Tm() : std::tm(), m_milliseconds(0.)
        {
        }
        double m_milliseconds;

        void
        Print(std::ostream &os) const;

    };
    class Date
    {
    public:
        typedef Date Self;

        /** Constructeur */
        Date()
        {
        }
        /** Destructeur */
        ~Date()
        {
        }
        // Return the Tm initialized with date formatted like YYYYMMDD or YYYY-MM-DD
        static Tm
        GetTmFromDateYYYYMMDD(const std::string & strDate);
        // Return the Tm initialized with date formated like YYYYMMDDHHMMSS or YYYY-MM-DD-HH-MM-SS
        static Tm
        GetTmFromDateYYYYMMDDHHMMSS(const std::string & strDate);
        // Return the Tm initialized with date formated like YYYYMMDDTHHMMSS or YYYY-MM-DDTHH-MM-SS
        static Tm
        GetTmFromDateYYYYMMDDTHHMMSS(const std::string & strDate);

        // Return the Tm initialized with date formated like YYYYMMDD or YYYY-MM-DD and strHourMinSec HH:MM:SS
        // if strHourMinSec is 10:26:11.936661, suppress .936661
        static Tm
        GetTmFromDateYYYYMMDD_And_HHMMSS(const std::string & strDate, const std::string & strHourMinSec);

        static Tm
        GetTmFromDateUTC(const std::string & utc_date);

        static Tm
        GetTmFromJulianDay(const double & julian_day);

        static std::string
        GetDateUTCFromTm(const Tm & p_tm);

        static std::string
        GetDateYYYYMMDDFromTm(const Tm & p_tm);

        static std::string
        GetDateHHMMSSFromTm(const Tm & p_tm);

        static std::string
		GetDateMillisecsFromTm(const Tm & p_tm);

        // Return  the julian day in "double" precision
        static double
        GetJulianDayAsDouble(const Tm & p_tm);

        // Return  the julian day in "double" precision
        static unsigned int
        GetJulianDayAsInteger(const Tm & p_tm)
        {
            return static_cast<unsigned int>(GetJulianDayAsDouble(p_tm));
        }



    private:
        /** The instance singleton */
//        Tm m_tm;

    };

}

#endif
