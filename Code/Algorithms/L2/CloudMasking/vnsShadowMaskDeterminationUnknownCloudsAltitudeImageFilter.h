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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 07 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter_h
#define __vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter_h

#include "itkProcessObject.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkTernaryFunctorImageFilter.h"
#include "itkConnectedComponentImageFilter.h"

#include "vnsTernaryLogicalOrFunctor.h"

#include "vnsValueCountImageFilter.h"
#include "vnsConditionalRatioImageFilter.h"
#include "vnsTernaryLogicalOrFunctor.h"
#include "vnsStreamingThresholdFromLabelImageFilter.h"
#include "otbStreamingMinMaxImageFilter.h"
#include "vnsShadowVariationThresholdValueEstimator.h"
#include "vnsStreamingThresholdFromLabelImageFilter.h"
#include "vnsCloudGeomMasksToKeptLabelMasksFilter.h"
#include "vnsImageListToImageListApplyMaskWithFunctorFilter.h"
#include "vnsShadowFinalPixelDetectionFilter.h"
#include "vnsComputesProjectionsMasksLabels.h"
#include "vnsFlattenCloudMaskImageListFilter.h"
#include "otbObjectList.h"
#include "otbImageList.h"

#include "vnsMacro.h"

#include "otbImage.h"
#include "otbImageListToVectorImageFilter.h"
#include "vnsStreamingStatisticsMapFromLabelImageFilter.h"

#include "vnsDEMDataManager.h"

namespace vns
{
    /** \class  ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter
     * \brief This class generates the shadow mask.
     *
     * The location of cloud is unknown so the projection is done for a range of altitude.
     * Shadow is then detected inside the identified potential shadow zones.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        class ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

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

            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            /** Filters typedefs */
            typedef ValueCountImageFilter<InputMaskType>       ValueCountFilterType;
            typedef typename ValueCountFilterType::Pointer     ValueCountFilterPointerType;

            typedef ValueCountImageFilter<VNSRealImageType>       ValueCountFilterInputImageType;

            typedef otb::Image<double, 2> DarkeningImageType;
            typedef typename DarkeningImageType::Pointer DarkeningImagePointerType;
            typedef ConditionalRatioImageFilter<InputImageType, InputMaskType, DarkeningImageType> DarkeningFilterType;
            typedef typename DarkeningFilterType::Pointer DarkeningFilterPointerType;
            typedef otb::ImageFileReader<DarkeningImageType> ReaderDarkeningImageType;
            typedef typename ReaderDarkeningImageType::Pointer ReaderDarkeningPointer;
            typedef otb::StreamingMinMaxImageFilter<DarkeningImageType>   MinMaxFilterType;
            typedef typename MinMaxFilterType::Pointer MinMaxFilterPointerType;
            typedef otb::ImageList<DarkeningImageType> DarkeningImageListType;
            typedef typename DarkeningImageListType::Pointer DarkeningImageListPointerType;
            typedef otb::ImageListToVectorImageFilter<DarkeningImageListType, InputImageType> DarkeningImageListToVectorImageFilterType;
            typedef typename DarkeningImageListToVectorImageFilterType::Pointer DarkeningImageListToVectorImageFilterPointerType;
            typedef ShadowVariationThresholdValueEstimator<InputImageType> ShadowVariationThresholdValueEstimatorFilterType;
            typedef typename ShadowVariationThresholdValueEstimatorFilterType::Pointer ShadowVariationThresholdValueEstimatorFilterPointerType;

            typedef ComputesProjectionsMasksLabels<DarkeningImageType, LabelImageType, InputMaskType, GRIDImageType, CLAImageType, DTMImageType> ComputesProjectionsMasksLabelsType;
            typedef typename ComputesProjectionsMasksLabelsType::Pointer ComputesProjectionsMasksLabelsPointer;
            typedef itk::ConnectedComponentImageFilter<InputMaskType, LabelImageType> LabelFilterType;
            typedef typename LabelFilterType::Pointer LabelPointerFilterType;
            typedef otb::ImageFileReader<LabelImageType> ReaderLabelType;
            typedef typename ReaderLabelType::Pointer ReaderLabelPointer;

            typedef Functor::MaskingImageFunctor<LabelImagePixelType, InputMaskPixelType, LabelImagePixelType> LabelMaskingFunctorType;
            typedef itk::BinaryFunctorImageFilter<LabelImageType, InputMaskType, LabelImageType, LabelMaskingFunctorType> LabelMaskingFilterType;
            typedef typename LabelMaskingFilterType::Pointer LabelMaskingFilterPointerType;

            typedef StreamingStatisticsMapFromLabelImageFilter<LabelImageType>
            StreamingStatisticsMapFromLabelImageFilterType;
            typedef typename StreamingStatisticsMapFromLabelImageFilterType::Pointer
                    StreamingStatisticsMapFromLabelImageFilterPointerType;

            typedef CloudGeomMasksToKeptLabelMasksFilter<LabelImageType, InputMaskType> ByLabelCloudShadowFilterType;
            typedef typename ByLabelCloudShadowFilterType::Pointer                      ByLabelCloudShadowFilterPointerType;
            typedef typename ByLabelCloudShadowFilterType::LabelImagePixelSetType       LabelImagePixelSetType;

            typedef otb::ImageList<LabelImageType> LabelImageListType;
            typedef vns::ImageListToImageListApplyMaskWithFunctorFilter<LabelImageListType, InputMaskType, LabelImageListType, LabelMaskingFunctorType > LabelImageListMaskingFilterType;
            typedef typename LabelImageListMaskingFilterType::Pointer LabelImageListMaskingFilterPointerType;
            typedef ValueCountImageFilter<LabelImageType>      LabelCountFilterType;
            typedef typename LabelCountFilterType::Pointer     LabelCountFilterPointerType;
            typedef otb::ObjectList<LabelCountFilterType>      LabelCountFilterListType;
            typedef typename LabelCountFilterListType::Pointer LabelCountFilterListPointerType;

            typedef ShadowFinalPixelDetectionFilter<DarkeningImageType, LabelImageType, InputMaskType> ShadowFinalPixelDetectionFilterType;
            typedef typename ShadowFinalPixelDetectionFilterType::Pointer ShadowFinalPixelDetectionFilterPointerType;
            typedef FlattenCloudMaskImageListFilter<InputMaskType, InputMaskType> MaskListFlatteningFilterType;
            typedef typename MaskListFlatteningFilterType::Pointer MaskListFlatteningFilterPointerType;
            typedef Functor::TernaryLogicalOrFunctor<InputMaskPixelType, InputMaskPixelType, InputMaskPixelType, InputMaskPixelType> TernaryLogicalFunctorType;
            typedef itk::TernaryFunctorImageFilter<InputMaskType, InputMaskType, InputMaskType, InputMaskType, TernaryLogicalFunctorType> TernaryLogicalFilterType;
            typedef typename TernaryLogicalFilterType::Pointer TernaryLogicalPointerFilterType;
            typedef ConstantImageSource<InputMaskType>   ConstantFilterType;
            typedef typename ConstantFilterType::Pointer ConstantPointerFilterType;


            typedef otb::ImageFileReader<InputMaskType> ReaderMaskType;
            typedef typename ReaderMaskType::Pointer ReaderMaskPointer;

            vnsMemberAndSetAndGetConstReferenceMacro(ShadowBandTOCR, unsigned int);
            vnsMemberAndSetAndGetConstReferenceMacro(ShadowBandRCR, unsigned int);

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
            vnsMemberAndSetAndGetConstReferenceMacro(SOLHRef, double)

            // Reference altitude for vie direction accessors
            vnsMemberAndSetAndGetConstReferenceMacro(VIEHRef, double)

//            vnsMemberAndSetAndGetConstReferenceMacro(DiscardedPixelsRatioMin,unsigned short)
            // CloudCoverMultFactor parameter
            vnsMemberAndSetAndGetConstReferenceMacro(CloudCoverMultFactor, double)
            // ShadowCloudRatioMax parameter
            vnsMemberAndSetAndGetConstReferenceMacro(ShadowCloudRatioMax, double)
            // ShadowCloudRatioMin parameter
            vnsMemberAndSetAndGetConstReferenceMacro(ShadowCloudRatioMin, double)
            // ShadowCloudRatioPixNb parameter
            vnsMemberAndSetAndGetConstReferenceMacro(ShadowCloudRatioPixNb,unsigned short)
            // ProjShadDarkeningMax parameter
            vnsMemberAndSetAndGetConstReferenceMacro(ProjShadDarkeningMax, double)
            // DarkeningMinPercentile parameter
            vnsMemberAndSetAndGetConstReferenceMacro(DarkeningMinPercentile,double)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsMemberSetGetObjectMacro(IPTOCRInput, InputImageType)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsMemberSetGetObjectMacro(L2inRCRInput, InputImageType)

            /** Set the edge mask of the input product */
            vnsMemberSetGetObjectMacro(IPEDGInput, InputMaskType)

            /** Set the no_data mask of the composite product */
            vnsMemberSetGetObjectMacro(L2inNDTInput, InputMaskType)

            /** Set the water mask of the composite product */
            vnsMemberSetGetObjectMacro(L2inWASInput, InputMaskType)

            /** Set the image of the cloud mask */
            vnsMemberSetGetObjectMacro(L2CLDSubInput, InputMaskType)

            /** Set the image of the cloud mask */
            vnsMemberSetGetObjectMacro(L2CLDDilatedSubInput, InputMaskType)

            /** Set the viewing direction grid */
            vnsMemberSetGetObjectMacro(IPVIEInput, GRIDImageType)

            /** Set the solar direction grid for the first reference altitude */
            vnsMemberSetGetObjectMacro(IPSOLInput, GRIDImageType)

            /** Set the cloud altitude image */
            vnsMemberSetGetObjectMacro(IPCLAInput, CLAImageType)

            /** Set the DTM image */
            vnsMemberSetGetObjectMacro(DTMInput, DTMImageType)

            /** Get the IPCLDShad output mask */
            vnsMemberGetObjectMacro(IPCLDShadOutput, OutputImageType)

            /** Get the IPCLA output image */
            vnsMemberGetObjectMacro(IPCLAOutput, CLAImageType)

            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter();
            /** Destructor */
            virtual
            ~ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            DarkeningFilterPointerType    m_DarkeningFilter;
            ReaderDarkeningPointer        m_DarkeningFilterOutputReader;
            MinMaxFilterPointerType       m_DarkeningMinMaxFilter;
            DarkeningImageListPointerType m_DarkeningImageList;
            DarkeningImageListToVectorImageFilterPointerType  m_DarkeningImageListToVectorImage;
            ShadowVariationThresholdValueEstimatorFilterPointerType m_ShadowThresholdValueEstimatorFilter;

            LabelPointerFilterType        m_DilatedCloudMaskAllLabelFilter;
            LabelMaskingFilterPointerType m_LabelMaskingFilter;
            StreamingStatisticsMapFromLabelImageFilterPointerType m_ComputeCLDPixNbMap;

            ComputesProjectionsMasksLabelsPointer        m_ComputesProjectionsMasksLabelsFilter;
            LabelCountFilterListPointerType              m_LabelCountFilterList_ShadByLab;
            std::map<LabelImagePixelType, unsigned long> m_ShadowByLabPixelsNumberMap;
            ByLabelCloudShadowFilterPointerType          m_ShadowGeomMaskByRatioFilter;
            LabelImageListMaskingFilterPointerType       m_ShadowRadiomMaskFilter;
            LabelCountFilterListPointerType              m_LabelCountFilterList_ShadByLab_Updated;
            std::map<LabelImagePixelType, unsigned long> m_ShadowByLabPixelsNumberMap_Updated;
            ByLabelCloudShadowFilterPointerType          m_ShadowRadiomMaskByRatioFilter;
            ShadowFinalPixelDetectionFilterPointerType   m_ShadowFinalPixelDetection;
            MaskListFlatteningFilterPointerType          m_FlatteningFilter;

            TernaryLogicalPointerFilterType m_ShadowLogicalSummaryFilter;
            ConstantPointerFilterType m_IPCLDShadConstantFilter;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter.txx"
#endif

#endif /* __vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter_h */
