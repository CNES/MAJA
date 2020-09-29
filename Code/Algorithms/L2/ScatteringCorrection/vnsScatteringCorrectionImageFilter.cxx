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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-2 : FA : LAIG-FA-MAC-1879-CNES : 8 sept 2016 : Retrait du std::out                         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 25 octobre 2013 : Methode GetListOfBandCode renommée en   *
 * 							  	    										GetListOfL2BandCode			    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsScatteringCorrectionImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "vnsLoggers.h"
#include "vnsMacro.h"

namespace vns
{
    /** Constructor */

    ScatteringCorrectionImageFilter::ScatteringCorrectionImageFilter() :
            m_RealL2NoData(0.), m_WriteL2Resolution(false)
    {
        this->SetNumberOfIndexedInputs(8);
        this->SetNumberOfRequiredInputs(4);

        this->SetTOACSubInput(NULL);
        this->SetAOTSubInput(NULL);
        this->SetEDGSubInput(NULL);
        this->SetDTMSubInput(NULL);
        this->SetTOACListInput(NULL);
        this->SetAOTListInput(NULL);
        this->SetEDGListInput(NULL);
        this->SetDTMListInput(NULL);
        this->SetAngleZoneMaskListInput(NULL);

        m_LUTMap.clear();

        m_TOCSubOutput = OutputImageType::New();
        m_TOCListOutput = OutputImageListType::New();

        m_ComputeScatteringCorrection = ComputeScatteringCorrectionImageFilterType::New();
        m_ComputeScatteringCorrectionList = ComputeScatteringCorrectionImageFilterListType::New();

    }

    /** Destructor */

    ScatteringCorrectionImageFilter::~ScatteringCorrectionImageFilter()
    {
    }

    /** BeforeThreaded generate data method. */

    void
    ScatteringCorrectionImageFilter::CheckInputs()
    {

        TOACImageConstPointer l_TOACSubPtr = this->GetTOACSubInput();
        AOTImageConstPointer l_AOTSubPtr = this->GetAOTSubInput();
        InputMaskConstPointer l_EDGSubPtr = this->GetEDGSubInput();
        DTMImageConstPointer l_DTMSubPtr = this->GetDTMSubInput();

        // Check TOAC image availability
        if (l_TOACSubPtr.IsNull() == true)
        {
            vnsExceptionBusinessMacro("Waiting for a TOAC image, but no one specified. Please set one.");
        }
        // Check AOT image availability
        if (l_AOTSubPtr.IsNull() == true)
        {
            vnsExceptionBusinessMacro("Waiting for an AOT image, but no one specified. Please set one.");
        }
        // Check EDG image availability
        if (l_EDGSubPtr.IsNull() == true)
        {
            vnsExceptionBusinessMacro("Waiting for an EDG image, but no one specified. Please set one.");
        }
        // Check DTM image availability
        if (l_DTMSubPtr.IsNull() == true)
        {
            vnsExceptionBusinessMacro("Waiting for an Altitude image, but no one specified. Please set one.");
        }

        // If the product at L2 resolution is written
        if (m_WriteL2Resolution == true)
        {

            // Check TOAC image availability
            if (!this->GetTOACListInput())
            {
                vnsExceptionBusinessMacro("Waiting for a TOAC image list, but no one specified. Please set one.");
            }
            // Check AOT image availability
            if (!this->GetAOTListInput())
            {
                vnsExceptionBusinessMacro("Waiting for an AOT image list, but no one specified. Please set one.");
            }
            // Check EDG image availability
            if (!this->GetEDGListInput())
            {
                vnsExceptionBusinessMacro("Waiting for an EDG image list, but no one specified. Please set one.");
            }
            // Check DTM image availability
            if (!this->GetDTMListInput())
            {
                vnsExceptionBusinessMacro("Waiting for an Altitude image list, but no one specified. Please set one.");
            }

        }
    }

    /** Main computation method. */

    void
    ScatteringCorrectionImageFilter::UpdateWithCaching()
    {

        // Validate the presence of all inputs and their sizes
        CheckInputs();

        TOACImageConstPointer l_TOACSubPtr = this->GetTOACSubInput();

        // ===============================================================================================
        // Scattering correction' at L2 coarse Resolution
        // ===============================================================================================

        m_ComputeScatteringCorrection->SetTOACInput(l_TOACSubPtr);
        m_ComputeScatteringCorrection->SetEDGInput(this->GetEDGSubInput());
        m_ComputeScatteringCorrection->SetDTMInput(this->GetDTMSubInput());
        m_ComputeScatteringCorrection->SetAOTInput(this->GetAOTSubInput());
        m_ComputeScatteringCorrection->SetLUTMap(m_LUTMap);

        vnsLogDebugMacro("Start m_ComputeScatteringCorrection ...")
        // Get the list of band id at L2 coarse resolution ( 0, 1, 2, ...)
        m_ComputeScatteringCorrection->SetListOfL2CoarseBandId(m_BandsDefinitions.GetListOfBandIdInL2Coarse());

        m_ComputeScatteringCorrection->SetNoData(m_RealL2NoData);

        // Get the image of surface reflectances at L2 coarse resolution
        m_ComputeScatteringCorrection->GetOutput()->UpdateOutputInformation();
        m_TOCSubOutput = m_ComputeScatteringCorrection->GetOutput();
        vnsLogDebugMacro("Start m_ComputeScatteringCorrection done.")

        // ===============================================================================================
        // Scattering correction' at L2 Resolutions
        // ===============================================================================================
        vnsLogDebugBooleanValueMacro(m_WriteL2Resolution)

        if (m_WriteL2Resolution == true)
        {

            // Load optional inputs if the product L2 is generated at L2 resolutions
            TOACImageListConstPointer l_TOACList = this->GetTOACListInput();
            AOTImageListConstPointer l_AOTList = this->GetAOTListInput();
            InputMaskListConstPointer l_EDGList = this->GetEDGListInput();
            DTMImageListConstPointer l_DTMList = this->GetDTMListInput();
            // optional input (if angle zones are taken into account)
            VectorMaskListConstPointer l_AngleZoneList = this->GetAngleZoneMaskListInput();
            vnsLogDebugMacro("Pointer l_AngleZoneList        : " << l_AngleZoneList)
            vnsLogDebugMacro("Check l_AngleZoneList.IsNull() : " << Utilities::BoolToLowerString(l_AngleZoneList.IsNull()))
            const unsigned int l_NumberOfResolution = m_BandsDefinitions.GetListOfL2Resolution().size();
            vnsLogDebugValueMacro(l_NumberOfResolution)

            //------------------------------------------------------
            // Resolution loop
            //------------------------------------------------------
            for (unsigned int l_Resol = 0; l_Resol < l_NumberOfResolution; l_Resol++)
            {
                ComputeScatteringCorrectionImageFilterPointer l_ScatteringFilter = ComputeScatteringCorrectionImageFilterType::New();

                vnsLogDebugValueMacro(l_TOACList->Size())
                vnsLogDebugValueMacro(l_EDGList->Size())
                vnsLogDebugValueMacro(l_DTMList->Size())
                vnsLogDebugValueMacro(l_AOTList->Size())

                l_ScatteringFilter->SetTOACInput(l_TOACList->GetNthElement(l_Resol));
                l_ScatteringFilter->SetEDGInput(l_EDGList->GetNthElement(l_Resol));
                l_ScatteringFilter->SetDTMInput(l_DTMList->GetNthElement(l_Resol));
                l_ScatteringFilter->SetAOTInput(l_AOTList->GetNthElement(l_Resol));
                l_ScatteringFilter->SetLUTMap(m_LUTMap);

                // For wide field sensor like SENTINEL2, the variation of viewing angles within the image is taken into account
                if (l_AngleZoneList.IsNull() == false)
                {
                    vnsLogDebugValueMacro(l_AngleZoneList->Size())
                    l_ScatteringFilter->SetAngleZoneMaskInput(l_AngleZoneList->GetNthElement(l_Resol));
                }

                // Get the list of band code at L2 resolution (SENTINEL2 - R1 : B02, B03, B04, B08, ...)
                const Utilities::ListOfStrings & l_ResollistOfBandCode = m_BandsDefinitions.GetListOfL2BandCode(
                        m_BandsDefinitions.GetListOfL2Resolution()[l_Resol]);
                vnsLogListDebugMacro("l_ResollistOfBandCode", l_ResollistOfBandCode)

                // Get the list of band id at L2 coarse resolution that corresponds to band codes at L2 resolution (SENTINEL2 - R1 : 1, 2, 3, 7, ...)
                const Utilities::ListOfUIntegers & l_ListOfL2CoarseBandId =
                        m_BandsDefinitions.GetListOfL2CoarseBandIdAssociatedToL2BandCode(l_ResollistOfBandCode);

                vnsLogListDebugMacro("l_ListOfL2CoarseBandId", l_ListOfL2CoarseBandId)

                for (unsigned int id = 0; id < l_ListOfL2CoarseBandId.size(); id++)
                {
                    vnsLogDebugMacro("l_ListOfL2CoarseBandId["<<id<<"] = "<<l_ListOfL2CoarseBandId.at(id));
                }

                l_ScatteringFilter->SetListOfL2CoarseBandId(l_ListOfL2CoarseBandId);

                l_ScatteringFilter->SetNoData(m_RealL2NoData);

                // Get the output image of surface reflectances at L2 resolution
                l_ScatteringFilter->GetOutput()->UpdateOutputInformation();
                l_ScatteringFilter->UpdateOutputInformation();

                // Add to the member of the class
                m_ComputeScatteringCorrectionList->PushBack(l_ScatteringFilter);

                m_TOCListOutput->PushBack(l_ScatteringFilter->GetOutput());

            } // end for each L2 resolution

        }

    }

    /** PrintSelf method */

    void
    ScatteringCorrectionImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
    }

} // End namespace vns
