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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 01 janvier 2017 : Correction qualite (code mort)          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution              *
 * VERSION : 1-0-0-3 : FA : 294 : 14 septembre 2011 : Prise en compte remarque SGC CNES VE-FT-294.          *
 * VERSION : 1-0-0-1 : FA : 244 : 27 juin 2011 : Sous echantillonnage du plan d'altitude des nuages         *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudMaskUndersamplingImageFilterDeprecated.cxx 12047 2016-04-19 08:36:16Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsCloudMaskUndersamplingImageFilterDeprecated.h"
#include "vnsMath.h"

#include "otbSubsampledImageRegionConstIterator.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "vnsLoggers.h"

namespace vns
{

    /** Constructor */
    CloudMaskUndersamplingImageFilterDeprecated::CloudMaskUndersamplingImageFilterDeprecated()
    {

        // Input/Output initialization
        this->SetNumberOfIndexedInputs(1);
        this->SetNumberOfRequiredInputs(1);
        this->SetNumberOfIndexedOutputs(2);

        this->SetNthOutput(0, OutputImageType::New());
        this->SetNthOutput(1, OutputMaskType::New());

        // Parameters initialization
        m_Radius.Fill(1);
        m_L1NoData = static_cast<RealNoDataType>(0);

    }

    //****************************** CallCopyInputRegionToOutputRegion ********************************/
    void
    CloudMaskUndersamplingImageFilterDeprecated::CallCopyInputRegionToOutputRegion(OutputImageRegionType & destRegion,
            const InputImageRegionType & srcRegion)
    {
        Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

        InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
        InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

        OutputImageRegionType::IndexType destIndex;
        OutputImageRegionType::SizeType destSize;

        for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
        {
            const double l_SubsampleFactor = vcl_abs(static_cast<double>(m_AreaToL2CoarseResolution.Spacing[i]) / static_cast<double>(m_AreaToCLDResolution.Spacing[i]));
            destIndex[i] = srcIndex[i] / static_cast<unsigned short>(l_SubsampleFactor);
            destSize[i] = static_cast<unsigned long> (vcl_ceil(static_cast<double> (srcSize[i]) / static_cast<double> (l_SubsampleFactor)));
        }

        destRegion.SetIndex(destIndex);

        destRegion.SetSize(destSize);

    }

    //****************************** CallCopyOutputRegionToInputRegion ********************************/
    void
    CloudMaskUndersamplingImageFilterDeprecated::CallCopyOutputRegionToInputRegion(InputImageRegionType & destRegion,
            const OutputImageRegionType & srcRegion)
    {
        Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

        OutputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
        OutputImageRegionType::SizeType srcSize = srcRegion.GetSize();

        InputImageRegionType::IndexType destIndex;
        InputImageRegionType::SizeType destSize;

        for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
        {
            const double l_SubsampleFactor = vcl_abs(static_cast<double>(m_AreaToL2CoarseResolution.Spacing[i]) / static_cast<double>(m_AreaToCLDResolution.Spacing[i]));
            destIndex[i] = srcIndex[i] * static_cast<unsigned short>(l_SubsampleFactor);
            destSize[i] = static_cast<unsigned long> (static_cast<double> (srcSize[i]) * static_cast<double> (l_SubsampleFactor));
        }

        destRegion.SetIndex(destIndex);
        destRegion.SetSize(destSize);
    }

    //****************************** GenerateOutputInformation ********************************/
    void
    CloudMaskUndersamplingImageFilterDeprecated::GenerateOutputInformation()
    {
        Superclass::GenerateOutputInformation();

        const double l_SubsampleFactor = vcl_abs(static_cast<double>(m_AreaToL2CoarseResolution.Spacing[0]) / static_cast<double>(m_AreaToCLDResolution.Spacing[0]));

        if (vnsDifferentDoubleMacro(l_SubsampleFactor,1.) == true)
        {
            // Set the radius to the upper integer
            const double l_RealRadius = ((l_SubsampleFactor - 1.) / 2.) + 0.5;
            m_Radius.Fill(static_cast<unsigned long>(l_RealRadius));

            OutputImageRegionType newRegion;
            this->CallCopyInputRegionToOutputRegion(newRegion, this->GetInput()->GetLargestPossibleRegion());

            this->GetIPCLAOutput()->SetLargestPossibleRegion(newRegion);
            this->GetIPCLDAltOutput()->SetLargestPossibleRegion(newRegion);
        }
    }

    //****************************** GenerateInputRequestedRegion ********************************/
    /** GenerateInputRequestedRegion method */

    void
    CloudMaskUndersamplingImageFilterDeprecated::GenerateInputRequestedRegion()
    {
        // call the superclass' implementation of this method
        Superclass::GenerateInputRequestedRegion();

        // get pointers to the input and output images
        Superclass::InputImagePointer l_L1CLAPtr = this->GetL1CLAInput();

        OutputImagePointer l_IPCLAPtr = this->GetIPCLAOutput();

        OutputMaskPointer l_IPCLDAltPtr = this->GetIPCLDAltOutput();

        if ((l_L1CLAPtr.IsNull() == true) || (l_IPCLAPtr.IsNull() == true) || (l_IPCLDAltPtr.IsNull() == true))
        {
            vnsExceptionBusinessMacro("CloudMaskUndersamplingImageFilterDeprecated: Internal error: One (or more) Input pointer is NULL !")
        }
        // get a copy of the input requested region (should equal the output
        // requested region)
        InputImageRegionType inputRequestedRegion;

        inputRequestedRegion = l_L1CLAPtr->GetRequestedRegion();

        // pad the input requested region by the operator radius
        SizeType maxRad;
        maxRad[0] = m_Radius[0];
        maxRad[1] = m_Radius[1];

        inputRequestedRegion.PadByRadius(maxRad);

        // crop the input requested region at the input's largest possible region
        if (inputRequestedRegion.Crop(l_L1CLAPtr->GetLargestPossibleRegion()))
        {
            l_L1CLAPtr->SetRequestedRegion(inputRequestedRegion);
            return;
        }
        else
        {
            // Couldn't crop the region (requested region is outside the largest
            // possible region).  Throw an exception.
            // store what we tried to request (prior to trying to crop)
            l_L1CLAPtr->SetRequestedRegion(inputRequestedRegion);

            // build an exception
            itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
            std::ostringstream msg;
            msg << this->GetNameOfClass() << "::GenerateInputRequestedRegion(): L1CLA ";
            e.SetLocation(msg.str().c_str());
            e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
            e.SetDataObject(l_L1CLAPtr);
            throw e;

        }
    }

    //****************************** ThreadedGenerateData ********************************/
    /** ThreadedGenerateData method */

    void
    CloudMaskUndersamplingImageFilterDeprecated::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
            itk::ThreadIdType /*threadId*/)
    {

        // get pointers to input images
        Superclass::InputImagePointer l_L1CLAPtr = this->GetL1CLAInput();

        // get pointers to output images
        OutputImagePointer l_IPCLAPtr = this->GetIPCLAOutput();
        OutputMaskPointer l_IPCLDAltPtr = this->GetIPCLDAltOutput();

        // Copy the threaded output region in the input region
        InputImageRegionType inputRegionForThread;
        this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

        // Declaration of output image iterators
        itk::ImageRegionIterator < OutputImageType > l_IPCLAIt(l_IPCLAPtr, outputRegionForThread);

        itk::ImageRegionIterator < OutputMaskType > l_IPCLDAltIt(l_IPCLDAltPtr, outputRegionForThread);

        // If Cloud_mask_Resolution is equal to the L2_Coarse_Resolution
        // --------------------------------------------------------------
        const double l_SubsampleFactor = static_cast<double>(m_AreaToL2CoarseResolution.Spacing[0])
                / static_cast<double>(m_AreaToCLDResolution.Spacing[0]);
        if (vnsEqualsDoubleMacro(l_SubsampleFactor,1.) == true)
        {
            // Declaration of input image iterator
            itk::ImageRegionIterator < InputImageType > l_L1CLAIt(l_L1CLAPtr, inputRegionForThread);

            l_L1CLAIt.GoToBegin();
            l_IPCLAIt.GoToBegin();
            l_IPCLDAltIt.GoToBegin();

            while (l_L1CLAIt.IsAtEnd() == false)
            {

                l_IPCLAIt.Set(itk::NumericTraits < OutputImagePixelType > ::Zero);
                l_IPCLDAltIt.Set(itk::NumericTraits < OutputMaskPixelType > ::Zero);

                OutputImagePixelType l_L1CLAValue = l_L1CLAIt.Get();

                if ( vnsDifferentDoubleMacro(l_L1CLAValue,m_L1NoData) == true)
                {
                    l_IPCLAIt.Set(static_cast<OutputImagePixelType>(l_L1CLAValue));
                    l_IPCLDAltIt.Set(static_cast<OutputMaskPixelType>(1));

                }

                ++l_L1CLAIt;
                ++l_IPCLAIt;
                ++l_IPCLDAltIt;

            } // end loop over input iterator

        } // end if m_SubsampleFactor == 1

        // If Cloud_mask_Resolution  < L2_Coarse_Resolution
        // ------------------------------------------------
        else
        {
            IndexType l_SubIndex;

            InputImageRegionType inputRequestedRegion = l_L1CLAPtr->GetBufferedRegion();
            NeighborhoodIteratorType l_NeighInputIt(m_Radius, l_L1CLAPtr, inputRequestedRegion);
            l_NeighInputIt.GoToBegin();

            // Search the pixels at Cloud_Mask_Resolution
            // corresponding to the output pixel at L2_Coarse_Resolution
            while (l_IPCLAIt.IsAtEnd() == false)
            {
                l_IPCLAIt.Set(itk::NumericTraits < OutputImagePixelType > ::Zero);
                l_IPCLDAltIt.Set(itk::NumericTraits < OutputMaskPixelType > ::Zero);

                l_SubIndex = l_IPCLAIt.GetIndex();
                for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
                {
                    l_SubIndex[i] = static_cast<IndexType::IndexValueType>(l_SubIndex[i] * l_SubsampleFactor);
                }

                //if (inputRegionForThread.IsInside(l_SubIndex) == true)
                if (inputRequestedRegion.IsInside(l_SubIndex) == true)
                {

                    l_NeighInputIt.SetLocation(l_SubIndex);
                    NeighborhoodType l_Neighborhood = l_NeighInputIt.GetNeighborhood();
                    NeighborhoodType::ConstIterator l_NeighborhoodIt = l_Neighborhood.Begin();

                    double l_Sum(0.);
                    unsigned long l_Number(0);

                    // Loop on the region
                    while (l_NeighborhoodIt != l_Neighborhood.End())
                    {
                        // Get the current valid pixel.
                        double l_CLAValue = static_cast<double>(*l_NeighborhoodIt);

                        if (( vnsDifferentDoubleMacro(l_CLAValue,0.) == true) && ( vnsDifferentDoubleMacro(l_CLAValue,m_L1NoData) == true))
                        {
                            l_Sum = l_Sum + l_CLAValue;
                            l_Number++;
                        }

                        ++l_NeighborhoodIt;
                    } // end loop over region

                    // Compute the mean of the pixels of the region
                    // and set the output pixels
                    if (l_Number != 0)
                    {
                        l_IPCLAIt.Set(static_cast<OutputImagePixelType>(l_Sum / static_cast<double>(l_Number)));
                        l_IPCLDAltIt.Set(static_cast<OutputMaskPixelType>(1));
                    }

                } // end l_SubIndex is inside the region

                ++l_IPCLAIt;
                ++l_IPCLDAltIt;

            } // end loop over input iterator

        } // end m_SubsampleFactor != 1

    }

    /** PrintSelf method */

    void
    CloudMaskUndersamplingImageFilterDeprecated::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
        os << "m_Radius:                 " << m_Radius << std::endl;
        os << "m_L1NoData:               " << m_L1NoData << std::endl;
    }

} // End namespace vns
