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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 juil 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowMaskProjection_txx
#define __vnsShadowMaskProjection_txx

#include "vnsShadowMaskProjection.h"
#include "itkImageRegionIterator.h"
#include "itkObjectFactory.h"

namespace vns
{

    /**
     *
     */
    template<class TOutputImage>
        ShadowMaskProjection<TOutputImage>::ShadowMaskProjection() :
                m_ShadowMap()
        {

            //Initial image is 64 wide in each direction.
            for (unsigned int i = 0; i < TOutputImage::GetImageDimension(); i++)
            {
                m_Size[i] = 64;
                m_Spacing[i] = 1.0;
                m_Origin[i] = 0.0;
            }

        }

    template<class TOutputImage>
        ShadowMaskProjection<TOutputImage>::~ShadowMaskProjection()
        {
        }

    template<class TOutputImage>
        void
        ShadowMaskProjection<TOutputImage>::SetSize(SizeValueArrayType sizeArray)
        {
            const unsigned int count = TOutputImage::ImageDimension;
            unsigned int i;
            // Checks if the new size is different
            for (i = 0; i < count; i++)
            {
                if (sizeArray[i] != this->m_Size[i])
                {
                    break;
                }
            }
            // If different, update the new value
            if (i < count)
            {
                this->Modified();
                for (i = 0; i < count; i++)
                {
                    this->m_Size[i] = sizeArray[i];
                }
            }
        }

    template<class TOutputImage>
        const typename ShadowMaskProjection<TOutputImage>::SizeValueType *
        ShadowMaskProjection<TOutputImage>::GetSize() const
        {
            return this->m_Size.GetSize();
        }

    template<class TOutputImage>
        void
        ShadowMaskProjection<TOutputImage>::SetSpacing(SpacingValueArrayType spacingArray)
        {
            const unsigned int count = TOutputImage::ImageDimension;
            unsigned int i;
            // Checks if the new spacing is different
            for (i = 0; i < count; i++)
            {
                if (spacingArray[i] != this->m_Spacing[i])
                {
                    break;
                }
            }
            // If different, update the new value
            if (i < count)
            {
                this->Modified();
                for (i = 0; i < count; i++)
                {
                    this->m_Spacing[i] = spacingArray[i];
                }
            }
        }

    template<class TOutputImage>
        void
        ShadowMaskProjection<TOutputImage>::SetOrigin(PointValueArrayType originArray)
        {
            const unsigned int count = TOutputImage::ImageDimension;
            unsigned int i;
            // Checks if the new origin is different
            for (i = 0; i < count; i++)
            {
                if (originArray[i] != this->m_Origin[i])
                {
                    break;
                }
            }
            // If different, update the new value
            if (i < count)
            {
                this->Modified();
                for (i = 0; i < count; i++)
                {
                    this->m_Origin[i] = originArray[i];
                }
            }
        }

    template<class TOutputImage>
        const typename ShadowMaskProjection<TOutputImage>::PointValueType *
        ShadowMaskProjection<TOutputImage>::GetOrigin() const
        {
            for (unsigned int i = 0; i < TOutputImage::ImageDimension; i++)
            {
                this->m_OriginArray[i] = this->m_Origin[i];
            }
            return this->m_OriginArray;
        }

    template<class TOutputImage>
        const typename ShadowMaskProjection<TOutputImage>::SpacingValueType *
        ShadowMaskProjection<TOutputImage>::GetSpacing() const
        {
            for (unsigned int i = 0; i < TOutputImage::ImageDimension; i++)
            {
                this->m_SpacingArray[i] = this->m_Spacing[i];
            }
            return this->m_SpacingArray;
        }

    /**
     *
     */
    template<class TOutputImage>
        void
        ShadowMaskProjection<TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            // Check the origin, spacing and size of the output image
            unsigned int i;
            os << indent << "Origin: [";
            for (i = 0; i < TOutputImage::ImageDimension - 1; i++)
            {
                os << m_Origin[i] << ", ";
            }
            os << m_Origin[i] << "]" << std::endl;

            os << indent << "Spacing: [";
            for (i = 0; i < TOutputImage::ImageDimension - 1; i++)
            {
                os << m_Spacing[i] << ", ";
            }
            os << m_Spacing[i] << "]" << std::endl;

            os << indent << "Size: [";
            for (i = 0; i < TOutputImage::ImageDimension - 1; i++)
            {
                os << m_Size[i] << ", ";
            }
            os << m_Size[i] << "]" << std::endl;
        }

    //----------------------------------------------------------------------------
    template<typename TOutputImage>
        void
        ShadowMaskProjection<TOutputImage>::GenerateOutputInformation()
        {
            TOutputImage *output;
            IndexType index;
            index.Fill(0);

            output = this->GetOutput(0);

            typename TOutputImage::RegionType largestPossibleRegion;
            largestPossibleRegion.SetSize(this->m_Size);
            largestPossibleRegion.SetIndex(index);
            output->SetLargestPossibleRegion(largestPossibleRegion);

            output->SetSignedSpacing(m_Spacing);
            output->SetOrigin(m_Origin);
        }

    //----------------------------------------------------------------------------
    template<typename TOutputImage>
        void
        ShadowMaskProjection<TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                itk::ThreadIdType /*threadId*/)
        {

            IndexType l_Index;

            // Get the output mask
            OutputImagePointer l_IPCLDShadPtr = this->GetIPCLDShadOutput();

            const unsigned long l_nc = l_IPCLDShadPtr->GetLargestPossibleRegion().GetSize(0);
            // V 4.4l_nl = l_IPCLDShadPtr->GetLargestPossibleRegion().GetSize(1);

            // Define the output image iterators
            itk::ImageRegionIterator<TOutputImage> l_IPCLDShadIt(l_IPCLDShadPtr, outputRegionForThread);

            l_IPCLDShadIt.GoToBegin();

            // Pixel loop
            while (l_IPCLDShadIt.IsAtEnd() == false)
            {

                // Initialization of the output image
                l_IPCLDShadIt.Set(static_cast<OutputImagePixelType>(0));

                l_Index = l_IPCLDShadIt.GetIndex();
                const unsigned long l_LinearIndex = l_Index[1] * l_nc + l_Index[0];

                // For each pixel listed in the shadow map, set the label value to the
                // output pixel
                if (m_ShadowMap.find(l_LinearIndex) != m_ShadowMap.end())
                {
                    l_IPCLDShadIt.Set(static_cast<OutputImagePixelType>(m_ShadowMap[l_LinearIndex]));
                }

                ++l_IPCLDShadIt;
            } // end pixel loop

        }

} // end namespace vns

#endif
