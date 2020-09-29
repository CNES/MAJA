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
 * VERSION : 3.2.1 : FA : LAIG-FA-MAJA-2866-CNES : 26 sept 2018 : Bug sur le calcul du jour julien          *
 * VERSION : 3.2.1 : FA : LAIG-FA-MAJA-2866-CNES : 26 sept 2018 : Added date with T parsing for S2 NATIF    *
 * VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2572-CNES : 22 mars 2018 : Nouveau Plugin SENTINEL2_TM               *
 * VERSION : 3.0.0 : FA : LAIG-FA-MAJA-157183-CS : 9 octobre 2017 : code quality                            *
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-2326-CNES : 26 juin 2017 : Modification date central produit         *
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 23 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 11 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Correction du code            *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-2-0 : DM : LAIG-DM-MAC-971-CNES : 17 decembre 2014 : Suite montee de version OTB             *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsDate.cxx 5593 2013-05-22 13:44:40Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsDate.h"
#include "vnsUtilities.h"
#include "vnsMacro.h"
#include <cstdio>

namespace vns
{
    Tm
    Date::GetTmFromDateYYYYMMDD(const std::string & strDate) //YYYYMMDD or YYYY-MM-DD
    {
        // ---------------------------------------------------------------------------------------------
        std::string l_strDate(strDate);
        // ---------------------------------------------------------------------------------------------
        // Get the date in the string YYYYMMDD
        // Remove the caracter '-' in the date
        const std::string strDate2 = Utilities::ReplaceAllSubStrings(l_strDate, "-", "");
        // ---------------------------------------------------------------------------------------------
        // Declare the day, month, year variables
        int l_Day(0);
        int l_Month(0);
        int l_Year(0);
        // ---------------------------------------------------------------------------------------------
        // Read the formated date in "%4d%02d%02d"
        const int code_return = std::sscanf(strDate2.c_str(), "%4d%02d%02d", &l_Year, &l_Month, &l_Day);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while formatting the date '" << strDate << "' with sscanf function.")
        }
        // ---------------------------------------------------------------------------------------------
        // Initialize the input Date Local Time
        Tm tm;
        tm.tm_isdst = -1;
        // ---------------------------------------------------------------------------------------------
        // Force the hour, minutes and second to 0 value
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        tm.m_milliseconds = 0.;
        // ---------------------------------------------------------------------------------------------
        // Set day, month and year
        tm.tm_mday = l_Day;
        tm.tm_wday = 0;
        tm.tm_yday = 0;
        tm.tm_mon = l_Month;
        tm.tm_year = l_Year; // year - 1900 ??;

        return tm;
    }

    Tm
    Date::GetTmFromDateYYYYMMDDHHMMSS(const std::string & strDate) //YYYYMMDDHHMMSS or YYYY-MM-DD-HH-MM-SS
    {
        // ---------------------------------------------------------------------------------------------
        std::string l_strDate(strDate);
        // ---------------------------------------------------------------------------------------------
        // Get the date in the string YYYYMMDD
        // Remove the caracter '-' in the date
        const std::string strDate2 = Utilities::ReplaceAllSubStrings(l_strDate, "-", "");
        // ---------------------------------------------------------------------------------------------
        // Declare the day, month, year variables
        int l_Day(0);
        int l_Month(0);
        int l_Year(0);
        // ---------------------------------------------------------------------------------------------
        // Declare the hour, minutes and seconds variables
        int l_Hour(0);
        int l_Min(0);
        int l_Sec(0);
        // ---------------------------------------------------------------------------------------------
        // Read the formated date in "%4d%02d%02d"
        const int code_return = std::sscanf(strDate2.c_str(), "%4d%02d%02d%2d%02d%02d", &l_Year, &l_Month, &l_Day, &l_Hour, &l_Min, &l_Sec);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while formatting the date '" << strDate << "' with sscanf function.")
        }

        // ---------------------------------------------------------------------------------------------
        // Initialize the input Date Local Time
        Tm tm;
        tm.tm_isdst = -1;
        // ---------------------------------------------------------------------------------------------
        // Force the hour, minutes and second value
        tm.tm_hour = l_Hour;
        tm.tm_min = l_Min;
        tm.tm_sec = l_Sec;
        tm.m_milliseconds = 0.;
        // ---------------------------------------------------------------------------------------------
        // Set day, month and year
        tm.tm_mday = l_Day;
        tm.tm_wday = 0;
        tm.tm_yday = 0;
        tm.tm_mon = l_Month;
        tm.tm_year = l_Year; // year - 1900 ??;

        return tm;
    }

    Tm
    Date::GetTmFromDateYYYYMMDDTHHMMSS(const std::string & strDate) //YYYYMMDDTHHMMSS or YYYY-MM-DDTHH-MM-SS
    {
        // ---------------------------------------------------------------------------------------------
        std::string l_strDate(strDate);
        // ---------------------------------------------------------------------------------------------
        // Get the date in the string YYYYMMDD
        // Remove the caracter '-' in the date
        const std::string strDate2 = Utilities::ReplaceAllSubStrings(l_strDate, "-", "");
        // ---------------------------------------------------------------------------------------------
        // Declare the day, month, year variables
        int l_Day(0);
        int l_Month(0);
        int l_Year(0);
        // ---------------------------------------------------------------------------------------------
        // Declare the hour, minutes and seconds variables
        int l_Hour(0);
        int l_Min(0);
        int l_Sec(0);
        // ---------------------------------------------------------------------------------------------
        // Read the formated date in "%4d%02d%02dT%2d%02d%02d"
        const int code_return = std::sscanf(strDate2.c_str(), "%4d%02d%02dT%2d%02d%02d", &l_Year, &l_Month, &l_Day, &l_Hour, &l_Min, &l_Sec);
        if (code_return < 6)
        {
            vnsStaticExceptionDataMacro("Internal error while formatting the date '" << strDate << "' with sscanf function.")
        }

        // ---------------------------------------------------------------------------------------------
        // Initialize the input Date Local Time
        Tm tm;
        tm.tm_isdst = -1;
        // ---------------------------------------------------------------------------------------------
        // Force the hour, minutes and second value
        tm.tm_hour = l_Hour;
        tm.tm_min = l_Min;
        tm.tm_sec = l_Sec;
        tm.m_milliseconds = 0.;
        // ---------------------------------------------------------------------------------------------
        // Set day, month and year
        tm.tm_mday = l_Day;
        tm.tm_wday = 0;
        tm.tm_yday = 0;
        tm.tm_mon = l_Month;
        tm.tm_year = l_Year; // year - 1900 ??;

        return tm;
    }



    Tm
    Date::GetTmFromDateYYYYMMDD_And_HHMMSS(const std::string & strDate, const std::string & strHourMinSec)
    {
        int l_Day(0);
        int l_Month(0);
        int l_Year(0);
        int l_Hour(0);
        int l_Min(0);
        int l_Sec(0);
        std::string l_strDate(strDate);

        // ---------------------------------------------------------------------------------------------
        // Get the date in the string YYYYMMDD
        std::string strDate2 = Utilities::ReplaceAllSubStrings(l_strDate, "-", "");
        int code_return = std::sscanf(strDate2.c_str(), "%4d%02d%02d", &l_Year, &l_Month, &l_Day);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while formatting the date '" << strDate << "' with sscanf function.")
        }

        // ---------------------------------------------------------------------------------------------
        // example: hour     = 10:26:11.936661
        // Suppress .936661
        std::string strDate3 = strHourMinSec;
        // ---------------------------------------------------------------------------------------------
        if (strDate3.find('.') != std::string::npos)
        {
            strDate3 = Utilities::SplitString(strDate3, '.')[0];
        }
        // ---------------------------------------------------------------------------------------------
        strDate3 = Utilities::ReplaceAllSubStrings(strDate3, "-", "");
        strDate3 = Utilities::ReplaceAllSubStrings(strDate3, ":", "");
        code_return = std::sscanf(strDate3.c_str(), "%2d%02d%02d", &l_Hour, &l_Min, &l_Sec);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while formatting the date '" << strDate << "' with sscanf function.")
        }

        // ---------------------------------------------------------------------------------------------
        // Initialize the input Date Local Time
        Tm tm;
        tm.tm_isdst = -1;
        // ---------------------------------------------------------------------------------------------
        // Force the hour, minutes and second value
        tm.tm_hour = l_Hour;
        tm.tm_min = l_Min;
        tm.tm_sec = l_Sec;
        tm.m_milliseconds = 0.;
        // ---------------------------------------------------------------------------------------------
        // Set day, month and year
        tm.tm_mday = l_Day;
        tm.tm_wday = 0;
        tm.tm_yday = 0;
        tm.tm_mon = l_Month;
        tm.tm_year = l_Year; // year - 1900 ??;

        return tm;
    }

    std::string
    Date::GetDateUTCFromTm(const Tm & p_tm)
    {
        // ---------------------------------------------------------------------------------------------
        char chaine[1024];
        // ---------------------------------------------------------------------------------------------
        // Write the date in the formated "UTC=%04d-%02d-%02dT%02d:%02d:%02d"
        const int code_return = std::snprintf(chaine, sizeof(chaine), "UTC=%04d-%02d-%02dT%02d:%02d:%02d" /*"UTC=%Y-%m-%dT%H:%M:%S"*/,
                p_tm.tm_year, p_tm.tm_mon, p_tm.tm_mday, p_tm.tm_hour, p_tm.tm_min, p_tm.tm_sec);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while getting the date with snprintf function.")
        }

        // ---------------------------------------------------------------------------------------------
        // Returns the value in a string type
        return chaine;
    }

    Tm
    Date::GetTmFromDateUTC(const std::string & utc_date)
    {
        // ---------------------------------------------------------------------------------------------
        // initialize date component
        Tm tm;
        int getYear(0);
        int getMonth(0);
        int getDay(0);
        int getHour(0);
        int getMin(0);
        int getSec(0);
        int getMilli(0);
        // ---------------------------------------------------------------------------------------------
        // try to parse input utc string
        if (utc_date.find(".") != std::string::npos)
        {
            const int code_return = std::sscanf(utc_date.c_str(), "UTC=%04d-%02d-%02dT%02d:%02d:%02d.%03d", &getYear, &getMonth, &getDay, &getHour,
                    &getMin, &getSec,&getMilli);
            if (code_return < 0)
            {
                // raise an exception if not UTC
                vnsStaticExceptionDataMacro("Internal error while formatting the date '" << utc_date << "' with sscanf function.")
            }
        } else {
            const int code_return = std::sscanf(utc_date.c_str(), "UTC=%04d-%02d-%02dT%02d:%02d:%02d", &getYear, &getMonth, &getDay, &getHour,
                    &getMin, &getSec);
            if (code_return < 0)
            {
                // raise an exception if not UTC
                vnsStaticExceptionDataMacro("Internal error while formatting the date '" << utc_date << "' with sscanf function.")
            }
        }

        // ---------------------------------------------------------------------------------------------
        // update date component with parser results
        tm.tm_isdst = -1;
        tm.tm_year = getYear;
        tm.tm_mon = getMonth;
        tm.tm_mday = getDay;
        tm.tm_hour = getHour;
        tm.tm_min = getMin;
        tm.tm_sec = getSec;        // outputDate.setFloatSec(static_cast<float>(getSec));
        tm.m_milliseconds = getMilli*1.0 / 1000.0;
        tm.tm_wday = 0;
        tm.tm_yday = 0;
        // ---------------------------------------------------------------------------------------------
        // return the built date
        return tm;
    }

    Tm
	Date::GetTmFromJulianDay(const double & julian_day)
    {
    	// reverse computing complete tm from a julian day

    	/* some constants used in the maths */
    	const int y = 4716;
    	const int j = 1401;
    	const int m = 2;
    	const int n = 12;
    	const int r = 4;
    	const int p = 1461;
    	const int v = 3;
    	const int u = 5;
    	const int s = 153;
    	const int w = 2;
    	const int B = 274277;
    	const int C = -38;

    	// floating / integer decomposition
		double float_part = julian_day - floor(julian_day);
		const int int_part = floor(julian_day);

		// reverse computation of tm date
		const int f = int_part + j + (((4*int_part + B)/146097)*3)/4 + C;
		const int e = r*f + v;
		const int g = (e%p)/r;
		const int h = u*g+w;
		const int D = (h%s)/u + 1;
		const int M = (h/s +m )%n +1;
		const int Y = (e/p) -y + (n+m-M)/n;

		// tm construction
		Tm result;
		result.tm_isdst = -1;
		result.tm_year = Y;
		result.tm_mon = M;
		result.tm_mday = D;
		if (float_part >= 0.5)
		{
			float_part -= 0.5;
			result.tm_mday += 1;
		} else {
			float_part += 0.5;
		}
		// computing hour
		result.tm_hour = floor(float_part*24.0);
		// minute
		float_part -= (result.tm_hour*1.0 / 24.0);
		result.tm_min = floor(float_part * 1440.0);
		// second
		float_part -= (result.tm_min*1.0 / 1440.0);
		result.tm_sec = floor(float_part * 86400.0);
		// millisecond
		float_part -= (result.tm_sec*1.0 / 86400.0);
		result.m_milliseconds = float_part;

		/**
		 * In order for mktime to work we need to reconvert our mt in the correct convention
		 * Indeed in our case the month are 1-12 while in tm it should be 0-11 and the year must be the number of year since 1900.
		 */
		//Month should be 0-11
		result.tm_mon -= 1;
		//year should be from 1900
		result.tm_year -= 1900;
		mktime(&result);
		//Get back in our convention
		result.tm_mon += 1;
		result.tm_year += 1900;
		// returning computed tm
		return result;
    }

    std::string
	Date::GetDateYYYYMMDDFromTm(const Tm & p_tm)
    {
        // ---------------------------------------------------------------------------------------------
        char chaine[1024];
        // ---------------------------------------------------------------------------------------------
        // Write the date in the formated "UTC=%04d-%02d-%02dT%02d:%02d:%02d"
        const int code_return = std::snprintf(chaine, sizeof(chaine), "%04d%02d%02d", p_tm.tm_year, p_tm.tm_mon, p_tm.tm_mday);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while getting the date with snprintf function.")
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the value in a string type
        return chaine;
    }

    std::string
    Date::GetDateHHMMSSFromTm(const Tm & p_tm)
    {
        // ---------------------------------------------------------------------------------------------
        char chaine[1024];
        // ---------------------------------------------------------------------------------------------
        // Write the date in the formated "UTC=%04d-%02d-%02dT%02d:%02d:%02d"
        const int code_return = std::snprintf(chaine, sizeof(chaine), "%02d%02d%02d", p_tm.tm_hour, p_tm.tm_min, p_tm.tm_sec);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while getting the date with snprintf function.")
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the value in a string type
        return chaine;
    }

    std::string
    Date::GetDateMillisecsFromTm(const Tm & p_tm)
    {
        // ---------------------------------------------------------------------------------------------
        char chaine[1024];
        // ---------------------------------------------------------------------------------------------
        const int code_return = std::snprintf(chaine, sizeof(chaine), "%03d", int(p_tm.m_milliseconds*1000.0));
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while getting the date with snprintf function.")
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the value in a string type
        return chaine;
    }

    double
    Date::GetJulianDayAsDouble(const Tm & p_tm)
    {
        const int year = p_tm.tm_year;
        const int month = p_tm.tm_mon;
        const int day = p_tm.tm_mday;
        const int hour = p_tm.tm_hour;
        const int minu = p_tm.tm_min;
        const int sec = p_tm.tm_sec;
        //Take millisec into account ?
        //const double millisec = p_tm.m_milliseconds;
        const double millisec = 0;
        // TODO revoir formlue calcul Julain Day (copy of otb TeraSarImageMetdata
        // Conversion to julian day
        // according to http://en.wikipedia.org/wiki/Julian_day
        // division are integer divisions:
        const int a = (14 - month) / 12;
        const int y = year + 4800 - a;
        const int m = month + 12 * a - 3;

        double julianDay = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

        // now, the division are NOT integer
        // the -12 is because the julian day start at noon
        julianDay += (hour-12) / 24. + minu / 1440. + sec / 86400.;

        // now, the millisecond
        julianDay += millisec;
        return julianDay;
    }

    void
    Tm::Print(std::ostream &os) const
    {
        os << "- tm_year    : " << this->tm_year << std::endl;
        os << "- tm_mon     : " << this->tm_mon << std::endl;
        os << "- tm_mday    : " << this->tm_mday << std::endl;
        os << "- tm_wday    : " << this->tm_wday << std::endl;
        os << "- tm_yday    : " << this->tm_yday << std::endl;
        os << "- tm_hour    : " << this->tm_hour << std::endl;
        os << "- tm_min     : " << this->tm_min << std::endl;
        os << "- tm_sec     : " << this->tm_sec << std::endl;
        os << "- m_millisec : " << this->m_milliseconds << std::endl;
        os << "- tm_isdst   : " << this->tm_isdst << std::endl;
    }

}

