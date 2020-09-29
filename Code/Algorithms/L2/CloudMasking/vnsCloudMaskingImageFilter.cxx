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
/************************************************************************************************************ *                                                                                                          *
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
 * VERSION : 3.0.0 : FA : LAIG-FA-MAJA-157183-CS : 9 octobre 2017 : code quality                            *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2298-CNES : 27 juillet 2017 : Prise en compte masque extension       *
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 21 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-1991-CNES : 29 novembre 2016 : Update CloudMasking ATCOR              *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 13 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1989-CNES : 13 octobre 2016 : Correction qualite (FIX ME)             *
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1666-CNES : 3 aout 2016 : Suppression parametre 'Min_Shadow_Variation'*
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 17 juin 2016 : Modifications cosmetique et menage, etc... *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-1691-CNES : 25 avril 2016 : Ecriture plan CLA uniquement si disponible*
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-3-0 : FA : LAIG-FA-MAC-1180-CNES : 21 mai 2015 : ajout crocodile ReflVar pour validation		*
 * VERSION : 4-1-2 : FA : LAIG-FA-MAC-1180-CNES : 19 mars 2015 : Pas de corrélation si date courante = date STO	*
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 10 decembre 2013 : Ajout controle sur size sur masque SNW  *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 25 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
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
#include "vnsCloudMaskingImageFilter.h"

#include "vnsMacro.h"
#include "vnsUtilities.h"

namespace vns
{

    // Constructor
    CloudMaskingImageFilter::CloudMaskingImageFilter() :
            m_DiscardedPixelsRatioMin(0.), m_ShadVarDarkeningMax(0.), m_ShadVarPercentile(0.), m_ShadVarCompositeLabelRatio(0.), m_CloudMaskingKnownCloudsAltitude(
                    false), m_AbsNbpixThreshold(0), m_ThresholdDiffImage(0.), m_CloudCoverMultFactor(0.), m_ShadowCloudRatioMax(0.), m_ShadowCloudRatioMin(
                    0.), m_ShadowCloudRatioPixNb(0), m_ProjShadDarkeningMax(0.), m_DarkeningMinPercentile(0.), m_NoData(
                    static_cast<RealNoDataType>(0)), m_L2CoarseResolution(1), m_InitMode(false), m_RefinementOption(true), m_BlueBandTOCR(
                    0), m_RedBandTOCR(0), m_NIRBandTOCR(0), m_HasSwirBand(false), m_SWIRBandTOCR(0), m_BlueBandRCR(0), m_RedBandRCR(0), m_NIRBandRCR(
                    0), m_ReflectanceWaterBand(0), m_ForegroundValue(static_cast<VectorMaskInternalPixelType>(1)), m_BlueReflectanceThresholdValue(
                    static_cast<InputInternalPixelType>(0)), m_RedReflectanceThresholdValue(static_cast<InputInternalPixelType>(0)), m_NirRedRatioMaxThresholdValue(
                    0), m_NirRedRatioMinThresholdValue(0), m_NirSwirRatioMinThresholdValue(0), m_JDay(0.), m_CloudForgettingDuration(0.), m_MinVarThreshold(
                    0.), m_MaxVarThreshold(10.), m_KNIRBlue(0.), m_KRedBlue(0.), m_DeltaThreshold(0.), m_MinPixelCorrelation(0), m_CorrelBandTOCR(
                    0), m_CorrelBandRCR(0), m_CorrelThreshold(0.), m_KsigmaAltitude(0), m_DefaultAltitude(0), m_DeltaHMax(0), m_DeltaHMin(
                    0), m_DeltaHStep(0), m_LocalDeltaHMax(0), m_LocalDeltaHMin(0), m_LocalDeltaHStep(0), m_SOLH1(0.), m_SOLH2(0.), m_VIEHRef(
                    0.), m_SOLHRef(0.), m_MaxCLA(0.), m_SECloudRadius(0), m_SEInputMasksRadius(0), m_SunglintFlag(false), m_WaterSunglintReflectanceThreshold(
                    0.), m_WaterReflectanceThreshold(0.), m_SharVarCompositeLabelRatio(0)
    {
        // Input/Output initialization
        this->SetNumberOfIndexedInputs(18);
        this->SetNumberOfRequiredInputs(9);

        // Init correl kernel
        m_NCorrel.Fill(3);

        // Logical Filter initializations
        m_OrFilter = OrFilterType::New();
        m_OrFilter2 = OrFilterType::New();
        m_OrFilter3 = OrFilterType::New();
        m_OrFilterExtensionAll = OrFilterType::New();
        // Threshold filter
        m_ReflectanceThresholdFilter = ReflectanceThresholdFilterType::New();
        m_ReflectanceVariationCorrelationFilter = ReflectanceVariationCorrelationFilterType::New();
        // Label filter
        m_IPCLDExtLabelFilter = LabelFilterType::New();
        m_CloudMaskExtensionFilter = CloudMaskExtensionFilterType::New();
        // DM 982
        m_TmpCloudSynthFilter = TernaryLogicalFilterType::New();
        m_TmpCloudLabelFilter = LabelFilterType::New();
        m_TmpCloudLabelReader = ReaderLabelType::New();
        // CLD pixel discarding filter
        m_CloudMaskExtensionDiscardedPixelValidationFilter = DiscardedCloudPixelFilterType::New();
        m_CloudMaskExtensionPostProcessingFilter = ValidateMaskFromLabelMapAndValidityMapImageFilterType::New();
        // fin DM 982
        m_SnowMaskCorrectionFilter = SnowMaskCorrectionFilterType::New();
        m_ConstantMaskFilter = ConstantFilterType::New();
        m_NaryLogicalFilter = NaryLogicalFilterType::New();
        m_LabelFilter = LabelFilterType::New();
        m_StereoCorrectionFilter = StereoCorrectionFilterType::New();
        m_ShadowMaskDeterminationFilter = ShadowMaskDeterminationFilterType::New();
        // DM 982
        m_ShadowMaskDeterminationWithoutCloudAltitudeFilter = ShadowMaskDeterminationWithoutAltitudeFilterType::New();
        // fin DM982
        m_CLAConstantFilter = CLAConstantFilterType::New();
        m_SolarFilter = SolarFilterType::New();
        m_OutsideCloudShadowMaskFilter = OutsideCloudsShadowMaskFilterType::New();
        // DM 982
        m_IPCLDShadCountFilter = ValueCountFilterType::New();
        m_ReflRatioFilter = RatioFilterType::New();
        m_ReflRatioFilterOutputReader = ReaderRatioImageType::New();
        m_ReflRatioMaskingFilter = ReflRatioMaskingFilterType::New();
        m_ReflRatioMaskedToVectorImage = RatioImageListToVectorImageFilterType::New();
        // Shadvar filter
        m_ShadVarThresholdValueEstimatorFilter = ShadowVariationThresholdValueEstimatorFilterType::New();
        m_ShadowVariationThresholdFilter = ShadowVariationThresholdFilterType::New();
        m_ShadVarLabelFilter = LabelFilterType::New();
        m_ShadVarLabelReader = ReaderLabelType::New();
        m_ShadowVariationMaskCompositeCombinaisonFilter = MaskMaskingFilterType::New();
        m_ShadowVariationPixelValidationFilter = DiscardedCloudPixelFilterType::New();
        m_ShadowVariationPixelDiscardingFilter = ValidateMaskFromLabelMapAndValidityMapImageFilterType::New();
        m_ShadowVariationCorrelationFilter = ShadowVariationCorrelationImageFilterType::New();
        m_ShadowVariationIPCLDShadVarOutputReader = ReaderMaskType::New();
        // fin DM 982
        m_TernaryLogicalSummaryFilter = TernaryLogicalFilterType::New();

        // Init IPCLD filters
        m_IPCLDSumDilateFilter = DilateFilterType::New();
        m_IPCLDAllNoExtensionDilateFilter = DilateFilterType::New();
        m_IPCLDAllDilateFilter = DilateFilterType::New();
        m_IPCLDReflDilateFilter = DilateFilterType::New();
        m_IPCLDReflVarDilateFilter = DilateFilterType::New();
        m_IPCLDExtDilateFilter = DilateFilterType::New();
        m_IPCLDAltDilateFilter = DilateFilterType::New();
        m_IPCLDShadDilateFilter = DilateFilterType::New();
        m_IPCLDShadVarDilateFilter = DilateFilterType::New();
        m_DilateWaterMaskFilter = DilateFilterType::New();
        m_DilateSnowMaskFilter = DilateFilterType::New();
        m_DilateCirrusMaskFilter = DilateFilterType::New();

        // MorphoMath filter
        m_ErodeWaterMaskFilter = ErodeFilterType::New();

        // IPCLD mask storage
        m_IPCLDList = ImageListType::New();
        m_IPCLDListToVectorImage = ImageListToVectorImageFilterType::New();

        // CLD mask processing filter
        m_CLDSumApplyMaskFilter = ApplyMaskFilterType::New();
        m_CloudMaskAllNoExtensionOutputReader = ReaderMaskType::New();
        m_CloudMaskAllOutputReader = ReaderMaskType::New();

        // Conditional modification filter
        m_VectorChangeValueWithMaskFilter = ChangeValueWithMaskVectorImageFilterType::New();

        // Init output readers
        m_IPCLDSumOutput = OutputImageType::New();
        m_L2CLAOutput = CLAImageType::New();
        m_IPCLDSubVectOutput = VectorMaskType::New();
        m_IPSNWSubOutput = InputMaskType::New();

        // Init IP output readers
        m_IPCLAOutputReader = ReaderCLAType::New();
        m_IPCLDSubVectOutputReader = ReaderCLDVectType::New();
        m_IPSNWSubOutputReader = ReaderMaskType::New();

        // Init shadow mask filter
        m_ShadowMaskDeterminationFilterIPCLDShadOutputReader = ReaderMaskType::New();

        m_ShadowVariationThresholdFilterIPCLDShadVarOutputReader = ReaderMaskType::New();

        // Init reflectance threshold filters
        m_ReflectanceThresholdFilterOutputReader = ReaderMaskType::New();

        // Init Readers
        m_LabelReader = ReaderLabelType::New();
        m_IPCLDLabelReader = ReaderLabelType::New();

        // Set NULL for optionnal input
        this->SetL2inRCRInput(NULL);
        this->SetL2inSTOInput(NULL);
        this->SetL2inNDTInput(NULL);
        this->SetL2inPXDInput(NULL);
        this->SetL2inWASInput(NULL);
        this->SetL2inShadVarInput(NULL);
        this->SetL2inShadowInput(NULL);
    }

    //================================== UpdateWithCaching ==========================================/
    // Main computation method.
    void
    CloudMaskingImageFilter::UpdateWithCaching()
    {
        // Check percentil is in [0 ... 1]
        if ((this->GetShadVarPercentile() < 0.) || (this->GetShadVarPercentile() > 1.))
        {
            vnsExceptionBusinessMacro(
                    "The ShadVarPercentile value '"<<this->GetShadVarPercentile()<<"' is incorrect. It's must be in [0...1]!")
        }
        if (this->GetCloudMaskingKnownCloudsAltitude() == false)
        {
            if ((this->GetDarkeningMinPercentile() < 0.) || (this->GetDarkeningMinPercentile() > 1.))
            {
                vnsExceptionBusinessMacro(
                        "The DarkeningMinPercentile value '"<<this->GetDarkeningMinPercentile()<<"' is incorrect. It's must be in [0...1]!")
            }
        }

        // Update Inputs Information to get the true sizes...
        (dynamic_cast<InputImageType *>(itk::ProcessObject::GetInput(0)))->UpdateOutputInformation();
        (dynamic_cast<VectorMaskType *>(itk::ProcessObject::GetInput(1)))->UpdateOutputInformation();
        (dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(2)))->UpdateOutputInformation();
        (dynamic_cast<CLAImageType *>(itk::ProcessObject::GetInput(6)))->UpdateOutputInformation();
        (dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(7)))->UpdateOutputInformation();
        (dynamic_cast<GRIDImageType *>(itk::ProcessObject::GetInput(5)))->UpdateOutputInformation();
        (dynamic_cast<GRIDImageType *>(itk::ProcessObject::GetInput(3)))->UpdateOutputInformation();
        //(dynamic_cast<GRIDImageType *> (itk::ProcessObject::GetInput(4)))->UpdateOutputInformation();
        (dynamic_cast<DTMImageType *>(itk::ProcessObject::GetInput(8)))->UpdateOutputInformation();

        // Get pointers to the input and output images
        InputImageConstPointer l_IPTOCRPtr = this->GetIPTOCRInput();
        VectorMaskConstPointer l_IPSATPtr = this->GetIPSATInput();
        InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
        CLAImageConstPointer l_L1CLAPtr = this->GetL1CLAInput();
        InputMaskConstPointer l_L1CLDAltPtr = this->GetL1CLDAltInput();
        GRIDImageConstPointer l_IPVIEPtr = this->GetIPVIEInput();
        GRIDImageConstPointer l_IPSOL1Ptr = this->GetIPSOL1Input();
        GRIDImageConstPointer l_IPSOL2Ptr = this->GetIPSOL2Input();
        DTMImageConstPointer l_DTMPtr = this->GetDTMInput();
        InputMaskConstPointer l_IPSNWSubPtr = this->GetIPSNWSubInput();
        InputMaskConstPointer l_IPCirrusMaskSubPtr = this->GetIPCirrusMaskSubInput();

        // From L2 composite product
        InputImageConstPointer l_L2inRCRPtr = this->GetL2inRCRInput();
        InputImageConstPointer l_L2inSTOPtr = this->GetL2inSTOInput();
        InputMaskConstPointer l_L2inNDTPtr = this->GetL2inNDTInput();
        DateImageConstPointer l_L2inPXDPtr = this->GetL2inPXDInput();
        InputMaskConstPointer l_L2inWASPtr = this->GetL2inWASInput();
        InputMaskConstPointer l_L2inShadVarPtr = this->GetL2inShadVarInput();
        InputMaskConstPointer l_L2inShadowPtr = this->GetL2inShadowInput();
        // Check the input images size
        if (m_InitMode == true)
        {
            vnsChecking6ImagesSizeMacro(l_IPTOCRPtr, l_IPSATPtr, l_IPEDGPtr, l_IPVIEPtr, l_IPSOL1Ptr, l_DTMPtr)
        }
        else
        {
            (dynamic_cast<InputImageType *>(itk::ProcessObject::GetInput(9)))->UpdateOutputInformation();
            (dynamic_cast<InputImageType *>(itk::ProcessObject::GetInput(10)))->UpdateOutputInformation();
            (dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(11)))->UpdateOutputInformation();
            (dynamic_cast<DateImageType *>(itk::ProcessObject::GetInput(12)))->UpdateOutputInformation();
            (dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(13)))->UpdateOutputInformation();

            vnsChecking11ImagesSizeMacro(l_IPTOCRPtr, l_IPSATPtr, l_IPEDGPtr, l_IPVIEPtr, l_IPSOL1Ptr, l_DTMPtr, l_L2inRCRPtr, l_L2inSTOPtr,
                    l_L2inNDTPtr, l_L2inPXDPtr, l_L2inWASPtr)
        }

        // Reflectance Threshold

        // Set the reflectance threshold on the water band tacking into account the sunglint flag
        double l_WaterThreshold(0.);
        if (m_SunglintFlag == true)
        {
            l_WaterThreshold = m_WaterSunglintReflectanceThreshold;
        }
        else
        {
            l_WaterThreshold = m_WaterReflectanceThreshold;
        }

        vnsLogDebugBooleanValueMacro(m_SunglintFlag)
        vnsLogDebugValueMacro(m_WaterSunglintReflectanceThreshold)
        vnsLogDebugValueMacro(m_WaterReflectanceThreshold)
        vnsLogDebugValueMacro(l_WaterThreshold)

		m_ReflectanceThresholdFilter->SetInput1(l_IPTOCRPtr);
        m_ReflectanceThresholdFilter->SetInput2(l_IPSATPtr);
        if (l_IPSNWSubPtr.IsNull() == false)
        {
            (dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(14)))->UpdateOutputInformation();
            vnsChecking2ImagesSizeMacro(l_IPEDGPtr, l_IPSNWSubPtr)
            m_OrFilter->SetInput1(l_IPEDGPtr);
            m_OrFilter->SetInput2(l_IPSNWSubPtr);
            m_ReflectanceThresholdFilter->SetInput3(m_OrFilter->GetOutput());
        }
        else
        {
        	m_ReflectanceThresholdFilter->SetInput3(l_IPEDGPtr);
        }

        // In init mode, the reflectance variation threshold test is not performed
        // and the water mask of the composite is not available
        // A constant mask is generate to proceed the treatments
        // This mask is also used when an option is not set and a simulated image has to be used
        m_ConstantMaskFilter->SetSize(l_IPEDGPtr->GetLargestPossibleRegion().GetSize());
        m_ConstantMaskFilter->SetSpacing(l_IPEDGPtr->GetSpacing());
        m_ConstantMaskFilter->SetOrigin(l_IPEDGPtr->GetOrigin());
        m_ConstantMaskFilter->SetConstantValue(static_cast<InputMaskPixelType>(0));
        m_ConstantMaskFilter->UpdateOutputInformation();

        if (m_InitMode == false)
        {
            // Water mask erosion (FTs OGM 114432)
            StructuringElementType l_SEMaskErosion;
            l_SEMaskErosion.SetRadius(m_SEInputMasksRadius); // 3x3 structuring element
            l_SEMaskErosion.CreateStructuringElement();

            m_ErodeWaterMaskFilter->SetInput(l_L2inWASPtr);
            m_ErodeWaterMaskFilter->SetErodeValue(1);
            m_ErodeWaterMaskFilter->SetKernel(l_SEMaskErosion);
            m_ErodeWaterMaskFilter->UpdateOutputInformation();

            m_ReflectanceThresholdFilter->SetInput4(m_ErodeWaterMaskFilter->GetOutput());
        }
        else
        {
            // The Water_mask_sub(D-1) doesn't exist
            m_ReflectanceThresholdFilter->SetInput4(m_ConstantMaskFilter->GetOutput());
        }

        m_ReflectanceThresholdFilter->GetFunctor().SetForegroundValue(m_ForegroundValue);
        m_ReflectanceThresholdFilter->GetFunctor().SetBlueBand(m_BlueBandTOCR);
        m_ReflectanceThresholdFilter->GetFunctor().SetRedBand(m_RedBandTOCR);
        m_ReflectanceThresholdFilter->GetFunctor().SetNirBand(m_NIRBandTOCR);
        if (m_HasSwirBand)
        {
            m_ReflectanceThresholdFilter->GetFunctor().SetSwirBand(m_SWIRBandTOCR);
            m_ReflectanceThresholdFilter->GetFunctor().SetNirSwirRatioMinThreshold(m_NirSwirRatioMinThresholdValue);
        }
        m_ReflectanceThresholdFilter->GetFunctor().SetWaterBand(m_ReflectanceWaterBand);
        m_ReflectanceThresholdFilter->GetFunctor().SetBlueReflectanceThreshold(m_BlueReflectanceThresholdValue);
        m_ReflectanceThresholdFilter->GetFunctor().SetRedReflectanceThreshold(m_RedReflectanceThresholdValue);
        m_ReflectanceThresholdFilter->GetFunctor().SetNirRedRatioMaxThreshold(m_NirRedRatioMaxThresholdValue);
        m_ReflectanceThresholdFilter->GetFunctor().SetNirRedRatioMinThreshold(m_NirRedRatioMinThresholdValue);
        m_ReflectanceThresholdFilter->GetFunctor().SetWaterReflectanceThreshold(l_WaterThreshold);

        if (m_InitMode == false)
        {

            // Definition of the structuring element used for the dilatation of the water and the snow masks
            StructuringElementType l_SEMask;
            l_SEMask.SetRadius(m_SEInputMasksRadius); // 3x3 structuring element
            l_SEMask.CreateStructuringElement();

            // dilatation of water and snow mask
            // Dilate water mask
            m_DilateWaterMaskFilter->SetInput(l_L2inWASPtr);
            m_DilateWaterMaskFilter->SetDilateValue(1);
            m_DilateWaterMaskFilter->SetKernel(l_SEMask);
            m_DilateWaterMaskFilter->UpdateOutputInformation();

            if (l_IPSNWSubPtr.IsNull() == false)
            {
                (dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(14)))->UpdateOutputInformation();
                //Or filter to take into account the Water mask and the Snow Mask in the same time.
                // Dilate snow mask
                m_DilateSnowMaskFilter->SetInput(l_IPSNWSubPtr);
                m_DilateSnowMaskFilter->SetDilateValue(1);
                m_DilateSnowMaskFilter->SetKernel(l_SEMask);

                m_OrFilter2->SetInput1(m_DilateWaterMaskFilter->GetOutput());
                m_OrFilter2->SetInput2(m_DilateSnowMaskFilter->GetOutput());

                m_ReflectanceVariationCorrelationFilter->SetConditionalMaskInput(m_OrFilter2->GetOutput());

            }
            else
            {
            	m_ReflectanceVariationCorrelationFilter->SetConditionalMaskInput(m_DilateWaterMaskFilter->GetOutput());
            }

            // Reflectance variation threshold only in standard mode
            m_ReflectanceVariationCorrelationFilter->SetIPTOCRInput(l_IPTOCRPtr);
            m_ReflectanceVariationCorrelationFilter->SetL2inRCRInput(l_L2inRCRPtr);
            m_ReflectanceVariationCorrelationFilter->SetL2inSTOInput(l_L2inSTOPtr);

            m_ReflectanceVariationCorrelationFilter->SetL2inPXDInput(l_L2inPXDPtr);
            m_ReflectanceVariationCorrelationFilter->SetIPEDGInput(l_IPEDGPtr);
            m_ReflectanceVariationCorrelationFilter->SetL2inNDTInput(l_L2inNDTPtr);
            //m_ReflectanceVariationCorrelationFilter->SetL2inWASInput(m_DilateWaterMaskFilter->GetOutput());
            m_ReflectanceVariationCorrelationFilter->SetIPCLDReflInput(m_ReflectanceThresholdFilterOutputReader->GetOutput());

            m_ReflectanceVariationCorrelationFilter->SetNoData(m_NoData);
            m_ReflectanceVariationCorrelationFilter->SetBlueBandTOCR(m_BlueBandTOCR);
            m_ReflectanceVariationCorrelationFilter->SetRedBandTOCR(m_RedBandTOCR);
            m_ReflectanceVariationCorrelationFilter->SetNIRBandTOCR(m_NIRBandTOCR);
            m_ReflectanceVariationCorrelationFilter->SetCorrelBandTOCR(m_CorrelBandTOCR);
            m_ReflectanceVariationCorrelationFilter->SetBlueBandRCR(m_BlueBandRCR);
            m_ReflectanceVariationCorrelationFilter->SetRedBandRCR(m_RedBandRCR);
            m_ReflectanceVariationCorrelationFilter->SetNIRBandRCR(m_NIRBandRCR);
            m_ReflectanceVariationCorrelationFilter->SetCorrelBandRCR(m_CorrelBandRCR);

            m_ReflectanceVariationCorrelationFilter->SetJDay(m_JDay);
            m_ReflectanceVariationCorrelationFilter->SetSTOListOfDates(m_STOListOfDates);
            m_ReflectanceVariationCorrelationFilter->SetForgettingDuration(m_CloudForgettingDuration);
            m_ReflectanceVariationCorrelationFilter->SetMinVarThreshold(m_MinVarThreshold);
            m_ReflectanceVariationCorrelationFilter->SetMaxVarThreshold(m_MaxVarThreshold);
            m_ReflectanceVariationCorrelationFilter->SetKNIRBlue(m_KNIRBlue);
            m_ReflectanceVariationCorrelationFilter->SetKRedBlue(m_KRedBlue);
            m_ReflectanceVariationCorrelationFilter->SetDeltaThreshold(m_DeltaThreshold);
            m_ReflectanceVariationCorrelationFilter->SetMinPixelCorrelation(m_MinPixelCorrelation);
            m_ReflectanceVariationCorrelationFilter->SetCorrelThreshold(m_CorrelThreshold);
            m_ReflectanceVariationCorrelationFilter->SetNCorrel(m_NCorrel);

            // UpdateOutputInformation to have access to the size for the vnsChecking2ImagesSizeMacro
            vnsLaunchBasicOnlyCommandInDebug(m_ReflectanceVariationCorrelationFilter->UpdateOutputInformation())

            // Label the mask of the extension of clouds
            m_IPCLDExtLabelFilter->SetInput(m_ReflectanceVariationCorrelationFilter->GetIPCLDExtOutput());

            vnsLaunchBasicOnlyCommandInDebug(m_IPCLDExtLabelFilter->UpdateOutputInformation())
            // Force the caching even if it seems useless : the itkConnectedComponentImageFilter is not streamed... thus we prevent some bad news !
            // Use a specific Caching Macro (that call an otbImageFileWriter) because using StreamingImageFileWriter generates which each stream is the first input image one.
            vnsCachingMacro(m_IPCLDLabelReader, LabelImageType, m_IPCLDExtLabelFilter->GetOutput(), "IPCLDExtLabelFilterOutput");

            vnsCrocodileClipMacro(LabelImageType, m_IPCLDExtLabelFilter->GetOutput(), "CloudMasking_IPCLDExtLabelFilter_GetOutput.tif")

            // Cloud mask extension test
            // Input images
            m_CloudMaskExtensionFilter->SetIPTOCRInput(l_IPTOCRPtr);
            m_CloudMaskExtensionFilter->SetL2inRCRInput(l_L2inRCRPtr);
            m_CloudMaskExtensionFilter->SetL2inSTOInput(l_L2inSTOPtr);
            m_CloudMaskExtensionFilter->SetIPCLDReflInput(m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput()); //m_ReflectanceThresholdFilterOutputReader->GetOutput());

            m_CloudMaskExtensionFilter->SetIPCLDReflVarInput(m_ReflectanceVariationCorrelationFilter->GetIPCLDReflVarOutput());

            m_CloudMaskExtensionFilter->SetIPCLDExtInput(m_ReflectanceVariationCorrelationFilter->GetIPCLDExtOutput());

            m_CloudMaskExtensionFilter->SetIPCLDExtLabelInput(m_IPCLDLabelReader->GetOutput());
            m_CloudMaskExtensionFilter->SetIPEDGInput(l_IPEDGPtr);
            m_CloudMaskExtensionFilter->SetL2inNDTInput(l_L2inNDTPtr);

            vnsLogDebugMacro("m_IPCLDExtLabelFilter->GetObjectCount() "<<m_IPCLDExtLabelFilter->GetObjectCount());

            m_CloudMaskExtensionFilter->SetLabelCount(m_IPCLDExtLabelFilter->GetObjectCount());

            m_CloudMaskExtensionFilter->SetNoData(m_NoData);
            // 4-1-0 : sensor mixing
            m_CloudMaskExtensionFilter->SetCorrelBandTOCR(m_CorrelBandTOCR);
            m_CloudMaskExtensionFilter->SetCorrelBandRCR(m_CorrelBandRCR);
            m_CloudMaskExtensionFilter->SetMinPixelCorrelation(m_MinPixelCorrelation);
            m_CloudMaskExtensionFilter->SetCorrelThreshold(m_CorrelThreshold);
            m_CloudMaskExtensionFilter->SetNCorrel(m_NCorrel);
            // LAIG-FA-MAC-1180-CNES : no correlation if the current date is equal to the STO one
            m_CloudMaskExtensionFilter->SetJDay(m_JDay);
            m_CloudMaskExtensionFilter->SetSTOListOfDates(m_STOListOfDates);

            vnsLaunchBasicOnlyCommandInDebug(m_CloudMaskExtensionFilter->UpdateWithCaching())

            // If remove, don't have same results... even with caching ...
            m_CloudMaskExtensionFilter->GetIPCLDExtOutput()->Update();

            vnsCrocodileClipMacro(InputMaskType, m_CloudMaskExtensionFilter->GetIPCLDExtOutput(),
                    "CloudMasking_CloudMaskExtensionFilter_GetIPCLDExtOutput.tif")
            vnsCrocodileClipMacro(InputMaskType, m_CloudMaskExtensionFilter->GetIPCLDExtCorelFlagOutput(),
                    "CloudMasking_CloudMaskExtensionFilter_GetIPCLDExtCorelFlagOutput.tif")

            // Correlation test post-processing (5.0.0-DM982)
            // -> TernaryLogicalFilter (refl || reflvar || ext): total cloud mask
            m_TmpCloudSynthFilter->SetInput1(m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput()); //m_ReflectanceThresholdFilterOutputReader->GetOutput());
            m_TmpCloudSynthFilter->SetInput2(m_ReflectanceVariationCorrelationFilter->GetIPCLDReflVarOutput());
            m_TmpCloudSynthFilter->SetInput3(m_CloudMaskExtensionFilter->GetIPCLDExtOutput());
            vnsLaunchBasicOnlyCommandInDebug(m_TmpCloudSynthFilter->GetOutput()->UpdateOutputInformation())

            vnsCrocodileClipMacro(InputMaskType, m_TmpCloudSynthFilter->GetOutput(), "CloudMasking_TmpCloudSynthFilter_GetOutput.tif")

            // -> Total cloud mask to label map
            m_TmpCloudLabelFilter->SetInput(m_TmpCloudSynthFilter->GetOutput());
            m_TmpCloudLabelFilter->UpdateOutputInformation();
            // Force the caching even if it seems useless : the itkConnectedComponentImageFilter is not streamed... thus we prevent some bad news !
            // Use a specific Caching Macro (that call an otbImageFileWriter) because using StreamingImageFileWriter generates which each stream is the first input image one.
            vnsCachingMacro(m_TmpCloudLabelReader, LabelImageType, m_TmpCloudLabelFilter->GetOutput(), "TmpCloudLabelOutput");

            vnsCrocodileClipMacro(LabelImageType, m_TmpCloudLabelFilter->GetOutput(), "CloudMasking_TmpCloudLabelFilter_GetOutput.tif")

            // Compute statistics over labels
            m_CloudMaskExtensionDiscardedPixelValidationFilter->SetInput(m_CloudMaskExtensionFilter->GetIPCLDExtCorelFlagOutput());
            m_CloudMaskExtensionDiscardedPixelValidationFilter->SetInputLabelImage(m_TmpCloudLabelReader->GetOutput());
            vnsLogDebugMacro("this->GetDiscardedPixelsRatioMin() = "<< this->GetDiscardedPixelsRatioMin())

            m_CloudMaskExtensionDiscardedPixelValidationFilter->SetLowerValidityThreshold(this->GetDiscardedPixelsRatioMin());
            vnsLaunchBasicOnlyCommandInDebug(m_CloudMaskExtensionDiscardedPixelValidationFilter->Update())

            vnsLogMapSuperDebugMacro("CloudMaskExtensionDiscardedPixelValidation map",
                    m_CloudMaskExtensionDiscardedPixelValidationFilter->GetLabelValidityStatusMap())

            // Filter Potentially discarded pixels (from Correlation in ext step)
            m_CloudMaskExtensionPostProcessingFilter->SetMaskInput(m_CloudMaskExtensionFilter->GetIPCLDExtOutput());
            m_CloudMaskExtensionPostProcessingFilter->SetExtCorrelFlagInput(m_CloudMaskExtensionFilter->GetIPCLDExtCorelFlagOutput());
            m_CloudMaskExtensionPostProcessingFilter->SetLabelInput(m_TmpCloudLabelReader->GetOutput());
            m_CloudMaskExtensionPostProcessingFilter->SetValidityMap(
                    m_CloudMaskExtensionDiscardedPixelValidationFilter->GetLabelValidityStatusMap());

            vnsCrocodileClipMacro(InputMaskType, m_CloudMaskExtensionPostProcessingFilter->GetOutput(),
                    "CloudMasking_CloudMaskExtensionPostProcessingFilter_GetOutput.tif")

            // Set the cloud mask produced by the reflectance variation threshold algorithm
            // for the snow mask correction algorithms
            m_SnowMaskCorrectionFilter->SetIPCLDReflVarSubInput(m_ReflectanceVariationCorrelationFilter->GetIPCLDReflVarOutput());

            // third input of the NaryLogical used to compute the cloud_mask_sub.all clouds
            vnsLogImageInfoDebugMacro("NaryLogicalFilter Input3", m_ReflectanceVariationCorrelationFilter->GetIPCLDReflVarOutput())
            m_NaryLogicalFilter->SetInput(2, m_ReflectanceVariationCorrelationFilter->GetIPCLDReflVarOutput());

        } // end if init_mode false
        else
        {
            // for the snow mask correction algorithms
            m_SnowMaskCorrectionFilter->SetIPCLDReflVarSubInput(m_ConstantMaskFilter->GetOutput());

            // Last input of the TernaryLogical filter is a constant image in init_mode
            vnsLogImageInfoDebugMacro("TernaryLogicalFilter Input3", m_ConstantMaskFilter->GetOutput())
            m_NaryLogicalFilter->SetInput(2, m_ConstantMaskFilter->GetOutput());
        }

        // Snow mask correction
        if (l_IPSNWSubPtr.IsNull() == false)
        {

            m_SnowMaskCorrectionFilter->SetIPSNWSubInput(l_IPSNWSubPtr);
            m_SnowMaskCorrectionFilter->SetIPEDGSubInput(l_IPEDGPtr);

            if (m_InitMode == false)
            {
                m_SnowMaskCorrectionFilter->SetIPCLDReflSubInput(m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput()); //m_ReflectanceThresholdFilterOutputReader->GetOutput());
            }
            else
            {
                m_SnowMaskCorrectionFilter->SetIPCLDReflSubInput(m_ReflectanceThresholdFilterOutputReader->GetOutput()); //m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput()); //m_ReflectanceThresholdFilterOutput);
            }
            // the cloud_mask_sub.refl_var is set above
            m_SnowMaskCorrectionFilter->SetDilateRadius(m_SEInputMasksRadius);
            m_SnowMaskCorrectionFilter->SetOutputMaskValue(static_cast<InputMaskPixelType>(1));

            vnsLaunchCommand(m_SnowMaskCorrectionFilter->UpdateWithCaching(), "Launch the Snow mask correction processing");

            // ========> Get the corrected snow output mask
            m_IPSNWSubOutput = m_SnowMaskCorrectionFilter->GetIPSNWSubOutput();

            vnsCrocodileClipMacro(InputMaskType, m_SnowMaskCorrectionFilter->GetIPCLDReflSubOutput(),
                    "CloudMasking_SnowMaskCorrectionFilter_GetIPCLDReflSubOutput.tif")

            // Set the cloud mask produced by the reflectance threshold algorithm
            vnsLogImageInfoDebugMacro("NaryLogicalFilter Input2", m_SnowMaskCorrectionFilter->GetIPCLDReflSubOutput())
            m_NaryLogicalFilter->SetInput(1,m_SnowMaskCorrectionFilter->GetIPCLDReflSubOutput());

        }
        // If the snow mask is not available
        else
        {
            if (m_InitMode == false)
            {
                m_NaryLogicalFilter->SetInput(1,m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput()); //m_ReflectanceThresholdFilterOutput);
            }
            else
            {
                vnsLogImageInfoDebugMacro("NaryLogicalFilter Input2", m_ReflectanceThresholdFilterOutputReader->GetOutput())
                m_NaryLogicalFilter->SetInput(1, m_ReflectanceThresholdFilterOutputReader->GetOutput()); //m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput()); //m_ReflectanceThresholdFilterOutput);
            }
        }

        // Synthesis of stereoscopic and radiometric criteria (using a logical or)
        vnsLogImageRawInfoDebugMacro("NaryLogicalFilter Input1", l_L1CLDAltPtr)
        m_NaryLogicalFilter->SetInput(0,l_L1CLDAltPtr);
        // the radimetric criteria has been set above
        vnsLaunchBasicOnlyCommandInDebug(m_ReflectanceThresholdFilter->UpdateOutputInformation())

        // Not delete that !
        vnsLaunchBasicOnlyCommandInDebug(m_NaryLogicalFilter->GetOutput()->UpdateOutputInformation())

        // Or with Cirrus_Mask if present !
        if (l_IPCirrusMaskSubPtr.IsNull() == false)
        {
            (dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(15)))->UpdateOutputInformation();
            m_OrFilter3->SetInput1(l_IPCirrusMaskSubPtr);
            m_OrFilter3->SetInput2(m_NaryLogicalFilter->GetOutput());
            // Call the "caching" not to call this filter for each stream
            vnsCachingMacro(m_CloudMaskAllNoExtensionOutputReader, InputMaskType, m_OrFilter3->GetOutput(), "CloudMaskAllNoExtensionOutput")
        }
        else
        {
            // Call the "caching" not to call this filter for each stream
            vnsCachingMacro(m_CloudMaskAllNoExtensionOutputReader, InputMaskType, m_NaryLogicalFilter->GetOutput(), "CloudMaskAllNoExtensionOutput")
        }

        //Add extension to cloud mask all
        if (m_InitMode == false)
        {
            m_OrFilterExtensionAll->SetInput1(m_CloudMaskAllNoExtensionOutputReader->GetOutput());
            m_OrFilterExtensionAll->SetInput2(m_CloudMaskExtensionPostProcessingFilter->GetOutput());
            // Call the "caching" not to call this filter for each stream
            vnsCachingMacro(m_CloudMaskAllOutputReader, InputMaskType, m_OrFilterExtensionAll->GetOutput(), "CloudMaskAllOutput")
        }
        else
        {
            // Call the "caching" not to call this filter for each stream
            vnsCachingMacro(m_CloudMaskAllOutputReader, InputMaskType, m_CloudMaskAllNoExtensionOutputReader->GetOutput(), "CloudMaskAllOutput")
        }

        // Dilatation of all cloud mask
        OutputImageInternalPixelType l_DilateValue(1);
        StructuringElementType l_SECloud;
        l_SECloud.SetRadius(m_SECloudRadius); // 3x3 structuring element
        l_SECloud.CreateStructuringElement();
        m_IPCLDAllNoExtensionDilateFilter->SetKernel(l_SECloud);
        m_IPCLDAllNoExtensionDilateFilter->SetInput(m_CloudMaskAllNoExtensionOutputReader->GetOutput());
        m_IPCLDAllNoExtensionDilateFilter->SetDilateValue(l_DilateValue);
        vnsLaunchBasicOnlyCommandInDebug(m_IPCLDAllNoExtensionDilateFilter->UpdateOutputInformation())
        m_IPCLDAllDilateFilter->SetKernel(l_SECloud);
        m_IPCLDAllDilateFilter->SetInput(m_CloudMaskAllOutputReader->GetOutput());
        m_IPCLDAllDilateFilter->SetDilateValue(l_DilateValue);
        vnsLaunchBasicOnlyCommandInDebug(m_IPCLDAllDilateFilter->UpdateOutputInformation())

        // Labellization of clouds
        m_LabelFilter->SetInput(m_CloudMaskAllNoExtensionOutputReader->GetOutput());
        // Use a specific Caching Macro (that call an otbImageFileWriter) because using StreamingImageFileWriter generates which each stream is the first input image one.
        vnsCachingMacro(m_LabelReader, LabelImageType, m_LabelFilter->GetOutput(), "LabelFilterOutput");

        vnsLogDebugMacro("Object count " << m_LabelFilter->GetObjectCount());

        // Assign an altitude to each pixel within each label
        m_StereoCorrectionFilter->SetIPCLAInput(l_L1CLAPtr);
        m_StereoCorrectionFilter->SetIPCLDAllInput(m_CloudMaskAllNoExtensionOutputReader->GetOutput());

        m_StereoCorrectionFilter->SetExcludedValue(static_cast<CLAImagePixelType>(0));
        m_StereoCorrectionFilter->SetDefaultValue(static_cast<LabelImagePixelType>(0));
        m_StereoCorrectionFilter->SetKsigmaAltitude(m_KsigmaAltitude);
        m_StereoCorrectionFilter->SetDefaultAltitude(m_DefaultAltitude);

        vnsLaunchBasicOnlyCommandInDebug(m_StereoCorrectionFilter->UpdateWithCaching())
        vnsCrocodileClipMacro(CLAImageType, m_StereoCorrectionFilter->GetIPCLAOutput(),
                "CloudMasking_StereoCorrectionFilter_GetIPCLAOutput.tif")

        // Projection of clouds shadows and iteration on altitude
        if (m_CloudMaskingKnownCloudsAltitude == true) //TODO: clean up nonVenus specific code
        {
            m_ShadowMaskDeterminationFilter->SetIPTOCRInput(l_IPTOCRPtr);
            m_ShadowMaskDeterminationFilter->SetIPEDGInput(l_IPEDGPtr);
            m_ShadowMaskDeterminationFilter->SetL2CLDSubInput(m_CloudMaskAllNoExtensionOutputReader->GetOutput());
            m_ShadowMaskDeterminationFilter->SetIPVIEInput(l_IPVIEPtr);
            m_ShadowMaskDeterminationFilter->SetIPSOL1Input(l_IPSOL1Ptr);
            m_ShadowMaskDeterminationFilter->SetIPSOL2Input(l_IPSOL2Ptr);
            m_ShadowMaskDeterminationFilter->SetIPCLAInput(m_StereoCorrectionFilter->GetIPCLAOutput());
            m_ShadowMaskDeterminationFilter->SetDTMInput(l_DTMPtr);

            m_LabelReader->GenerateOutputInformation();
            m_ShadowMaskDeterminationFilter->SetL2CLDSubLabelInput(m_LabelReader->GetOutput());

            // In init mode, the composite product is not available
            if (m_InitMode == false)
            {
                m_ShadowMaskDeterminationFilter->SetL2inRCRInput(l_L2inRCRPtr);
                m_ShadowMaskDeterminationFilter->SetL2inNDTInput(l_L2inNDTPtr);
                m_ShadowMaskDeterminationFilter->SetL2inWASInput(l_L2inWASPtr);
            }

            // parameters
            m_ShadowMaskDeterminationFilter->SetNoData(m_NoData);
            m_ShadowMaskDeterminationFilter->SetL2CoarseResolution(m_L2CoarseResolution);
            m_ShadowMaskDeterminationFilter->SetInitMode(m_InitMode);
            m_ShadowMaskDeterminationFilter->SetRefinementOption(m_RefinementOption);
            m_ShadowMaskDeterminationFilter->SetDeltaHMax(m_DeltaHMax);
            m_ShadowMaskDeterminationFilter->SetDeltaHMin(m_DeltaHMin);
            m_ShadowMaskDeterminationFilter->SetDeltaHStep(m_DeltaHStep);
            m_ShadowMaskDeterminationFilter->SetSOLH1(m_SOLH1);
            m_ShadowMaskDeterminationFilter->SetSOLH2(m_SOLH2);
            m_ShadowMaskDeterminationFilter->SetSOLHRef(m_SOLHRef);
            m_ShadowMaskDeterminationFilter->SetVIEHRef(m_VIEHRef);
            m_ShadowMaskDeterminationFilter->SetAbsNbpixThreshold(m_AbsNbpixThreshold);
            m_ShadowMaskDeterminationFilter->SetThresholdDiffImage(m_ThresholdDiffImage);
            m_ShadowMaskDeterminationFilter->SetShadowBandTOCR(m_ShadowBandTOCR);
            m_ShadowMaskDeterminationFilter->SetShadowBandRCR(m_ShadowBandRCR);

            // Call the main of the ShadowMaskDeterminationFilter
            vnsLaunchBasicOnlyCommandInDebug(m_ShadowMaskDeterminationFilter->UpdateWithCaching())

            vnsCachingMacro(m_ShadowMaskDeterminationFilterIPCLDShadOutputReader, InputMaskType,
                    m_ShadowMaskDeterminationFilter->GetIPCLDShadOutput(), "ShadowMaskDeterminationFilterIPCLDShadOutput");

            // ========> Set the cloud altitude output mask
            vnsCachingRealDataSingleImageMacro(m_IPCLAOutputReader, m_ShadowMaskDeterminationFilter->GetIPCLAOutput(), "CloudMaskingIPCLA");
            m_L2CLAOutput = m_IPCLAOutputReader->GetOutput();
        }
        else //Not Venus -> DM982:
        {
            // In init mode, the composite product is not available
            if (m_InitMode == false)
            {
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetNoData(m_NoData);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2CoarseResolution(m_L2CoarseResolution);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDeltaHMin(m_DeltaHMin);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDeltaHMax(m_DeltaHMax);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDeltaHStep(m_DeltaHStep);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowBandTOCR(m_ShadowBandTOCR);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowBandRCR(m_ShadowBandRCR);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetSOLH1(m_SOLH1);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetVIEHRef(m_VIEHRef);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetCloudCoverMultFactor(this->GetCloudCoverMultFactor());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowCloudRatioMax(this->GetShadowCloudRatioMax());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowCloudRatioMin(this->GetShadowCloudRatioMin());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowCloudRatioPixNb(this->GetShadowCloudRatioPixNb());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetProjShadDarkeningMax(this->GetProjShadDarkeningMax());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDarkeningMinPercentile(this->GetDarkeningMinPercentile());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2inRCRInput(l_L2inRCRPtr);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2inNDTInput(l_L2inNDTPtr);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2inWASInput(l_L2inWASPtr);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPTOCRInput(l_IPTOCRPtr);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2CLDSubInput(m_CloudMaskAllNoExtensionOutputReader->GetOutput());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPEDGInput(l_IPEDGPtr);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2CLDDilatedSubInput(m_IPCLDAllNoExtensionDilateFilter->GetOutput());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPVIEInput(l_IPVIEPtr);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPSOLInput(l_IPSOL1Ptr);
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPCLAInput(m_StereoCorrectionFilter->GetIPCLAOutput());
                m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDTMInput(l_DTMPtr);

                // Call the main of the ShadowMaskDeterminationFilter
                vnsLaunchBasicOnlyCommandInDebug(m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->UpdateWithCaching())

                vnsLogDebugValueMacro(m_ShadowMaskDeterminationWithoutCloudAltitudeFilter)

                vnsCachingMacro(m_ShadowMaskDeterminationFilterIPCLDShadOutputReader, InputMaskType,
                        m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->GetIPCLDShadOutput(),
                        "ShadowMaskDeterminationFilterIPCLDShadOutput");

            }
            else //init mode: no shadow mask determination
            {
                //
                vnsCachingMacro(m_ShadowMaskDeterminationFilterIPCLDShadOutputReader, InputMaskType, m_ConstantMaskFilter->GetOutput(),
                        "ShadowMaskDeterminationFilterIPCLDShadOutput");
            }
            // LAIG-DM-MAC-1691-CNES : on ecrit pas le plan CLA
            m_L2CLAOutput = NULL;
        }

        vnsCrocodileClipMacro(InputMaskType, m_ShadowMaskDeterminationFilterIPCLDShadOutputReader->GetOutput(),
                "CloudMasking_ShadowMaskDeterminationFilter_GetIPCLDShadOutput.tif")

        if (m_InitMode == false)
        {
            // Generate a constant image with max cloud altitude
            m_CLAConstantFilter->SetSize(l_IPEDGPtr->GetLargestPossibleRegion().GetSize());
            m_CLAConstantFilter->SetSpacing(l_IPEDGPtr->GetSpacing());
            m_CLAConstantFilter->SetOrigin(l_IPEDGPtr->GetOrigin());
            m_CLAConstantFilter->SetConstantValue(static_cast<CLAImagePixelType>(m_MaxCLA));

            if (l_IPSOL2Ptr.IsNull() == true)
            {
                m_OutsideCloudShadowMaskFilter->SetIPSOLInput(l_IPSOL1Ptr);
                m_OutsideCloudShadowMaskFilter->SetSOLHRef(m_SOLH1);
            }
            else
            {
                // Estimate the solar direction for the max cloud altitude
                // Two solar reference grids for two altitudes are available
                m_SolarFilter->SetInput1(l_IPSOL1Ptr);
                m_SolarFilter->SetInput2(l_IPSOL2Ptr);
                m_SolarFilter->SetInput3(m_CLAConstantFilter->GetOutput());

                m_SolarFilter->GetFunctor().SetH1(m_SOLH1);
                m_SolarFilter->GetFunctor().SetH2(m_SOLH2);
                m_SolarFilter->GetFunctor().SetSOLHRef(m_SOLHRef);

                m_OutsideCloudShadowMaskFilter->SetIPSOLInput(m_SolarFilter->GetOutput());
                m_OutsideCloudShadowMaskFilter->SetSOLHRef(m_MaxCLA);

                vnsCrocodileClipMacro(GRIDImageType, l_IPSOL2Ptr, "CloudMaskingSolarFilterIPSOL2.tif")
                vnsCrocodileClipMacro(GRIDImageType, m_SolarFilter->GetOutput(), "CloudMaskingSolarFilterIPSOL.tif")
            }
            vnsCrocodileClipMacro(GRIDImageType, l_IPSOL1Ptr, "CloudMaskingSolarFilterIPSOL1.tif")
            vnsCrocodileClipMacro(GRIDImageType, l_IPVIEPtr, "CloudMaskingSolarFilterIPVIE.tif")

            // Projection of clouds outside the image
            m_OutsideCloudShadowMaskFilter->SetIPEDGInput(l_IPEDGPtr);
            m_OutsideCloudShadowMaskFilter->SetDTMInput(l_DTMPtr);
            m_OutsideCloudShadowMaskFilter->SetIPVIEInput(l_IPVIEPtr);

            m_OutsideCloudShadowMaskFilter->SetL2CoarseResolution(m_L2CoarseResolution);
            m_OutsideCloudShadowMaskFilter->SetMaxCLA(m_MaxCLA);
            m_OutsideCloudShadowMaskFilter->SetVIEHRef(m_VIEHRef);
            vnsLaunchBasicOnlyCommandInDebug(m_OutsideCloudShadowMaskFilter->UpdateOutputInformation())

            vnsCrocodileClipMacro(InputMaskType, m_OutsideCloudShadowMaskFilter->GetIPCLDOutShadOutput(),
                    "CloudMasking_OutsideCloudShadowMaskFilter_GetIPCLDOutShadOutput.tif")

            // Radiometric criterion to detect shadow in the zone
            m_ReflRatioFilter->SetIPTOCRInput(l_IPTOCRPtr);
            m_ReflRatioFilter->SetL2inRCRInput(l_L2inRCRPtr);
            m_ReflRatioFilter->SetL2CLDSubInput(m_CloudMaskAllNoExtensionOutputReader->GetOutput());
            m_ReflRatioFilter->SetIPEDGInput(l_IPEDGPtr);
            m_ReflRatioFilter->SetL2inWASInput(l_L2inWASPtr);
            m_ReflRatioFilter->SetNoData(m_NoData);
            m_ReflRatioFilter->SetShadowBandTOCR(m_ShadowBandTOCR);
            m_ReflRatioFilter->SetShadowBandRCR(m_ShadowBandRCR);

            vnsCrocodileClipMacro(RatioImageType, m_ReflRatioFilter->GetOutput(), "CloudMasking_ReflRatioFilter_GetOutput.tif")

            // Call the "caching" not to call the previous filter for each stream
            vnsCachingRealDataSingleImageMacro(m_ReflRatioFilterOutputReader, m_ReflRatioFilter->GetOutput(), "ShadowVarMaskDetRatioRefl")

            // Verify if there are enougth shadow pixel to proceed
            // compute cloud cover
            m_IPCLDShadCountFilter->SetImage(m_ShadowMaskDeterminationFilterIPCLDShadOutputReader->GetOutput());
            m_IPCLDShadCountFilter->SetPixelValue(static_cast<InputMaskPixelType>(1));
            vnsLaunchBasicOnlyCommandInDebug(m_IPCLDShadCountFilter->Update());
            const unsigned long l_IPCLDShadPixNb(m_IPCLDShadCountFilter->GetValueCount());
            vnsLogDebugMacro("m_IPCLDShadCountFilter->GetValueCount(): "<< l_IPCLDShadPixNb)

            if (l_IPCLDShadPixNb > 0)
            {
                m_ReflRatioMaskingFilter->SetInput1(m_ReflRatioFilterOutputReader->GetOutput());
                m_ReflRatioMaskingFilter->SetInput2(m_ShadowMaskDeterminationFilterIPCLDShadOutputReader->GetOutput());
                m_ReflRatioMaskingFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskPixelType>(0));
                m_ReflRatioMaskingFilter->GetFunctor().SetReplaceValue(static_cast<InputInternalPixelType>(0));
                vnsLaunchBasicOnlyCommandInDebug(m_ReflRatioMaskingFilter->UpdateOutputInformation())

                vnsCrocodileClipMacro(RatioImageType, m_ReflRatioMaskingFilter->GetOutput(),
                        "CloudMasking_ReflRatioMaskingFilter_GetOutput.tif")

                m_ReflRatioMaskedToVectorImage->SetInput(m_ReflRatioMaskingFilter->GetOutput());

                vnsCrocodileClipMacro(InputImageType, m_ReflRatioMaskedToVectorImage->GetOutput(),
                        "CloudMasking_ReflRatioMaskedToVectorImage_GetOutput.tif")

                // Radiometric Threshold Estimation
                PersistentShadowVariationThresholdValueEstimator<InputImageType>::CountVectorType l_NumberOfBins(1); // one component only
                l_NumberOfBins[0] = 1000;
                m_ShadVarThresholdValueEstimatorFilter->SetReflRatio(m_ReflRatioMaskedToVectorImage->GetOutput());
                m_ShadVarThresholdValueEstimatorFilter->SetNoDataValue(m_NoData);
                m_ShadVarThresholdValueEstimatorFilter->SetShadVarPercentile(this->GetShadVarPercentile());   //XML Handler
                m_ShadVarThresholdValueEstimatorFilter->SetShadVarDarkeningMax(this->GetShadVarDarkeningMax()); //XML Handler
                vnsLaunchBasicOnlyCommandInDebug(m_ShadVarThresholdValueEstimatorFilter->Update())

                vnsLogDebugValueMacro(m_ShadVarThresholdValueEstimatorFilter)

                vnsLogDebugValueMacro(m_ShadVarThresholdValueEstimatorFilter->GetThresholdValue())

                // Radiometric Thresholding
                //m_ShadowVariationThresholdFilter->SetIPTOCRInput(l_IPTOCRPtr);
                m_ShadowVariationThresholdFilter->SetReflRatio(m_ReflRatioFilterOutputReader->GetOutput());
                m_ShadowVariationThresholdFilter->SetNoData(m_NoData);
                m_ShadowVariationThresholdFilter->SetThresholdValue(m_ShadVarThresholdValueEstimatorFilter->GetThresholdValue());
                m_ShadowVariationThresholdFilter->SetIPCLDOutShadInput(m_OutsideCloudShadowMaskFilter->GetIPCLDOutShadOutput());

                vnsCrocodileClipMacro(InputMaskType, m_ShadowVariationThresholdFilter->GetIPCLDShadVarOutput(),
                        "CloudMasking_ShadowVariationThresholdFilter_GetIPCLDShadVarOutput.tif")

                vnsCachingMacro(m_ShadowVariationThresholdFilterIPCLDShadVarOutputReader, InputMaskType,
                        m_ShadowVariationThresholdFilter->GetIPCLDShadVarOutput(), "ShadowVariationThresholdFilter_GetIPCLDShadVarOutput")

                // Labelisation of ShadVarTemp
                m_ShadVarLabelFilter->SetInput(m_ShadowVariationThresholdFilterIPCLDShadVarOutputReader->GetOutput());
                vnsLaunchBasicOnlyCommandInDebug(m_ShadVarLabelFilter->UpdateOutputInformation())

                vnsCrocodileClipMacro(LabelImageType, m_ShadVarLabelFilter->GetOutput(), "CloudMasking_ShadVarLabelFilter_GetOutput.tif")

                vnsCachingMacro(m_ShadVarLabelReader, LabelImageType, m_ShadVarLabelFilter->GetOutput(), "ShadVarLabelOutput");

                // ShadVarTemp post-processing (over labels)
                m_ShadowVariationMaskCompositeCombinaisonFilter->SetInput1(l_L2inShadVarPtr);                //D-1 shadvar
                m_ShadowVariationMaskCompositeCombinaisonFilter->SetInput2(l_L2inShadowPtr);                //D-1 shadow
                m_ShadowVariationMaskCompositeCombinaisonFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskPixelType>(1));
                m_ShadowVariationMaskCompositeCombinaisonFilter->GetFunctor().SetReplaceValue(static_cast<InputMaskPixelType>(0));
                vnsLaunchBasicOnlyCommandInDebug(m_ShadowVariationMaskCompositeCombinaisonFilter->UpdateOutputInformation())

                vnsCrocodileClipMacro(LabelImageType, m_ShadVarLabelFilter->GetOutput(),
                        "CloudMasking_ShadowVariationMaskCompositeCombinaisonFilter_GetOutput.tif")

                m_ShadowVariationPixelValidationFilter->SetInput(m_ShadowVariationMaskCompositeCombinaisonFilter->GetOutput());
                m_ShadowVariationPixelValidationFilter->SetInputLabelImage(m_ShadVarLabelReader->GetOutput());
                m_ShadowVariationPixelValidationFilter->SetLowerValidityThreshold(this->GetShadVarCompositeLabelRatio());
                vnsLaunchBasicOnlyCommandInDebug(m_ShadowVariationPixelValidationFilter->Update())

                /// Filtering

                m_ShadowVariationPixelDiscardingFilter->SetMaskInput(m_ShadowVariationThresholdFilterIPCLDShadVarOutputReader->GetOutput());
                m_ShadowVariationPixelDiscardingFilter->SetLabelInput(m_ShadVarLabelReader->GetOutput());
                m_ShadowVariationPixelDiscardingFilter->SetValidityMap(m_ShadowVariationPixelValidationFilter->GetLabelValidityStatusMap());
                // No ExtCorrelFlagInput in this case
                m_ShadowVariationPixelDiscardingFilter->SetExtCorrelFlagInput(ITK_NULLPTR);

                vnsLaunchBasicOnlyCommandInDebug(m_ShadowVariationPixelDiscardingFilter->Update())

                vnsCrocodileClipMacro(InputMaskType, m_ShadowVariationPixelDiscardingFilter->GetOutput(),
                        "CloudMasking_ShadowVariationPixelDiscardingFilter_GetOutput.tif")

                // ShadVarTemp correlation
                m_ShadowVariationCorrelationFilter->SetIPCLDShadVarInput(m_ShadowVariationPixelDiscardingFilter->GetOutput()); //ShadowVariationPixelDiscardingFilterOutput);
                m_ShadowVariationCorrelationFilter->SetIPTOCRInput(l_IPTOCRPtr);
                m_ShadowVariationCorrelationFilter->SetL2inRCRInput(l_L2inRCRPtr);
                m_ShadowVariationCorrelationFilter->SetL2inSTOInput(l_L2inSTOPtr);
                m_ShadowVariationCorrelationFilter->SetIPEDGInput(l_IPEDGPtr);
                m_ShadowVariationCorrelationFilter->SetL2inNDTInput(l_L2inNDTPtr);
                m_ShadowVariationCorrelationFilter->SetL2inWASInput(l_L2inWASPtr);
                m_ShadowVariationCorrelationFilter->SetNoData(m_NoData);

                m_ShadowVariationCorrelationFilter->SetCorrelBandTOCR(m_ShadowBandTOCR); //m_CorrelBandTOCR);
                m_ShadowVariationCorrelationFilter->SetCorrelBandRCR(m_ShadowBandRCR); //m_CorrelBandRCR);
                m_ShadowVariationCorrelationFilter->SetJDay(m_JDay);
                m_ShadowVariationCorrelationFilter->SetSTOListOfDates(m_STOListOfDates);
                m_ShadowVariationCorrelationFilter->SetMinPixelCorrelation(m_MinPixelCorrelation);
                m_ShadowVariationCorrelationFilter->SetCorrelThreshold(m_CorrelThreshold);
                m_ShadowVariationCorrelationFilter->SetNCorrel(m_NCorrel);

                vnsLaunchBasicOnlyCommandInDebug(m_ShadowVariationCorrelationFilter->Update())

                vnsCrocodileClipMacro(InputMaskType, m_ShadowVariationCorrelationFilter->GetIPCLDShadVarOutput(),
                        "CloudMasking_ShadowVariationCorrelationFilter_GetIPCLDShadVarOutput.tif")

                vnsCachingMacro(m_ShadowVariationIPCLDShadVarOutputReader, InputMaskType,
                        m_ShadowVariationCorrelationFilter->GetIPCLDShadVarOutput(), "ShadowVariationIPCLDShadVarOutput")

                m_TernaryLogicalSummaryFilter->SetInput3(m_ShadowVariationCorrelationFilter->GetIPCLDShadVarOutput());

            } // end  (l_IPCLDShadPixNb > 0)

            else
            {
                vnsLogWarningMacro("No Shadow pixel detected -> IPCLDShadVar is empty")
                m_TernaryLogicalSummaryFilter->SetInput3(m_ConstantMaskFilter->GetOutput());

                vnsCachingMacro(m_ShadowVariationIPCLDShadVarOutputReader, InputMaskType, m_ConstantMaskFilter->GetOutput(),
                        "ShadowVariationIPCLDShadVarOutput")
            }

        } // end init_mode false

        else
        {
            m_TernaryLogicalSummaryFilter->SetInput3(m_ConstantMaskFilter->GetOutput());
        }

        // Cloud mask summary
        m_TernaryLogicalSummaryFilter->SetInput1(m_CloudMaskAllOutputReader->GetOutput());
        m_TernaryLogicalSummaryFilter->SetInput2(m_ShadowMaskDeterminationFilterIPCLDShadOutputReader->GetOutput());
        vnsLaunchBasicOnlyCommandInDebug(m_TernaryLogicalSummaryFilter->UpdateOutputInformation())

        vnsCrocodileClipMacro(InputMaskType, m_TernaryLogicalSummaryFilter->GetOutput(),
                "CloudMasking_TernaryLogicalSummaryFilter_GetOutput.tif")
        // Dilate Cloud Masks
        m_IPCLDSumDilateFilter->SetKernel(l_SECloud);
        m_IPCLDSumDilateFilter->SetInput(m_TernaryLogicalSummaryFilter->GetOutput());
        m_IPCLDSumDilateFilter->SetDilateValue(l_DilateValue);

        vnsLaunchBasicOnlyCommandInDebug(m_IPCLDSumDilateFilter->UpdateOutputInformation())
        vnsCrocodileClipMacro(InputMaskType, m_IPCLDSumDilateFilter->GetOutput(), "CloudMasking_IPCLDSumDilateFilter_GetOutput.tif")

        m_IPCLDReflDilateFilter->SetKernel(l_SECloud);
        // If the SNW mask is available
        if (l_IPSNWSubPtr.IsNull() == false)
        {
            //=> the cloud_mask_sub.refl might have been updated
            // by the snow mask correction algorithm
            m_IPCLDReflDilateFilter->SetInput(m_SnowMaskCorrectionFilter->GetIPCLDReflSubOutput());
        }
        else
        {
            if (m_InitMode == false)
            {
                m_IPCLDReflDilateFilter->SetInput(m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput()); //m_ReflectanceThresholdFilterOutput);
            }
            else
            {
                m_IPCLDReflDilateFilter->SetInput(m_ReflectanceThresholdFilterOutputReader->GetOutput()); //m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput()); //m_ReflectanceThresholdFilterOutput);
            }
        }
        m_IPCLDReflDilateFilter->SetDilateValue(l_DilateValue);

        vnsLaunchBasicOnlyCommandInDebug(m_IPCLDReflDilateFilter->UpdateOutputInformation())

        // If init mode, the reflectance variation threshold and the shadow variation
        // is not performed
        // The output mask is a constant mask. So, there is no need to dilate it.
        if (m_InitMode == false)
        {
            m_IPCLDReflVarDilateFilter->SetKernel(l_SECloud);
            m_IPCLDReflVarDilateFilter->SetInput(m_ReflectanceVariationCorrelationFilter->GetIPCLDReflVarOutput());
            m_IPCLDReflVarDilateFilter->SetDilateValue(l_DilateValue);

            vnsLaunchBasicOnlyCommandInDebug(m_IPCLDReflVarDilateFilter->UpdateOutputInformation())

            m_IPCLDShadVarDilateFilter->SetKernel(l_SECloud);
            m_IPCLDShadVarDilateFilter->SetInput(m_ShadowVariationIPCLDShadVarOutputReader->GetOutput());
            m_IPCLDShadVarDilateFilter->SetDilateValue(l_DilateValue);

            vnsLaunchBasicOnlyCommandInDebug(m_IPCLDShadVarDilateFilter->UpdateOutputInformation())

            // same thing for the cloud mask extension
            m_IPCLDExtDilateFilter->SetKernel(l_SECloud);
            m_IPCLDExtDilateFilter->SetInput(m_CloudMaskExtensionPostProcessingFilter->GetOutput());
            m_IPCLDExtDilateFilter->SetDilateValue(l_DilateValue);
            vnsLaunchBasicOnlyCommandInDebug(m_IPCLDExtDilateFilter->UpdateOutputInformation())
        }

        m_IPCLDAltDilateFilter->SetKernel(l_SECloud);
        m_IPCLDAltDilateFilter->SetInput(l_L1CLDAltPtr);
        m_IPCLDAltDilateFilter->SetDilateValue(l_DilateValue);

        vnsLaunchBasicOnlyCommandInDebug(m_IPCLDAltDilateFilter->UpdateOutputInformation())

        m_IPCLDShadDilateFilter->SetKernel(l_SECloud);
        m_IPCLDShadDilateFilter->SetInput(m_ShadowMaskDeterminationFilterIPCLDShadOutputReader->GetOutput());
        m_IPCLDShadDilateFilter->SetDilateValue(l_DilateValue);

        vnsLaunchBasicOnlyCommandInDebug(m_IPCLDShadDilateFilter->UpdateOutputInformation())

        // Concatenation to generate a vector image of the cloud masks
        // Concatenation of all Mono band images to the vector image
        //*************************************************************************************************
        //*************************************************************************************************
        // ATTENTION : The order of the PushBack define the order of the data in the L2 Product
        //             For ChecktoolProcessorQuickLookL2::Launch method, we use index of these bands
        //*************************************************************************************************
        //*************************************************************************************************
        // AVANT MEMORY IN CHAIN CORE ALGORITHMS - DEPRECATED
        //                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud, extension and shadow masks
        //                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks and extension
        //                    Bit 3 - Cloud_Mask.refl : reflectance threshold
        //                    Bit 4 - Cloud_Mask.refl_var : reflectance variation threshold
        //                    Bit 5 - Cloud_Mask.extension : extension of the cloud mask
        //                    Bit 6 - Cloud_Mask.alt : stereoscopic mask  =>  VENUS et vide pour les autres
        //                    Bit 7 - Cloud_Mask.shadows : shadows mask from clouds within image
        //                    Bit 8 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
        //                    Bit 9 - Cirrus => L8 et S2

        // APRES MEMORY IN CHAIN CORE ALGORITHMS : V 4-1-0
        //                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud, extension and shadow masks
        //                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks and extension
        //                    Bit 3 - Cloud_Mask.shadows : shadows mask from clouds within image
        //                    Bit 4 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
        //                    Bit 5 - Cloud_Mask.refl : reflectance threshold
        //                    Bit 6 - Cloud_Mask.refl_var : reflectance variation threshold
        //                    Bit 7 - Cloud_Mask.extension : extension of the cloud mask
        //                    Bit 8 - Cloud_Mask.alt : stereoscopic mask  =>  VENUS et vide pour les autres
        //                    Bit 9 - Cirrus : Pour  L8 et S2 et vide pour les autres

        // FORMAT DISK
        //                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud and shadow masks
        //                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks
        //                    Bit 3 - Cloud_Mask.shadows : shadows mask from clouds within image
        //                    Bit 4 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
        //                    Bit 5 - Cloud_Mask.refl : reflectance threshold
        //                    Bit 6 - Cloud_Mask.refl_var : reflectance variation threshold
        //                    Bit 7 - Cloud_Mask.extension : extension of the cloud mask
        //                    Bit 8 - VENUS : Cloud_Mask.alt : stereoscopic mask
        //                    Bit 8 - L8 et S2 : Bit 9 - Cirrus
        //                    Bit 8 - Vide pour les autres

        // Bit 1
        m_IPCLDList->PushBack(m_IPCLDSumDilateFilter->GetOutput());
        // Bit 2
        m_IPCLDList->PushBack(m_IPCLDAllDilateFilter->GetOutput());
        // Bit 3
        m_IPCLDList->PushBack(m_IPCLDShadDilateFilter->GetOutput());

        // Bit 4
        if (m_InitMode == false)
        {
            m_IPCLDList->PushBack(m_IPCLDShadVarDilateFilter->GetOutput());
        }
        else
        {
            m_IPCLDList->PushBack(m_ConstantMaskFilter->GetOutput());
        }

        // Bit 5
        m_IPCLDList->PushBack(m_IPCLDReflDilateFilter->GetOutput());

        // Bit 6
        if (m_InitMode == false)
        {
            m_IPCLDList->PushBack(m_IPCLDReflVarDilateFilter->GetOutput());
        }
        else
        {
            m_IPCLDList->PushBack(m_ConstantMaskFilter->GetOutput());
        }

        // Bit 7 - Cloud_Mask.extension
        if (m_InitMode == false)
        {
            m_IPCLDList->PushBack(m_IPCLDExtDilateFilter->GetOutput());
        }
        else
        {
            m_IPCLDList->PushBack(m_ConstantMaskFilter->GetOutput());
        }

        // Bit 8
        m_IPCLDList->PushBack(m_IPCLDAltDilateFilter->GetOutput());

        // Cirrus mask dilatation and addition to the final cloud mask
        // Bit 9
        if (l_IPCirrusMaskSubPtr.IsNull() == false)
        {
            m_DilateCirrusMaskFilter->SetInput(l_IPCirrusMaskSubPtr);
            m_DilateCirrusMaskFilter->SetDilateValue(l_DilateValue);
            m_DilateCirrusMaskFilter->SetKernel(l_SECloud);
            m_DilateCirrusMaskFilter->UpdateOutputInformation();

            m_IPCLDList->PushBack(m_DilateCirrusMaskFilter->GetOutput());
        }
        // 4-1 : new: force 9 bits. The bits written are selected in the "plugin" CLD image writer !!
        else
        {
            m_IPCLDList->PushBack(m_ConstantMaskFilter->GetOutput());
        }

        m_IPCLDListToVectorImage->SetInput(m_IPCLDList);
        m_IPCLDListToVectorImage->UpdateOutputInformation();

        vnsCrocodileClipCxxListImageMacro(InputMaskType, m_IPCLDList, "CloudMasking_Output_CLD")

        // Apply the edge mask to the output cloud mask
        m_VectorChangeValueWithMaskFilter->SetInput1(m_IPCLDListToVectorImage->GetOutput());
        m_VectorChangeValueWithMaskFilter->SetInput2(l_IPEDGPtr);
        m_VectorChangeValueWithMaskFilter->GetFunctor().SetBackgroundMaskValue(0);

        VectorMaskPixelType l_ChangeValue;
        l_ChangeValue.SetSize(m_IPCLDList->Size());
        l_ChangeValue.Fill(0);

        m_VectorChangeValueWithMaskFilter->GetFunctor().SetChange(l_ChangeValue);

        // ========> Set the cloud output mask
        vnsCachingMacro(m_IPCLDSubVectOutputReader, VectorMaskType, m_VectorChangeValueWithMaskFilter->GetOutput(),
                "CloudMaskingIPCLDSubVect")
        m_IPCLDSubVectOutput = m_IPCLDSubVectOutputReader->GetOutput();

        // Specifics outputs for algorithms:
        //  - CLD Summary
        //  - CLD Extension
        // Apply the edge mask to the output cloud summary mask
        m_CLDSumApplyMaskFilter->SetInput1(m_IPCLDSumDilateFilter->GetOutput());
        m_CLDSumApplyMaskFilter->SetInput2(l_IPEDGPtr);
        m_CLDSumApplyMaskFilter->GetFunctor().SetReplaceValue(static_cast<InputMaskPixelType>(0));
        m_CLDSumApplyMaskFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskPixelType>(1));

        vnsLaunchBasicOnlyCommandInDebug(m_CLDSumApplyMaskFilter->UpdateOutputInformation())

        vnsCrocodileClipMacro(InputMaskType, m_CLDSumApplyMaskFilter->GetOutput(), "CloudMasking_CLSSumApplyMaskFilter_GetOutput.tif")

        // ========> Set the cloud summary output mask
        m_IPCLDSumOutput = m_CLDSumApplyMaskFilter->GetOutput();

    }

    // PrintSelf method
    void
    CloudMaskingImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
        // Show alls members of the class
        // Generic
        os << "m_NoData:    " << m_NoData << std::endl;
        os << "m_InitMode:    " << m_InitMode << std::endl;
        os << "m_RefinementOption:    " << m_RefinementOption << std::endl;
        os << "m_SECloudRadius    " << (int) (m_SECloudRadius) << std::endl;
        // Initialisation
        // des bandes
        os << "m_BlueBandTOCR:    " << m_BlueBandTOCR << std::endl;
        os << "m_RedBandTOCR:    " << m_RedBandTOCR << std::endl;
        os << "m_NIRBandTOCR:    " << m_NIRBandTOCR << std::endl;
        os << "m_SWIRBandTOCR:   " << m_SWIRBandTOCR << std::endl;
        os << "m_HasSwirBand:    " << m_HasSwirBand << std::endl;
        os << "m_BlueBandRCR:    " << m_BlueBandRCR << std::endl;
        os << "m_RedBandRCR:    " << m_RedBandRCR << std::endl;
        os << "m_NIRBandRCR:    " << m_NIRBandRCR << std::endl;
        os << "m_CorrelBandTOCR:    " << m_CorrelBandTOCR << std::endl;
        os << "m_CorrelBandRCR:    " << m_CorrelBandRCR << std::endl;
        // Affichage des seuils sur les bandes "classiques"
        os << "---> Reflectance threshold" << std::endl;
        os << "m_ForegroundValue    " << (int) (m_ForegroundValue) << std::endl;
        os << "m_ReflectanceWaterBand    " << m_ReflectanceWaterBand << std::endl;
        os << "m_BlueReflectanceThresholdValue  " << (double) (m_BlueReflectanceThresholdValue) << std::endl;
        os << "m_RedReflectanceThresholdValue  " << (double) (m_RedReflectanceThresholdValue) << std::endl;
        // Affichage des seuils sur les bandes "nir"
        os << "m_NirRedRatioMaxThresholdValue  " << (double) (m_NirRedRatioMaxThresholdValue) << std::endl;
        os << "m_NirRedRatioMinThresholdValue  " << (double) (m_NirRedRatioMinThresholdValue) << std::endl;
        os << "m_NirSwirRatioMinThresholdValue  " << (double) (m_NirSwirRatioMinThresholdValue) << std::endl;
        // Display for Reflectance variation threshold
        os << "---> Reflectance variation threshold" << std::endl;
        os << "m_JDay:    " << m_JDay << std::endl;
        os << "m_CloudForgettingDuration:    " << m_CloudForgettingDuration << std::endl;
        os << "m_MinVarThreshold:    " << m_MinVarThreshold << std::endl;
        os << "m_MaxVarThreshold:    " << m_MaxVarThreshold << std::endl;
        os << "m_KRedBlue:    " << m_KRedBlue << std::endl;
        os << "m_KNIRBlue:    " << m_KNIRBlue << std::endl;
        os << "m_DeltaThreshold:    " << m_DeltaThreshold << std::endl;
        os << "m_MinPixelCorrelation:    " << m_MinPixelCorrelation << std::endl;
        os << "m_CorrelThreshold:    " << m_CorrelThreshold << std::endl;
        os << "m_NCorrel:   " << (int) m_NCorrel[0] << " " << (int) m_NCorrel[1] << std::endl;
        // Display for Stereo correction parameters
        os << "---> Stereo correction" << std::endl;
        os << "m_DefaultAltitude:   " << m_DefaultAltitude << std::endl;
        os << "m_KsigmaAltitude:   " << m_KsigmaAltitude << std::endl;
        // Display for Shadow mask determination
        os << "---> Shadow mask determination" << std::endl;
        os << "m_DeltaHMax:    " << m_DeltaHMax << std::endl;
        os << "m_DeltaHMin:    " << m_DeltaHMin << std::endl;
        os << "m_DeltaHStep:    " << m_DeltaHStep << std::endl;
        // Display SOL parameters
        os << "m_SOLH1:    " << m_SOLH1 << std::endl;
        os << "m_SOLH2:    " << m_SOLH2 << std::endl;
        // Display SOL and VI HRef parameters
        os << "m_SOLHRef:    " << m_SOLHRef << std::endl;
        os << "m_VIEHRef:    " << m_VIEHRef << std::endl;
        os << "m_AbsNbpixThreshold:    " << m_AbsNbpixThreshold << std::endl;
        os << "m_ThresholdDiffImage:    " << m_ThresholdDiffImage << std::endl;
        // Display Outside clouds shadow mask
        os << "---> Outside clouds shadow mask" << std::endl;
        os << "m_VIEHRef:    " << m_VIEHRef << std::endl;
        os << "m_MaxCLA:    " << m_MaxCLA << std::endl;
        os << "m_ShadVarCompositeLabelRatio: "<<m_ShadVarCompositeLabelRatio<<std::endl;
    }

} // End namespace vns

