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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowFinalPixelDetectionFilter_h
#define __vnsShadowFinalPixelDetectionFilter_h

#include "itkProcessObject.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkAddImageFilter.h"

#include "vnsShadowVariationThresholdValueEstimator.h"

#include "vnsShadowZoneLabelsDarkeningThresholdImageFunctor.h"

#include "otbImage.h"
#include "otbObjectList.h"
#include "otbImageList.h"
#include "otbVectorImage.h"
#include "otbImageListToVectorImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "vnsLowerConditionalThresholdImageFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include "vnsConstantImageSource.h"
#include "vnsKeepValueWithMaskFunctor.h"
#include "vnsMacro.h"



namespace vns
{
    /** \class  ShadowFinalPixelDetectionFilter
     * \brief This class filter the remaining shadow pixels from the
     * shadow by labels image list.
     *
     * The filtering is pixel-wise and computes a darkening threshold relatively to
     * all the potentially shadowy pixel in each image of the list.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TRatioImage, class TLabelImage,class TMaskImage>
        class ShadowFinalPixelDetectionFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef ShadowFinalPixelDetectionFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ShadowFinalPixelDetectionFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef TRatioImage RatioImageType;
            typedef typename RatioImageType::ConstPointer RatioImageConstPointer;
            typedef typename RatioImageType::Pointer RatioImagePointer;
            typedef typename RatioImageType::RegionType RegionType;
            typedef typename RatioImageType::PixelType RatioImagePixelType;
            typedef typename RatioImageType::SizeType SizeType;
            //typedef typename RatioImageType::InternalPixelType InputInternalPixelType;

            typedef otb::VectorImage<RatioImagePixelType> VectorImageType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            typedef otb::ImageList<LabelImageType> LabelImageListType;
            typedef typename LabelImageListType::Pointer LabelImageListPointerType;

            typedef unsigned long NbPixelsType;
            typedef typename std::map<LabelImagePixelType, NbPixelsType> NbPixelsForEachLabelMapType;

            typedef TMaskImage MaskImageType;
            typedef typename MaskImageType::Pointer MaskImagePointer;
            typedef typename MaskImageType::ConstPointer MaskImageConstPointer;
            typedef typename MaskImageType::PixelType MaskImagePixelType;
            typedef typename MaskImageType::IndexType IndexType;
            typedef typename MaskImageType::PointType PointType;
            typedef typename MaskImageType::SpacingType SpacingType;

            typedef otb::ImageList<MaskImageType> MaskImageListType;
            typedef typename MaskImageListType::Pointer MaskImageListPointerType;

            /** Filters typedefs */
            typedef Functor::KeepValueWithMaskFunctor<RatioImagePixelType, LabelImagePixelType, RatioImagePixelType> RatioMaskingFunctorType;
            typedef itk::BinaryFunctorImageFilter<RatioImageType, LabelImageType, RatioImageType, RatioMaskingFunctorType> MaskingFilterType;
            typedef typename MaskingFilterType::Pointer MaskingFilterPointerType;
            typedef otb::ObjectList<MaskingFilterType> MaskingFilterListType;
            typedef typename MaskingFilterListType::Pointer MaskingFilterListPointerType;

            typedef otb::ImageList<RatioImageType> DarkeningImageListType;
            typedef typename DarkeningImageListType::Pointer DarkeningImageListPointerType;
            typedef otb::ObjectList<DarkeningImageListType> DarkeningImageListListType;
            typedef typename DarkeningImageListListType::Pointer DarkeningImageListListPointerType;

            typedef otb::ImageListToVectorImageFilter<DarkeningImageListType, VectorImageType> DarkeningImageListToVectorImageFilterType;
            typedef typename DarkeningImageListToVectorImageFilterType::Pointer DarkeningImageListToVectorImageFilterPointerType;
            typedef otb::ObjectList<DarkeningImageListToVectorImageFilterType> DarkeningImageListToVectorImageFilterListType;
            typedef typename DarkeningImageListToVectorImageFilterListType::Pointer DarkeningImageListToVectorImageFilterListPointerType;

            typedef ShadowVariationThresholdValueEstimator<VectorImageType> ThresholdValueEstimatorFilterType;
            typedef typename ThresholdValueEstimatorFilterType::Pointer ThresholdValueEstimatorFilterPointerType;
            typedef otb::ObjectList<ThresholdValueEstimatorFilterType> ThresholdValueEstimatorListType;
            typedef typename ThresholdValueEstimatorListType::Pointer ThresholdValueEstimatorListPointerType;

            typedef ConstantImageSource<MaskImageType> ConstantFilterType;
            typedef typename ConstantFilterType::Pointer ConstantPointerFilterType;

            typedef Functor::LowerConditionalThresholdImageFunctor<RatioImagePixelType, MaskImagePixelType> RatioThresholdingFunctorType;
            typedef itk::UnaryFunctorImageFilter<RatioImageType, MaskImageType, RatioThresholdingFunctorType> ThresholdingFilterType;
            typedef typename ThresholdingFilterType::Pointer ThresholdingFilterPointerType;
            typedef otb::ObjectList<ThresholdingFilterType> ThresholdingFilterListType;
            typedef typename ThresholdingFilterListType::Pointer ThresholdingFilterListPointerType;

            vnsMemberAndSetAndGetConstReferenceMacro(ShadowCloudRatio, double)
            vnsMemberAndSetAndGetConstReferenceMacro(CLDPixelNbMap, NbPixelsForEachLabelMapType)
            vnsMemberAndSetAndGetConstReferenceMacro(RejectedPixelsNumberByLabel, NbPixelsForEachLabelMapType)

            // ProjShadDarkeningMax parameter
            vnsMemberAndSetAndGetConstReferenceMacro(ProjShadDarkeningMax, double)

            vnsMemberSetGetObjectMacro(InputMaskList, LabelImageListType);
            vnsMemberSetGetObjectMacro(InputDarkening, RatioImageType);

            // No_data pixel value accessors
            vnsMemberAndSetAndGetConstReferenceMacro(NoData, RealNoDataType)

            /** Get the altitude correction map output */
            vnsGetMacro(OutputMaskImageList, MaskImageListPointerType)

            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            ShadowFinalPixelDetectionFilter();
            /** Destructor */
            virtual
            ~ShadowFinalPixelDetectionFilter();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ShadowFinalPixelDetectionFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Filters declaration */
            MaskingFilterListPointerType m_MaskingImageFilterList;
            DarkeningImageListListPointerType m_DarkeningMaskedImageListList;
            DarkeningImageListToVectorImageFilterListPointerType m_DarkeningMaskedToVectorImageList;
            ThresholdValueEstimatorListPointerType m_ShadowThresholdValueEstimatorFilterList;
            std::vector<double> m_ThresholdVector;
            ThresholdingFilterListPointerType m_DarkeningThresholdFilterList;
            MaskImageListPointerType m_OutputMaskImageList;

                };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowFinalPixelDetectionFilter.txx"
#endif

#endif /* __vnsShadowFinalPixelDetectionFilter_h */
