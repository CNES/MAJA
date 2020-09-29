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
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 27 jan. 2014 : Calcul du nombre de mnuages seulement dans  *
 *                                                              les zones visibles                          *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution                *
 * VERSION : 1-0-0-3 : DM : 357 : 25 juil. 2011 : Add a the computation of cloud pixels.                      *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCountCirrusPixelGenerator_txx
#define __vnsCountCirrusPixelGenerator_txx

#include "vnsCountCirrusPixelGenerator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

#include "vnsMath.h"
#include "itkIntTypes.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask>
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::PersistentCountCirrusPixelGenerator()
        {
            this->itk::ProcessObject::SetNumberOfIndexedInputs(2);
            this->itk::ProcessObject::SetNumberOfRequiredInputs(2);
            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around real types

            for (int i = 1; i < 4; i++)
            {
                typename ULongPixelObjectType::Pointer output = static_cast<ULongPixelObjectType*>(this->MakeOutput(i).GetPointer());
                this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
            }

            m_AltitudeThreshold = static_cast<InternalPixelType>(0);
            m_L1NoData = static_cast<RealNoDataType>(0);

        }

    template<class TInputImage, class TInputMask>
        itk::DataObject::Pointer
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::MakeOutput(unsigned int output)
        {
            switch (output)
                {
            case 0:
                return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
            case 1:
            case 2:
            case 3:
                return static_cast<itk::DataObject*>(ULongPixelObjectType::New().GetPointer());
            default:
                // might as well make an image
                return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                }

        }

    template<class TInputImage, class TInputMask>
        typename PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::ULongPixelObjectType*
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::GetCirrusPixelNumberOutput()
        {
            return static_cast<ULongPixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
        }

    template<class TInputImage, class TInputMask>
        const typename PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::ULongPixelObjectType*
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::GetCirrusPixelNumberOutput() const
        {
            return static_cast<const ULongPixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
        }

    template<class TInputImage, class TInputMask>
        typename PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::ULongPixelObjectType*
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::GetValidPixelNumberOutput()
        {
            return static_cast<ULongPixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
        }

    template<class TInputImage, class TInputMask>
        const typename PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::ULongPixelObjectType*
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::GetValidPixelNumberOutput() const
        {
            return static_cast<const ULongPixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
        }

    template<class TInputImage, class TInputMask>
        typename PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::ULongPixelObjectType*
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::GetCloudPixelNumberOutput()
        {
            return static_cast<ULongPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
        }

    template<class TInputImage, class TInputMask>
        const typename PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::ULongPixelObjectType*
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::GetCloudPixelNumberOutput() const
        {
            return static_cast<const ULongPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
        }

    template<class TInputImage, class TInputMask>
        void
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::GenerateOutputInformation()
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

    template<class TInputImage, class TInputMask>
        void
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputImage, class TInputMask>
        void
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::Reset()
        {
            TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
            TInputMask * maskPtr = const_cast<TInputMask *>(this->GetEDGSubInput());
            inputPtr->UpdateOutputInformation();
            maskPtr->UpdateOutputInformation();

            const itk::ThreadIdType numberOfThreads = this->GetNumberOfThreads();

            // Initialize the tempories

            unsigned long tempTemporiesULongPixel(0);
            m_ThreadCirrus = ArrayULongPixelType(numberOfThreads, tempTemporiesULongPixel);
            m_ThreadCloud = ArrayULongPixelType(numberOfThreads, tempTemporiesULongPixel);

            m_ThreadValid = ArrayULongPixelType(numberOfThreads, tempTemporiesULongPixel);

        }

    template<class TInputImage, class TInputMask>
        void
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::Synthetize()
        {
            const itk::ThreadIdType numberOfThreads = this->GetNumberOfThreads();

            unsigned long l_CirrusPixelNumber(0);
            unsigned long l_CloudPixelNumber(0);
            unsigned long l_ValidPixelNumber(0);

            // Accumulate cirrus and valid pixels over all threads
            for (itk::ThreadIdType i = 0; i < numberOfThreads; i++)
            {
                l_CirrusPixelNumber += m_ThreadCirrus[i];
                l_CloudPixelNumber += m_ThreadCloud[i];
                l_ValidPixelNumber += m_ThreadValid[i];

            }

            // Set the outputs
            this->GetCirrusPixelNumberOutput()->Set(l_CirrusPixelNumber);
            this->GetCloudPixelNumberOutput()->Set(l_CloudPixelNumber);
            this->GetValidPixelNumberOutput()->Set(l_ValidPixelNumber);

        }

    template<class TInputImage, class TInputMask>
        void
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::ThreadedGenerateData(const RegionType& outputRegionForThread,
                itk::ThreadIdType threadId)
        {
            // Get the input image
            typedef typename TInputImage::ConstPointer InputImageConstPointer;

            InputImageConstPointer inputPtr = this->GetInput();
            InputMaskPointer maskPtr = const_cast<TInputMask *>(this->GetEDGSubInput());

            itk::ImageRegionConstIteratorWithIndex < TInputImage > it(inputPtr, outputRegionForThread);
            itk::ImageRegionConstIteratorWithIndex < TInputMask > itEDG(maskPtr, outputRegionForThread);

            it.GoToBegin();
            itEDG.GoToBegin();


            // Pixel loop
            while (it.IsAtEnd() == false)
            {
                const PixelType value = it.Get();
                const MaskPixelType edg = itEDG.Get();

                RealType realValue = static_cast<RealType>(value);

                // Count the pixels that are different from no_data value
                if (edg == 0)
                {
                    // Count Cirrus pixels
                    if (realValue > m_AltitudeThreshold)
                    {
                        m_ThreadCirrus[threadId]++;
                    }
                    // Count cloud pixels
                    if (realValue > 0.)
                    {
                        m_ThreadCloud[threadId]++;
                    }
                    m_ThreadValid[threadId]++;
                }

                ++it;
                ++itEDG;
            }
        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask>
        void
        PersistentCountCirrusPixelGenerator<TInputImage, TInputMask>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            //Superclass::PrintSelf(os, indent);

            os << indent << "Number of cirrus pixels: " << this->GetCirrusPixelNumberOutput()->Get() << std::endl;
            os << indent << "Number of cloud pixels: " << this->GetCloudPixelNumberOutput()->Get() << std::endl;
            os << indent << "Number of valid pixels: " << this->GetValidPixelNumberOutput()->Get() << std::endl;

        }

}
// End namespace vns

#endif /* __vnsCountCirrusPixelGenerator_txx */
