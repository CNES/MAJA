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
 * VERSION : 2.1.0 : FA : LAIG-FA-MAJA-157185-CS : 9 mai 2017 : Correction de règles qualité                *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2240-CNES : 30 mars 2017 : Retrait de log à l'execution de commande  *
 * VERSION : 1.1.0 : FA : LAIG-FA-MAJA-152790-CS : 23 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-975-CNES : 13 decembre 2016 : Copie des detfootprint dans la sortie   *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 11 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 13 mai 2016 : Modification mineure                        *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-7-0 : FA : LAIG-FA-MAC-1475-CNES : 7 decembre 2015 : Correction produit NOTV qui contient    *
 *                                       encore des produits VALD dans l'arborescence du produit final NOTV *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 4-2-0 : FA : LAIG-FA-MAC-864-CNES : 11 mars 2015 : Modification method RemoveDirectory.        *
 *                                                              Force with rm if ITK method failed.         *
 * VERSION : 4-0-1 : FA : LAIG-FA-MAC-865-CNES : 11 juin 2014 : Decompression du produit L2 en entree       *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-748-CNES : 13 mars 2014 : Ajout d'un booléen pour supprimer le DBL    *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-354-CNES : 10 mars 2014 : Correction qualité sur règles de codage     *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
 * VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 28 octobre 2013 : Creation plugin LANDSAT                  *
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 10 octobre 2013 : Modification pour les TU                *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avr. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsSystem.h"

#include <sys/types.h>
#include <iostream>
#include "itksys/Process.h"
#include <itksys/SystemTools.hxx>
#include <itksys/Directory.hxx>
#include "itkTimeProbe.h"
#include "itkMacro.h"

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <fstream>

#include "vnsLoggers.h"
#include "vnsMacro.h"
#include "vnsConfigure.h"

namespace vns
{

    std::string System::m_HostName = "";
    std::string System::m_ProcessName = "";
    const std::string System::m_VarEnvRootInstallDirectoryOfMaja = "MAJA_INSTALL_DIR";

    // Returns the host name by calling system primitives
    std::string
    System::GetHostName()
    {
        // Check the validity of the hostname
        if (m_HostName.empty())
        {
            // Initialize the default hostname value
            m_HostName = "UNKNOWN_HOSTNAME";

            // Declar a char buffer
            char nameBuffer[255];
            // Read the hostname with basic function "gethostname"
            gethostname(nameBuffer, sizeof(nameBuffer));
            // Get this value in a std::string
            m_HostName = nameBuffer;
        }
        return m_HostName;
    }

    // Returns the process name by calling system primitives
    std::string
    System::GetProcessName()
    {
        // Check if empty
        if (m_ProcessName.empty())
        {
            // then find its name and update ProcessName
            std::string processName("UNKNOWN_PROCESSNAME");

            // Open the /proc/<proc_id>/cmdline file and read the command line
            int pid = GetPID();
            char proc_pid_cmdline_path[PATH_MAX];
            char cmdline[1024];

            // try to build the name of the process
            const int code_return = std::snprintf(proc_pid_cmdline_path, sizeof(proc_pid_cmdline_path), "/proc/%d/cmdline", pid);
            if (code_return < 0)
            {
                // Raise an exception
                vnsStaticExceptionDataMacro("Internal error while building the processor name with snprintf function.")
            }

            // retrieve command line
            FILE *proc_pid_cmdline = fopen(proc_pid_cmdline_path, "r");
            fgets(cmdline, sizeof(cmdline), proc_pid_cmdline);
            fclose(proc_pid_cmdline);

            // Ignore arguments by looking for one space
            // ex : "/toto/tata/process -a tutu -b titi"
            char* exePath = strtok(cmdline, " ");

            // Keep only exe name (not path)
            char* token = strtok(exePath, "/");
            while (token != 0)
            {
                processName = token;
                token = strtok(NULL, "/");
            }
            // update ProcessName
            m_ProcessName = processName;
        }
        // Return the process name
        return m_ProcessName;
    }

    // Returns the process ID by calling system primitives
    int
    System::GetPID()
    {
        return getpid();
    }

    System::ReturnStatus
    System::Rasterize(const std::string & inputFilename, const unsigned int xmin, const unsigned int ymin, const unsigned int xmax,
            const unsigned int ymax, const unsigned int size_x, const unsigned int size_y, const std::string & projection,
            const std::string & outputFilename, const std::string & gdalAdditionalCommandLineParameters)
    {
      // Call the gdal_rasterize command with the specific parameters
      return CallCommandBasic("gdal_rasterize " + gdalAdditionalCommandLineParameters + " -te " + Utilities::AsString(xmin) + " "
                        + Utilities::AsString(ymin) + " " + Utilities::AsString(xmax) + " " + Utilities::AsString(ymax) + " -ts "
                        + Utilities::AsString(size_x) + " " + Utilities::AsString(size_y) + " -a_srs '" + projection + "' " + inputFilename
                        + " " + outputFilename + " &> /dev/null");
    }

    void
    System::CompressDBLProduct(const std::string & DBLFilename, const bool clean_dir_after_compressed)
    {
        // ---------------------------------------------------------------------------------------------
        std::string DBLFilenameDIR = DBLFilename;
        //DBLFilenameDIR = DBLFilenameDIR.setExtension(".DBL.DIR");
        // ---------------------------------------------------------------------------------------------
        // Intialize the DBL.DIR
        DBLFilenameDIR = DBLFilenameDIR + ".DIR";
        vnsLogInfoMacro("Compress the DBL product <"<<DBLFilename<<"> ...")
        // ---------------------------------------------------------------------------------------------
        // Compress the filename
        ReturnStatus result = CompressDirectoryBZIP2(DBLFilename, DBLFilenameDIR);
        // ---------------------------------------------------------------------------------------------
        // Checks the result of the compression. Throws an exception if error
        if (result.first != 0)
        {
            vnsStaticExceptionDataMacro("Error while compressing the DBL product <"<<DBLFilename<<"> !, Log of execution :"<<result.second)
        }
        // ---------------------------------------------------------------------------------------------
        vnsLogInfoMacro("Compress the DBL product done.")
        // ---------------------------------------------------------------------------------------------
        // If the option of cleaning the directory of the data, clean the directory
        if (clean_dir_after_compressed == true)
        {
            vnsLogInfoMacro("Delete the DBL product <"<<DBLFilenameDIR<<"> ...")
            // ---------------------------------------------------------------------------------------------
            result = RemoveDirectory(DBLFilenameDIR);
            // ---------------------------------------------------------------------------------------------
            // Checks the result of the remove directory. Throws an exception if error
            if (result.first != 0)
            {
                vnsStaticExceptionDataMacro(
                        "Error while removing the DBL.DIR product <"<<DBLFilenameDIR<<"> !, Log of execution :"<<result.second)
            }
            vnsLogInfoMacro("Delete the DBL product done ...")
        }
    }
    void
    System::UnCompressDBLProduct(const std::string & DBLFilename, const bool clean_archive_after_uncompressed)
    {
        vnsLogInfoMacro("UnCompress the DBL product <"<<DBLFilename<<"> ...")
        // ---------------------------------------------------------------------------------------------
        //UnCompress the DBL file (creation of the DBL.DIR directory)
        const std::string DBLPathFileWithoutExtension = itksys::SystemTools::GetParentDirectory(DBLFilename.c_str()) + "/"
                + itksys::SystemTools::GetFilenameWithoutLastExtension(DBLFilename);
        // ---------------------------------------------------------------------------------------------
        // Get the DBL.DIR directory
        std::string DBLDIRName = DBLPathFileWithoutExtension + ".DBL.DIR";
        // Get the .BIN filename
        std::string DBLBINName = DBLPathFileWithoutExtension + ".DBL.BIN";
        // ---------------------------------------------------------------------------------------------
        // Already exist, do anything, alors uncompress the DBL file
        // If symlink, get the real path
        DBLDIRName = itksys::SystemTools::GetRealPath(DBLDIRName.c_str());
        DBLBINName = itksys::SystemTools::GetRealPath(DBLBINName.c_str());
        // ---------------------------------------------------------------------------------------------
        // Checks if the DBL.DIR directory exists
        if (itksys::SystemTools::FileIsDirectory(DBLDIRName.c_str()) == true)
        {
            vnsLogDebugMacro("The DBL.DIR is <"<<DBLDIRName<<">.")
            vnsLogInfoMacro("The DBL product <"<<DBLFilename<<"> is already uncompressed (.DBL.DIR directory already exists).")
        }
        // ---------------------------------------------------------------------------------------------
        // Checks if the DBL.BIN file exists
        else if (itksys::SystemTools::FileExists(DBLBINName.c_str()) == true)
        {
            vnsLogDebugMacro("The DBL.BIN is <"<<DBLBINName<<">.")
            vnsLogInfoMacro("The DBL product <"<<DBLFilename<<"> is already uncompressed (.DBL.BIN file already exists).")
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // Uncompress the DBL file
            const ReturnStatus result = UnCompressFile(DBLFilename, itksys::SystemTools::GetFilenamePath(DBLFilename));
            // ---------------------------------------------------------------------------------------------
            // Check the result of the command and throws an exception if error
            if (result.first != 0)
            {
                vnsStaticExceptionDataMacro(
                        "Error while uncompressing the DBL product <"<<DBLFilename<<"> in the destination directory <"<< itksys::SystemTools::GetFilenamePath(DBLFilename) <<">!, Log of execution :"<<result.second)
            }
            vnsLogInfoMacro("UnCompress the DBL product done.")
        }

        // ---------------------------------------------------------------------------------------------
        // Clean the input archive if the option is enable
        if (clean_archive_after_uncompressed == true)
        {
            vnsLogInfoMacro("Delete the input DBL product <"<<DBLFilename<<"> file...")
            RemoveFileBase(DBLFilename);
        }
    }

    //ex: tar cvf ./toto.tar -C ~/MACCS/Logiciel/MACCS/Interfaces Config --exclude '.svn'
    System::ReturnStatus
    System::CompressDirectoryBZIP2(const std::string & destinationFilename, const std::string & sourceDirectory)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("Compress the directory <"<<sourceDirectory<<"> to the tar file <"<<destinationFilename<<">...")
        const std::string basePath = itksys::SystemTools::GetFilenamePath(sourceDirectory);
        const std::string baseModule = itksys::SystemTools::GetFilenameName(sourceDirectory);
        // ---------------------------------------------------------------------------------------------
        // Call the tar basic command
        return CallCommandBasic("tar -cjf " + destinationFilename + " -C " + basePath + " " + baseModule + " --exclude '.svn'");
    }
    System::ReturnStatus
    System::CompressZipFile(const std::string & destinationFilename, const std::string & sourceFilename)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("Compress the file <"<<sourceFilename<<"> to the filename <"<<destinationFilename<<"> without storing the path...")
        // ---------------------------------------------------------------------------------------------
        // Calls the zip -j command to junk the full path of the sourceFilename and
        // store it in the destinationFilename tacking into account its path
        return CallCommandBasic("zip -jv " + destinationFilename + " " + sourceFilename);
    }

    void
    System::CompressZIPFile(const std::string & destinationFilename, const std::string & sourceFilename,
            bool remove_source_file_after_compression)
    {
        vnsLogInfoMacro("Compress the ZIP product <"<<destinationFilename<<"> with the source <"<<sourceFilename<<"> ...")
        // ---------------------------------------------------------------------------------------------
        // Compress the ZIP file
        const ReturnStatus result = CompressZipFile(destinationFilename, sourceFilename);
        // ---------------------------------------------------------------------------------------------
        // Check the result of the command and throws an exception if error
        if (result.first != 0)
        {
            vnsLogDebugMacro("result.first :  "<<result.first)
            vnsLogDebugMacro("result.second : "<<result.second)
            vnsStaticExceptionDataMacro(
                    "Error while compressing the ZIP product <"<<destinationFilename<<"> of the input file <"<< destinationFilename <<">!, Log of execution :"<<result.second)
        }
        if (remove_source_file_after_compression == true)
        {
            RemoveFile(sourceFilename);
        }
        vnsLogInfoMacro("Compress the ZIP product done.")
    }

    System::ReturnStatus
    System::UnCompressFile(const std::string & sourceFilename, const std::string & destinationDirectory)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("UnCompress the tar file <"<<sourceFilename<<"> to the directory <"<<destinationDirectory<<">...")
        // ---------------------------------------------------------------------------------------------
        // Calls the tar basic command
        return CallCommandBasic("tar -xf " + sourceFilename + " -C " + destinationDirectory);
    }
    System::ReturnStatus
    System::UnCompressFileBZIP2(const std::string & sourceFilename, const std::string & destinationDirectory)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro(
                "UnCompress the BZIP2 (-j option) tar file <"<<sourceFilename<<"> to the directory <"<<destinationDirectory<<">...")
        // ---------------------------------------------------------------------------------------------
        // Calls the tar basic command
        return CallCommandBasic("tar -xjf " + sourceFilename + " -C " + destinationDirectory);
    }
    System::ReturnStatus
    System::UnCompressZipFile(const std::string & sourceFilename, const std::string & destinationFilename)
    {
        const std::string destinationDirectory = itksys::SystemTools::GetFilenamePath(destinationFilename);
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("UnCompress the zip file <"<<sourceFilename<<"> to the directory <"<<destinationDirectory<<">...")
        // ---------------------------------------------------------------------------------------------
        if (itksys::SystemTools::FileExists(destinationFilename.c_str()) == true)
        {
            vnsLogInfoMacro("The file <"<<destinationFilename<<"> is already uncompressed.")
            ReturnStatus ret;
            ret.first = 0;
            ret.second = "";
            return ret;
        }
        else
        {
            // Calls the unzip basic command and stores the file(s) in the destinationDirectory
            // -u force do nothing if the file already exist. For basch mode !
            return CallCommandBasic("unzip -u " + sourceFilename + " -d " + destinationDirectory);
        }
    }
    void
    System::UnCompressZIPFile(const std::string & sourceFilename, const std::string & destinationFilename)
    {
        vnsLogInfoMacro("Uncompress the ZIP product <"<<sourceFilename<<"> ...")

        // ---------------------------------------------------------------------------------------------
        // Compress the ZIP file
        const ReturnStatus result = UnCompressZipFile(sourceFilename, destinationFilename);
        // ---------------------------------------------------------------------------------------------
        // Check the result of the command and throws an exception if error
        if (result.first != 0)
        {
            vnsStaticExceptionDataMacro(
                    "Error while uncompressing the ZIP product <"<<sourceFilename<<"> into the output directory <"<< destinationFilename <<">!, Log of execution :"<<result.second)
        }
        vnsLogInfoMacro("Uncompress the ZIP product done.")
    }

    System::ReturnStatus
    System::CheckXmlWithXsd(const std::string & sourceFilename, const std::string & lSchemaFullPath)
    {
        vnsLogDebugMacro("Check the xml file <"<<sourceFilename<<"> with its schema <"<<lSchemaFullPath<<">...")
        // ---------------------------------------------------------------------------------------------
        // Call the xmllint command
        return CallCommandBasic("xmllint --noout " + sourceFilename + " --schema " + lSchemaFullPath);
    }

    System::ReturnStatus
    System::TranslateXsl(const std::string & sourceFilename, const std::string & lXslStyleSheetFilename)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("Apply for xml file <"<<sourceFilename<<"> with style sheet <"<<lXslStyleSheetFilename<<">...")
        // ---------------------------------------------------------------------------------------------
        // Call the xsltproc command
        return CallCommandBasic(
                "xsltproc --output " + sourceFilename + " " + lXslStyleSheetFilename + " " + sourceFilename + " 2> /dev/null > /dev/null");
    }

    void
    System::CopyFile(const std::string & sourceDir, const std::string & destinationDir, const bool & enable_exception_if_error)
    {
        // ---------------------------------------------------------------------------------------------
        const bool result = itksys::SystemTools::CopyFileAlways(sourceDir, destinationDir);
        // ---------------------------------------------------------------------------------------------
        // Check the result of the command and throws an exception if error
        if (result == false)
        {
            if (enable_exception_if_error == true)
            {
                vnsStaticExceptionDataMacro("Error while copying the filename <"<<sourceDir<<"> to <"<<destinationDir<<"> !")
            }
            else
            {
                vnsLogDebugMacro("Copy the file <"<<sourceDir<<"> to <"<<destinationDir<<"> done (but FAILED).")
            }
        }
        else
        {
            vnsLogDebugMacro("Copy the file <"<<sourceDir<<"> to <"<<destinationDir<<"> done.")
        }
    }
    std::string
    System::CopyFileToDirectory(const std::string & sourceFilename, const std::string & destinationDir)
    {
        const std::string destinationFilename = Utilities::JoinFilenames(destinationDir,
                itksys::SystemTools::GetFilenameName(sourceFilename));
        CopyFile(sourceFilename, destinationFilename);
        return destinationFilename;
    }

    void
    System::CopyListOfFilesToDirectory(const Utilities::ListOfStrings & list_sourceFilename, const std::string & destinationDir)
    {
        const unsigned int l_Nbiles = list_sourceFilename.size();
        for (unsigned int i = 0; i < l_Nbiles; ++i)
        {
            CopyFileToDirectory(list_sourceFilename[i], destinationDir);
        }
    }

    void
    System::CopyFilesFromDirectoryToDirectory(const std::string & sourceDir, const std::string & destinationDir)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("Copy all files (*) from the directory <"<<sourceDir<<"> to ... <"<<destinationDir<<"> ...")
        // ---------------------------------------------------------------------------------------------
        const bool result = itksys::SystemTools::CopyADirectory(sourceDir, destinationDir);
        // ---------------------------------------------------------------------------------------------
        // Check the result of the command and throws an exception if error
        if (result == false)
        {
            vnsStaticExceptionDataMacro(
                    "Error while copying all files (*) from the directory <"<<sourceDir<<"> to ... <"<<destinationDir<<"> !")
        }
    }
    void
    System::RemoveFileBase(const std::string & source)
    {
        vnsLogSuperDebugMacro("Remove the file <"<<source<<"> ?")
        // ---------------------------------------------------------------------------------------------
        // Check the result of the command and returns -1 if error
        const bool result = itksys::SystemTools::RemoveFile(source);
        if (result == false)
        {
            vnsLogWarningMacro("Impossible to delete the file <"<<source<<">!")
        }
    }
    void
    System::RemoveFile(const std::string & source)
    {
        // ---------------------------------------------------------------------------------------------
        // Check the result of the command and returns -1 if error
        vnsLogSuperDebugMacro("Remove the file <"<<source<<"> ?")
        // ---------------------------------------------------------------------------------------------
        // Check the result of the command and returns -1 if error
        const bool result = itksys::SystemTools::RemoveFile(source);
        if (result == false)
        {
            vnsStaticExceptionDataMacro("Impossible to delete the file <"<<source<<">!")
        }
    }

    System::ReturnStatus
    System::RemoveDirectory(const std::string & sourceDir)
    {
        vnsLogDebugMacro("Remove the directory <"<<sourceDir<<"> ?")
        ReturnStatus res;
        res.second = "";
        res.first = 0;
        // ---------------------------------------------------------------------------------------------
        // Checks if the source is a directory
        if (itksys::SystemTools::FileIsDirectory(sourceDir) == true)
        {
            // ---------------------------------------------------------------------------------------------
            // Check the result of the command and returns -1 if error
            vnsLogDebugMacro("Remove the directory <"<<sourceDir<<"> ...")
            const bool bresult = itksys::SystemTools::RemoveADirectory(sourceDir);
            if (bresult == false)
            {
                vnsLogWarningMacro("Impossible to delete the directory <"<<sourceDir<<"> completely!")
                res.first = -1;
            }
        }
        return res;
    }

    void
    System::CreateDirectory(const std::string & comment, const std::string & sourceDir)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro(comment << " (<"<<sourceDir<<">) ...")
        //        bool result = sourceDir.createDirectory(true);
        if (sourceDir.empty() == true)
        {
            vnsStaticExceptionDataMacro("The directory path is not initialized!")
        }
        // ---------------------------------------------------------------------------------------------
        // If the file does not exist
        if (itksys::SystemTools::FileExists(sourceDir.c_str()) == false)
        {
            const bool result = itksys::SystemTools::MakeDirectory(sourceDir.c_str());
            if (result == false)
            {
                vnsStaticExceptionDataMacro(comment << ": error while creating the directory <"<<sourceDir<<"> !")
            }
        }
        // ---------------------------------------------------------------------------------------------
        // If the file exists
        else
        {
            vnsLogInfoMacro(comment << " <"<<sourceDir<<"> already exist!")
        }
        vnsLogDebugMacro(comment << " <"<<sourceDir<<"> done.")

    }
    void
    System::CreateDirectory(const std::string & sourceDir)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("Create the directory <"<<sourceDir<<"> ...")
        //        bool result = sourceDir.createDirectory(true);
        // ---------------------------------------------------------------------------------------------
        const bool result = itksys::SystemTools::MakeDirectory(sourceDir.c_str());
        if (result == false)
        {
            vnsStaticExceptionDataMacro("Error while creating the directory <"<<sourceDir<<"> !")
        }

    }
    void
    System::Rename(const std::string & oldname, const std::string & newname)
    {
        // ---------------------------------------------------------------------------------------------
        // Does the file exist ?
        if (itksys::SystemTools::FileExists(newname.c_str()) == true)
        {
            vnsLogDebugMacro("Before rename the filename <"<<oldname<<"> to <"<<newname<<">, remove the destination file <"<<newname<<">.")
            if (itksys::SystemTools::FileIsDirectory(newname) == true)
            {
                const ReturnStatus result = RemoveDirectory(newname);
                // ---------------------------------------------------------------------------------------------
                // Checks the result of the remove directory. Throws an exception if error
                if (result.first != 0)
                {
                    vnsStaticExceptionDataMacro("Error while removing the directory <"<<newname<<"> !, Log of execution :"<<result.second)
                }
            }
            else
            {
                RemoveFile(newname);
            }
        }
        const int result = std::rename(oldname.c_str(), newname.c_str());
        // ---------------------------------------------------------------------------------------------
        // File not renamed
        if (result != 0)
        {
            vnsStaticExceptionDataMacro("Impossible to rename the filename <"<<oldname<<"> to <"<<newname<<">!")
        }
    }

    void
    System::RenameFilename(const std::string & filename, const unsigned short recursive_dir, const bool & apply_rename,
            const std::string & origin_key, const std::string & final_key)
    {
        vnsLogSuperDebugMacro(
                "Rename filename <"<<filename<<"> with replace keymatching <"<<origin_key<<"> by <"<<final_key<<"> (recurse="<<recursive_dir<<" and apply_rename="<<Utilities::BoolToLowerString(apply_rename)<<")...")
        // ---------------------------------------------------------------------------------------------
        std::string l_Pathname = itksys::SystemTools::GetFilenamePath(filename);
        std::string l_Filename = itksys::SystemTools::GetFilenameName(filename);

        // ---------------------------------------------------------------------------------------------
        for (unsigned short i = 0; i < recursive_dir; i++)
        {
            l_Filename = itksys::SystemTools::GetFilenameName(l_Pathname) + "/" + l_Filename;
            l_Pathname = itksys::SystemTools::GetFilenamePath(l_Pathname);
        }
        // ---------------------------------------------------------------------------------------------
        l_Filename = Utilities::RenameString(l_Filename, origin_key, final_key);
        l_Filename = Utilities::JoinFilenames(l_Pathname, l_Filename);

        vnsLogSuperDebugMacro(" => so rename filename <"<<filename<<"> to <"<<l_Filename<<"> ...")

        // ---------------------------------------------------------------------------------------------
        if ((itksys::SystemTools::FileExists(filename.c_str()) == true) && (apply_rename == true))
        {
            // Replace the file
            Rename(filename, l_Filename);
        }
    }

    std::string
    System::RenameStringFilename(const std::string & rootDirectory, const std::string & filename, const std::string & origin_key,
            const std::string & final_key)
    {
        // ---------------------------------------------------------------------------------------------
        std::string l_ShortFilename = Utilities::GetFilenameWithoutRootDirectory(rootDirectory, filename);
        l_ShortFilename = Utilities::RenameString(l_ShortFilename, origin_key, final_key);
        return Utilities::JoinFilenames(rootDirectory, l_ShortFilename);
    }

    void
    System::RenameAllFilesInADirectory(const std::string & input_directory, const std::string & origin_key, const std::string & final_key)
    {

        // Rename all file in the input_directory and sub-directories
        // ---------------------------------------------------------------------------------------------
        Utilities::ListOfStrings l_ListOfFilenames;
        GetListOfAllsFilesFromDirectory(input_directory, l_ListOfFilenames, true /* recursively */);
        const unsigned int l_NbFiles = l_ListOfFilenames.size();
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            const std::string l_Filename = l_ListOfFilenames[i];
            // Remove root in the filename
            // ---------------------------------------------------------------------------------------------
            std::string l_ShortDirectoryFilename = Utilities::RenameString(l_Filename, input_directory, "");

            // Check if contain 'origin_key'
            if (l_ShortDirectoryFilename.find(origin_key) != std::string::npos)
            {
                RenameFilename(l_Filename, 0 /*recursive_dir*/, true /* apply_rename*/, origin_key, final_key);
            }
        }
    }

    void
    System::RenameAllFilesAndDirectoriesInADirectory(const std::string & input_directory, const std::string & origin_key,
            const std::string & final_key)
    {

        vnsLogDebugMacro(
                "Rename all files and directories contained in the directory <"<<input_directory<<"> with replace keymatching <"<<origin_key<<"> by <"<<final_key<<">...")
        // Rename all files and directories in the input_directory
        // ---------------------------------------------------------------------------------------------
        // retrieve list of files
        Utilities::ListOfStrings l_ListOfFilenames = GetListOfFilenamesInDirectory(input_directory);
        const unsigned int l_NbFiles = l_ListOfFilenames.size();
        // for each file
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            // Get short filename
            const std::string l_OriginFilename = l_ListOfFilenames[i];
            const std::string l_OriginShortFilename = itksys::SystemTools::GetFilenameName(l_OriginFilename);

            if (l_OriginShortFilename.find(origin_key) != std::string::npos)
            {
                // find key to replace and replace it with the new key
                std::string l_FinalShortFilename = Utilities::RenameString(l_OriginShortFilename, origin_key, final_key);
                // reconstruct the full filename
                l_FinalShortFilename = Utilities::JoinFilenames(itksys::SystemTools::GetFilenamePath(l_OriginFilename),
                        l_FinalShortFilename);
                //rename the file
                Rename(l_OriginFilename, l_FinalShortFilename);
            }
        }
        // Enter in all directories in the input_directory
        // ---------------------------------------------------------------------------------------------
        const Utilities::ListOfStrings l_ListOfSubDirectories = GetListOfSubDirectoriesInDirectory(input_directory);
        const unsigned int l_NbSubDirectories = l_ListOfSubDirectories.size();
        for (unsigned int i = 0; i < l_NbSubDirectories; i++)
        {
            // Function recursive
            RenameAllFilesAndDirectoriesInADirectory(l_ListOfSubDirectories[i], origin_key, final_key);
        }
    }

    void
    System::TouchFilename(const std::string & filename)
    {
        // ---------------------------------------------------------------------------------------------
        const bool result = itksys::SystemTools::Touch(filename, true);
        // ---------------------------------------------------------------------------------------------
        if (result == false)
        {
            vnsStaticExceptionDataMacro("Utilities: Impossible to create (touch) the filename "<<filename<<".")
        }
    }

    System::ReturnStatus
    System::CallCommandBasic(int argc, char * argv[])
    {
        // ---------------------------------------------------------------------------------------------
        // Parse all arguments and add in a string value separated with a "space" value
        std::string commandline("");
        for (int i = 0; i < argc; i++)
        {
            commandline = commandline + " " + argv[i];
        }
        // ---------------------------------------------------------------------------------------------
        // Calls the command
        return CallCommandBasic(commandline);
    }
    System::ReturnStatus
    System::CallCommandBasic(const std::string & commandline)
    {
        const unsigned int BUFFER_SIZE = 4096;
        ReturnStatus return_value;
        return_value.second = std::string("Command line : ")+commandline+std::string(" .Log : ");
        // ---------------------------------------------------------------------------------------------
        // Disable Log for basic command
        vnsLogSuperDebugMacro("Call the basic command <"<<commandline<<"> ...")

        //s2ipfMsgDevMacro("Command line : "<<commandline);
        //This actually fork the process creating a new process for kakadu to perform
        // ---------------------------------------------------------------------------------------------
        // Launches the command with a popen command
        // ---------------------------------------------------------------------------------------------
        itk::TimeProbe clock;
        clock.Start();

        // ---------------------------------------------------------------------------------------------
        // Create FILE process
        //Redirect error to stdout
        const std::string command_noout = commandline + " 2>&1";
        FILE* processfile = popen(command_noout.c_str(), "r");
        vnsLogSuperDebugMacro("command FILE* : "<< processfile)
        // ---------------------------------------------------------------------------------------------
        // Checks the result
        if (processfile == NULL)
        {
            return_value.first = 1;
	    return_value.second += strerror(errno);
        }
        else
        {
            char buffer[BUFFER_SIZE];
            // ---------------------------------------------------------------------------------------------
            // Loops the process outputs
            vnsLogSuperDebugMacro("Avant while...")
            while (fgets(buffer, BUFFER_SIZE, processfile) != NULL)
            {
                return_value.second += buffer;
            }
            vnsLogSuperDebugMacro("While fini.")
            const int preturncode = pclose(processfile);
            // ---------------------------------------------------------------------------------------------
            //Return code handling
            switch (preturncode)
                {
            case 0:
                {
                    // ---------------------------------------------------------------------------------------------
                    //This case is the normal case
                    return_value.first = 0;
                    break;
                }
            case -1:
                {    // ---------------------------------------------------------------------------------------------
                     //Execution error
                    return_value.first = -1;
                    break;
                }
            default:
                {
                    return_value.first = -2;
                    break;
                }
                }
        }
        // ---------------------------------------------------------------------------------------------
        // Stop clock
        clock.Stop();

        vnsLogSuperDebugMacro(
                "Command run in "<< Utilities::GetStringFormatedTime(clock.GetTotal())<<" (code return '"<<return_value.first<<"').");

        return return_value;
    }

    Utilities::ListOfStrings
    System::GetListOfFilesInDirectory(const std::string & input_directory)
    {
        // ---------------------------------------------------------------------------------------------
        // Load the input directory
        const Utilities::ListOfStrings l_ListOfStrings = GetListOfFilenamesInDirectory(input_directory);
        const unsigned int l_NbFiles = l_ListOfStrings.size();
        Utilities::ListOfStrings l_OutListOfStrings;
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            // ---------------------------------------------------------------------------------------------
            // Check if the file exists before adding it to the list
            if (itksys::SystemTools::FileExists(l_ListOfStrings[i].c_str(), true /* isFile */) == true)
            {
                l_OutListOfStrings.push_back(l_ListOfStrings[i]);
            }
        }
        return l_OutListOfStrings;
    }

    Utilities::ListOfStrings
    System::GetListOfSubDirectoriesInDirectory(const std::string & input_directory)
    {
        // ---------------------------------------------------------------------------------------------
        // Load the input directory
        const Utilities::ListOfStrings l_ListOfStrings = GetListOfFilenamesInDirectory(input_directory);
        const unsigned int l_NbFiles = l_ListOfStrings.size();
        Utilities::ListOfStrings l_OutListOfStrings;
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            // ---------------------------------------------------------------------------------------------
            // Check if the file exists before adding it to the list
            if (itksys::SystemTools::FileIsDirectory(l_ListOfStrings[i].c_str()) == true)
            {
                l_OutListOfStrings.push_back(l_ListOfStrings[i]);
            }
        }
        return l_OutListOfStrings;
    }

    Utilities::ListOfStrings
    System::GetListOfFilenamesInDirectory(const std::string & input_directory)
    {
        // Test if the input path is a directory
        vnsLogSuperDebugMacro("Get all filenames of the directory: "<<input_directory)
        if (itksys::SystemTools::FileIsDirectory(input_directory.c_str()) == false)
        {
            vnsStaticExceptionDataMacro("The input path <"<<input_directory<<"> is not a valid directory !")
        }
        // Load the input directory
        Utilities::ListOfStrings l_ListOfStrings;
        itksys::Directory l_dir;
        const bool l_ret = l_dir.Load(input_directory.c_str());
        // Test the validity of the input directory
        if (l_ret == false)
        {
            vnsStaticExceptionDataMacro(
                    "Error while parsing the input directory <"<<input_directory<<"> (from the itksys::Directory Load method)")
        }
        // Get the content in the input directory
        const unsigned int l_NbFiles = l_dir.GetNumberOfFiles();
        vnsLogSuperDebugMacro("In the sub directories, nb files found: "<<l_NbFiles)
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            // Test if the current item is a file by looking for the extension
            std::string filename = l_dir.GetFile(i);
            if ((filename != ".") && (filename != ".."))
            {
                filename = Utilities::JoinFilenames(input_directory, filename);
                // Add the sub directory to the output list
                vnsLogSuperDebugMacro("Add the filename: "<<filename)
                l_ListOfStrings.push_back(filename);
            }
        }
        return l_ListOfStrings;
    }

    char *
    System::ReadBinaryFile(const std::string & filename, std::streamoff & file_length)
    {

        std::fstream lFlux;
        lFlux.open(filename.c_str(), std::ios::in | std::ios::binary);

        if (lFlux.fail() == true)
        {
            vnsStaticExceptionDataMacro("Error while opening the binary filename <"<<filename<<"> !")
        }
        lFlux.seekg(0, lFlux.end);
        file_length = lFlux.tellg();
        lFlux.seekg(0, lFlux.beg);
        vnsLogSuperDebugMacro("ReadBinaryFile: file size: "<<file_length)

        // ---------------------------------------------------------------------------------------------
        //Lectures du fichier d'un seul coup
        char * l_Buf = new char[file_length + 1];

        // ---------------------------------------------------------------------------------------------
        lFlux.read(l_Buf, file_length);
        l_Buf[lFlux.gcount()] = '\0';

        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("ReadBinaryFile: close file...")
        lFlux.close();
        return l_Buf;
    }

    std::string
    System::ReadAsciiFile(const std::string & filename)
    {
        // ---------------------------------------------------------------------------------------------
        std::ifstream fluxfile(filename.c_str());
        // ---------------------------------------------------------------------------------------------
        if (!fluxfile)
        {
            vnsStaticExceptionDataMacro("Error while reading the ASCII file <"<<filename<<"> !")
        }
        // ---------------------------------------------------------------------------------------------
        std::string listLineFileTest("");
        std::string strfileref("");
        std::getline(fluxfile, listLineFileTest);
        while (std::getline(fluxfile, strfileref))
        {
            listLineFileTest += "\n" + strfileref;
        }
        fluxfile.close();
        return listLineFileTest;
    }

    System::DiscSizeType
    System::GetTheSpaceDiscSizeOfADirectory(const std::string & p_InputDirectory, bool p_recursively)
    {
        DiscSizeType l_Size(0);
        const Utilities::ListOfStrings l_ListOfFiles = GetListOfAllsFilesFromDirectory(p_InputDirectory, p_recursively);
        const unsigned int l_NbFiles = l_ListOfFiles.size();
        vnsLogDebugMacro("Compute space disc size of the directory: "<<p_InputDirectory << " : Nb files found: "<<l_NbFiles)
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            // Copy in the p_ListOfFiles
            l_Size += static_cast<DiscSizeType>(itksys::SystemTools::FileLength(l_ListOfFiles[i]));
        }
        vnsLogDebugMacro("The size of the directory <"<<p_InputDirectory<<"> is <"<<l_Size<<">.")
        return l_Size;
    }

    Utilities::ListOfStrings
    System::GetListOfAllsFilesFromDirectory(const std::string & p_InputDirectory, bool p_recursively)
    {
        // ---------------------------------------------------------------------------------------------
        Utilities::ListOfStrings l_ListOfFiles;
        // ---------------------------------------------------------------------------------------------
        GetListOfAllsFilesFromDirectory(p_InputDirectory, l_ListOfFiles, p_recursively);
        // ---------------------------------------------------------------------------------------------
        return l_ListOfFiles;
    }

    void
    System::GetListOfAllsFilesFromDirectory(const std::string & p_InputDirectory, Utilities::ListOfStrings & p_ListOfFiles,
            bool p_recursively)
    {
        // ---------------------------------------------------------------------------------------------
        const Utilities::ListOfStrings ListOfFilenames = GetListOfFilesInDirectory(p_InputDirectory);
        // ---------------------------------------------------------------------------------------------
        const unsigned int l_NbFiles = ListOfFilenames.size();
        vnsLogSuperDebugMacro("Nb files in the sub directory: "<<l_NbFiles)
        // ---------------------------------------------------------------------------------------------
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            // ---------------------------------------------------------------------------------------------
            // Copy in the p_ListOfFiles
            p_ListOfFiles.push_back(ListOfFilenames[i]);
        }
        // ---------------------------------------------------------------------------------------------
        if (p_recursively == true)
        {
            // ---------------------------------------------------------------------------------------------
            //Found the directories
            const Utilities::ListOfStrings ListOfDirectories = GetListOfSubDirectoriesInDirectory(p_InputDirectory);
            // ---------------------------------------------------------------------------------------------
            const unsigned int l_NbFiles = ListOfDirectories.size();
            // ---------------------------------------------------------------------------------------------
            vnsLogSuperDebugMacro("Nb sub-directories in the directory: "<<l_NbFiles)
            // ---------------------------------------------------------------------------------------------
            for (unsigned int i = 0; i < l_NbFiles; i++)
            {
                const std::string l_SubDirName = itksys::SystemTools::GetFilenameName(ListOfDirectories[i]);
                // ---------------------------------------------------------------------------------------------
                // Call recursively this method
                GetListOfAllsFilesFromDirectory(p_InputDirectory + "/" + l_SubDirName, p_ListOfFiles, p_recursively);
            }
        }
    }
}

