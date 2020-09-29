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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 24 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection **
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsCloudGeomMasksToKeptLabelMasksFilter_txx
#define vnsCloudGeomMasksToKeptLabelMasksFilter_txx

#include "vnsCloudGeomMasksToKeptLabelMasksFilter.h"

#include <limits>
#include "itkImageRegionIterator.h"
#include "vnsImageListPixelIterator.h"

namespace vns
{

    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::CloudGeomMasksToKeptLabelMasksFilter() :
                m_ShadowCloudRatioPixNumberThreshold(std::numeric_limits<NbPixelsType>::max()), m_ShadowCloudRatioMin(
                        std::numeric_limits<double>::max()), m_ShadowCloudRatioMax(std::numeric_limits<double>::min())
        {
            this->SetNumberOfRequiredOutputs(1);
            this->SetNthOutput(0, OutputKeptLabelsMasksType::New());
        }

    /*virtual*/
    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        void
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << indent << "ShadowCloudRatioPixNumberThreshold: " << m_ShadowCloudRatioPixNumberThreshold << "\n";
            os << indent << "ShadowCloudRatioMin:                " << m_ShadowCloudRatioMin << "\n";
            os << indent << "ShadowCloudRatioMax:                " << m_ShadowCloudRatioMax << "\n";
        }

    /**
     * GenerateOutputInformation
     */
    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        void
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::GenerateOutputInformation(void)
        {
            InputGeomLabelImageListPointerType inputPtr = this->GetInput();
            OutputKeptLabelsMasksPointerType outputPtr = this->GetOutput();
            if (inputPtr->Size() > 0)
            {
                typename InputGeomLabelImageListType::ConstIterator inputListIt = inputPtr->Begin();
                outputPtr->CopyInformation(inputListIt.Get());
                outputPtr->SetLargestPossibleRegion(inputListIt.Get()->GetLargestPossibleRegion());
            }
        }

    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        void
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::GenerateInputRequestedRegion(void)
        {
            InputGeomLabelImageListPointerType inputPtr = this->GetInput();
            OutputKeptLabelsMasksPointerType outputPtr = this->GetOutput();
            itkAssertInDebugAndIgnoreInReleaseMacro(inputPtr.IsNotNull());
            itkAssertInDebugAndIgnoreInReleaseMacro(outputPtr.IsNotNull());

            typename OutputKeptLabelsMasksType::RegionType requestedRegion = outputPtr->GetRequestedRegion();

            typename InputGeomLabelImageListType::ConstIterator b = inputPtr->Begin();
            typename InputGeomLabelImageListType::ConstIterator e = inputPtr->End();
            while (b != e)
            {
                b.Get()->SetRequestedRegion(requestedRegion);
                // Increment iterator
                ++b;
            }
        }

    /*virtual*/
    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        void
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::GenerateData()
        {
            BeforeGenerateData();
            DoGenerateData();
        }

    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        void
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::BeforeGenerateData()
        {
            typename InputGeomLabelImageListType::Pointer geomLabelImageList = this->GetInput();
            typename OutputKeptLabelsMasksType::Pointer output = this->GetOutput();

            // 1- checks
            itkAssertInDebugAndIgnoreInReleaseMacro(geomLabelImageList.IsNotNull());
            if (geomLabelImageList.IsNull() == true)
            {
                vnsExceptionBusinessMacro("Internal error : The input geomLabelImageList pointer is NULL!")
            }
            itkAssertInDebugAndIgnoreInReleaseMacro(geomLabelImageList->Size() > 0);
            if (geomLabelImageList->Size() == 0)
            {
                vnsExceptionBusinessMacro("Internal error : The input geomLabelImageList is empty!")
            }
            itkAssertInDebugAndIgnoreInReleaseMacro(output.IsNotNull());
            if (m_ShadowCloudRatioPixNumberThreshold == std::numeric_limits<NbPixelsType>::max())
            {
                vnsExceptionBusinessMacro("Internal error : The ShadowCloudRatioPixNumberThreshold is not initialized!")
            }
            itkAssertInDebugAndIgnoreInReleaseMacro(m_ShadowCloudRatioMin <= m_ShadowCloudRatioMax);
            if (m_ShadowCloudRatioMin > m_ShadowCloudRatioMax)
            {
                vnsExceptionBusinessMacro("Internal error : The m_ShadowCloudRatioMin must be lower than m_ShadowCloudRatioMax!")
            }

            // 2- Allocate
            output->SetBufferedRegion(output->GetRequestedRegion());
            output->Allocate();
        }

    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        void
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::DoGenerateData()
        {
            DoBuildLabelLists();
            DoBuildMaskImage();
        }

    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        void
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::DoBuildLabelLists()
        {
            // Loop
            typename NbPixelsForEachLabelMapType::const_iterator b = m_ShadowPixelsNumberMap.begin();
            typename NbPixelsForEachLabelMapType::const_iterator e = m_ShadowPixelsNumberMap.end();
            while (b != e)
            {
                const LabelImagePixelType label = b->first;
                // warning, the next operation may alter the map => this filter is not
                // thread safe!
                const NbPixelsType cloudPixelsNumber = m_CloudPixelsNumberMap[label];
                const double shadowCloudRatio =
                        (cloudPixelsNumber < m_ShadowCloudRatioPixNumberThreshold) ? m_ShadowCloudRatioMax : m_ShadowCloudRatioMin;
                const NbPixelsType shadowPixelsNumber = b->second;

                vnsLogDebugMacro(
                        "label=" << label << " - cldPixNb=" << cloudPixelsNumber << (cloudPixelsNumber < m_ShadowCloudRatioPixNumberThreshold ? "<" : ">=") << m_ShadowCloudRatioPixNumberThreshold << " -> ratio=" << shadowCloudRatio << "...");

                if (shadowPixelsNumber <= shadowCloudRatio * cloudPixelsNumber)
                {
                    // Keep the label
                    m_KeptLabelList.insert(label);
                    vnsLogDebugMacro(
                            " ... shadPixNb="<<shadowPixelsNumber<< "<= shadowCloudRatio * cloudPixelsNumber="<<shadowCloudRatio * cloudPixelsNumber<< " ==> KEEP");
                }
                else
                {
                    // Reject the label
                    m_RejectedLabelList.insert(label);
                    vnsLogDebugMacro(
                            " ...shadPixNb="<<shadowPixelsNumber<< "> shadowCloudRatio * cloudPixelsNumber="<<shadowCloudRatio * cloudPixelsNumber<< " ==> DISCARD");
                }
                // Increment iterator
                ++b;
            }
        }

    template<typename TInputGeomLabelImageList, typename TOutputKeptLabelsMasks>
        void
        CloudGeomMasksToKeptLabelMasksFilter<TInputGeomLabelImageList, TOutputKeptLabelsMasks>::DoBuildMaskImage()
        {
            vnsLogDebugMacro(" building mask image")
            typedef itk::ImageRegionIterator<InputGeomLabelImageType> InputLabelIteratorType;
            typedef ImageListPixelIterator<InputLabelIteratorType> InputLabelListIteratorType;
            typedef typename InputLabelListIteratorType::PixelReferenceType InputPixelType;
            typedef itk::ImageRegionIterator<OutputKeptLabelsMasksType> OutputKeptLabelsMasksIteratorType;
            typedef typename OutputKeptLabelsMasksIteratorType::PixelType OutputPixelType;

            InputGeomLabelImageListPointerType geomLabelImageList = this->GetInput();
            OutputKeptLabelsMasksPointerType output = this->GetOutput();

            itkAssertInDebugAndIgnoreInReleaseMacro(geomLabelImageList.IsNotNull());
            itkAssertInDebugAndIgnoreInReleaseMacro(output.IsNotNull());

            // iterators
            InputLabelListIteratorType inIt(*geomLabelImageList, output->GetRequestedRegion());
            OutputKeptLabelsMasksIteratorType outIt(output, output->GetRequestedRegion());

            const std::size_t N = geomLabelImageList->Size();
            vnsLogDebugMacro("Starting iteration with N="<<N<<"...");
            // Loop
            for (; !inIt.IsAtEnd(); ++inIt, ++outIt)
            {
                itkAssertInDebugAndIgnoreInReleaseMacro(
                        !outIt.IsAtEnd() && "input and output shall have the same number of pixels by construction");

                InputPixelType const& pixel = *inIt;

                bool isSetAndKept = false;
                for (std::size_t i = 0; i != N; ++i)
                {
                    // TODO: extract a function!
                    typename InputPixelType::SubPixelType v = pixel[i];
                    const bool isSet = v > 0;
                    if (isSet && (m_KeptLabelList.count(v) == 1))
                    {
                        isSetAndKept = true;
                        break; // no need to loop again!
                    }
                }
                const OutputPixelType outPix = isSetAndKept ? 1 : 0;
                outIt.Set(outPix);
            }
            itkAssertInDebugAndIgnoreInReleaseMacro(
                    outIt.IsAtEnd() && "input and output shall have the same number of pixels by construction");

            vnsLogDebugMacro("mask image built")
        }
}

#endif // vnsCloudGeomMasksToKeptLabelMasksFilter_txx
