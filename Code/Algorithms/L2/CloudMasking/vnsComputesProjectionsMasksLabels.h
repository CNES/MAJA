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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputesProjectionsMasksLabels_h
#define __vnsComputesProjectionsMasksLabels_h

#include "vnsMaskingImageFunctor.h"
#include "itkProcessObject.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkAddImageFilter.h"

#include "vnsShadowMaskProjection.h"
#include "vnsShadowProjectionWithOnlyLabelImageImageFilter.h"
#include "vnsShadowZoneLabelsDarkeningThresholdImageFunctor.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbObjectList.h"
#include "otbImageList.h"

#include "vnsKeepValueFunctor.h"
#include "vnsMacro.h"

#include "vnsDEMDataManager.h"
#include "vnsConstantImageSource.h"

namespace vns
{
    /** \class  ComputesProjectionsMasksLabels
     * \brief This class computes the altitude refinement of shadows.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TRatioImage, class TLabelImage, class TMaskImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        class ComputesProjectionsMasksLabels : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef ComputesProjectionsMasksLabels Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ComputesProjectionsMasksLabels, ProcessObject )

            /** Some convenient typedefs. */
            typedef TRatioImage RatioImageType;
            typedef typename RatioImageType::ConstPointer RatioImageConstPointer;
            typedef typename RatioImageType::Pointer RatioImagePointer;
            typedef typename RatioImageType::RegionType RegionType;
            typedef typename RatioImageType::PixelType RatioImagePixelType;
            typedef typename RatioImageType::SizeType SizeType;
            typedef typename RatioImageType::InternalPixelType InputInternalPixelType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            typedef TMaskImage MaskImageType;
            typedef typename MaskImageType::Pointer      MaskImagePointer;
            typedef typename MaskImageType::ConstPointer MaskImageConstPointer;
            typedef typename MaskImageType::PixelType    MaskImagePixelType;

            typedef TGRIDImage GRIDImageType;
            typedef typename GRIDImageType::Pointer GRIDImagePointer;
            typedef typename GRIDImageType::ConstPointer GRIDImageConstPointer;
            typedef typename GRIDImageType::PixelType GRIDImagePixelType;
            typedef typename GRIDImageType::IndexType IndexType;
            typedef typename GRIDImageType::PointType PointType;
            typedef typename GRIDImageType::SpacingType SpacingType;

            typedef TCLAImage CLAImageType;
            typedef typename CLAImageType::Pointer CLAImagePointer;
            typedef typename CLAImageType::ConstPointer CLAImageConstPointer;
            typedef typename CLAImageType::PixelType CLAImagePixelType;

            typedef TDTMImage DTMImageType;
            typedef typename DTMImageType::Pointer DTMImagePointer;
            typedef typename DTMImageType::ConstPointer DTMImageConstPointer;
            typedef typename DTMImageType::PixelType DTMImagePixelType;

            /** Filters typedefs */

            typedef DEMDataManager<DTMImageType> Intersector3DType;

            typedef ShadowProjectionWithOnlyLabelImageImageFilter<LabelImageType, DTMImageType, GRIDImageType, CLAImageType> ShadowProjectionFilterType;
            typedef otb::ObjectList<ShadowProjectionFilterType> ShadowProjectionFilterPointerTypeList;

            // Result map typedef
            typedef unsigned long LinearIndex;
            typedef typename std::map<LinearIndex, LabelImagePixelType> LabelMapType;

            typedef unsigned long NbPixelsType;
            typedef typename std::map<LabelImagePixelType, NbPixelsType> NbPixelsForEachLabelMapType;

            typedef ShadowMaskProjection<LabelImageType> ShadowMaskProjectionFilterType;

            typedef otb::ObjectList<ShadowMaskProjectionFilterType> ShadowMaskProjectionFilterPointerTypeList;

            typedef ConstantImageSource<CLAImageType> CLAConstantFilterType;

            typedef otb::ImageList<LabelImageType> LabelImageListType;
            typedef typename LabelImageListType::Pointer LabelImageListPointer;

            typedef vns::Functor::KeepValueFunctor<LabelImagePixelType, LabelImagePixelType> KeepsCloudLabelFunctorType;
            typedef itk::UnaryFunctorImageFilter<LabelImageType, LabelImageType, KeepsCloudLabelFunctorType> KeepsCloudLabelFilterType;
            typedef otb::ObjectList<KeepsCloudLabelFilterType> KeepsCloudLabelFilterPointerTypeList;

            typedef Functor::MaskingImageFunctor<LabelImagePixelType, MaskImagePixelType, LabelImagePixelType> LabelMaskingFunctorType;
            typedef itk::BinaryFunctorImageFilter<LabelImageType, MaskImageType, LabelImageType, LabelMaskingFunctorType> LabelMaskingFilterType;
            typedef typename LabelMaskingFilterType::Pointer LabelMaskingFilterPointerType;
            typedef otb::ObjectList<LabelMaskingFilterType> LabelMaskingFilterListType;
            typedef typename LabelMaskingFilterListType::Pointer LabelMaskingFilterListPointerType;

            typedef Functor::MaskingImageFunctor<MaskImagePixelType, RatioImagePixelType, MaskImagePixelType> MaskFusionFunctorType;
            typedef itk::BinaryFunctorImageFilter<MaskImageType, RatioImageType, MaskImageType, MaskFusionFunctorType> MaskFusionFilterType;
            typedef typename MaskFusionFilterType::Pointer MaskFusionFilterPointerType;

            typedef Functor::ShadowZoneLabelsDarkeningThresholdImageFunctor<LabelImagePixelType, RatioImagePixelType, LabelImagePixelType> DarkeningThresholdFunctorType;
            typedef itk::BinaryFunctorImageFilter<LabelImageType, RatioImageType, LabelImageType, DarkeningThresholdFunctorType> DarkeningThresholdFilterType;
            typedef otb::ObjectList<DarkeningThresholdFilterType> DarkeningThresholdFilterListType;

            vnsMemberAndSetAndGetConstReferenceMacro(CLDLabelsCount, LabelImagePixelType)

            // No_data pixel value accessors
            vnsMemberAndSetAndGetConstReferenceMacro(NoData, RealNoDataType)

            // DeltaHMax parameter accessors
            vnsMemberAndSetAndGetConstReferenceMacro(DeltaHMax, short)

            // DeltaHMin parameter accessors
            vnsMemberAndSetAndGetConstReferenceMacro(DeltaHMin, short)

            // DeltaHStep parameter accessors
            vnsMemberAndSetAndGetConstReferenceMacro(DeltaHStep, short)

            // First reference altitude parameter accessors
            vnsMemberAndSetAndGetConstReferenceMacro(SOLH1, double)

            // Reference altitude for sun direction accessors
            vnsMemberAndSetAndGetConstReferenceMacro(VIEHRef, double)

            vnsMemberAndSetAndGetConstReferenceMacro(DarkeningThreshold, double)

           //            // Shadow band accessors
//            itkSetMacro(ShadowBand, unsigned int)
//
//            itkGetConstReferenceMacro(ShadowBand, unsigned int)

            /** Set the image of label of the cloud mask */
            vnsMemberSetGetObjectMacro(IPCLDSubLabelInput, LabelImageType)

            /** Set the viewing direction grid */
            vnsMemberSetGetObjectMacro(IPVIEInput, GRIDImageType)

            /** Set the solar direction grid for the first reference altitude */
            vnsMemberSetGetObjectMacro(IPSOL1Input, GRIDImageType)

            /** Set the cloud altitude image */
            vnsMemberSetGetObjectMacro(IPCLAInput, CLAImageType)

            /** Set the DTM image */
            vnsMemberSetGetObjectMacro(DTMInput, DTMImageType)

            /** Set the EDG Mask image */
            vnsMemberSetGetObjectMacro(EDGInput, MaskImageType)

            /** Set the CLD Mask image */
            vnsMemberSetGetObjectMacro(CLDInput, MaskImageType)

            /** Set Darkening threshold input */
            // Only NOMINAL
            vnsMemberSetGetObjectMacro(DarkeningInput, RatioImageType)

            /** Get the altitude correction map output */
            vnsGetMacro(MapOfNbPixelsForEachLabel, NbPixelsForEachLabelMapType)

            vnsGetMacro(LabeledShadowZoneOutputImageList, LabelImageListPointer)
            // Only in NOMINAL mode
            vnsGetMacro(LabeledShadowZoneDarkenedThresholdOutputImageList, LabelImageListPointer)


            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            ComputesProjectionsMasksLabels();
            /** Destructor */
            virtual
            ~ComputesProjectionsMasksLabels();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ComputesProjectionsMasksLabels(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            bool
            LoadI3D(void);

            /** Filters declaration */
            typename ShadowMaskProjectionFilterPointerTypeList::Pointer m_ShadowMaskProjectionFilterList;

            /** Intersector 3D object declaration */
            Intersector3DType m_I3D;

            typename KeepsCloudLabelFilterPointerTypeList::Pointer m_KeepsCloudLabelFilterPointerTypeList;

            NbPixelsForEachLabelMapType m_MapOfNbPixelsForEachLabel;
            MaskFusionFilterPointerType                        m_CLDandDarkFusionFilter;
            LabelMaskingFilterListPointerType                  m_ShadowMaskCleaningFilterList;
            typename DarkeningThresholdFilterListType::Pointer m_DarkeningThresholdFilterList;

            LabelImageListPointer m_LabeledShadowZoneOutputImageList;
            // Only in NOMINAL mode
            LabelImageListPointer m_LabeledShadowZoneDarkenedThresholdOutputImageList;
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsComputesProjectionsMasksLabels.txx"
#endif

#endif /* __vnsComputesProjectionsMasksLabels_h */
