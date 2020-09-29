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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 11 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Correction du code            *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 13 mai 2016 : Ajout calcul somme et moyenne sur liste     *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-7-0 : FA : LAIG-FA-MAC-1513-CNES : 1 decembre 2015 : SetEnv pour corriger PB NbThreads > 100 *
 * VERSION : 4-6-3 : FA : LAIG-FA-MAC-1504-CNES : 23 novembre 2015 : Erreur à la lecteur d'un produit L1    *
 *                                                                                                 S2 GPP   *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-119935-CS : 24 avril 2014 : Ajout Extendfilename pour ecriture images *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120000-CS : 20 mars 2014 : Methode de conversion IntToString ajoutee  *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-119924-CS : 19 mars 2014 : Fonction formattage Version                *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 10 octobre 2013 : Modification pour les TU                *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-355-CNES : 11 juin 2012 : Correction Header S2, Validity start/stop   *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-318-CNES : 30 mai 2012 : L3 Formosat, erreur sur l'utilisation d'un   *
 *                                                             indice dans une map.                         *
 * VERSION : 1-0-0-3 : DM : 310 : 14 septembre 2011 : Gestion d'un liste de donnees meteo.                    *
 * VERSION : 1-0-0-3 : FA : 315 : 8 septembre 2011 : Correction sur le "site" qui doit etre sur 8 caracteres  *
 * VERSION : 1-0-0-1 : FA : 214 : 17 mai 2011 : Correction sur la periode de validite du produit L3             *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 28 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsUtilities_h
#define __vnsUtilities_h

#include <cstdio>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <numeric>
#include "vnsMacro.h"

namespace vns
{
#define DECIMAL_1_FORMAT "%.1f"
#define DECIMAL_2_FORMAT "%.2f"
#define DECIMAL_3_FORMAT "%.3f"
#define DECIMAL_4_FORMAT "%.4f"
#define DECIMAL_6_FORMAT "%.6f"
#define DECIMAL_9_FORMAT "%.9f"
#define DECIMAL_10_FORMAT "%.10g"
#define DECIMAL_12_FORMAT "%.12g"
#define DECIMAL_15_FORMAT "%.15g"
#define DEFAULT_DOUBLE_FORMAT "%.10g"
#define CHECKTOOL_DOUBLE_FORMAT "%.15f" //10 digits after the coma
#define VERSION_FORMATTED_XX_YY "%02d.%02d"

    /** \class  Utilities
     * \brief This class provides some basics functionnalities.
     *
     * For example, provide filename functionnalities, ...
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
    class Utilities
    {
    public:
        /** Constructor */
        Utilities();
        /** Destructor */
        virtual
        ~Utilities();

        /** Typedef basic for return values */
        typedef std::set<std::string> SetOfStrings;
        typedef std::vector<std::string> ListOfStrings;
        typedef std::vector<ListOfStrings> ListOfListOfStrings;
        typedef std::vector<std::string> ListOfFilenames;
        typedef std::vector<float> ListOfFloats;
        typedef std::vector<ListOfFloats> ListOfListOfFloats;
        typedef std::vector<double> ListOfDoubles;
        typedef std::vector<ListOfDoubles> ListOfListOfDoubles;
        typedef std::vector<int> ListOfIntegers;
        typedef std::vector<ListOfIntegers> ListOfListOfIntegers;
        typedef std::vector<unsigned int> ListOfUIntegers;
        typedef std::vector<ListOfUIntegers> ListOfListOfUIntegers;
        typedef std::vector<unsigned char> ListOfUInt8;

        /** AOT method */
        typedef enum
        {
            MULTITEMPORAL = 0, MULTISPECTRAL = 1, SPECTROTEMPORAL = 2, AOT_NOT_INITIALIZED = 3
        } AOTMethodEnumType;

        // Plugin information : GIPPFileType
        typedef class GIPPFile
        {
        public:
            std::string Nature;
            std::string FileLocation;
            std::string LogicalName;
        } GIPPFileType;
        typedef std::vector<GIPPFileType> ListOfGIPPFilesType;

        // Avoid duplication of the same product
        static ListOfStrings
        RemovingDuplicationFilenames(const ListOfFilenames & listOfFilename);
        static void
        RelativeToAbsoluteFilename(const std::string & path, ListOfFilenames & listOfFilename);
        static void
        CheckingExistenceFilenames(const ListOfFilenames & listOfFilename);
        static void
        CheckingExistenceFilename(const std::string & Filename);
        static void
        CheckingExistenceDirectory(const std::string & Filename);
        static std::string
        ProvideFilename(const std::string & directory, const std::string & baseFilename = std::string("default_filename"),
                const std::string & extentionFilename = std::string("tif"));
        static std::string
        ProvideCachingFilename(const std::string & directory, const std::string & baseFilename = std::string("default_filename"),
                const std::string & extentionFilename = std::string("tif"));
        static std::string
        ProvideCrocodileclipFilename(const std::string & directory, const std::string & baseFilename, const bool & overwriteOutputFilename);

        /** Return the satellite FileTpe from the ExarthExplorer filname (ex: VE_TEST_VSC_L2VALD_CAMARGUE_20120103.HDR => VSC) */
        static std::string
        ExtractFileTypeFromEarthExplorerFilename(const std::string & filename, const unsigned int & position_of_file_type = 2);

        static ListOfStrings
        GetListOfUsedProductsId(const ListOfFilenames & listOfFilename);

        typedef std::map<unsigned int, unsigned int> MapIndexesType;

        static bool
        IsANumber(const std::string & value);

        static unsigned int
        GetIntegerDateInternalFilename(const std::string & filename);
        static std::string
        RenameString(const std::string & filename, const std::string & origin_key = "VALD", const std::string & final_key = "NOTV");

        static std::string
        ReplaceAllSubStrings(std::string subject, const std::string& search, const std::string& replace);

        /** Get formatted performance time for logger messages */
        static std::string
        GetStringFormatedTime(const double& input_time);

        /** Get formatted performance memory for logger messages */
        static std::string
        GetStringFormatedMemory(const double& input_memory);

        /** Get formatted performance memory for logger messages */
        static std::string
        GetStringFormatedSpaceDisk(const unsigned long& p_size);

        static bool
        AsBool(const std::string & str);

        static std::string
        BoolToString(const bool & boolean);
        static std::string
        BoolToLowerString(const bool & boolean);

        static std::string
        DoubleToString(const double & value, const char * const format = DECIMAL_9_FORMAT);

        static std::string
        AsString(const int & value);

        static std::string
        AsString(const unsigned int & value);

        static std::string
        AsString(const unsigned long & value);

        static unsigned char
		AsUnsignedChar(const std::string & value);

        static int
        AsInt(const std::string & value);

        static unsigned int
        AsUnsignedInt(const std::string & value);

        static long
        AsLong(const std::string & value);

        static unsigned long
        AsUnsignedLong(const std::string & value);

        static float
        AsFloat(const std::string & value);

        static double
        AsDouble(const std::string & value);

        static std::string
        GetFormatedSite(const std::string& site);

        static AOTMethodEnumType
        GetAOTMethod(const std::string & method);

        static std::string
        GetEnv(const std::string& envVarStr);

        static void
        SetEnv(const std::string& envVarName, const std::string& envVarValue);

        static std::string
        GetFilenameWithoutRootDirectory(const std::string& rootDirectory, const std::string& filename);

        static std::string
        GetFilenameWithoutNumbersOfPrefixSubDirectory(const std::string& filename, const unsigned int nb_sub_directory);

        static std::string
        Expand(const std::string& path);

        static std::string
        SetExtension(const std::string& filename, const std::string& new_extension, const bool full_path = true);

        static bool
        IsAnImageFilenameByCheckingExtension(const std::string & filename);

        static ListOfStrings
        SplitString(const std::string& filename, char separator = ' ', bool remove_empty = false);

        static ListOfDoubles
        SplitStringAsDouble(const std::string& filename, char separator = ' ', bool remove_empty = false);

        static ListOfUIntegers
		SplitStringAsUnsigned(const std::string& filename, char separator = ' ', bool remove_empty = false);

        static ListOfStrings
        SplitFilename(const std::string& filename, char separator = '_', bool without_extension = true);

        static ListOfUIntegers
		StringListAsUnsigned(const ListOfStrings& list);

        static ListOfDoubles
		StringListAsDouble(const ListOfStrings& list);

        static ListOfUInt8
		StringListAsUnsignedChar(const ListOfStrings& list);

        static std::string
        JoinFilenames(const std::string& filename1, const std::string& filename2);

        static std::string
        GetVersionFormatted_XXYY();

        static std::string
        GetExtendedFilenameWriteImageFileJpeg();
        static std::string
        GetExtendedFilenameWriteImageFileStandard();
        static std::string
        GetExtendedFilenameWriteMaskFileMuscate();

        static std::string
        GetStringContainingValueFromListOfValues(const ListOfStrings & vector, const std::string & to_search);

        template<class T>
        static double
        GetSumOfListOfValues(T & v)
        {
            return std::accumulate(v.begin(), v.end(), 0.);
        }
        template<class T>
        static double
        GetMeanOfListOfValues(const T & v)
        {
            if (v.empty() == true)
            {
                vnsStaticExceptionDataMacro(
                        "Utilities::GetMeanOfListOfValues: Impossible to compute the mean of a list when the list is empty !!")
            }
            return (GetSumOfListOfValues(v) / v.size());
        }

        static std::string
        GetOutputFileClass(const bool & p_test_mode, const std::string & product_file_class);

        template<class T>
        static std::string
        GetStringFromListOfValues(const T & vector, const unsigned int nb_values, const std::string & separator = " ")
        {
            std::string l_String("");
            if (nb_values > 0)
            {
                for (unsigned int i = 0; i < nb_values - 1; i++)
                {
                    std::ostringstream oss;
                    oss << vector[i];
                    l_String = l_String + oss.str() + separator;
                }
                std::ostringstream oss;
                oss << vector[nb_values - 1];
                l_String = l_String + oss.str().c_str();

            }
            return l_String;
        }
        template<class T>
        static std::string
        GetStringFromListOfValuesFormat(const T & vector, const unsigned int & nb_values, const std::string & format,
                const std::string & separator = " ")
        {
            std::string l_String("");
            if (nb_values > 0)
            {
                char char_value[30];
                for (unsigned int i = 0; i < nb_values - 1; i++)
                {
                    const int code_return = std::snprintf(char_value, sizeof(char_value), format.c_str(), vector[i]);
                    if (code_return < 0)
                    {
                        vnsStaticExceptionDataMacro("Internal error while getting string value from list of values with snprintf function.")
                    }
                    l_String += char_value;
                    l_String += separator;
                }
                const int code_return = std::snprintf(char_value, sizeof(char_value), format.c_str(), vector[nb_values - 1]);
                if (code_return < 0)
                {
                    vnsStaticExceptionDataMacro("Internal error while getting string value from list of values with snprintf function.")
                }
                l_String += char_value;
            }
            return l_String;
        }
        template<class T>
        static std::string
        GetStringFromSdtListOfValues(const T & vector, const std::string & separator = " ")
        {
            return GetStringFromListOfValues(vector, vector.size(), separator);
        }
        template<class T>
        static std::string
        GetStringFromVariableLengthVectorListOfValues(const T & vector, const std::string & format, const std::string & separator = " ")
        {
            return GetStringFromListOfValuesFormat(vector, vector.Size(), format, separator);
        }

        template<class T>
        static void
        PrintVector(const std::string & comment, const T & vector, std::ostream& os)
        {
            const unsigned int nb_values = vector.size();
            os << "----------------------------------------------------" << std::endl;
            os << comment << " with '" << nb_values << "' :"; // << std::endl;
            for (unsigned int i = 0; i < nb_values; i++)
            {
                os << vector[i] << " ";
            }
            os << std::endl;
        }
        template<class T>
        static void
        PrintList(const std::string & comment, const T & list, std::ostream& os)
        {
            const std::string l_message = Loggers::PrintList(comment, list);
            os << l_message;
        }
        template<class T>
        static void
        PrintList(const std::string & comment, T & list, std::ostream& os)
        {
            const std::string l_message = Loggers::PrintList(comment, list);
            os << l_message;
        }
        template<class T>
        static void
        PrintMap(const std::string & comment, T & p_map, std::ostream& os)
        {
            const std::string l_message = Loggers::PrintMap(comment, p_map);
            os << l_message;
        }
        template<class T>
        static void
        PrintMap(const std::string & comment, const T & p_map, std::ostream& os)
        {
            const std::string l_message = Loggers::PrintMap(comment, p_map);
            os << l_message;
        }

    private:

        static std::string
        ExpandVarEnv(const std::string& path);

    };

} // End namespace vns

// PrintSelf macro helper
#define vnsPrintSelfValueMacro(value) os << indent << #value << " : " <<  this->Get##value() << std::endl;
#define vnsPrintSelfValueOptionnalMacro(value) try { vnsPrintSelfValueMacro(value) } catch (...) { os << "Impossible to read the value for the node <this->Get"<<#value<<"(). But it's optional"<< std::endl;}
#define vnsPrintSelfUCharValue(value) os << indent <<  #value << " : " << (int)this->Get##value() << std::endl;
#define vnsPrintSelfParamMacro(value, index) os << indent << #value << " : " << this->Get##value( index ) << std::endl;

#define vnsPrintSelfListOfValuesMacro(value) vns::Utilities::PrintList(#value, this->Get##value(), os);



#endif /* __vnsUtilities_h */
