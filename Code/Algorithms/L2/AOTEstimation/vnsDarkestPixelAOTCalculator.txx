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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mars 2014 : Améliorations algorithmiques                *
 * 								Sélectionne le "dark" pixel correspondant à l'altitude minimale (FD 118993) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juin 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef vnsDarkestPixelAOTCalculator_txx
#define vnsDarkestPixelAOTCalculator_txx

#include "vnsDarkestPixelAOTCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageScanlineConstIterator.h"

#include "vnsMath.h"

namespace vns
{

    template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
        PersistentDarkestPixelAOTCalculator<TInputImage, TInputDTM, TInputMask, TLUT>::PersistentDarkestPixelAOTCalculator() :
                m_L2inIsAvalaible(true)
        {

            this->SetNumberOfIndexedInputs(7);
            this->SetNumberOfRequiredInputs(5);
            this->SetNumberOfIndexedOutputs(2);

            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types
            typename DoubleObjectType::Pointer outputDouble = static_cast<DoubleObjectType*>(this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, outputDouble.GetPointer());

            typename BoolObjectType::Pointer outputBool = static_cast<BoolObjectType*>(this->MakeOutput(2).GetPointer());
            this->itk::ProcessObject::SetNthOutput(2, outputBool.GetPointer());

            m_DarkestChannelTOAC = 0;
            m_DarkestChannelRTC = 0;
            m_DarkSurfaceReflectance = 0;
            m_ThresholdDarkPixel = 0;

        }

    template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
        itk::DataObject::Pointer
        PersistentDarkestPixelAOTCalculator<TInputImage, TInputDTM, TInputMask, TLUT>::MakeOutput(unsigned int output)
        {
            switch (output)
                {
            case 0:
                {
                    return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                }
            case 1:
                {
                    return static_cast<itk::DataObject*>(DoubleObjectType::New().GetPointer());
                }
            case 2:
                {
                    return static_cast<itk::DataObject*>(BoolObjectType::New().GetPointer());
                }
            default:
                {
                    // might as well make an image
                    return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                }
                }
        }

    template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
        void
        PersistentDarkestPixelAOTCalculator<TInputImage, TInputDTM, TInputMask, TLUT>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {
            /**
             * Grab the inputs
             */
            InputImagePointerType IPTOACSubPtr = const_cast<InputImageType *>(this->GetIPTOACSub());

            InputMaskPointerType L2CLDSubPtr = const_cast<InputMaskType *>(this->GetL2CLDSub());

            InputMaskPointerType IPSHDSubPtr = const_cast<InputMaskType *>(this->GetIPSHDSub());

            InputMaskPointerType IPEDGPtr = const_cast<InputMaskType *>(this->GetIPEDG());

            InputDTMPointerType DTMSubPtr = const_cast<InputDTMType *>(this->GetDTMSub());

//            InputMaskPointerType L2inEDGPtr;
//            InputImagePointerType L2inRTCPtr;

            // Support progress methods/callbacks

            /* Iterators */
//            itk::ImageRegionConstIterator < InputImageType > itIPTOACSub(IPTOACSubPtr, outputRegionForThread);
//            itk::ImageRegionConstIterator < InputMaskType > itL2CLDSub(L2CLDSubPtr, outputRegionForThread);
//            itk::ImageRegionConstIterator < InputMaskType > itIPSHDSub(IPSHDSubPtr, outputRegionForThread);
//            itk::ImageRegionConstIterator < InputMaskType > itIPEDG(IPEDGPtr, outputRegionForThread);
//            itk::ImageRegionConstIterator < InputDTMType > itDTMSub(DTMSubPtr, outputRegionForThread);
            typedef itk::ImageRegionConstIteratorWithIndex<InputImageType>   InputImageConstIteratorWithIndexType;
            typedef itk::ImageRegionConstIteratorWithIndex<InputMaskType>   InputMaskConstIteratorWithIndexType;
            itk::ImageScanlineConstIterator<InputImageType> itIPTOACSub(IPTOACSubPtr, outputRegionForThread);
            itk::ImageScanlineConstIterator<InputMaskType> itL2CLDSub(L2CLDSubPtr, outputRegionForThread);
            itk::ImageScanlineConstIterator<InputMaskType> itIPSHDSub(IPSHDSubPtr, outputRegionForThread);
            itk::ImageScanlineConstIterator<InputMaskType> itIPEDG(IPEDGPtr, outputRegionForThread);
            itk::ImageScanlineConstIterator<InputDTMType> itDTMSub(DTMSubPtr, outputRegionForThread);

//            typedef itk::ImageLinearConstIteratorWithIndex<InputImageType> InputImageConstIteratorWithIndexType;
//            typedef itk::ImageLinearConstIteratorWithIndex<InputMaskType> InputMaskConstIteratorWithIndexType;

            InputMaskConstIteratorWithIndexType itL2inEDG;
            InputImageConstIteratorWithIndexType itL2inRTC;

//            itIPTOACSub.GoToBegin();
//            itL2CLDSub.GoToBegin();
//            itIPSHDSub.GoToBegin();
//            itIPEDG.GoToBegin();
//            itDTMSub.GoToBegin();

            // ===================================================
            // In nominal mode the composite product is available
            // ===================================================
            if (m_L2inIsAvalaible == true)
            {

                InputMaskPointerType L2inEDGPtr = const_cast<InputMaskType *>(this->GetL2inEDG());
                InputImagePointerType L2inRTCPtr = const_cast<InputImageType *>(this->GetL2inRTC());

                InputMaskConstIteratorWithIndexType itL2inEDG = InputMaskConstIteratorWithIndexType(L2inEDGPtr, outputRegionForThread);
                InputImageConstIteratorWithIndexType itL2inRTC = InputImageConstIteratorWithIndexType(L2inRTCPtr, outputRegionForThread);

                itIPTOACSub.GoToBegin();
                itL2CLDSub.GoToBegin();
                itIPSHDSub.GoToBegin();
                itIPEDG.GoToBegin();
                itDTMSub.GoToBegin();

//                itL2inEDG.SetDirection(2);
//                itL2inRTC.SetDirection(2);
                itL2inEDG.GoToBegin();
                itL2inRTC.GoToBegin();

                // do the work
                while (itIPTOACSub.IsAtEnd() == false)
                {
// V 4.4 - optimization
                    while (itIPTOACSub.IsAtEndOfLine() == false)
                    {
//                        InputImagePixelType lL2inRTCPix = itL2inRTC.Get(); // R_surf_sub(D)
                        const InputImagePixelType & lL2inRTCPix = itL2inRTC.Get(); // R_surf_sub(D)

                        const double lL2inRTCVal = static_cast<double>(lL2inRTCPix[m_DarkestChannelRTC]);

                        if ((lL2inRTCVal < m_ThresholdDarkPixel) && (itL2CLDSub.Get() == 0) && (itIPSHDSub.Get() == 0)
                                && (itIPEDG.Get() == 0) && (itL2inEDG.Get() == 0))
                        {

//                            InternalPixelType lIPTOACSubVal = itIPTOACSub.Get()[m_DarkestChannelTOAC];
                            const InternalPixelType & lIPTOACSubVal = itIPTOACSub.Get()[m_DarkestChannelTOAC];

                            // Find the lowest value of IPTOACSub
                            // If several pixels have the lowest value of IPTOACSub
                            if (vnsEqualsDoubleMacro(m_ThreadMin[threadId], lIPTOACSubVal))
                            {
                                // Select the pixel associated to the lowest altitude
                                // TODO PERF: extract variable itDTMSub.Get()
                                if (m_ThreadAlt[threadId] > itDTMSub.Get())
                                {
                                    m_ThreadAlt[threadId] = itDTMSub.Get();
                                    m_ThreadIndex[threadId] = itL2inRTC.GetIndex();
                                }
                            }
                            else
                            {
                                if (m_ThreadMin[threadId] > lIPTOACSubVal)
                                {
                                    m_ThreadMin[threadId] = lIPTOACSubVal; // R_toa*_sub
                                    m_ThreadIndex[threadId] = itL2inRTC.GetIndex();
                                    m_ThreadAlt[threadId] = itDTMSub.Get();
                                }
                            }
                        }

                        ++itIPTOACSub;
                        ++itL2inRTC;
                        ++itL2CLDSub;
                        ++itIPSHDSub;
                        ++itIPEDG;
                        ++itL2inEDG;
                        ++itDTMSub;
                    }
                    itIPTOACSub.NextLine();
                    itL2CLDSub.NextLine();
                    itIPSHDSub.NextLine();
                    itIPEDG.NextLine();
                    itDTMSub.NextLine();
                }
            } // end if L2inIsAvailable

            // ===================================================
            // In init mode the composite product is not available
            // ===================================================
            else
            {
                itIPTOACSub.GoToBegin();
                itL2CLDSub.GoToBegin();
                itIPSHDSub.GoToBegin();
                itIPEDG.GoToBegin();
                itDTMSub.GoToBegin();
                // do the work
                while (itIPTOACSub.IsAtEnd() == false)
                {
                    while (itIPTOACSub.IsAtEndOfLine() == false)
                    {
                        if ((itL2CLDSub.Get() == 0) && (itIPSHDSub.Get() == 0) && (itIPEDG.Get() == 0))
                        {
//                            InternalPixelType lIPTOACSubVal = itIPTOACSub.Get()[m_DarkestChannelTOAC];
                            const InternalPixelType & lIPTOACSubVal = itIPTOACSub.Get()[m_DarkestChannelTOAC];

                            // Find the lowest value of IPTOACSub
                            if (vnsEqualsDoubleMacro(m_ThreadMin[threadId], lIPTOACSubVal))
                            {
                                // Select the pixel associated to the lowest altitude
                                if (m_ThreadAlt[threadId] > itDTMSub.Get())
                                {
                                    m_ThreadAlt[threadId] = itDTMSub.Get();
// V 4.4 - bug correction; No numeric impact because m_ThreadIndex is used only for debug (print index)!!!
//                                    m_ThreadIndex[threadId] = itL2inRTC.GetIndex();
                                    m_ThreadIndex[threadId] = itIPTOACSub.GetIndex();
                                }
                            }
                            else
                            {
                                if (m_ThreadMin[threadId] > lIPTOACSubVal)
                                {
                                    m_ThreadMin[threadId] = lIPTOACSubVal; // R_toa*_sub
                                    m_ThreadIndex[threadId] = itIPTOACSub.GetIndex();
                                    m_ThreadAlt[threadId] = itDTMSub.Get();
                                }
                            }
                            // If several pixels have the lowest value of IPTOACSub

                        }

                        ++itIPTOACSub;
                        ++itL2CLDSub;
                        ++itIPSHDSub;
                        ++itIPEDG;
                        ++itDTMSub;
                    }
                    itIPTOACSub.NextLine();
                    itL2CLDSub.NextLine();
                    itIPSHDSub.NextLine();
                    itIPEDG.NextLine();
                    itDTMSub.NextLine();
                }
            } // end if L2inIsAvailable
        }

    template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
        void
        PersistentDarkestPixelAOTCalculator<TInputImage, TInputDTM, TInputMask, TLUT>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
        void
        PersistentDarkestPixelAOTCalculator<TInputImage, TInputDTM, TInputMask, TLUT>::GenerateOutputInformation()
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

    template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
        void
        PersistentDarkestPixelAOTCalculator<TInputImage, TInputDTM, TInputMask, TLUT>::Synthetize(void)
        {
            const unsigned int lNumberOfThreads = this->GetNumberOfThreads();
            double lDarkestPixelTOARefl = m_ThreadMin[0];
            IndexType lIndex = m_ThreadIndex[0];
            double lAlt = m_ThreadAlt[0]; //0.;

            /** Look for the minimum reflectance and corresponding altitude */
            for (unsigned int i = 1; i < lNumberOfThreads; i++)
            {
                // If several pixels have the lowest value of IPTOACSub
                if (vnsEqualsDoubleMacro(m_ThreadMin[i], lDarkestPixelTOARefl))
                {
                    // Select the pixel associated to the lowest altitude
                    if (m_ThreadAlt[i] < lAlt)
                    {
                        lAlt = m_ThreadAlt[i];
                        lIndex = m_ThreadIndex[i];
                    }
                }
                else
                {
                    if (m_ThreadMin[i] < lDarkestPixelTOARefl)
                    {
                        lDarkestPixelTOARefl = m_ThreadMin[i];
                        lIndex = m_ThreadIndex[i];
                        lAlt = m_ThreadAlt[i];
                    }
                }
            }

            vnsLogDebugMacro("DarkestPixelTOARefl : " << lDarkestPixelTOARefl);
            vnsLogDebugMacro("Associated index : "<<lIndex);
            vnsLogDebugMacro("DTM value : " << lAlt);

            /** Find AOTdark with miniLUT and :
             * lAlt                     -> h_DTM
             * lDarkestPixelTOARefl     -> rô_TOAdark
             * m_DarkSurfaceReflectance -> rô_surfdark */
            LUTPointType lLUTPoint;
            lLUTPoint[0] = m_LUT->FindIndexInParameterValues(0, lAlt);
            lLUTPoint[1] = 0; // AOT dimension : searched information
            lLUTPoint[2] = m_LUT->FindIndexInParameterValues(2, lDarkestPixelTOARefl);

            /** dimension 1 represents AOT dimension */
            unsigned int lTargetedDim = 1; // AOT Dim
            double lDarkestPixelAOT = 0;
            bool lResultFound = m_LUT->InvertLUT(lLUTPoint, m_DarkestChannelTOAC, lTargetedDim, m_DarkSurfaceReflectance, lDarkestPixelAOT);

            typename DoubleObjectType::Pointer outputDouble = static_cast<DoubleObjectType*>(this->itk::ProcessObject::GetOutput(1));

            outputDouble->Set(lDarkestPixelAOT);
            this->itk::ProcessObject::SetNthOutput(1, outputDouble.GetPointer());

            typename BoolObjectType::Pointer outputBool = static_cast<BoolObjectType*>(this->itk::ProcessObject::GetOutput(2));
            outputBool->Set(lResultFound);
            this->itk::ProcessObject::SetNthOutput(2, outputBool.GetPointer());

        }

    template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
        void
        PersistentDarkestPixelAOTCalculator<TInputImage, TInputDTM, TInputMask, TLUT>::Reset(void)
        {
            InputImageType * IPTOACSubPtr = const_cast<InputImageType *>(this->GetIPTOACSub());
            IPTOACSubPtr->UpdateOutputInformation();

            InputMaskType * L2CLDSubPtr = const_cast<InputMaskType *>(this->GetL2CLDSub());
            L2CLDSubPtr->UpdateOutputInformation();

            InputMaskType * IPSHDSubPtr = const_cast<InputMaskType *>(this->GetIPSHDSub());
            IPSHDSubPtr->UpdateOutputInformation();

            InputMaskType * IPEDGPtr = const_cast<InputMaskType *>(this->GetIPEDG());
            IPEDGPtr->UpdateOutputInformation();

            InputDTMType * DTMSubPtr = const_cast<InputDTMType *>(this->GetDTMSub());
            DTMSubPtr->UpdateOutputInformation();

            if (m_L2inIsAvalaible == true)
            {
                InputImageType * L2inRTCPtr = const_cast<InputImageType *>(this->GetL2inRTC());
                L2inRTCPtr->UpdateOutputInformation();

                InputMaskType * L2inEDGPtr = const_cast<InputMaskType *>(this->GetL2inEDG());
                L2inEDGPtr->UpdateOutputInformation();
            }

            const unsigned int numberOfThreads = this->GetNumberOfThreads();

            // Initialize
            const double lVal = itk::NumericTraits<double>::max();
            m_ThreadMin = DoubleListType(numberOfThreads, lVal);

            IndexType lIndex;
            lIndex.Fill(0);
            m_ThreadIndex = IndexListType(numberOfThreads, lIndex);

            // Initialize
            InputDTMInternalPixelType lAlt = itk::NumericTraits<InputDTMInternalPixelType>::Zero;
            m_ThreadAlt = AltListType(numberOfThreads, lAlt);
        }

} //end of namespace

#endif /* vnsDarkestPixelAOTCalculator_txx */
