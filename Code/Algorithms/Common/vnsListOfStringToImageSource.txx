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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 6 juillet 2016 : Audit code - atoi, atof, atol            *
 * VERSION : 4-8-0 : DM : LAIG-DM-MAC-1661-CNES : 23 mars 2016 : Extrapolation des valeurs s'ils en manquent*
 * VERSION : 4-8-0 : FA : LAIG-FA-MAC-1652-CNES : 23 mars 2016 : Extrapolation des valeurs s'ils en manquent*
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 15 novembre 2013 : Correction information géographique     *
 * 								Ajout de la projection dans l'image de sortie								*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsListOfStringToImageSource_txx
#define __vnsListOfStringToImageSource_txx

#include "vnsListOfStringToImageSource.h"

#include "itkImageRegionIterator.h"
#include "itkObjectFactory.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

#include "vnsMacro.h"
#include "vnsUtilities.h"

namespace vns
{

    /**
     *
     */
    template<class TOutputImage>
        ListOfStringToImageSource<TOutputImage>::ListOfStringToImageSource()
        {

            m_InternalListOfString.clear();
            //Initial image is 64 wide in each direction.
            for (unsigned int i = 0; i < TOutputImage::GetImageDimension(); i++)
            {
                m_Size[i] = 64;
                m_Spacing[i] = 1.0;
                m_Origin[i] = 0.0;
            }
            m_ProjectionRef = "";
        }

    template<class TOutputImage>
        ListOfStringToImageSource<TOutputImage>::~ListOfStringToImageSource()
        {
        }

    template<class TOutputImage>
        void
        ListOfStringToImageSource<TOutputImage>::SetListOfString(const ListOfStrings & list, const bool p_ActivateDataExtrapolation)
        {
            // Get the number of line in the image
            const unsigned int l_InputSizeY = list.size();
            // Get the first line
            const std::string first_line = list.at(0);
            // Split the first line and computes the number of values in the line
        	const ListOfStrings line_of_values = Utilities::SplitString(first_line, ' ', false /*remove_empty*/);
            const unsigned int l_InputSizeX = line_of_values.size();

            ListOfStrings l_OutputLine;
            m_InternalListOfString.clear();
            if (p_ActivateDataExtrapolation == false)
            {
                // Loops on the size Y (nb lines)
                for (unsigned int i = 0; i < l_InputSizeY; i++)
                {
                    // Split the line
                	const ListOfStrings values = Utilities::SplitString(list.at(i), ' ', false /*remove_empty*/);
                    // CHecks the number of values contained in the line
                    if (l_InputSizeX != values.size())
                    {
                        vnsExceptionBusinessMacro(
                                "ListOfStringToImageSource Internal Error: The size '"<<values.size()<<"' of the line '"<<i<<"' is different than the official line computed '"<<l_InputSizeX<<"' !")
                    }
                    // Loops on the values and copy to the internal list of string values
                    for (unsigned int j = 0; j < l_InputSizeX; j++)
                    {
                        m_InternalListOfString.push_back(values.at(j));
                    }

                }
                // Initialize the Size of the Image
                m_Size[0] = l_InputSizeX;
                m_Size[1] = l_InputSizeY;            // Add a line int the Output Image

            }
            else
            {
                // Effet miroir pour compenser manque recouvrement. A SUPPRIMER lorsque les produits S2 (natif, GPP) seront correctes
                //---------------------------------------------------------------------------
                // 1. Replace the NaN value by the nearest value
                // 2. For solve Expand Edge Padding Value for the following algorithms
                //     * Add a value at the end of each line
                //     * Add a new line at the end
                //  NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN
                //  NaN NaN NaN NaN 4.7 8.9 7.4 NaN NaN NaN NaN NaN
                //  NaN NaN NaN 4.2 4.5 1.5 NaN NaN NaN NaN NaN NaN
                //  NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN
                vnsLogDebugMacro("*************************************************************************************************")
                vnsLogDebugMacro("ATTENTION: code pour lecture ListOfStringToImageSource SENTINEL2 : extrapolation (simple padding) !!!")
                vnsLogDebugMacro("**************************************************************************************************")
                unsigned int l_NumberOfUpperEmptyLine(0);
                bool l_AreaNumericValueValideAlreadyFounded(false);

                // Test if the image contains only NaN
                //  NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN
                //  NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN
                //  NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN
                //  NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN
                //  NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN NaN
                bool l_ImageWithOnlyNaN = true;
                for (unsigned int i = 0; ((i < l_InputSizeY) && (l_ImageWithOnlyNaN == true)); i++)
                {
                    //---------------------------------------------------------------------------
                    // Find the first non "NaN" value
                	const ListOfStrings line = Utilities::SplitString(list.at(i), ' ', false /*remove_empty*/);
                    const unsigned int l_LineSize = line.size();
                    if (l_LineSize != l_InputSizeX)
                    {
                        vnsExceptionBusinessMacro(
                                "ListOfStringToImageSource Internal Error: The size '"<<l_LineSize<<"' of the line '"<<i<<"' is different than the official line computed '"<<l_InputSizeX<<"' !")
                    }
                    for (unsigned int j = 0; ((j < l_InputSizeX) && (l_ImageWithOnlyNaN == true)); j++)
                    {
                        if (line.at(j) != "NaN")
                        {
                            l_ImageWithOnlyNaN = false;
                        }
                    }
                }
                if (l_ImageWithOnlyNaN == true)
                {
                    for (unsigned int i = 0; i < l_InputSizeY; i++)
                    {
                        //---------------------------------------------------------------------------
                        // Find the first non "NaN" value
                    	const ListOfStrings line = Utilities::SplitString(list.at(i), ' ', false /*remove_empty*/);
                        const unsigned int l_LineSize = line.size();
                        for (unsigned int j = 0; j < l_LineSize; j++)
                        {
                            m_InternalListOfString.push_back("0");
                        }
                    }
                    // Initialize the Size of the Image
                    m_Size[0] = l_InputSizeX;
                    m_Size[1] = l_InputSizeY;                // Add a line int the Output Image
                }
                else
                {

                    for (unsigned int i = 0; i < l_InputSizeY; i++)
                    {
                        //---------------------------------------------------------------------------
                        // Find the pos of the first "NaN"
                    	const ListOfStrings line = Utilities::SplitString(list.at(i), ' ', false /*remove_empty*/);
                        const int l_LineSize = line.size();
                        int posFirst(0);
                        int j(0);
                        bool stop(false);
                        while ((stop == false) && (j < l_LineSize))
                        {
                            if (line.at(j) != "NaN")
                            {
                                stop = true;
                                posFirst = j;
                            }
                            j++;
                        }
                        // No valid data on the line
                        if (stop == false)
                        { //We should copy the next valide line (which contains one or more numerical values)
                            if (l_AreaNumericValueValideAlreadyFounded == false)
                            {
                                l_NumberOfUpperEmptyLine++;
                            }
                            else
                            {
                                // Add the last line computed
                                const unsigned int l_OutputLineSize = l_OutputLine.size();
                                for (unsigned int i = 0; i < l_OutputLineSize; i++)
                                {
                                    m_InternalListOfString.push_back(l_OutputLine.at(i));
                                }
                            }
                        }
                        else

                        {
                            //---------------------------------------------------------------------------
                            // Find the pos of the last "NaN"
                            stop = false;
                            j = l_LineSize - 1;
                            int posLast(j);
                            while ((stop == false) && (j >= 0))
                            {
                                if (line.at(j) != "NaN")
                                {
                                    stop = true;
                                    posLast = j;
                                }
                                j--;
                            }

                            //---------------------------------------------------------------------------
                            // Gen the good line
                            const std::string l_FirstValidValue(line.at(posFirst));
                            const std::string l_LastValidValue(line.at(posLast));
                            l_OutputLine.clear();
                            for (int j = 0; j < l_LineSize; j++)
                            {
                                if (j < posFirst)
                                {
                                    l_OutputLine.push_back(l_FirstValidValue);
                                }
                                else if (j > posLast)
                                {
                                    l_OutputLine.push_back(l_LastValidValue);
                                }
                                else //Copy

                                {
                                    l_OutputLine.push_back(line.at(j));
                                }
                            }
                            // Check the size of the line
                            if (l_InputSizeX != l_OutputLine.size())
                            {
                                vnsExceptionBusinessMacro(
                                        "ListOfStringToImageSource Internal Error: The size '"<<l_OutputLine.size()<<"' of the line '"<<i<<"' is different than the official line computed '"<<l_InputSizeX<<"' !")
                            }

                            // Add a column = copy of the last // For Expand problem
                            std::string l_LastValue = l_OutputLine.at(l_OutputLine.size() - 1);
                            l_OutputLine.push_back(l_LastValue);
                            const unsigned int l_OutputLineSize = l_OutputLine.size();
                            if (l_AreaNumericValueValideAlreadyFounded == false)
                            {
                                //Copy l_NumberOfUpperEmptyLine the first valid line computed
                                for (unsigned int cpt = 0; cpt < l_NumberOfUpperEmptyLine; cpt++)
                                {
                                    for (unsigned int i = 0; i < l_OutputLineSize; i++)
                                    {
                                        m_InternalListOfString.push_back(l_OutputLine.at(i));
                                    }
                                }
                                l_AreaNumericValueValideAlreadyFounded = true;
                                l_NumberOfUpperEmptyLine = 0;
                            }
                            // Add the current line
                            for (unsigned int i = 0; i < l_OutputLineSize; i++)
                            {
                                m_InternalListOfString.push_back(l_OutputLine.at(i));
                            }
                        }
                    }

                    // Add a line = copy of the last // For Expand problem
                    const unsigned int l_OutputLineSize = l_OutputLine.size();
                    for (unsigned int i = 0; i < l_OutputLineSize; i++)
                    {
                        m_InternalListOfString.push_back(l_OutputLine.at(i));
                    }
                    m_Size[0] = l_InputSizeX + 1;
                    m_Size[1] = l_InputSizeY + 1; // Add a line int the Output Image

                }
            }
            // Check the size of the line
            if ((m_Size[0] * m_Size[1]) != m_InternalListOfString.size())
            {
                vnsExceptionBusinessMacro(
                        "ListOfStringToImageSource Internal Error: The 'm_InternalListOfString' output size '"<<m_InternalListOfString.size()<<"' is different than the official output image size "<<m_Size[0]<<"*"<<m_Size[1]<<" !")
            }
            this->Modified();
        }

    template<class TOutputImage>
        void
        ListOfStringToImageSource<TOutputImage>::SetProjectionRef(std::string projection)
        {
            // Get the metadata dictionary of the image source
            itk::MetaDataDictionary & dict = this->GetOutput(0)->GetMetaDataDictionary();
            itk::EncapsulateMetaData<std::string>(dict, otb::MetaDataKey::ProjectionRefKey, projection);
            m_ProjectionRef = projection;
        }

    template<class TOutputImage>
        const std::string
        ListOfStringToImageSource<TOutputImage>::GetProjectionRef() const
        {
            // Get the projection if it has been set before !
            return this->m_ProjectionRef;
        }

    /**
     *
     */
    template<class TOutputImage>
        void
        ListOfStringToImageSource<TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            unsigned int i;
            // Print the origin
            os << indent << "Origin: [";
            for (i = 0; i < TOutputImage::ImageDimension - 1; i++)
            {
                os << m_Origin[i] << ", ";
            }
            os << m_Origin[i] << "]" << std::endl;

            // Print the spacing
            os << indent << "Spacing: [";
            for (i = 0; i < TOutputImage::ImageDimension - 1; i++)
            {
                os << m_Spacing[i] << ", ";
            }
            os << m_Spacing[i] << "]" << std::endl;

            // Print the size
            os << indent << "Size: [";
            for (i = 0; i < TOutputImage::ImageDimension - 1; i++)
            {
                os << m_Size[i] << ", ";
            }
            os << m_Size[i] << "]" << std::endl;
        }

    //----------------------------------------------------------------------------
    template<typename TOutputImage>
        void
        ListOfStringToImageSource<TOutputImage>::GenerateOutputInformation()
        {
            TOutputImage *output;
            IndexType index;
            index.Fill(0);

            // Get the output
            output = this->GetOutput(0);

            typename TOutputImage::RegionType largestPossibleRegion;
            // Sert the output largest possible region
            largestPossibleRegion.SetSize(this->m_Size);
            largestPossibleRegion.SetIndex(index);
            output->SetLargestPossibleRegion(largestPossibleRegion);

            // Set the spacing of the image
            output->SetSignedSpacing(m_Spacing);
            // Set the origin of the image
            output->SetOrigin(m_Origin);
        }

    //----------------------------------------------------------------------------
    template<typename TOutputImage>
        void
        ListOfStringToImageSource<TOutputImage>::GenerateData()
        {
            typedef typename TOutputImage::PixelType scalarType;
            typename TOutputImage::Pointer image = this->GetOutput(0);

            // Allocates the image
            image->SetBufferedRegion(image->GetRequestedRegion());
            image->Allocate();

            // Get an iterator on the image
            itk::ImageRegionIterator<TOutputImage> it(image, image->GetRequestedRegion());
            it.GoToBegin();
            ListOfStrings::const_iterator stringIt;
            stringIt = m_InternalListOfString.begin();

            // Loops on the list of string and convertes the value to "float" and after in the output pixel type
            while (it.IsAtEnd() != true)
            {
                it.Set(static_cast<scalarType>(Utilities::AsFloat(*stringIt)));
                ++it;
                ++stringIt;
            }
        }

} // end namespace vns

#endif
