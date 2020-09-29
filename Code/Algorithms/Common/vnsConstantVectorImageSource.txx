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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 25 avril 2016 : Ajout methode pour positionner parametres *
 *                                                                                 avec une image en entree *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 15 novembre 2013 : Correction information géographique     *
 * 								Ajout de la projection dans l'image de sortie								*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConstantVectorImageSource_txx
#define __vnsConstantVectorImageSource_txx

#include "vnsConstantVectorImageSource.h"
#include "itkImageRegionIterator.h"
#include "itkObjectFactory.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

namespace vns
{

    /**
     *
     */
    template<class TOutputImage>
        ConstantVectorImageSource<TOutputImage>::ConstantVectorImageSource()
        {

            //Initial image is 64 wide in each direction.
            for (unsigned int i = 0; i < TOutputImage::GetImageDimension(); i++)
            {
                m_Size[i] = 64;
                m_Spacing[i] = 1.0;
                m_Origin[i] = 0.0;
            }

//            m_ConstantValue = (OutputImagePixelType) (0);
            m_ProjectionRef = "";
        }

    template<class TOutputImage>
        ConstantVectorImageSource<TOutputImage>::~ConstantVectorImageSource()
        {
        }

    template<class TOutputImage>
        void
        ConstantVectorImageSource<TOutputImage>::SetSize(SizeValueArrayType sizeArray)
        {
            // Check the size is modified
            const unsigned int count = TOutputImage::ImageDimension;
            unsigned int i;
            for (i = 0; i < count; i++)
            {
                if (sizeArray[i] != this->m_Size[i])
                {
                    break;
                }
            }
            // Update the size if necessary
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
        const typename ConstantVectorImageSource<TOutputImage>::SizeValueType *
        ConstantVectorImageSource<TOutputImage>::GetSize() const
        {
            return this->m_Size.GetSize();
        }

    template<class TOutputImage>
        void
        ConstantVectorImageSource<TOutputImage>::SetSpacing(SpacingValueArrayType spacingArray)
        {
            //Check the spacing is modified
            const unsigned int count = TOutputImage::ImageDimension;
            unsigned int i;
            for (i = 0; i < count; i++)
            {
                if (spacingArray[i] != this->m_Spacing[i])
                {
                    break;
                }
            }
            // Update the spacing if necessary
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
        ConstantVectorImageSource<TOutputImage>::SetOrigin(PointValueArrayType originArray)
        {
            //Check the origin is modified
            const unsigned int count = TOutputImage::ImageDimension;
            unsigned int i;
            for (i = 0; i < count; i++)
            {
                if (originArray[i] != this->m_Origin[i])
                {
                    break;
                }
            }
            // Update the origin if necessary
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
        void
        ConstantVectorImageSource<TOutputImage>::SetOriginSpacingSizeFromImage(const TOutputImage * image)
        {
            m_Origin = image->GetOrigin();
            m_Spacing = image->GetSignedSpacing();
            m_Size = image->GetLargestPossibleRegion().GetSize();
        }

    template<class TOutputImage>
        void
        ConstantVectorImageSource<TOutputImage>::SetProjectionRef(std::string projection)
        {
            // Get the metadata dictionary of the image source
            itk::MetaDataDictionary & dict = this->GetOutput(0)->GetMetaDataDictionary();
            // Set the projection
            itk::EncapsulateMetaData<std::string>(dict, otb::MetaDataKey::ProjectionRefKey, projection);
            m_ProjectionRef = projection;
        }

    template<class TOutputImage>
        const typename ConstantVectorImageSource<TOutputImage>::PointValueType *
        ConstantVectorImageSource<TOutputImage>::GetOrigin() const
        {
            // Return the Origin in Array type
            for (unsigned int i = 0; i < TOutputImage::ImageDimension; i++)
            {
                this->m_OriginArray[i] = this->m_Origin[i];
            }
            return this->m_OriginArray;
        }

    template<class TOutputImage>
        const typename ConstantVectorImageSource<TOutputImage>::SpacingValueType *
        ConstantVectorImageSource<TOutputImage>::GetSpacing() const
        {
            // Return the spacing in Array type
            for (unsigned int i = 0; i < TOutputImage::ImageDimension; i++)
            {
                this->m_SpacingArray[i] = this->m_Spacing[i];
            }
            return this->m_SpacingArray;
        }

    template<class TOutputImage>
        const std::string
        ConstantVectorImageSource<TOutputImage>::GetProjectionRef() const
        {
            // Get the projection if it has been set before !
            return this->m_ProjectionRef;
        }
    /**
     *
     */
    template<class TOutputImage>
        void
        ConstantVectorImageSource<TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << indent << "Constant Value: " << static_cast<typename itk::NumericTraits<OutputImagePixelType>::PrintType>(m_ConstantValue)
                    << std::endl;
            unsigned int i;
            // Print the Origin
            os << indent << "Origin: [";
            for (i = 0; i < TOutputImage::ImageDimension - 1; i++)
            {
                os << m_Origin[i] << ", ";
            }
            os << m_Origin[i] << "]" << std::endl;

            // Print the Spacing
            os << indent << "Spacing: [";
            for (i = 0; i < TOutputImage::ImageDimension - 1; i++)
            {
                os << m_Spacing[i] << ", ";
            }
            os << m_Spacing[i] << "]" << std::endl;

            // Print the Size
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
        ConstantVectorImageSource<TOutputImage>::GenerateOutputInformation()
        {
            TOutputImage *output;
            IndexType index;
            index.Fill(0);

            // Get the output
            output = this->GetOutput(0);

            typename TOutputImage::RegionType largestPossibleRegion;
            // Set the largest possible region
            largestPossibleRegion.SetSize(this->m_Size);
            largestPossibleRegion.SetIndex(index);
            output->SetLargestPossibleRegion(largestPossibleRegion);

            // Checks if the "Instance" is a VectorImage
            if (strcmp(output->GetNameOfClass(), "VectorImage") == 0)
            {
                // Copy the NumberOfComponentsPerpixel
                output->SetNumberOfComponentsPerPixel(m_ConstantValue.GetSize());
            }

            // Set the Spacing and the Origin
            output->SetSignedSpacing(m_Spacing);
            output->SetOrigin(m_Origin);
        }

    //----------------------------------------------------------------------------
    template<typename TOutputImage>
        void
        ConstantVectorImageSource<TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                itk::ThreadIdType /*threadId*/)
        {
            itkDebugMacro(<<"Generating a random image of scalars");

            // Support progress methods/callbacks

            typedef typename TOutputImage::PixelType scalarType;
            typename TOutputImage::Pointer image = this->GetOutput(0);

            itk::ImageRegionIterator<TOutputImage> it(image, outputRegionForThread);

            for (; !it.IsAtEnd(); ++it)
            {
                it.Set((scalarType) m_ConstantValue);
            }
        }

} // end namespace vns

#endif
