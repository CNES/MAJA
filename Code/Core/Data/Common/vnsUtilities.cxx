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
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction données des CAMS             *
 * VERSION : 2.1.0 : FA : LAIG-FA-MAJA-2328-CNES : 27 juin 2017 : Correction fautes d'anglais               *
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 15 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 24 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Correction du code            *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-142535-CS : 2 mars 2016 : Mauvaise extraction de la date du fichier   *
 *                                                                                             LUT LTC cree *
 * VERSION : 4-7-0 : FA : LAIG-FA-MAC-1513-CNES : 1 decembre 2015 : SetEnv pour corriger PB NbThreads > 100 *
 * VERSION : 4-6-3 : FA : LAIG-FA-MAC-1504-CNES : 23 novembre 2015 : Erreur à la lecteur d'un produit L1    *
 *                                                                                                 S2 GPP   *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 *                                                                                   sur le code source C++ *
 * VERSION : 4-3-1 : FA : LAIG-FA-MAC-1286-CNES : 1 juin 2015 : Correction pour gen rapport de production   *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-119935-CS : 24 avril 2014 : Ajout Extendfilename pour ecriture images *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120000-CS : 20 mars 2014 : Methode de conversion IntToString ajoutee  *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-119924-CS : 19 mars 2014 : Fonction formattage Version                *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 10 octobre 2013 : Modification pour les TU                *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-355-CNES : 11 juin 2012 : Correction Header S2, Validity start/stop   *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-318-CNES : 30 mai 2012 : L3 Formosat, erreur sur l'utilisation d'un   *
 *                                                             indice dans une map.                         *
 * VERSION : 1-0-0-3 : DM : 310 : 14 septembre 2011 : Gestion d'un liste de donnees meteo.                    *
 * VERSION : 1-0-0-3 : FA : 315 : 8 septembre 2011 : Correction sur le "site" qui doit etre sur 8 caracteres  *
 * VERSION : 1-0-0-1 : FA : 214 : 17 mai 2011 : Correction sur la periode de validite du produit L3           *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 28 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include <cstdio>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <locale>
#include "itksys/Process.h"
#include "itksys/SystemTools.hxx"

#include "vnsUtilities.h"
#include "vnsMacro.h"
#include "vnsConfigure.h"
#include "vnsDate.h"

namespace vns
{

    // Constructor
    Utilities::Utilities()
    {
    }

    // Destructor
    Utilities::~Utilities()
    {
    }

    // Avoid duplication of the same product
    Utilities::ListOfStrings
    Utilities::RemovingDuplicationFilenames(const ListOfFilenames & listOfFilename)
    {
        const SetOfStrings l_Set(listOfFilename.begin(), listOfFilename.end());
        const ListOfFilenames l_listOfFilename(l_Set.begin(), l_Set.end());
        return l_listOfFilename;
    }

    void
    Utilities::RelativeToAbsoluteFilename(const std::string & path, ListOfFilenames & listOfFilename)
    {
        const unsigned int nbFilenames(listOfFilename.size());
        for (unsigned int i = 0; i < nbFilenames; i++)
        {
            listOfFilename[i] = Utilities::JoinFilenames(path, listOfFilename[i]);
        }
    }
    void
    Utilities::CheckingExistenceFilenames(const ListOfFilenames & listOfFilename)
    {
        const unsigned int nbFilenames(listOfFilename.size());
        for (unsigned int i = 0; i < nbFilenames; i++)
        {
            CheckingExistenceFilename(listOfFilename[i]);
        }
    }

    void
    Utilities::CheckingExistenceFilename(const std::string & Filename)
    {
        if (itksys::SystemTools::FileExists(Filename) == false)
        {
            vnsStaticExceptionDataMacro("Utilities::CheckingExtentFilenames: The filename " << Filename << " doesn't exist !")
        }
        if (itksys::SystemTools::FileIsDirectory(Filename) == true)
        {
            vnsStaticExceptionDataMacro(
                    "Utilities::CheckingExtentFilenames: The filename " << Filename << " doesn't exist. It's a directory !")
        }
    }
    void
    Utilities::CheckingExistenceDirectory(const std::string & Filename)
    {
        if (itksys::SystemTools::FileIsDirectory(Filename) == false)
        {
            vnsStaticExceptionDataMacro("Utilities::CheckingExtentFilenames: The directory " << Filename << " doesn't exist !")
        }
    }

    std::string
    Utilities::ProvideFilename(const std::string & directory, const std::string & baseFilename, const std::string & extentionFilename)
    {
        // ---------------------------------------------------------------------------------------------
        // Set the number max of filename
        const unsigned int nbMaxFilenames(10000);
        unsigned int cptFilenames(0);
        bool provide(false);
        std::string filename("");

        // ---------------------------------------------------------------------------------------------
        // Loops and find a valid filename
        while ((provide == false) && (cptFilenames < nbMaxFilenames))
        {
            std::ostringstream flux;
            flux << directory << "/" << baseFilename << cptFilenames << "." + extentionFilename;
            filename = flux.str();
            // ---------------------------------------------------------------------------------------------
            // Checks if the filename already exists
            if (itksys::SystemTools::FileExists(filename) == false)
            {
                provide = true;
            }
            cptFilenames++;
        }
        // ---------------------------------------------------------------------------------------------
        // 'nbMaxFilenames' file already exists.
        // Error: user must be clear it working dir
        if (provide == false)
        {
            vnsStaticExceptionDataMacro(
                    "Utilities::ProvideFilename: Impossible to find a available filename in the " << directory
                            << " caching directory ! The maximum number of filename is " << nbMaxFilenames << ". Clean the directory.")
        }
        return filename;
    }
    std::string
    Utilities::ProvideCachingFilename(const std::string & directory, const std::string & baseFilename,
            const std::string & extentionFilename)
    {
        // ---------------------------------------------------------------------------------------------
        // Returns a valid caching filename
        return ProvideFilename(directory, "vns_caching_" + baseFilename, extentionFilename);
    }
    std::string
    Utilities::ProvideCrocodileclipFilename(const std::string & directory, const std::string & baseFilename,
            const bool & overwriteOutputFilename)
    {
        // ---------------------------------------------------------------------------------------------
        // Returns a valid crocodile clip filename

        // ---------------------------------------------------------------------------------------------
        // Manage a cpt to disable overwriting the output file
        if (overwriteOutputFilename == false)
        {
            return ProvideFilename(directory, "vns_crocodileclip_" + itksys::SystemTools::GetFilenameWithoutExtension(baseFilename),
                    itksys::SystemTools::GetFilenameLastExtension(baseFilename));
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            return directory + "/vns_crocodileclip_" + baseFilename;
        }
    }

    std::string
    Utilities::ExtractFileTypeFromEarthExplorerFilename(const std::string & filename, const unsigned int & position_of_file_type)
    {
        const ListOfStrings list_baseFilename = SplitFilename(filename, '_');
        if (list_baseFilename.size() < position_of_file_type)
        {
            vnsStaticExceptionDataMacro(
                    "Utilities::ExtractFileTypeFromEarthExplorerFilename: Internal error. Impossible to detect the satellite FirleType at the index position '"
                            << position_of_file_type << "' in the filename " << filename << " !")
        }
        return list_baseFilename[position_of_file_type];

    }

    Utilities::ListOfStrings
    Utilities::GetListOfUsedProductsId(const ListOfFilenames & listOfFilename)
    {
        // ---------------------------------------------------------------------------------------------
        ListOfStrings l_listOfUsedProductsId;
        const unsigned int l_NbFiles(listOfFilename.size());
        // ---------------------------------------------------------------------------------------------
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            l_listOfUsedProductsId.push_back(itksys::SystemTools::GetFilenameWithoutLastExtension(listOfFilename[i]));
        }
        return l_listOfUsedProductsId;
    }

    bool
    Utilities::IsANumber(const std::string & value)
    {
        bool isNumber = true;
        for (std::string::const_iterator k = value.begin(); (k != value.end()) && (isNumber == true); ++k)
        {
            if (std::isdigit(*k) == false)
            {
                isNumber = false;
            }
        }
        return isNumber;
    }
    unsigned int
    Utilities::GetIntegerDateInternalFilename(const std::string & filename)
    {
        // ---------------------------------------------------------------------------------------------
        // 4.3 find automatically the date in the filename. Find 'number' and without character
        // to be sur, remove the extention for working with this example: .._TOTO_20150414.mha
        // revert loop and finds number
        // Used to detect the date for :
        // L2 EarthExplorer product              : ..../S4_TEST_S4C_L2VALD_AFAngola00D00B_20130223.HDR                          -> detects 20130223
        // L2 Private EarthExplorer LTC mha file : ...LTC.DBL.DIR/S4_TEST_S4C_LUTANX_L2VALD_CVERSAIL00D00B_20130402_LTC.DBL.mha -> detects 20130402
        // ---------------------------------------------------------------------------------------------
//        const ListOfStrings filenamekey = SplitFilename(itksys::SystemTools::GetFilenameWithoutExtension(filename), '_');
        const ListOfStrings filenamekey = SplitFilename(filename, '_', true /*without_extension*/);

        // Reverse find of first field that is a number
        ListOfStrings::const_reverse_iterator wh = std::find_if(filenamekey.rbegin(), filenamekey.rend(), IsANumber);
        if (wh == filenamekey.rend())
        {
            vnsStaticExceptionDataMacro(
                    "Utilities::GetIntegerDateInternalFilename: Impossible to find a number (with split '_' for the string " << filename
                            << "!")
        }

        const Tm inputDate = Date::GetTmFromDateYYYYMMDD(*wh);
        return Date::GetJulianDayAsInteger(inputDate);
    }

    std::string
    Utilities::ReplaceAllSubStrings(std::string subject, const std::string& search, const std::string& replace)
    {
        itksys::SystemTools::ReplaceString(subject, search, replace);
        return subject;
    }

    std::string
    Utilities::RenameString(const std::string & filename, const std::string & origin_key, const std::string & final_key)
    {
        // ---------------------------------------------------------------------------------------------
        return ReplaceAllSubStrings(filename, origin_key, final_key);
    }

    std::string
    Utilities::GetStringFormatedTime(const double& input_time)
    {
        // ---------------------------------------------------------------------------------------------
        std::ostringstream flux;
        // ---------------------------------------------------------------------------------------------
        // Set the precision
        flux.precision(2);
        // ---------------------------------------------------------------------------------------------
        // MS value
        if (input_time < 1.)
        {
            flux << (input_time * 1000.) << " MS";
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // Second
            if ((input_time / 60.) < 1.)
            {
                flux << input_time << " second";
            }
            else
            {
                // ---------------------------------------------------------------------------------------------
                // Minutes
                flux << input_time / 60. << " minutes";
            }
        }
        // ---------------------------------------------------------------------------------------------
        return flux.str();
    }

    std::string
    Utilities::GetStringFormatedMemory(const double& input_memory)
    {
        // ---------------------------------------------------------------------------------------------
        std::ostringstream flux;
        double l_memory(input_memory);

        // ---------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------
        if ((l_memory / 1024.) < 1.)
        {
            // ---------------------------------------------------------------------------------------------
            // Kb
            flux << static_cast<unsigned int>(l_memory) << " o";
        }
        // ---------------------------------------------------------------------------------------------
        else
        {
            // ---------------------------------------------------------------------------------------------
            l_memory /= 1024.;
            // ---------------------------------------------------------------------------------------------
            if ((l_memory / 1024.) < 1.)
            {
                // ---------------------------------------------------------------------------------------------
                // Mb
                flux << static_cast<unsigned int>(l_memory) << " K";
            }
            // ---------------------------------------------------------------------------------------------
            else
            {
                // ---------------------------------------------------------------------------------------------
                l_memory /= 1024.;
                // ---------------------------------------------------------------------------------------------
                if ((l_memory / 1024.) < 1.)
                {
                    // ---------------------------------------------------------------------------------------------
                    // Mb
                    flux << static_cast<unsigned int>(l_memory) << " M";
                }
                // ---------------------------------------------------------------------------------------------
                else
                {
                    // ---------------------------------------------------------------------------------------------
                    // Set the precision
                    flux.precision(1);
                    l_memory /= 1024.;
                    flux << l_memory << " G";
                }
            }
        }
        // ---------------------------------------------------------------------------------------------
        return flux.str();
    }

    /** Get formatted performance memory for logger messages */
    std::string
    Utilities::GetStringFormatedSpaceDisk(const unsigned long& p_size)
    {
        return GetStringFormatedMemory(static_cast<double>(p_size));
    }

    bool
    Utilities::AsBool(const std::string & str)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert a boolean from the format string to the format boolean
        // tacking into account different syntaxes : yes -> true no -> false ...
        // ---------------------------------------------------------------------------------------------
        const std::string lLowerCaseString = itksys::SystemTools::LowerCase(str);

        if (lLowerCaseString.empty() == true)
        {
            return false;
        }
        // TRUE
        if ((lLowerCaseString == "true") || (lLowerCaseString == "yes") || (lLowerCaseString == "y") || (lLowerCaseString == "t")
                || (lLowerCaseString == "1"))
        {
            return true;
        }
        // FALSE
        else if ((lLowerCaseString == "false") || (lLowerCaseString == "no") || (lLowerCaseString == "n") || (lLowerCaseString == "f")
                || (lLowerCaseString == "0"))
        {
            return false;
        }
        // Integer
        else if (AsInt(lLowerCaseString))
        {
            return true;
        }
        else
        {
            return false;
        }

    }

//convert a boolean to a string.
    std::string
    Utilities::BoolToString(const bool & boolean)
    {
        // ---------------------------------------------------------------------------------------------
        // Returns the string "TRUE" if the boolean is true
        if (boolean == true)
        {
            return std::string("TRUE");
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the string "FALSE" in other case
        else
        {
            return std::string("FALSE");
        }
    }
    std::string
    Utilities::BoolToLowerString(const bool & boolean)
    {
        // ---------------------------------------------------------------------------------------------
        // Returns the string "true" if the boolean is true
        if (boolean == true)
        {
            return std::string("true");
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the string "false" in other case
        else
        {
            return std::string("false");
        }
    }

    std::string
    Utilities::DoubleToString(const double & value, const char * const format)
    {
        // ---------------------------------------------------------------------------------------------
        char char_value[30];
        const int code_return = std::snprintf(char_value, sizeof(char_value), format, value);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while building string from double value with snprinf function.")
        }

        return std::string(char_value);
    }

    std::string
    Utilities::AsString(const unsigned int & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        std::string lstring("");
        oss << value;
        oss >> lstring;
        return lstring;
    }

    std::string
    Utilities::AsString(const unsigned long & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        std::string lstring("");
        oss << value;
        oss >> lstring;
        return lstring;
    }

    std::string
    Utilities::AsString(const int & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        std::string lstring("");
        oss << value;
        oss >> lstring;
        return lstring;
    }

    unsigned char
    Utilities::AsUnsignedChar(const std::string & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        unsigned char lint(0);
        oss << value;
        oss >> lint;
        return lint;
    }

    int
    Utilities::AsInt(const std::string & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        int lint(0);
        oss << value;
        oss >> lint;
        return lint;
    }
    unsigned int
    Utilities::AsUnsignedInt(const std::string & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        unsigned int lint(0);
        oss << value;
        oss >> lint;
        return lint;
    }
    long
    Utilities::AsLong(const std::string & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        long lint(0);
        oss << value;
        oss >> lint;
        return lint;
    }
    unsigned long
    Utilities::AsUnsignedLong(const std::string & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        unsigned long lint(0);
        oss << value;
        oss >> lint;
        return lint;
    }
    float
    Utilities::AsFloat(const std::string & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        float lint(0.);
        oss << value;
        oss >> lint;
        return lint;
    }
    double
    Utilities::AsDouble(const std::string & value)
    {
        // ---------------------------------------------------------------------------------------------
        // Convert the band index integer value in string
        std::stringstream oss;
        double lint(0.);
        oss << value;
        oss >> lint;
        return lint;
    }

    std::string
    Utilities::GetFormatedSite(const std::string & site)
    {
        // ---------------------------------------------------------------------------------------------
        // Creation of a string with 8 characters (from VENUS mission)
        std::string site_final = "________";
        const unsigned int l_size_site_final(site_final.size());
        const unsigned int l_size(site.size());
        // ---------------------------------------------------------------------------------------------
        // Copy each character in the final site string from the input string
        if (l_size < l_size_site_final)
        {
            for (unsigned int i = 0; i < l_size; i++)
            {
                site_final[i] = site[i];
            }
            // ---------------------------------------------------------------------------------------------
            // Return the site formatted with 8 characters
            return site_final;
        }
        else
        {
            return site;
        }
    }

    Utilities::AOTMethodEnumType
    Utilities::GetAOTMethod(const std::string & method)
    {
        // ---------------------------------------------------------------------------------------------
        Utilities::AOTMethodEnumType aotMethod(AOT_NOT_INITIALIZED);

        // ---------------------------------------------------------------------------------------------
        // Convert the AOT estimation metho "string value" in the enumerate value
        // ---------------------------------------------------------------------------------------------
        // Multi T user case
        if (method == "MULTITEMPORAL")
        {
            aotMethod = MULTITEMPORAL;
        }
        // ---------------------------------------------------------------------------------------------
        // Multi S user case
        else if (method == "MULTISPECTRAL")
        {
            aotMethod = MULTISPECTRAL;
        }
        // ---------------------------------------------------------------------------------------------
        // Spectra Tempo user case
        else if (method == "SPECTROTEMPORAL")
        {
            aotMethod = SPECTROTEMPORAL;
        }
        // ---------------------------------------------------------------------------------------------
        // Error
        else
        {
            vnsStaticExceptionDataMacro("Utilities:GetAOTMethod: The AOT method " << method << "does not exist.")
        }
        return aotMethod;
    }

    std::string
    Utilities::GetEnv(const std::string& envVarStr)
    {
        // ---------------------------------------------------------------------------------------------
        // Get the var env value
        std::string l_string("");
        const bool l_exist = itksys::SystemTools::GetEnv(envVarStr, l_string);
        if (l_exist == false)
        {
            vnsStaticExceptionDataMacro("Utilities:GetEnv: Error while reading the varenv '" << envVarStr << "' !")
        }
        return l_string;
    }

    void
    Utilities::SetEnv(const std::string& envVarName, const std::string& envVarValue)
    {
        // ---------------------------------------------------------------------------------------------
        // Get the var env value
        const bool l_code_return = itksys::SystemTools::PutEnv(envVarName + "=" + envVarValue);
        if (l_code_return == false)
        {
            vnsStaticExceptionDataMacro(
                    "Utilities:SetEnv: Error while setting the varenv '" << envVarName << "' with the value '" << envVarValue << "' !")
        }

        const std::string l_checkvalue = GetEnv(envVarName);
        if (l_checkvalue != envVarValue)
        {
            vnsStaticExceptionDataMacro(
                    "Utilities:SetEnv: Error while checking the varenv '" << envVarName << "' with the value '" << envVarValue << "' !")
        }
    }

    std::string
    Utilities::ExpandVarEnv(const std::string& path)
    {
        //TODO : check with S. DINOT. Example as source code : http://stackoverflow.com/questions/1902681/expand-file-names-that-have-environment-variables-in-their-path
        const std::string s(path);
        // Find '${' characters
        if (s.find("${") == std::string::npos)
        {
            // If not found, exit
            return s;
        }
        // Find other '${'
        std::string pre = s.substr(0, s.find("${"));
        std::string post = s.substr(s.find("${") + 2);

        // Find the post '}' character
        if (post.find('}') == std::string::npos)
        {
            return s;
        }
        // Get the name of the variable env
        const std::string variable = post.substr(0, post.find('}'));
        std::string value = "";

        post = post.substr(post.find('}') + 1);

        // Expand the value
        const bool l_exist = itksys::SystemTools::GetEnv(variable, value);
        if (l_exist == false)
        {
            vnsLogWarningMacro("Impossible to expand the variable '" << variable << "' from the '" << path << "' path!")
        }
        // Call recursivly this function !
        return ExpandVarEnv(pre + value + post);
    }

    std::string
    Utilities::Expand(const std::string& path)
    {
        // ---------------------------------------------------------------------------------------------
        // Expand a filename
        std::string l_String(path);
        itksys::SystemTools::ConvertToUnixSlashes(l_String);
        return ExpandVarEnv(l_String);
    }

    std::string
    Utilities::GetFilenameWithoutRootDirectory(const std::string& rootDirectory, const std::string& filename)
    {
        // ---------------------------------------------------------------------------------------------
        // remove the prefix of a full path
        std::string l_filename(filename);
        itksys::SystemTools::ReplaceString(l_filename, rootDirectory, "");
        // ---------------------------------------------------------------------------------------------
        // returns the "postfix" value
        return l_filename;
    }

    std::string
    Utilities::GetFilenameWithoutNumbersOfPrefixSubDirectory(const std::string& filename, const unsigned int nb_sub_directory)
    {
        // ---------------------------------------------------------------------------------------------
        // remove the prefix of a full path
        std::string l_filename(filename);
        std::string l_PrefixRoot(filename);
        for (unsigned int i = 0; i < nb_sub_directory; i++)
        {
            l_PrefixRoot = itksys::SystemTools::GetFilenamePath(l_PrefixRoot);
        }
        itksys::SystemTools::ReplaceString(l_filename, l_PrefixRoot, "");
        // ---------------------------------------------------------------------------------------------
        // returns the "postfix" value
        return l_filename;
    }

    std::string
    Utilities::SetExtension(const std::string& filename, const std::string& new_extension, const bool full_path)
    {
        // ---------------------------------------------------------------------------------------------
        std::string l_String = itksys::SystemTools::GetFilenameWithoutExtension(filename) + new_extension;
        if (full_path == true)
        {
            l_String = itksys::SystemTools::GetFilenamePath(filename) + "/" + l_String;
        }
        return l_String;
    }

    bool
    Utilities::IsAnImageFilenameByCheckingExtension(const std::string & filename)
    {
        const std::string extension = itksys::SystemTools::LowerCase(itksys::SystemTools::GetFilenameExtension(filename));
        if ((extension == ".tif") || (extension == ".tiff") || (extension == ".hdr") || (extension == ".img") || (extension == ".png")
                || (extension == ".jpg") || (extension == ".jpeg") || (extension == ".jp2") || (extension == ".j2k")
                || (extension == ".jp2000") || (extension == ".jpeg2000"))
        {
            return true;
        }
        return false;
    }

    Utilities::ListOfStrings
    Utilities::SplitString(const std::string& string, char separator, bool remove_empty)
    {
        ListOfStrings filenamekey;
        // ---------------------------------------------------------------------------------------------
        // Splits filename with 'separator' value
        itksys::SystemTools::Split(string, filenamekey, separator);
        ListOfStrings filenamekeyFormated;
        if (remove_empty == false)
        {
            return filenamekey;
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // Loops on the values
            const unsigned int NbSplit = filenamekey.size();
            for (unsigned int i = 0; i < NbSplit; i++)
            {
                if (!filenamekey[i].empty())
                {
                    filenamekeyFormated.push_back(filenamekey[i]);
                }
            }
            // ---------------------------------------------------------------------------------------------
            return filenamekeyFormated;
        }
    }

    Utilities::ListOfDoubles
    Utilities::SplitStringAsDouble(const std::string& string, char separator, bool remove_empty)
    {
        const ListOfStrings l_ListOfString = SplitString(string, separator, remove_empty);
        const unsigned int l_ListOfStringSize = l_ListOfString.size();
        ListOfDoubles l_ListOfDouble;
        for (unsigned int i = 0; i < l_ListOfStringSize; i++)
        {
            l_ListOfDouble.push_back(vns::Utilities::AsDouble(l_ListOfString[i]));
        }
        return l_ListOfDouble;
    }

    Utilities::ListOfUIntegers
    Utilities::SplitStringAsUnsigned(const std::string& string, char separator, bool remove_empty)
    {
        const ListOfStrings l_ListOfString = SplitString(string, separator, remove_empty);
        const unsigned int l_ListOfStringSize = l_ListOfString.size();
        ListOfUIntegers l_ListOfUnsigned;
        for (unsigned int i = 0; i < l_ListOfStringSize; i++)
        {
            l_ListOfUnsigned.push_back(vns::Utilities::AsUnsignedInt(l_ListOfString[i]));
        }
        return l_ListOfUnsigned;
    }


    Utilities::ListOfStrings
    Utilities::SplitFilename(const std::string& filename, const char separator, const bool without_extension)
    {
        // ---------------------------------------------------------------------------------------------
        // Multi T user case
        //If filename is VE_VM0x_VSC_PDTANX_L2VALD_PARIS____20120101_ATB_R1.HDR
        // find ATB. Attention site is short name, not formated in 8 characters.
        std::string l_refFileName = itksys::SystemTools::GetFilenameName(filename);
        if (without_extension == true)
        {
            l_refFileName = itksys::SystemTools::GetFilenameWithoutExtension(l_refFileName);
        }
        // ---------------------------------------------------------------------------------------------
        // Splits filename with 'separator' value
        return SplitString(l_refFileName, separator, true /* bool remove_empty */);
    }

    Utilities::ListOfUIntegers
	Utilities::StringListAsUnsigned(const ListOfStrings& list)
    {
    	ListOfUIntegers l_outlist;
    	ListOfStrings::const_iterator itConst = list.begin();
    	for (;itConst != list.end(); ++itConst)
    	{
    		l_outlist.push_back(AsUnsignedInt(*itConst));
    	}
    	return l_outlist;
    }

    Utilities::ListOfDoubles
	Utilities::StringListAsDouble(const ListOfStrings& list)
        {
        	ListOfDoubles l_outlist;
        	ListOfStrings::const_iterator itConst = list.begin();
        	for (;itConst != list.end(); ++itConst)
        	{
        		l_outlist.push_back(AsDouble(*itConst));
        	}
        	return l_outlist;
        }

    Utilities::ListOfUInt8
	Utilities::StringListAsUnsignedChar(const ListOfStrings& list)
    {
    	ListOfUInt8 l_outlist;
    	ListOfStrings::const_iterator itConst = list.begin();
    	for (;itConst != list.end(); ++itConst)
    	{
    		l_outlist.push_back(AsUnsignedChar(*itConst));
    	}
    	return l_outlist;
    }

    std::string
    Utilities::JoinFilenames(const std::string& filename1, const std::string& filename2)
    {
        std::string relativePath(filename1);
        const std::string relativeName(filename2);
        if ((relativePath.size() && relativePath[relativePath.size() - 1] != '/') && (relativeName.size() && relativeName[0] != '/'))
        {
            relativePath += "/";
        }
        return relativePath + filename2;
    }

    std::string
    Utilities::GetVersionFormatted_XXYY()
    {
        // ---------------------------------------------------------------------------------------------
        char chaine[50];
        // ---------------------------------------------------------------------------------------------
        // Formated version in XX.YY string value
        const int code_return = std::snprintf(chaine, sizeof(chaine), VERSION_FORMATTED_XX_YY, MAJA_VERSION_MAJOR, MAJA_VERSION_MINOR);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while building string from version of MACCS with snprinf function.")
        }
        return std::string(chaine);
    }

    std::string
    Utilities::GetExtendedFilenameWriteImageFileJpeg()
    {
        // Compression quality for the JPG image, generated with otbImageFileWriter
        // Quality used with OTB gdal Extended filename
        // 95 : value for non regression before OTB 3.13 and OTB 3.20 which used GDALImageIO driver for JPEG (and not ITK JPEGImageIO)
        // disable writing the .geom file
        return "?&gdal:co:QUALITY=95&writegeom=false";
    }

    std::string
    Utilities::GetExtendedFilenameWriteImageFileStandard()
    {
        // Return the classic extend filename for all image file write in a L2/L3 product
        return "?&writegeom=false";
    }

    std::string
    Utilities::GetExtendedFilenameWriteMaskFileMuscate()
    {
        // Return the classic extend filename for all image file write in a L2/L3 product
        return "?&gdal:co:COMPRESS=DEFLATE&writegeom=false";
    }

    /** Initialize the output file class */
    std::string
    Utilities::GetOutputFileClass(const bool & p_test_mode, const std::string & product_file_class)
    {
        // Force the FileClass to TESTING mode
        if (p_test_mode == true)
        {
            return "TEST";
        }        // Else copy the input product file class to the output product file class
        else
        {
            return product_file_class; // Use only if Test is false.
        }
    }
    /** Get a string containing the value, if not return empty string */
    std::string
    Utilities::GetStringContainingValueFromListOfValues(const ListOfStrings & vector, const std::string & to_search)
    {
        std::string l_ret("");
        std::string l_norms = to_search;
        std::transform(l_norms.begin(), l_norms.end(), l_norms.begin(), ::tolower);
        ListOfStrings::const_iterator itConst = vector.begin();
        for (;itConst != vector.end(); ++itConst)
        {
            std::string l_tmp = *itConst;
            std::transform(l_tmp.begin(), l_tmp.end(), l_tmp.begin(), ::tolower);
            if ( l_tmp.find(l_norms) != std::string::npos)
            {
                return *itConst;
            }
        }
        return l_ret;
    }

} // End namespace vns
