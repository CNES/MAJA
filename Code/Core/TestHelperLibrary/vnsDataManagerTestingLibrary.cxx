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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 6 mars 2017 : Refactoring pour ameliorer la qualite      *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 24 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Correction du code            *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-0 : DM : LAIG-DM-MAC-1283-CNES : 19 juin 2015 : Creation plug-in Sentinel2 natif           *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
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
#include "vnsDataManagerTestingLibrary.h"

#include "itksys/SystemTools.hxx"

#include "vnsMacro.h"
#include "vnsSystem.h"
#include "vnsUtilities.h"
#include "vnsConfigure.h"

#include "vnsLoggers.h"
#include "vnsL1ImageInformationsProviderBase.h"
#include "vnsL1ImageInformationsProviderFactory.h"
#include "vnsL2ImageFileReaderBase.h"
#include "vnsL2ImageFileReaderFactory.h"
#include "vnsL3ImageFileReader.h"
#include "vnsL3ImageFileReaderFactory.h"
#include "vnsPluginsActivation.h"
#include "vnsDataApplicationHandler.h"

#ifdef PLUGIN_VENUS
#include "vnsVenusPlugin.h"
#endif
#ifdef PLUGIN_FORMOSAT_MUSCATE_PROTO
#include "vnsFormosatMuscateProtoPlugin.h"
#endif
#ifdef PLUGIN_LANDSAT_MUSCATE_PROTO
#include "vnsLandsatMuscateProtoPlugin.h"
#endif
#ifdef PLUGIN_LANDSAT8_MUSCATE_PROTO
#include "vnsLandsat8MuscateProtoPlugin.h"
#endif
#ifdef PLUGIN_SPOT4_MUSCATE_PROTO
#include "vnsSpot4MuscateProtoPlugin.h"
#endif
#ifdef PLUGIN_SENTINEL2_GPP
#include "vnsSentinel2GppPlugin.h"
#endif
#ifdef PLUGIN_SENTINEL2
#include "vnsSentinel2Plugin.h"
#endif
#ifdef PLUGIN_SENTINEL2_MUSCATE
#include "vnsSentinel2MuscatePlugin.h"
#endif
#ifdef PLUGIN_LANDSAT_MUSCATE
#include "vnsLandsatMuscatePlugin.h"
#endif
#ifdef PLUGIN_LANDSAT8_MUSCATE
#include "vnsLandsat8MuscatePlugin.h"
#endif
#ifdef PLUGIN_LANDSAT8
#include "vnsLandsat8Plugin.h"
#endif

namespace vns
{
#define  vnsRunMethodTestingLibraryMacro(method) \
    try \
    {  \
      /** Invoke the processing method. */ \
      (method); \
    } \
    /** Catch vns::ExceptionChain */ \
    catch (ExceptionChain & e) \
    { \
        std::cout << e.GetDescription() << " ["<<itksys::SystemTools::GetFilenameName(e.GetFile()) << ":" << e.GetLine() << "] [MAJA Chain Exception]" <<std::endl; \
        exit( m_TESTING_EXIT_FAILURE_VNSEXCEPTIONCHAIN); \
    } \
    /** Catch vns::ExceptionBusiness */ \
    catch (ExceptionBusiness & e) \
    { \
        std::cout <<  e.GetDescription() << " ["<<itksys::SystemTools::GetFilenameName(e.GetFile()) << ":" << e.GetLine() << "] [MAJA Business Exception]" <<std::endl;  \
        exit( m_TESTING_EXIT_FAILURE_VNSEXCEPTIONBUSINESS); \
    } \
    /** Catch vns::ExceptionData */ \
    catch (ExceptionData & e) \
    { \
        std::cout << e.GetDescription() << " ["<<itksys::SystemTools::GetFilenameName(e.GetFile()) << ":" << e.GetLine() << "] [MAJA Data Exception]" <<std::endl;  \
        exit( m_TESTING_EXIT_FAILURE_VNSEXCEPTIONDATA); \
    } \
    /** Catch vns::ExceptionBase */ \
    catch (ExceptionBase & e) \
    { \
        std::cout <<  e.GetDescription() << " ["<<itksys::SystemTools::GetFilenameName(e.GetFile()) << ":" << e.GetLine() << "] [MAJA Base Exception]" <<std::endl;  \
        exit( m_TESTING_EXIT_FAILURE_VNSEXCEPTIONBASE); \
    } \
    /** Catch itk::ExceptionObject */ \
    catch ( itk::ExceptionObject & e ) \
    { \
        std::cout << e.GetDescription() << " ["<<itksys::SystemTools::GetFilenameName(e.GetFile()) << ":" << e.GetLine() << "] [ITK Exception]" <<std::endl;  \
        exit( m_TESTING_EXIT_FAILURE_ITKEXCEPTION); \
    } \
    /** Catch std::bad_alloc */ \
    catch ( std::bad_alloc & e ) \
    { \
        std::cout << "std::bad_alloc error: "<<(char*)e.what() <<std::endl;  \
        exit( m_TESTING_EXIT_FAILURE_STDBADALLOC); \
    } \
    /** Catch std::exception */ \
    catch (const std::exception& e) \
    { \
        std::cout << "std::exception error: "<<e.what() <<std::endl; \
        exit( m_TESTING_EXIT_FAILURE_STDEXCEPTION); \
    } \
    /** Catch alls other unknown exception */ \
    catch ( ... ) \
    { \
        std::cout << "Unknown exception thrown !" <<std::endl;  \
        exit( m_TESTING_EXIT_FAILURE_UNKNOWNED); \
    }

    /** Exit code definition */
    const int DMTesting::m_TESTING_EXIT_SUCCES = 0;
    const int DMTesting::m_TESTING_EXIT_FAILURE_VNSEXCEPTIONCHAIN = 135;
    const int DMTesting::m_TESTING_EXIT_FAILURE_VNSEXCEPTIONBUSINESS = 134;
    const int DMTesting::m_TESTING_EXIT_FAILURE_VNSEXCEPTIONDATA = 133;
    const int DMTesting::m_TESTING_EXIT_FAILURE_VNSEXCEPTIONBASE = 132;
    const int DMTesting::m_TESTING_EXIT_FAILURE_ITKEXCEPTION = 131;
    const int DMTesting::m_TESTING_EXIT_FAILURE_STDBADALLOC = 130;
    const int DMTesting::m_TESTING_EXIT_FAILURE_STDEXCEPTION = 129;
    const int DMTesting::m_TESTING_EXIT_FAILURE_UNKNOWNED = 128;

    std::string
    DMTesting::GET_FONT_FILENAME()
    {
        // ---------------------------------------------------------------------------------------------
	std::string l_RootInstallDirectoryOfMaja("");
        const bool l_exist = itksys::SystemTools::GetEnv(System::m_VarEnvRootInstallDirectoryOfMaja,
							 l_RootInstallDirectoryOfMaja);
        if (l_exist == false) {
         l_RootInstallDirectoryOfMaja = MAJA_INSTALL_DIR;
        }
        std::string l_FontFileName("");

        // ---------------------------------------------------------------------------------------------
        bool LAUNCH_TU_IN_BINARY_DIR = false;
        // ---------------------------------------------------------------------------------------------
        if (LAUNCH_TU_IN_BINARY_DIR == true)
        {
            l_FontFileName = l_RootInstallDirectoryOfMaja + "/Plugins/Common/Interfaces/Config/Fonts/amble/Amble-Italic.ttf";
        }
        else
        {
            l_FontFileName = l_RootInstallDirectoryOfMaja + "/share/config/Fonts/amble/Amble-Italic.ttf";
        }
        return l_FontFileName;
    }
    void
    DMTesting::LOAD_TU_CONTEXT_WITH_INSTALL_MAJA_DIR(const std::string & p_key_camera, const std::string & temporary_directory,
            LoggerBase::LevelType level)
    {
        LOAD_TU_CONTEXT(p_key_camera, temporary_directory, level);
    }
    std::string
    DMTesting::GET_PLUGIN_NAME(const std::string & id_camera)
    {
        // ---------------------------------------------------------------------------------------------
        // Load the camera dictionary associated to the processed camera
        std::cout << "DMTesting::GET_PLUGIN_NAME id_camera: " << id_camera << std::endl;
        if (0)
        {

        }
#ifdef PLUGIN_VENUS
        else if (id_camera.find("VENUS") != std::string::npos)
        {
            return VenusPlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_FORMOSAT_MUSCATE_PROTO
        else if (id_camera.find("FORMOSAT_MUSCATE_PROTO") != std::string::npos)
        {
            return FormosatMuscateProtoPlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_LANDSAT8_MUSCATE_PROTO
        else if (id_camera.find("LANDSAT8_MUSCATE_PROTO") != std::string::npos)
        {
            return Landsat8MuscateProtoPlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_LANDSAT_MUSCATE_PROTO
        else if (id_camera.find("LANDSAT_MUSCATE_PROTO") != std::string::npos)
        {
            return LandsatMuscateProtoPlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_SPOT4_MUSCATE_PROTO
        else if (id_camera.find("SPOT4_MUSCATE_PROTO") != std::string::npos)
        {
            return Spot4MuscateProtoPlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_SENTINEL2_GPP
        else if (id_camera.find("SENTINEL2_GPP") != std::string::npos)
        {
            return Sentinel2GppPlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_SENTINEL2_MUSCATE
        else if (id_camera.find("SENTINEL2_MUSCATE") != std::string::npos)
        {
            return Sentinel2MuscatePlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_SENTINEL2
        else if (id_camera.find("SENTINEL2") != std::string::npos)
        {
            return Sentinel2Plugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_LANDSAT8_MUSCATE
        else if (id_camera.find("LANDSAT8_MUSCATE") != std::string::npos)
        {
            return Landsat8MuscatePlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_LANDSAT_MUSCATE
        else if (id_camera.find("LANDSAT_MUSCATE") != std::string::npos)
        {
            return LandsatMuscatePlugin::GetInstance().GetPluginName();
        }
#endif
#ifdef PLUGIN_LANDSAT8
        else if (id_camera.find("LANDSAT8") != std::string::npos)
        {
            return Landsat8Plugin::GetInstance().GetPluginName();
        }
#endif
        else
        {
            std::cout << "id_camera : '" << id_camera << "'" << std::endl;
            std::cout << "Update DMTesting::GET_PLUGIN_NAME() method ?" << std::endl;
            return "SATELLITE_UNKNOWED";
        }
    }

    /** Load the application, for testing context, NO OPERATIONAL */
    void
    DMTesting::DATA_APPLICATION_HANDLER_LOAD(const std::string & string_arguments)
    {
        Utilities::ListOfStrings filenamekey;
        // ---------------------------------------------------------------------------------------------
        // Splits filename with 'separator' value
        itksys::SystemTools::Split(string_arguments.c_str(), filenamekey, ' ' /* space*/);
        Utilities::ListOfStrings filenamekeyFormated;
        // ---------------------------------------------------------------------------------------------
        // Loops on the values
        const unsigned int NbSplit = filenamekey.size();
        for (unsigned int i = 0; i < NbSplit; i++)
        {
            if (filenamekey[i] != "")
            {
                filenamekeyFormated.push_back(filenamekey[i]);
            }
        }
        // ---------------------------------------------------------------------------------------------
        const unsigned int NbParemeters = filenamekeyFormated.size();
        int argc(NbParemeters);
        char ** argv = new char*[argc];
        for (int i = 0; i < argc; i++)
        {
            argv[i] = new char[filenamekeyFormated[i].size() + 1];
            strcpy(argv[i], filenamekeyFormated[i].c_str());
        }
        vnsRunMethodTestingLibraryMacro(DataApplicationHandler::Load(argc, argv))

    }
    std::string
    DMTesting::LOAD_CONTEXT_MAJA_LOAD_GET_ADDITIONAL_COMMAND_LINE_PARAMETERS()
    {
        // ---------------------------------------------------------------------------------------------
        std::cout
                << "DMTesting: Use the VARENV 'MAJA_LOAD_ADD_PARAMETERS' to add command line parameters to initialize the context (launching ./maccs with default parameters), Ex: For enable crocodile_clip, set  MAJA_LOAD_ADD_PARAMETERS=--enableTest true"
                << std::endl;
        std::string l_string("");
        // ---------------------------------------------------------------------------------------------
        const bool l_exist = itksys::SystemTools::GetEnv("MAJA_LOAD_ADD_PARAMETERS", l_string);
        if (l_exist == true)
        {
            l_string = " " + l_string;
            std::cout << "DMTesting: Additional command line parameters (launching ./maccs) : " << l_string << std::endl;
        }
        // ---------------------------------------------------------------------------------------------
        else
        {
            l_string = "";
        }
        return l_string;
    }
    void
    DMTesting::LOAD_TU_CONTEXT(const std::string & p_key_camera, const std::string & temporary_directory, LoggerBase::LevelType /* level */)
    {
        // ---------------------------------------------------------------------------------------------
        // Register all plugins
        std::cout << "DMTesting::LOAD_TU_CONTEXT key_camera : '" << p_key_camera << "'" << std::endl;
        RegisterAllPlugins();
        // ---------------------------------------------------------------------------------------------
        DATA_APPLICATION_HANDLER_LOAD(
                "stub.exe --mode L2INIT --output " + temporary_directory + " --loglevel DEBUG"
                        + LOAD_CONTEXT_MAJA_LOAD_GET_ADDITIONAL_COMMAND_LINE_PARAMETERS());
        // ---------------------------------------------------------------------------------------------
        vnsRunMethodTestingLibraryMacro(
                DataApplicationHandler::GetMutableInstance().SetCurrentPluginInstance(GET_PLUGIN_NAME(p_key_camera)))
    }

    void
    DMTesting::LOAD_TVA_CONTEXT(const std::string & satellite /*jobOrderfilename*/, const std::string & temporary_directory,
            LoggerBase::LevelType /* level */)
    {
        // ---------------------------------------------------------------------------------------------
        // Register all plugins
        RegisterAllPlugins();
        // ---------------------------------------------------------------------------------------------
        DATA_APPLICATION_HANDLER_LOAD(
                "stub.exe --mode L2INIT --output " + temporary_directory + " --loglevel DEBUG"
                        + LOAD_CONTEXT_MAJA_LOAD_GET_ADDITIONAL_COMMAND_LINE_PARAMETERS());
        // ---------------------------------------------------------------------------------------------
        vnsRunMethodTestingLibraryMacro(
                DataApplicationHandler::GetMutableInstance().SetCurrentPluginInstance(GET_PLUGIN_NAME(satellite)))
        // ---------------------------------------------------------------------------------------------
        std::cout << "TestMode: " << Utilities::BoolToString(DataApplicationHandler::GetInstance().GetTestMode()) << std::endl;
    }
    // the parameter tileId is optional (null by default) and used only by the SENTINEL2 plug-in
    void /*JobOrderProviderBase::Pointer*/
    DMTesting::LOAD_OPERATIONAL_CONTEXT_WITH_JOBORDER(const std::string & joborderfilename, const std::string & temporary_directory,
            const std::string & additionalParameters, LoggerBase::LevelType /* level */)
    {
        // Register all plugins
        RegisterAllPlugins();
        DATA_APPLICATION_HANDLER_LOAD(
                "stub.exe --mode L2INIT --jobOrder " + joborderfilename + " --output " + temporary_directory + " --loglevel DEBUG "
                        + additionalParameters + LOAD_CONTEXT_MAJA_LOAD_GET_ADDITIONAL_COMMAND_LINE_PARAMETERS());
        vnsRunMethodTestingLibraryMacro(
                DataApplicationHandler::GetMutableInstance().SetCurrentPluginInstance(GET_PLUGIN_NAME(joborderfilename)))
    }
    void /*JobOrderProviderBase::Pointer*/
    DMTesting::LOAD_OPERATIONAL_CONTEXT(const std::string & satellite /*joborderfilename*/, const std::string & temporary_directory,
            const std::string & additionalParameters, LoggerBase::LevelType /* level */)
    {
        // Register all plugins
        RegisterAllPlugins();
        DATA_APPLICATION_HANDLER_LOAD(
                "stub.exe --mode L2INIT --output " + temporary_directory + " --loglevel DEBUG" + additionalParameters
                        + LOAD_CONTEXT_MAJA_LOAD_GET_ADDITIONAL_COMMAND_LINE_PARAMETERS());
        vnsRunMethodTestingLibraryMacro(
                DataApplicationHandler::GetMutableInstance().SetCurrentPluginInstance(GET_PLUGIN_NAME(satellite)))
    }

    void /* JobOrderProviderBase::Pointer */
    DMTesting::LOAD_TESTING_CONTEXT_L3(const std::string & satellite /*joborderfilename*/, LoggerBase::LevelType /* level */,
            const std::string & p_additionals_parameters)
    {
        // Register all plugins
        RegisterAllPlugins();
        DATA_APPLICATION_HANDLER_LOAD(
                "stub.exe --mode L3 --loglevel DEBUG " + p_additionals_parameters
                        + LOAD_CONTEXT_MAJA_LOAD_GET_ADDITIONAL_COMMAND_LINE_PARAMETERS());
        vnsRunMethodTestingLibraryMacro(
                DataApplicationHandler::GetMutableInstance().SetCurrentPluginInstance(GET_PLUGIN_NAME(satellite)))
    }


} // namespace vns

