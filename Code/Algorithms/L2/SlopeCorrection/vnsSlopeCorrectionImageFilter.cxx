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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 3 mars 2017 : Refactoring pour ameliorer la qualite      *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 27 jan. 2014 : Utilisation du filtre de calcul de l'angle  *
 *                                                              d'incidence solaire                         *
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 25 octobre 2013 : Methode GetListOfBandCode renommée en   *
 * 							  	    GetListOfL2BandCode			    *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-354-CNES : 8 juin 2012 : Correction qualite (regle ou metrique)       *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 07 novembre 2011 : Modification pour prise en compte            *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsSlopeCorrectionImageFilter.h"
#include "vnsMacro.h"
#include "vnsCaching.h"
#include "vnsDedicatedPadAndFastResampleImageFilter.h"

namespace vns
{

    //
    //******************* Constructor  ****************
    //

    SlopeCorrectionImageFilter::SlopeCorrectionImageFilter() :
            m_RealL2NoData(0.), m_ThetaS(0.), m_PhiS(0.), m_MinCosI(0.), m_MinCosE(0.), m_MinCosRatio(0.)
    {

        // Input/Output initialization
        this->SetNumberOfRequiredInputs(7);
        this->SetNumberOfIndexedInputs(8);

        // Instantiate Filters
        m_ExpandTdirList = DedicatedPadAndFastResampleImageFilterListType::New();
        m_ExpandTdifList = DedicatedPadAndFastResampleImageFilterListType::New();
        m_IncidenceAngleList = IncidenceAngleFilterListType::New();

        m_ComputeTransmission = ComputeTransmissionImageFilterType::New();
        m_InternalSlopeCorrectionList = InternalSlopeCorrectionImageFilterListType::New();

        m_TdirExtractList = MultiChannelExtractROIListType::New();
        m_TdifExtractList = MultiChannelExtractROIListType::New();

        //outputs
        m_L2FREList = OutputVectorImageListType::New();
        m_L2STLList = OutputMaskListType::New();
        m_L2TGSList = OutputMaskListType::New();

    }

    //
    //******************* Destructor  ****************
    //

    SlopeCorrectionImageFilter::~SlopeCorrectionImageFilter()
    {
    }

    //
    //******************* BeforeThreadedGenerateData  ****************
    //

    void
    SlopeCorrectionImageFilter::CheckInputs()
    {

        // Update the output information of the input images
        static_cast<InputVectorImageType *>(itk::ProcessObject::GetInput(0))->UpdateOutputInformation();
        static_cast<InputImageType *>(itk::ProcessObject::GetInput(1))->UpdateOutputInformation();
        static_cast<InputDTMType *>(itk::ProcessObject::GetInput(2))->UpdateOutputInformation();
        static_cast<InputImageType *>(itk::ProcessObject::GetInput(3))->UpdateOutputInformation();
        static_cast<InputImageType *>(itk::ProcessObject::GetInput(4))->UpdateOutputInformation();
        static_cast<InputMaskType *>(itk::ProcessObject::GetInput(5))->UpdateOutputInformation();
        //static_cast<InputMaskType *> (itk::ProcessObject::GetInput(6))->UpdateOutputInformation();
        static_cast<InputVectorImageType *>(itk::ProcessObject::GetInput(7))->UpdateOutputInformation();

        // Get the input images
        InputVectorImageListConstPointer lL2SREPtr = this->GetL2SRE();
        InputImageConstPointerType lIPAOTSubPtr = this->GetIPAOTSub();
        InputImageListConstPointer lAPtr = this->GetA();
        InputImageListConstPointer lSPtr = this->GetS();
        InputDTMConstPointerType lDTMSubPtr = this->GetDTMSub();
        InputMaskConstPointerType lIPEDGPtr = this->GetIPEDGSub();
        InputVectorImageListConstPointer lRhoEnvPtr = this->GetRhoEnv();

        // Check if the input images have been set
        if ((lL2SREPtr.IsNull() == true) || (lIPAOTSubPtr.IsNull() == true) || (lAPtr.IsNull() == true) || (lSPtr.IsNull() == true)
                || (lDTMSubPtr.IsNull() == true) || (lIPEDGPtr.IsNull() == true) || (lRhoEnvPtr.IsNull() == true))
        {
            itkExceptionMacro(<< "At least one input is missing.");
        }

        // Check the input images size at L2 Coarse Resolution
        vnsChecking3ImagesSizeMacro(lIPAOTSubPtr, lDTMSubPtr, lIPEDGPtr)

    }

    //
    //******************* GenerateData  ****************
    //
    void
    SlopeCorrectionImageFilter::UpdateWithCaching()
    {
        this->CheckInputs();

        // Load Inputs
        InputVectorImageListConstPointer lL2SREListPtr = this->GetL2SRE();
        InputImageConstPointerType lIPAOTSubPtr = this->GetIPAOTSub();
        InputImageListConstPointer lAListPtr = this->GetA();
        InputImageListConstPointer lSListPtr = this->GetS();
        InputDTMConstPointerType lDTMSubPtr = this->GetDTMSub();
        InputMaskConstPointerType lIPEDGPtr = this->GetIPEDGSub();
        InputVectorImageListConstPointer lRhoEnvListPtr = this->GetRhoEnv();

        // Computes number of bands
        const unsigned int lNbOfBands = m_BandsDefinitions.GetL2CoarseBandMap().size();

        // Init the ComputeTransmission filter
        m_ComputeTransmission->SetIPAOTSub(lIPAOTSubPtr);
        m_ComputeTransmission->SetIPEDG(lIPEDGPtr);
        m_ComputeTransmission->SetDTMSub(lDTMSubPtr);
        m_ComputeTransmission->SetNbOfBands(lNbOfBands);
        m_ComputeTransmission->SetThetaS(m_ThetaS);

        const unsigned int l_nbRes = m_BandsDefinitions.GetListOfL2Resolution().size();
        const Utilities::ListOfStrings & l_ListOfResolution = m_BandsDefinitions.GetListOfL2Resolution();

        //filter initialisation
        for (unsigned int l_res = 0; l_res < l_nbRes; l_res++)
        {
            // Init local filters
            // in the loop
            MultiChannelExtractROIPointer l_TdirExtract = MultiChannelExtractROIType::New();
            MultiChannelExtractROIPointer l_TdifExtract = MultiChannelExtractROIType::New();
            DedicatedPadAndFastResampleImageFilterPointerType l_ExpandTdir = DedicatedPadAndFastResampleImageFilterType::New();
            DedicatedPadAndFastResampleImageFilterPointerType l_ExpandTdif = DedicatedPadAndFastResampleImageFilterType::New();
            IncidenceAngleFilterPointer l_IncidenceAngleFilter = IncidenceAngleFilterType::New();
            InternalSlopeCorrectionImageFilterPointerType l_InternalSlopeCorrection = InternalSlopeCorrectionImageFilterType::New();

            // Add in the list
            m_TdirExtractList->PushBack(l_TdirExtract);
            m_TdifExtractList->PushBack(l_TdifExtract);
            m_ExpandTdirList->PushBack(l_ExpandTdir);
            m_ExpandTdifList->PushBack(l_ExpandTdif);
            m_IncidenceAngleList->PushBack(l_IncidenceAngleFilter);
            m_InternalSlopeCorrectionList->PushBack(l_InternalSlopeCorrection);

        }

        // L2 Resolution loop
        for (unsigned int l_res = 0; l_res < l_nbRes; l_res++)
        {

            const unsigned int l_nbBand = m_BandsDefinitions.GetListOfL2BandCode(l_ListOfResolution[l_res]).size();
            const Utilities::ListOfStrings & l_listOfBandCode = m_BandsDefinitions.GetListOfL2BandCode(l_ListOfResolution[l_res]);

            // L2Coarse area
            AreaType l_DEMAreaToL2Resolution;
            l_DEMAreaToL2Resolution.Origin = lL2SREListPtr->GetNthElement(l_res)->GetOrigin();
            l_DEMAreaToL2Resolution.Spacing = lL2SREListPtr->GetNthElement(l_res)->GetSpacing();
            l_DEMAreaToL2Resolution.Size = lL2SREListPtr->GetNthElement(l_res)->GetLargestPossibleRegion().GetSize();

            if (l_nbBand != lNbOfBands)
            {
                vnsLogDebugMacro("Cas multi Resolution");

                //Extract list of band
                m_TdirExtractList->GetNthElement(l_res)->SetInput(m_ComputeTransmission->GetTdir());
                m_TdifExtractList->GetNthElement(l_res)->SetInput(m_ComputeTransmission->GetTdif());

                // Loop under bands
                for (unsigned int b = 0; b < l_nbBand; b++)
                {
                    const std::string l_bandCode = l_listOfBandCode[b];
                    // +1 car channel doit commencer a 1 pour otb::MultiChannelExtractROI
                    unsigned int channel = m_BandsDefinitions.GetBandIdInL2Coarse(l_bandCode) + 1;

                    m_TdirExtractList->GetNthElement(l_res)->SetChannel(channel);
                    m_TdifExtractList->GetNthElement(l_res)->SetChannel(channel);
                }
                m_TdirExtractList->GetNthElement(l_res)->UpdateOutputInformation();
                m_TdifExtractList->GetNthElement(l_res)->UpdateOutputInformation();
                vnsImagePadAndFastDedicatedResamplerMacro("ExpandTdirList", /* DedicatedPadAndFastResampleImageFilterType, */
                m_ExpandTdirList->GetNthElement(l_res), m_TdirExtractList->GetNthElement(l_res)->GetOutput() /* Input */,
                        ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,l_DEMAreaToL2Resolution)
                //, l_edgeVal /* edgePaddingValue */)
                vnsImagePadAndFastDedicatedResamplerMacro("ExpandTdifList", /* DedicatedPadAndFastResampleImageFilterType, */
                m_ExpandTdifList->GetNthElement(l_res), m_TdifExtractList->GetNthElement(l_res)->GetOutput() /* Input */,
                        ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,l_DEMAreaToL2Resolution)
                //, l_edgeVal /* edgePaddingValue */)
            }
            else
            {
                vnsLogDebugMacro("Cas mono Resolution");
                vnsImagePadAndFastDedicatedResamplerMacro("ExpandTdirList", /* DedicatedPadAndFastResampleImageFilterType, */
                m_ExpandTdirList->GetNthElement(l_res), m_ComputeTransmission->GetTdir() /* Input */,
                        ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,l_DEMAreaToL2Resolution)
                //, l_edgeVal /* edgePaddingValue */)
                vnsImagePadAndFastDedicatedResamplerMacro("ExpandTdifList", /* DedicatedPadAndFastResampleImageFilterType, */
                m_ExpandTdifList->GetNthElement(l_res), m_ComputeTransmission->GetTdif() /* Input */,
                        ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,l_DEMAreaToL2Resolution)
                //, l_edgeVal /* edgePaddingValue */)
            }

            m_IncidenceAngleList->GetNthElement(l_res)->SetInput1(lAListPtr->GetNthElement(l_res));
            m_IncidenceAngleList->GetNthElement(l_res)->SetInput2(lSListPtr->GetNthElement(l_res));
            m_IncidenceAngleList->GetNthElement(l_res)->GetFunctor().SetPhiS(m_PhiS * otb::CONST_PI_180);
            m_IncidenceAngleList->GetNthElement(l_res)->GetFunctor().SetThetaS(m_ThetaS * otb::CONST_PI_180);

            // Init the SlopeCorrection list filters
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetS(lSListPtr->GetNthElement(l_res));
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetA(lAListPtr->GetNthElement(l_res));
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetIncidenceAngle(m_IncidenceAngleList->GetNthElement(l_res)->GetOutput());
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetL2SRE(lL2SREListPtr->GetNthElement(l_res));
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetRhoEnv(lRhoEnvListPtr->GetNthElement(l_res));
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetTdir(m_ExpandTdirList->GetNthElement(l_res)->GetOutput());
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetTdif(m_ExpandTdifList->GetNthElement(l_res)->GetOutput());
// MACCS 4.3 - optimization
//                m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetT(m_AddTdirTdifList->GetNthElement(l_res)->GetOutput());
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetThetaS(m_ThetaS * otb::CONST_PI_180);
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetPhiS(m_PhiS * otb::CONST_PI_180);
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetThetaV(m_ThetaV.at(l_res));
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetPhiV(m_PhiV.at(l_res));
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetMinCosI(m_MinCosI);
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetMinCosE(m_MinCosE);
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetMinCosRatio(m_MinCosRatio);
            m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetNoData(m_RealL2NoData);

            m_InternalSlopeCorrectionList->GetNthElement(l_res)->UpdateOutputInformation();

            m_L2FREList->PushBack(m_InternalSlopeCorrectionList->GetNthElement(l_res)->GetL2FRE());
            m_L2STLList->PushBack(m_InternalSlopeCorrectionList->GetNthElement(l_res)->GetL2STL());
            m_L2TGSList->PushBack(m_InternalSlopeCorrectionList->GetNthElement(l_res)->GetL2TGS());
        } // end resol loop
    }

    //
    //******************* Printself method  ****************
    //
    void
    SlopeCorrectionImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        this->Superclass::PrintSelf(os, indent);
    }

}// End namespace vns
