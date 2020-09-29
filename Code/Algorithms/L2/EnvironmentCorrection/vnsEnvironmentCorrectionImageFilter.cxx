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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 23 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 25 octobre 2013 : Methode GetListOfBandCode renommée en   *
 * 							  	    GetListOfL2BandCode			    *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 07 novembre 2011 : Modification pour prise en compte            *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0-3 : DM : 237 : 02 aout 2011 : Modification expand dans Environnement Correction            *
 *                                             et mise à jour du masque de bord en sortie                   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsEnvironmentCorrectionImageFilter.h"
#include "vnsCaching.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"

namespace vns
{

    //
    //******************* Constructor  ****************
    //

    EnvironmentCorrectionImageFilter::EnvironmentCorrectionImageFilter() :
            m_RealL2NoData(0.)
    {

        // Input/Output initialization
        this->SetNumberOfRequiredInputs(6);
        this->SetNumberOfIndexedInputs(6);

        m_RhoEnvList = OutputVectorImageListType::New();
        m_L2SREList = OutputVectorImageListType::New();

        m_InterpolateForEnvCor = InterpolateForEnvCorrectionImageFilterType::New();
        m_Convolution = ConvolutionVectorImageFilterType::New();
        m_VectorChangeValueWithMaskFilter = ChangeValueWithMaskVectorImageFilterType::New();
        m_ComputeEnvironmentCorrectionImageFilterList = ComputeEnvironmentCorrectionImageFilterListType::New();
        m_RhoEnvExtractList = MultiChannelExtractROIListType::New();
        m_TdirExtractList = MultiChannelExtractROIListType::New();
        m_TdifExtractList = MultiChannelExtractROIListType::New();
        m_SatmExtractList = MultiChannelExtractROIListType::New();

//            m_IdRhoEnvOutputReader = VectorImageReaderType::New();
//            m_IdRhoEnvOutput = OutputVectorImageType::New();
    }

    //
    //******************* Destructor  ****************
    //

    EnvironmentCorrectionImageFilter::~EnvironmentCorrectionImageFilter()
    {
    }

    //
    //******************* BeforeThreadedGenerateData  ****************
    //

    void
    EnvironmentCorrectionImageFilter::CheckInputs()
    {
        // Get the input images at L2 resolutions
        InputVectorImageListConstPointer lIPTOCPtr = this->GetIPTOC();
        InputMaskListConstPointer lL2EDGPtr = this->GetL2EDG();
        // Get the input images at L2 coarse resolution
        InputVectorImageConstPointerType lIPTOCSubPtr = this->GetIPTOCSub();
        InputImageConstPointerType lIPAOTSubPtr = this->GetIPAOTSub();
        InputDTMConstPointerType lDTMSubPtr = this->GetDTMSub();
        InputMaskConstPointerType lIPEDGPtr = this->GetIPEDGSub();

        // Check if the input images have been set
        if ((lIPTOCPtr.IsNull() == true) || (lIPTOCSubPtr.IsNull() == true) || (lIPAOTSubPtr.IsNull() == true)
                || (lDTMSubPtr.IsNull() == true) || (lIPEDGPtr.IsNull() == true) || (lL2EDGPtr.IsNull() == true))
        {
            // if not, raise an exception
            itkExceptionMacro(<< "At least one input is missing.");
        }
        // Check if the number of input images is consistent with the number of L2 resolution (one image per resolution)
        const unsigned int l_nbRes = m_BandsDefinitions.GetListOfL2Resolution().size();
        if (lIPTOCPtr->Size() != l_nbRes)
        {
            vnsExceptionBusinessMacro(
                    "EnvironmentCorrectionImageFilter Internal error: Number of L2 resolution is "<<l_nbRes<<" lIPTOCPtr Image list size is "<<lIPTOCPtr->Size()<<"!")
        }
        // Check that an image has been set for each resolution
        for (unsigned int l_res = 0; l_res < l_nbRes; l_res++)
        {
            if ((lIPTOCPtr->GetNthElement(l_res).IsNull() == true) || (lL2EDGPtr->GetNthElement(l_res).IsNull() == true))
            {
                vnsExceptionBusinessMacro("EnvironmentCorrectionImageFilter Internal error: At least one input is missing.");
            }
        }
        // Check the input images size at L2 Coarse Resolution
        vnsChecking4ImagesSizeMacro(lIPTOCSubPtr, lIPAOTSubPtr, lDTMSubPtr, lIPEDGPtr)

    }

    //
    //******************* GenerateData  ****************
    //

    void
    EnvironmentCorrectionImageFilter::UpdateWithCaching()
    {

        // Validate the presence of all inputs and their sizes
        CheckInputs();

        // Load Inputs
        InputVectorImageListConstPointer lIPTOCPtr = this->GetIPTOC();
        InputVectorImageConstPointerType lIPTOCSubPtr = this->GetIPTOCSub();
        InputImageConstPointerType lIPAOTSubPtr = this->GetIPAOTSub();
        InputDTMConstPointerType lDTMSubPtr = this->GetDTMSub();
        InputMaskConstPointerType lIPEDGPtr = this->GetIPEDGSub();
        InputMaskListConstPointer lL2EDGPtr = this->GetL2EDG();

        // 1 - Convolution produces rho env sub
        // WARNING :to obtain exactly the same result that the CNES reference
        // we have to set 0 for the NoData value of the convolution.
        // But in fact, we should create a stricter mask in order not to use the blurred
        // values due to the expand filter. (Frontier between the data and noData values)
        // Such a mask could be created by thresholding
        m_Convolution->SetNoData(0.);
        m_Convolution->SetInput(lIPTOCSubPtr);
        m_Convolution->SetMask(lIPEDGPtr);

        // Caching is used to load the image entirely to avoid artefacts apparently due to Expand Image Filter
        vnsCachingCxxMacroWithReader(l_IdRhoEnvOutputReader, OutputVectorImageType, m_Convolution->GetOutput(), "EnvironmentCorrectionRhoEnvSub")

        // Apply the edge mask to the rho_env image
        m_VectorChangeValueWithMaskFilter->SetInput1(l_IdRhoEnvOutputReader->GetOutput());
        m_VectorChangeValueWithMaskFilter->SetInput2(lIPEDGPtr);
        m_VectorChangeValueWithMaskFilter->GetFunctor().SetBackgroundMaskValue(0);
        const unsigned int lPixSize = l_IdRhoEnvOutputReader->GetOutput()->GetNumberOfComponentsPerPixel();
        InputVectorImagePixelType lChangeValue;
        lChangeValue.SetSize(lPixSize);
        lChangeValue.Fill(10000);
        vnsLogDebugMacro("lChangeValue Size = "<<lChangeValue.GetSize())
        m_VectorChangeValueWithMaskFilter->GetFunctor().SetChange(lChangeValue);
        m_VectorChangeValueWithMaskFilter->UpdateOutputInformation();

        // 2 - Interpolations with AOTSub and the different LUTs
        dynamic_cast<InputVectorImageType *>(itk::ProcessObject::GetInput(1))->UpdateOutputInformation();
        const unsigned int l_NbBandSub = lIPTOCSubPtr->GetNumberOfComponentsPerPixel();
        m_InterpolateForEnvCor->SetIPAOTSub(lIPAOTSubPtr);
        m_InterpolateForEnvCor->SetIPEDG(lIPEDGPtr);
        m_InterpolateForEnvCor->SetDTMSub(lDTMSubPtr);
        m_InterpolateForEnvCor->SetNbOfBands(l_NbBandSub);
        m_InterpolateForEnvCor->UpdateOutputInformation();

        OutputVectorPixelType l_edgeVal;
        l_edgeVal.SetSize(m_InterpolateForEnvCor->GetTdir()->GetNumberOfComponentsPerPixel());
        l_edgeVal.Fill(static_cast<OutputVectorImageInternalPixelType>(CONST_EPSILON));

        vnsLogDebugMacro("l_edgeVal Size = "<<l_edgeVal.GetSize())

        // 3 - Expand the obtained images
        const unsigned int l_nbRes = m_BandsDefinitions.GetListOfL2Resolution().size();
        const Utilities::ListOfStrings & l_ListOfResolution = m_BandsDefinitions.GetListOfL2Resolution();

        //filter initialization
        for (unsigned int l_res = 0; l_res < l_nbRes; l_res++)
        {
            MultiChannelExtractROIPointer l_RhoEnvExtract = MultiChannelExtractROIType::New();
            MultiChannelExtractROIPointer l_TdirExtract = MultiChannelExtractROIType::New();
            MultiChannelExtractROIPointer l_TdifExtract = MultiChannelExtractROIType::New();
            MultiChannelExtractROIPointer l_SatmExtract = MultiChannelExtractROIType::New();

            m_RhoEnvExtractList->PushBack(l_RhoEnvExtract);
            m_TdirExtractList->PushBack(l_TdirExtract);
            m_TdifExtractList->PushBack(l_TdifExtract);
            m_SatmExtractList->PushBack(l_SatmExtract);
        }
        // For each L2 resolution
        for (unsigned int l_res = 0; l_res < l_nbRes; l_res++)
        {
            const unsigned int l_nbBandL2 = m_BandsDefinitions.GetListOfL2BandCode(l_ListOfResolution[l_res]).size();
            const Utilities::ListOfStrings & l_listOfBandCode = m_BandsDefinitions.GetListOfL2BandCode(l_ListOfResolution[l_res]);

            ComputeEnvironmentCorrectionImageFilterPointer l_ComputeEnvironmentCorrectionImageFilter =
                    ComputeEnvironmentCorrectionImageFilterType::New();

            // Attention !!! The number of bands at L2 resolution should be different from the number of bands at L2 coarse resolution.
            // This test supposes that the L2 coarse resolution contains all the bands of the L1 product.
            // Multi Resolution case
            vnsLogDebugMacro("l_nbBandL2 = "<<l_nbBandL2 << " and l_NbBandSub = "<<l_NbBandSub)
            if (l_nbBandL2 != l_NbBandSub)
            {
                vnsLogDebugMacro("Multi Resolution L2");

                // Set the direct, diffuse, environment reflectance and the atmospheric albedo images
                // for each L2 resolution
                m_RhoEnvExtractList->GetNthElement(l_res)->SetInput(m_VectorChangeValueWithMaskFilter->GetOutput());
                m_TdirExtractList->GetNthElement(l_res)->SetInput(m_InterpolateForEnvCor->GetTdir());
                m_TdifExtractList->GetNthElement(l_res)->SetInput(m_InterpolateForEnvCor->GetTdif());
                m_SatmExtractList->GetNthElement(l_res)->SetInput(m_InterpolateForEnvCor->GetSatm());

                // Extract the L2 bands of the list of bands at L2 coarse resolution
                for (unsigned int b = 0; b < l_nbBandL2; b++)
                {
                    const std::string l_bandCode = l_listOfBandCode[b];
                    // Get the matching between the indexes at L2 resolution and the
                    // indexes at L2 coarse resolution
                    // +1 because band index begins to one in otb::MultiChannelExtractROI
                    unsigned int channel = m_BandsDefinitions.GetBandIdInL2Coarse(l_bandCode) + 1;

                    m_RhoEnvExtractList->GetNthElement(l_res)->SetChannel(channel);
                    m_TdirExtractList->GetNthElement(l_res)->SetChannel(channel);
                    m_TdifExtractList->GetNthElement(l_res)->SetChannel(channel);
                    m_SatmExtractList->GetNthElement(l_res)->SetChannel(channel);
                }

                // Update the filters to get the output image information
                m_RhoEnvExtractList->GetNthElement(l_res)->UpdateOutputInformation();
                m_TdirExtractList->GetNthElement(l_res)->UpdateOutputInformation();
                m_TdifExtractList->GetNthElement(l_res)->UpdateOutputInformation();
                m_SatmExtractList->GetNthElement(l_res)->UpdateOutputInformation();

                l_ComputeEnvironmentCorrectionImageFilter->SetRhoEnvSub(m_RhoEnvExtractList->GetNthElement(l_res)->GetOutput());
                l_ComputeEnvironmentCorrectionImageFilter->SetLUT_Tdir(m_TdirExtractList->GetNthElement(l_res)->GetOutput());
                l_ComputeEnvironmentCorrectionImageFilter->SetLUT_Tdif(m_TdifExtractList->GetNthElement(l_res)->GetOutput());
                l_ComputeEnvironmentCorrectionImageFilter->SetLUT_Satm(m_SatmExtractList->GetNthElement(l_res)->GetOutput());
            }
            // If only one L2 resolution exists
            else
            {
                vnsLogDebugMacro("One L2 Resolution");
                l_ComputeEnvironmentCorrectionImageFilter->SetRhoEnvSub(m_VectorChangeValueWithMaskFilter->GetOutput());
                l_ComputeEnvironmentCorrectionImageFilter->SetLUT_Tdir(m_InterpolateForEnvCor->GetTdir());
                l_ComputeEnvironmentCorrectionImageFilter->SetLUT_Tdif(m_InterpolateForEnvCor->GetTdif());
                l_ComputeEnvironmentCorrectionImageFilter->SetLUT_Satm(m_InterpolateForEnvCor->GetSatm());
            }

            lIPTOCPtr->GetNthElement(l_res)->UpdateOutputInformation();
            l_ComputeEnvironmentCorrectionImageFilter->SetIPTOC(lIPTOCPtr->GetNthElement(l_res));
            l_ComputeEnvironmentCorrectionImageFilter->SetL2EDG(lL2EDGPtr->GetNthElement(l_res));

//                l_ComputeEnvironmentCorrectionImageFilter->SetL2L2CoarseRatio(m_BandsDefinitions.GetL2L2CoarseRatio(
//                        l_ListOfResolution[l_res]));
//                l_ComputeEnvironmentCorrectionImageFilter->SetL2ProductImageSize(m_BandsDefinitions.GetL2ProductImageSize(
//                        l_ListOfResolution[l_res]));
            l_ComputeEnvironmentCorrectionImageFilter->SetNoData(m_RealL2NoData /* m_BandsDefinitions.GetRealL2NoData() */);
            l_ComputeEnvironmentCorrectionImageFilter->UpdateWithCaching();

            m_ComputeEnvironmentCorrectionImageFilterList->PushBack(l_ComputeEnvironmentCorrectionImageFilter);

            m_RhoEnvList->PushBack(m_ComputeEnvironmentCorrectionImageFilterList->GetNthElement(l_res)->GetRhoEnv());
            m_L2SREList->PushBack(m_ComputeEnvironmentCorrectionImageFilterList->GetNthElement(l_res)->GetL2SRE());

        }

    }

    //
    //******************* Printself method  ****************
    //

    void
    EnvironmentCorrectionImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        this->Superclass::PrintSelf(os, indent);
    }

} // End namespace vns
