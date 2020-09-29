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
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-1754-CNES : 10 janvier 2017 : Water Vapor Post-Processing             *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 9 aout 2016 : Correction qualité                          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-7-3 : FA : LAIG-FA-MAC-141975-CS : 15 fevrier 2016 : Correction dans la boucle sur voisinage *
 *                                   lors du calcul de la somme des poids + jout controle division par zero *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsGapFillingImageFilter_txx
#define vnsGapFillingImageFilter_txx

#include "vnsGapFillingImageFilter.h"
#include "vnsMath.h"

namespace vns
{

//
//******************* Constructor  ****************
//
    template<class TInputImage, class TOutputImage, class TOutputMask>
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::GapFillingImageFilter() :
                m_NoData(static_cast<NoDataType>(0.)), m_DefaultValue(0.), m_MeanValue(0.), m_InitWindowRadius(1), m_MaxWindowRadius(1), m_HasValidPixel(
                        true)
        {
            this->SetNumberOfRequiredInputs(1);
            this->SetNumberOfIndexedInputs(1);
            this->SetNumberOfIndexedOutputs(2);

            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, OutputMaskType::New());
        }

//
//******************* ThreadedGenerateData function *******************
//
    template<class TInputImage, class TOutputImage, class TOutputMask>
        void
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Evaluate the progress

            // Get the input image pointer
            InputImageConstPointerType lInputImagePtr = this->GetInputImage();

            // Get the output filled image
            OutputImagePointerType lOutputFilledImagePtr = this->GetOutputFilledImage();

            // Get the output mask
            OutputMaskPointerType lOutputMaskPtr = this->GetOutputMask();

            // Define the boundary conditions (for input data)
            ConstantBCType lInputImageConstantBC;
            lInputImageConstantBC.SetConstant(m_NoData);

            RadiusType lMaxWindowRadius;
            lMaxWindowRadius.Fill(m_MaxWindowRadius);

            // Define output region Iterators
            OutputImageRegionIterator lOutputFilledImageIt = itk::ImageRegionIterator<OutputImageType>(lOutputFilledImagePtr, outputRegionForThread);
            lOutputFilledImageIt.GoToBegin();

            OutputMaskRegionIterator lOutputMaskIt = itk::ImageRegionIterator<OutputMaskType>(lOutputMaskPtr, outputRegionForThread);
            lOutputMaskIt.GoToBegin();

            // Define input image neighborhood iterator
            ImageNeighborhoodIteratorType lInputImageIt = ImageNeighborhoodIteratorType(lMaxWindowRadius, lInputImagePtr, outputRegionForThread);
            lInputImageIt.OverrideBoundaryCondition(&lInputImageConstantBC);
            lInputImageIt.GoToBegin();

            const int l_InitWindowRadius = static_cast<int>(m_InitWindowRadius);
            const int l_MaxWindowRadius = static_cast<int>(m_MaxWindowRadius);

            const unsigned int l_TwoInitWindowRadiuPlusOne = 2 * m_InitWindowRadius + 1;
            // TODO optimization: sortir le if  la boucle: faire deux boucle OU ne pas appeler GapFilling dans ce cas et générer images constantes
            while (!lInputImageIt.IsAtEnd())
            {
                if (m_HasValidPixel)
                {
                    // Copy the input value
                    lOutputFilledImageIt.Set(lInputImageIt.GetCenterPixel());
                    lOutputMaskIt.Set(0); // filled pixels
                    // Radius initialization
                    int lRadius = l_InitWindowRadius;
                    unsigned int lLoopId = 0;
                    OffsetType lOffset;

                    // Do the work
                    while (( vcl_abs(lOutputFilledImageIt.Get() - m_NoData) < CONST_EPSILON )
                    && ( lRadius <= l_MaxWindowRadius ) )
                    {
                        // Count the valid pixels
                        unsigned int lNbOfValidPixels = 0;
                        double lSumWeight = 0;
                        double lSumProd = 0;

                        for (int l = -lRadius; l<= lRadius; l++)
                        {
                            lOffset[0] = l;
                            for (int p = -lRadius; p<= lRadius; p++)
                            {
                                lOffset[1] = p;
                                InputImagePixelType lPix = lInputImageIt.GetPixel(lOffset);
                                if ( vcl_abs(static_cast<double>(lPix)- m_NoData) > CONST_EPSILON )
                                {
                                    lNbOfValidPixels++;
                                }
                            }
                        }

                        if ( lNbOfValidPixels > l_TwoInitWindowRadiuPlusOne )
                        {
                            lSumWeight = 0;
                            lSumProd = 0;
                            const MatrixType & lWeight = m_Weights[lLoopId];
                            OffsetType lOffset;
                            // Access on each pixel of the current Region
                            for(int l = -lRadius; l<=lRadius; l++ )
                            {
                                lOffset[1]=l;
                                for(int p = -lRadius; p<=lRadius; p++ )
                                {
                                    lOffset[0]=p;
                                    InputImagePixelType lPix = lInputImageIt.GetPixel(lOffset);

                                    if ( vcl_abs(static_cast<double>(lPix)- m_NoData) > CONST_EPSILON )
                                    {
                                        const double weight = lWeight[l+lRadius][p+lRadius];
                                        lSumWeight += weight;
                                        lSumProd += static_cast<double>(lPix)*weight;
                                    }
                                }
                            }
                        }

                        if( vcl_abs(lSumWeight) > 0 )
                        {
                            lOutputFilledImageIt.Set(static_cast<InputImagePixelType> ( lSumProd / lSumWeight ));
                            lOutputMaskIt.Set(1);
                        }
                        else
                        {
                            lRadius *= 2;
                        }
                        lLoopId++;

                    } // end of increase of the radius

                    if ( vcl_abs(static_cast<double>(lOutputFilledImageIt.Get())-m_NoData) < CONST_EPSILON )
                    {
                        lOutputFilledImageIt.Set(m_MeanValue);
                        lOutputMaskIt.Set(1);
                    }
                }  //end if meanValue != NoData
                else
                {
                    lOutputFilledImageIt.Set(m_DefaultValue);
                    lOutputMaskIt.Set(1);
                }

                ++lInputImageIt;
                ++lOutputFilledImageIt;
                ++lOutputMaskIt;

            } //end facelist iteration
        }

//
//****************************** PadByRadiusRequestedRegion ********************************/
//
    template<class TInputImage, class TOutputImage, class TOutputMask>
        void
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::PadByRadiusRequestedRegion(ImageBaseType * pImagePtr)
        {
            // Pad the input requested region by the operator radius
            SizeType lMaxRad;
            lMaxRad.Fill(m_MaxWindowRadius);

            // Get a copy of the input requested region (should equal the output
            // requested region)
            InputImageRegionType lTempRequestedRegion = pImagePtr->GetRequestedRegion();

            lTempRequestedRegion.PadByRadius(lMaxRad);

            // Crop the input requested region at the input's largest possible region
            if (lTempRequestedRegion.Crop(pImagePtr->GetLargestPossibleRegion()))
            {
                pImagePtr->SetRequestedRegion(lTempRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                pImagePtr->SetRequestedRegion(lTempRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region is (at least partially) outside the largest possible region.");
            }
        }

//
//****************************** GenerateInputRequestedRegion ********************************/
//
    template<class TInputImage, class TOutputImage, class TOutputMask>
        void
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::GenerateInputRequestedRegion()
        {

            // Call the superclass' implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // Get the input masks pointers
            ImageBaseType * lInputImage = dynamic_cast<ImageBaseType *>(itk::ProcessObject::GetInput(0));

            // Add margings to the requested region
            this->PadByRadiusRequestedRegion(lInputImage);

        }

//
//******************* BeforeThreadedGenerateData function. Validate inputs *******************
//
    template<class TInputImage, class TOutputImage, class TOutputMask>
        void
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::BeforeThreadedGenerateData()
        {
            // Get the input masks pointers
            InputImageConstPointerType lInputImage = this->GetInputImage();

            if (lInputImage.IsNull() == true)
            {
                itkExceptionMacro(<< "Input file is missing.");
            }

            this->InitWeights();
        }

//
//******************* Initialization of the vector Weights *******************
//
    template<class TInputImage, class TOutputImage, class TOutputMask>
        void
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::InitWeights()
        {
            m_Weights.clear();
            unsigned int lRadius = m_InitWindowRadius;
            while (lRadius <= m_MaxWindowRadius)
            {
                m_Weights.push_back(this->ComputeWeight(lRadius));
                lRadius *= 2;
            }

        }

//
//******************* Computation of a weigh matrix *******************
//
    template<class TInputImage, class TOutputImage, class TOutputMask>
        typename GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::MatrixType
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::ComputeWeight(unsigned int pRadius)
        {
            // Note : Width = 1 + 2*radius !!
            unsigned int lWidth = 1 + 2 * pRadius;
            vnl_matrix<double> lW(lWidth, 1, 0.); // rows, column, initValue

            for (unsigned int i = 0; i < pRadius; i++)
            {
                lW[i][0] = 1. - 2. * (pRadius - static_cast<double>(i)) / (2*pRadius);
            }

            for (unsigned int i = pRadius; i < lWidth; i++)
            {
                lW[i][0] = 1. - 2. * (static_cast<double>(i) - pRadius) / (2*pRadius);
            }

            const vnl_matrix<double> lWeight = lW * lW.transpose();

            return lWeight;
        }

//******************* OutputFilledImage image accessor  ****************
//
    /** Get the output image */
    template<class TInputImage, class TOutputImage, class TOutputMask>
        typename GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::OutputImageType *
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::GetOutputFilledImage(void)
        {
            return itkDynamicCastInDebugMode<OutputImageType *>(itk::ProcessObject::GetOutput(0));
        }

//
//******************* output mask accessor  ****************
//
    /** Get the output mask */
    template<class TInputImage, class TOutputImage, class TOutputMask>
        typename GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::OutputMaskType *
        GapFillingImageFilter<TInputImage, TOutputImage, TOutputMask>::GetOutputMask(void)
        {
            return itkDynamicCastInDebugMode<OutputMaskType *>(itk::ProcessObject::GetOutput(1));
        }

} // End namespace vns

#endif /* vnsGapFillingImageFilter_txx */
