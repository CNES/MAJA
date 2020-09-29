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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 sept. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsMapLookUpTableFileReader_txx
#define vnsMapLookUpTableFileReader_txx

#include "vnsMapLookUpTableFileReader.h"
#include "vnsUtilities.h"
#include "vnsDate.h"
namespace vns
{

// Constructor
    template<class TLookUpTableType>
        MapLookUpTableFileReader<TLookUpTableType>::MapLookUpTableFileReader()
        {
        }

// Destructor
    template<class TLookUpTableType>
        MapLookUpTableFileReader<TLookUpTableType>::~MapLookUpTableFileReader()
        {
        }

    template<class TLookUpTableType>
        void
        MapLookUpTableFileReader<TLookUpTableType>::SetListOfFilenames(const ListOfFilenames & pList, const std::string & pReferenceDate)
        {
            // ---------------------------------------------------------------------------------------------
            // Set the list of filenames
            m_ListOfFilenames = pList;
            // ---------------------------------------------------------------------------------------------
            // Get the reference date
            const unsigned int lJDayRef = Date::GetJulianDayAsInteger(Date::GetTmFromDateYYYYMMDD(pReferenceDate));
            // ---------------------------------------------------------------------------------------------
            vnsLogDebugMacro("LookUpTable, DayRef <"<<lJDayRef<<">.")
            // ---------------------------------------------------------------------------------------------
            const unsigned int lSize = m_ListOfFilenames.size();
            // ---------------------------------------------------------------------------------------------
            for (unsigned int i = 0; i < lSize; i++)
            {
                // ---------------------------------------------------------------------------------------------
                unsigned int lJDay = Utilities::GetIntegerDateInternalFilename(m_ListOfFilenames[i]);
                if (lJDay < lJDayRef)
                {
                    vnsExceptionDataMacro("Cannot fill the LookUpTable with filename <"<<m_ListOfFilenames[i] << "> detected at Julian Day " << lJDay << " as it's prior to reference date " << pReferenceDate << " (" << lJDayRef<< ").");
                }
                // ---------------------------------------------------------------------------------------------
                // Extract date from the filename and convert into JulianDay
                vnsLogDebugMacro("Fill the LookUpTable with filename <"<<m_ListOfFilenames[i]<<"> and with the date detected (in filename) <"<<lJDay<<">.");
                lJDay = lJDay - lJDayRef;
                // ---------------------------------------------------------------------------------------------
                m_ListOfJulianDays.push_back(lJDay);
            }
        }

    template<class TLookUpTableType>
        void
        MapLookUpTableFileReader<TLookUpTableType>::AddParameterValues(const ParameterValuesType &NewParamValues)
        {
            vnsLogDebugMacro("LookUpTable <- parameter value: "<<NewParamValues.ParameterName);
            m_ParametersValues.push_back(NewParamValues);
        }

    template<class TLookUpTableType>
        void
        MapLookUpTableFileReader<TLookUpTableType>::GenerateLUTMap(void)
        {

            // ---------------------------------------------------------------------------------------------
            vnsLogDebugMacro("LookUpTable::GenerateLUTMap begin")
            // ---------------------------------------------------------------------------------------------
            // Check that the number of LUTs is equal to the number of dates
            // to use the date as the key of the map
            const unsigned int lNbOfFilenames = m_ListOfFilenames.size();
            const unsigned int lNbOfParams = m_ParametersValues.size();
            // ---------------------------------------------------------------------------------------------
            if (m_ListOfJulianDays.size() != lNbOfFilenames)
            {
                vnsExceptionDataMacro(
                        "MapLookUpTableFileReader::GenerateLUTMap: Internal error: The Number of LUT filenames  ("<<lNbOfFilenames<<") is different than the list of Julian Days ("<<m_ListOfJulianDays.size()<<") !")
            }
            // ---------------------------------------------------------------------------------------------
            for (unsigned int i = 0; i < lNbOfFilenames; i++)
            {
                // ---------------------------------------------------------------------------------------------
                // LUT Reader typedefs
                typedef itk::ImageFileReader<LookUpTableType> ImageFileReaderType;
                typename ImageFileReaderType::Pointer l_Reader = ImageFileReaderType::New();
                l_Reader->SetFileName(m_ListOfFilenames[i]);
                // ---------------------------------------------------------------------------------------------
                // Load the LUT
                l_Reader->Update();

                // ---------------------------------------------------------------------------------------------
                // Get pointer to the output LUT
                LookUpTablePointerType lNewLUT = l_Reader->GetOutput();

                // ---------------------------------------------------------------------------------------------
                // Set indexes
                for (unsigned int j = 0; j < lNbOfParams; j++)
                {
                    lNewLUT->AddParameterValues(m_ParametersValues[j]);
                }

                // ---------------------------------------------------------------------------------------------
                // And add into the map
                m_LUTMap[m_ListOfJulianDays[i]] = lNewLUT;
                vnsLogDebugMacro("LUT[reader#" << i << ": " << m_ListOfJulianDays[i]<< " <- new lut")
            }
            vnsLogDebugMacro("LookUpTable::GenerateLUTMap end")
        }

} // End namespace vns

#endif /* vnsMapLookUpTableFileReader_txx */
