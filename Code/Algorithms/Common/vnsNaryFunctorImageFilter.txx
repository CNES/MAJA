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
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction données des CAMS             *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 6 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __itkNaryFunctorImageFilter_txx
#define __itkNaryFunctorImageFilter_txx

#include <vnsNaryFunctorImageFilter.h>
#include "itkImageRegionIterator.h"

namespace vns
{

    /**
     * Constructor
     */
    template<class TInputImage, class TOutputImage, class TFunction>
        NaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>::NaryFunctorImageFilter()
        {
            // This number will be incremented each time an image
            // is added over the two minimum required
            this->SetNumberOfRequiredInputs(2);
            this->InPlaceOff();
        }

    /**
     * ThreadedGenerateData Performs the pixel-wise addition
     */
    template<class TInputImage, class TOutputImage, class TFunction>
        void
        NaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>::ThreadedGenerateData(
                const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {

            // Get the number of inputs images
            const unsigned int numberOfInputImages = static_cast<unsigned int>(this->GetNumberOfInputs());

            typedef itk::ImageRegionConstIterator<TInputImage> ImageRegionConstIteratorType;
            std::vector<ImageRegionConstIteratorType *> inputItrVector;
            inputItrVector.reserve(numberOfInputImages);

            // support progress methods/callbacks.
            // count the number of inputs that are non-null
            for (unsigned int i = 0; i < numberOfInputImages; ++i)
            {
                // Get pointer
                InputImagePointer inputPtr = dynamic_cast<TInputImage*>(itk::ProcessObject::GetInput(i));

                if (inputPtr)
                {
                    inputItrVector.push_back(new ImageRegionConstIteratorType(inputPtr, outputRegionForThread));
                }
            }

            const unsigned int numberOfValidInputImages = inputItrVector.size();

            // Check validity of number of inputs
            if (numberOfValidInputImages == 0)
            {
                //No valid regions in the thread
                //(and no region iterators to delete)
                return;
            }

            NaryArrayType naryInputArray(numberOfValidInputImages);

            OutputImagePointer outputPtr = this->GetOutput(0);
            itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

            typename std::vector<ImageRegionConstIteratorType *>::iterator regionIterators;
            const typename std::vector<ImageRegionConstIteratorType *>::const_iterator regionItEnd = inputItrVector.end();

            typename NaryArrayType::iterator arrayIt;

            // Pixel loop
            while (!outputIt.IsAtEnd())
            {
                arrayIt = naryInputArray.begin();
                regionIterators = inputItrVector.begin();
                // Loops on region
                while (regionIterators != regionItEnd)
                {
                    *arrayIt++ = (*regionIterators)->Get();
                    ++(*(*regionIterators));
                    ++regionIterators;
                }

                // Set the output pixel
                outputIt.Set(m_Functor(naryInputArray));
                ++outputIt;
            }

            // Free memory
            regionIterators = inputItrVector.begin();
            while (regionIterators != regionItEnd)
            {
                delete (*regionIterators++);
            }
        }

} // end namespace vns

#endif
