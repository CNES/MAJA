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
 * VERSION : 4-7-1 : FA : LAIG-FA-MAC-1555-CNES : 7 janvier 2016 : PB mauvais cast sur InputImage           *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mai 2014 : Améliorations algorithmiques (Spec v2.1)		*
 * 							Correction boucle sur les threads dans méthode Synthetize						*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudAlreadyDetectedImageFilter_txx
#define __vnsCloudAlreadyDetectedImageFilter_txx

#include "vnsCloudAlreadyDetectedImageFilter.h"

namespace vns
{

    template<class TInputMask, class TInputLabel>
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::PersistentCloudAlreadyDetectedImageFilter() :
                m_InsideValue(0), m_ExcludedLabelValue(0), m_CloudFoundVectorMap()
        {
            this->SetNumberOfRequiredInputs(3);
            this->SetNumberOfIndexedInputs(3);
            this->SetNumberOfIndexedOutputs(2);

            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types
            typename MapBoolObjectType::Pointer outputCloudFound = static_cast<MapBoolObjectType*>(this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, outputCloudFound.GetPointer());

        }

    template<class TInputMask, class TInputLabel>
        itk::DataObject::Pointer
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::MakeOutput(unsigned int output)
        {
            // According to the output id, create the valid data
            switch (output)
                {
            case 0:
                {
                    // input image of cloud label
                    return static_cast<itk::DataObject*>(TInputLabel::New().GetPointer());
                }
            case 1:
                {
                    // map of boolean - the key is a label
                    return static_cast<itk::DataObject*>(MapBoolObjectType::New().GetPointer());
                }
            default:
                {
                    // might as well make an image
                    return static_cast<itk::DataObject*>(TInputLabel::New().GetPointer());
                }
                }
        }

    // Get the boolean cloud found
    template<class TInputMask, class TInputLabel>
        const typename PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::MapBoolObjectType *
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::GetCloudFoundMap() const
        {
            return static_cast<const MapBoolObjectType*>(this->itk::ProcessObject::GetOutput(1));
        }

    template<class TInputMask, class TInputLabel>
        void
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {

            // Grab the inputs
            const InputLabelPointerType l_IPCLDExtLabelPtr = const_cast<InputLabelType *>(this->GetIPCLDExtLabelInput());
            const InputMaskPointerType l_IPCLDReflPtr = const_cast<InputMaskType *>(this->GetIPCLDReflInput());
            const InputMaskPointerType l_IPCLDReflVarPtr = const_cast<InputMaskType *>(this->GetIPCLDReflVarInput());

            // Support progress methods/callbacks

            // Iterators
            itk::ImageRegionConstIterator<InputLabelType> l_IPCLDExtLabelIt(l_IPCLDExtLabelPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> l_IPCLDReflIt(l_IPCLDReflPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> l_IPCLDReflVarIt(l_IPCLDReflVarPtr, outputRegionForThread);

            // Init the iterators to begin
            l_IPCLDExtLabelIt.GoToBegin();
            l_IPCLDReflIt.GoToBegin();
            l_IPCLDReflVarIt.GoToBegin();

            // do the work
            while (l_IPCLDExtLabelIt.IsAtEnd() == false)
            {

                // Get the input pixel value
                InputLabelPixelType l_Label = l_IPCLDExtLabelIt.Get();
                InputMaskPixelType l_IPCLDReflValue = l_IPCLDReflIt.Get();
                InputMaskPixelType l_IPCLDReflVarValue = l_IPCLDReflVarIt.Get();

                if (l_Label != static_cast<InputLabelPixelType>(m_ExcludedLabelValue))
                {
                    if ((l_IPCLDReflValue == static_cast<InputMaskPixelType>(m_InsideValue))
                            || (l_IPCLDReflVarValue == static_cast<InputMaskPixelType>(m_InsideValue)))
                    {
                        m_CloudFoundVectorMap[threadId][l_Label] = true;
                    }

                }

                // Incremenst iterators
                ++l_IPCLDExtLabelIt;
                ++l_IPCLDReflIt;
                ++l_IPCLDReflVarIt;

            }
        }

    template<class TInputMask, class TInputLabel>
        void
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputMask, class TInputLabel>
        void
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::GenerateOutputInformation()
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

    template<class TInputMask, class TInputLabel>
        void
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::Synthetize(void)
        {
            //// Here we fuse all the found maps.

            const unsigned int l_NumberOfThreads = this->GetNumberOfThreads();

            std::vector<InputLabelPixelType> keyList(0, static_cast<InputLabelPixelType>(0));

            //// First step : store every keys found in the map
            // Loop over the number of threads
            for (unsigned int threadId = 0; threadId < l_NumberOfThreads; threadId++)
            {
                // Load the maps of the current threads
                BoolMapType curMapBool = m_CloudFoundVectorMap[threadId];

                typename BoolMapType::iterator iter = curMapBool.begin();
                while (iter != curMapBool.end())
                {
                    unsigned int loop = 0;
                    bool found = false;
                    unsigned int vectSize = keyList.size();
                    // Find every used key in the map list
                    // (except the first one : it is the final map intialization)
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
            //// Init the global maps to the first thread maps. For each current thread maps:
            //// If a key is already in the final map, just add the current thread value
            //// otherwise, it add a new key (value a copy from the current thread map).
            BoolMapType l_CloudFound;            // = m_CloudFoundVectorMap[0];
            const unsigned int vectSize = keyList.size();
            for (unsigned int j = 0; j < vectSize; j++)
            {
                InputLabelPixelType myKey = keyList.at(j);
                // Map initialization
                l_CloudFound[myKey] = false;

                unsigned int threadId(0);

                while ((threadId < l_NumberOfThreads) && (l_CloudFound[myKey] == false))
                {
                    // If the label doesn't exist yet
                    if (m_CloudFoundVectorMap[threadId].find(myKey) != m_CloudFoundVectorMap[threadId].end())
                    {
                        if (m_CloudFoundVectorMap[threadId][myKey] == true)
                        {
                            l_CloudFound[myKey] = true;
                        }
                    }
                    threadId++;
                }
            }
            // Grab the outputs
            typename MapBoolObjectType::Pointer outputCloudFound = static_cast<MapBoolObjectType*>(this->itk::ProcessObject::GetOutput(1));

            // The values to the outputs
            outputCloudFound->Set(l_CloudFound);

        }

    template<class TInputMask, class TInputLabel>
        void
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::Reset(void)
        {
            InputLabelPointerType l_IPCLDExtLabelPtr = const_cast<InputLabelType *>(this->GetIPCLDExtLabelInput());
            InputMaskPointerType l_IPCLDReflPtr = const_cast<InputMaskType *>(this->GetIPCLDReflInput());
            InputMaskPointerType l_IPCLDReflVarPtr = const_cast<InputMaskType *>(this->GetIPCLDReflVarInput());

            // update the input image information
            l_IPCLDExtLabelPtr->UpdateOutputInformation();
            l_IPCLDReflPtr->UpdateOutputInformation();
            l_IPCLDReflVarPtr->UpdateOutputInformation();

            // Load the number of thread
            unsigned int numberOfThreads = this->GetNumberOfThreads();

            BoolMapType boolMap;

            // Initialize the output values
            m_CloudFoundVectorMap = BoolVectorMapType(numberOfThreads, boolMap);

        }

    /** PrintSelf method */
    template<class TInputMask, class TInputLabel>
        void
        PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_InsideValue:    " << m_InsideValue << std::endl;
            os << "m_ExcludedLabelValue:    " << m_ExcludedLabelValue << std::endl;

            // Compute the number of threads
            unsigned int l_NumberOfThreads = this->GetNumberOfThreads();

            // Display if a cloud has been already detected with reflectance threshold
            // The result is given by thread
            os << "====== Sum Maps found ======" << std::endl;
            for (unsigned int threadId = 0; threadId < l_NumberOfThreads; threadId++)
            {
                BoolMapType curMap = m_CloudFoundVectorMap[threadId];
                os << "=== Sum Map for thread #" << threadId << " ===" << std::endl;
                // map iteration
                typename BoolMapType::iterator iter = curMap.begin();
                while (iter != curMap.end())
                {
                    // key = label value = true/false
                    os << indent << (*iter).first << "  " << (*iter).second << std::endl;
                    iter++;
                }
            }
            os << std::endl;
        }

    /** PrintSelf method */
    template<class TInputMask, class TInputLabel>
        void
        CloudAlreadyDetectedImageFilter<TInputMask, TInputLabel>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            //Superclass::PrintSelf(os, indent);
            BoolMapType foundMap = this->GetCloudFound();

            // Display the sum of value per label
            os << "====== Sum Map ======" << std::endl;
            typename BoolMapType::const_iterator iterFound = foundMap.begin();
            // Loop the map
            while (iterFound != foundMap.end())
            {
                os << indent << (*iterFound).first << "  " << (*iterFound).second << std::endl;
                iterFound++;
            }
            os << std::endl;
        }

} //end of namespace

#endif /* __vnsCloudAlreadyDetectedImageFilter_txx */
