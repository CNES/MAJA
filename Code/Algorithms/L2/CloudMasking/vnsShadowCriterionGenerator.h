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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 16 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 												Factorisation du calcul du critère de détection des ombres  *
 * VERSION : 4-0-0 : <TypeFT> : <NumFT> : 6 janvier 2014 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsShadowCriterionGenerator.h 3545 2011-11-29 14:21:48Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowCriterionGenerator_h
#define __vnsShadowCriterionGenerator_h

#include <map>
#include "itkProcessObject.h"
#include "vnsValidShadowMaskImageFilter.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  ShadowCriterionGenerator
     * \brief This class computes the shadow criterion used to determine if a pixel is a shadow pixel or not.
     *
     * Shadow_Criterion(altitude,label)=average((TOCR(D,Shadow_band)-TOCR(D-1,Shadow_band)) / TOCR(D-1)(Shadow_band))
     *
     * If the pixel is shadowed, the shadow criterion is negative.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TDiffImage, class TLabelImage>
        class ShadowCriterionGenerator : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef ShadowCriterionGenerator Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ShadowCriterionGenerator, ProcessObject )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;

            typedef TDiffImage DiffImageType;
            typedef typename DiffImageType::Pointer DiffImagePointer;
            typedef typename DiffImageType::ConstPointer DiffImageConstPointer;
            typedef typename DiffImageType::PixelType DiffImagePixelType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            /** Filters typedefs */
            typedef unsigned long LinearIndex;
            typedef typename std::map<LinearIndex, LabelImagePixelType> LabelMapType;

            typedef ValidShadowMaskImageFilter<DiffImageType, InputImageType, LabelImageType> ValidShadowFilterType;
            typedef typename ValidShadowFilterType::Pointer ValidShadowFilterPointerType;
            typedef typename ValidShadowFilterType::DoubleMapType DoubleMapType;
            typedef typename ValidShadowFilterType::UIMapType UIMapType;

            // No_data pixel value accessors
            itkSetMacro(NoData, RealNoDataType)

            itkGetConstReferenceMacro(NoData, RealNoDataType)

            // Minimum number of pixels in label to perform the altitude refinement accessors
            itkSetMacro(AbsNbpixThreshold, unsigned short)

            itkGetConstReferenceMacro(AbsNbpixThreshold, unsigned short)

            // Altitude index value accessors
            itkSetMacro(LevelIndex, unsigned short)

            itkGetConstReferenceMacro(LevelIndex, unsigned short)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsMemberSetGetObjectMacro(L2inRCRInput, InputImageType)

            /** Set the difference reflectance image between the current image and the composite product */
            vnsMemberSetGetObjectMacro(DiffInput, DiffImageType)

            /** Set the image of label of the cloud shadow mask */
            vnsMemberSetGetObjectMacro(IPCLDShadLabelInput, LabelImageType)

            /** Set the previous shadow criterion map generated for another tested altitude */
            void
            SetShadowCriterionMap(DoubleMapType &map);
            DoubleMapType &
            GetShadowCriterionMap();

            /** Set the previous altitude by label map */
            void
            SetLevelMap(UIMapType &map);
            UIMapType &
            GetLevelMap();

            void
            SetShadowBand(unsigned int pParam)
            {
                //Note 4-1 : the ShadowBand is iued for the L2inRCRInput. So must be set with the ShadowBandRCR (and not ShadowBandTOCR)
                m_ValidShadowFilter->SetShadowBand(pParam);
            }

            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            ShadowCriterionGenerator();
            /** Destructor */
            virtual
            ~ShadowCriterionGenerator();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ShadowCriterionGenerator(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Parameters declaration */
            /** No_data value declaration*/
            RealNoDataType m_NoData;
            /** Level index used to cover the altitude range */
            unsigned short m_LevelIndex;
            /** Minimum number of usable pixels in a label to perform the altitude refinement */
            unsigned short m_AbsNbpixThreshold;

            /** Filters declaration */
            ValidShadowFilterPointerType m_ValidShadowFilter;

            DoubleMapType m_ShadowCriterionMap;
            UIMapType m_LevelMap;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowCriterionGenerator.txx"
#endif

#endif /* __vnsShadowCriterionGenerator_h */
