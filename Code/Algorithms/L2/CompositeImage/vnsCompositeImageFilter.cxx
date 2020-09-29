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
 * VERSION : 3.0.0 : FA : LAIG-FA-MAJA-157183-CS : 9 octobre 2017 : code quality                            *
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 21 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-DM-MAC-1757-CNES : 19 juillet 2016 : amélioration du calcul des statistiques *
 *                                                                  de l’AOT                                *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-116796-CS : 20 fevrier 2013 : Valeur du No_Data dans le plan PXD 		*
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 19 décembre 2013 : Améliorations algorithmiques			*
 * 								Suppression de la recherche des zones de forte AOT près des nuages			*
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 3 novembre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 10 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsCompositeImageFilter.h"
#include "vnsMacro.h"
namespace vns
{

    /** Constructor */

    CompositeImageFilter::CompositeImageFilter() :
            m_InitMode(false), m_RealL2NoData(0), m_L2NoData(0), m_CoefSigmaAOT(0), m_MinimumHighAOTStd(0.), m_MaxAOT(0.), m_MaxAge(0), m_HotspotFlag(
                    false), m_CirrusFlag(false), m_RainFlag(false), m_JDay(0), m_STOBandNumber(10), m_STOBandIndex(0), m_SEAOTRadius(0)
    {
        // Input TOA image initialization
        m_IPTOACInput = InputImageType::New();
        // Input TOCR image initialization
        m_IPTOCInput = InputImageType::New();
        m_IPTOCRInput = InputImageType::New();
        // Input EDG image initialization
        m_IPEDGInput = InputMaskType::New();
        m_IPCLDSumInput = InputMaskType::New();

        // Input SAT image initialization
        m_IPSATSubInput = VectorMaskType::New();
        // Input AOT image initialization
        m_IPAOTSubInput = AOTImageType::New();

        // Input L2in reflectance image initialization
        m_L2inRTAInput = InputImageType::New();
        m_L2inRCRInput = InputImageType::New();
        m_L2inRTCInput = InputImageType::New();
        // Input L2in STO image initialization
        m_L2inSTOInput = InputImageType::New();
        // Input L2in PXD image initialization
        m_L2inPXDInput = DateImageType::New();
        // Input L2in NDT image initialization
        m_L2inNDTInput = InputMaskType::New();

        // Output image initialisation
        // Input L2 out reflectance image initialization
        m_L2RTAOutput = OutputImageType::New();
        m_L2RCROutput = OutputImageType::New();
        m_L2RTCOutput = OutputImageType::New();
        // Input L2 output STO image initialization
        m_L2STOOutput = OutputImageType::New();
        // Input L2 output PXD image initialization
        m_L2PXDOutput = DateImageType::New();
        // Input L2 output NDT image initialization
        m_L2NDTOutput = InputMaskType::New();

        // Filter initialization
        m_MaskList = InputMaskListType::New();
        m_MaskListToVectorMaskFilter = MaskListToVectorMaskFilterType::New();
        m_ConcatenateVectorImageFilter = ConcatenateVectorImageFilterType::New();
        m_VectorMaskToMaskFilter = VectorMaskToMaskFilterType::New();
        m_StatisticsFilter = StatisticsFilterType::New();
        m_HighAOTFilter = HighAOTImageFilterType::New();

        // Filter initialization
        m_CompositeUpdateFilter = CompositeUpdateFilterType::New();
        m_OrFilter = OrFilterType::New();

        // Extract Filter initialization
        m_IPTOACExtractChannelsFilter = MultiChannelExtractROIType::New();
        m_IPTOCRExtractChannelsFilter = MultiChannelExtractROIType::New();
        m_IPTOCExtractChannelsFilter = MultiChannelExtractROIType::New();

        // Label Filter initialization
        m_LabelizeFilterOutput = InternalLabelType::New();
        m_LabelizeReader = InternalLabelReaderType::New();

        m_ListOfBandIndexForComposite.clear();

        // SNW Input image initialization
        m_IPSNWSubInput = NULL;
    }

    /** Destructor */

    CompositeImageFilter::~CompositeImageFilter()
    {
    }

    void
    CompositeImageFilter::CheckInputs()
    {

        // Check all the Input pointer
        // If one of them is NULL, throws a exception
        if ((m_IPTOACInput.IsNull() == true) || (m_IPTOCRInput.IsNull() == true) || (m_IPTOCInput.IsNull() == true)
                || (m_IPEDGInput.IsNull() == true) || (m_IPCLDSumInput.IsNull() == true) || (m_IPSATSubInput.IsNull() == true)
                || (m_IPAOTSubInput.IsNull() == true))
        {
            vnsExceptionBusinessMacro("At least one of the input is null.");
        }

        // Check the input images size
        // The size must be identical
        // For Init mode user case
        if (m_InitMode == true)
        {
            vnsChecking7ImagesSizeMacro(m_IPTOACInput, m_IPTOCRInput, m_IPTOCInput, m_IPEDGInput, m_IPCLDSumInput, m_IPSATSubInput,
                    m_IPAOTSubInput)
        }
        // For the others modes

        else
        {
            vnsChecking13ImagesSizeMacro(m_IPTOACInput, m_IPTOCRInput, m_IPTOCInput, m_IPEDGInput, m_IPCLDSumInput, m_IPSATSubInput,
                    m_IPAOTSubInput, m_L2inRTAInput, m_L2inRCRInput, m_L2inRTCInput, m_L2inSTOInput, m_L2inPXDInput, m_L2inNDTInput)
        }

    }

    /** Main computation method. */

    void
    CompositeImageFilter::UpdateWithCaching()
    {
        // Checks the inputs
        this->CheckInputs();

        double l_AOTMean(0.);
        double l_AOTStd(0.);

        // Logical addition of the input masks
        m_MaskList->PushBack(m_IPEDGInput);
        m_MaskList->PushBack(m_IPCLDSumInput);

        // Use SNW mask if available
        if (m_IPSNWSubInput.IsNull() == false)
        {
            m_MaskList->PushBack(m_IPSNWSubInput);
        }

        // Vector image generation
        m_MaskListToVectorMaskFilter->SetInput(m_MaskList);
        // Concatenation
        m_ConcatenateVectorImageFilter->SetInput1(m_IPSATSubInput);
        m_ConcatenateVectorImageFilter->SetInput2(m_MaskListToVectorMaskFilter->GetOutput());
        m_ConcatenateVectorImageFilter->UpdateOutputInformation();

        // Flattening
        m_VectorMaskToMaskFilter->SetInput(m_ConcatenateVectorImageFilter->GetOutput());
        m_VectorMaskToMaskFilter->UpdateOutputInformation();

        // Compute image statistics with an input mask
        m_StatisticsFilter->SetInput(m_IPAOTSubInput);
        m_StatisticsFilter->SetEnableVariance(true);
        m_StatisticsFilter->SetMaskInput(m_VectorMaskToMaskFilter->GetOutput());
        m_StatisticsFilter->SetMaskForegroundValue(0);
        m_StatisticsFilter->Update();

        // Statistics used in debug mode
        vnsLogDebugMacro("CompositeImageFilter::UpdateWithCaching() " "m_StatisticsFilter->GetMean():              " << m_StatisticsFilter->GetMean());
        vnsLogDebugMacro("CompositeImageFilter::UpdateWithCaching() " "m_StatisticsFilter->GetStandardDeviation(): " << m_StatisticsFilter->GetStandardDeviation());
        vnsLogDebugMacro("CompositeImageFilter::UpdateWithCaching() " "m_StatisticsFilter->GetVariance():          " << m_StatisticsFilter->GetVariance());
        vnsLogDebugMacro("CompositeImageFilter::UpdateWithCaching() " "NbOfValidValues:    " << m_StatisticsFilter->GetNbOfValidValues());
        vnsLogDebugMacro("CompositeImageFilter::UpdateWithCaching() " "NbOfExcludedValues: " << m_StatisticsFilter->GetNbOfExcludedValues());

        // Relative standard deviation for AOT
        l_AOTMean = m_StatisticsFilter->GetMean();
        l_AOTStd = m_StatisticsFilter->GetStandardDeviation();

        // Statistics used in debug mode
        vnsLogDebugMacro("CompositeImageFilter::UpdateWithCaching() " "AOT mean = "<<l_AOTMean);
        vnsLogDebugMacro("CompositeImageFilter::UpdateWithCaching() " "AOT std = "<<l_AOTStd);

        // Generate image of pixels that have a high AOT value
        m_HighAOTFilter->SetIPAOTSubInput(m_IPAOTSubInput);
        m_HighAOTFilter->SetIPEDGInput(m_IPEDGInput);
        m_HighAOTFilter->SetL2CLDSubInput(m_IPCLDSumInput);
        m_HighAOTFilter->SetAOTMean(l_AOTMean);
        m_HighAOTFilter->SetAOTStd(l_AOTStd);
        m_HighAOTFilter->SetCoefSigmaAOT(m_CoefSigmaAOT);
        m_HighAOTFilter->SetMinimumHighAOTStd(m_MinimumHighAOTStd);

        // Set input images
        if (m_IPSNWSubInput.IsNull() == true)
        {
            // snw not available: use only edg
            m_CompositeUpdateFilter->SetIPNotValidMaskSubInput(m_IPEDGInput);
        }
        else
        {
            // use both edg and snw
            m_OrFilter->SetInput1(m_IPEDGInput);
            m_OrFilter->SetInput2(m_IPSNWSubInput);
            m_CompositeUpdateFilter->SetIPNotValidMaskSubInput(m_OrFilter->GetOutput());
        }

        // Select band for the output reflectance products for the composite)
        // - IPTOAC (correspond with RTA)
        // - IPTOCR (correspond with RCR)
        // - IPTOC (correspond with RTC)
        m_IPTOACExtractChannelsFilter->SetInput(m_IPTOACInput);
        m_IPTOCRExtractChannelsFilter->SetInput(m_IPTOCRInput);
        m_IPTOCExtractChannelsFilter->SetInput(m_IPTOCInput);

        const unsigned int l_NbListOfBandIndexForComposite = m_ListOfBandIndexForComposite.size();
        for (unsigned int i = 0; i < l_NbListOfBandIndexForComposite; i++)
        {
            m_IPTOACExtractChannelsFilter->SetChannel(m_ListOfBandIndexForComposite[i] + 1 /* +1 because MultiChanels starts to [1...] */);
            m_IPTOCRExtractChannelsFilter->SetChannel(m_ListOfBandIndexForComposite[i] + 1 /* +1 because MultiChanels starts to [1...] */);
            m_IPTOCExtractChannelsFilter->SetChannel(m_ListOfBandIndexForComposite[i] + 1 /* +1 because MultiChanels starts to [1...] */);
        }

        // Update filter
        m_CompositeUpdateFilter->SetIPTOACInput(m_IPTOACExtractChannelsFilter->GetOutput());
        m_CompositeUpdateFilter->SetIPTOCRInput(m_IPTOCRExtractChannelsFilter->GetOutput());
        m_CompositeUpdateFilter->SetIPTOCInput(m_IPTOCExtractChannelsFilter->GetOutput());

        m_CompositeUpdateFilter->SetIPCLDSumInput(m_IPCLDSumInput);
        m_CompositeUpdateFilter->SetIPSATInput(m_IPSATSubInput);
        m_CompositeUpdateFilter->SetHighAOTMaskInput(m_HighAOTFilter->GetOutput());

        // If Init mode
        if (m_InitMode == false)
        {
            m_CompositeUpdateFilter->SetL2inRTAInput(m_L2inRTAInput);
            m_CompositeUpdateFilter->SetL2inRCRInput(m_L2inRCRInput);
            m_CompositeUpdateFilter->SetL2inRTCInput(m_L2inRTCInput);
            m_CompositeUpdateFilter->SetL2inSTOInput(m_L2inSTOInput);
            m_CompositeUpdateFilter->SetL2inPXDInput(m_L2inPXDInput);
            m_CompositeUpdateFilter->SetL2inNDTInput(m_L2inNDTInput);
        }

        // Set parameters
        m_CompositeUpdateFilter->SetInitMode(m_InitMode);
        m_CompositeUpdateFilter->SetRealL2NoData(m_RealL2NoData);
        m_CompositeUpdateFilter->SetL2NoData(m_L2NoData);
        m_CompositeUpdateFilter->SetMaxAOT(m_MaxAOT);
        m_CompositeUpdateFilter->SetMaxAge(m_MaxAge);
        m_CompositeUpdateFilter->SetHotspotFlag(m_HotspotFlag);
        m_CompositeUpdateFilter->SetCirrusFlag(m_CirrusFlag);
        m_CompositeUpdateFilter->SetRainFlag(m_RainFlag);
        m_CompositeUpdateFilter->SetJDay(static_cast<short>(m_JDay));
        m_CompositeUpdateFilter->SetAOTMean(l_AOTMean);
        m_CompositeUpdateFilter->SetSTOBandNumber(m_STOBandNumber);
        m_CompositeUpdateFilter->SetSTOBandIndex(m_STOBandIndex);

        m_CompositeUpdateFilter->UpdateOutputInformation();

        // Set the output images
        m_L2RTAOutput = m_CompositeUpdateFilter->GetL2RTAOutput();
        m_L2RCROutput = m_CompositeUpdateFilter->GetL2RCROutput();
        m_L2RTCOutput = m_CompositeUpdateFilter->GetL2RTCOutput();
        m_L2STOOutput = m_CompositeUpdateFilter->GetL2STOOutput();
        m_L2PXDOutput = m_CompositeUpdateFilter->GetL2PXDOutput();
        m_L2NDTOutput = m_CompositeUpdateFilter->GetL2NDTOutput();
    }

    /** PrintSelf method */

    void
    CompositeImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        // Call superclass print self
        Superclass::PrintSelf(os, indent);

        os << "m_InitMode:          " << m_InitMode << std::endl;
        os << "m_RealL2NoData:      " << m_RealL2NoData << std::endl;
        os << "m_L2NoData:          " << m_L2NoData << std::endl;
        os << "m_CoefSigmaAOT:      " << m_CoefSigmaAOT << std::endl;
        os << "m_MinimumHighAOTStd: " << m_MinimumHighAOTStd << std::endl;
        os << "m_MaxAOT:            " << m_MaxAOT << std::endl;
        os << "m_MaxAge:            " << m_MaxAge << std::endl;
        os << "m_HotspotFlag:       " << m_HotspotFlag << std::endl;
        os << "m_CirrusFlag:        " << m_CirrusFlag << std::endl;
        os << "m_HotspotFlag:       " << m_HotspotFlag << std::endl;
        os << "m_RainFlag:          " << m_RainFlag << std::endl;
        os << "m_JDay:              " << m_JDay << std::endl;
        os << "m_STOBandNumber:     " << m_STOBandNumber << std::endl;
        os << "m_STOBandIndex:      " << m_STOBandIndex << std::endl;
        os << "m_SEAOTRadius:       " << m_SEAOTRadius << std::endl;
        const unsigned int l_NbListOfBandIndexForComposite = m_ListOfBandIndexForComposite.size();
        os << "l_NbListOfBandIndexForComposite: " << l_NbListOfBandIndexForComposite << std::endl;
        for (unsigned int i = 0; i < l_NbListOfBandIndexForComposite; i++)
        {
            os << "m_ListOfBandForComposite["<< i<<"]:      " << m_ListOfBandIndexForComposite[i] << std::endl;
        }
    }

} // End namespace vns
