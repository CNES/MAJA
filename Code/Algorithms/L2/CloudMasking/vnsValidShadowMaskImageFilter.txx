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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mars 2014 : Améliorations algorithmiques                *
 * 											Calcul de la somme de l'assombrissement relatif et non absolu   *
 * VERSION : 1-0-0-3 : DM : 357 : 19 sept. 2011 : Calcule la somme des reflectances par label					*
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juin 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsValidShadowMaskImageFilter_txx
#define __vnsValidShadowMaskImageFilter_txx

#include "vnsValidShadowMaskImageFilter.h"

#include "vnsMath.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace vns
{

    template<class TInputDiff, class TInputImage, class TInputLabel>
        PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::PersistentValidShadowMaskImageFilter() :
                m_SumList(), m_NumberList(), m_ReflSum(), m_ExcludedValue(0), m_ExcludedLabelValue(0), m_ShadowBand(0)
        {
            this->SetNumberOfRequiredInputs(3);
            this->SetNumberOfIndexedInputs(3);
            this->SetNumberOfIndexedOutputs(3);

            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types
            typename MapDoubleObjectType::Pointer outputDouble = static_cast<MapDoubleObjectType*>(this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, outputDouble.GetPointer());

            typename MapUIObjectType::Pointer outputUC = static_cast<MapUIObjectType*>(this->MakeOutput(2).GetPointer());
            this->itk::ProcessObject::SetNthOutput(2, outputUC.GetPointer());

            typename MapDoubleObjectType::Pointer outputRefl = static_cast<MapDoubleObjectType*>(this->MakeOutput(3).GetPointer());
            this->itk::ProcessObject::SetNthOutput(3, outputRefl.GetPointer());
        }

    template<class TInputDiff, class TInputImage, class TInputLabel>
        itk::DataObject::Pointer
        PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::MakeOutput(unsigned int output)
        {
            // According to the output id, create the valid data
            switch (output)
                {
            case 0:
                {
                    return static_cast<itk::DataObject*>(TInputLabel::New().GetPointer());
                }
            case 1:
                {
                    return static_cast<itk::DataObject*>(MapDoubleObjectType::New().GetPointer());
                }
            case 2:
                {
                    return static_cast<itk::DataObject*>(MapUIObjectType::New().GetPointer());
                }
            case 3:
                {
                    return static_cast<itk::DataObject*>(MapDoubleObjectType::New().GetPointer());
                }
            default:
                {
                    // might as well make an image
                    return static_cast<itk::DataObject*>(TInputLabel::New().GetPointer());
                }
                }
        }

    template<class TInputDiff, class TInputImage, class TInputLabel>
        void
        PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {
            // Grab the inputs
            InputLabelPointerType shadLabelPtr = const_cast<InputLabelType *>(this->GetIPCLDShadLabel());
            InputDiffPointerType diffPtr = const_cast<InputDiffImageType *>(this->GetIPDiffInput());
            ReflectanceImagePointer l_ReflPtr = const_cast<ReflectanceImageType *>(this->GetReflInput());

            // Iterators
            /*itk::ImageRegionConstIteratorWithIndex<InputLabelType> itLabel(maskPtr,
             outputRegionForThread);*/
            itk::ImageRegionConstIteratorWithIndex<InputLabelType> itShadLabel(shadLabelPtr, outputRegionForThread);
            itk::ImageRegionConstIteratorWithIndex<InputDiffImageType> itDiff(diffPtr, outputRegionForThread);
            itk::ImageRegionConstIteratorWithIndex<ReflectanceImageType> itRefl(l_ReflPtr, outputRegionForThread);

            /*itLabel.GoToBegin();*/
            itShadLabel.GoToBegin();
            itDiff.GoToBegin();
            itRefl.GoToBegin();

            // do the work
            while ((itDiff.IsAtEnd() == false)) /*(itLabel.IsAtEnd() == false) && */
            {

                /*                InputLabelPixelType myKey = itLabel.Get();*/
                const InputLabelPixelType shadLabelValue = itShadLabel.Get();

                const double myVal = static_cast<double>(itDiff.Get());
                const double lRefl = static_cast<double>(itRefl.Get()[m_ShadowBand]);

                // If the diff pixel is valid and the shadow is not 0 (background)
                if ((vnsDifferentDoubleMacro(myVal,static_cast<double> (m_ExcludedValue)) == true)
                        && (shadLabelValue != m_ExcludedLabelValue) && (vnsDifferentDoubleMacro(lRefl, 0) == true))
                {
                    // If the shadow label doesn't exist yet
                    if (m_SumList[threadId].find(shadLabelValue) == m_SumList[threadId].end())
                    {
                        // Use to calculate the shadow criterion = mean of the relative darkening
                        m_SumList[threadId][shadLabelValue] = myVal / lRefl;
                        m_ReflSum[threadId][shadLabelValue] = lRefl;
                        m_NumberList[threadId][shadLabelValue] = 1;

                    }
                    // If the shadow label already exist in the map, just add values
                    else
                    {
                        m_SumList[threadId][shadLabelValue] = m_SumList[threadId][shadLabelValue] + (myVal / lRefl);
                        m_ReflSum[threadId][shadLabelValue] = m_ReflSum[threadId][shadLabelValue] + lRefl;
                        m_NumberList[threadId][shadLabelValue] = m_NumberList[threadId][shadLabelValue] + 1;
                    }
                }

                /*++itLabel;*/
                ++itShadLabel;
                ++itDiff;
                ++itRefl;
            }
        }

    template<class TInputDiff, class TInputImage, class TInputLabel>
        void
        PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputDiff, class TInputImage, class TInputLabel>
        void
        PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::GenerateOutputInformation()
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

    template<class TInputDiff, class TInputImage, class TInputLabel>
        void
        PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::Synthetize(void)
        {
            //// Here we fuse all the found maps.

            unsigned int lNumberOfThreads = this->GetNumberOfThreads();
            DoubleMapType lSum = m_SumList[0];
            UIMapType lValidPix = m_NumberList[0];
            DoubleMapType lReflSum = m_ReflSum[0];

            std::vector<InputLabelPixelType> keyList(0, static_cast<InputLabelPixelType>(0));

            //// First step : store every keys found in the map
            // Loop over the number of threads
            for (unsigned int threadId = 1; threadId < lNumberOfThreads; threadId++)
            {
                // Load the maps of the current threads
                DoubleMapType curMapDouble = m_SumList[threadId];
                UIMapType curMapUI = m_NumberList[threadId];

                typename DoubleMapType::iterator iter = curMapDouble.begin();
                while (iter != curMapDouble.end())
                {
                    unsigned int loop = 0;
                    bool found = false;
                    unsigned int vectSize = keyList.size();
                    // Find every used key in the map list
                    // (except the first one : it is the final map initialization)
                    while ((loop < vectSize) && (found == false))
                    {
                        if (keyList[loop] == (*iter).first)
                        {
                            found = true;
                        }
                        loop = loop + 1;
                    }
                    if (found != true)
                    {
                        keyList.push_back((*iter).first);
                    }

                    iter++;
                }
            }

            //// Second step : once the keys are found, fill the final global maps.
            //// Initialize the global maps to the first thread maps. For each current thread maps:
            //// If a key is already in the final map, just add the current thread value
            //// otherwise, it add a new key (value a copy from the current thread map).
            unsigned int vectSize = keyList.size();
            for (unsigned int j = 0; j < vectSize; j++)
            {
                InputLabelPixelType myKey = keyList.at(j);
                for (unsigned int threadId = 1; threadId < lNumberOfThreads; threadId++)
                {

                    // If the label doesn't exist yet
                    if (m_SumList[threadId].find(myKey) != m_SumList[threadId].end())
                    {
                        if (lSum.find(myKey) == lSum.end())
                        {
                            lSum[myKey] = m_SumList[threadId][myKey];
                            lValidPix[myKey] = m_NumberList[threadId][myKey];
                            lReflSum[myKey] = m_ReflSum[threadId][myKey];
                        }
                        else
                        {
                            lSum[myKey] = m_SumList[threadId][myKey] + lSum[myKey];
                            lValidPix[myKey] = m_NumberList[threadId][myKey] + lValidPix[myKey];
                            lReflSum[myKey] = m_ReflSum[threadId][myKey] + lReflSum[myKey];
                        }
                    }
                }
            }

            // Grab the outputs
            typename MapDoubleObjectType::Pointer outputSum = static_cast<MapDoubleObjectType*>(this->itk::ProcessObject::GetOutput(1));

            typename MapUIObjectType::Pointer outputValid = static_cast<MapUIObjectType*>(this->itk::ProcessObject::GetOutput(2));

            typename MapDoubleObjectType::Pointer outputReflSum = static_cast<MapDoubleObjectType*>(this->itk::ProcessObject::GetOutput(3));

            // The values to the outputs
            outputSum->Set(lSum);
            outputValid->Set(lValidPix);
            outputReflSum->Set(lReflSum);

        }

    template<class TInputDiff, class TInputImage, class TInputLabel>
        void
        PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::Reset(void)
        {
            InputDiffPointerType diffPtr = const_cast<InputDiffImageType *>(this->GetIPDiffInput());
            ReflectanceImagePointer reflPtr = const_cast<ReflectanceImageType *>(this->GetReflInput());

            // update the input image information
            diffPtr->UpdateOutputInformation();
            reflPtr->UpdateOutputInformation();

            // Load the number of thread
            unsigned int numberOfThreads = this->GetNumberOfThreads();

            DoubleMapType doubleMap;
            UIMapType uiMap;
            DoubleMapType doubleMap2;

            // Initialize the output values
            m_SumList = DoubleVectorMapType(numberOfThreads, doubleMap);
            m_NumberList = UIVectorMapType(numberOfThreads, uiMap);
            m_ReflSum = DoubleVectorMapType(numberOfThreads, doubleMap2);
        }

    /** PrintSelf method */
    template<class TInputDiff, class TInputImage, class TInputLabel>
        void
        PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            // Compute the number of threads
            unsigned int lNumberOfThreads = this->GetNumberOfThreads();

            // Display the sum of value per label and par thread
            os << "====== Sum Maps found ======" << std::endl;
            for (unsigned int threadId = 1; threadId < lNumberOfThreads; threadId++)
            {
                DoubleMapType curMap = m_SumList[threadId];
                os << "=== Sum Map for thread #" << threadId << " ===" << std::endl;
                typename DoubleMapType::iterator iter = curMap.begin();
                // Loop under map
                while (iter != curMap.end())
                {
                    os << indent << (*iter).first << "  " << (*iter).second << std::endl;
                    iter++;
                }
            }

            // Display the number of valid pixel per label and par thread
            os << "====== Valid Pixel Maps found ======" << std::endl;
            for (unsigned int threadId = 1; threadId < lNumberOfThreads; threadId++)
            {
                UIMapType curMap = m_NumberList[threadId];
                os << "=== Valid pixel Map for thread #" << threadId << " ===" << std::endl;
                typename UIMapType::iterator iter = curMap.begin();
                // Loop under map
                while (iter != curMap.end())
                {
                    os << indent << (*iter).first << "  " << (*iter).second << std::endl;
                    iter++;
                }

            }

            // Display the sum of reflectance per label and par thread
            os << "====== Reflectance Sum Maps found ======" << std::endl;
            for (unsigned int threadId = 1; threadId < lNumberOfThreads; threadId++)
            {
                DoubleMapType curMap = m_ReflSum[threadId];
                os << "=== Reflectance Sum Map for thread #" << threadId << " ===" << std::endl;
                typename DoubleMapType::iterator iter = curMap.begin();
                // Loop under map
                while (iter != curMap.end())
                {
                    os << indent << (*iter).first << "  " << (*iter).second << std::endl;
                    iter++;
                }
            }

            os << std::endl;
        }

    /** PrintSelf method */
    template<class TInputDiff, class TInputImage, class TInputLabel>
        void
        ValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            //Superclass::PrintSelf(os, indent);
            DoubleMapType sumMap = this->GetSumOfPixels();
            UIMapType validMap = this->GetNumberOfValidPixels();
            DoubleMapType sumReflMap = this->GetReflSum();

            // Display the sum of value per label
            os << "====== Sum Map ======" << std::endl;
            typename DoubleMapType::const_iterator iterSum = sumMap.begin();
            // Loop the map
            while (iterSum != sumMap.end())
            {
                os << indent << (*iterSum).first << "  " << (*iterSum).second << std::endl;
                iterSum++;
            }

            // Display the number of valid pixel per label
            os << "====== Valid Pixel Map ======" << std::endl;
            typename UIMapType::iterator iterVal = validMap.begin();
            // Loop the map
            while (iterVal != validMap.end())
            {
                os << indent << (*iterVal).first << "  " << (*iterVal).second << std::endl;
                iterVal++;
            }

            // Display the number of valid pixel per label
            os << "====== Reflectance Sum Map ======" << std::endl;
            typename DoubleMapType::const_iterator iterReflSum = sumReflMap.begin();
            // Loop the map
            while (iterReflSum != sumReflMap.end())
            {
                os << indent << (*iterReflSum).first << "  " << (*iterReflSum).second << std::endl;
                iterReflSum++;
            }

            os << std::endl;
        }

} //end of namespace

#endif /*__vnsValidShadowMaskImageFilter_txx*/
