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
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2299-CNES : 27 juillet 2017 : Seuillage WaterVapor                   *
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 21 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-2090-CNES : 12 decembre 2016 : Correction utilisation du VAP NoData   *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 21 novembre 2013 : Correction de l'information géographique*
 * 									dans les produits. Correction de l'origine dans l'image VAP				*
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 03 novembre 2011 : Modification pour prise en compte            *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsAtmosphericAbsorptionCorrectionImageFilter.h"
#include "vnsMacro.h"
#include "vnsResamplerHelper.h"

namespace vns
{
    /** Constructor */

    AtmosphericAbsorptionCorrectionImageFilter::AtmosphericAbsorptionCorrectionImageFilter() :
            m_RealL2NoData(0.), m_L2NoData(0.), m_UseWaterAmountValue(false), m_WaterAmountValue(3.), m_IsSetLUT(false), m_WaterVaporReferenceBandId(
                    11), m_WaterVaporBandId(12), m_WaterVaporMaxThreshold(8.), m_OzoneAmountValue(0.3), m_ThetaS(0.), m_ThetaVSub(1, 0.), m_WriteL2Resolution(false)
    {
        // Set the number of inputs
        this->SetNumberOfIndexedInputs(4);
        this->SetNumberOfRequiredInputs(4);

        // Initialize the LUT pointer
        m_LUT = LUTType::New();

        //Instanciate filters
        m_PressureFilter = PressureFilterType::New();
        m_WaterAmountGenerator = WaterAmountImageGeneratorType::New();
        m_TOACorrectionFilter = TOACorrectionImageFilterType::New();
        m_WaterAmountValueConstantImageFilter = VAPImageConstantImageSourceFilterType::New();
        m_PressureFilterList = PressureFilterListType::New();
        m_WaterAmountValueConstantImageFilterList = VAPImageConstantImageSourceFilterListType::New();
        m_TOACorrectionFilterList = TOACorrectionImageFilterListType::New();
        m_ExpandList = PadAndResampleImageFilterListType::New();

// Initialize the list of the SMAC coefficients
        m_SMACCoefficientsSubList.clear();

        // Allocate the Output pointers
        m_IPTOACSubOutput = TOACImageType::New();
        m_IPTOACImageListOutput = TOACImageListType::New();
        m_VAPSubOutput = VAPImageType::New();
        m_L2VAPImageListOutput = VAPImageListType::New();
    }

    /** Destructor */

    AtmosphericAbsorptionCorrectionImageFilter::~AtmosphericAbsorptionCorrectionImageFilter()
    {
    }

    /******************* LUT H2O accessors ****************/
    /** Set */

    void
    AtmosphericAbsorptionCorrectionImageFilter::SetLUT(LUTPointer lut)
    {
        m_LUT = lut;
        m_IsSetLUT = true;
        this->Modified();
    }

    /** Main computation method. */

    void
    AtmosphericAbsorptionCorrectionImageFilter::UpdateWithCaching()
    {
        // Load inputs
        TOAImageConstPointer l_IPTOASub = this->GetIPTOASubInput();
        DTMImageConstPointer l_DTMSub = this->GetDTMSubInput();
        TOAImageListConstPointer l_IPTOAList = this->GetIPTOAInput();
        DTMImageListConstPointer l_DTMList = this->GetDTMInput();

        // ===========================
        // Check the input images size
        // ===========================
        //vnsChecking2ImagesSizeMacro( l_IPTOA, l_DTM )

        if ((l_IPTOASub.IsNull() == true) || (l_DTMSub.IsNull() == true))
        {
            vnsExceptionBusinessMacro("TOA or DTM input at L2 coarse resolution are null...");
        }
        if ((l_IPTOAList.IsNull() == true) || (l_DTMList.IsNull() == true))
        {
            vnsExceptionBusinessMacro("TOA or DTM list input at L2 resolution are null...");
        }
        if ((m_IsSetLUT == true) && (m_LUT.IsNull() == true))
        {
            vnsExceptionBusinessMacro("LUT input is null.");
        }

        /// Process at L2 coarse resolution
        // Class used to estimate the pressure as a function of the altitude
        m_PressureFilter->SetInput(l_DTMSub);
        // The water vapour is computed using the TOA reflectance images at 910 (or 940) and 865 nm
        if (m_UseWaterAmountValue == false)
        {
            // Check if the GIP_L2WATV is available
            if (m_IsSetLUT == false)
            {
                vnsExceptionBusinessMacro("Waiting for a LUT h2O, but no one specified. Please set a LUT or use a Water Amount Value.");
            }
            // Set the image of the TOA reflectance used to compute the water vapour
            m_WaterAmountGenerator->SetIPTOAInput(l_IPTOASub);
            m_WaterAmountGenerator->SetLUT(m_LUT);
            m_WaterAmountGenerator->SetWaterVaporReferenceBandId(m_WaterVaporReferenceBandId);
            m_WaterAmountGenerator->SetWaterVaporBandId(m_WaterVaporBandId);
            m_WaterAmountGenerator->SetVAPNodataValue(m_VAPNodataValue);
            m_WaterAmountGenerator->SetMaxThreshold(m_WaterVaporMaxThreshold);
            m_TOACorrectionFilter->SetWaterInput(m_WaterAmountGenerator->GetOutput());
        }
        // The spectral bands are not available in the input product or the user chooses to use a constant value
        else
        {
            m_TOACorrectionFilter->SetWaterAmountDefaultValue(m_WaterAmountValue);
        }

        // Compute the TOA reflectances after correction of the absorbing atmospheric molecules
        m_TOACorrectionFilter->SetTOAInput(l_IPTOASub);
        m_TOACorrectionFilter->SetPressureInput(m_PressureFilter->GetOutput());
        m_TOACorrectionFilter->SetSmacCoefContainerList(m_SMACCoefficientsSubList);
        m_TOACorrectionFilter->SetOzoneAmount(m_OzoneAmountValue);
        m_TOACorrectionFilter->SetThetaS(m_ThetaS);
        m_TOACorrectionFilter->SetThetaV(m_ThetaVSub);
        m_TOACorrectionFilter->SetL2NoData(m_RealL2NoData);
        m_IPTOACSubOutput = m_TOACorrectionFilter->GetOutput();

        // If the correction has to be computed at L2 resolution
        if (m_WriteL2Resolution == true)
        {
            if (m_UseWaterAmountValue == false)
            {
                m_WaterAmountGenerator->UpdateOutputInformation();

                m_VAPSubOutput = m_WaterAmountGenerator->GetOutput();
            }
            else
            {
                // L2Coarse area
                AreaType l_AreaToL2CoarseResolution;
                l_AreaToL2CoarseResolution.Origin = l_DTMSub->GetOrigin();
                l_AreaToL2CoarseResolution.Spacing = l_DTMSub->GetSignedSpacing();
                l_AreaToL2CoarseResolution.Size = l_DTMSub->GetLargestPossibleRegion().GetSize();

                // Generate a constant image with the constant value set in the GIP_L2COMM
                VAPImageConstantImageSourceFilterPointer l_VAPConstantImageFilter = VAPImageConstantImageSourceFilterType::New();
                l_VAPConstantImageFilter->SetSize(l_AreaToL2CoarseResolution.Size);
                l_VAPConstantImageFilter->SetSpacing(l_AreaToL2CoarseResolution.Spacing);
                l_VAPConstantImageFilter->SetOrigin(l_AreaToL2CoarseResolution.Origin);

                VAPImageType::PixelType pixel;
                pixel = static_cast<VAPImageType::PixelType>(m_WaterAmountValue);
                l_VAPConstantImageFilter->SetConstantValue(pixel);
                l_VAPConstantImageFilter->UpdateOutputInformation();
                m_WaterAmountValueConstantImageFilterList->PushBack(l_VAPConstantImageFilter);

                m_VAPSubOutput = l_VAPConstantImageFilter->GetOutput();
            }

            // Get the number of resolution of the L2 product
            const unsigned int l_nbRes = l_DTMList->Size();
            if (l_nbRes != m_SMACCoefficientsList.size())
            {
                vnsExceptionBusinessMacro(
                        "Internal ERROR: The number of resolution for the SMAC Coefficients list '"<<m_SMACCoefficientsList.size()<<"' is different than the number of possible resolution '"<<l_nbRes<<"' !")
            }
            if (l_nbRes != m_ThetaV.size())
            {
                vnsExceptionBusinessMacro(
                        "Internal ERROR: The number of resolution for the ThetaV list '"<<m_ThetaV.size()<<"' is different than the number of possible resolution '"<<l_nbRes<<"' !")
            }

            // For each L2 resolution
            for (unsigned int l_Res = 0; l_Res < l_nbRes; l_Res++)
            {
                PressureFilterPointer l_pressureFilter = PressureFilterType::New();
                // Compute the water vapor at L2 resolution
                l_pressureFilter->SetInput(l_DTMList->GetNthElement(l_Res));
                m_PressureFilterList->PushBack(l_pressureFilter);

                TOACorrectionImageFilterPointer l_TOACorrectionFilter = TOACorrectionImageFilterType::New();

                PadAndResampleImageFilterPointer l_Expand = PadAndResampleImageFilterType::New();

                // L2Res area
                AreaType l_AreaToL2Resolution;
                l_AreaToL2Resolution.Origin = l_DTMList->GetNthElement(l_Res)->GetOrigin();
                l_AreaToL2Resolution.Spacing = l_DTMList->GetNthElement(l_Res)->GetSignedSpacing();
                l_AreaToL2Resolution.Size = l_DTMList->GetNthElement(l_Res)->GetLargestPossibleRegion().GetSize();

                if (m_UseWaterAmountValue == false)
                {
                    if (m_IsSetLUT == false)
                    {
                        vnsExceptionBusinessMacro(
                                "Waiting for a LUT h2O, but no one specified. Please set a LUT or use a Water Amount Value.");
                    }

                    // Resample from L2Coarse to L2 resolution
                    vnsImagePadAndResamplerMacro("WaterAmountGenerator", l_Expand, m_WaterAmountGenerator->GetOutput(),
                            ResamplerHelper::RB_INTERPOLATOR_LINEAR, l_AreaToL2Resolution)
                    m_ExpandList->PushBack(l_Expand);
                    m_L2VAPImageListOutput->PushBack(l_Expand->GetOutput());
                    l_TOACorrectionFilter->SetWaterInput(l_Expand->GetOutput());
                }
                else
                {
                    // Generate a constant image with the constant value set in the GIP_L2COMM
                    VAPImageConstantImageSourceFilterPointer l_VAPConstantImageFilter = VAPImageConstantImageSourceFilterType::New();
                    l_VAPConstantImageFilter->SetSize(l_AreaToL2Resolution.Size);
                    l_VAPConstantImageFilter->SetSpacing(l_AreaToL2Resolution.Spacing);
                    l_VAPConstantImageFilter->SetOrigin(l_AreaToL2Resolution.Origin);

                    VAPImageType::PixelType pixel;
                    pixel = static_cast<VAPImageType::PixelType>(m_WaterAmountValue);
                    l_VAPConstantImageFilter->SetConstantValue(pixel);
                    l_VAPConstantImageFilter->UpdateOutputInformation();
                    m_WaterAmountValueConstantImageFilterList->PushBack(l_VAPConstantImageFilter);

                    m_L2VAPImageListOutput->PushBack(l_VAPConstantImageFilter->GetOutput());
                    l_TOACorrectionFilter->SetWaterAmountDefaultValue(m_WaterAmountValue);
                }
                // Compute the TOA reflectances after correction of the absorbing atmospheric molecules
                // at the current L2 resolution
                l_TOACorrectionFilter->SetTOAInput(l_IPTOAList->GetNthElement(l_Res));
                l_TOACorrectionFilter->SetPressureInput(l_pressureFilter->GetOutput());
                l_TOACorrectionFilter->SetSmacCoefContainerList(m_SMACCoefficientsList[l_Res]); //.at(l_Res));
                l_TOACorrectionFilter->SetOzoneAmount(m_OzoneAmountValue);
                l_TOACorrectionFilter->SetThetaS(m_ThetaS);
                l_TOACorrectionFilter->SetThetaV(m_ThetaV[l_Res]); //.at(l_Res));
                l_TOACorrectionFilter->SetL2NoData(m_RealL2NoData);
                m_TOACorrectionFilterList->PushBack(l_TOACorrectionFilter);

                l_TOACorrectionFilter->UpdateOutputInformation();

                m_IPTOACImageListOutput->PushBack(l_TOACorrectionFilter->GetOutput());

            } // end resol loop

        } // end if WriteL2Resolution

    }

    /** PrintSelf method */

    void
    AtmosphericAbsorptionCorrectionImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
        // Display Water amount flag
        os << "UseWaterAmountValue: " << m_UseWaterAmountValue << std::endl;
        if (m_UseWaterAmountValue == true)
        {
            // Display Water amount value
            os << "m_WaterAmountValue:       " << m_WaterAmountValue << std::endl;
        }
        // Display Water vapor reference band
        os << "m_WaterVaporReferenceBand1Id       : " << m_WaterVaporReferenceBandId << std::endl;
        // Display Water vapor band
        os << "m_WaterVaporBandId       : " << m_WaterVaporBandId << std::endl;
        // Display Ozone amount value
        os << "m_OzoneAmountValue: " << m_OzoneAmountValue << std::endl;
        // Display Theta S
        os << "m_ThetaS          : " << m_ThetaS << std::endl;
        // Display Theta V sub values
        const unsigned int thetaSubSize = m_ThetaVSub.size();
        os << "m_ThetaVSub          : " << std::endl;
        for (unsigned int i = 0; i < thetaSubSize; i++)
        {
            os << m_ThetaVSub.at(i) << std::endl;
        }
        // Display Theta V values
        const unsigned int nbResol = m_ThetaV.size();
        for (unsigned int r = 0; r < nbResol; r++)
        {
            const unsigned int thetaSize = m_ThetaV.at(r).size();
            os << "m_ThetaV[" << r << "]          : " << std::endl;

            for (unsigned int i = 0; i < thetaSize; i++)
            {
                os << m_ThetaV.at(r).at(i) << std::endl;
            }
        }
        // Display Pressure filter
        os << "m_PressureFilter:" << std::endl;
        os << m_PressureFilter << std::endl;
        if (m_UseWaterAmountValue == true)
        {
            // Display Water amount filters
            os << "m_WaterAmountGenerator:" << std::endl;
            os << m_WaterAmountGenerator << std::endl;
        }
        // Display TOA Correction filter
        os << "m_TOACorrectionFilter:" << std::endl;
        os << m_TOACorrectionFilter << std::endl;
    }

} // End namespace vns
