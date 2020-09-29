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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 2 mai 2016 : Correction warning, qualite, etc             *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-127944-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-372-CNES : 18 avril 2014 : Decoupage des methodes trop complexes.      *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 3-0-0 : FA : LAIG-FA-MAC-371-CNES : 05 octobre 2012 : Correction qualite : taux de commentaire *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ***********************************************************************************************************/
#ifndef __vnsLookUpTableFileReader_txx
#define __vnsLookUpTableFileReader_txx

#include "vnsLookUpTableFileReader.h"
#include "itksys/SystemTools.hxx"
#include "itkIndex.h"
#include "itkMacro.h"
#include "vnsSystem.h"

namespace vns
{

    // Constructor
    template<class TLookUpTableType>
        LookUpTableFileReader<TLookUpTableType>::LookUpTableFileReader()
        {
            // Setup the number of required inputs.
            this->SetNumberOfRequiredInputs(0);
            this->SetNumberOfIndexedInputs(0);
            m_LUT = LookUpTableType::New();
            m_LUT->SetNumberOfComponentsPerPixel(1);
        }

    template<class TLookUpTableType>
        bool
        LookUpTableFileReader<TLookUpTableType>::IsLookUpTableFile(int num)
        {
            // ---------------------------------------------------------------------------------------------
            std::string lFilename = this->GetBandFileName(num);
            // If empty filename, throws an exception
            if (lFilename == "")
            {
                vnsLogErrorMacro("No Look up Table file specified.");
                return false;
            }

            // ---------------------------------------------------------------------------------------------
            // If file don't exist, throws an exception
            if (itksys::SystemTools::FileExists(lFilename.c_str()) == false)
            {
                vnsLogErrorMacro("Look up Table file "<< lFilename<<" doesn't exist.");
                return false;
            }

            // ---------------------------------------------------------------------------------------------
            // If the filename is not a file (ex: directory, ...)
            if (itksys::SystemTools::FileIsDirectory(lFilename.c_str()) == true)
            {
                vnsLogErrorMacro("Look up Table file "<< lFilename<<" is not a file.");
                return false;
            }

            return true;
        }

    // Parse the LUT file and generate the LookUpTable.
    template<class TLookUpTableType>
        void
        LookUpTableFileReader<TLookUpTableType>::GenerateLUT(void)
        {

            // Initialization
            const unsigned int lNbBands(m_BandFileName.size());
            const unsigned int lSize(m_LUT->GetNumberOfComponentsPerPixel());
            //std::fstream lFile;
            // ----------------------------------------------------------
            // Check the number of spectral band of the LUT
            // ----------------------------------------------------------
            if (lNbBands != lSize)
            {
                vnsExceptionDataMacro(
                        "LookUpTableFileReader::GenerateData() : the number of bands is not correct : "<<lNbBands <<" it should be : "<<lSize);
            }

            // ----------------------------------------------------------
            // Open the binary files
            // ----------------------------------------------------------
            Utilities::ListOfListOfFloats l_BuffersBands;
            for (unsigned int lNumBand = 0; lNumBand < lNbBands; lNumBand++)
            {
                // std::fstream* plFile = new std::fstream();
                std::string lFilename = this->GetBandFileName(lNumBand);
                vnsLogDebugMacro("GenerateLUT: Reading file for GenerateData(): " << lFilename);

                // Check if one LUT file exists for each spectral band
                bool isLutFile = this->IsLookUpTableFile(lNumBand);
                if (isLutFile == false)
                {
                    vnsExceptionDataMacro("LookUpTableFileReader::GenerateData() : Invalid Look up Table file : "<<lFilename<<".");
                }
                // ----------------------------------------------------------
                // Read the binary file
                // ----------------------------------------------------------
                l_BuffersBands.push_back(System::ReadBinaryFileIntoVector<float>(lFilename));
                vnsLogSuperDebugMacro("GenerateLUT: Reading '"<<l_BuffersBands.back().size() <<"' values for filename " << lFilename << ".");

            }
            // ----------------------------------------------------------
            // Check nb value for each bands
            const std::size_t l_NbValues = l_BuffersBands[0].size();
            for (unsigned int lNumBand = 1; lNumBand < lNbBands; lNumBand++)
            {
                if (l_BuffersBands[lNumBand].size() != l_NbValues)
                {
                    vnsExceptionDataMacro(
                            "Error: incoherence between nb value read from the band " << lNumBand + 1 << " and the fnb value "<< l_NbValues << " from the first band in the binaries file of the LookUpTable !")
                }
            }

            // ----------------------------------------------------------
            // Fill the LUT
            // ----------------------------------------------------------
            // The LUT that provides surface reflectance depends on several parameters
            // (TOA reflectance, AOT, altitude, viewing and solar zenith angles, relative azimuth angle)
            // For Venus, only the first three parameters varies and the other ones are set
            // to a constant value In order to accelerate the interpolation,
            // ----------------------------------------------------------
            // Reorganize the LUT :
            // TOA reflectance value -> AOT value -> altitude value -> one value for each spectral band
            // ----------------------------------------------------------
            this->GenerateXDLUT(this->GetLUT(), l_BuffersBands, lNbBands);
        }

    //
    template<class TLookUpTableType>
        void
        LookUpTableFileReader<TLookUpTableType>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            // Print
            Superclass::PrintSelf(os, indent);
        }

} // end namespace itk

#endif
