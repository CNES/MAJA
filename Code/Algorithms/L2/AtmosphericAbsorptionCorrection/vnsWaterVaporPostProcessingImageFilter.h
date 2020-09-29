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
 *                                        __  __    __     ____   __                                        *
 *                                       (  \/  )  /__\   (_  _) /__\                                       *
 *                                        )    (  /(__)\ .-_)(  /(__)\                                      *
 *                                       (_/\/\_)(__)(__)\____)(__)(__)                                     *
 *                                                                                                          *
 *                                                                                                          *
 ************************************************************************************************************
 *                                                                                                          *
 * Author: CS Systemes d'Information  (France)                                                              *
 *                                                                                                          *
 ************************************************************************************************************
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2368-CNES : 27 juillet 2017 : Seuillage WaterPP sur bande B8/B9      *
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-1754-CNES : 24 nov 2016 : Creation                                      *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$                                                                                                     *
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsWaterVaporPostProcessingImageFilter_h
#define __vnsWaterVaporPostProcessingImageFilter_h

#include <vnsMaskingImageFunctor.h>

#include "itkProcessObject.h"

#include "vnsMacro.h"
#include "vnsUtilities.h"
#include "otbObjectList.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "itkUnaryFunctorImageFilter.h"
#include "vnsSupOrEqualThresholdImageFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "vnsStreamingConditionalStatisticsImageFilter.h"
#include "vnsGapFillingImageFilter.h"
#include "vnsPadAndResampleImageFilter.h"
#include "vnsExpandThresholdImageFilter.h"
#include "vnsConstantImageSource.h"



namespace vns
{

    /** \class WaterVaporPostProcessingImageFilter
     * \brief This class implements a water vapor post-processing.
     *
     *
     */

    class WaterVaporPostProcessingImageFilter : public itk::ProcessObject
    {
    public:
        typedef WaterVaporPostProcessingImageFilter Self;
        typedef itk::ProcessObject                  Superclass;
        typedef itk::SmartPointer<Self>             Pointer;
        typedef itk::SmartPointer<const Self>       ConstPointer;

        /** Type macro */
        itkNewMacro(Self)

        /** Creation through object factory macro */
        itkTypeMacro(WaterVaporPostProcessingImageFilter, ProcessObject)

        /** Some convenient typedefs. */
        /** IPTOCR input image typedefs. */
        typedef VNSRealVectorImageType IPTOCRImageType;
        typedef otb::ObjectList<IPTOCRImageType> IPTOCRImageListType;
        typedef IPTOCRImageListType::ConstPointer IPTOCRImageListConstPointer;
        typedef IPTOCRImageType::ConstPointer IPTOCRImageConstPointerType;
        typedef IPTOCRImageType::RegionType RegionType;
        typedef IPTOCRImageType::PixelType IPTOCRImagePixelType;
        typedef IPTOCRImageType::InternalPixelType IPTOCRImageInternalPixelType;
        typedef IPTOCRImageType::SizeType SizeType;
        /** DTM input image typedefs. */
        typedef VNSDTMImageType DTMImageType;
        typedef otb::ObjectList<DTMImageType> DTMImageListType;
        typedef DTMImageListType::ConstPointer DTMImageListConstPointerType;
        typedef DTMImageType::ConstPointer DTMImageConstPointerType;
        typedef DTMImageType::PixelType DTMImagePixelType;
        typedef DTMImageType::InternalPixelType DTMImageInternalPixelType;
        /** Vapor amount image typedefs. */
        typedef VNSRealImageType VAPImageType;
        typedef otb::ObjectList<VAPImageType> VAPImageListType;
        typedef VAPImageListType::Pointer VAPImageListPointerType;
        typedef VAPImageType::Pointer VAPImagePointerType;
        typedef VAPImageType::ConstPointer VAPImageConstPointerType;
        typedef VAPImageType::PixelType VAPImagePixelType;
        typedef VAPImageType::InternalPixelType VAPImageInternalPixelType;
        /** CLD Mask typedefs. */
        typedef VNSMaskImageType MaskImageType;
        typedef otb::ObjectList<MaskImageType> MaskImageListType;
        typedef MaskImageListType::Pointer MaskImageListPointerType;
        typedef MaskImageType::Pointer MaskImagePointerType;
        typedef MaskImageType::ConstPointer MaskImageConstPointerType;
        typedef MaskImageType::RegionType MaskImageRegionType;
        typedef MaskImageType::PixelType MaskImagePixelType;
        typedef MaskImageType::InternalPixelType MaskImageInternalPixelType;

        /** Filters typedefs. */
        typedef otb::MultiToMonoChannelExtractROI<IPTOCRImageInternalPixelType, IPTOCRImageInternalPixelType> MultiToMonoChannelExtractFilterType;
        typedef MultiToMonoChannelExtractFilterType::Pointer MultiToMonoChannelExtractFilterPointerType;
        typedef MultiToMonoChannelExtractFilterType::OutputImageType MonoChannelImageType;
        typedef MonoChannelImageType::PixelType MonoChannelImagePixelType;

        typedef Functor::SupOrEqualThresholdImageFunctor<MonoChannelImagePixelType, MaskImageInternalPixelType> ThresholdFunctorType;
        typedef itk::UnaryFunctorImageFilter<MonoChannelImageType, MaskImageType, ThresholdFunctorType> ThresholdFilterType;
        typedef ThresholdFilterType::Pointer ThresholdFilterPointerType;

        typedef Functor::MaskingImageFunctor<MaskImagePixelType, MaskImagePixelType, MaskImagePixelType> ApplyMaskFunctorType;
        typedef itk::BinaryFunctorImageFilter<MaskImageType, MaskImageType, MaskImageType, ApplyMaskFunctorType> ApplyMaskFilterType;
        typedef ApplyMaskFilterType::Pointer ApplyMaskFilterPointerType;

        typedef itk::BinaryBallStructuringElement<MaskImagePixelType, MaskImageType::ImageDimension> StructuringElementType;
        typedef itk::BinaryDilateImageFilter<MaskImageType, MaskImageType, StructuringElementType> DilateFilterType;
        typedef DilateFilterType::Pointer DilateFilterPointerType;

        typedef Functor::MaskingImageFunctor<VAPImagePixelType, MaskImagePixelType, VAPImagePixelType> VAPMaskingFunctorType;
        typedef itk::BinaryFunctorImageFilter<VAPImageType, MaskImageType, VAPImageType, VAPMaskingFunctorType> VAPMaskingFilterType;
        typedef VAPMaskingFilterType::Pointer VAPMaskingFilterPointerType;

        typedef StreamingConditionalStatisticsImageFilter<VAPImageType> StreamingConditionalStatisticsImageFilterType;
        typedef StreamingConditionalStatisticsImageFilterType::Pointer StreamingConditionalStatisticsImageFilterPointerType;

        typedef GapFillingImageFilter<VAPImageType, VAPImageType, MaskImageType> GapFillingImageFilterType;
        typedef GapFillingImageFilterType::Pointer GapFillingImageFilterPointerType;

        /** Expand filter */
        typedef PadAndResampleImageFilter<VAPImageType, VAPImageType> PadAndResampleImageFilterType;
        typedef PadAndResampleImageFilterType::Pointer PadAndResampleImageFilterPointerType;
        typedef otb::ObjectList<PadAndResampleImageFilterType> PadAndResampleImageFilterListType;

        typedef ExpandThresholdImageFilter<MaskImageType, MaskImageType> ExpandThresholdMaskFilterType;
        typedef ExpandThresholdMaskFilterType::Pointer ExpandThresholdMaskFilterPointerType;
        typedef otb::ObjectList<ExpandThresholdMaskFilterType> ExpandThresholdMaskFilterListType;

        /** Constant image for VAP + Mask */
        typedef ConstantImageSource<VAPImageType> VAPImageConstantImageSourceFilterType;
        typedef VAPImageConstantImageSourceFilterType::Pointer VAPImageConstantImageSourceFilterPointerType;
        typedef otb::ObjectList<VAPImageConstantImageSourceFilterType> VAPImageConstantImageSourceFilterListType;

        typedef ConstantImageSource<MaskImageType> VAPMaskConstantImageSourceFilterType;
        typedef VAPMaskConstantImageSourceFilterType::Pointer VAPMaskConstantImageSourceFilterPointerType;
        typedef otb::ObjectList<VAPMaskConstantImageSourceFilterType> VAPMaskConstantImageSourceFilterListType;


        /** Set the input VAP image at L2Coarse */
        vnsSetGetInputMacro(VAPSub, VAPImageType, 0 )
        /** Set Cloud Mask at L2Coarse */
        vnsSetGetInputMacro(CLDSub , MaskImageType, 1 )
        /** Set surface reflectance image at L2Coarse */
        vnsSetGetInputMacro(IPTOCR, IPTOCRImageType, 2 )
        /** Set the DTM image(s) at L2 resolution(s) */
        vnsSetGetInputMacro(DTM, DTMImageListType, 3 )

        /** Get the post-processed VAP outputs at L2 resolution */
        itkGetObjectMacro(L2VAPImageListOutput, VAPImageListType)
        itkGetConstObjectMacro(L2VAPImageListOutput, VAPImageListType)
        /** Get the VAP filled value mask */
        itkGetObjectMacro(L2VAPFillMaskListOutput, MaskImageListType)
        itkGetConstObjectMacro(L2VAPFillMaskListOutput, MaskImageListType)

        /** Get the post-processed VAP outputs at L2 resolution */
        itkGetObjectMacro(VAPImageSubOutput, VAPImageType)
        itkGetConstObjectMacro(VAPImageSubOutput, VAPImageType)
        /** Get the VAP filled value mask */
        itkGetObjectMacro(VAPFillMaskSubOutput, MaskImageType)
        itkGetConstObjectMacro(VAPFillMaskSubOutput, MaskImageType)



        /**  Set the no data value. */
        itkSetMacro(NoData, RealNoDataType)
        /**  Set the default VAP value. */
        itkSetMacro(DefaultWaterVapor, VAPImagePixelType)
        /** Water amount default value boolean */
        itkSetMacro(UseDefaultConstantWaterAmount, bool)
        /**  Set the water vapor reference band code. */
        itkSetMacro(WaterVaporReferenceBandCode, unsigned int)
        /**  Set the water vapor reflectance threshold. */
        itkSetMacro(WaterVaporReflectanceThresholdRef, double)
		/**  Set the water vapor band code. */
		itkSetMacro(WaterVaporBandCode, unsigned int)
		/**  Set the water vapor reflectance threshold. */
		itkSetMacro(WaterVaporReflectanceThreshold, double)
        /**  Set the init window radius. */
        itkSetMacro(WaterVaporInitWindowRadius, unsigned int)
        /**  Set the max window radius. */
        itkSetMacro(WaterVaporMaxWindowRadius, unsigned int)
        /**  Set the dilatation structuring element radius. */
        itkSetMacro(WaterVaporSEValidRadius, unsigned short)

        /** Main computation method. */
        virtual void
        UpdateWithCaching(void);

    protected:
        /** Constructor */
        WaterVaporPostProcessingImageFilter();
        /** Destructor */
        virtual
        ~WaterVaporPostProcessingImageFilter();
        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
        WaterVaporPostProcessingImageFilter(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented

        /** NoData. */
        RealNoDataType m_NoData;
        /** DefaultWaterVapor */
        VAPImagePixelType m_DefaultWaterVapor;
        /** UseWaterAmountValue */
        bool m_UseDefaultConstantWaterAmount;
        /**  WaterVaporReferenceBandCode. */
        unsigned int m_WaterVaporReferenceBandCode;
        /**  WaterVaporReflectanceThresholdRef. */
        double m_WaterVaporReflectanceThresholdRef;
        /**  WaterVaporBandCode. */
        unsigned int m_WaterVaporBandCode;
        /**  WaterVaporReflectanceThreshold. */
        double m_WaterVaporReflectanceThreshold;
        /**  WaterVaporInitWindowRadius. */
        unsigned int m_WaterVaporInitWindowRadius;
        /**  WaterVaporMaxWindowRadius. */
        unsigned int m_WaterVaporMaxWindowRadius;
        /**  WaterVaporSEValidRadius. */
        unsigned int m_WaterVaporSEValidRadius;

        // Filter declaration
        MultiToMonoChannelExtractFilterPointerType m_MultiToMonoChannelExtractFilterRef;
        ThresholdFilterPointerType m_ThresholdFilterRef;
        MultiToMonoChannelExtractFilterPointerType m_MultiToMonoChannelExtractFilter;
        ThresholdFilterPointerType m_ThresholdFilter;
        ApplyMaskFilterPointerType m_VapourApplyMaskFilter;
        ApplyMaskFilterPointerType m_CLDSumApplyMaskFilter;
        DilateFilterPointerType m_DilateFilter;
        VAPMaskingFilterPointerType m_VAPMaskingFilter;
        StreamingConditionalStatisticsImageFilterPointerType m_StreamingConditionalStatisticsImageFilter;
        GapFillingImageFilterPointerType m_GapFillingImageFilter;
        PadAndResampleImageFilterListType::Pointer m_ExpandList;
        ExpandThresholdMaskFilterListType::Pointer m_ExpandVAPFillMaskList;
        VAPImageConstantImageSourceFilterListType::Pointer m_WaterAmountValueConstantImageFilterList;
        VAPMaskConstantImageSourceFilterListType::Pointer  m_WaterAmountValueConstantMaskFilterList;

        // Outputs
        VAPImageListPointerType  m_L2VAPImageListOutput;
        MaskImageListPointerType m_L2VAPFillMaskListOutput;
        VAPImagePointerType      m_VAPImageSubOutput;
        MaskImagePointerType     m_VAPFillMaskSubOutput;
    };


}// end namespace vns

#endif /* __vnsWaterVaporPostProcessingImageFilter_h */
