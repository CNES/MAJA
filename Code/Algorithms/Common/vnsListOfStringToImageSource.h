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
 * VERSION : 4-8-0 : DM : LAIG-DM-MAC-1661-CNES : 23 mars 2016 : Extrapolation des valeurs s'ils en manquent*
 * VERSION : 4-8-0 : FA : LAIG-FA-MAC-1652-CNES : 23 mars 2016 : Extrapolation des valeurs s'ils en manquent*
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 15 novembre 2013 : Correction information géographique     *
 * 								Ajout de la projection dans l'image de sortie								*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 novembre 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsListOfStringToImageSource_h
#define __vnsListOfStringToImageSource_h

#include "itkImageSource.h"
#include "itkNumericTraits.h"
#include "vnsUtilities.h"
#include "itkMetaDataDictionary.h"

namespace vns
{

    /** \class ListOfStringToImageSource
     * \brief Generate an n-dimensional image of specifics pixel values.
     *
     * ListOfStringToImageSource generates an image of specific pixel values using a list of string
     *
     * \ingroup DataSources Multithreaded
     */
    template<typename TOutputImage>
        class ListOfStringToImageSource : public itk::ImageSource<TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ListOfStringToImageSource Self;
            typedef itk::ImageSource<TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            typedef Utilities::ListOfStrings ListOfStrings;

            /** Typedef for the output image PixelType. */
            typedef typename TOutputImage::PixelType OutputImagePixelType;

            /** Typedef to describe the output image region type. */
            typedef typename TOutputImage::RegionType OutputImageRegionType;

            /** Run-time type information (and related methods). */
            itkTypeMacro(ListOfStringToImageSource,ImageSource)

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Basic types from the OutputImageType */
            typedef typename TOutputImage::SizeType SizeType;
            typedef typename TOutputImage::IndexType IndexType;
            typedef typename TOutputImage::SpacingType SpacingType;
            typedef typename TOutputImage::PointType PointType;
            typedef typename SizeType::SizeValueType SizeValueType;
            typedef typename TOutputImage::SpacingValueType SpacingValueType;
            typedef typename TOutputImage::PointValueType PointValueType;

            /** Set/Get spacing of the output image */
            itkSetMacro( Spacing, SpacingType )
            itkGetMacro( Spacing, SpacingType )

            /** Set/Get origin of the output image */
            itkSetMacro( Origin, PointType )
            itkGetMacro( Origin, PointType )

            /** Set/Get the reference projection of the output image */
            void SetProjectionRef( std::string );
            const std::string
            GetProjectionRef() const;

            /** Set List of string*/
            void
            SetListOfString(const ListOfStrings & list, const bool p_ActivateDataExtrapolation);

        protected:
            ListOfStringToImageSource();
            virtual
            ~ListOfStringToImageSource();
            void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            virtual void
            GenerateOutputInformation();

            /** Main computation method */
            virtual void
            GenerateData();

        private:
            ListOfStringToImageSource(const ListOfStringToImageSource&); //purposely not implemented
            void
            operator=(const ListOfStringToImageSource&); //purposely not implemented

            SizeType m_Size; //size of the output image
            SpacingType m_Spacing; //spacing
            PointType m_Origin; //origin
            ListOfStrings m_InternalListOfString;
            std::string m_ProjectionRef; // coordinate system

        };

}
// end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsListOfStringToImageSource.txx"
#endif

#endif // __vnsListOfStringToImageSource_h
