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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 12 octobre 2016 : Audit code - Correction qualite :       *
 *                                                               Regle : Missing parameter name in function *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Correction du code            *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 21 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-0 : DM : LAIG-DM-MAC-1283-CNES : 19 juin 2015 : Creation plug-in Sentinel2 natif           *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 10 octobre 2013 : Ajout LOAD_TU_CONTEXT_WITH_INSTALL_MAJA_DIR *
 * VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Prise en compte de la DM incluant notamment   *
 *                                                            le MACCS generique (PM LAIG-NT-MAC-0239-CNES) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 juillet 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef ____vnsDataManagerTestingLibrary_h___
#define ____vnsDataManagerTestingLibrary_h___

#include "vnsUtilities.h"
#include "vnsLoggerBase.h"
#include "itkTimeProbe.h"
#include "vnsMath.h"
#include <map>

namespace vns
{
    class DMTesting
    {
    public:
        typedef std::map<std::string, std::string> ListOfRegisterFileType;

        static std::string GET_FONT_FILENAME();
        static std::string
        GET_PLUGIN_NAME(const std::string & id_camera);

        static void
        DATA_APPLICATION_HANDLER_LOAD(const std::string & string_arguments);

        static std::string
        LOAD_CONTEXT_MAJA_LOAD_GET_ADDITIONAL_COMMAND_LINE_PARAMETERS();

        static void
        LOAD_TU_CONTEXT(const std::string & p_key_camera, const std::string & temporary_directory, LoggerBase::LevelType level=LoggerBase::DEBUG);

        static void
        LOAD_TU_CONTEXT_WITH_INSTALL_MAJA_DIR(const std::string & p_key_camera, const std::string & temporary_directory, LoggerBase::LevelType level=LoggerBase::DEBUG);

        static void
        LOAD_TVA_CONTEXT(const std::string & satellite, const std::string & temporary_directory, LoggerBase::LevelType level=LoggerBase::DEBUG);
        static void /*JobOrderProviderBase::Pointer*/
        LOAD_OPERATIONAL_CONTEXT_WITH_JOBORDER(const std::string & joborderfilename, const std::string & temporary_directory,
        		const std::string & additionalParameters = "", LoggerBase::LevelType level=LoggerBase::DEBUG);
        static void /* JobOrderProviderBase::Pointer */
        LOAD_OPERATIONAL_CONTEXT( const std::string & satellite, const std::string & temporary_directory, const std::string & additionalParameters = "", LoggerBase::LevelType level=LoggerBase::DEBUG);
        static void /* JobOrderProviderBase::Pointer */
        LOAD_TESTING_CONTEXT_L3( const std::string & satellite, LoggerBase::LevelType level=LoggerBase::DEBUG, const std::string & p_additionals_parameters = "");
private:
        /** Define the possible exit value for processing */
        static const int m_TESTING_EXIT_SUCCES;
        static const int m_TESTING_EXIT_FAILURE_VNSEXCEPTIONCHAIN;
        static const int m_TESTING_EXIT_FAILURE_VNSEXCEPTIONBUSINESS;
        static const int m_TESTING_EXIT_FAILURE_VNSEXCEPTIONDATA;
        static const int m_TESTING_EXIT_FAILURE_VNSEXCEPTIONBASE;
        static const int m_TESTING_EXIT_FAILURE_ITKEXCEPTION;
        static const int m_TESTING_EXIT_FAILURE_STDBADALLOC;
        static const int m_TESTING_EXIT_FAILURE_STDEXCEPTION;
        static const int m_TESTING_EXIT_FAILURE_UNKNOWNED;


    };
} // namespace vns


#endif
