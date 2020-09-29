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
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2298-CNES : 27 juillet 2017 : Prise en compte masque extension       *
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-1991-CNES : 29 novembre 2016 : Update CloudMasking ATCOR              *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1666-CNES : 3 aout 2016 : Suppression parametre 'Min_Shadow_Variation'*
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 25 octobre 2011 : Modification pour prise en compte        *
 *                                                            d'evolutions liees a MACCS                    *
 *                                                                                                          *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution                *
 * VERSION : 1-0-0-3 : DM : 357 : 25 juil. 2011 : Add the dilatation of water mask for the sub method         *
 *                                               Reflectance variation correlation                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 2 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskingImageFilter_h
#define __vnsCloudMaskingImageFilter_h

#include "vnsChangeValueWithMaskVectorFunctor.h"
#include "itkProcessObject.h"
#include "itkTernaryFunctorImageFilter.h"
#include "itkNaryFunctorImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkOrImageFilter.h"

#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbImageFileReader.h"
#include "otbQuaternaryFunctorImageFilter.h"

#include "vnsMacro.h"
#include "vnsReflectanceThresholdFunctor.h"
#include "vnsReflectanceVariationCorrelationImageFilter.h"
#include "vnsCloudMaskExtensionCorrelationGenerator.h"
#include "vnsStreamingThresholdFromLabelImageFilter.h"
#include "vnsValidateMaskFromLabelMapAndValidityMapImageFilter.h"
#include "vnsSnowMaskCorrectionImageFilter.h"
#include "vnsConstantImageSource.h"
#include "vnsTernaryLogicalOrFunctor.h"
#include "vnsNaryLogicalOrFunctor.h"
#include "vnsStereoCorrectionImageFilter.h"
#include "vnsShadowMaskDeterminationImageFilter.h"
#include "vnsSolarDirectionForCloudAltitudeFunctor.h"
#include "vnsOutsideCloudsShadowMaskGenerator.h"
#include "vnsConditionalRatioImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "vnsShadowVariationThresholdValueEstimator.h"
#include "vnsShadowVariationThresholdImageFilter.h"
#include "vnsShadowVariationPixelValidationImageFilter.h"
#include "vnsShadowVariationCorrelationImageFilter.h"
#include "otbStreamingStatisticsMapFromLabelImageFilter.h"
#include "vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter.h"

#include "vnsUtilities.h"
#include "vnsComputesProjectionsMasksLabels.h"

namespace vns
{
    /** \class  CloudMaskingImageFilter
     * \brief This class is used to detect clouds and cloud shadows within the images.
     * Few criteria are used to detect clouds:
     * \begin{itemize}
     *      \item the stereoscopic mask detects most clouds except thin or very low clouds
     *      \item a threshold on surface reflectance in the blue detect thick clouds
     *      (the saturated pixels are assimilated to clouds with this test)
     *      \item intense variations of surface reflectances still in the blue band.
     *      This filter tests also if there no surface change (by comparing the relative
     *      reflectances in he NIR and in the blue or in the red and in the blue) or
     *      if the correlation with the composite product or with the stack of images is
     *      not good.
     * \end{itemize}
     * Once clouds are detected, cloud shadows are detected taking into account the
     * cloud altitude when it is known otherwise an a priori cloud altitude.
     * Finally, the cloud and shadow masks are dilated since cloud edges are difficult
     * to detect.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
//    template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage, class TGRIDImage, class TCLAImage, class TDTMImage,
//            class TOutputImage>
        class CloudMaskingImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef CloudMaskingImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(CloudMaskingImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            //typedef TInputImage InputImageType;
            typedef VNSRealVectorImageType InputImageType;
            typedef InputImageType::ConstPointer InputImageConstPointer;
            typedef InputImageType::Pointer InputImagePointer;
            typedef InputImageType::RegionType RegionType;
            typedef InputImageType::PixelType InputImagePixelType;
            typedef InputImageType::SizeType SizeType;
            typedef InputImageType::InternalPixelType InputInternalPixelType;

            //typedef TInputMask InputMaskType;
            typedef VNSMaskImageType InputMaskType;
            typedef InputMaskType::Pointer InputMaskPointer;
            typedef InputMaskType::ConstPointer InputMaskConstPointer;
            typedef InputMaskType::PixelType InputMaskPixelType;

            //typedef TVectorMask VectorMaskType;
            typedef VNSMaskVectorImageType VectorMaskType;
            typedef VectorMaskType::Pointer VectorMaskPointer;
            typedef VectorMaskType::ConstPointer VectorMaskConstPointer;
            typedef VectorMaskType::PixelType VectorMaskPixelType;
            typedef VectorMaskType::InternalPixelType VectorMaskInternalPixelType;

            //typedef TDateImage DateImageType;
            typedef VNSIntegerImageType DateImageType;
            typedef DateImageType::Pointer DateImagePointer;
            typedef DateImageType::ConstPointer DateImageConstPointer;
            typedef DateImageType::PixelType DateImagePixelType;

            //typedef TGRIDImage GRIDImageType;
            typedef VNSRealVectorImageType GRIDImageType;
            typedef GRIDImageType::Pointer GRIDImagePointer;
            typedef GRIDImageType::ConstPointer GRIDImageConstPointer;
            typedef GRIDImageType::PixelType GRIDImagePixelType;
            typedef GRIDImageType::IndexType IndexType;
            typedef GRIDImageType::PointType PointType;
            typedef GRIDImageType::SpacingType SpacingType;

            //typedef TCLAImage CLAImageType;
            typedef VNSRealImageType CLAImageType;
            typedef CLAImageType::Pointer CLAImagePointer;
            typedef CLAImageType::ConstPointer CLAImageConstPointer;
            typedef CLAImageType::PixelType CLAImagePixelType;

            //typedef TDTMImage DTMImageType;
            typedef VNSDTMImageType DTMImageType;
            typedef DTMImageType::Pointer DTMImagePointer;
            typedef DTMImageType::ConstPointer DTMImageConstPointer;
            typedef DTMImageType::PixelType DTMImagePixelType;

            //typedef TOutputImage OutputImageType;
            typedef VNSMaskImageType OutputImageType;
            typedef OutputImageType::Pointer OutputImagePointer;
            typedef OutputImageType::RegionType OutputImageRegionType;
            typedef OutputImageType::PixelType OutputImagePixelType;
            typedef OutputImageType::InternalPixelType OutputImageInternalPixelType;

            typedef itk::Size<InputImageType::ImageDimension> RadiusSizeType;

            /** Filter Types */
            typedef itk::OrImageFilter<InputMaskType, InputMaskType, InputMaskType> OrFilterType;
            typedef OrFilterType::Pointer OrFilterPointerType;

            // Water mask erosion
            typedef itk::BinaryBallStructuringElement<InputMaskPixelType, InputMaskType::ImageDimension> StructuringElementType;
            typedef itk::BinaryErodeImageFilter<InputMaskType, InputMaskType, StructuringElementType> ErodeFilterType;
            typedef ErodeFilterType::Pointer ErodePointerFilterType;

            // Reflectance threshold type
            typedef Functor::ReflectanceThresholdFunctor<InputImagePixelType, VectorMaskPixelType, InputMaskPixelType, InputMaskPixelType> ReflectanceThresholdFunctorType;
            typedef otb::QuaternaryFunctorImageFilter<InputImageType, VectorMaskType, InputMaskType, InputMaskType, InputMaskType,
                    ReflectanceThresholdFunctorType> ReflectanceThresholdFilterType;
            typedef ReflectanceThresholdFilterType::Pointer ReflectanceThresholdPointerFilterType;

            // Reflectance variation threshold type
            typedef ReflectanceVariationCorrelationImageFilter<InputImageType, InputMaskType, DateImageType, InputMaskType> ReflectanceVariationCorrelationFilterType;
            typedef ReflectanceVariationCorrelationFilterType::Pointer ReflectanceVariationCorrelationPointerFilterType;

            // Label filter type
            typedef unsigned short LabelPixelType;
            typedef otb::Image<LabelPixelType, InputImageType::ImageDimension> LabelImageType;
            typedef LabelImageType::Pointer LabelImagePointer;
            typedef LabelImageType::PixelType LabelImagePixelType;

            typedef itk::ConnectedComponentImageFilter<InputMaskType, LabelImageType> LabelFilterType;
            typedef LabelFilterType::Pointer LabelPointerFilterType;

            // Extension of clouds filter type
            typedef CloudMaskExtensionCorrelationGenerator<InputImageType, InputMaskType, LabelImageType, InputMaskType> CloudMaskExtensionFilterType;
            typedef CloudMaskExtensionFilterType::Pointer CloudMaskExtensionPointerFilterType;
            // Correlation discarded pixel post filtering
            typedef StreamingThresholdFromLabelImageFilter<InputMaskType, LabelImageType> DiscardedCloudPixelFilterType;
            typedef DiscardedCloudPixelFilterType::Pointer DiscardedCloudPixelFilterPointerType;

            // Extension of cloud PostProcessing
            typedef ValidateMaskFromLabelMapAndValidityMapImageFilter<InputMaskType, LabelImageType, InputMaskType> ValidateMaskFromLabelMapAndValidityMapImageFilterType;
            typedef ValidateMaskFromLabelMapAndValidityMapImageFilterType::Pointer ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType;

            // Snow mask correction image filter
            typedef SnowMaskCorrectionImageFilter<InputMaskType> SnowMaskCorrectionFilterType;
            typedef SnowMaskCorrectionFilterType::Pointer SnowMaskCorrectionPointerFilterType;

            // Filters used to generate images constants
            typedef ConstantImageSource<InputMaskType> ConstantFilterType;
            typedef ConstantFilterType::Pointer ConstantPointerFilterType;
            typedef ConstantImageSource<CLAImageType> CLAConstantFilterType;
            typedef CLAConstantFilterType::Pointer CLAConstantPointerFilterType;
//            typedef itk::BinaryThresholdImageFilter<LabelImageType, LabelImageType> LabelConstantFilterType;
//            typedef LabelConstantFilterType::Pointer LabelConstantPointerFilterType;

            // Ternary logical or funtor type
            typedef Functor::TernaryLogicalOrFunctor<InputMaskPixelType, InputMaskPixelType, InputMaskPixelType, InputMaskPixelType> TernaryLogicalFunctorType;

            typedef itk::TernaryFunctorImageFilter<InputMaskType, InputMaskType, InputMaskType, InputMaskType, TernaryLogicalFunctorType> TernaryLogicalFilterType;

            typedef TernaryLogicalFilterType::Pointer TernaryLogicalPointerFilterType;

            // Nary logical or funtor type
            typedef Functor::NaryLogicalOrFunctor<InputMaskPixelType, InputMaskPixelType> NaryLogicalFunctorType;

            typedef itk::NaryFunctorImageFilter<InputMaskType, InputMaskType, NaryLogicalFunctorType> NaryLogicalFilterType;

            typedef NaryLogicalFilterType::Pointer NaryLogicalPointerFilterType;


            // Stereoscopic altitude correction
            typedef StereoCorrectionImageFilter<CLAImageType, InputMaskType, LabelImageType, CLAImageType> StereoCorrectionFilterType;
            typedef StereoCorrectionFilterType::Pointer StereoCorrectionPointerFilterType;

            // Shadow mask determination
            typedef ShadowMaskDeterminationImageFilter<InputImageType, InputMaskType, LabelImageType, GRIDImageType, CLAImageType,
                    DTMImageType, InputMaskType> ShadowMaskDeterminationFilterType;
            typedef ShadowMaskDeterminationFilterType::Pointer ShadowMaskDeterminationPointerFilterType;

            // DM982

            typedef ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter<InputImageType, InputMaskType, LabelImageType,
                    GRIDImageType, CLAImageType, DTMImageType, InputMaskType>                   ShadowMaskDeterminationWithoutAltitudeFilterType;
            typedef ShadowMaskDeterminationWithoutAltitudeFilterType::Pointer          ShadowMaskDeterminationWithoutAltitudePointerFilterType;

            typedef ValueCountImageFilter<InputMaskType> ValueCountFilterType;
            typedef ValueCountFilterType::Pointer ValueCountFilterPointerType;
            typedef otb::Image<double, 2> DarkeningImageType;
            typedef DarkeningImageType::Pointer DarkeningImagePointerType;
            typedef ConditionalRatioImageFilter<InputImageType, InputMaskType, DarkeningImageType> DarkeningFilterType;
            typedef DarkeningFilterType::Pointer DarkeningFilterPointerType;
            typedef otb::ImageFileReader<DarkeningImageType> ReaderDarkeningImageType;
            typedef ReaderDarkeningImageType::Pointer ReaderDarkeningPointer;
            typedef otb::ImageList<DarkeningImageType> DarkeningImageListType;
            typedef DarkeningImageListType::Pointer DarkeningImageListPointerType;
            typedef otb::ImageListToVectorImageFilter<DarkeningImageListType, InputImageType> DarkeningImageListToVectorImageFilterType;
            typedef DarkeningImageListToVectorImageFilterType::Pointer DarkeningImageListToVectorImageFilterPointerType;
            typedef vns::Functor::MaskingImageFunctor<LabelImagePixelType, InputMaskPixelType, LabelImagePixelType> LabelMaskingFunctorType;
            typedef itk::BinaryFunctorImageFilter<InputMaskType, LabelImageType, InputMaskType, LabelMaskingFunctorType> LabelMaskingFilterType;
            typedef LabelMaskingFilterType::Pointer LabelMaskingFilterPointerType;
            typedef otb::StreamingStatisticsMapFromLabelImageFilter<InputImageType, LabelImageType>
                  StreamingStatisticsMapFromLabelImageFilterType;
              typedef StreamingStatisticsMapFromLabelImageFilterType::Pointer
                  StreamingStatisticsMapFromLabelImageFilterPointerType;


            // Generate the solar direction for cloud max altitude
            typedef Functor::SolarDirectionForCloudAltitudeFunctor<InputImagePixelType, InputImagePixelType, CLAImagePixelType,
                    InputImagePixelType> SolarFunctorType;
            typedef itk::TernaryFunctorImageFilter<InputImageType, InputImageType, CLAImageType, InputImageType, SolarFunctorType> SolarFilterType;
            typedef SolarFilterType::Pointer SolarFilterPointerType;

            // Shadow from clouds outside the image
            typedef OutsideCloudsShadowMaskGenerator<InputMaskType, DTMImageType, GRIDImageType, InputMaskType> OutsideCloudsShadowMaskFilterType;
            typedef OutsideCloudsShadowMaskFilterType::Pointer OutsideCloudsShadowMaskPointerFilterType;

            // Shadow variation threshold
            typedef otb::Image<double, 2> RatioImageType;
            typedef RatioImageType::Pointer RatioImagePointer;
            typedef ConditionalRatioImageFilter<InputImageType, InputMaskType, RatioImageType> RatioFilterType;
            typedef RatioFilterType::Pointer RatioFilterPointerType;

            typedef Functor::MaskingImageFunctor<InputMaskPixelType, InputMaskPixelType, InputMaskPixelType> MaskMaskingFunctorType;
            typedef itk::BinaryFunctorImageFilter<InputMaskType, InputMaskType, InputMaskType, MaskMaskingFunctorType> MaskMaskingFilterType;
            typedef MaskMaskingFilterType::Pointer MaskMaskingFilterPointerType;

            typedef Functor::MaskingImageFunctor<double, InputMaskPixelType, double> RatioMaskingFunctorType;
            typedef itk::BinaryFunctorImageFilter<RatioImageType, InputMaskType, RatioImageType, RatioMaskingFunctorType> ReflRatioMaskingFilterType;
            typedef ReflRatioMaskingFilterType::Pointer ReflRatioMaskingFilterPointerType;
            typedef otb::ImageFileReader<RatioImageType> ReaderRatioImageType;
            typedef ReaderRatioImageType::Pointer ReaderRatioPointer;
            typedef otb::ImageToVectorImageCastFilter<RatioImageType, InputImageType> RatioImageListToVectorImageFilterType;

            typedef RatioImageListToVectorImageFilterType::Pointer RatioImageListToVectorImageFilterPointerType;
            typedef ShadowVariationThresholdValueEstimator<InputImageType> ShadowVariationThresholdValueEstimatorFilterType;
            typedef ShadowVariationThresholdValueEstimatorFilterType::Pointer ShadowVariationThresholdValueEstimatorFilterPointerType;
            typedef ShadowVariationThresholdImageFilter<RatioImageType, InputMaskType, InputMaskType> ShadowVariationThresholdFilterType;
            typedef ShadowVariationThresholdFilterType::Pointer ShadowVariationThresholdPointerFilterType;
            typedef ShadowVariationPixelValidationImageFilter<InputMaskType, LabelImageType> ShadowVariationPixelValidationImageFilterType;
            typedef ShadowVariationPixelValidationImageFilterType::Pointer ShadowVariationPixelValidationImageFilterPointerType;

            typedef ShadowVariationCorrelationImageFilter<InputImageType, InputMaskType, InputMaskType> ShadowVariationCorrelationImageFilterType;
            typedef ShadowVariationCorrelationImageFilterType::Pointer ShadowVariationCorrelationImageFilterPointerType;

            // Dilatation of all cloud masks
            typedef itk::BinaryDilateImageFilter<InputMaskType, InputMaskType, StructuringElementType> DilateFilterType;
            typedef DilateFilterType::Pointer DilatePointerFilterType;

            // Concatenation of each cloud mask
            typedef otb::ImageList<InputMaskType> ImageListType;
            typedef otb::ImageListToVectorImageFilter<ImageListType, VectorMaskType> ImageListToVectorImageFilterType;

            // Apply the edge mask to the output masks
            typedef Functor::MaskingImageFunctor<InputMaskPixelType, InputMaskPixelType, InputMaskPixelType> FunctorType;

            typedef itk::BinaryFunctorImageFilter<InputMaskType, InputMaskType, InputMaskType, FunctorType> ApplyMaskFilterType;
            typedef ApplyMaskFilterType::Pointer ApplyMaskFilterPointer;

            typedef Functor::ChangeValueWithMaskVectorFunctor<VectorMaskPixelType, InputMaskPixelType, VectorMaskPixelType>
                    ChangeValueVectorFunctorType;
            typedef itk::BinaryFunctorImageFilter<VectorMaskType, InputMaskType, VectorMaskType,
                    ChangeValueVectorFunctorType> ChangeValueWithMaskVectorImageFilterType;
            typedef ChangeValueWithMaskVectorImageFilterType::Pointer ChangeValueWithMaskVectorImageFilterPointer;


//            typedef ComputesProjectionsMasksLabels<RatioImageType, LabelImageType, GRIDImageType, CLAImageType, DTMImageType> ComputesProjectionsMasksLabelsType;
//            typedef ComputesProjectionsMasksLabelsType::Pointer ComputesProjectionsMasksLabelsPointer;


            // Readers for caching
            typedef otb::ImageFileReader<CLAImageType> ReaderCLAType;
            typedef ReaderCLAType::Pointer ReaderCLAPointer;
            typedef otb::ImageFileReader<VectorMaskType> ReaderCLDVectType;
            typedef ReaderCLDVectType::Pointer ReaderCLDVectPointer;
            typedef otb::ImageFileReader<InputMaskType> ReaderMaskType;
            typedef ReaderMaskType::Pointer ReaderMaskPointer;
            typedef otb::ImageFileReader<LabelImageType> ReaderLabelType;
            typedef ReaderLabelType::Pointer ReaderLabelPointer;

            // Accessors definitions
            // No_data pixel value accessors
            itkSetMacro(NoData, RealNoDataType)

            // L2 Coarse Resolution accessors
            itkSetMacro(L2CoarseResolution, unsigned short)

            // Init mode parameter accessors
            itkSetMacro(InitMode, bool)

            // Blue band index accessors for TOCR
            itkSetMacro(BlueBandTOCR, unsigned int)

            // Red band index accessors for TOCR
            itkSetMacro(RedBandTOCR, unsigned int)

            // NIR band index accessors for TOCR
            itkSetMacro(NIRBandTOCR, unsigned int)

            // SWIR availability
            itkSetMacro(HasSwirBand, bool)

            // SWIR band index accessors for TOCR
            itkSetMacro(SWIRBandTOCR, unsigned int)

            // Blue band index accessors for RCR
            itkSetMacro(BlueBandRCR, unsigned int)

            // Red band index accessors for RCR
            itkSetMacro(RedBandRCR, unsigned int)

            // NIR band index accessors for RCR
            itkSetMacro(NIRBandRCR, unsigned int)

            // Shadow band index accessors for the TOCR band
            vnsMemberAndSetAndGetConstReferenceMacro(ShadowBandTOCR, unsigned int);

            // Shadow band index accessors for the RCR band
            vnsMemberAndSetAndGetConstReferenceMacro(ShadowBandRCR, unsigned int);

            // Reflectance threshold parameters
            // ForegroundValue accessors
            itkSetMacro(ForegroundValue, VectorMaskInternalPixelType)

            // blue reflectance threshold value accessors
            itkSetMacro(BlueReflectanceThresholdValue, InputInternalPixelType)
            // blue reflectance threshold value accessors
            itkSetMacro(RedReflectanceThresholdValue, InputInternalPixelType)
            // blue reflectance threshold value accessors
            itkSetMacro(NirRedRatioMaxThresholdValue, double)
            // blue reflectance threshold value accessors
            itkSetMacro(NirRedRatioMinThresholdValue, double)
            // blue reflectance threshold value accessors
            itkSetMacro(NirSwirRatioMinThresholdValue, double)

            // Erosion or dilatation of input masks
            // radius of the stucturing element use for erosion
            itkSetMacro(SEInputMasksRadius, unsigned short)

            // Sunglint flag
            itkSetMacro(SunglintFlag, bool)

            // Threshold on surface reflectance in the water band
            // for pixels declared water (if sunglint_flag is True)
            itkSetMacro(WaterSunglintReflectanceThreshold, double)

            // Threshold on surface reflectance in the  water band
            // for pixels declared water (if sunglint_flag is False)
            itkSetMacro(WaterReflectanceThreshold, double)

            // Band used for thresholding of reflectance over water
            itkSetMacro(ReflectanceWaterBand, unsigned int)

            // Reflectance variation parameters
            // Product julian date accessors
            itkSetMacro(JDay, double)

            //STO list of dates
            void
            SetSTOListOfDates(Utilities::ListOfDoubles &listOfDates)
            {
                m_STOListOfDates = listOfDates;
            }
            const Utilities::ListOfDoubles &
            GetSTOListOfDates(void)
            {
                return m_STOListOfDates;
            }

            // Cloud Forgetting duration accessors
            itkSetMacro(CloudForgettingDuration, double)

            // Min variation threshold accessors
            itkSetMacro(MinVarThreshold, double)

            // Max variation threshold accessors
            itkSetMacro(MaxVarThreshold, double)

            // KNIRBlue parameter accessors
            itkSetMacro(KNIRBlue, double)

            // KRedBlue parameter accessors
            itkSetMacro(KRedBlue, double)

            // DeltaThreshold parameter accesors
            itkSetMacro(DeltaThreshold, double)

            // MinPixelCorrelation parameter accesors
            itkSetMacro(MinPixelCorrelation, unsigned long)

            // Correlation Band parameter accesors band TOCR
            itkSetMacro(CorrelBandTOCR, unsigned int)

            // Correlation Band parameter accesors band RCR
            itkSetMacro(CorrelBandRCR, unsigned int)

            // CorrelThreshold parameter accesors
            itkSetMacro(CorrelThreshold, double)

            // Radius accesors
            itkSetMacro(NCorrel, RadiusSizeType)

            /** Set unsigned int radius */
            void
            SetNCorrel(unsigned int radius)
            {
                m_NCorrel.Fill(radius);
            }

            // Stereoscopic altitude correction
            // KsigmaAltitude parameter accesors
            itkSetMacro(KsigmaAltitude, unsigned short)

            // DefaultAltitude parameter accesors
            itkSetMacro(DefaultAltitude, short)

            // Shadow mask determination
            // No Refinement option accessors
            itkSetMacro(RefinementOption, bool)

            // DeltaHMax parameter accessors
            itkSetMacro(DeltaHMax, short)

            // DeltaHMin parameter accessors
            itkSetMacro(DeltaHMin, short)

            // DeltaHStep parameter accessors
            itkSetMacro(DeltaHStep, short)

            // Local DeltaHMax parameter accessors
            itkSetMacro(LocalDeltaHMax, short)

            // Local DeltaHMin parameter accessors
            itkSetMacro(LocalDeltaHMin, short)

            // Local DeltaHStep parameter accessors
            itkSetMacro(LocalDeltaHStep, short)

            // First reference altitude parameter accessors
            itkSetMacro(SOLH1, double)

            // Second reference altitude parameter accessors
            itkSetMacro(SOLH2, double)

            // Reference altitude for sun direction accessors
            itkSetMacro(VIEHRef, double)

            // Reference altitude for sun direction accessors
            itkSetMacro(SOLHRef, double)

            // MACCS 5.0.0 : New algo
            vnsMemberAndSetAndGetConstReferenceMacro(DiscardedPixelsRatioMin, double)
            vnsMemberAndSetAndGetConstReferenceMacro(ShadVarDarkeningMax, double)
            vnsMemberAndSetAndGetConstReferenceMacro(ShadVarPercentile,double)
            vnsMemberAndSetAndGetConstReferenceMacro(ShadVarCompositeLabelRatio, double)
            // Enable/Disable known clouds altitude
            // if CloudMaskingKnownCloudsAltitude == true (ex VENUS)
            //   - AbsNbpixThreshold
            //   - ThresholdDiffImage
            //   - NumberMaxIter
            // else CloudMaskingKnownCloudsAltitude == false (ex: Others)
            //   - ShadowCloudRatioMax
            //   - ShadowCloudRatioMin
            //   - ShadowCloudRatioPixNb
            //   - ProjShadDarkeningMax
            //   - DarkeningMinPercentile
            vnsBooleanMemberAndSetAndGetConstReferenceMacro(CloudMaskingKnownCloudsAltitude)
            // Minimum number of usable pixels in a label to perform the altitude refinement
            vnsMemberAndSetAndGetConstReferenceMacro(AbsNbpixThreshold,unsigned short)
            // Minimum darkening of image to decide to change the cloud altitude
            vnsMemberAndSetAndGetConstReferenceMacro(ThresholdDiffImage, double)

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

            // Shadows from clouds outside the image
            // Max cloud altitude accessors
            itkSetMacro(MaxCLA, double)

            // Shadow variation threshold

            // Dilatation of all cloud masks
            // radius of the stucturing element use for dilatation
            itkSetMacro(SECloudRadius, unsigned short)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro( IPTOCR, InputImageType, 0 )
            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inRCR , InputImageType , 9 )
            /** Set the stack of previous surface reflectance images corrected by absorption and rayleigh
             * of the composite product */

            vnsSetGetInputMacro( L2inSTO , InputImageType , 10 )
            /** Set the saturated vector image of the input product*/
            vnsSetGetInputMacro( IPSAT , VectorMaskType , 1 )
            /** Set the edge mask of the input product */

            vnsSetGetInputMacro( IPEDG , InputMaskType , 2)
            /** Set the no_data mask of the composite product */
            vnsSetGetInputMacro( L2inNDT , InputMaskType , 11)
            /** Set the image of date of the composite product */
            vnsSetGetInputMacro( L2inPXD , DateImageType , 12)
            /** Set the water mask of the composite product */
            vnsSetGetInputMacro( L2inWAS , InputMaskType , 13 )
            /** Set the viewing direction grid */
            vnsSetGetInputMacro( IPVIE , GRIDImageType , 5)
            /**
             * Set the solar direction grid for the first reference altitude
             * Note: The values of the solar grid are different between VENUS and Formosat :
             * - VENUS: read from the HDF grids
             * - FORMOSAT: constant values read from the GIP_L2COMM ("SOLH1", "UsefulImageSolarAngleZenith", "UsefulImageSolarAngleAzimuth")
             *   and computed with a formula (in FormosatL1ImageFileReader)
             */
            vnsSetGetInputMacro( IPSOL1 , GRIDImageType , 3 )
            /** Set the solar direction grid for the second reference altitude
             * Note: The values of the solar grid are different between VENUS and Formosat :
             * - VENUS: read from the HDF grids
             * - FORMOSAT: constant values read from the GIP_L2COMM ("SOLH2", "UsefulImageSolarAngleZenith", "UsefulImageSolarAngleAzimuth")
             *   and computed with a formula (in FormosatL1ImageFileReader)
             */
            vnsSetGetInputMacro( IPSOL2 , GRIDImageType , 4 )
            /** Set the L1 cloud altitude image */
            vnsSetGetInputMacro( L1CLA , CLAImageType , 6)
            vnsSetGetInputMacro( L1CLDAlt , InputMaskType , 7)

            /** Set the DTM image */
            vnsSetGetInputMacro( DTM , DTMImageType , 8 )
            /** Set the snow mask sub image  */
            vnsSetGetInputMacro( IPSNWSub , InputMaskType , 14 )
            /** Set the cirrus mask sub image (S2)  */
            vnsSetGetInputMacro( IPCirrusMaskSub , InputMaskType , 15)

            /** D-1 shadvar mask sub image */
            vnsSetGetInputMacro( L2inShadVar , InputMaskType , 16)
            /** D-1 shadow mask sub image */
            vnsSetGetInputMacro( L2inShadow , InputMaskType , 17)

            /** Get the summary cloud mask */
            itkGetObjectMacro(IPCLDSumOutput, OutputImageType)
            itkGetConstObjectMacro(IPCLDSumOutput, OutputImageType)

            /** Get the refined value for the altitude of clouds */
            itkGetObjectMacro(L2CLAOutput, CLAImageType)
            itkGetConstObjectMacro(L2CLAOutput, CLAImageType)

            /** Get the cloud vector mask */
            itkGetObjectMacro(IPCLDSubVectOutput, VectorMaskType)
            itkGetConstObjectMacro(IPCLDSubVectOutput, VectorMaskType)

            /** Get the corrected snow mask */
            itkGetObjectMacro(IPSNWSubOutput, InputMaskType)
            itkGetConstObjectMacro(IPSNWSubOutput, InputMaskType)

            /** Main computation method. */
            virtual void
            UpdateWithCaching(void);

        protected:
            /** Constructor */
            CloudMaskingImageFilter();
            /** Destructor */
            virtual
            ~CloudMaskingImageFilter()
            {
            }


            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            CloudMaskingImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** No_data value */
            RealNoDataType m_NoData;

            /** L2 coarse resolution */
            unsigned short m_L2CoarseResolution;

            /** Init mode parameter */
            bool m_InitMode;
            /** Refinement option declaration */
            bool m_RefinementOption;

            /** Blue band definition for TOCR*/
            unsigned int m_BlueBandTOCR;
            /** Red band definition  for TOCR*/
            unsigned int m_RedBandTOCR;
            /** NIR band definition  for TOCR*/
            unsigned int m_NIRBandTOCR;
            /** boolean for SWIR availability*/
            bool m_HasSwirBand;
            /** SWIR band definition  for TOCR*/
            unsigned int m_SWIRBandTOCR;
            /** Blue band definition for RCR*/
            unsigned int m_BlueBandRCR;
            /** Red band definition  for RCR*/
            unsigned int m_RedBandRCR;
            /** NIR band definition  for RCR*/
            unsigned int m_NIRBandRCR;
            //** Water band definition */
            unsigned int m_ReflectanceWaterBand;

            // Reflectance Thershold parameters
            /** Foreground value declaration */
            VectorMaskInternalPixelType m_ForegroundValue;
            /** Blue reflectance Threshold value declaration */
            InputInternalPixelType m_BlueReflectanceThresholdValue;
            /** RedReflectance Threshold value declaration */
            InputInternalPixelType m_RedReflectanceThresholdValue;
            /** NirRedRatio max Threshold value declaration */
            double m_NirRedRatioMaxThresholdValue;
            /** NirRedRatio min Threshold value declaration */
            double m_NirRedRatioMinThresholdValue;
            /** NirSwirRatio min Threshold value declaration */
            double m_NirSwirRatioMinThresholdValue;

            // Reflectance variation threshold parameters
            /** Date of the input image declaration */
            double m_JDay;
            /** List of dates in STO image */
            Utilities::ListOfDoubles m_STOListOfDates;
            /** Declaration of time delay after which the reflectance variation threshold is doubled */
            double m_CloudForgettingDuration;
            /** Declaration of the minimum threshold on reflectance variation between date D and D-1 */
            double m_MinVarThreshold;
            /** Declaration of the maximum threshold on reflectance variation between date D and D-1 */
            double m_MaxVarThreshold;
            /** Declaration of parameter for comparison of relative variations in the blue and in the NIR */
            double m_KNIRBlue;
            /** Declaration of parameter for comparison of variations in the blue and in the red */
            double m_KRedBlue;
            /** Declaration of the difference between the threshold used for the initial variation detection step
             * and the one used for the extension step */
            double m_DeltaThreshold;
            /** Declaration of the minimum number of pixels to take correlation into account */
            unsigned long m_MinPixelCorrelation;
            /** Declaration of the correlation band for TOCR */
            unsigned int m_CorrelBandTOCR;
            /** Declaration of the correlation band for RCR */
            unsigned int m_CorrelBandRCR;
            /** Declaration of the threshold on correlation between images of date D and D-1 */
            double m_CorrelThreshold;

            /** Radius of the neighborhood iterator declaration */
            RadiusSizeType m_NCorrel;

            // Stereoscopic cloud altitude correction
            /** Cloud altitudes are filtered to discard outliers.
             *  KsigmaAltitude is the factor that defines the strictness of this filter
             */
            unsigned short m_KsigmaAltitude;
            /** Default altitude attribute to cloud pixels
             * if stereoscopy has not detected any cloud */
            short m_DefaultAltitude;

            /** Altitude excursion around the L1 altitude for the global refinement*/
            short m_DeltaHMax;
            short m_DeltaHMin;
            short m_DeltaHStep;
            /** Altitude excursion around the L1 altitude for the local refinement */
            short m_LocalDeltaHMax;
            short m_LocalDeltaHMin;
            short m_LocalDeltaHStep;
            /** First reference altitude for solar grid */
            double m_SOLH1;
            /** Second reference altitude for solar grid */
            double m_SOLH2;
            /** Reference altitude of viewing direction */
            double m_VIEHRef;
            /** Reference altitude of solar direction */
            double m_SOLHRef;

            // Shadow from clouds outside the image
            /** Maximum cloud altitude declaration */
            double m_MaxCLA;

            // Dilatation
            unsigned short m_SECloudRadius;

            // Erosion
            unsigned short m_SEInputMasksRadius;

            // Sunglint flag
            bool m_SunglintFlag;

            // Reflectance thresholds in water band depending on sunglint flag
            double m_WaterSunglintReflectanceThreshold;
            double m_WaterReflectanceThreshold;

            // Filter declaration
            OrFilterPointerType m_OrFilter;
            OrFilterPointerType m_OrFilter2;
            OrFilterPointerType m_OrFilter3;
            OrFilterPointerType m_OrFilterExtensionAll;
            ReflectanceThresholdPointerFilterType m_ReflectanceThresholdFilter;
            ReflectanceVariationCorrelationPointerFilterType m_ReflectanceVariationCorrelationFilter;
            CloudMaskExtensionPointerFilterType m_CloudMaskExtensionFilter;
            // DM982
            TernaryLogicalPointerFilterType m_TmpCloudSynthFilter;
            LabelPointerFilterType m_TmpCloudLabelFilter;
            ReaderLabelPointer m_TmpCloudLabelReader;
            double m_LowerValidityThreshold;
            DiscardedCloudPixelFilterPointerType m_CloudMaskExtensionDiscardedPixelValidationFilter;
            ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType m_CloudMaskExtensionPostProcessingFilter;
            SnowMaskCorrectionPointerFilterType m_SnowMaskCorrectionFilter;
            ConstantPointerFilterType m_ConstantMaskFilter;
            NaryLogicalPointerFilterType m_NaryLogicalFilter;
            LabelPointerFilterType m_IPCLDExtLabelFilter;
            LabelPointerFilterType m_LabelFilter;
//            LabelConstantPointerFilterType m_LabelConstantFilter;
            StereoCorrectionPointerFilterType m_StereoCorrectionFilter;
            ShadowMaskDeterminationPointerFilterType m_ShadowMaskDeterminationFilter;

            //DM982
//            ValueCountFilterPointerType   m_CLDCountFilter;
//            DarkeningFilterPointerType    m_DarkeningFilter;
//            DarkeningImageType            m_DarkeningFilterOutput;
//            ReaderDarkeningPointer        m_DarkeningFilterOutputReader;
//            DarkeningImageListPointerType m_DarkeningImageList;
//            DarkeningImageListToVectorImageFilterPointerType  m_DarkeningImageListToVectorImage;
//            ShadowVariationThresholdValueEstimatorFilterPointerType m_ShadowThresholdValueEstimatorFilter;
//            LabelPointerFilterType        m_DilatedCloudMaskAllLabelFilter;
//            LabelImagePointer             m_DilatedCloudMaskAllLabelFilterOutput;
//            ReaderLabelPointer            m_DilatedCloudMaskAllLabelReader;
//            LabelMaskingFilterPointerType m_LabelMaskingFilter;
//            StreamingStatisticsMapFromLabelImageFilterPointerType m_ComputeCLDPixNbMap;
//
//            ReflRatioMaskingFilterPointerType m_DarkeningMaskingFilter;
//            RatioImageListPointerType m_DarkeningMaskedImageList;
//            RatioImageListToVectorImageFilterPointerType m_DarkeningMaskedToVectorImage;
//
//            TernaryLogicalPointerFilterType m_ShadowLogicalSummaryFilter;

            ShadowMaskDeterminationWithoutAltitudePointerFilterType
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter;

            //fin DM982


            CLAConstantPointerFilterType m_CLAConstantFilter;
            SolarFilterPointerType m_SolarFilter;
            OutsideCloudsShadowMaskPointerFilterType m_OutsideCloudShadowMaskFilter;
            //ShadowVariationThresholdPointerFilterType m_ShadowVariationThresholdFilter;
            TernaryLogicalPointerFilterType m_TernaryLogicalSummaryFilter;
            //DM982
            ValueCountFilterPointerType m_IPCLDShadCountFilter;
            RatioFilterPointerType m_ReflRatioFilter;
            ReaderRatioPointer m_ReflRatioFilterOutputReader;
            ReflRatioMaskingFilterPointerType m_ReflRatioMaskingFilter;
            RatioImageListToVectorImageFilterPointerType m_ReflRatioMaskedToVectorImage;
            ShadowVariationThresholdValueEstimatorFilterPointerType m_ShadVarThresholdValueEstimatorFilter;
            ShadowVariationThresholdPointerFilterType m_ShadowVariationThresholdFilter;
            LabelPointerFilterType m_ShadVarLabelFilter;
            ReaderLabelPointer m_ShadVarLabelReader;
            double m_SharVarCompositeLabelRatio;
            MaskMaskingFilterPointerType m_ShadowVariationMaskCompositeCombinaisonFilter;
            DiscardedCloudPixelFilterPointerType m_ShadowVariationPixelValidationFilter;
            ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType m_ShadowVariationPixelDiscardingFilter;
            ShadowVariationCorrelationImageFilterPointerType m_ShadowVariationCorrelationFilter;

            DilatePointerFilterType m_IPCLDSumDilateFilter;
            DilatePointerFilterType m_IPCLDAllNoExtensionDilateFilter;
            DilatePointerFilterType m_IPCLDAllDilateFilter;
            DilatePointerFilterType m_IPCLDReflDilateFilter;
            DilatePointerFilterType m_IPCLDReflVarDilateFilter;
            DilatePointerFilterType m_IPCLDExtDilateFilter;
            DilatePointerFilterType m_IPCLDAltDilateFilter;
            DilatePointerFilterType m_IPCLDShadDilateFilter;
            DilatePointerFilterType m_IPCLDShadVarDilateFilter;
            DilatePointerFilterType m_DilateWaterMaskFilter;
            DilatePointerFilterType m_DilateSnowMaskFilter;
            DilatePointerFilterType m_DilateCirrusMaskFilter;

            ErodePointerFilterType m_ErodeWaterMaskFilter;

            // Concatenation of cloud masks in a vector image
            ImageListType::Pointer m_IPCLDList;
            ImageListToVectorImageFilterType::Pointer m_IPCLDListToVectorImage;

            // Apply the edge mask to the output mask
            ApplyMaskFilterPointer m_CLDSumApplyMaskFilter;
            ChangeValueWithMaskVectorImageFilterPointer m_VectorChangeValueWithMaskFilter;

            OutputImagePointer m_IPCLDSumOutput;
            CLAImagePointer m_L2CLAOutput;
            VectorMaskPointer m_IPCLDSubVectOutput;
            InputMaskPointer m_IPSNWSubOutput;

            ReaderMaskPointer m_ShadowMaskDeterminationFilterIPCLDShadOutputReader;

            ReaderMaskPointer m_ShadowVariationThresholdFilterIPCLDShadVarOutputReader;

            ReaderMaskPointer m_ShadowVariationIPCLDShadVarOutputReader;

            ReaderMaskPointer m_CloudMaskAllNoExtensionOutputReader;
            ReaderMaskPointer m_CloudMaskAllOutputReader;
            ReaderMaskPointer m_ReflectanceThresholdFilterOutputReader;

            ReaderLabelPointer m_IPCLDLabelReader;
            ReaderLabelPointer m_LabelReader;

            /** Reader */
            ReaderCLAPointer m_IPCLAOutputReader;
            ReaderCLDVectPointer m_IPCLDSubVectOutputReader;
            ReaderMaskPointer m_IPSNWSubOutputReader;

//            ComputesProjectionsMasksLabelsPointer m_ComputesProjectionsMasksLabelsFilter;

        };

} // End namespace vns

#endif /* __vnsCloudMaskingImageFilter_h */
