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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-7-2 : FA : LAIG-FA-MAC-1555-CNES : 8 fevrier 2016 : Seg fault aleatoire : modif map Cloud    *
 * VERSION : 4-7-2 : FA : LAIG-FA-MAC-1446-CNES : 8 fevrier 2016 : Seg fault aleatoire : modif map Cloud    *
 * VERSION : 4-3-0 : FA : LAIG-FA-MAC-1180-CNES : 21 mai 2015 : Initialisation à zéro inutile				*
 * VERSION : 4-1-2 : FA : LAIG-FA-MAC-1180-CNES : 19 mars 2015 : Pas de corrélation si date courante = date STO	*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskExtensionCorrelationImageFilter_txx
#define __vnsCloudMaskExtensionCorrelationImageFilter_txx

#include "vnsCloudMaskExtensionCorrelationImageFilter.h"

#include "vnsUtilities.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        CloudMaskExtensionCorrelationImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::CloudMaskExtensionCorrelationImageFilter() :
                m_NoData(0.), m_CorrelBandTOCR(0), m_CorrelBandRCR(0), m_LabelCount(0), m_CorrelThreshold(0.), m_MinPixelCorrelation(0), m_JDay(
                        0.)
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(9);
            this->SetNumberOfRequiredInputs(9);
            this->SetNumberOfIndexedOutputs(2);
            this->SetNumberOfRequiredOutputs(2);

            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, OutputImageType::New());

            // Parameters initialization
            m_NCorrel.Fill(3);
            m_CloudFoundMap.clear();
        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        CloudMaskExtensionCorrelationImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::~CloudMaskExtensionCorrelationImageFilter()
        {
        }

    //========================= GenerateInputRequestedRegion ===========================/
    /** GenerateInputRequestedRegion method */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        void
        CloudMaskExtensionCorrelationImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::GenerateInputRequestedRegion()
        {
            // call the superclass' implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // get pointers to the input and output images
            InputMaskPointer l_IPinCLDExtPtr = const_cast<InputMaskType *>(this->GetIPCLDExtInput());
            InputImagePointer l_IPTOCRPtr = const_cast<InputImageType *>(this->GetIPTOCRInput());
            InputImagePointer l_L2inRCRPtr = const_cast<InputImageType *>(this->GetL2inRCRInput());
            InputImagePointer l_L2inSTOPtr = const_cast<InputImageType *>(this->GetL2inSTOInput());
            InputMaskPointer l_IPCLDReflPtr = const_cast<InputMaskType *>(this->GetIPCLDReflInput());
            InputMaskPointer l_IPCLDReflVarPtr = const_cast<InputMaskType *>(this->GetIPCLDReflVarInput());
            LabelImagePointer l_IPCLDExtLabelPtr = const_cast<LabelImageType *>(this->GetIPCLDExtLabelInput());
            InputMaskPointer l_IPEDGPtr = const_cast<InputMaskType *>(this->GetIPEDGInput());
            InputMaskPointer l_L2inNDTPtr = const_cast<InputMaskType *>(this->GetL2inNDTInput());

            OutputImagePointer l_IPCLDExtPtr = this->GetIPCLDExtOutput();
            OutputImagePointer l_IPCLDExtCorelFlagPtr = this->GetIPCLDExtCorelFlagOutput();

            if ((l_IPinCLDExtPtr.IsNull() != false) || (l_IPTOCRPtr.IsNull() != false) || (l_L2inRCRPtr.IsNull() != false)
                    || (l_L2inSTOPtr.IsNull() != false) || (l_IPCLDReflPtr.IsNull() != false) || (l_IPCLDReflVarPtr.IsNull() != false)
                    || (l_IPCLDExtLabelPtr.IsNull() != false) || (l_IPCLDExtPtr.IsNull() != false)
                    || (l_IPCLDExtCorelFlagPtr.IsNull() != false) || (l_IPEDGPtr.IsNull() != false) || (l_L2inNDTPtr.IsNull() != false))
            {
                vnsExceptionBusinessMacro(
                        "CloudMaskExtensionCorrelationImageFilter: Internal error: At least one of the input is null. "<< "( l_IPinCLDExtPtr.IsNull():" << Utilities::BoolToLowerString(l_IPinCLDExtPtr.IsNull()) << "l_IPTOCRPtr.IsNull():" << Utilities::BoolToLowerString(l_IPTOCRPtr.IsNull()) << "l_L2inRCRPtr.IsNull():" << Utilities::BoolToLowerString(l_L2inRCRPtr.IsNull()) << "l_L2inSTOPtr.IsNull():" << Utilities::BoolToLowerString(l_L2inSTOPtr.IsNull()) << "l_IPCLDReflPtr.IsNull():" << Utilities::BoolToLowerString(l_IPCLDReflPtr.IsNull()) << "l_IPCLDReflVarPtr.IsNull():" << Utilities::BoolToLowerString(l_IPCLDReflVarPtr.IsNull()) << "l_IPCLDExtLabelPtr.IsNull():" << Utilities::BoolToLowerString(l_IPCLDExtLabelPtr.IsNull()) << "l_IPCLDExtPtr.IsNull():" << Utilities::BoolToLowerString(l_IPCLDExtPtr.IsNull()) << "l_IPCLDExtCorelFlagPtr.IsNull():" << Utilities::BoolToLowerString(l_IPCLDExtCorelFlagPtr.IsNull()))
            }
            // get a copy of the input requested region (should equal the output
            // requested region)
            RegionType l_IPinCLDExtRequestedRegion;
            RegionType l_IPTOCRRequestedRegion;
            RegionType l_L2inRCRRequestedRegion;
            RegionType l_L2inSTORequestedRegion;
            RegionType l_IPCLDReflRequestedRegion;
            RegionType l_IPCLDReflVarRequestedRegion;
            RegionType l_IPCLDExtLabelRequestedRegion;
            RegionType l_IPEDGRequestedRegion;
            RegionType l_L2inNDTRequestedRegion;

            // pad the input requested region by the operator radius
            SizeType maxRad;
            maxRad[0] = m_NCorrel[0];
            maxRad[1] = m_NCorrel[1];

            // IPinCLDExt Region
            l_IPinCLDExtRequestedRegion = l_IPinCLDExtPtr->GetRequestedRegion();

            l_IPinCLDExtRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPinCLDExtRequestedRegion.Crop(l_IPinCLDExtPtr->GetLargestPossibleRegion()))
            {
                l_IPinCLDExtPtr->SetRequestedRegion(l_IPinCLDExtRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_IPinCLDExtPtr->SetRequestedRegion(l_IPinCLDExtRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of IPCLDExt is (at least partially) outside the largest possible region.");
            }

            // IPTOCR Region
            l_IPTOCRRequestedRegion = l_IPTOCRPtr->GetRequestedRegion();

            l_IPTOCRRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPTOCRRequestedRegion.Crop(l_IPTOCRPtr->GetLargestPossibleRegion()))
            {
                l_IPTOCRPtr->SetRequestedRegion(l_IPTOCRRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_IPTOCRPtr->SetRequestedRegion(l_IPTOCRRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of IPTOCR is (at least partially) outside the largest possible region.");
            }

            // L2inRCR Region
            l_L2inRCRRequestedRegion = l_L2inRCRPtr->GetRequestedRegion();

            l_L2inRCRRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inRCRRequestedRegion.Crop(l_L2inRCRPtr->GetLargestPossibleRegion()))
            {
                l_L2inRCRPtr->SetRequestedRegion(l_L2inRCRRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_L2inRCRPtr->SetRequestedRegion(l_L2inRCRRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of L2inRCR is (at least partially) outside the largest possible region.");
            }

            // L2inSTO Region
            l_L2inSTORequestedRegion = l_L2inSTOPtr->GetRequestedRegion();

            l_L2inSTORequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inSTORequestedRegion.Crop(l_L2inSTOPtr->GetLargestPossibleRegion()))
            {
                l_L2inSTOPtr->SetRequestedRegion(l_L2inSTORequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_L2inSTOPtr->SetRequestedRegion(l_L2inSTORequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of L2inSTO is (at least partially) outside the largest possible region.");
            }

            // IPCLDRefl Region
            l_IPCLDReflRequestedRegion = l_IPCLDReflPtr->GetRequestedRegion();

            l_IPCLDReflRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPCLDReflRequestedRegion.Crop(l_IPCLDReflPtr->GetLargestPossibleRegion()))
            {
                l_IPCLDReflPtr->SetRequestedRegion(l_IPCLDReflRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_IPCLDReflPtr->SetRequestedRegion(l_IPCLDReflRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of IPCLDRefl is (at least partially) outside the largest possible region.");
            }

            // IPCLDReflVar Region
            l_IPCLDReflVarRequestedRegion = l_IPCLDReflVarPtr->GetRequestedRegion();

            l_IPCLDReflVarRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPCLDReflVarRequestedRegion.Crop(l_IPCLDReflVarPtr->GetLargestPossibleRegion()))
            {
                l_IPCLDReflVarPtr->SetRequestedRegion(l_IPCLDReflVarRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_IPCLDReflVarPtr->SetRequestedRegion(l_IPCLDReflVarRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of IPCLDReflVar is (at least partially) outside the largest possible region.");
            }

            // IPCLDExtLabel Region
            l_IPCLDExtLabelRequestedRegion = l_IPCLDExtLabelPtr->GetRequestedRegion();

            l_IPCLDExtLabelRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPCLDExtLabelRequestedRegion.Crop(l_IPCLDExtLabelPtr->GetLargestPossibleRegion()))
            {
                l_IPCLDExtLabelPtr->SetRequestedRegion(l_IPCLDExtLabelRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_IPCLDExtLabelPtr->SetRequestedRegion(l_IPCLDExtLabelRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of IPCLDExtLabel is (at least partially) outside the largest possible region.");
            }

            // IPEDG Region
            l_IPEDGRequestedRegion = l_IPEDGPtr->GetRequestedRegion();

            l_IPEDGRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPEDGRequestedRegion.Crop(l_IPEDGPtr->GetLargestPossibleRegion()))
            {
                l_IPEDGPtr->SetRequestedRegion(l_IPEDGRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_IPEDGPtr->SetRequestedRegion(l_IPEDGRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of IPEDG is (at least partially) outside the largest possible region.");
            }

            // L2inNDT Region
            l_L2inNDTRequestedRegion = l_L2inNDTPtr->GetRequestedRegion();

            l_L2inNDTRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inNDTRequestedRegion.Crop(l_L2inNDTPtr->GetLargestPossibleRegion()))
            {
                l_L2inNDTPtr->SetRequestedRegion(l_L2inNDTRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_L2inNDTPtr->SetRequestedRegion(l_L2inNDTRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of L2inNDT is (at least partially) outside the largest possible region.");
            }
        }

    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        void
        CloudMaskExtensionCorrelationImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::BeforeThreadedGenerateData()
        {
            // Conversion of the correlation threshold parameter that is in percent
            // MACCS 5.0 : Already /100. m_CorrelThreshold /= static_cast<CorrelationPixelType>(100.); // Initial parameter in percent

            // First, clear the functor list
            m_FunctorVector.clear();

            const unsigned int l_NumberOfThreads = this->GetNumberOfThreads();
            m_FunctorVector.reserve(l_NumberOfThreads);

            for (unsigned int i = 0; i < l_NumberOfThreads; i++)
            {
                FunctorType l_Functor;
                l_Functor.SetNoData(m_NoData);
                l_Functor.SetMinPixelCorrelation(m_MinPixelCorrelation);
                l_Functor.SetCorrelBandA(m_CorrelBandTOCR);
                l_Functor.SetCorrelBandB(m_CorrelBandRCR);
                m_FunctorVector.push_back(l_Functor);
            }
        }

    //======================= ThreadedGenerateData =================================/
    /** ThreadedGenerateData method */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        void
        CloudMaskExtensionCorrelationImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {
            itk::ZeroFluxNeumannBoundaryCondition<InputImageType> l_IPTOCRnbC;
            itk::ZeroFluxNeumannBoundaryCondition<InputImageType> l_L2inRCRnbC;
            itk::ZeroFluxNeumannBoundaryCondition<InputImageType> l_L2inSTOnbC;
            itk::ZeroFluxNeumannBoundaryCondition<InputMaskType> l_IPEDGnbC;
            itk::ZeroFluxNeumannBoundaryCondition<InputMaskType> l_L2inNDTnbC;


            // Get pointers to input images
            InputImageConstPointer l_IPTOCRPtr = this->GetIPTOCRInput();
            InputImageConstPointer l_L2inRCRPtr = this->GetL2inRCRInput();
            InputImageConstPointer l_L2inSTOPtr = this->GetL2inSTOInput();

            InputMaskConstPointer l_IPCLDReflPtr = this->GetIPCLDReflInput();
            InputMaskConstPointer l_IPCLDReflVarPtr = this->GetIPCLDReflVarInput();
            InputMaskConstPointer l_IPinCLDExtPtr = this->GetIPCLDExtInput();

            LabelImageConstPointer l_IPCLDExtLabelPtr = this->GetIPCLDExtLabelInput();

            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
            InputMaskConstPointer l_L2inNDTPtr = this->GetL2inNDTInput();

            // Get the output mask
            OutputImagePointer l_IPCLDExtPtr = this->GetIPCLDExtOutput();
            OutputImagePointer l_IPCLDExtCorelFlagPtr = this->GetIPCLDExtCorelFlagOutput();

            // Get the coefficient of correlation
            // CorrelationPixelType l_CorrelationCoef;
            // CorrelationPixelType l_StackCorrelationCoef;

            RadiusType l_Radius;
            l_Radius[0] = m_NCorrel[0];
            l_Radius[1] = m_NCorrel[1];

            // Define the input image region iterators
            itk::ImageRegionConstIterator<InputMaskType>  l_IPCLDReflIt(l_IPCLDReflPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType>  l_IPCLDReflVarIt(l_IPCLDReflVarPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType>  l_IPinCLDExtIt(l_IPinCLDExtPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<LabelImageType> l_IPCLDExtLabelIt(l_IPCLDExtLabelPtr, outputRegionForThread);

            // Define the output image iterators
            // (Images not used for the correlation)
            itk::ImageRegionIterator<OutputImageType> l_OutIPCLDExtIt(l_IPCLDExtPtr, outputRegionForThread);
            itk::ImageRegionIterator<OutputImageType> l_OutIPCLDExtCorelFlagIt(l_IPCLDExtCorelFlagPtr, outputRegionForThread);

            // Define the input neighborhood iterators

            NeighborhoodIteratorType l_IPTOCRIt = itk::ConstNeighborhoodIterator<InputImageType>(l_Radius, l_IPTOCRPtr,
                    outputRegionForThread);
            NeighborhoodIteratorType l_L2inRCRIt = itk::ConstNeighborhoodIterator<InputImageType>(l_Radius, l_L2inRCRPtr,
                    outputRegionForThread);
            NeighborhoodIteratorType l_L2inSTOIt = itk::ConstNeighborhoodIterator<InputImageType>(l_Radius, l_L2inSTOPtr,
                    outputRegionForThread);

            MaskNeighborhoodIteratorType l_IPEDGIt = itk::ConstNeighborhoodIterator<InputMaskType>(l_Radius, l_IPEDGPtr,
                    outputRegionForThread);
            MaskNeighborhoodIteratorType l_L2inNDTIt = itk::ConstNeighborhoodIterator<InputMaskType>(l_Radius, l_L2inNDTPtr,
                    outputRegionForThread);

            // Iterator initializations
            l_IPTOCRIt.GoToBegin();
            l_IPTOCRIt.OverrideBoundaryCondition(&l_IPTOCRnbC);

            l_L2inRCRIt.GoToBegin();
            l_L2inRCRIt.OverrideBoundaryCondition(&l_L2inRCRnbC);

            l_L2inSTOIt.GoToBegin();
            l_L2inSTOIt.OverrideBoundaryCondition(&l_L2inSTOnbC);

            l_IPEDGIt.GoToBegin();
            l_IPEDGIt.OverrideBoundaryCondition(&l_IPEDGnbC);

            l_L2inNDTIt.GoToBegin();
            l_L2inNDTIt.OverrideBoundaryCondition(&l_L2inNDTnbC);

            // Set the image region iterators to the same region of the neighborhood iterators
            l_IPCLDReflIt.GoToBegin();
            l_IPCLDReflVarIt.GoToBegin();
            l_IPinCLDExtIt.GoToBegin();
            l_IPCLDExtLabelIt.GoToBegin();
            l_OutIPCLDExtIt.GoToBegin();
            l_OutIPCLDExtCorelFlagIt.GoToBegin();

            // Reference to the functor
            FunctorType & l_ThreadFunc = m_FunctorVector[threadId];

            static const InputMaskPixelType maskPixel_1(1);
            static const OutputImagePixelType outPixel_0(0);
            static const OutputImagePixelType outPixel_1(1);
            // Pixel loop
            while (l_IPCLDExtLabelIt.IsAtEnd() == false)
            {
                assert(!l_IPCLDReflIt.IsAtEnd());
                assert(!l_IPCLDReflVarIt.IsAtEnd());
                assert(!l_OutIPCLDExtIt.IsAtEnd());
                assert(!l_OutIPCLDExtCorelFlagIt.IsAtEnd());
                assert(!l_IPTOCRIt.IsAtEnd());
                assert(!l_L2inRCRIt.IsAtEnd());
                assert(!l_L2inSTOIt.IsAtEnd());
                assert(!l_IPEDGIt.IsAtEnd());
                assert(!l_L2inNDTIt.IsAtEnd());
                LabelImagePixelType const& l_IPCLDExtLabelValue = l_IPCLDExtLabelIt.Get();
                InputMaskPixelType const& l_IPCLDReflValue = l_IPCLDReflIt.Get();
                InputMaskPixelType const& l_IPCLDReflVarValue = l_IPCLDReflVarIt.Get();

                l_OutIPCLDExtIt.Set(l_IPinCLDExtIt.Get());
                l_OutIPCLDExtCorelFlagIt.Set(outPixel_0);

                // For the part of the cloud already detected, extension flag is set to 0
                // MACCS 4.7.2 : Attention : m_CloudFoundMap[l_IPCLDExtLabelValue] change le map (add key if not present !!! -> no threads safe
                if (m_CloudFoundMap.find(l_IPCLDExtLabelValue) != m_CloudFoundMap.end())
                {
                    if ((l_IPCLDReflValue == maskPixel_1) || (l_IPCLDReflVarValue == maskPixel_1))
                    {
                        l_OutIPCLDExtIt.Set(outPixel_0);
                    }
                }
                // If no pixel has already been detected,
                // the detection by the lower threshold is discarded
                else
                {
                    l_OutIPCLDExtIt.Set(outPixel_0);
                }

                // Correlation

                if (l_OutIPCLDExtIt.Get() == outPixel_1)
                {
                    // Correlation between the input surface reflectance image and the composite product
                    l_ThreadFunc.SetCorrelBandB(m_CorrelBandRCR);
                    // Get the coefficient of correlation
                    CorrelationPixelType l_CorrelationCoef = l_ThreadFunc(l_IPTOCRIt, l_L2inRCRIt, l_IPEDGIt);

                    // Number of bands of the stack of images
                    // LAIG-FA-MAC-1180-CNES
                    const unsigned int l_L2inSTOSize = l_L2inSTOIt.GetCenterPixel().Size();

                    // The correlation is stopped as soon as a good correlation point is obtained
                    for (unsigned int l_band = 0; (l_CorrelationCoef <= m_CorrelThreshold) && (l_band < l_L2inSTOSize); ++l_band)
                    {
                        if (vnsDifferentDoubleMacro(m_STOListOfDates[l_band],m_JDay) == true)
                        {
                            // Set the correlation band of the stack of images
                            l_ThreadFunc.SetCorrelBandB(l_band);
                            // Correlation between the input surface reflectance image and the previous image of date D-1-bd
                            const CorrelationPixelType l_StackCorrelationCoef = l_ThreadFunc(l_IPTOCRIt, l_L2inSTOIt, l_IPEDGIt);

                            // If the correlation was successful
                            if ( vnsDifferentDoubleMacro(l_StackCorrelationCoef,m_NoData) == true)
                            {
                                l_CorrelationCoef = l_StackCorrelationCoef;
                            }
                        }
                    }

                    // Final decision to set the pixel cloudy or not
                    if (l_CorrelationCoef > m_CorrelThreshold)
                    {
                        // The pixel is not cloudy since it correlated with a previous image
                        l_OutIPCLDExtCorelFlagIt.Set(outPixel_1);

                    }
                }

                ++l_IPCLDExtLabelIt;
                ++l_IPinCLDExtIt;
                ++l_IPCLDReflIt;
                ++l_IPCLDReflVarIt;
                ++l_IPTOCRIt;
                ++l_L2inRCRIt;
                ++l_L2inSTOIt;
                ++l_IPEDGIt;
                ++l_L2inNDTIt;
                ++l_OutIPCLDExtIt;
                ++l_OutIPCLDExtCorelFlagIt;

            } // end pixel loop

        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        void
        CloudMaskExtensionCorrelationImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_NoData:                " << m_NoData << std::endl;
            os << "m_LabelCount:            " << m_LabelCount << std::endl;
            os << "m_CorrelBandTOCR:        " << m_CorrelBandTOCR << std::endl;
            os << "m_CorrelBandRCR:         " << m_CorrelBandRCR << std::endl;
            os << "m_NCorrel:               " << m_NCorrel << std::endl;
            os << "m_CorrelThreshold:       " << m_CorrelThreshold << std::endl;
            os << "m_MinPixelCorrelation:   " << m_MinPixelCorrelation << std::endl;
            os << "m_JDay:                  " << m_JDay << std::endl;
            Utilities::PrintList("m_STOListOfDates", m_STOListOfDates, os);

        }

} // End namespace vns

#endif /* __vnsCloudMaskExtensionCorrelationImageFilter_txx */

