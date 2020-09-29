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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 juil 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsNoDataShadowMapUpdating.txx 2406 2011-01-19 15:38:23Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsNoDataShadowMapUpdating_txx
#define __vnsNoDataShadowMapUpdating_txx

#include "vnsNoDataShadowImageFilter.h"

#include "vnsMath.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace vns
{

    template<class TInputImage>
        PersistentNoDataShadowImageFilter<TInputImage>::PersistentNoDataShadowImageFilter() :
                m_ShadowMap(), m_LabelKOVList(), m_LabelVList(), m_NoDataValue(0)
        {
            this->SetNumberOfRequiredInputs(1);
            this->SetNumberOfIndexedInputs(1);
            this->SetNumberOfIndexedOutputs(3);

            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types
            typename LabelListObjectType::Pointer outputList = static_cast<LabelListObjectType*>(this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, outputList.GetPointer());

            typename ULObjectType::Pointer outputLabelNumber = static_cast<ULObjectType*>(this->MakeOutput(2).GetPointer());
            this->itk::ProcessObject::SetNthOutput(2, outputLabelNumber.GetPointer());
        }

    template<class TInputImage>
        itk::DataObject::Pointer
        PersistentNoDataShadowImageFilter<TInputImage>::MakeOutput(unsigned int output)
        {
            // According to the output id, create the valid data
            switch (output)
                {
            case 0:
                {
                    // Input image =  difference image between the current image and the composite one
                    return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                }
            case 1:
                {
                    // list of labels
                    return static_cast<itk::DataObject*>(LabelListObjectType::New().GetPointer());
                }
            case 2:
                {
                    // Number of used labels
                    return static_cast<itk::DataObject*>(ULObjectType::New().GetPointer());
                }
            default:
                {
                    // might as well make an image
                    return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                }
                }
        }

    template<class TInputImage>
        void
        PersistentNoDataShadowImageFilter<TInputImage>::ThreadedGenerateData(const InputImageRegionType& /*outputRegionForThread*/,
                itk::ThreadIdType threadId)
        {
            LinearIndex linearId;
            LabelType label;
            IndexType pixCoord;

            // Get the inputs
            InputImagePointerType diffPtr = const_cast<InputImageType *>(this->GetDiffImage());

            const unsigned long nc = diffPtr->GetLargestPossibleRegion().GetSize(0);

            // Load the shadow  map
            typename LabelMapType::iterator iter = m_ShadowMap.begin();

            // Loop over the shadow map
            while (iter != m_ShadowMap.end())
            {
                // Get the associated label
                label = (*iter).second;

                // List of label used to count the number of labels
                const bool isFound = FindLabel(m_LabelVList[threadId], label);
                if (isFound == false)
                {
                    m_LabelVList[threadId].push_back(label);
                }

                // Get the linear index of the current pixel
                linearId = (*iter).first;

                // Get the pixel index (l,p)
                const unsigned long line = static_cast<unsigned int>(linearId / nc);
                const unsigned long col = linearId - (line * nc);

                // Set the index
                pixCoord[0] = col;
                pixCoord[1] = line;

                // Get the value of the difference between the current image and the composite
                const InputImagePixelType diffValue = diffPtr->GetPixel(pixCoord);

                // If one pixel of the shadow label is not NoData in the diff image (ie image composite)
                // The shadow label is invalidated
                // This label is added to the list of invalid pixels for the current thread
                if (vnsDifferentDoubleMacro(diffValue, m_NoDataValue) == true)
                {
                    // Test if the label has already been added to the list
                    const bool found = FindLabel(m_LabelKOVList[threadId], label);
                    if (found == false)
                    {
                        m_LabelKOVList[threadId].push_back(label);
                    }
                }
                iter++;
            }
        }

    template<class TInputImage>
        void
        PersistentNoDataShadowImageFilter<TInputImage>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputImage>
        void
        PersistentNoDataShadowImageFilter<TInputImage>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();
            // This is the part that allows to do threading.
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

    template<class TInputImage>
        void
        PersistentNoDataShadowImageFilter<TInputImage>::Synthetize(void)
        {
            // Here we fuse all the found labels
            unsigned int numberOfThreads = this->GetNumberOfThreads();
            LabelListType labelKOList = m_LabelKOVList[0];
            LabelListType labelList = m_LabelVList[0];
            unsigned long numberOfLabels(0);

            // Get all the invalid labels of all the threads
            // Loop over the number of threads
            for (unsigned int threadId = 1; threadId < numberOfThreads; threadId++)
            {
                // Load the list of the current threads (invalid labels)
                LabelListType curLabelKOList = m_LabelKOVList[threadId];

                // Add the labels to the output list of invalid labels
                typename LabelListType::iterator iterKO = curLabelKOList.begin();
                while (iterKO != curLabelKOList.end())
                {
                    labelKOList.push_back(*iterKO);
                    iterKO++;
                }

                // Load the list of the current threads (all the labels)
                LabelListType curLabelList = m_LabelVList[threadId];

                // Add the labels to the output list of labels
                typename LabelListType::iterator iter = curLabelList.begin();
                while (iter != curLabelList.end())
                {
                    labelList.push_back(*iter);
                    iter++;
                }
            }

            // Remove the duplicated labels in the output list of invalid labels
            labelKOList.sort();
            labelKOList.unique();

            labelList.sort();
            labelList.unique();
            numberOfLabels = labelList.size();

            if (labelKOList.size() > numberOfLabels)
            {
                vnsExceptionBusinessMacro("Error : the number of invalid labels is greater than the number of labels !");
            }

            // Grab the outputs
            typename LabelListObjectType::Pointer outputList = static_cast<LabelListObjectType*>(this->itk::ProcessObject::GetOutput(1));
            typename ULObjectType::Pointer outputLabelNumber = static_cast<ULObjectType*>(this->itk::ProcessObject::GetOutput(2));

            // The values to the output
            outputList->Set(labelKOList);
            outputLabelNumber->Set(numberOfLabels);

        }

    template<class TInputImage>
        void
        PersistentNoDataShadowImageFilter<TInputImage>::Reset(void)
        {
            InputImagePointerType diffPtr = const_cast<InputImageType *>(this->GetDiffImage());

            // update the input image information
            diffPtr->UpdateOutputInformation();
            // Load the number of threads
            unsigned int numberOfThreads = this->GetNumberOfThreads();

            LabelListType labelKOList;
            LabelListType labelList;

            // Initialize the output values
            m_LabelKOVList = LabelVectorListType(numberOfThreads, labelKOList);
            m_LabelVList = LabelVectorListType(numberOfThreads, labelList);

        }

    template<class TInputImage>
        bool
        PersistentNoDataShadowImageFilter<TInputImage>::FindLabel(LabelListType & pLabelList, LabelType & pLabel)
        {
            typename LabelListType::iterator it = pLabelList.begin();
            bool found(false);

            while ((found == false) && (it != pLabelList.end()))
            {
                if (*it == pLabel)
                {
                    found = true;
                }
                ++it;
            }

            return found;
        }

    /** PrintSelf method */
    template<class TInputImage>
        void
        PersistentNoDataShadowImageFilter<TInputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            // Compute the number of threads
            unsigned int lNumberOfThreads = this->GetNumberOfThreads();

            // Display the list of invalid labels and par thread
            os << "====== Label KO found by thread ======" << std::endl;
            for (unsigned int threadId = 1; threadId < lNumberOfThreads; threadId++)
            {
                LabelListType curList = m_LabelKOVList[threadId];
                os << "=== Label KO for thread #" << threadId << " ===" << std::endl;
                typename LabelListType::iterator iter = curList.begin();
                // Loop under map
                while (iter != curList.end())
                {
                    os << indent << (*iter) << std::endl;
                    iter++;
                }
            }

            os << std::endl;
        }

    /** PrintSelf method */
    template<class TInputImage>
        void
        NoDataShadowImageFilter<TInputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            //Superclass::PrintSelf(os, indent);
            unsigned long number = this->GetNumberOfLabelsOutput();
            os << "Number of labels : " << number << std::endl;

            LabelListType labelList = this->GetLabelKOListOutput();

            // Display the list of invalid labels
            os << "====== Label KO ======" << std::endl;
            typename LabelListType::const_iterator iter = labelList.begin();
            // Loop the map
            while (iter != labelList.end())
            {
                os << indent << (*iter) << std::endl;
                iter++;
            }

            os << std::endl;
        }

} //end of namespace

#endif /*__vnsValidShadowMaskImageFilter_txx*/
