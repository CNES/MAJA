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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * MOD : VERSION : 1-0-0  : 24 nov. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConcatenatePerZoneVectorImageFilter_txx
#define __vnsConcatenatePerZoneVectorImageFilter_txx

#include "vnsConcatenatePerZoneVectorImageFilter.h"
#include "vnsLoggers.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TOutputImage>
        ConcatenatePerZoneVectorImageFilter<TInputImage, TInputMask, TOutputImage>::ConcatenatePerZoneVectorImageFilter()
        {
            this->SetNthOutput(0, OutputImageType::New());
            m_DefaultIndex = 99;
            m_NoData = 0;
        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TOutputImage>
        ConcatenatePerZoneVectorImageFilter<TInputImage, TInputMask, TOutputImage>::~ConcatenatePerZoneVectorImageFilter()
        {
        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ConcatenatePerZoneVectorImageFilter<TInputImage, TInputMask, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            this->Superclass::PrintSelf(os, indent);
        }

    /** Set List of zone */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ConcatenatePerZoneVectorImageFilter<TInputImage, TInputMask, TOutputImage>::SetListOfZones(Utilities::ListOfUIntegers zones)
        {
            const unsigned int nbZones = zones.size();
            for (unsigned int i = 0; i < nbZones; i++)
            {
                m_ImageIndexMap[zones[i]] = i;
            }
        }

    /** GenerateOutputInformation */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ConcatenatePerZoneVectorImageFilter<TInputImage, TInputMask, TOutputImage>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();
            OutputImagePointer output = this->GetOutput();
            InputImageListConstPointer inputList = this->GetInputList();
            inputList->GetNthElement(0)->UpdateOutputInformation();
            output->SetNumberOfComponentsPerPixel(inputList->GetNthElement(0)->GetNumberOfComponentsPerPixel());
        }

    /** GenerateInputRequestedRegion */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ConcatenatePerZoneVectorImageFilter<TInputImage, TInputMask, TOutputImage>::GenerateInputRequestedRegion()
        {
            Superclass::GenerateInputRequestedRegion();
            InputImageListConstPointer inputList = this->GetInputList();
            RegionType inputRegion;
            this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetInputMask()->GetRequestedRegion());
            const unsigned int nbImages = inputList->Size();
            for (unsigned int i = 0; i < nbImages; i++)
            {
                inputList->GetNthElement(i)->SetRequestedRegion(inputRegion);
            }
        }

    /** Update Data */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        /*ConcatenatePerZoneVectorImageFilter<TInputImage, TInputMask, TOutputImage>::ThreadedGenerateData(
         const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType threadId)*/
        ConcatenatePerZoneVectorImageFilter<TInputImage, TInputMask, TOutputImage>::GenerateData()
        {

            // Get the Input
            InputImageListConstPointer inputList = this->GetInputList();
            InputMaskConstPointer inputMask = this->GetInputMask();
            // Get the Ouput
            OutputImagePointer output = this->GetOutput();
            //output->SetNumberOfComponentsPerPixel(inputList->GetNthElement(0)->GetNumberOfComponentsPerPixel());
            // Allocate buffer
            output->SetBufferedRegion(output->GetRequestedRegion());
            this->AllocateOutputs();

            // Init iterator
            OutputImageIteratorType outIt(output, output->GetRequestedRegion());
            InputMaskConstIteratorType maskIt(inputMask, inputMask->GetRequestedRegion());

            // Set the list of inputs by zone
            std::vector<InputImageConstIteratorType> inputItList;
            const unsigned int nbZone = inputList->Size();
            for (unsigned int i = 0; i < nbZone; i++)
            {
                inputList->GetNthElement(i)->Update();
                InputImageConstIteratorType inIt(inputList->GetNthElement(i), inputList->GetNthElement(i)->GetRequestedRegion());
                inIt.GoToBegin();
                inputItList.push_back(inIt);
            }

            // Set the iterator to begin
            outIt.GoToBegin();
            maskIt.GoToBegin();

            // Init the default pixel
            OutputImagePixelType defaultPixel;
            defaultPixel.SetSize(inputList->GetNthElement(0)->GetNumberOfComponentsPerPixel());
            defaultPixel.Fill(m_NoData);
            // Loop on the region
            while (outIt.IsAtEnd() != true)
            {
                // Initialze the output pixel value
                outIt.Set(defaultPixel);
                InputMaskPixelType maskValue = maskIt.Get();
                unsigned int imageIndex = this->GetImageIndexFromZone(static_cast<unsigned int>(maskValue));

                if (imageIndex != m_DefaultIndex)
                {
                    outIt.Set(inputItList[imageIndex].Get());
                }

                for (unsigned int i = 0; i < nbZone; i++)
                {
                    ++inputItList[i];
                }
                // increment iterators
                ++outIt;
                ++maskIt;
            }
        }

} // End namespace vns

#endif /* __vnsConcatenatePerZoneVectorImageFilter_txx */
