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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 8 juillet 2016 : Audit code - suppresion de macros ID3    *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 29 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsOutsideCloudsShadowMaskGenerator_txx
#define __vnsOutsideCloudsShadowMaskGenerator_txx

#include "vnsOutsideCloudsShadowMaskGenerator.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "vnsMath.h"

#include "vnsDEMDataManager.h"
#include "vnsMacro.h"
#include <iomanip>

namespace vns
{

    // Constructor
    template<class TInputMask, class TDTMImage, class TGRIDImage, class TOutputImage>
        OutsideCloudsShadowMaskGenerator<TInputMask, TDTMImage, TGRIDImage, TOutputImage>::OutsideCloudsShadowMaskGenerator()
        {

            // Input/Output initialization
            this->SetNumberOfIndexedInputs(4);
            this->SetNumberOfRequiredInputs(4);
            this->SetNumberOfRequiredOutputs(1);
            this->SetNumberOfIndexedOutputs(1);

            this->SetNthOutput(0, OutputImageType::New());
            m_MaxCLA = 0.;
            m_VIEHRef = 0.;
            m_SOLHRef = 0.;

            // Init Translation
            m_Translation.Fill(0);
        }

    // Destructor
    template<class TInputMask, class TDTMImage, class TGRIDImage, class TOutputImage>
        OutsideCloudsShadowMaskGenerator<TInputMask, TDTMImage, TGRIDImage, TOutputImage>::~OutsideCloudsShadowMaskGenerator()
        {
        }

    //****************************** GenerateOutputInformation ********************************/
    // GenerateOutputInformation method
    template<class TInputMask, class TDTMImage, class TGRIDImage, class TOutputImage>
        void
        OutsideCloudsShadowMaskGenerator<TInputMask, TDTMImage, TGRIDImage, TOutputImage>::GenerateOutputInformation()
        {

            // Call the superclass' implementation of this method
            Superclass::GenerateOutputInformation();

            // Initialization of Intersector3D
            DTMImagePointer l_DTMPtr = dynamic_cast<DTMImageType *>(itk::ProcessObject::GetInput(1));

            m_I3D.Load(l_DTMPtr);

            // Projection of the center of the image
            IndexType l_CenterCoord;
            RegionType l_Region;
            PointType l_GroundPoint;
            PointType l_NadirPoint;
            PointType l_IPVIEGroundPoint;
            PointType l_IPSOLGroundPoint;
            PointType l_ProjPoint;
            IndexType l_ProjIndex;
            DTMImagePixelType l_DTMpix;

            std::vector<double> l_NadirVector(3);
            std::vector<double> l_VIEVector(3);
            std::vector<double> l_ProjVector(3);

            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
            GRIDImagePointer l_IPVIEPtr = dynamic_cast<GRIDImageType *>(itk::ProcessObject::GetInput(3));
            GRIDImagePointer l_IPSOLPtr = dynamic_cast<GRIDImageType *>(itk::ProcessObject::GetInput(2));

            // Get the largest possible region
            l_Region = l_IPEDGPtr->GetLargestPossibleRegion();

            // Set center coordinates
            l_CenterCoord[0] = l_Region.GetIndex()[0] + static_cast<int>(l_Region.GetSize()[0] / 2);
            l_CenterCoord[1] = l_Region.GetIndex()[1] + static_cast<int>(l_Region.GetSize()[1] / 2);

            // Determination of ground coordinates of the pixel
            //vnsLogSuperDebugMacro("DTM Origin " << l_DTMPtr->GetOrigin());
            //vnsLogSuperDebugMacro("DTM Spacing " << l_DTMPtr->GetSpacing());

            l_DTMPtr->TransformIndexToPhysicalPoint(l_CenterCoord, l_GroundPoint);

            // To have access to the pixel, use the image RequestedRegion and than Update()
            // The used region has a size of one and as index the l_CenterCoord
            RegionType l_ReqRegion;
            SizeType l_ReqSize;
            l_ReqSize.Fill(1);
            l_ReqRegion.SetSize(l_ReqSize);
            l_ReqRegion.SetIndex(l_CenterCoord);
            l_IPVIEPtr->SetRequestedRegion(l_ReqRegion);
            l_IPVIEPtr->Update();

            // Compute the viewing direction of the center pixel
            // IPVIE is a vector => the origin is not taken into account
            l_IPVIEGroundPoint[0] = l_IPVIEPtr->GetPixel(l_CenterCoord)[0];
            l_IPVIEGroundPoint[1] = l_IPVIEPtr->GetPixel(l_CenterCoord)[1];

            // Compute the solar direction of the center pixel
            // IPSOL is a vector => the origin is not taken into account
            l_IPSOLPtr->SetRequestedRegion(l_ReqRegion);
            l_IPSOLPtr->Update();
            l_IPSOLGroundPoint[0] = l_IPSOLPtr->GetPixel(l_CenterCoord)[0];
            l_IPSOLGroundPoint[1] = l_IPSOLPtr->GetPixel(l_CenterCoord)[1];

            //vnsLogSuperDebugMacro("l_CenterCoord " << l_CenterCoord);
            //vnsLogSuperDebugMacro("l_IPVIEPtr " << l_IPVIEPtr->GetPixel(l_CenterCoord)[0] << " " << l_IPVIEPtr->GetPixel(l_CenterCoord)[1]);
            //vnsLogSuperDebugMacro("l_IPSOLPtr " << l_IPSOLPtr->GetPixel(l_CenterCoord)[0] << " " << l_IPSOLPtr->GetPixel(l_CenterCoord)[1]);

            // Altitude of the center pixel
            l_DTMPtr->SetRequestedRegion(l_ReqRegion);
            l_DTMPtr->Update();
            l_DTMpix = l_DTMPtr->GetPixel(l_CenterCoord);

            // Determination of Nadir location of cloud (taking into account the viewing direction)
            if (vnsEqualsDoubleMacro(m_VIEHRef,0) == true)
            {
                vnsExceptionBusinessMacro("The reference altitude for viewing direction is null !");
            }

            //vnsLogSuperDebugMacro("l_GroundPoint " << l_GroundPoint[0] << " " << l_GroundPoint[1]);
            //vnsLogSuperDebugMacro("l_DTMpix " << l_DTMpix);
            //vnsLogSuperDebugMacro("l_IPVIEGroundPoint " << l_IPVIEGroundPoint[0] << " " << l_IPVIEGroundPoint[1]);
            //vnsLogSuperDebugMacro("m_VIEHRef " << m_VIEHRef);
            //vnsLogSuperDebugMacro("l_IPSOLGroundPoint " << l_IPSOLGroundPoint[0] << " " << l_IPSOLGroundPoint[1]);

            l_NadirPoint[0] = l_GroundPoint[0] + (m_MaxCLA - l_DTMpix) * l_IPVIEGroundPoint[0] / m_VIEHRef;
            l_NadirPoint[1] = l_GroundPoint[1] + (m_MaxCLA - l_DTMpix) * l_IPVIEGroundPoint[1] / m_VIEHRef;

            // Determination of shadow coordinates corresponding to the cloud pixel
            l_NadirVector[0] = static_cast<double>(l_NadirPoint[0]);
            l_NadirVector[1] = static_cast<double>(l_NadirPoint[1]);
            l_NadirVector[2] = m_MaxCLA;

            l_VIEVector[0] = static_cast<double>(-l_IPSOLGroundPoint[0]);
            l_VIEVector[1] = static_cast<double>(-l_IPSOLGroundPoint[1]);

            l_VIEVector[2] = -m_SOLHRef;

            // Call the utilities Intersection3D
            const bool l_Valid = m_I3D.LaunchesIntersection3D("OutsideCloudsShadowMaskGenerator", l_NadirVector, l_VIEVector, l_ProjVector,
                    true /* pWithoutError */, __FILE__, __FUNCTION__, __LINE__);

            // Intersection3D generated a result
            if (l_Valid == true)
            {
                //vnsLogSuperDebugMacro("Resultat : " << l_ProjVector[0] << ", " << l_ProjVector[1] << ", " << l_ProjVector[2]);
                if (l_ProjVector[2] <= l_NadirVector[2])
                {
                    l_ProjPoint[0] = l_ProjVector[0];
                    l_ProjPoint[1] = l_ProjVector[1];

                    l_DTMPtr->TransformPhysicalPointToIndex(l_ProjPoint, l_ProjIndex);

                    m_Translation[0] = l_ProjIndex[0] - l_CenterCoord[0];
                    m_Translation[1] = l_ProjIndex[1] - l_CenterCoord[1];

                }
                else
                {
                    vnsLogWarningMacro("Intersector3D : The input point is hidden !!!");
                }
            }
            else
            {
                vnsLogWarningMacro("Intersector3D : No intersection found !!!")
            }

        }

    //****************************** ThreadedGenerateData ********************************/
    // ThreadedGenerateData method
    template<class TInputMask, class TDTMImage, class TGRIDImage, class TOutputImage>
        void
        OutsideCloudsShadowMaskGenerator<TInputMask, TDTMImage, TGRIDImage, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {

            IndexType l_TranslateIndex;
            IndexType l_RegionIndex;
            IndexType l_IPEDGIndex;

            RegionType l_RegionProj;
            RegionType l_RegionCrop;

            InputMaskPixelType l_Value(0);


            // Get pointers to input images
            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
            DTMImageConstPointer l_DTMPtr = this->GetDTMInput();

            // Get the output mask
            OutputImagePointer l_IPCLDOutShadPtr = this->GetOutput(0);

            // Define the input image iterators
            itk::ImageRegionConstIterator<InputMaskType> l_IPEDGIt(l_IPEDGPtr, outputRegionForThread);

            // Define the output image iterators
            itk::ImageRegionIterator<OutputImageType> l_IPCLDOutShadIt(l_IPCLDOutShadPtr, outputRegionForThread);

            // Projection of the input edge mask
            l_RegionProj = l_IPEDGPtr->GetLargestPossibleRegion();
            l_RegionIndex[0] = l_IPEDGPtr->GetLargestPossibleRegion().GetIndex()[0] + m_Translation[0];
            l_RegionIndex[1] = l_IPEDGPtr->GetLargestPossibleRegion().GetIndex()[1] + m_Translation[1];
            l_RegionProj.SetIndex(l_RegionIndex);

            // Compute the intersection of the input edge mask and the projected one
            l_RegionProj.Crop(l_IPEDGPtr->GetLargestPossibleRegion());

            l_IPEDGIt.GoToBegin();
            l_IPCLDOutShadIt.GoToBegin();

            // Pixel loop
            while (l_IPCLDOutShadIt.IsAtEnd() == false)
            {
                l_Value = l_IPEDGIt.Get();

                // Initialization of the output mask with the complementary of
                // the input edge mask
                // (pixel = 0 => output pixel = 1) and
                // (pixel = 1 => output pixel = 0)
                if (l_Value == 0)
                {
                    l_IPCLDOutShadIt.Set(static_cast<OutputImagePixelType>(1));
                }
                else
                {
                    l_IPCLDOutShadIt.Set(static_cast<OutputImagePixelType>(0));
                }

                // Get the index of the pixel of the input edge mask
                l_IPEDGIndex = l_IPEDGIt.GetIndex();

                // Test if the pixel is inside the crop region
                // (intersection of the input edge mask and the projected one)
                if (l_RegionProj.IsInside(l_IPEDGIndex) == true)
                {
                    // If the pixel does not belong to edges of the input mask
                    if (l_IPEDGIt.Get() == 0)
                    {
                        // The pixel is considered as belonging to the projected mask
                        // Then, we look for the pixel index in the input edge mask
                        l_TranslateIndex[0] = l_IPEDGIndex[0] - m_Translation[0];
                        l_TranslateIndex[1] = l_IPEDGIndex[1] - m_Translation[1];

                        // if this pixel does not belong to edges, the output
                        // mask is set to one
                        if (l_IPEDGPtr->GetPixel(l_TranslateIndex) == 0)
                        {
                            l_IPCLDOutShadIt.Set(static_cast<OutputImagePixelType>(0));
                        }
                    }

                }
                ++l_IPEDGIt;
                ++l_IPCLDOutShadIt;
            }

        }

    // PrintSelf method
    template<class TInputMask, class TDTMImage, class TGRIDImage, class TOutputImage>
        void
        OutsideCloudsShadowMaskGenerator<TInputMask, TDTMImage, TGRIDImage, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_VIEHRef:    " << m_VIEHRef << std::endl;
            os << "m_MaxCLA:    " << m_MaxCLA << std::endl;
        }

} // End namespace vns

#endif /* __vnsOutsideCloudsShadowMaskGenerator_txx */
