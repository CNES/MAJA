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
 *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2149-CNES : 24 fevrier 2017 : Correction Cirrus - création           *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCirrusMultiMeanStatisticsVectorImageFilter_txx
#define __vnsCirrusMultiMeanStatisticsVectorImageFilter_txx

#include <vnsCirrusMultiMeanStatisticsVectorImageFilter.h>
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsLoggers.h"
#include "vnsUtilities.h"

namespace vns
{

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::PersistentCirrusMultiMeanStatisticsVectorImageFilter() :
    m_CirrusForegroundValue(static_cast<MaskPixelType>(0)), m_ThinCirrusForegroundValue(static_cast<MaskPixelType>(0)),
    m_NonCirrusForegroundValue(static_cast<MaskPixelType>(0)), m_IsValid(false)
    {
        // first output is a copy of the image, DataObject created by
        // superclass

        // allocate the data objects for the outputs which are
        // just decorators around vector/matrix types
        for (unsigned int i = 1; i < 7; ++i)
        {
            this->itk::ProcessObject::SetNthOutput(i, this->MakeOutput(i).GetPointer());
        }
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    itk::DataObject::Pointer
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::MakeOutput(unsigned int output)
    {
        switch (output)
        {
        case 0:
            return static_cast<itk::DataObject*>(TInputImage1::New().GetPointer());
            // 1 and 2 outputs : pixel objects
        case 1:
            // L1 thin cirrus mean
            return static_cast<itk::DataObject*>(RealObjectType::New().GetPointer());
            // 2 and 3 outputs : Real pixel objects
        case 2:
        case 3:
            // L2 thin and non cirrus mean
            return static_cast<itk::DataObject*>(RealPixelObjectType::New().GetPointer());
            // 4,5 and 6 output : UI objects
        case 4:
        case 5:
        case 6:
            return static_cast<itk::DataObject*>(UIObjectType::New().GetPointer());
        default:
            // might as well make an image
            return static_cast<itk::DataObject*>(TInputImage1::New().GetPointer());
        }
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::RealObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetL1ThinCirrusMeanOutput()
    {
        return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(1));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    const typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::RealObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetL1ThinCirrusMeanOutput() const
    {
        return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(1));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::RealPixelObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetL2ThinCirrusMeanOutput()
    {
        return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    const typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::RealPixelObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetL2ThinCirrusMeanOutput() const
    {
        return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::RealPixelObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetL2NonCirrusMeanOutput()
    {
        return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    const typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::RealPixelObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetL2NonCirrusMeanOutput() const
    {
        return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
    }


    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::UIObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetNbOfValidCirrusValuesOutput()
    {
        return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(4));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    const typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::UIObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetNbOfValidCirrusValuesOutput() const
    {
        return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(4));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::UIObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetNbOfValidThinCirrusValuesOutput()
    {
        return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(5));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    const typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::UIObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetNbOfValidThinCirrusValuesOutput() const
    {
        return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(5));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::UIObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetNbOfValidNonCirrusValuesOutput()
    {
        return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(6));
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    const typename PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::UIObjectType*
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GetNbOfValidNonCirrusValuesOutput() const
    {
        return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(6));
    }



    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    void
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::GenerateOutputInformation()
    {
        Superclass::GenerateOutputInformation();
        if (this->GetInput())
        {
            this->GetOutput()->CopyInformation(this->GetInput());
            this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

            if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
            {
                this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
            }
        }
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    void
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::AllocateOutputs()
    {
        // This is commented to prevent the streaming of the whole image for the first stream strip
        // It shall not cause any problem because the output image of this filter is not intended to be used.
        //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
        //this->GraftOutput( image );
        // Nothing that needs to be allocated for the remaining outputs
    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    void
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::Reset()
    {
        TInputImage1 * inputPtr = const_cast<TInputImage1 *>(this->GetInput());
        inputPtr->UpdateOutputInformation();

        unsigned int numberOfThreads = this->GetNumberOfThreads();
        unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

        //Zero values
        RealPixelType zeroRealPixel;
        zeroRealPixel.SetSize(numberOfComponent);
        zeroRealPixel.Fill(itk::NumericTraits<PrecisionType>::ZeroValue());
        RealType zeroReal = itk::NumericTraits<RealType>::ZeroValue();

        // Class parameters
        m_NumberOfCirrusValidPixels.SetSize(numberOfThreads);
        m_NumberOfCirrusValidPixels.Fill(itk::NumericTraits<long>::Zero);
        m_NumberOfThinCirrusValidPixels.SetSize(numberOfThreads);
        m_NumberOfThinCirrusValidPixels.Fill(itk::NumericTraits<long>::Zero);
        m_NumberOfNonCirrusValidPixels.SetSize(numberOfThreads);
        m_NumberOfNonCirrusValidPixels.Fill(itk::NumericTraits<long>::Zero);

        // Initialize the output values
        this->GetNbOfValidCirrusValuesOutput()->Set(itk::NumericTraits<long>::Zero);
        this->GetNbOfValidThinCirrusValuesOutput()->Set(itk::NumericTraits<long>::Zero);
        this->GetNbOfValidNonCirrusValuesOutput()->Set(itk::NumericTraits<long>::Zero);

        // Set the mean and sum values to the output image
        this->GetL1ThinCirrusMeanOutput()->Set(zeroReal);
        this->GetL2NonCirrusMeanOutput()->Set(zeroRealPixel);
        this->GetL2ThinCirrusMeanOutput()->Set(zeroRealPixel);
        m_ThreadFirstOrderL1ThinCirrusAccumulators.resize(numberOfThreads);
        std::fill(m_ThreadFirstOrderL1ThinCirrusAccumulators.begin(), m_ThreadFirstOrderL1ThinCirrusAccumulators.end(), zeroReal);
        m_ThreadFirstOrderL2ThinCirrusAccumulators.resize(numberOfThreads);
        std::fill(m_ThreadFirstOrderL2ThinCirrusAccumulators.begin(), m_ThreadFirstOrderL2ThinCirrusAccumulators.end(), zeroRealPixel);
        m_ThreadFirstOrderL2NonCirrusAccumulators.resize(numberOfThreads);
        std::fill(m_ThreadFirstOrderL2NonCirrusAccumulators.begin(), m_ThreadFirstOrderL2NonCirrusAccumulators.end(), zeroRealPixel);

        // Boolean used to indicate if the number of pixels used to compute the statistics is not null
        m_IsValid = false;

    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    void
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::Synthetize()
    {
        TInputImage1 * inputPtr = const_cast<TInputImage1 *>(this->GetInput());
        const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();
        //Init
        RealPixelType streamFirstOrderL2ThinAccumulator(numberOfComponent);
        streamFirstOrderL2ThinAccumulator.Fill(itk::NumericTraits<PrecisionType>::Zero);
        RealPixelType streamFirstOrderL2NonAccumulator(numberOfComponent);
        streamFirstOrderL2NonAccumulator.Fill(itk::NumericTraits<PrecisionType>::Zero);
        RealType streamFirstOrderL1ThinAccumulator(itk::NumericTraits<PrecisionType>::Zero);

        long nbValidCirrusPixels(0);
        long nbValidThinCirrusPixels(0);
        long nbValidNonCirrusPixels(0);

        // Accumulate results from all threads
        const unsigned int numberOfThreads = this->GetNumberOfThreads();
        for (unsigned int threadId = 0; threadId < numberOfThreads; ++threadId)
        {
            //Valid pixel per band
            nbValidCirrusPixels += m_NumberOfCirrusValidPixels[threadId];
            nbValidThinCirrusPixels += m_NumberOfThinCirrusValidPixels[threadId];
            nbValidNonCirrusPixels += m_NumberOfNonCirrusValidPixels[threadId];
            streamFirstOrderL2ThinAccumulator += m_ThreadFirstOrderL2ThinCirrusAccumulators[threadId];
            streamFirstOrderL2NonAccumulator += m_ThreadFirstOrderL2NonCirrusAccumulators[threadId];
            streamFirstOrderL1ThinAccumulator += m_ThreadFirstOrderL1ThinCirrusAccumulators[threadId];
        }
        // Test if all the pixels are invalid => return null statistics
        if (nbValidThinCirrusPixels == 0)
        {
            m_IsValid = false;
            vnsLogWarningMacro(
                    "CirrusMultiMeanStatisticsVectorImageFilter::Synthetize." "No thin cirrus pixel is valid (Not masked nb valid pixels="<<nbValidCirrusPixels<<"). Return null statistics");
            this->GetL1ThinCirrusMeanOutput()->Set(static_cast<RealType>(streamFirstOrderL1ThinAccumulator));
            this->GetL2ThinCirrusMeanOutput()->Set(static_cast<RealPixelType>(streamFirstOrderL2ThinAccumulator));
        }
        // If some pixels are valid => Set the outputs
        else
        {
            //This flag is always true if a single pixel from a single band is correct
            m_IsValid = true;
            RealType l_L1Mean(itk::NumericTraits<PrecisionType>::Zero);
            l_L1Mean = streamFirstOrderL1ThinAccumulator / static_cast<RealType>(nbValidThinCirrusPixels);
            this->GetL1ThinCirrusMeanOutput()->Set(l_L1Mean); //nbPixels);
            RealPixelType l_L2ThinMean(numberOfComponent);
            l_L2ThinMean.Fill(itk::NumericTraits<PrecisionType>::Zero);
            l_L2ThinMean = streamFirstOrderL2ThinAccumulator / static_cast<RealType>(nbValidThinCirrusPixels);
            this->GetL2ThinCirrusMeanOutput()->Set(l_L2ThinMean); //nbPixels);

        } // end if nbValidThinPixels
        if (nbValidNonCirrusPixels == 0)
        {
            m_IsValid = false;
            vnsLogWarningMacro(
                    "CirrusMultiMeanStatisticsVectorImageFilter::Synthetize." "No non cirrus pixel is valid (Not masked nb valid pixels="<<nbValidCirrusPixels<<"). Return null statistics");
            this->GetL2NonCirrusMeanOutput()->Set(static_cast<RealPixelType>(streamFirstOrderL2NonAccumulator));
        }
        // If some pixels are valid => Set the outputs
        else
        {
            //This flag is always true if a single pixel from a single band is correct
            m_IsValid = m_IsValid && true;
            RealPixelType l_L2NonMean(numberOfComponent);
            l_L2NonMean.Fill(itk::NumericTraits<PrecisionType>::Zero);
            l_L2NonMean = streamFirstOrderL2NonAccumulator / static_cast<RealType>(nbValidNonCirrusPixels);
            this->GetL2NonCirrusMeanOutput()->Set(l_L2NonMean); //nbPixels);
        } // end if nbValidPixels


        // Set the number of used pixels to compute statistics and the number of excluded values
        this->GetNbOfValidCirrusValuesOutput()->Set(nbValidCirrusPixels);
        this->GetNbOfValidThinCirrusValuesOutput()->Set(nbValidThinCirrusPixels);
        this->GetNbOfValidNonCirrusValuesOutput()->Set(nbValidNonCirrusPixels);

    }

    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    void
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision>::ThreadedGenerateData(
            const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
            {

        // Support progress methods/callbacks

        // Grab the input image
        InputImagePointer inputPtr = const_cast<TInputImage1 *>(this->GetInput());
        L1InputImagePointer inputL1Ptr = const_cast<TInputImage2 *>(this->GetL1TOACirrusInput());
        // Get the input mask
        InputMaskPointer maskCirrusPtr = this->GetCirrusMaskInput();
        InputMaskPointer maskThinCirrusPtr = this->GetThinCirrusMaskInput();
        InputMaskPointer maskNonCirrusPtr = this->GetNonCirrusMaskInput();

        itk::ImageRegionConstIteratorWithIndex<TInputImage1> itL2(inputPtr, outputRegionForThread);
        itk::ImageRegionConstIteratorWithIndex<TInputImage2> itL1(inputL1Ptr, outputRegionForThread);
        itk::ImageRegionConstIteratorWithIndex<TInputMask> maskCirrusIt(maskCirrusPtr, outputRegionForThread);
        itk::ImageRegionConstIteratorWithIndex<TInputMask> maskThinCirrusIt(maskThinCirrusPtr, outputRegionForThread);
        itk::ImageRegionConstIteratorWithIndex<TInputMask> maskNonCirrusIt(maskNonCirrusPtr, outputRegionForThread);
        //Init iterators on masks
        maskCirrusIt.GoToBegin();
        maskThinCirrusIt.GoToBegin();
        maskNonCirrusIt.GoToBegin();
        //Masks foreground values
        const double l_DoubleForegroundCirrusValue = static_cast<double>(m_CirrusForegroundValue);
        const double l_DoubleForegroundThinCirrusValue = static_cast<double>(m_ThinCirrusForegroundValue);
        const double l_DoubleForegroundNonCirrusValue = static_cast<double>(m_NonCirrusForegroundValue);

        itL2.GoToBegin();
        itL1.GoToBegin();
        for (itL2.GoToBegin(); !itL2.IsAtEnd(); ++itL2) //MACCS 4.3 - disable progress : progress.CompletedPixel())
        {
            // Get the values of the mask pixels used for the selection of the pixels
            const MaskPixelType& maskCirrusValue = maskCirrusIt.Get();
            const MaskPixelType& maskThinCirrusValue = maskThinCirrusIt.Get();
            const MaskPixelType& maskNonCirrusValue = maskNonCirrusIt.Get();

            // Statistics computed if the pixel of the mask is equal to m_ForegroundValue
            //Thin stats
            if (vnsEqualsDoubleMacro(static_cast<double>(maskThinCirrusValue), l_DoubleForegroundThinCirrusValue))
            {
                //L2 Thin
                const PixelType& vectorL2Value = itL2.Get();
                const unsigned int l_NbValues = vectorL2Value.GetSize();
                //Count good values
                for (unsigned int j = 0; j < l_NbValues; ++j)
                {
                    RealPixelType& threadFirstOrder = m_ThreadFirstOrderL2ThinCirrusAccumulators[threadId];
                    threadFirstOrder[j] += vectorL2Value[j];
                }
                //L1 Thin
                const L1PixelType& vectorL1Value = itL1.Get();
                RealType& threadFirstL1Order = m_ThreadFirstOrderL1ThinCirrusAccumulators[threadId];
                threadFirstL1Order += vectorL1Value;

                // Count the number of valid pixels per thread
                m_NumberOfThinCirrusValidPixels[threadId]++;

            } // end if excluded value
            //Non stats
            if (vnsEqualsDoubleMacro(static_cast<double>(maskNonCirrusValue), l_DoubleForegroundNonCirrusValue))
            {
                //L2 Non
                const PixelType& vectorL2Value = itL2.Get();
                const unsigned int l_NbValues = vectorL2Value.GetSize();
                //Count good values
                for (unsigned int j = 0; j < l_NbValues; ++j)
                {
                    RealPixelType& threadFirstOrder = m_ThreadFirstOrderL2NonCirrusAccumulators[threadId];
                    threadFirstOrder[j] += vectorL2Value[j];
                }
                // Count the number of valid pixels per thread
                m_NumberOfNonCirrusValidPixels[threadId]++;

            } // end if excluded value

            //Cirrus stats, only count pixels
            if (vnsEqualsDoubleMacro(static_cast<double>(maskCirrusValue), l_DoubleForegroundCirrusValue))
            {
                // Count the number of valid pixels per thread
                m_NumberOfCirrusValidPixels[threadId]++;
            } // end if excluded value

            ++maskCirrusIt;
            ++maskThinCirrusIt;
            ++maskNonCirrusIt;
            ++itL1;
        }
            }

    template<class TImage1, class TImage2, class TInputMask, class TPrecision>
    void
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TImage1, TImage2, TInputMask, TPrecision>::PrintSelf(std::ostream& os,
            itk::Indent indent) const
            {
        Superclass::PrintSelf(os, indent);
            }

} // end namespace otb
#endif
