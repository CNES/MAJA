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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsValueCountImageFilter_txx
#define __vnsValueCountImageFilter_txx

#include "vnsValueCountImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "vnsMath.h"


namespace vns
{

    template<class TInputImage, class TOutputImage>
        PersistentValueCountImageFilter<TInputImage, TOutputImage>::PersistentValueCountImageFilter() :
            m_PixelValue(0), m_CountValueByThread()
        {

            this->SetNumberOfRequiredInputs(1);
            this->SetNumberOfIndexedInputs(1);
            this->SetNumberOfIndexedOutputs(2);

            m_CountValueByThread.clear();

            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types
            typename CountValueObjectType::Pointer outputULong =
                    static_cast<CountValueObjectType*> (this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, outputULong.GetPointer());

        }

    template<class TInputImage, class TOutputImage>
        itk::DataObject::Pointer
        PersistentValueCountImageFilter<TInputImage, TOutputImage>::MakeOutput(
                unsigned int output)
        {

            // According to the output id, create the valid data
            switch (output)
                {
            case 0:
                {
                    return static_cast<itk::DataObject*> (TInputImage::New().GetPointer());
                }
            case 1:
                {
                    return static_cast<itk::DataObject*> (CountValueObjectType::New().GetPointer());
                }
            default:
                {
                    // might as well make an image
                    return static_cast<itk::DataObject*> (TInputImage::New().GetPointer());
                }
                }

        }

    template<class TInputImage, class TOutputImage>
        void
        PersistentValueCountImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {

            // Grab the inputs
            // In VENµS, the input image is the mask of No_Data (L2inNDT)
            InputImagePointerType l_ImagePtr = const_cast<InputImageType *> (this->GetImageInput());

            // Support progress methods/callbacks

            // Iterators
            itk::ImageRegionConstIteratorWithIndex<InputImageType> l_ImageIt(l_ImagePtr,
                    outputRegionForThread);

            l_ImageIt.GoToBegin();

            // Pixel Loop
            while (l_ImageIt.IsAtEnd() == false)
            {

                InputImagePixelType l_Pix = l_ImageIt.Get();

                // If the pixel is equal to the input pixel value
                if (vnsEqualsDoubleMacro(static_cast<double>(l_Pix),static_cast<double>(m_PixelValue)) == true)
                {
                    m_CountValueByThread[threadId]++;
                }

                ++l_ImageIt;
            }
        }

    template<class TInputImage, class TOutputImage>
        void
        PersistentValueCountImageFilter<TInputImage, TOutputImage>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputImage, class TOutputImage>
        void
        PersistentValueCountImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
        {

            Superclass::GenerateOutputInformation();
            // This is the part that allows to do threading.
            if (this->GetInput())
            {
                this->GetOutput()->CopyInformation(this->GetInput());
                this->GetOutput()->SetLargestPossibleRegion(
                        this->GetInput()->GetLargestPossibleRegion());

                if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
                {
                    this->GetOutput()->SetRequestedRegion(
                            this->GetOutput()->GetLargestPossibleRegion());
                }
            }
        }

    template<class TInputImage, class TOutputImage>
        void
        PersistentValueCountImageFilter<TInputImage, TOutputImage>::Synthetize(void)
        {

            unsigned int lNumberOfThreads = this->GetNumberOfThreads();

            CountType l_CountValue(0);

            // Loop over the number of threads
            for (unsigned int threadId = 0; threadId < lNumberOfThreads; threadId++)
            {
                l_CountValue = l_CountValue + m_CountValueByThread[threadId];
            }

            // Grab the output
            typename CountValueObjectType::Pointer outputCount =
                    static_cast<CountValueObjectType*> (this->itk::ProcessObject::GetOutput(1));


            // The values to the outputs
            outputCount->Set(l_CountValue);


        }

    template<class TInputImage, class TOutputImage>
        void
        PersistentValueCountImageFilter<TInputImage, TOutputImage>::Reset(void)
        {

            InputImagePointerType l_ImagePtr = const_cast<InputImageType *> (this->GetImageInput());

            // update the input image information
            l_ImagePtr->UpdateOutputInformation();

            // Load the number of thread
            unsigned int numberOfThreads = this->GetNumberOfThreads();

            CountType l_Count(0);

            // Initialize the output values
            m_CountValueByThread = VectorCountType(numberOfThreads, l_Count);

        }

    /** PrintSelf method */
    template<class TInputImage, class TOutputImage>
        void
        PersistentValueCountImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            // Compute the number of threads
            unsigned int lNumberOfThreads = this->GetNumberOfThreads();

            // Display the sum of value per label and par thread
            os << "====== Number of pixel by thread ======" << std::endl;
            for (unsigned int threadId = 1; threadId < lNumberOfThreads; threadId++)
            {
                CountType l_Count = m_CountValueByThread[threadId];
                os << "=== Count for thread #" << threadId << " ===" << std::endl;
                os << indent << l_Count << std::endl;

            }

            os << std::endl;
        }

    /** PrintSelf method */
    template<class TInputImage>
        void
        ValueCountImageFilter<TInputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            //Superclass::PrintSelf(os, indent);
            CountType l_Count = this->GetValueCount();

            os << indent << "Number of pixel equal to a given value : "<< l_Count << std::endl;
            os << std::endl;
        }

} //end of namespace

#endif /*__vnsValueCountImageFilter_txx*/

