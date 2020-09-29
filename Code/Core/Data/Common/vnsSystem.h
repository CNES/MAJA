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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2240-CNES : 30 mars 2017 : Retrait de log à l'execution de commande  *
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-975-CNES : 13 decembre 2016 : Copie des detfootprint dans la sortie   *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 12 octobre 2016 : Audit code - Correction qualite :       *
 *                                                               Regle : Missing parameter name in function *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 13 mai 2016 : Modification mineure                        *
 * VERSION : 4-7-0 : FA : LAIG-FA-MAC-1475-CNES : 7 decembre 2015 : Correction produit NOTV qui contient    *
 *                                       encore des produits VALD dans l'arborescence du produit final NOTV *
 * VERSION : 4-0-1 : FA : LAIG-FA-MAC-865-CNES : 11 juin 2014 : Decompression du produit L2 en entree       *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-748-CNES : 13 mars 2014 : Ajout d'un booléen pour supprimer le DBL    *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
 * VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 28 octobre 2013 : Mise a jur suite creation plugin LANDSAT *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avr. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSystem_h
#define __vnsSystem_h

#include <string>
#include "vnsUtilities.h"

namespace vns
{

    /** \class System
     * \brief System operations.
     *
     * System operations, filename, PID, Host name, etc.
     *
     */
    class System
    {
    public:

        typedef std::pair<int,std::string> ReturnStatus;

        /** Standard class typedefs. */
        typedef System Self;

        typedef unsigned long DiscSizeType;

        /** Get the host name */
        static std::string
        GetHostName();
        /** Get the process name */
        static std::string
        GetProcessName();

        /** Get the process id */
        static int
        GetPID();

        /** Rasterize gml file */
        static ReturnStatus
        Rasterize(const std::string & inputFilename, const unsigned int xmin, const unsigned int ymin, const unsigned int xmax,
                const unsigned int ymax, const unsigned int size_x, const unsigned int size_y, const std::string & projection,
                const std::string & outputFilename, const std::string & gdalAdditionalCommandLineParameters);

        static void
        CompressDBLProduct(const std::string & DBLFilename, const bool clean_dir_after_compressed);

        static void
        UnCompressDBLProduct(const std::string & DBLFilename, const bool clean_archive_after_uncompressed);

        static ReturnStatus
        UnCompressFile(const std::string & sourceFilename, const std::string & destinationDirectory);

        static ReturnStatus
        CompressDirectoryBZIP2(const std::string & destinationFilename, const std::string & sourceDirectory);
        static ReturnStatus
        UnCompressFileBZIP2(const std::string & sourceFilename, const std::string & destinationDirectory);

        static ReturnStatus
        CompressZipFile(const std::string & destinationFilename, const std::string & sourceFilename);

        static void
        CompressZIPFile(const std::string & destinationFilename, const std::string & sourceFilename,
                bool remove_source_file_after_compression = false);

        static ReturnStatus
        UnCompressZipFile(const std::string & sourceFilename, const std::string & destinationFilename);

        static void
        UnCompressZIPFile(const std::string & sourceFilename, const std::string & destinationFilename);

        static ReturnStatus
        CheckXmlWithXsd(const std::string & sourceFilename, const std::string & lSchemaFullPath);
        static ReturnStatus
        TranslateXsl(const std::string & sourceFilename, const std::string & lXslStyleSheetFilename);

        static void
        CopyFile(const std::string & sourceDir, const std::string & destinationDir, const bool & enable_exception_if_error = true);

        static std::string
        CopyFileToDirectory(const std::string & sourceFilename, const std::string & destinationDir);

        static void
        CopyListOfFilesToDirectory(const Utilities::ListOfStrings & list_sourceFilename, const std::string & destinationDir);

        static void
        CopyFilesFromDirectoryToDirectory(const std::string & sourceDir, const std::string & destinationDir);

        static void
        CreateDirectory(const std::string & comment, const std::string & sourceDir);

        static void
        CreateDirectory(const std::string & sourceDir);

        static ReturnStatus
        RemoveDirectory(const std::string & sourceDir);

        static void
        RemoveFileBase(const std::string & source);

        static void
        RemoveFile(const std::string & source);

        static void
        Rename(const std::string & oldname, const std::string & newname);

        static void
        RenameFilename(const std::string & filename, const unsigned short recursive_dir = 0, const bool & apply_rename = true,
                const std::string & origin_key = "VALD", const std::string & final_key = "NOTV");

        static std::string
        RenameStringFilename(const std::string & rootDirectory, const std::string & filename, const std::string & origin_key = "VALD",
                const std::string & final_key = "NOTV");

        static void
        RenameAllFilesInADirectory(const std::string & input_directory, const std::string & origin_key = "VALD",
                const std::string & final_key = "NOTV");

        static void
        RenameAllFilesAndDirectoriesInADirectory(const std::string & input_directory, const std::string & origin_key = "VALD",
                const std::string & final_key = "NOTV");

        static void
        TouchFilename(const std::string & filename);

        static ReturnStatus
        CallCommandBasic(int argc, char * argv[]);

        static ReturnStatus
        CallCommandBasic(const std::string & commandline);

        // Get all filenames in directory (files and sub-directories)
        static Utilities::ListOfStrings
        GetListOfFilenamesInDirectory(const std::string & input_directory);

        // Get all files  directory
        static Utilities::ListOfStrings
        GetListOfFilesInDirectory(const std::string & input_directory);

        // Get all directories in directory
        static Utilities::ListOfStrings
        GetListOfSubDirectoriesInDirectory(const std::string & input_directory);

        static char *
        ReadBinaryFile(const std::string & filename, std::streamoff & file_length);

        template<class T>
            static std::vector<T>
            ReadBinaryFileIntoVector(const std::string & filename)
            {
                std::fstream lFlux;
                lFlux.open(filename.c_str(), std::ios::in | std::ios::binary);

                if (lFlux.fail() == true)
                {
                    vnsStaticExceptionDataMacro("Error while opening the binary filename <" << filename << "> !")
                }
                lFlux.seekg(0, lFlux.end);
                std::streampos file_length = lFlux.tellg();
                lFlux.seekg(0, lFlux.beg);
                std::streampos l_NbValues = (file_length / sizeof(T));
                vnsLogSuperDebugMacro("ReadBinaryFileIntoVector: file size: " << file_length)
                vnsLogSuperDebugMacro("ReadBinaryFileIntoVector: type sizeof: " << sizeof(T))
                vnsLogSuperDebugMacro("ReadBinaryFileIntoVector: nb values: " << l_NbValues)

                std::vector<T> l_v(l_NbValues);

                // ---------------------------------------------------------------------------------------------
                //Lectures du fichier d'un seul coup
                lFlux.read(reinterpret_cast<char *>(&l_v[0]), static_cast<std::size_t>(file_length));
                if (!lFlux)
                {
                    vnsStaticExceptionDataMacro(
                            "Error while reading the binary file <" << filename << ">. Only " << lFlux.gcount() << " could be read of "
                                    << file_length << " in the file !")
                }

                // ---------------------------------------------------------------------------------------------
                vnsLogSuperDebugMacro("ReadBinaryFile: close file...")
                lFlux.close();
                return l_v;
            }

        static std::string
        ReadAsciiFile(const std::string & filename);

        static void
        GetListOfAllsFilesFromDirectory(const std::string & p_InputDirectory, Utilities::ListOfStrings & p_ListOfFiles, bool p_recursively =
                true);
        static Utilities::ListOfStrings
        GetListOfAllsFilesFromDirectory(const std::string & p_InputDirectory, bool p_recursively = true);

        static DiscSizeType
        GetTheSpaceDiscSizeOfADirectory(const std::string & p_InputDirectory, bool p_recursively = true);

        /*
         * Root install dir
         */
        static const std::string m_VarEnvRootInstallDirectoryOfMaja;

    private:
        /* Store information for perf reason */
        static std::string m_HostName;
        static std::string m_ProcessName;
    };

} // namespace vns

#endif
