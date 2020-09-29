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
 * VERSION : 5-1-0 : FA : LAIG-DM-MAC-1757-CNES : 19 juillet 2016 : amélioration du calcul des statistiques *
 *                                                                  de l’AOT                                *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-116796-CS : 20 février 2013 : Valeur du No_Data dans le plan PXD 		*
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 19 décembre 2013 : Améliorations algorithmiques			*
 * 								Suppression de la recherche des zones de forte AOT près des nuages			*
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 28 octobre 2013 : Mise a jour d'un commentaire            *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 03 novembre 2011 : Modification pour prise en compte       *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 6 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCompositeImageUpdateImageFilter_txx
#define __vnsCompositeImageUpdateImageFilter_txx

#include "vnsCompositeImageUpdateImageFilter.h"

#include "vnsMath.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
            class TOutputImage>
        CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
                TOutputImage>::CompositeImageUpdateImageFilter(): m_InitMode(false), m_RealL2NoData(0), m_L2NoData(0),
                m_MaxAOT(0.), m_MaxAge(0), m_HotspotFlag(false), m_CirrusFlag(false),
                m_RainFlag(false), m_JDay(0), m_AOTMean(0.), m_STOBandNumber(10), m_STOBandIndex(0)
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(13);
            this->SetNumberOfRequiredInputs(7);

            this->SetNumberOfIndexedOutputs(6);
            this->SetNumberOfRequiredOutputs(6);

            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, OutputImageType::New());
            this->SetNthOutput(2, OutputImageType::New());
            this->SetNthOutput(3, OutputImageType::New());
            this->SetNthOutput(4, DateImageType::New());
            this->SetNthOutput(5, InputMaskType::New());
        }

    // Destructor
    template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
            class TOutputImage>
        CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
                TOutputImage>::~CompositeImageUpdateImageFilter()
        {
        }

   // BeforeThreadedGenerateData Method
   template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
            class TOutputImage>
        void
        CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
                TOutputImage>::BeforeThreadedGenerateData()
        {

            // Get the required input images
            InputImageConstPointer l_IPTOACPtr = this->GetIPTOACInput();
            InputImageConstPointer l_IPTOCRPtr = this->GetIPTOCRInput();
            InputImageConstPointer l_IPTOCPtr = this->GetIPTOCInput();
            // Get the mask input pointer
            InputMaskConstPointer l_IPNotValidPtr = this->GetIPNotValidMaskSubInput();
            InputMaskConstPointer l_IPCLDSumPtr = this->GetIPCLDSumInput();
            VectorMaskConstPointer l_IPSATPtr = this->GetIPSATInput();
            InputMaskConstPointer l_HighAOTNCMaskPtr = this->GetHighAOTMaskInput();

            // Check the validity of
            // all input pointers
            if ((l_IPTOACPtr.IsNull() == true) || (l_IPTOCRPtr.IsNull() == true)
                    || (l_IPTOCPtr.IsNull() == true) || (l_IPNotValidPtr.IsNull() == true)
                    || (l_IPCLDSumPtr.IsNull() == true) || (l_IPSATPtr.IsNull() == true)
                    || (l_HighAOTNCMaskPtr.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

        }

   /** Generate output information method. */
    template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
            class TOutputImage>
        void
        CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
                TOutputImage>::GenerateOutputInformation()
        {

            // Call superclass implementation
            Superclass::GenerateOutputInformation();

            // Set the number of output channels to the number of input one.
            // Check IPTOAC image availability
            if (!this->GetIPTOACInput())
            {
                vnsExceptionBusinessMacro("Waiting for a TOAC image, but no one specified. Please set one.");
            }
            const unsigned int l_NumberOfBands(this->GetIPTOACInput()->GetNumberOfComponentsPerPixel());

            this->GetL2RTAOutput()->SetNumberOfComponentsPerPixel(l_NumberOfBands);
            this->GetL2RCROutput()->SetNumberOfComponentsPerPixel(l_NumberOfBands);
            this->GetL2RTCOutput()->SetNumberOfComponentsPerPixel(l_NumberOfBands);

            // If Init mode, no STO image has been generated before
            if (m_InitMode == true)
            {
                m_STOBandNumber = 1;
                this->GetL2STOOutput()->SetNumberOfComponentsPerPixel(m_STOBandNumber);
            }
            else
            {
                if (!this->GetL2inSTOInput())
                {
                    vnsExceptionBusinessMacro("Waiting for a STO image, but no one specified. Please set one.");
                }

                const unsigned int l_NumberOfL2inSTOBands(this->GetL2inSTOInput()->GetNumberOfComponentsPerPixel());

                // The number of output STO band depends on the
                // number of previous STO images generated since
                // init mode (D-1 to D-N_WM).
                // Then, the current date D is added to the previous ones.
                if (l_NumberOfL2inSTOBands < m_STOBandNumber)
                {
                    m_STOBandNumber = l_NumberOfL2inSTOBands + 1;
                    this->GetL2STOOutput()->SetNumberOfComponentsPerPixel(m_STOBandNumber);
                }
                // In this case, the older mask is replaced by the new one (date D)
                else
                {
                    this->GetL2STOOutput()->SetNumberOfComponentsPerPixel(m_STOBandNumber);
                }
            }

            // If Init mode, no PXD image has been generated before
            if (m_InitMode == true)
            {
                this->GetL2PXDOutput()->SetNumberOfComponentsPerPixel(1);
            }
            else
            {
                if (!this->GetL2inPXDInput())
                {
                    vnsExceptionBusinessMacro("Waiting for a L2inPXD image, but no one specified. Please set one.");
                }
                unsigned int l_NumberOfDateBands(0);
                l_NumberOfDateBands = this->GetL2inPXDInput()->GetNumberOfComponentsPerPixel();

                this->GetL2PXDOutput()->SetNumberOfComponentsPerPixel(l_NumberOfDateBands);
            }

            // If Init mode, no NDT image has been generated before
            if (m_InitMode == true)
            {
                this->GetL2NDTOutput()->SetNumberOfComponentsPerPixel(1);
            }
            else
            {
                if (!this->GetL2inNDTInput())
                {
                    vnsExceptionBusinessMacro("Waiting for a L2inNDT image, but no one specified. Please set one.");
                }
                unsigned int l_NumberOfMaskBands(0);
                l_NumberOfMaskBands = this->GetL2inNDTInput()->GetNumberOfComponentsPerPixel();

                this->GetL2NDTOutput()->SetNumberOfComponentsPerPixel(l_NumberOfMaskBands);
            }

        }

   //====================== Main computation method ======================/
   template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
            class TOutputImage>
        void
        CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
                TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {

            // Load inputs
            // Input availability are checked in the BeforeThreadedGenerateData.

            // Get the required input images
            InputImageConstPointer l_IPTOACPtr = this->GetIPTOACInput();
            InputImageConstPointer l_IPTOCRPtr = this->GetIPTOCRInput();
            InputImageConstPointer l_IPTOCPtr = this->GetIPTOCInput();
            InputMaskConstPointer l_IPNotValidPtr = this->GetIPNotValidMaskSubInput();
            InputMaskConstPointer l_IPCLDSumPtr = this->GetIPCLDSumInput();
            VectorMaskConstPointer l_IPSATPtr = this->GetIPSATInput();

            InputMaskConstPointer l_HighAOTNCMaskPtr = this->GetHighAOTMaskInput();

            // Optional inputs declaration (L2 in images not set in init mode)
            InputImageConstPointer l_L2inRTAPtr;
            InputImageConstPointer l_L2inRCRPtr;
            InputImageConstPointer l_L2inRTCPtr;
            InputImageConstPointer l_L2inSTOPtr;
            DateImageConstPointer l_L2inPXDPtr;
            InputMaskConstPointer l_L2inNDTPtr;

            // Load outputs
            OutputImagePointer l_L2RTAPtr = this->GetL2RTAOutput();
            OutputImagePointer l_L2RCRPtr = this->GetL2RCROutput();
            OutputImagePointer l_L2RTCPtr = this->GetL2RTCOutput();
            OutputImagePointer l_L2STOPtr = this->GetL2STOOutput();
            DateImagePointer l_L2PXDPtr = this->GetL2PXDOutput();
            InputMaskPointer l_L2NDTPtr = this->GetL2NDTOutput();


            // Prepare the threaded region input local iterators
            InputImageConstIteratorType l_IPTOACIt(l_IPTOACPtr, outputRegionForThread);
            InputImageConstIteratorType l_IPTOCRIt(l_IPTOCRPtr, outputRegionForThread);
            InputImageConstIteratorType l_IPTOCIt(l_IPTOCPtr, outputRegionForThread);

            itk::ImageRegionConstIterator<InputMaskType> l_IPNotValidIt(l_IPNotValidPtr,
                    outputRegionForThread);

            itk::ImageRegionConstIterator<InputMaskType> l_IPCLDSumIt(l_IPCLDSumPtr,
                                outputRegionForThread);
            itk::ImageRegionConstIterator<VectorMaskType> l_IPSATIt(l_IPSATPtr,
                                            outputRegionForThread);

            itk::ImageRegionConstIterator<InputMaskType> l_HighAOTNCMaskIt(l_HighAOTNCMaskPtr,
                                            outputRegionForThread);

            InputImageConstIteratorType l_L2inRTAIt;
            InputImageConstIteratorType l_L2inRCRIt;
            InputImageConstIteratorType l_L2inRTCIt;
            InputImageConstIteratorType l_L2inSTOIt;
            itk::ImageRegionConstIterator<DateImageType> l_L2inPXDIt;
            itk::ImageRegionConstIterator<InputMaskType> l_L2inNDTIt;


            // Init to the beginning
            l_IPTOACIt.GoToBegin();
            l_IPTOCRIt.GoToBegin();
            l_IPTOCIt.GoToBegin();
            l_IPNotValidIt.GoToBegin();
            l_IPCLDSumIt.GoToBegin();
            l_IPSATIt.GoToBegin();
            l_HighAOTNCMaskIt.GoToBegin();

            // Prepare the threaded region output local iterator
            OutputImageIteratorType l_L2RTAIt(l_L2RTAPtr, outputRegionForThread);
            OutputImageIteratorType l_L2RCRIt(l_L2RCRPtr, outputRegionForThread);
            OutputImageIteratorType l_L2RTCIt(l_L2RTCPtr, outputRegionForThread);
            OutputImageIteratorType l_L2STOIt(l_L2STOPtr, outputRegionForThread);
            itk::ImageRegionIterator<DateImageType>
                    l_L2PXDIt(l_L2PXDPtr, outputRegionForThread);
            itk::ImageRegionIterator<InputMaskType>
                    l_L2NDTIt(l_L2NDTPtr, outputRegionForThread);

            // Init to the beginning
            l_L2RTAIt.GoToBegin();
            l_L2RCRIt.GoToBegin();
            l_L2RTCIt.GoToBegin();
            l_L2STOIt.GoToBegin();
            l_L2PXDIt.GoToBegin();
            l_L2NDTIt.GoToBegin();

            // Progress reporter

// 4.4            unsigned int l_NumberOfSTOBands(0);
//            l_NumberOfSTOBands = l_L2STOPtr->GetNumberOfComponentsPerPixel();

            VectorMaskVectorType l_IPSATNull;
            l_IPSATNull.SetSize(l_IPSATPtr->GetNumberOfComponentsPerPixel());
            l_IPSATNull.Fill(static_cast<VectorMaskInternalPixelType>(0));

            // STANDARD MODE
            if (m_InitMode == false)
            {

                // Optional inputs declaration (L2 in images not set in init mode)
                l_L2inRTAPtr = this->GetL2inRTAInput();
                l_L2inRCRPtr = this->GetL2inRCRInput();
                l_L2inRTCPtr = this->GetL2inRTCInput();
                l_L2inSTOPtr = this->GetL2inSTOInput();
                l_L2inPXDPtr = this->GetL2inPXDInput();
                l_L2inNDTPtr = this->GetL2inNDTInput();

                // Prepare the threaded region input local iterators
                l_L2inRTAIt = InputImageConstIteratorType(l_L2inRTAPtr, outputRegionForThread);
                l_L2inRCRIt = InputImageConstIteratorType (l_L2inRCRPtr, outputRegionForThread);
                l_L2inRTCIt = InputImageConstIteratorType (l_L2inRTCPtr, outputRegionForThread);
                l_L2inSTOIt = InputImageConstIteratorType (l_L2inSTOPtr, outputRegionForThread);

                l_L2inPXDIt = itk::ImageRegionConstIterator<DateImageType> (l_L2inPXDPtr,
                        outputRegionForThread);

                l_L2inNDTIt = itk::ImageRegionConstIterator<InputMaskType>(l_L2inNDTPtr,
                                    outputRegionForThread);

                // Init to the beginning
                l_L2inRTAIt.GoToBegin();
                l_L2inRCRIt.GoToBegin();
                l_L2inRTCIt.GoToBegin();
                l_L2inSTOIt.GoToBegin();
                l_L2inPXDIt.GoToBegin();
                l_L2inNDTIt.GoToBegin();

                // Nominal case => no global flag is raised
                if ((m_HotspotFlag == false) && (m_CirrusFlag == false) && (m_RainFlag == false)
                        && (m_AOTMean < m_MaxAOT))
                {

                    // Loop over input and output images
                    while (l_IPTOCRIt.IsAtEnd() == false)
                    {

                        // If pixel is not cloudy, nor edges, nor high AOT near cloud, nor saturated in
                        // any of the spectral bands
                        if ((l_IPCLDSumIt.Get() == 0) && (l_IPNotValidIt.Get() == 0) &&
                                (l_HighAOTNCMaskIt.Get() == 0) && (l_IPSATIt.Get() == l_IPSATNull))
                        {

                            // ========> Call the FillPixels method that updates reflectance
                            // values with the current ones, sets the pixel date with the product
                            // one and the no_data pixel mask to 0
                            FillPixels(l_IPTOACIt, l_IPTOCRIt, l_IPTOCIt,
                                    m_JDay, static_cast<InputMaskPixelType> (0), l_L2RTAIt,
                                    l_L2RCRIt, l_L2RTCIt, l_L2PXDIt, l_L2NDTIt);

                        }
                        // If the pixel is cloudy, edge, high AOT near cloud or saturated
                        else
                        {
                            // ========> Test if the pixel value is not too old in the composite product
                            OldReflectanceTest(l_L2inRTAIt, l_L2inRCRIt, l_L2inRTCIt,
                                    l_L2inPXDIt, static_cast<InputMaskPixelType> (0), l_L2RTAIt,
                                    l_L2RCRIt, l_L2RTCIt, l_L2PXDIt, l_L2NDTIt);

                        } // end if pixel cloudy, edge ...


                        // ========> Update the stack of images (STO) (surface reflectance
                        // corrected by Rayleigh (RCR))
                        FillSTOPixels(l_L2inSTOIt, l_IPTOCRIt, l_L2STOIt);


                        // input image iterators
                        ++l_IPTOACIt;
                        ++l_L2inRTAIt;
                        ++l_IPTOCRIt;
                        ++l_L2inRCRIt;
                        ++l_IPTOCIt;
                        ++l_L2inRTCIt;
                        ++l_L2inSTOIt;
                        ++l_L2inPXDIt;
                        ++l_IPNotValidIt;
                        ++l_L2inNDTIt;
                        ++l_IPCLDSumIt;
                        ++l_IPSATIt;
                        ++l_HighAOTNCMaskIt;

                        // output image iterators
                        ++l_L2RTAIt;
                        ++l_L2RCRIt;
                        ++l_L2RTCIt;
                        ++l_L2STOIt;
                        ++l_L2PXDIt;
                        ++l_L2NDTIt;

                    } // end pixel loop

                } // end nominal case

                // One global flag for the image has been raised

                // The composite is not updated

                else
                {

                    // Loop over input and output images
                     while (l_IPTOCRIt.IsAtEnd() == false)
                     {

                         // ========> Test if the pixel value is not too old in the composite product
                         OldReflectanceTest(l_L2inRTAIt, l_L2inRCRIt, l_L2inRTCIt, l_L2inPXDIt,
                                static_cast<InputMaskPixelType> (0), l_L2RTAIt, l_L2RCRIt,
                                l_L2RTCIt, l_L2PXDIt, l_L2NDTIt);


                         // ========> Update the stack of images (STO) (surface reflectance
                         // corrected by Rayleigh (RCR))
                         FillSTOPixels(l_L2inSTOIt, l_IPTOCRIt, l_L2STOIt);


                         // input image iterators
                         ++l_IPTOACIt;
                         ++l_L2inRTAIt;
                         ++l_IPTOCRIt;
                         ++l_L2inRCRIt;
                         ++l_IPTOCIt;
                         ++l_L2inRTCIt;
                         ++l_L2inSTOIt;
                         ++l_L2inPXDIt;
                         ++l_IPNotValidIt;
                         ++l_L2inNDTIt;
                         ++l_IPCLDSumIt;
                         ++l_IPSATIt;
                         ++l_HighAOTNCMaskIt;

                         // output image iterators
                         ++l_L2RTAIt;
                         ++l_L2RCRIt;
                         ++l_L2RTCIt;
                         ++l_L2STOIt;
                         ++l_L2PXDIt;
                         ++l_L2NDTIt;
                     }

                } // if one global flag raised
            }
            // INIT MODE
            else
            {
                // Nominal case => no global flag is raised
                // No test on mean AOT and rain flag is not availablee in init mode
                if ( (m_HotspotFlag == false) && (m_CirrusFlag == false) )
                {

                    // Loop over input and output images
                    while (l_IPTOCRIt.IsAtEnd() == false)
                    {

                        // If pixel is not cloudy, nor (edges or snow), nor high AOT near cloud, nor saturated in
                        // any of the spectral bands
                        if ((l_IPCLDSumIt.Get() == 0) && (l_IPNotValidIt.Get() == 0) &&
                                (l_HighAOTNCMaskIt.Get() == 0) && (l_IPSATIt.Get() == l_IPSATNull))
                        {

                            // ========> Call the FillPixels method
                            FillPixels(l_IPTOACIt, l_IPTOCRIt, l_IPTOCIt,
                                   m_JDay, static_cast<InputMaskPixelType> (0), l_L2RTAIt,
                                   l_L2RCRIt, l_L2RTCIt, l_L2PXDIt, l_L2NDTIt);

                        }
                        // If the pixel is cloudy or edge or high AOT near cloud or saturated
                        else
                        {
                            // =========> Set the output pixels to no_data
                            FillPixels(l_L2RTAIt, l_L2RCRIt, l_L2RTCIt, l_L2PXDIt, l_L2NDTIt);

                        } // end if pixel cloudy, edge ...

                        // ========> Update the stack of images (STO) (surface reflectance
                        // corrected by Rayleigh (RCR))
                        FillSTOPixels(l_L2inSTOIt, l_IPTOCRIt, l_L2STOIt);


                        // input image iterators
                        ++l_IPTOACIt;
                        ++l_IPTOCRIt;
                        ++l_IPTOCIt;
                        ++l_IPNotValidIt;
                        ++l_IPCLDSumIt;
                        ++l_IPSATIt;
                        ++l_HighAOTNCMaskIt;

                        // output image iterators
                        ++l_L2RTAIt;
                        ++l_L2RCRIt;
                        ++l_L2RTCIt;
                        ++l_L2STOIt;
                        ++l_L2PXDIt;
                        ++l_L2NDTIt;

                    } // end pixel loop

                } // end nominal case

                // One global flag for the image has been raised

                // The composite is not updated and set to no_data
                else
                {

                    // Loop over input and output images
                     while (l_IPTOCRIt.IsAtEnd() == false)
                     {

                         // =========> Set the output pixels to no_data
                         FillPixels(l_L2RTAIt, l_L2RCRIt, l_L2RTCIt, l_L2PXDIt, l_L2NDTIt);

                         // ========> Update the stack of images (STO) (surface reflectance
                         // corrected by Rayleigh (RCR))
                         FillSTOPixels(l_L2inSTOIt, l_IPTOCRIt, l_L2STOIt);


                         // input image iterators
                         ++l_IPTOACIt;
                         ++l_IPTOCRIt;
                         ++l_IPTOCIt;
                         ++l_IPNotValidIt;
                         ++l_IPCLDSumIt;
                         ++l_IPSATIt;
                         ++l_HighAOTNCMaskIt;

                         // output image iterators
                         ++l_L2RTAIt;
                         ++l_L2RCRIt;
                         ++l_L2RTCIt;
                         ++l_L2STOIt;
                         ++l_L2PXDIt;
                         ++l_L2NDTIt;
                     }

                } // if one global flag raised
            } // end if Init mode

        }

   // OldReflectanceTest Method
    template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
            class TOutputImage>
        void
        CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
                TOutputImage>::OldReflectanceTest(InputImageConstIteratorType& l_L2inRTAIt,
                InputImageConstIteratorType& l_L2inRCRIt, InputImageConstIteratorType& l_L2inRTCIt,
                DateImageConstIteratorType& l_L2inPXDIt, const InputMaskPixelType& inMsk,
                OutputImageIteratorType& l_L2RTAIt, OutputImageIteratorType& l_L2RCRIt,
                OutputImageIteratorType& l_L2RTCIt, DateImageIteratorType& l_L2PXDIt,
                InputMaskIteratorType& l_L2NDTIt)
        {

            // Test if the pixel value is not too old in the composite product
            if (vcl_abs(m_JDay - l_L2inPXDIt.Get()) <= m_MaxAge)
            {
                FillPixels(l_L2inRTAIt, l_L2inRCRIt, l_L2inRTCIt, l_L2inPXDIt.Get(),
                        inMsk, l_L2RTAIt, l_L2RCRIt, l_L2RTCIt,
                        l_L2PXDIt, l_L2NDTIt);
            }
            // Set the output pixel to no_data and raised the pixel in the no_data mask
            else
            {
                FillPixels(l_L2RTAIt, l_L2RCRIt, l_L2RTCIt, l_L2PXDIt, l_L2NDTIt);
            }
        }

   // FillPixels method in the nominal case
   template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
          class TOutputImage>
      void
      CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
              TOutputImage>::FillPixels(InputImageConstIteratorType& inIt1,
                                        InputImageConstIteratorType& inIt2,
                                        InputImageConstIteratorType& inIt3,
                                        const DateImagePixelType& inPxd,
                                        const InputMaskPixelType& inMsk,
                                        OutputImageIteratorType& outIt1,
                                        OutputImageIteratorType& outIt2,
                                        OutputImageIteratorType& outIt3,
                                        DateImageIteratorType& outPxdIt,
                                        InputMaskIteratorType& outMskIt)
      {
           // Update the pixel value in the composite product
           outIt1.Set(static_cast<OutputImagePixelType>(inIt1.Get()));
           outIt2.Set(static_cast<OutputImagePixelType>(inIt2.Get()));
           outIt3.Set(static_cast<OutputImagePixelType>(inIt3.Get()));
           outPxdIt.Set(inPxd);
           outMskIt.Set(inMsk);
      }

   // FillPixels method with no_data value
   template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
          class TOutputImage>
      void
      CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
              TOutputImage>::FillPixels(OutputImageIteratorType& outIt1,
                                        OutputImageIteratorType& outIt2,
                                        OutputImageIteratorType& outIt3,
                                        DateImageIteratorType& outPxdIt,
                                        InputMaskIteratorType& outMskIt)
      {
           OutputVectorType l_NoDataVect;

           // Get the number of band of the input reflectance images
           unsigned int l_NbOfBands(0);
           l_NbOfBands = outIt1.Get().GetSize();

           // Create a VariableLengthVector of no_data
           l_NoDataVect.SetSize(l_NbOfBands);
           l_NoDataVect.Fill(m_RealL2NoData);

           // Set the output pixels to no_data
           outIt1.Set(static_cast<OutputImagePixelType>(l_NoDataVect));
           outIt2.Set(static_cast<OutputImagePixelType>(l_NoDataVect));
           outIt3.Set(static_cast<OutputImagePixelType>(l_NoDataVect));
           // FA 116796: Set No_Data value (-10000) and not real No_Data value ( No_Data x QuantificationValue = -10)
           // to have the same No_Data value in all the L2 output images
           outPxdIt.Set(static_cast<DateImagePixelType>(m_L2NoData));
           outMskIt.Set(static_cast<InputMaskPixelType>(1));
      }


   // FillSTOPixels method with the L2RCR output pixel values
   template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
          class TOutputImage>
      void
      CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
              TOutputImage>::FillSTOPixels(InputImageConstIteratorType& inSTOIt,
                                        InputImageConstIteratorType& inTOCRIt,
                                        OutputImageIteratorType& outSTOIt)
      {

            // Update the stack of images (STO) (surface reflectance
            // corrected by Rayleigh (RCR))

            OutputImagePixelType l_Pix;
            l_Pix.SetSize(m_STOBandNumber);
            l_Pix.Fill(static_cast<OutputInternalPixelType>(0));

            // Copy the current RCR pixel value in the first STO band
            l_Pix[0] = static_cast<OutputInternalPixelType>(inTOCRIt.Get()[m_STOBandIndex]);

            // Copy the previous masks in the output ones
            for (unsigned int l_date = 1; l_date < m_STOBandNumber; l_date++)
            {
                l_Pix[l_date] = static_cast<OutputInternalPixelType>(inSTOIt.Get()[l_date - 1]);
            }
            outSTOIt.Set(l_Pix);

      }


    // PrintSelf method
    template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage,
            class TOutputImage>
        void
        CompositeImageUpdateImageFilter<TInputImage, TInputMask, TVectorMask, TDateImage,
                TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << "m_InitMode:       "<< m_InitMode <<std::endl;
            os << "m_RealL2NoData:   "<< m_RealL2NoData <<std::endl;
            os << "m_L2NoData:       "<< m_L2NoData <<std::endl;
            os << "m_MaxAOT:         "<< m_MaxAOT <<std::endl;
            os << "m_MaxAge:         "<< m_MaxAge <<std::endl;
            os << "m_HotspotFlag:    "<< m_HotspotFlag <<std::endl;
            os << "m_CirrusFlag:     "<< m_CirrusFlag <<std::endl;
            os << "m_RainFlag:       "<< m_RainFlag <<std::endl;
            os << "m_JDay:           "<< m_JDay <<std::endl;
            os << "m_AOTMean:        "<< m_AOTMean <<std::endl;
            os << "m_STOBandNumer    "<< m_STOBandNumber <<std::endl;
            os << "m_STOBandIndex    "<< m_STOBandIndex <<std::endl;
        }

} // End namespace vns

#endif /* __vnsCompositeImageUpdateImageFilter_txx */
