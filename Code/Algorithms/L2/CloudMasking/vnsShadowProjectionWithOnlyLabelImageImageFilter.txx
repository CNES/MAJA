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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 8 juillet 2016 : Audit code - suppresion de macros ID3    *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowProjectionWithOnlyLabelImageImageFilter_txx
#define __vnsShadowProjectionWithOnlyLabelImageImageFilter_txx

#include "vnsShadowProjectionWithOnlyLabelImageImageFilter.h"
#include "vnsMacro.h"
#include "vnsMath.h"
#include <iomanip>
#include "vnsDEMDataManager.h"

namespace vns
{

    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::PersistentShadowProjectionWithOnlyLabelImageImageFilter() :
                m_SOLHRef(0.), m_L2CoarseResolution(1), m_InvVIEHRef(0.)
        {
            this->SetNumberOfRequiredInputs(4);
            this->SetNumberOfIndexedInputs(4);
            this->SetNumberOfIndexedOutputs(1);

            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types
            typename MapLabelObjectType::Pointer outputShadows = static_cast<MapLabelObjectType*>(this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, outputShadows.GetPointer());

            // Init L1 image spacing
            m_I3D = NULL;
        }

    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        itk::DataObject::Pointer
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::MakeOutput(
                unsigned int output)
        {
            // According to the output id, create the valid data
            switch (output)
                {
            case 0:
                {
                    return static_cast<itk::DataObject*>(TLabelImage::New().GetPointer());
                }
            case 1:
                {
                    return static_cast<itk::DataObject*>(MapLabelObjectType::New().GetPointer());
                }
            default:
                {
                    // might as well make an image
                    return static_cast<itk::DataObject*>(TLabelImage::New().GetPointer());
                }
                }
        }

    // Get the shadow labels
    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        const typename PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::MapLabelObjectType *
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::GetShadowMap() const
        {
            return static_cast<const MapLabelObjectType*>(this->itk::ProcessObject::GetOutput(1));
        }

    //================================== ThreadedGenerateData ====================================/
    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        void
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::ThreadedGenerateData(
                const LabelImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {
            // Initializations
            if ( vnsIsNotZeroDoubleMacro(m_DeltaH) == true)
            {//            CLAImageConstPointer l_IPCLAPtr = const_cast<CLAImageType *>(this->GetIPCLAInput());

                double l_DHnuage(0.);
                PointType l_GroundPoint;
                PointType l_IPVIEGroundPoint;
                PointType l_IPSOLGroundPoint;
                PointType l_NadirPoint;
                PointType l_ProjPoint;
                IndexType l_ProjIndex;

                LinearIndex l_ProjLinearIndex;

                std::vector<double> l_NadirVector(3);
                std::vector<double> l_VIEVector(3);
                std::vector<double> l_ProjVector(3);

                // Get pointers to input images
                LabelImageConstPointer l_L2CLDSubLabelPtr = const_cast<LabelImageType *>(this->GetL2CLDSubLabelInput());
                DTMImageConstPointer l_DTMPtr = const_cast<DTMImageType *>(this->GetDTMInput());
                GRIDImageConstPointer l_IPSOLPtr = const_cast<GRIDImageType *>(this->GetIPSOLInput());
                GRIDImageConstPointer l_IPVIEPtr = const_cast<GRIDImageType *>(this->GetIPVIEInput());

                const unsigned long l_nc = l_L2CLDSubLabelPtr->GetLargestPossibleRegion().GetSize(0);

                // Define the input image iterators
                itk::ImageRegionConstIterator<LabelImageType> l_L2CLDSubLabelIt(l_L2CLDSubLabelPtr, outputRegionForThread);
                itk::ImageRegionConstIterator<DTMImageType> l_DTMIt(l_DTMPtr, outputRegionForThread);
                itk::ImageRegionConstIterator<GRIDImageType> l_IPSOLIt(l_IPSOLPtr, outputRegionForThread);
                itk::ImageRegionConstIterator<GRIDImageType> l_IPVIEIt(l_IPVIEPtr, outputRegionForThread);

                l_L2CLDSubLabelIt.GoToBegin();
                l_DTMIt.GoToBegin();
                l_IPSOLIt.GoToBegin();
                l_IPVIEIt.GoToBegin();
                const LabelImagePixelType l_L2CLDSubLabelValueZero = static_cast<LabelImagePixelType>(0);

                // Pixel loop
                while (l_L2CLDSubLabelIt.IsAtEnd() == false)
                {
                    const LabelImagePixelType l_L2CLDSubLabelValue = l_L2CLDSubLabelIt.Get();

                    // Process only cloud pixels for which an altitude has been determined
                    if ((l_L2CLDSubLabelValue != l_L2CLDSubLabelValueZero)) // && (vnsDifferentDoubleMacro(l_IPCLAIt.Get(), 0.) == true))
                    {

                        // Determination of cloud mask altitude with regard to DTM
                        l_DHnuage = m_DeltaH - l_DTMIt.Get();

                        // Test if the altitude of the cloud is under the dtm
                        if (l_DHnuage <= 1.)
                        {
                            l_DHnuage = 1.;
                        }

                        // Determination of ground coordinates of the pixel
                        l_DTMPtr->TransformIndexToPhysicalPoint(l_L2CLDSubLabelIt.GetIndex(), l_GroundPoint);

                        // Compute the viewing direction of the center pixel
                        // IPVIE is a vector => the origin is not taken into account
                        // Convert the pixel index in physical coordinates
                        l_IPVIEGroundPoint[0] = l_IPVIEIt.Get()[0];
                        l_IPVIEGroundPoint[1] = l_IPVIEIt.Get()[1];

                        // Compute the solar direction of the center pixel
                        // IPSOL is a vector => the origin is not taken into account
                        l_IPSOLGroundPoint[0] = l_IPSOLIt.Get()[0];
                        l_IPSOLGroundPoint[1] = l_IPSOLIt.Get()[1];

                        // Determination of Nadir location of cloud (taking into account the viewing direction)
                        l_NadirPoint[0] = l_GroundPoint[0] + l_DHnuage * l_IPVIEGroundPoint[0] * m_InvVIEHRef;
                        l_NadirPoint[1] = l_GroundPoint[1] + l_DHnuage * l_IPVIEGroundPoint[1] * m_InvVIEHRef;

                        // Determination of shadow coordinates corresponding to the cloud pixel
                        l_NadirVector[0] = static_cast<double>(l_NadirPoint[0]);
                        l_NadirVector[1] = static_cast<double>(l_NadirPoint[1]);
                        l_NadirVector[2] = m_DeltaH; //l_IPCLAIt.Get());

                        l_VIEVector[0] = static_cast<double>(-l_IPSOLGroundPoint[0]);
                        l_VIEVector[1] = static_cast<double>(-l_IPSOLGroundPoint[1]);
                        l_VIEVector[2] = -m_SOLHRef;

                        // Call the utilities Intersection3D
                        const bool l_Valid = m_I3D->LaunchesIntersection3D("PersistentShadowProjectionWithOnlyLabelImageImageFilter",
                                l_NadirVector, l_VIEVector, l_ProjVector, true /* pWithoutError */, __FILE__, __FUNCTION__, __LINE__);

                        if (l_Valid == true)
                        {
                            if (l_ProjVector[2] <= l_NadirVector[2])
                            {
                                l_ProjPoint[0] = l_ProjVector[0];
                                l_ProjPoint[1] = l_ProjVector[1];

                                // Get the index of the intersected point
                                l_DTMPtr->TransformPhysicalPointToIndex(l_ProjPoint, l_ProjIndex);

                                l_ProjLinearIndex = l_ProjIndex[1] * l_nc + l_ProjIndex[0];
                                m_ShadowVectorMap[threadId][l_ProjLinearIndex] = l_L2CLDSubLabelValue;
                            }
                        }

                    } // if cloud pixel

                    ++l_L2CLDSubLabelIt;
                    ++l_DTMIt;
                    ++l_IPSOLIt;
                    ++l_IPVIEIt;
                } // end pixel loop
            }

        }

    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        void
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        void
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();
            // This is the part that allows to do threading.
            if (this->GetInput())
            {
                // Set the output requested region with the input largest possible region
                this->GetOutput()->CopyInformation(this->GetInput());
                this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

                if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
                {
                    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
                }
            }
        }

    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        void
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::Synthetize(void)
        {

            //// Here we fuse all the found maps.
            unsigned int l_NumberOfThreads = this->GetNumberOfThreads();
            LabelMapType l_ShadowMap = m_ShadowVectorMap[0];

            //// fill the final global maps.
            // Loop over the number of threads
            for (unsigned int threadId = 1; threadId < l_NumberOfThreads; threadId++)
            {
                // Load the maps of the current threads
                LabelMapType curMapLabel = m_ShadowVectorMap[threadId];

                typename LabelMapType::iterator iter = curMapLabel.begin();
                while (iter != curMapLabel.end())
                {
                    l_ShadowMap.insert((*iter));
                    iter++;
                }
            }

            // Grab the outputs
            typename MapLabelObjectType::Pointer outputShadows = static_cast<MapLabelObjectType*>(this->itk::ProcessObject::GetOutput(1));

            // The values to the outputs
            outputShadows->Set(l_ShadowMap);

        }

    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        void
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::Reset(void)
        {
            LabelImagePointer l_L2CLDSubLabelPtr = const_cast<LabelImageType *>(this->GetL2CLDSubLabelInput());
            DTMImagePointer l_DTMPtr = const_cast<DTMImageType *>(this->GetDTMInput());
            GRIDImagePointer l_IPSOLPtr = const_cast<GRIDImageType*>(this->GetIPSOLInput());
            GRIDImagePointer l_IPVIEPtr = const_cast<GRIDImageType *>(this->GetIPVIEInput());

            // update the input image information
            l_L2CLDSubLabelPtr->UpdateOutputInformation();
            l_DTMPtr->UpdateOutputInformation();
            l_IPSOLPtr->UpdateOutputInformation();
            l_IPVIEPtr->UpdateOutputInformation();

            // Load the number of thread
            const unsigned int numberOfThreads = this->GetNumberOfThreads();

            LabelMapType l_LabelMap;

            // Initialize the output values
            m_ShadowVectorMap = LabelVectorMapType(numberOfThreads, l_LabelMap);

        }

    /** PrintSelf method */
    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        void
        PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_InvVIEHRef:    " << m_InvVIEHRef << std::endl;

            // Compute the number of threads
            unsigned int l_NumberOfThreads = this->GetNumberOfThreads();

            // Display if a cloud has been already detected with reflectance threshold
            os << "====== Sum Maps found ======" << std::endl;
            // Thread loop
            for (unsigned int threadId = 1; threadId < l_NumberOfThreads; threadId++)
            {
                LabelMapType curMap = m_ShadowVectorMap[threadId];
                os << "=== Sum Map for thread #" << threadId << " ===" << std::endl;
                typename LabelMapType::iterator iter = curMap.begin();
                // Map loop
                while (iter != curMap.end())
                {
                    os << indent << (*iter).first << "  " << (*iter).second;
                    os << std::endl;
                    iter++;
                }
            }
            os << std::endl;
        }

    /** PrintSelf method */
    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        void
        ShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            //Superclass::PrintSelf(os, indent);
            LabelMapType l_ShadowMap = this->GetShadowMap();

            // Display the sum of value per label
            os << "====== Sum Map ======" << std::endl;
            typename LabelMapType::const_iterator iterShad = l_ShadowMap.begin();
            // Loop the map
            while (iterShad != l_ShadowMap.end())
            {
                os << indent << (*iterShad).first << "  " << (*iterShad).second;
                os << std::endl;
                iterShad++;
            }
            os << std::endl;
        }

} //end of namespace

#endif /* __vnsShadowProjectionWithOnlyLabelImageImageFilter_txx */
