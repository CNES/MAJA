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
 * VERSION : 4-6-3 : FA : LAIG-FA-MAC-1506-CNES : 23 novembre 2015 : Ajout message sur zone manquante       *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 07 novembre 2011 : Modification pour prise en compte       *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 avr. 2010 : Creation                                              *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputeScatteringCorrectionImageFilter_txx
#define __vnsComputeScatteringCorrectionImageFilter_txx

#include "vnsComputeScatteringCorrectionImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "vnsUtilities.h"

#include "vnsMacro.h"

namespace vns
{
    /** Constructor */
    template<class TTOACInputImage, class TAOTInputImage, class TDTMInputImage, class TInputMask, class TVectorMask, class TOutputImage,
            class TLUTInput>
        ComputeScatteringCorrectionImageFilter<TTOACInputImage, TAOTInputImage, TDTMInputImage, TInputMask, TVectorMask, TOutputImage,
                TLUTInput>::ComputeScatteringCorrectionImageFilter() :
                m_NoData(0)
        {
            this->SetNumberOfIndexedInputs(5);
            this->SetNumberOfRequiredInputs(4);

            m_LUTMap.clear();
            m_ListOfL2CoarseBandId.clear();

        }

    /** Destructor */
    template<class TTOACInputImage, class TAOTInputImage, class TDTMInputImage, class TInputMask, class TVectorMask, class TOutputImage,
            class TLUTInput>
        ComputeScatteringCorrectionImageFilter<TTOACInputImage, TAOTInputImage, TDTMInputImage, TInputMask, TVectorMask, TOutputImage,
                TLUTInput>::~ComputeScatteringCorrectionImageFilter()
        {
        }

    /** Generate output information method. */
    template<class TTOACInputImage, class TAOTInputImage, class TDTMInputImage, class TInputMask, class TVectorMask, class TOutputImage,
            class TLUTInput>
        void
        ComputeScatteringCorrectionImageFilter<TTOACInputImage, TAOTInputImage, TDTMInputImage, TInputMask, TVectorMask, TOutputImage,
                TLUTInput>::GenerateOutputInformation()
        {
            // Call superclass implementation
            Superclass::GenerateOutputInformation();

            // Set the number of output channels to the nulber of input one.
            // Check TOAC image availability
            if (!this->GetTOACInput())
            {
                vnsExceptionBusinessMacro("Waiting for a TOAC image, but no one specified. Please set one.");
            }
            this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetTOACInput()->GetNumberOfComponentsPerPixel());

        }

    /** BeforeThreaded generate data method. */
    template<class TTOACInputImage, class TAOTInputImage, class TDTMInputImage, class TInputMask, class TVectorMask, class TOutputImage,
            class TLUTInput>
        void
        ComputeScatteringCorrectionImageFilter<TTOACInputImage, TAOTInputImage, TDTMInputImage, TInputMask, TVectorMask, TOutputImage,
                TLUTInput>::BeforeThreadedGenerateData()
        {
            // Call superclass implementation
            Superclass::BeforeThreadedGenerateData();

            TOACImageConstPointer l_TOAC = this->GetTOACInput();
            AOTImageConstPointer l_AOT = this->GetAOTInput();
            InputMaskConstPointer l_EDG = this->GetEDGInput();
            DTMImageConstPointer l_DTM = this->GetDTMInput();

            // Check TOAC image availability
            if (l_TOAC.IsNull() == true)
            {
                vnsExceptionBusinessMacro("Waiting for a TOAC image, but no one specified. Please set one.");
            }
            // Check AOT image availability
            if (l_AOT.IsNull() == true)
            {
                vnsExceptionBusinessMacro("Waiting for an AOT image, but no one specified. Please set one.");
            }
            // Check EDG image availability
            if (l_EDG.IsNull() == true)
            {
                vnsExceptionBusinessMacro("Waiting for an EDG image, but no one specified. Please set one.");
            }
            // Check DTM image availability
            if (l_DTM.IsNull() == true)
            {
                vnsExceptionBusinessMacro("Waiting for an Altitude image, but no one specified. Please set one.");
            }

            // Check the input images size
            vnsChecking4ImagesSizeMacro(this->GetTOACInput(), this->GetAOTInput(), this->GetEDGInput(), this->GetDTMInput())

            // Check Angle zone mask availability
            VectorMaskConstPointer l_AngleZonePtr = this->GetAngleZoneMaskInput();
            if (l_AngleZonePtr.IsNull() == false)
            {
                vnsChecking2ImagesSizeMacro(this->GetTOACInput(), this->GetAngleZoneMaskInput())
            }

        }

    /** Main computation method. */
    template<class TTOACInputImage, class TAOTInputImage, class TDTMInputImage, class TInputMask, class TVectorMask, class TOutputImage,
            class TLUTInput>
        void
        ComputeScatteringCorrectionImageFilter<TTOACInputImage, TAOTInputImage, TDTMInputImage, TInputMask, TVectorMask, TOutputImage,
                TLUTInput>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Load inputs
            // Input availability are checked in the BeforeThreadedGenerateData.
            TOACImageConstPointer l_TOAC = this->GetTOACInput();
            AOTImageConstPointer l_AOT = this->GetAOTInput();
            InputMaskConstPointer l_EDG = this->GetEDGInput();
            DTMImageConstPointer l_DTM = this->GetDTMInput();

            // Optional input declaration
            VectorMaskConstPointer l_AngleZonePtr = this->GetAngleZoneMaskInput();

            // Load outputs
            OutputImagePointer l_out = this->GetOutput();

            // Prepare the threaded region input local iterators
            itk::ImageRegionConstIterator<TOACImageType> toaIt(l_TOAC, outputRegionForThread);
            itk::ImageRegionConstIterator<AOTImageType> aotIt(l_AOT, outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> edgIt(l_EDG, outputRegionForThread);
            itk::ImageRegionConstIterator<DTMImageType> dtmIt(l_DTM, outputRegionForThread);
            // Init to the beginning
            toaIt.GoToBegin();
            aotIt.GoToBegin();
            edgIt.GoToBegin();
            dtmIt.GoToBegin();

            // Prepare the threaded region output local iterator
            itk::ImageRegionIterator<OutputImageType> outIt(l_out, outputRegionForThread);
            // Init to the beginning
            outIt.GoToBegin();

            // Progress reporter

            LUTPointType point;

            const unsigned int nbOfChannels(l_TOAC->GetNumberOfComponentsPerPixel());
            OutputImagePixelType DefaultOutPix;
            DefaultOutPix.SetSize(nbOfChannels);
            DefaultOutPix.Fill(static_cast<OutputImageInternalPixelType>(m_NoData));

            // ============================================================================================
            // If the viewing angles depend on the detector (ie. the angle zone in the image)
            // ============================================================================================
            if (l_AngleZonePtr.IsNull() == false)
            {
                itk::ImageRegionConstIterator<VectorMaskType> angleZoneIt(l_AngleZonePtr, outputRegionForThread);
                angleZoneIt.GoToBegin();

                // Loop over the input and output images
                while ((outIt.IsAtEnd() == false) && (toaIt.IsAtEnd() == false) && (aotIt.IsAtEnd() == false) && (edgIt.IsAtEnd() == false)
                        && (dtmIt.IsAtEnd() == false))
                {
                    // Set the output default pix to NoData.
                    // If no edge
                    if ((edgIt.Get() == 0))
                    {
                        OutputImagePixelType outPix(DefaultOutPix);
                        LUTPointType point;
                        // Loop over the channels
                        const TOACImagePixelType & toaPix = toaIt.Get();
                        point[0] = static_cast<LUTPointValueType>(dtmIt.Get());
                        point[1] = static_cast<LUTPointValueType>(aotIt.Get());

                        // Band loop
                        for (unsigned int id = 0; id < nbOfChannels; id++)
                        {
                            // Get the L2 coarse band id associated to the current band id
                            const unsigned int l2CoarseBand = m_ListOfL2CoarseBandId[id];

                            // Get the angle zone (detector) of the pixel for wide field sensor (S2)
                            const unsigned int lzone = static_cast<unsigned int>(angleZoneIt.Get()[id]);

                            point[2] = static_cast<LUTPointValueType>(toaPix[id]);
                            // Check if a miniLUT exists for this zone (detector)

                            if (m_LUTMap.find(lzone) == m_LUTMap.end())
                            {
                                typename LUTMapType::iterator iter = m_LUTMap.begin();
                                std::string lStrZonesAvalaible = "";
                                while (iter != m_LUTMap.end())
                                {
                                    // Get the zone
                                    lStrZonesAvalaible += Utilities::AsString((*iter).first) + ",";
                                    ++iter;
                                }
                                vnsExceptionBusinessMacro("For band id '"<<id<<"' (nb channel='"<<nbOfChannels<<"') ' no miniLUT has been generated for this angle zone (detector) : '"<<lzone<<"'. Angles zones are ["<<lStrZonesAvalaible<<"] !!!,Check coherency between metadata and input zone mask source.")vnsExceptionBusinessMacro("For band id '"<<id<<"' (nb channel='"<<nbOfChannels<<"') ' no miniLUT has been generated for this angle zone '"<<lzone<<"'. Angles zones are ["<<lStrZonesAvalaible<<"] !!!")
                            }
                            // Interpolate value
                            // The LUT contains all the spectral bands at L2 coarse resolution.
                            // So, at L2 resolution, we have to use the L2 coarse band id that corresponds to the band at L2 resolution :
                            //      - SENTINEL2 R1 l2bandId = 3 (B8) l2coarseBandId = 7
                            //      - SENTINEL2 R2 l2bandId = 2 (B7) l2coarseBandId = 6
                            LUTPixelType interpValue = m_LUTMap[lzone]->InterpolateValue(point);

                            outPix[id] = static_cast<OutputImageInternalPixelType>(interpValue[l2CoarseBand]);
                        }
                        // Fill the output pixel iterator
                        outIt.Set(outPix);

                    }
                    else
                    {
                        outIt.Set(DefaultOutPix);
                    }

                    ++toaIt;
                    ++aotIt;
                    ++edgIt;
                    ++angleZoneIt;
                    ++dtmIt;
                    ++outIt;
                }

            }
            // ============================================================================================
            // The viewing angles are set at image center
            // ============================================================================================
            else
            {

                // Loop over the input and output images
                while ((outIt.IsAtEnd() == false) && (toaIt.IsAtEnd() == false) && (aotIt.IsAtEnd() == false) && (edgIt.IsAtEnd() == false)
                        && (dtmIt.IsAtEnd() == false))
                {
                    // Set the output default pix to NoData.
                    // If no edge
                    if ((edgIt.Get() == 0))
                    {

                        OutputImagePixelType outPix(DefaultOutPix);
                        LUTPointType point;
                        // Loop over the channels
                        const TOACImagePixelType & toaPix = toaIt.Get();
                        point[0] = static_cast<LUTPointValueType>(dtmIt.Get());
                        point[1] = static_cast<LUTPointValueType>(aotIt.Get());

                        // Band loop
                        for (unsigned int id = 0; id < nbOfChannels; id++)
                        {
                            // Get the L2 coarse band id associated to the current band id
                            const unsigned int l2CoarseBand = m_ListOfL2CoarseBandId[id];

                            point[2] = static_cast<LUTPointValueType>(toaPix[id]);
                            // Interpolate value - only one miniLUT is generated with viewing angles at the center of the image
                            // the key of the map is 0 (no detector associated)
                            LUTPixelType interpValue = m_LUTMap[0]->InterpolateValue(point);

                            outPix[id] = static_cast<OutputImageInternalPixelType>(interpValue[l2CoarseBand]);
                        }
                        // Fill the output pixel iterator
                        outIt.Set(outPix);

                    }
                    else
                    {
                        outIt.Set(DefaultOutPix);
                    }

                    ++toaIt;
                    ++aotIt;
                    ++edgIt;
                    ++dtmIt;
                    ++outIt;
                }
            }
        }

    /** PrintSelf method */
    template<class TTOACInputImage, class TAOTInputImage, class TDTMInputImage, class TInputMask, class TVectorMask, class TOutputImage,
            class TLUTInput>
        void
        ComputeScatteringCorrectionImageFilter<TTOACInputImage, TAOTInputImage, TDTMInputImage, TInputMask, TVectorMask, TOutputImage,
                TLUTInput>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif
