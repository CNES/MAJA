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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 18 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 21 novembre 2013 : Correction de l'information géographique*
 * 									dans les produits. Correction du spacing dans l'image de sortie         *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsLinearSubSampleVectorImageFilter_txx
#define __vnsLinearSubSampleVectorImageFilter_txx

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#include "vnsLinearSubSampleVectorImageFilter.h"

#include "vnsMacro.h"
#include "itkObjectFactory.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkSpecialCoordinatesImage.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"

namespace vns
{

    /**
     * Initialize new instance
     */
    template<class TInputImage, class TOutputImage>
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::LinearSubSampleVectorImageFilter() :
            m_NumberOfBands(1), m_LinearCoef(), m_SubsampleFactor(1)
        {
            m_OutputSpacing.Fill(1.0);
            m_OutputOrigin.Fill(0.0);
            m_OutputDirection.SetIdentity();

            m_Size.Fill(0);
            m_OutputStartIndex.Fill(0);
        }

    /****************************** CallCopyInputRegionToOutputRegion ********************************/
    template<class TInputImage, class TOutputImage>
        void
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::CallCopyInputRegionToOutputRegion(OutputImageRegionType & destRegion,
                const InputImageRegionType & srcRegion)
        {
            Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

            typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
            typename InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

            typename OutputImageRegionType::IndexType destIndex;
            typename OutputImageRegionType::SizeType destSize;

            for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
            {
                destIndex[i]
                        = static_cast<long int> (vcl_ceil(static_cast<double> (srcIndex[i]) / static_cast<double> (m_SubsampleFactor)));
                destSize[i] = static_cast<long int> (vcl_ceil(static_cast<double> (srcSize[i]) / static_cast<double> (m_SubsampleFactor)));
            }

            destRegion.SetIndex(destIndex);
            destRegion.SetSize(destSize);
        }

    /****************************** CallCopyOutputRegionToInputRegion ********************************/
    template<class TInputImage, class TOutputImage>
        void
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::CallCopyOutputRegionToInputRegion(InputImageRegionType & destRegion,
                const OutputImageRegionType & srcRegion)
        {
            Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

            typename OutputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
            typename OutputImageRegionType::SizeType srcSize = srcRegion.GetSize();

            typename InputImageRegionType::IndexType destIndex;
            typename InputImageRegionType::SizeType destSize;

            for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
            {
                destIndex[i] = srcIndex[i] * m_SubsampleFactor;
                destSize[i] = srcSize[i] * m_SubsampleFactor;
            }

            destRegion.SetIndex(destIndex);
            destRegion.SetSize(destSize);

        }

    template<class TInputImage, class TOutputImage>
        void
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();

            // Get the output pointers
            OutputImagePointer outputPtr = this->GetOutput();
            // Get ths input pointers
            InputImageConstPointer inputPtr = this->GetInput();

            if ((inputPtr.IsNotNull() == true) && (outputPtr.IsNotNull() == true))
            {
                m_NumberOfBands = inputPtr->GetNumberOfComponentsPerPixel();
                outputPtr->SetNumberOfComponentsPerPixel(m_NumberOfBands);

                if (m_SubsampleFactor != 1)
                {
                    OutputImageRegionType newRegion;
                    this->CallCopyInputRegionToOutputRegion(newRegion, inputPtr->GetLargestPossibleRegion());
                    this->GetOutput()->SetLargestPossibleRegion(newRegion);
                    // LAIG-FA-MAC-701-CNES : Spacing = (input spacing * (and not /) m_SubsampleFactor where
                    // m_SubsampleFactor = input resolution / output resolution (int(input resolution) < int(output resolution))
                    outputPtr->SetSpacing(inputPtr->GetSpacing() * static_cast<double> (m_SubsampleFactor));
                }
            }
        }

    /**
     * Print out a description of self
     */
    template<class TInputImage, class TOutputImage>
        void
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << indent << "Sample Factor: " << m_SubsampleFactor << std::endl;
            os << indent << "Size: " << m_Size << std::endl;
            os << indent << "OutputStartIndex: " << m_OutputStartIndex << std::endl;
            os << indent << "OutputOrigin: " << m_OutputOrigin << std::endl;
            os << indent << "OutputSpacing: " << m_OutputSpacing << std::endl;
            os << indent << "OutputDirection: " << m_OutputDirection << std::endl;
            os << indent << "Linear coefficeints: " << std::endl;
            unsigned int size = m_LinearCoef.size();
            for (unsigned int i = 0; i < size; i++)
            {
                os << indent << m_LinearCoef(i) << std::endl;
            }
            return;
        }

    /**
     * BeforeThreadedGenerateData
     */
    template<class TInputImage, class TOutputImage>
        void
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
        {
            // Class superclass method
            Superclass::BeforeThreadedGenerateData();

            // Check teh subsample ratio
            if (m_SubsampleFactor == 0)
            {
                vnsExceptionBusinessMacro("Invalid subsample factor: "<<m_SubsampleFactor<<". It has to be greater than 0.")
            }

            this->EvaluateCoef();
        }

    /**
     * ThreadedGenerateData
     */
    template<class TInputImage, class TOutputImage>
        void
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Get the output pointers
            OutputImagePointer outputPtr = this->GetOutput();
            // Copy the threaded output region in the input region
            InputImageRegionType inputRegionForThread;
            this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

            // If m_SubsampleFactor == 0, an exception is thrown in the BefeoretThreadedGenerateData

            // If m_SubsampleFactor == 1, the output copy the input.
            if (m_SubsampleFactor == 1)
            {
                // Get ths input pointers
                InputImageConstPointer inputPtr = this->GetInput();

                // Declaration of input image iterator
                itk::ImageRegionConstIterator<InputImageType> inIt(inputPtr, inputRegionForThread);
                inIt.GoToBegin();
                // Declaration of output image iterator
                itk::ImageRegionIterator<OutputImageType> outIt(outputPtr, outputRegionForThread);
                outIt.GoToBegin();

                // Loop and just copy input pixel into the output one
                while ((inIt.IsAtEnd() == false) && (outIt.IsAtEnd() == false))
                {
                    outIt.Set(inIt.Get());

                    ++inIt;
                    ++outIt;

                } // end loop over input iterator

            } // end if m_SubsampleFactor == 0
            // If m_SubsampleFactor > 1 process to the sub sampling
            else
            {
                // Get ths input pointers
                InputImagePointer inputPtr = const_cast<InputImageType*> (this->GetInput());

                // Declaration of input image neighborhood iterator
                itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;
                SizeType rad;
                rad.Fill(m_SubsampleFactor);

                InputImageRegionType inputRequestedRegion = inputPtr->GetBufferedRegion();
                NeighborhoodTypeIterator neighInputIt = NeighborhoodTypeIterator(rad, inputPtr, inputRequestedRegion);
                neighInputIt.OverrideBoundaryCondition(&nbc);
                neighInputIt.GoToBegin();

                // Output iterator over the outputRegionForThread
                itk::ImageRegionIteratorWithIndex<OutputImageType> outIt = itk::ImageRegionIteratorWithIndex<OutputImageType>(outputPtr,
                        outputRegionForThread);
                outIt.GoToBegin();

                // support progress methods/callbacks
                int radi = static_cast<int> (m_SubsampleFactor - 1);
                const unsigned int l_NumberOfBands(m_NumberOfBands);
                typename OutputImageType::PixelType outPix;
                outPix.SetSize(l_NumberOfBands);
                const unsigned int nblineRes(2 * radi + 1);
                //*****************************************************************************************************
                //*****************************************************************************************************
                //*****************************************************************************************************
                CoefContainerType BandlineRes(l_NumberOfBands * nblineRes, 0.);
                while (outIt.IsAtEnd() == false)
                {

                    // Store the current output pixel index
                    // Deduct the corresponding input pixel index
                    IndexType inputId = outIt.GetIndex();
                    for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
                    {
                        inputId[i] = inputId[i] * m_SubsampleFactor;
                    }

                    outPix.Fill(0);

                    if (inputRequestedRegion.IsInside(inputId))
                    {
                        neighInputIt.SetLocation(inputId);

                        // The first and the last BCO coef is nul, we don't use them -> m_BCORadius-1
                        //                            CoefContainerType lineRes(l_NbRes, 0.);
                        //                            vnl_vector<CoefContainerType> BandlineRes(l_NumberOfBands, lineRes);
                        BandlineRes.fill(0.);
                        unsigned int p = 0;
                        // Proceed column
                        OffsetType off;
                        for (int i = -radi; i <= radi; i++)
                        {
                            off[0] = i;
                            // The first and the last linear coef is nul, we don't use them -> coefId = 1
                            int coefId = 1;
                            for (int j = -radi; j <= radi; j++)
                            {
                                off[1] = j;
                                const InputPixelType & inputPixel = neighInputIt.GetPixel(off);
                                const double m_LinearCoefId(m_LinearCoef(coefId));
                                for (unsigned int bandId = 0; bandId < l_NumberOfBands; bandId++)
                                {
                                    BandlineRes(bandId * nblineRes + p) = BandlineRes(bandId * nblineRes + p)
                                            + static_cast<double> (inputPixel[bandId]) * m_LinearCoefId;
                                }
                                coefId++;
                            }

                            p++;
                        }

                        // Proceed column
                        for (unsigned int bandId = 0; bandId < l_NumberOfBands; bandId++)
                        {
                            double res = 0.;
                            for (unsigned int l = 0; l < nblineRes; l++)
                            {
                                // The first and the last linear coef is nul, we don't use them -> l+1
                                res = res + BandlineRes(bandId * nblineRes + l) * m_LinearCoef(l + 1);
                            }

                            outPix[bandId] = static_cast<InternalPixelType> (res);
                        }
                    }

                    outIt.Set(outPix);
                    ++outIt;

                } // while

            } // else test crop

            return;
        }

    template<class TInputImage, class TOutputImage>
        void
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::EvaluateCoef()
        {
            // Init Linear coefficient container
            unsigned int size = 0;
            size = 2 * m_SubsampleFactor + 1;
            m_LinearCoef = CoefContainerType(size, 0.);
            //double val = 0.;
            //double step = 1./static_cast<double>(m_SubsampleFactor);

            // Compute Linear coefficients
            for (unsigned int i = 0; i <= m_SubsampleFactor; i++)
            {
                const double l_coef = -1 / vcl_pow(static_cast<double> (m_SubsampleFactor), 2) * vcl_abs(static_cast<double> (i)) + 1
                        / static_cast<double> (m_SubsampleFactor);

                m_LinearCoef[m_SubsampleFactor - i] = l_coef;
                m_LinearCoef[m_SubsampleFactor + i] = l_coef;

            }
        }

    /**
     * Inform pipeline of necessary input image region
     *
     * Determining the actual input region is non-trivial, especially
     * when we cannot assume anything about the transform being used.
     * So we do the easy thing and request the entire input image.
     */
    template<class TInputImage, class TOutputImage>
        void
        LinearSubSampleVectorImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion()
        {
            // call the superclass's implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // get pointers to the input and output images
            InputImagePointer inputPtr = const_cast<InputImageType *> (this->GetInput(0));
            OutputImagePointer outputPtr = this->GetOutput();

            if ((inputPtr.IsNull() == true) || (outputPtr.IsNull() == true))
            {
                return;
            }
            // get a copy of the input requested region (should equal the output
            // requested region)
            InputImageRegionType inputRequestedRegion = inputPtr->GetRequestedRegion();

            // pad the input requested region by the operator radius
            inputRequestedRegion.PadByRadius(m_SubsampleFactor);

            // crop the input requested region at the input's largest possible region
            if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
            {
                inputPtr->SetRequestedRegion(inputRequestedRegion);
                return;
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                inputPtr->SetRequestedRegion(inputRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region is (at least partially) outside the largest possible region.");
            }

            return;
        }

} // end namespace vns

#endif

