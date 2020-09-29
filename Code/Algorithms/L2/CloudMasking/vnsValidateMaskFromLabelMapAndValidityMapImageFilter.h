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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection **
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsValidateMaskFromLabelMapAndValidityMapImageFilter.h 10095 2016-01-15 09:42:27Z abricier $            *
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsValidateMaskFromLabelMapAndValidityMapImageFilter_h
#define __vnsValidateMaskFromLabelMapAndValidityMapImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionConstIterator.h"

#include "vnsMacro.h"
#include "vnsUtilities.h"

namespace vns
{
    /** \class  ValidateMaskFromLabelMapAndValidityMapImageFilter
     * \brief This class implements .
     *
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputMask, class TLabelImage, class TOutputImage>
        class ValidateMaskFromLabelMapAndValidityMapImageFilter : public itk::ImageToImageFilter<TInputMask, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ValidateMaskFromLabelMapAndValidityMapImageFilter Self;
            typedef itk::ImageToImageFilter<TInputMask, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(ValidateMaskFromLabelMapAndValidityMapImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;
            typedef typename InputMaskType::RegionType RegionType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            typedef std::map<LabelImagePixelType, bool> LabelValidityMapType;

            // Validity map parameter: true if potentially discarded pixels for associated label
            // must be discarded, false otherwise
            void
            SetValidityMap(const LabelValidityMapType & map)
            {
                this->m_ValidityMap = map;
                this->Modified();
            }
            itkGetConstReferenceMacro(ValidityMap, LabelValidityMapType)

            /** Set the image of the cloud mask extension */
            vnsSetGetInputMacro(Mask , InputMaskType, 0)

            /** Set the image of labels of the cloud mask extension */
            vnsSetGetInputMacro(Label , LabelImageType, 1)

            /** Set the image of the cloud flag correl extension */
            vnsSetGetInputMacro(ExtCorrelFlag, InputMaskType, 2)


        protected:
            /** Constructor */
            ValidateMaskFromLabelMapAndValidityMapImageFilter();
            /** Destructor */
            virtual
            ~ValidateMaskFromLabelMapAndValidityMapImageFilter();

            /** Multi-thread version GenerateData. */
            void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ValidateMaskFromLabelMapAndValidityMapImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            LabelValidityMapType m_ValidityMap;
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsValidateMaskFromLabelMapAndValidityMapImageFilter.txx"
#endif

#endif /* __vnsValidateMaskFromLabelMapAndValidityMapImageFilter_h */

