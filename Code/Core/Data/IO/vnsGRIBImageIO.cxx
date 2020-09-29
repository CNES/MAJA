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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 17 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 4-6-3 : FA : LAIG-FA-MAC-1504-CNES : 23 novembre 2015 : Erreur à la lecteur d'un produit L1    *
 *                                                                                                 S2 GPP   *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-354-CNES : 10 mars 2014 : Correction qualité sur règles de codage     *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 24 octobre 2011 : Modification pour prise en compte        *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 12 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsGRIBImageIO.h"
#include <cstring>  // for strncpy
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsUtilities.h"
#include "itkMetaDataObject.h"
#include "itksys/SystemTools.hxx"

namespace vns
{

    // Define 256
    const unsigned long int GRIBImageIO::m_Const256 = 256;
    // Define 256*256
    const unsigned long int GRIBImageIO::m_Const256_2 = 65536;
    // Denfine 256*256*256
    const unsigned long int GRIBImageIO::m_Const256_3 = 16777216;

    GRIBImageIO::GRIBImageIO()
    {
        // ---------------------------------------------------------------------------------------------
        m_PixelType = SCALAR;
        m_ComponentType = FLOAT;

        // ---------------------------------------------------------------------------------------------
        // By default set number of dimensions to two.
        this->SetNumberOfDimensions(2);

        // ---------------------------------------------------------------------------------------------
        // Set default spacing to one
        m_Spacing[0] = 1.0;
        m_Spacing[1] = 1.0;
        // ---------------------------------------------------------------------------------------------
        // Set default origin to zero
        m_Origin[0] = 0.0;
        m_Origin[1] = 0.0;

        // ---------------------------------------------------------------------------------------------
        // Init mantisse values to 0
        m_RefValue = 0;
        m_ScaleFact = 0;
        m_DecScaleFact = 0;

        // ---------------------------------------------------------------------------------------------
        // Set the supported file name extension.
        this->AddSupportedReadExtension(".grb");
        this->AddSupportedReadExtension(".GRIB");

        // ---------------------------------------------------------------------------------------------
        // Init the band to read at 1

        m_DatasetNumber = 1;

        // ---------------------------------------------------------------------------------------------
        // Init cursor to 0
        m_ParameterInd = 0;
        m_GridPosition = 0;
    }

    GRIBImageIO::~GRIBImageIO()
    {
        if (m_File.is_open() == true)
        {
            m_File.close();
        }
    }

    bool
    GRIBImageIO::CanReadFile(const char* filename)
    {
        // ---------------------------------------------------------------------------------------------
        std::string lFileName;
        // ---------------------------------------------------------------------------------------------
        // Find the filename (retrieve band Id) : No, the Band Id detection is in charge of the ImageFileReader. Like GDAL process.
        lFileName = filename;

        // ---------------------------------------------------------------------------------------------
        // Check file existence and if it is a real file
        // ---------------------------------------------------------------------------------------------
        bool exist = itksys::SystemTools::FileExists(lFileName.c_str());
        // ---------------------------------------------------------------------------------------------
        bool isDir = itksys::SystemTools::FileIsDirectory(lFileName.c_str());
        // ---------------------------------------------------------------------------------------------
        if ((exist != true) || (isDir == true))
        {
            // ---------------------------------------------------------------------------------------------
            vnsLogSuperDebugMacro("  GRIB CanRead filename doesn't exist : "<<lFileName);
            return false;
        }

        // ---------------------------------------------------------------------------------------------
        // Discard extensions
        const bool l_IsAnImageFilename = Utilities::IsAnImageFilenameByCheckingExtension(lFileName);
        // ---------------------------------------------------------------------------------------------
        if (l_IsAnImageFilename == true)
        {
            return false;
        }

        // ---------------------------------------------------------------------------------------------
        // Look if the file is already open. If yes, close it.
        // ---------------------------------------------------------------------------------------------
        if (m_File.is_open() == true)
        {
            // ---------------------------------------------------------------------------------------------
            m_File.close();
        }

        // ---------------------------------------------------------------------------------------------
        std::fstream l_file;
        // ---------------------------------------------------------------------------------------------
        l_file.open(lFileName.c_str(), std::ios::in | std::ios::binary);
        // ---------------------------------------------------------------------------------------------
        if (l_file.fail() == true)
        {
            vnsExceptionDataMacro("GRIBImageIO::CanReadFile() failed open !");
        }

        // ---------------------------------------------------------------------------------------------
        // Update filename
        m_FileName = lFileName;

        // ---------------------------------------------------------------------------------------------
        // Extract header info (size, real GRIB file)
        const bool lResult = InternalReadHeaderInformation(l_file, false);
        // ---------------------------------------------------------------------------------------------
        l_file.close();
        vnsLogSuperDebugMacro("  GRIB Try to read InternalReadHeaderInformation : "<<lResult);
        // ---------------------------------------------------------------------------------------------

        return (lResult);
    }

    // Used to print information about this object
    void
    GRIBImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
    }

    // Read a 3D image (or event more bands)... not implemented yet
    void
    GRIBImageIO::ReadVolume(void*)
    {
    }

    // Read image
    void
    GRIBImageIO::Read(void* buffer)
    {
        // ---------------------------------------------------------------------------------------------
        char * p = static_cast<char *>(buffer);

        // ---------------------------------------------------------------------------------------------
        int lNbLines = this->GetIORegion().GetSize()[1];
        int lNbColumns = this->GetIORegion().GetSize()[0];
        // ---------------------------------------------------------------------------------------------
        int lFirstLine = this->GetIORegion().GetIndex()[1]; // [1... ]
        int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

        // ---------------------------------------------------------------------------------------------
        otbMsgDevMacro( <<" GRIBImageIO::Read()  ")
        otbMsgDevMacro( <<" Image size  : "<<m_Dimensions[0]<<","<<m_Dimensions[1])
        otbMsgDevMacro( <<" Region read (IORegion)  : "<<this->GetIORegion())
        otbMsgDevMacro( <<" Nb Of Components       : "<<this->GetNumberOfComponents())

        // ---------------------------------------------------------------------------------------------
        unsigned long int cpt(0);

        // ---------------------------------------------------------------------------------------------
        char dataValue[3 + 1];
        dataValue[3] = '\0';

        // ---------------------------------------------------------------------------------------------
        // We read 2 pixels at each loop because grid binary data are coded in 12 bits. Here we read 3 bytes = 2 pixels
        // If the first pixel line index is odd, we will have to read the previous pixel to but write only the last.
        bool startIdIsEven = true;
        // ---------------------------------------------------------------------------------------------
        // If the last pixel line index is odd, we will have to read the following pixel to but write only the first.
        bool stopIdIsEven = true;

        // ---------------------------------------------------------------------------------------------
        const float scaleFact_2 = static_cast<float>(vcl_pow(2.0, m_ScaleFact));
        const float scaleFact_10 = static_cast<float>(vcl_pow(10.0, -m_DecScaleFact));

        // ---------------------------------------------------------------------------------------------
        for (int lineNo = 0; lineNo < lNbLines; lineNo++)
        {
        	int startPixNb = (lFirstLine + lineNo) * m_Dimensions[0] + lFirstColumn;
        	int stopPixNb = startPixNb + lNbColumns;
            if (startPixNb % 2 != 0)
            {
                startIdIsEven = false;
                startPixNb--;
            }
            // ---------------------------------------------------------------------------------------------
            // *1.5 because 1 values is coded over 12 bits = 1.5 bytes
            int startPos = static_cast<unsigned int>(vcl_floor(static_cast<double>(startPixNb) * 1.5));

            // ---------------------------------------------------------------------------------------------
            if (stopPixNb % 2 != 0)
            {
                stopIdIsEven = false;
            }

            // ---------------------------------------------------------------------------------------------
            m_File.seekg(m_GridPosition, std::ios::beg);
            m_File.seekg(startPos, std::ios::cur);

            // ---------------------------------------------------------------------------------------------
            for (int colNo = 0; colNo < lNbColumns; colNo = colNo + 2)
            {
                // ---------------------------------------------------------------------------------------------
                m_File.read(dataValue, 3);
                unsigned long int a = static_cast<unsigned long int>((unsigned char) (dataValue[0])) * m_Const256_2
                        + static_cast<unsigned long int>((unsigned char) (dataValue[1])) * m_Const256
                        + static_cast<unsigned long int>((unsigned char) (dataValue[2]));

                // ---------------------------------------------------------------------------------------------
                // extract first binary data
                unsigned long int X1 = a >> static_cast<unsigned long int>(12);
                // ---------------------------------------------------------------------------------------------
                // extract second binary data
                unsigned long int X2 = static_cast<unsigned long int>((a & static_cast<unsigned long int>(4095)));

                // ---------------------------------------------------------------------------------------------
                // Rebuild the real pixel values.
                float pix1 = (m_RefValue + static_cast<float>(X1) * scaleFact_2) * scaleFact_10;
                float pix2 = (m_RefValue + static_cast<float>(X2) * scaleFact_2) * scaleFact_10;

                // ---------------------------------------------------------------------------------------------
                if ((startIdIsEven == false) && (colNo == 0))
                {
                    // ---------------------------------------------------------------------------------------------
                    // Only write the last read element. The first one is outside the wanted area
                    memcpy((void*) (&(p[cpt])), (const void*) (&(pix2)), (size_t) (sizeof(pix2)));
                    cpt += sizeof(pix2);
                    colNo--; // because +2 at each loop whereas here we write only 1 pixel.
                }
                else if ((stopIdIsEven == false) && (colNo == lNbColumns - 1))
                {
                    // ---------------------------------------------------------------------------------------------
                    // Only write the first read element. The last one is outside the wanted area
                    memcpy((void*) (&(p[cpt])), (const void*) (&(pix1)), (size_t) (sizeof(pix1)));
                    cpt += sizeof(pix1);
                    colNo--; // because +2 at each loop whereas here we write only 1 pixel.
                }
                // ---------------------------------------------------------------------------------------------
                else
                {
                    // ---------------------------------------------------------------------------------------------
                    memcpy((void*) (&(p[cpt])), (const void*) (&(pix1)), (size_t) (sizeof(pix1)));
                    cpt += sizeof(pix1);
                    // ---------------------------------------------------------------------------------------------
                    memcpy((void*) (&(p[cpt])), (const void*) (&(pix2)), (size_t) (sizeof(pix2)));
                    cpt += sizeof(pix2);
                }
            }
        }

        m_File.close();
    }

    void
    GRIBImageIO::ReadImageInformation()
    {
        // ---------------------------------------------------------------------------------------------
        // Find the filename (retrieve band Id)

        vnsLogDebugMacro("  GRIB ReadImageInformation m_FileName : "<<m_FileName);
        vnsLogDebugMacro("  GRIB ReadImageInformation m_DatasetNumber : "<<m_DatasetNumber);

        // ---------------------------------------------------------------------------------------------
        // Check file existence and if it is a real file
        bool exist = itksys::SystemTools::FileExists(m_FileName.c_str());
        // ---------------------------------------------------------------------------------------------
        bool isDir = itksys::SystemTools::FileIsDirectory(m_FileName.c_str());
        // ---------------------------------------------------------------------------------------------
        if ((exist != true) || (isDir == true))
        {
            return;
        }

        // ---------------------------------------------------------------------------------------------
        // Look if the file is already open. If yes, close it
        if (m_File.is_open() == true)
        {
            m_File.close();
        }

        // ---------------------------------------------------------------------------------------------
        // Try to open the file.
        m_File.open(m_FileName.c_str(), std::ios::in | std::ios::binary);
        // ---------------------------------------------------------------------------------------------
        if (m_File.fail() == true)
        {
            vnsExceptionDataMacro("GRIBImageIO::ReadImageInformation() failed header open ! ");
        }

        // ---------------------------------------------------------------------------------------------
        //Read header informations
        const bool lResult = InternalReadHeaderInformation(m_File, true);
        vnsLogDebugMacro("         Read  file         : "<< m_FileName);
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("         Size               : "<< m_Dimensions[0]<<","<<m_Dimensions[1]);
        vnsLogDebugMacro("         ComponentType      : "<< this->GetComponentType());
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("         NumberOfComponents : "<< this->GetNumberOfComponents());
        vnsLogDebugMacro("         ComponentSize      : "<< this->GetComponentSize());
        vnsLogDebugMacro("         PixelSize          : "<< this->GetPixelSize());
        vnsLogDebugMacro("         DatasetNumber      : "<< this->GetDatasetNumber());
        vnsLogDebugMacro("         InternalReadHeaderInformation : " << Utilities::BoolToLowerString(lResult));
    }

    bool
    GRIBImageIO::InternalReadHeaderInformation(std::fstream & file, const bool reportError)
    {

        // Class used to read header information as :
        // - filename
        // - size
        // - dataset number
        // - scale factor
        // - grid position
        // - ...

        // indicatrice section size in bytes
        unsigned short l_ind = 8;

        // Check GRIB file : find grill + asked band available
        bool isValidFile = false;
        unsigned int l_Band = 1;

        // Read the metadata dictionary associated to the input image
        itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
        unsigned int l_resol(0);
        bool result = itk::ExposeMetaData<unsigned int>(dict, otb::MetaDataKey::ResolutionFactor, l_resol);
        if (result == false)
        {
            if (reportError != false)
            {
                vnsExceptionDataMacro("Error while reading the MetaDataDictionary!");
            }
        }
        m_DatasetNumber = l_resol;

        while ((file.eof() == false) && ((isValidFile == false) || (l_Band <= m_DatasetNumber)))
        {
            char fileId[5];
            fileId[4] = '\0';
            file.read(fileId, 4);

            // Check if the file indetifier is GRIB
            if (strcmp(fileId, "GRIB") == 0)
            {
                isValidFile = true;
                l_Band++;
            }
        }

        // go to the beginning of the file.
        file.seekg(0, std::ios::beg);

        // If the input file is not a GRIB one
        if (isValidFile == false)
        {
            if (reportError != false)
            {
                vnsExceptionDataMacro("GRIB : No grid detected");
            }
            else
            {
                return false;
            }
        }
        // The number of band section should be equal to the number of resolution
        if (l_Band < m_DatasetNumber)
        {

            if (reportError != false)
            {
                vnsExceptionDataMacro("Invalid param band ID to read, band belong to [1; "<<m_DatasetNumber<<"]");
            }
            else
            {
                return false;
            }
        }

        unsigned short int usi256 = static_cast<unsigned short int>(m_Const256);

        // Go to the asked grid (unsigned long int)(
        l_Band = 1;
        // Go to the asked band section
        while (l_Band < m_DatasetNumber)
        {
            ///// Section indicatrice, 8 bytes */
            // Check if the file is GRIB and valid
            char trashDeb[5];
            trashDeb[4] = '\0';
            file.read(trashDeb, 4);

            char mesSize[4];
            mesSize[3] = '\0';
            file.read(mesSize, 3);

            unsigned long int mesSizeTot = static_cast<unsigned long int>((unsigned char) (mesSize[0])) * m_Const256_2
                    + static_cast<unsigned long int>((unsigned char) (mesSize[1])) * m_Const256
                    + static_cast<unsigned long int>((unsigned char) (mesSize[2]));

            char trash[mesSizeTot - 7 + 1];
            trash[mesSizeTot - 7] = '\0';
            file.read(trash, mesSizeTot - 7);

            l_Band++;
        }

        // Here the pointer is set to GRIB (fourth first octet of indicatrice section)
        //////// In indicatrice section
        // Indicateur
        char trashInd[l_ind + 8 + 1];
        trashInd[l_ind + 8] = '\0';
        file.read(trashInd, l_ind + 8);

        //////// In parameter definition section
        // Indicator parameter
        char indPar[2];
        indPar[1] = '\0';
        file.read(indPar, 1);
        m_ParameterInd = static_cast<short int>(static_cast<unsigned char>(indPar[0]));

        // Facteur d'echelle decimale
        char trashD[17 + 1];
        trashD[17] = '\0';
        file.read(trashD, 17);
        char ds[3];
        ds[2] = '\0';
        file.read(ds, 2);
        m_DecScaleFact = static_cast<unsigned short int>(static_cast<unsigned char>(ds[0])) * usi256
                + static_cast<unsigned short int>((unsigned char) (ds[1]));

        //////// In grid parameter definition section
        // Sizes
        char trashSize[6 + 1];
        trashSize[6] = '\0';
        file.read(trashSize, 6);
        // Size : X
        char sixeXY[2 + 1];
        sixeXY[2] = '\0';
        file.read(sixeXY, 2);

        unsigned short int l_sizeX = static_cast<unsigned short int>(static_cast<unsigned char>(sixeXY[0])) * usi256
                + static_cast<unsigned short int>((unsigned char) (sixeXY[1]));

        // Size : Y
        file.read(sixeXY, 2);
        unsigned short int l_sizeY = static_cast<unsigned short int>(static_cast<unsigned char>(sixeXY[0])) * usi256
                + static_cast<unsigned short int>((unsigned char) (sixeXY[1]));

        //////// In binary data section
        // Scale factor
        char trashScale[26 + 1];
        trashScale[26] = '\0';
        file.read(trashScale, 26);
        char scale[2 + 1];
        scale[2] = '\0';
        file.read(scale, 2);
        unsigned short int l_scale = static_cast<unsigned short int>((unsigned char) (scale[0])) * usi256
                + static_cast<unsigned short int>((unsigned char) (scale[1]));

        // Reference value
        char refVal[4 + 1];
        refVal[4] = '\0';
        file.read(refVal, 4);
        unsigned long int l_refVal = static_cast<unsigned long int>((unsigned char) (refVal[0])) * m_Const256_3
                + static_cast<unsigned long int>((unsigned char) (refVal[1])) * m_Const256_2
                + static_cast<unsigned long int>((unsigned char) (refVal[2])) * m_Const256
                + static_cast<unsigned long int>((unsigned char) (refVal[3]));

        // Move the pointer to be at the grid binary data beginning
        char tmp[1 + 1];
        tmp[1] = '\0';
        file.read(tmp, 1);
        // Store the position to have a faster access to data grid
        m_GridPosition = file.tellg();

        m_ScaleFact = vcl_pow(-1.,
        static_cast<double>((l_scale & ((unsigned long int) (1) << (unsigned long int) (15))) >> (unsigned long int) (15)))
        * (l_scale & (unsigned long int) (32767));

        unsigned long int l_s = (l_refVal & (unsigned long int) (((unsigned long int) (1) << (unsigned long int) (31))))
                >> (unsigned long int) (31);
        unsigned long int l_a = (l_refVal & (unsigned long int) (((unsigned long int) (127) << (unsigned long int) (24))))
                >> (unsigned long int) (24);
        unsigned long int l_m = (l_refVal & (unsigned long int) ((unsigned long int) (16777215)));

        // -----------------------------------------------------
        // Reference value used to build the real pixel value
        // -----------------------------------------------------
        m_RefValue = vcl_pow(-1, static_cast<double>(l_s)) * vcl_pow(2.0, -24.0) * static_cast<double>(l_m)
        * vcl_pow(16.0, static_cast<double>(l_a) - 64.0);

        //Initialization of image information
        m_Dimensions[0] = l_sizeX;
        m_Dimensions[1] = l_sizeY;
        this->SetNumberOfComponents(1);
        this->SetComponentType(FLOAT);
        this->SetFileTypeToBinary();
        this->SetNumberOfDimensions(2);

        return (true);
    }

} // end namespace vns

