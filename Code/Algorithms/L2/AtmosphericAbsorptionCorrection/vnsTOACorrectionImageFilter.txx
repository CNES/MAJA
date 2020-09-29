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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsTOACorrectionImageFilter_txx
#define __vnsTOACorrectionImageFilter_txx

#include "vnsTOACorrectionImageFilter.h"
#include "itkImageScanlineIterator.h"
#include "itkImageScanlineConstIterator.h"


namespace vns
{

    /**
     * Constructor
     */
    template<class TInputTOAImage, class TInputPressureImage, class TOutputTOASurfImage, class TInputWaterImage>
        TOACorrectionImageFilter<TInputTOAImage, TInputPressureImage, TOutputTOASurfImage, TInputWaterImage>::TOACorrectionImageFilter() :
                m_IsWaterImage(false), m_WaterAmountDefaultValue(3.)
        {
            this->InPlaceOff();
            this->SetNumberOfRequiredInputs(2);
            this->SetNumberOfIndexedInputs(3);
        }

    template<class TInputTOAImage, class TInputPressureImage, class TOutputTOASurfImage, class TInputWaterImage>
        void
        TOACorrectionImageFilter<TInputTOAImage, TInputPressureImage, TOutputTOASurfImage, TInputWaterImage>::AddSmacCoefContainer(
                const SmacCoefContainer & coef)
        {
            this->GetFunctor().AddSmacCoef(coef);
            this->Modified();
        }

    template<class TInputTOAImage, class TInputPressureImage, class TOutputTOASurfImage, class TInputWaterImage>
        void
        TOACorrectionImageFilter<TInputTOAImage, TInputPressureImage, TOutputTOASurfImage, TInputWaterImage>::AddSmacCoefContainer(
                const std::vector<double> & vectCoef)
        {
            SmacCoefContainer coef(vectCoef);
            this->AddSmacCoefContainer(coef);
        }

    template<class TInputTOAImage, class TInputPressureImage, class TOutputTOASurfImage, class TInputWaterImage>
        void
        TOACorrectionImageFilter<TInputTOAImage, TInputPressureImage, TOutputTOASurfImage, TInputWaterImage>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();

            // Check that SMAC coefficient are available
            if (this->GetSmacCoefContainerList().size() == 0)
            {
                vnsExceptionBusinessMacro("No SMAC coefficients set.");
            }
            // Check that we have 1 SMAC coefficient list per channel
            if (this->GetSmacCoefContainerList().size() != this->GetInput()->GetNumberOfComponentsPerPixel())
            {
                vnsExceptionBusinessMacro(
                        "Invalid SMAC coefficients. One per input image band is expected, "<<this->GetSmacCoefContainerList().size()<<" are given and there are "<<this->GetInput()->GetNumberOfComponentsPerPixel()<<" image bands.");
            }
        }

    /**
     * BeforeThreadedGenerateData function. Validate inputs
     */
    template<class TInputTOAImage, class TInputPressureImage, class TOutputTOASurfImage, class TInputWaterImage>
        void
        TOACorrectionImageFilter<TInputTOAImage, TInputPressureImage, TOutputTOASurfImage, TInputWaterImage>::BeforeThreadedGenerateData()
        {
            // Get the toa input pointer
            InputTOAImagePointer toaPtr = dynamic_cast<const InputTOAImageType*>((itk::ProcessObject::GetInput(0)));
            // Get the pressure input
            InputPressureImagePointer pressPtr = dynamic_cast<const InputPressureImageType*>((itk::ProcessObject::GetInput(1)));

            // Checks the validity input
            if ((toaPtr.IsNull() == true) || (pressPtr.IsNull() == true))
            {
                vnsExceptionBusinessMacro(
                        "At least one input is missing." << " TOA Input is " << toaPtr.GetPointer() << ", " << " Pressure Input is " << pressPtr.GetPointer());
            }

            // Get the water input pointer
            if (m_IsWaterImage == true)
            {
                InputWaterImagePointer waterPtr = dynamic_cast<const InputWaterImageType*>((itk::ProcessObject::GetInput(2)));
                // Checks the validity input
                if (waterPtr.IsNull() == true)
                {
                    vnsExceptionBusinessMacro("At least one input is missing." << " Water Input is " << waterPtr.GetPointer());
                }
            }

            // Checks the size of the ThetaV buffer and the toa number of components
            if (this->GetThetaV().size() != toaPtr->GetNumberOfComponentsPerPixel())
            {
                vnsExceptionBusinessMacro(
                        "Invalid thetaV argument, should have 1 value per bands (nb thetaV values: " << this->GetThetaV().size() << ", product number of components: " << toaPtr->GetNumberOfComponentsPerPixel() << ".");
            }
        }

    /**
     * ThreadedGenerateData function. Performs the pixel-wise addition
     */
    template<class TInputTOAImage, class TInputPressureImage, class TOutputTOASurfImage, class TInputWaterImage>
        void
        TOACorrectionImageFilter<TInputTOAImage, TInputPressureImage, TOutputTOASurfImage, TInputWaterImage>::ThreadedGenerateData(
                const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // We use dynamic_cast since inputs are stored as DataObjects.  The
            // ImageToImageFilter::GetInput(int) always returns a pointer to a
            // TInputImage1 so it cannot be used for the second or third input.
            InputTOAImagePointer toaPtr = dynamic_cast<const InputTOAImageType*>((itk::ProcessObject::GetInput(0)));
            InputPressureImagePointer pressPtr = dynamic_cast<const InputPressureImageType*>((itk::ProcessObject::GetInput(1)));
            OutputImagePointer outputPtr = this->GetOutput(0);

// V 4.4 - optimization
//            itk::ImageRegionConstIterator < InputTOAImageType > toaIt(toaPtr, outputRegionForThread);
//            itk::ImageRegionConstIterator < InputPressureImageType > pressIt(pressPtr, outputRegionForThread);
//            itk::ImageRegionIterator < OutputImageType > outputIt(outputPtr, outputRegionForThread);

            itk::ImageScanlineConstIterator<InputTOAImageType> toaIt(toaPtr, outputRegionForThread);
            itk::ImageScanlineConstIterator<InputPressureImageType> pressIt(pressPtr, outputRegionForThread);
            itk::ImageScanlineIterator<OutputImageType> outputIt(outputPtr, outputRegionForThread);

            // Initialize the iterators to the begin
            toaIt.GoToBegin();
            pressIt.GoToBegin();
            outputIt.GoToBegin();

            // V 4.4 Optimization - pass the Output pixel preinitialized.
            const unsigned int nbBands(toaIt.Get().GetSize());

            if( nbBands != m_Functor.GetSmacCoefsList().size())
            {
                vnsStaticExceptionBusinessMacro("TOACorrectionImageFunctor: Nb values for toaPix: '"<<nbBands<<"' and m_SmacCoefsList coef nb values : '"<<m_Functor.GetSmacCoefsList().size()  <<"'");
            }

            OutputImagePixelType outPix;
            outPix.SetSize(nbBands);
            outPix.Fill(static_cast<OutputInternalPixelType>(0.));


            // If Water is available
            if (m_IsWaterImage == true)
            {
                InputWaterImagePointer waterPtr = dynamic_cast<const InputWaterImageType*>((itk::ProcessObject::GetInput(2)));
                // Add the water amount image
//                itk::ImageRegionConstIterator<InputWaterImageType> waterIt(waterPtr, outputRegionForThread);
                itk::ImageScanlineConstIterator<InputWaterImageType> waterIt(waterPtr, outputRegionForThread);
                waterIt.GoToBegin();

                // Loops on the toa image
                while (toaIt.IsAtEnd() == false)
                {
                    while (!toaIt.IsAtEndOfLine())
                    {
//                        outputIt.Set(m_Functor(toaIt.Get(), waterIt.Get(), pressIt.Get()));
                        m_Functor(toaIt.Get(), waterIt.Get(), pressIt.Get(), nbBands, outPix);
                        outputIt.Set(outPix);
                        ++toaIt;
                        ++waterIt;
                        ++pressIt;
                        ++outputIt;
                    }
                    toaIt.NextLine();
                    waterIt.NextLine();
                    pressIt.NextLine();
                    outputIt.NextLine();
                }
            }
            // No water available
            else
            {
                const InputWaterImagePixelType l_WaterAmountDefaultValue = static_cast<InputWaterImagePixelType>(m_WaterAmountDefaultValue);
                // Loops on the toa image
                while (toaIt.IsAtEnd() == false)
                {
                    while (!toaIt.IsAtEndOfLine())
                    {
                        //outputIt.Set(m_Functor(toaIt.Get(), static_cast<InputWaterImagePixelType>(m_WaterAmountDefaultValue), pressIt.Get()));
                        m_Functor(toaIt.Get(), l_WaterAmountDefaultValue, pressIt.Get(), nbBands, outPix);
                        outputIt.Set(outPix);
                        ++toaIt;
                        ++pressIt;
                        ++outputIt;
                    }
                    toaIt.NextLine();
                    pressIt.NextLine();
                    outputIt.NextLine();
                }
            }

        }

    template<class TInputTOAImage, class TInputPressureImage, class TOutputTOASurfImage, class TInputWaterImage>
        void
        TOACorrectionImageFilter<TInputTOAImage, TInputPressureImage, TOutputTOASurfImage, TInputWaterImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "Filter parameters: " << std::endl;
            os << "Ozone amount         : " << this->GetOzoneAmount() << std::endl;
            os << "solar azimuthal angle: " << this->GetThetaS() << std::endl;
            os << "view azimuthal angle : " << std::endl;
            // Get the number of ThetaV values
            unsigned int size = this->GetThetaV().size();
            // Loops on the thetav and print the value
            for (unsigned int i = 0; i < size; i++)
            {
                os << this->GetThetaV().at(i) << std::endl;
            }
            // Get the number of smac coefficients values
            const SmacCoefContainerListType coefList = this->GetSmacCoefContainerList();
            size = coefList.size();
            os << "SMAC coefficients lists:" << std::endl;
            // Loops on the smac coef and print the value
            for (unsigned int i = 0; i < size; i++)
            {
                os << "-------------- Band " << i + 1 << ":" << std::endl;
                coefList[i].Print(os);
            }
        }

} // end namespace vns

#endif
