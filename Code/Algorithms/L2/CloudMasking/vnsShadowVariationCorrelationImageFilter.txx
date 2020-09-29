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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowVariationCorrelationImageFilter_txx
#define __vnsShadowVariationCorrelationImageFilter_txx

#include "vnsShadowVariationCorrelationImageFilter.h"
#include "vnsMath.h"

#include "otbImage.h"

#include "itkImageRegionIterator.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TOutputImage>
        ShadowVariationCorrelationImageFilter<TInputImage, TInputMask, TOutputImage>::ShadowVariationCorrelationImageFilter()
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(7);
            this->SetNumberOfRequiredInputs(7);
            this->SetNumberOfIndexedOutputs(1);

            this->SetNthOutput(0, OutputImageType::New());

            // Parameters initialization
            m_NoData = static_cast<RealNoDataType>(0);
            m_RedBandTOCR = 0;
            m_RedBandRCR = 0;

            m_JDay = 0.;
            m_ForgettingDuration = 0.;
            m_MinShadowVariation = 0.;
            m_MinPixelCorrelation = 0;
            m_CorrelBandTOCR = 0;
            m_CorrelBandRCR = 0;
            m_CorrelThreshold = 0.;

            m_NCorrel.Fill(3);

        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TOutputImage>
        ShadowVariationCorrelationImageFilter<TInputImage, TInputMask, TOutputImage>::~ShadowVariationCorrelationImageFilter()
        {
        }

    //****************************** GenerateInputRequestedRegion ********************************/
    /** GenerateInputRequestedRegion method */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ShadowVariationCorrelationImageFilter<TInputImage, TInputMask, TOutputImage>::GenerateInputRequestedRegion()
        {

            // call the superclass' implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // get pointers to the input and output images
            InputImagePointer l_IPTOCRPtr = const_cast<InputImageType *>(this->GetIPTOCRInput());
            InputImagePointer l_L2inRCRPtr = const_cast<InputImageType *>(this->GetL2inRCRInput());
            InputImagePointer l_L2inSTOPtr = const_cast<InputImageType *>(this->GetL2inSTOInput());
            InputMaskPointer l_IPEDGPtr = const_cast<InputMaskType *>(this->GetIPEDGInput());
            InputMaskPointer l_L2inNDTPtr = const_cast<InputMaskType *>(this->GetL2inNDTInput());
            InputMaskPointer l_L2inWASPtr = const_cast<InputMaskType *>(this->GetL2inWASInput());
            InputMaskPointer l_IPCLDShadVarInPtr = const_cast<InputMaskType *>(this->GetIPCLDShadVarInput());

            OutputImagePointer l_IPCLDShadVarPtr = this->GetIPCLDShadVarOutput();

            if ((l_IPTOCRPtr.IsNull() == true) || (l_L2inRCRPtr.IsNull() == true) || (l_L2inSTOPtr.IsNull() == true)
                    || (l_IPEDGPtr.IsNull() == true) || (l_L2inNDTPtr.IsNull() == true)
                    || (l_L2inWASPtr.IsNull() == true) || (l_IPCLDShadVarInPtr.IsNull() == true) || (l_IPCLDShadVarPtr.IsNull() == true))
            {
                vnsExceptionBusinessMacro("ShadowVariationCorrelationImageFilter: Internal error: One (or more) Input pointer is NULL !")
            }
            // get a copy of the input requested region (should equal the output
            // requested region)
            RegionType l_IPTOCRRequestedRegion;
            RegionType l_L2inRCRRequestedRegion;
            RegionType l_L2inSTORequestedRegion;
            RegionType l_IPEDGRequestedRegion;
            RegionType l_L2inNDTRequestedRegion;
            RegionType l_L2inWASRequestedRegion;
            RegionType l_IPCLDShadVarInRequestedRegion;
            RegionType l_IPCLDShadVarRequestedRegion;

            // pad the input requested region by the operator radius
            SizeType maxRad;
            maxRad[0] = m_NCorrel[0];
            maxRad[1] = m_NCorrel[1];

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

            // L2inWAS Region
            l_L2inWASRequestedRegion = l_L2inWASPtr->GetRequestedRegion();

            l_L2inWASRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inWASRequestedRegion.Crop(l_L2inWASPtr->GetLargestPossibleRegion()))
            {
                l_L2inWASPtr->SetRequestedRegion(l_L2inWASRequestedRegion);

            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_L2inWASPtr->SetRequestedRegion(l_L2inWASRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of L2inWAS is (at least partially) outside the largest possible region.");
            }

            // IPCLDShadVarIn Region
            l_IPCLDShadVarInRequestedRegion = l_IPCLDShadVarInPtr->GetRequestedRegion();

            l_IPCLDShadVarInRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPCLDShadVarInRequestedRegion.Crop(l_IPCLDShadVarInPtr->GetLargestPossibleRegion()))
            {
                l_IPCLDShadVarInPtr->SetRequestedRegion(l_IPCLDShadVarInRequestedRegion);

            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_IPCLDShadVarInPtr->SetRequestedRegion(l_IPCLDShadVarInRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region of IPCLDShadVarIn is (at least partially) outside the largest possible region.");
            }
        }

    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ShadowVariationCorrelationImageFilter<TInputImage, TInputMask, TOutputImage>::BeforeThreadedGenerateData()
        {

            // Conversion of the correlation threshold parameter that is in percent
            // MACCS 5.0 : Already /100. m_CorrelThreshold = m_CorrelThreshold / 100.; // Initial parameter in percent

            // First, clear the functor list
            m_FunctorVector.clear();

            const unsigned int l_NumberOfThreads(this->GetNumberOfThreads());

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

    //****************************** ThreadedGenerateData ********************************/
    /** ThreadedGenerateData method */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ShadowVariationCorrelationImageFilter<TInputImage, TInputMask, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {
            // TOCR/L2inRCR
            itk::ZeroFluxNeumannBoundaryCondition<InputImageType> l_IPTOCRnbC;
            itk::ZeroFluxNeumannBoundaryCondition<InputImageType> l_L2inRCRnbC;
            itk::ZeroFluxNeumannBoundaryCondition<InputImageType> l_L2inSTOnbC;
            itk::ZeroFluxNeumannBoundaryCondition<InputMaskType> l_IPEDGnbC;
            itk::ZeroFluxNeumannBoundaryCondition<InputMaskType> l_L2inNDTnbC;


            // Get pointers to input images
            InputImageConstPointer l_IPTOCRPtr = this->GetIPTOCRInput();
            InputImageConstPointer l_L2inRCRPtr = this->GetL2inRCRInput();
            InputImageConstPointer l_L2inSTOPtr = this->GetL2inSTOInput();
            //DateImageConstPointer l_L2inPXDPtr = this->GetL2inPXDInput();
            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
            InputMaskConstPointer l_L2inNDTPtr = this->GetL2inNDTInput();
            InputMaskConstPointer l_L2inWASPtr = this->GetL2inWASInput();
            InputMaskPointer l_IPCLDShadVarInPtr = this->GetIPCLDShadVarInput();

            // Get the output mask
            OutputImagePointer l_IPCLDShadVarPtr = this->GetIPCLDShadVarOutput();

            // Get the coefficient of correlation
            CorrelationPixelType l_CorrelationCoef;
            CorrelationPixelType l_StackCorrelationCoef;

            RadiusType l_Radius;
            l_Radius[0] = m_NCorrel[0];
            l_Radius[1] = m_NCorrel[1];

            // Define the input neighborhood iterators
            NeighborhoodIteratorType l_IPTOCRIt(l_Radius, l_IPTOCRPtr, outputRegionForThread);
            NeighborhoodIteratorType l_L2inRCRIt(l_Radius, l_L2inRCRPtr, outputRegionForThread);
            NeighborhoodIteratorType l_L2inSTOIt(l_Radius, l_L2inSTOPtr, outputRegionForThread);
            MaskNeighborhoodIteratorType l_IPEDGIt(l_Radius, l_IPEDGPtr, outputRegionForThread);
            MaskNeighborhoodIteratorType l_L2inNDTIt(l_Radius, l_L2inNDTPtr, outputRegionForThread);

            // Define the input image region iterators
            // Those iterators are used to perform the correlation between the
            // surface reflectance images of D and D-1
            itk::ImageRegionConstIterator<InputMaskType> l_L2inWASIt(l_L2inWASPtr, outputRegionForThread);
            itk::ImageRegionIterator<InputMaskType> l_IPCLDShadVarInIt(l_IPCLDShadVarInPtr, outputRegionForThread);

            // Define the output image iterators
            // (Images not used for the correlation)
            itk::ImageRegionIterator<OutputImageType> l_IPCLDShadVarIt(l_IPCLDShadVarPtr, outputRegionForThread);

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
            l_L2inWASIt.GoToBegin();
            l_IPCLDShadVarInIt.GoToBegin();
            l_IPCLDShadVarIt.GoToBegin();

            // Number of bands
            const unsigned int l_BandNumber(l_L2inSTOPtr->GetNumberOfComponentsPerPixel());

            // Pixel loop
            while (l_IPCLDShadVarIt.IsAtEnd() == false)
            {

                const InputMaskPixelType l_L2inWASValue = l_L2inWASIt.Get();
                const InputMaskPixelType l_IPCLDShadVarInValue = l_IPCLDShadVarInIt.Get();

                // Get the no_data mask values
                const InputMaskPixelType l_IPEDGValue = l_IPEDGIt.GetCenterPixel();
                const InputMaskPixelType l_L2inNDTValue = l_L2inNDTIt.GetCenterPixel();

                // Output mask initialization
                l_IPCLDShadVarIt.Set(itk::NumericTraits<OutputImagePixelType>::Zero);

                FunctorType & l_ThreadFunc = m_FunctorVector[threadId];

                // Shadows are only detected in the outside clouds shadow zone
                // If the pixel is not an edge pixel or equal to No_Data for dates D and D-1

                // Detection not activated for water surfaces

                if ((l_IPCLDShadVarInValue == static_cast<InputMaskPixelType>(1)) && (l_L2inWASValue == static_cast<InputMaskPixelType>(0))
                        && (l_IPEDGValue == static_cast<InputMaskPixelType>(0)) && (l_L2inNDTValue == static_cast<InputMaskPixelType>(0)))
                {

                    // If correlation test could not be done,
                    // relative variation test is trusted
                    l_IPCLDShadVarIt.Set(static_cast<OutputImagePixelType>(1));

                    // Correlation

                    // Correlation between the input surface reflectance image and the composite product
                    l_ThreadFunc.SetCorrelBandB(m_CorrelBandRCR);
                    l_CorrelationCoef = l_ThreadFunc(l_IPTOCRIt, l_L2inRCRIt, l_IPEDGIt);

                    // Previous images loop
                    unsigned int l_band(0);

                    // The correlation is stopped as soon as a good correlation point is obtained
                    while ((l_CorrelationCoef <= m_CorrelThreshold) && (l_band < l_BandNumber))
                    {
                        if (vnsDifferentDoubleMacro(m_STOListOfDates[l_band],m_JDay) == true)
                        {
                            // Set the correlation band of the stack of images
                            l_ThreadFunc.SetCorrelBandB(l_band);
                            // Correlation between the input surface reflectance image and the previous image of date D-1-bd
                            l_StackCorrelationCoef = l_ThreadFunc(l_IPTOCRIt, l_L2inSTOIt, l_IPEDGIt);

                            // If the correlation was successful
                            if ( vnsDifferentDoubleMacro(l_StackCorrelationCoef,m_NoData) == true)
                            {
                                l_CorrelationCoef = l_StackCorrelationCoef;
                            }
                        }
                        l_band++;

                    }

                    // Final decision to set the pixel shadow or not
                    if (l_CorrelationCoef > m_CorrelThreshold)
                    {
                        // The pixel is shadow
                        l_IPCLDShadVarIt.Set(static_cast<OutputImagePixelType>(0));
                    }

                } // end if no water

                ++l_IPTOCRIt;
                ++l_L2inRCRIt;
                ++l_L2inSTOIt;
                ++l_IPEDGIt;
                ++l_L2inNDTIt;
                ++l_L2inWASIt;
                ++l_IPCLDShadVarInIt;
                ++l_IPCLDShadVarIt;

            } // end pixel loop

        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ShadowVariationCorrelationImageFilter<TInputImage, TInputMask, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_NoData:    " << m_NoData << std::endl;
            os << "m_RedBandTOCR:    " << m_RedBandTOCR << std::endl;
            os << "m_RedBandRCR:    " << m_RedBandRCR << std::endl;
            os << "m_CorrelBandTOCR:    " << m_CorrelBandTOCR << std::endl;
            os << "m_CorrelBandRCR:    " << m_CorrelBandRCR << std::endl;

            os << "m_JDay:    " << m_JDay << std::endl;
            os << "m_ForgettingDuration:    " << m_ForgettingDuration << std::endl;
            os << "m_MinPixelCorrelation:    " << m_MinPixelCorrelation << std::endl;
            os << "m_CorrelThreshold:    " << m_CorrelThreshold << std::endl;
            os << "m_NCorrel:   " << m_NCorrel << std::endl;
        }

} // End namespace vns

#endif
