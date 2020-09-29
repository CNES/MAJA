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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsRayleighCorrectionImageFilter.h"

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "vnsMacro.h"

namespace vns
{

    // Constructor
    RayleighCorrectionImageFilter::RayleighCorrectionImageFilter() :
            m_DefaultAOT(0.), m_NoData(0)
    {
        // Input/Output initialization
        this->SetNumberOfIndexedInputs(3);
        this->SetNumberOfRequiredInputs(3);
        this->SetNumberOfIndexedOutputs(1);

        this->SetNthOutput(0, OutputImageType::New());
    }

    // Destructor
    RayleighCorrectionImageFilter::~RayleighCorrectionImageFilter()
    {
    }

    //===============  Generate output information method ===================/
    void
    RayleighCorrectionImageFilter::GenerateOutputInformation()
    {
        // Call superclass implementation
        Superclass::GenerateOutputInformation();

        // Set the number of output channels to the number of input one.
        // Check TOAC image availability
        if (!this->GetIPTOACInput())
        {
            vnsExceptionBusinessMacro("Waiting for a TOAC image, but no one specified. Please set one.");
        }
        // Check edges mask availability
        if (!this->GetIPEDGInput())
        {
            vnsExceptionBusinessMacro("Waiting for a edges mask, but no one specified. Please set one.");
        }
        this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetIPTOACInput()->GetNumberOfComponentsPerPixel());

    }

    //=============== BeforeThreaded generate data method ================/
    void
    RayleighCorrectionImageFilter::BeforeThreadedGenerateData()
    {
        // Call superclass implementation
        Superclass::BeforeThreadedGenerateData();

        // Check LUT availability
        if (m_LUT.IsNull() == true)
        {
            vnsExceptionBusinessMacro("Waiting for a LUT, but no one specified. Please set one.");
        }

        // Check TOAC image availability
        if (!this->GetIPTOACInput())
        {
            vnsExceptionBusinessMacro("Waiting for a TOAC image, but no one specified. Please set one.");
        }
        // Check edges mask availability
        if (!this->GetIPEDGInput())
        {
            vnsExceptionBusinessMacro("Waiting for a edges mask, but no one specified. Please set one.");
        }

        // Check DTM image availability
        if (!this->GetDTMInput())
        {
            vnsExceptionBusinessMacro("Waiting for an Altitude image, but no one specified. Please set one.");
        }

        // Check the input images size
        vnsChecking3ImagesSizeMacro(this->GetIPTOACInput(), this->GetIPEDGInput(), this->GetDTMInput())

    }

    //====================== Main computation method ======================/
    void
    RayleighCorrectionImageFilter::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
    {
        // Load inputs
        // Input availability are checked in the BeforeThreadedGenerateData.
        InputImageConstPointer l_IPTOACPtr = this->GetIPTOACInput();
        InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
        DTMImageConstPointer l_DTMPtr = this->GetDTMInput();

        // Load outputs
        OutputImagePointer l_IPTOCRPtr = this->GetOutput();

        // Prepare the threaded region input local iterators
        itk::ImageRegionConstIterator<InputImageType> l_IPTOACIt(l_IPTOACPtr, outputRegionForThread);
        itk::ImageRegionConstIterator<InputMaskType> l_IPEDGIt(l_IPEDGPtr, outputRegionForThread);
        itk::ImageRegionConstIterator<DTMImageType> l_DTMIt(l_DTMPtr, outputRegionForThread);

        // Init to the beginning
        l_IPTOACIt.GoToBegin();
        l_IPEDGIt.GoToBegin();
        l_DTMIt.GoToBegin();

        // Prepare the threaded region output local iterator
        itk::ImageRegionIterator<OutputImageType> l_IPTOCRIt(l_IPTOCRPtr, outputRegionForThread);

        // Init to the beginning
        l_IPTOCRIt.GoToBegin();

        // Progress reporter

        LUTPointType point;

        // Get the number of band of the input image
        const unsigned int nbOfChannels(l_IPTOACPtr->GetNumberOfComponentsPerPixel());

        // Set the output default pixel value to NoData.
        OutputImagePixelType l_IPTOCRPix;
        l_IPTOCRPix.SetSize(nbOfChannels);

        // Set the point[1] value
        point[1] = static_cast<LUTPointValueType>(m_DefaultAOT);
        // Loop over input and output images
        while ((l_IPTOCRIt.IsAtEnd() == false) && (l_IPTOACIt.IsAtEnd() == false))
        {

            l_IPTOCRPix.Fill(static_cast<OutputImageInternalPixelType>(m_NoData));

            // Loop over the channels
            InputImagePixelType l_IPTOACPix = l_IPTOACIt.Get();

            // If the pixel is not a No_data pixel
            if (l_IPEDGIt.Get() == 0)
            {
                point[0] = static_cast<LUTPointValueType>(l_DTMIt.Get());

                // Band loop
                for (unsigned int i = 0; i < nbOfChannels; i++)
                {
                    point[2] = static_cast<LUTPointValueType>(l_IPTOACPix[i]);
                    LUTPixelType interpValue = m_LUT->InterpolateValue(point);

                    l_IPTOCRPix[i] = static_cast<OutputImageInternalPixelType>(interpValue[i]);
                }
            }
            // Fill the output pixel iterator
            l_IPTOCRIt.Set(l_IPTOCRPix);

            ++l_IPTOACIt;
            ++l_IPEDGIt;
            ++l_DTMIt;
            ++l_IPTOCRIt;

        } // end pixel loop
    }

    // PrintSelf method
    void
    RayleighCorrectionImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);

        os << "m_DefaultAOT:    " << m_DefaultAOT << std::endl;
    }

} // End namespace vns
