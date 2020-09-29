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
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 18 mai 2016 : Correction refactoring,menage, etc...       *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de détection *
 *                                                                  des ombres de nuages                    *
 * VERSION : 4-7-1 : FA : LAIG-FA-MAC-1446-CNES : 8 janvier 2016 : Correction PB mauvais cast sur InputImage*
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-751-CNES : 17 février 2014 : Sélection des pixels pour l'inversion    *
 * 																	(Spec v2.2)								*
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 21 novembre 2013 : Correction de l'information géographique*
 * 									dans les produits. Correction du spacing dans l'image de sortie         *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 13 oct. 2011 : Méthode multi temporelle                         *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsMultiTemporalComputeAOTImageFilter_txx
#define vnsMultiTemporalComputeAOTImageFilter_txx

#include "vnsMultiTemporalComputeAOTImageFilter.h"
#include "itkMacro.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkTimeProbe.h"

namespace vns
{

    //
    //******************* Constructor *******************
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::MultiTemporalComputeAOTImageFilter() :
                m_JDay(0.), m_MinSamplingInterval(0), m_MaxSamplingInterval(0), m_IncrementNDistance(1), m_Radius(0), m_StepAOT(1), m_RelativeVarMax(
                        0), m_MaxAge(0.), m_VARBandD(0), m_VARBandDm1(0), m_STDEVBand(0), m_StdNoise(1.), m_MinNumberOfPixels(0), m_MinStdDevThreshold(
                        0), m_F_Tolerance(0.), m_G_Tolerance(0.), m_X_Tolerance(0.), m_Epsilon_Function(0.), m_Max_Iterations(0), m_AOTinitialD(
                        0.), m_AOTinitialDm1(0.),m_UseCams(false),m_KPondCAMS(0.2),m_MeanAltitude(0),m_HeightScale(2000)

        {
            this->SetNumberOfRequiredInputs(12);
            this->SetNumberOfIndexedInputs(12);
            this->SetNumberOfIndexedOutputs(2);

            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, OutputMaskType::New());
            this->SetNthOutput(2, OutputImageType::New());

            // Parameters initialization
            m_NoData = static_cast<NoDataType>(0);

            m_RunLMOptimization = RunLMOptimizationType::New();
        }

    //
    //******************* Destructor *******************
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::~MultiTemporalComputeAOTImageFilter()
        {
        }

    //
    //****************************** GenerateInputRequestedRegion ********************************/
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        void
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::GenerateInputRequestedRegion()
        {

            // Call the superclass' implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // Get the input masks pointers
            ImageBaseType * lL2CLDSub = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(0));
            ImageBaseType * lIPSHDSub = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(1));
            ImageBaseType * lIPSAT = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(2));
            ImageBaseType * lL2WASSub = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(3));
            ImageBaseType * lIPEDG = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(4));
            ImageBaseType * lIPSNWSub = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(5));
            ImageBaseType * lL2inNDT = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(6));

            // Get the input images pointers
            ImageBaseType * lIPTOACSub = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(7));
            ImageBaseType * lL2inRTA = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(8));
            ImageBaseType * lL2inRTC = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(9));

            // Get the date plan
            ImageBaseType * lL2inPXD = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(10));

            // Get the dtm
            ImageBaseType * lDTMSub = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(11));

            //Get the cams aot
            if(m_UseCams)
            {
            	ImageBaseType * lAOTCamsSub = itkDynamicCastInDebugMode<ImageBaseType *>(itk::ProcessObject::GetInput(12));
            	this->PadByRadiusRequestedRegion(lAOTCamsSub);
            }

            // Add margings to the requested region
            this->PadByRadiusRequestedRegion(lL2CLDSub);
            this->PadByRadiusRequestedRegion(lIPSHDSub);
            this->PadByRadiusRequestedRegion(lIPSAT);
            this->PadByRadiusRequestedRegion(lL2WASSub);
            this->PadByRadiusRequestedRegion(lIPEDG);
            this->PadByRadiusRequestedRegion(lIPSNWSub);
            this->PadByRadiusRequestedRegion(lL2inNDT);
            this->PadByRadiusRequestedRegion(lIPTOACSub);
            this->PadByRadiusRequestedRegion(lL2inRTA);
            this->PadByRadiusRequestedRegion(lL2inRTC);
            this->PadByRadiusRequestedRegion(lL2inPXD);
            this->PadByRadiusRequestedRegion(lDTMSub);
        }

    //
    //****************************** PadByRadiusRequestedRegion ********************************/
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        void
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::PadByRadiusRequestedRegion(ImageBaseType * pImagePtr)
        {
            // Pad the input requested region by the operator radius
            SizeType lMaxRad;
            lMaxRad.Fill(m_Radius * m_MaxSamplingInterval); //2*m_Radius*m_MaxSamplingInterval+1

            // Get a copy of the input requested region (should equal the output
            // requested region)
            InputImageRegionType lTempRequestedRegion = pImagePtr->GetRequestedRegion();

            lTempRequestedRegion.PadByRadius(lMaxRad);

            // Crop the input requested region at the input's largest possible region
            if (lTempRequestedRegion.Crop(pImagePtr->GetLargestPossibleRegion()))
            {
                pImagePtr->SetRequestedRegion(lTempRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                pImagePtr->SetRequestedRegion(lTempRequestedRegion);

                // build an exception
                vnsExceptionBusinessMacro("Requested region is (at least partially) outside the largest possible region.");
            }
        }

    //
    //******************* BeforeThreadedGenerateData function. Validate inputs *******************
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        void
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::BeforeThreadedGenerateData()
        {
            // Get the input masks pointers
            InputMaskConstPointerType lL2CLDSub = this->GetL2CLDSub();
            InputMaskConstPointerType lIPSHDSub = this->GetIPSHDSub();
            InputVectorMaskConstPointerType lIPSAT = this->GetIPSAT();
            InputMaskConstPointerType lL2WASSub = this->GetL2WASSub();
            InputMaskConstPointerType lIPEDG = this->GetIPEDG();
            InputMaskConstPointerType lIPSNWSub = this->GetIPSNWSub();
            InputMaskConstPointerType lL2inNDT = this->GetL2inNDT();

            // Get the input images pointers
            InputImageConstPointerType lIPTOACSub = this->GetIPTOACSub();
            InputImageConstPointerType lL2inRTA = this->GetL2inRTA();
            InputImageConstPointerType lL2inRTC = this->GetL2inRTC();

            // Get the date plan
            InputDateConstPointerType lL2inPXD = this->GetL2inPXD();

            // Get the dtm
            InputDTMConstPointerType lDTMSub = this->GetDTMSub();

            if ((lL2CLDSub.IsNull() == true) || (lIPSHDSub.IsNull() == true) || (lIPSAT.IsNull() == true) || (lL2WASSub.IsNull() == true)
                    || (lIPEDG.IsNull() == true) || (lIPSNWSub.IsNull() == true) || (lL2inNDT.IsNull() == true)
                    || (lIPTOACSub.IsNull() == true) || (lL2inRTA.IsNull() == true) || (lL2inRTC.IsNull() == true)
                    || (lL2inPXD.IsNull() == true) || (lDTMSub.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

            this->CopyLMOptimizationAttributes();
        }

    //
    //******************* ThreadedGenerateData function *******************
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        void
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {
            // Evaluate the progress

            // Enlarge output region to the correct size
            InputImageRegionType lInputRegionForThread;
            this->CallCopyOutputRegionToInputRegion(lInputRegionForThread, outputRegionForThread);

            // Get the input masks pointers
            InputMaskConstPointerType lL2CLDSubPtr = this->GetL2CLDSub();
            InputMaskConstPointerType lIPSHDSubPtr = this->GetIPSHDSub();
            InputVectorMaskConstPointerType lIPSATPtr = this->GetIPSAT();
            InputMaskConstPointerType lL2WASSubPtr = this->GetL2WASSub();
            InputMaskConstPointerType lIPEDGPtr = this->GetIPEDG();
            InputMaskConstPointerType lIPSNWSubPtr = this->GetIPSNWSub();
            InputMaskConstPointerType lL2inNDTPtr = this->GetL2inNDT();

            // Get the input images pointers
            InputImageConstPointerType lIPTOACSubPtr = this->GetIPTOACSub();
            InputImageConstPointerType lL2inRTAPtr = this->GetL2inRTA();
            InputImageConstPointerType lL2inRTCPtr = this->GetL2inRTC();

            // Get the date plan
            InputDateConstPointerType lL2inPXDPtr = this->GetL2inPXD();
            // Get the dtm
            InputDTMConstPointerType lDTMSubPtr = this->GetDTMSub();

            // Get the output image AOT
            OutputImagePointerType lIPAOTPtr = this->GetIPAOT();

            // Get the output image AOT Cams
            OutputImagePointerType lIPAOTCamsPtr = this->GetIPAOTCams();

            //If cams get cams image
            AOTImagePointerType lAOTCamsSubPtr;
            if(m_UseCams)
            {
            	lAOTCamsSubPtr = this->GetAOTCamsSub();
            }

            // Get the output mask
            OutputMaskPointerType lOutputMaskPtr = this->GetOutputMask();

            // Define the boundary conditions (for input data)
            MaskZeroFluxNeumannBCType lL2CLDSubnbC;
            MaskZeroFluxNeumannBCType lIPSHDSubnbC;
            VectorMaskZeroFluxNeumannBCType lIPSATnbC;
            MaskZeroFluxNeumannBCType lL2WASSubnbC;
            MaskZeroFluxNeumannBCType lIPEDGnbC;
            MaskZeroFluxNeumannBCType lIPSNWSubnbC;
            MaskZeroFluxNeumannBCType lL2inNDTnbC;
            ImageZeroFluxNeumannBCType lIPTOACSubnbC;
            ImageZeroFluxNeumannBCType lL2inRTAnbC;
            ImageZeroFluxNeumannBCType lL2inRTCnbC;
            DateZeroFluxNeumannBCType lL2inPXDnbC;
            DTMZeroFluxNeumannBCType lDTMSubnbC;

            // A max radius is set to avoid having changing region dimensions with the facelist
            RadiusType lMaxRadius;
            lMaxRadius.Fill(m_Radius * m_MaxSamplingInterval);

            // Define output Iterators
            OutputImageRegionIterator lIPAOTIt = itk::ImageRegionIteratorWithIndex<OutputImageType>(lIPAOTPtr, outputRegionForThread);
            lIPAOTIt.GoToBegin();
            OutputImageRegionIterator lIPAOTCamsIt = itk::ImageRegionIteratorWithIndex<OutputImageType>(lIPAOTCamsPtr, outputRegionForThread);
                        lIPAOTCamsIt.GoToBegin();
            OutputMaskRegionIterator lOutputMaskIt = itk::ImageRegionIteratorWithIndex<OutputMaskType>(lOutputMaskPtr,
                    outputRegionForThread);
            lOutputMaskIt.GoToBegin();

            InputImageRegionType lInputRequestedRegion = lIPTOACSubPtr->GetBufferedRegion();
            // Neighborhood Iterator with size 'maxRadius'
            MaskNeighborhoodIteratorType lL2CLDSubIt = MaskNeighborhoodIteratorType(lMaxRadius, lL2CLDSubPtr, lInputRequestedRegion);
            MaskNeighborhoodIteratorType lIPSHDSubIt = MaskNeighborhoodIteratorType(lMaxRadius, lIPSHDSubPtr, lInputRequestedRegion);
            VectorMaskNeighborhoodIteratorType lIPSATIt = VectorMaskNeighborhoodIteratorType(lMaxRadius, lIPSATPtr, lInputRequestedRegion);
            MaskNeighborhoodIteratorType lL2WASSubIt = MaskNeighborhoodIteratorType(lMaxRadius, lL2WASSubPtr, lInputRequestedRegion);
            MaskNeighborhoodIteratorType lIPEDGIt = MaskNeighborhoodIteratorType(lMaxRadius, lIPEDGPtr, lInputRequestedRegion);
            MaskNeighborhoodIteratorType lIPSNWSubIt = MaskNeighborhoodIteratorType(lMaxRadius, lIPSNWSubPtr, lInputRequestedRegion);
            MaskNeighborhoodIteratorType lL2inNDTIt = MaskNeighborhoodIteratorType(lMaxRadius, lL2inNDTPtr, lInputRequestedRegion);
            ImageNeighborhoodIteratorType lIPTOACSubIt = ImageNeighborhoodIteratorType(lMaxRadius, lIPTOACSubPtr, lInputRequestedRegion);
            ImageNeighborhoodIteratorType lL2inRTAIt = ImageNeighborhoodIteratorType(lMaxRadius, lL2inRTAPtr, lInputRequestedRegion);
            ImageNeighborhoodIteratorType lL2inRTCIt = ImageNeighborhoodIteratorType(lMaxRadius, lL2inRTCPtr, lInputRequestedRegion);
            DateNeighborhoodIteratorType lL2inPXDIt = DateNeighborhoodIteratorType(lMaxRadius, lL2inPXDPtr, lInputRequestedRegion);
            DTMNeighborhoodIteratorType lDTMSubIt = DTMNeighborhoodIteratorType(lMaxRadius, lDTMSubPtr, lInputRequestedRegion);

            // Neighborhood Iterator initializations
            lL2CLDSubIt.GoToBegin();
            lL2CLDSubIt.OverrideBoundaryCondition(&lL2CLDSubnbC);
            lIPSHDSubIt.GoToBegin();
            lIPSHDSubIt.OverrideBoundaryCondition(&lIPSHDSubnbC);
            lIPSATIt.GoToBegin();
            lIPSATIt.OverrideBoundaryCondition(&lIPSATnbC);
            lL2WASSubIt.GoToBegin();
            lL2WASSubIt.OverrideBoundaryCondition(&lL2WASSubnbC);
            lIPEDGIt.GoToBegin();
            lIPEDGIt.OverrideBoundaryCondition(&lIPEDGnbC);
            lIPSNWSubIt.GoToBegin();
            lIPSNWSubIt.OverrideBoundaryCondition(&lIPSNWSubnbC);
            lL2inNDTIt.GoToBegin();
            lL2inNDTIt.OverrideBoundaryCondition(&lL2inNDTnbC);
            lIPTOACSubIt.GoToBegin();
            lIPTOACSubIt.OverrideBoundaryCondition(&lIPTOACSubnbC);
            lL2inRTAIt.GoToBegin();
            lL2inRTAIt.OverrideBoundaryCondition(&lL2inRTAnbC);
            lL2inRTCIt.GoToBegin();
            lL2inRTCIt.OverrideBoundaryCondition(&lL2inRTCnbC);
            lL2inPXDIt.GoToBegin();
            lL2inPXDIt.OverrideBoundaryCondition(&lL2inPXDnbC);
            lDTMSubIt.GoToBegin();
            lDTMSubIt.OverrideBoundaryCondition(&lDTMSubnbC);

            OffsetType lCentralOffset;
            lCentralOffset[0] = 0;
            lCentralOffset[1] = 0;

            // Do the work
            while (!lIPAOTIt.IsAtEnd())
            {
                lIPAOTIt.Set(m_NoData);
                lOutputMaskIt.Set(0);

                // Get the output index
                IndexType lIndexOut = lIPAOTIt.GetIndex();

                IndexType lIndexIn;
                for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
                {
                    lIndexIn[i] = lIndexOut[i] * m_StepAOT;
                }

                if (lInputRequestedRegion.IsInside(lIndexIn))
                {
                    lL2CLDSubIt.SetLocation(lIndexIn);
                    lIPSHDSubIt.SetLocation(lIndexIn);
                    lIPSATIt.SetLocation(lIndexIn);
                    lL2WASSubIt.SetLocation(lIndexIn);
                    lIPEDGIt.SetLocation(lIndexIn);
                    lIPSNWSubIt.SetLocation(lIndexIn);
                    lL2inNDTIt.SetLocation(lIndexIn);
                    lIPTOACSubIt.SetLocation(lIndexIn);
                    lL2inRTAIt.SetLocation(lIndexIn);
                    lL2inRTCIt.SetLocation(lIndexIn);
                    lL2inPXDIt.SetLocation(lIndexIn);
                    lDTMSubIt.SetLocation(lIndexIn);

                    //Get the cams value and set the output cams value depending on the altitude
                    AOTPixelType l_camAot = 0;
                    AOTPixelType l_camAotAlt = 0;
                    if(m_UseCams)
                    {
                    	l_camAot = lAOTCamsSubPtr->GetPixel(lIndexIn);
                    	l_camAotAlt = l_camAot;
                    	// Set pixel to DefaultValue for all bands
                    	if (lDTMSubIt.GetPixel(lCentralOffset) >= m_MeanAltitude)
                    	{
                    		//AOTCams function of altitude
                    		l_camAotAlt = l_camAot * vcl_exp( - (lDTMSubIt.GetPixel(lCentralOffset) - m_MeanAltitude)/ m_HeightScale);
                    	}
                    	lIPAOTCamsIt.Set(l_camAotAlt);
                    }

                    // Initializations
                    unsigned int lInterval = m_MinSamplingInterval;

                    // Try with the current radius (lRadius)
                    while ((lIPAOTIt.Get() == m_NoData) && (lInterval <= m_MaxSamplingInterval))
                    {
                        PixelVectType lValidIPTOAC;
                        PixelVectType lValidL2inRTA;

                        DateListType lDates;
                        OffsetVectType lValidOffsets;
                        OffsetType lOffset;

                        bool lNeighborhoodNotEmpty = false;
                        bool lAOTcomputed = false;
                        const int lRadius = static_cast<int>(m_Radius * lInterval); // (spec: N_AOT*S_N)

                        // Do not work if the central pixel is equal to NoData
                        if (lIPEDGIt.GetPixel(lCentralOffset) == 0)
                        {
                            // Access on each pixel of the current Region
                            for (int l = -lRadius; l <= lRadius; l = l + lInterval)
                            {
                                lOffset[0] = l;
                                for (int p = -lRadius; p <= lRadius; p = p + lInterval)
                                {
                                    lOffset[1] = p;
                                    // Multi temporal selection of the pixels for the local inversion
                                    MultiTemporalAOTMethodFunctionsType::AOTPixelSelection(lOffset, lIPTOACSubIt.GetPixel(lOffset),
                                            lL2inRTAIt.GetPixel(lOffset), lIPEDGIt.GetPixel(lOffset), lIPSNWSubIt.GetPixel(lOffset),
                                            lL2inNDTIt.GetPixel(lOffset), lL2CLDSubIt.GetPixel(lOffset), lIPSATIt.GetPixel(lOffset),
                                            lIPSHDSubIt.GetPixel(lOffset), lL2WASSubIt.GetPixel(lOffset), lL2inPXDIt.GetPixel(lOffset),
                                            m_AOTbandsD, m_VARBandD, m_VARBandDm1, m_RelativeVarMax, m_LowReflectanceThresholds,
                                            m_HighReflectanceThresholds, m_MaxAge, m_JDay, lValidOffsets, lValidIPTOAC, lValidL2inRTA,
                                            lDates, lNeighborhoodNotEmpty);

                                } // end for
                            } // end for
                        } // end if central pixel is not NoData

                        // A GARDER VALIDATION image du nb de pixels valides avt Pixel List 2
                        //lIPAOTIt.Set(static_cast<OutputImagePixelType>(lValidOffsets().size()));

                        if (lNeighborhoodNotEmpty)
                        {
                            // Find lLPD local previous date
                            const int lGreaterFreqLPD = MultiTemporalAOTMethodFunctionsType::ComputeGreaterFrequencyDate(lDates);

                            // Extract the lists (cf. spec : build Pixel List 3)

                            const OffsetVectType lExtractedOffsets = MultiTemporalAOTMethodFunctionsType::ExtractMoreFreqValues(lDates,
                                    lValidOffsets, lGreaterFreqLPD);

                            if (lExtractedOffsets.size() >= m_MinNumberOfPixels)
                            {
                                // Compute weight for the inversion equation
                                double lWeq1(0.);

                                const bool res = MultiTemporalAOTMethodFunctionsType::ComputeWeight(lGreaterFreqLPD, lDates, lValidIPTOAC,
                                        lValidL2inRTA, m_AOTbandsD, m_AOTbandsReflectanceCompositeDm1, m_STDEVBand, m_MinStdDevThreshold,
                                        m_StdNoise, lWeq1);

                                if (res == true)
                                {
                                    // Estimate AOT using least squares inversion - Levenberg Marquardt Inversion
                                    const OutputImagePixelType lPixAOT = this->LeastSquaresInversion(lWeq1, lGreaterFreqLPD,
                                            lExtractedOffsets, lIPTOACSubIt, lL2inRTAIt, lL2inRTCIt, lDTMSubIt, l_camAot,m_VectorOptim[threadId]);

                                    lIPAOTIt.Set(lPixAOT);

                                    // A GARDER VALIDATION Weq1 <-> image Kpond
                                    //lIPAOTIt.Set(static_cast<OutputImagePixelType>(lWeq1));

                                    // A GARDER VALIDATION -> nb de pixels <=> NbPix_MT
                                    //lOutputMaskIt.Set(static_cast<OutputMaskPixelType>(lExtractedOffsets.size()));

                                    // A GARDER VALIDATION -> image S_N <=> wide_MT
                                    //lIPAOTIt.Set(static_cast<OutputImagePixelType>(lInterval));

                                    lOutputMaskIt.Set(1);
                                    lAOTcomputed = true;
                                }
                            }
                        }
                        if (lAOTcomputed == false)
                        {
                            lInterval += m_IncrementNDistance;
                        }
                    } // end while != NoData

                } //end if inside buffered region

                ++lIPAOTIt;
                ++lIPAOTCamsIt;
                ++lOutputMaskIt;
            } // end iterate on outputRegionForThread
        }

    //
    //******************* LeastSquaresInversion *******************
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        typename MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT,
                TOutputImage, TOutputMask>::OutputImagePixelType
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::LeastSquaresInversion(const double pWeq1, const unsigned int pLPD, const OffsetVectType & pExtractedOffsets,
                ImageNeighborhoodIteratorType & pIPTOACSubIt, ImageNeighborhoodIteratorType & pL2inRTAIt,
                ImageNeighborhoodIteratorType & pL2inRTCIt, DTMNeighborhoodIteratorType & pDTMSubIt,
				const AOTPixelType & pAotCams, RunLMOptimizationPointerType & pLMOptimizer)
        {
            // Note that the other parameters such as "DarkestPixelAOT" are set
            // in CopyLMOptimizationAttributes during the BeforeThreadedGenerateData
            pLMOptimizer->GetCostFunction()->SetOffsetVect(pExtractedOffsets);
            pLMOptimizer->GetCostFunction()->SetWeq1(pWeq1);
            pLMOptimizer->GetCostFunction()->SetIPTOACSub(pIPTOACSubIt);
            pLMOptimizer->GetCostFunction()->SetL2inRTA(pL2inRTAIt);
            pLMOptimizer->GetCostFunction()->SetL2inRTC(pL2inRTCIt);
            pLMOptimizer->GetCostFunction()->SetDTMSub(pDTMSubIt);
            // Warning, SetMeasureSize must be set AFTER the AOTUsedBands
            pLMOptimizer->GetCostFunction()->SetSpaceDimension(2);
            pLMOptimizer->GetCostFunction()->SetMeasureSize(pExtractedOffsets.size());
            if (m_UseCams)
            {
            	pLMOptimizer->GetCostFunction()->SetCAMSAot(pAotCams);
            }

            // Warning, don't forget to precise the LPD LUT
            if (m_LUTMap.find(pLPD) == m_LUTMap.end())
            {
                vnsExceptionBusinessMacro(
                        "MultiTemporalComputeAOTImageFilter::LeastSquaresInversion : the LUTMap is not complete considering the dates into the PXD image. "<<pLPD<<" doesn't exist.");
            }
            else
            {
                pLMOptimizer->GetCostFunction()->SetLUTDm1(m_LUTMap[pLPD]);
            }
            // Call the Levenberg Marquardt least squares inversion
            const OutputImagePixelType lPix = pLMOptimizer->RunLMOptimization(m_F_Tolerance, m_G_Tolerance, m_X_Tolerance,
                    m_Epsilon_Function, m_Max_Iterations, m_AOTinitialD, m_AOTinitialDm1);

            return lPix;
        }

    //
    //******************* CopyLMOptimizationAttributes *******************
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        void
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::CopyLMOptimizationAttributes()
        {
            // Copy attributes from m_RunLMOptimization to the other optimization objects contained into m_VectorOptim
            // Used to work with treaded generate data
            const unsigned int lNumberOfThreads = this->GetNumberOfThreads();

            // Find AOT max  value only once
            m_RunLMOptimization->GetCostFunction()->FindAOTmax();

            // For each thread
            for (unsigned int i = 0; i < lNumberOfThreads; i++)
            {
                RunLMOptimizationPointerType lRunLMOptimization = RunLMOptimizationType::New();
                lRunLMOptimization->GetCostFunction()->SetUseCamsData(m_UseCams);
                lRunLMOptimization->GetCostFunction()->SetKPondCams(m_KPondCAMS);
                lRunLMOptimization->GetCostFunction()->SetLUTD(m_RunLMOptimization->GetCostFunction()->GetLUTD());
                lRunLMOptimization->GetCostFunction()->SetAOTbandsD(m_RunLMOptimization->GetCostFunction()->GetAOTbandsD());
                lRunLMOptimization->GetCostFunction()->SetAOTbandsReflectanceCompositeDm1(
                        m_RunLMOptimization->GetCostFunction()->GetAOTbandsReflectanceCompositeDm1());
                lRunLMOptimization->GetCostFunction()->SetAOTbandsLUTCompositeDm1(
                        m_RunLMOptimization->GetCostFunction()->GetAOTbandsLUTCompositeDm1());
                lRunLMOptimization->GetCostFunction()->SetDarkestPixelAOT(m_RunLMOptimization->GetCostFunction()->GetDarkestPixelAOT());
                lRunLMOptimization->GetCostFunction()->SetWeq2(m_RunLMOptimization->GetCostFunction()->GetWeq2());
                lRunLMOptimization->GetCostFunction()->SetWdark(m_RunLMOptimization->GetCostFunction()->GetWdark());
                lRunLMOptimization->GetCostFunction()->SetWbounds(m_RunLMOptimization->GetCostFunction()->GetWbounds());
                lRunLMOptimization->GetCostFunction()->SetAOTmin(m_RunLMOptimization->GetCostFunction()->GetAOTmin());
                lRunLMOptimization->GetCostFunction()->SetAOTmax(m_RunLMOptimization->GetCostFunction()->GetAOTmax());

                // Save the new object
                m_VectorOptim.push_back(lRunLMOptimization);
            }
        }

    /****************************** CallCopyInputRegionToOutputRegion ********************************/
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        void
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::CallCopyInputRegionToOutputRegion(OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion)
        {
            Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

            typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
            typename InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

            typename OutputImageRegionType::IndexType destIndex;
            typename OutputImageRegionType::SizeType destSize;

            for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
            {
                destIndex[i] = static_cast<long int>(vcl_ceil(static_cast<double> (srcIndex[i]) / static_cast<double> (m_StepAOT)));

                destSize[i] = static_cast<long int> (vcl_ceil(static_cast<double> (srcSize[i]) / static_cast<double> (m_StepAOT)));
            }

            destRegion.SetIndex(destIndex);
            destRegion.SetSize(destSize);
        }

    //****************************** CallCopyOutputRegionToInputRegion ********************************
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        void
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::CallCopyOutputRegionToInputRegion(InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion)
        {
            Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

            typename OutputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
            typename OutputImageRegionType::SizeType srcSize = srcRegion.GetSize();

            typename InputImageRegionType::IndexType destIndex;
            typename InputImageRegionType::SizeType destSize;

            for (unsigned int i = 0; i < InputImageType::ImageDimension; i++)
            {
                destIndex[i] = srcIndex[i] * m_StepAOT;
                destSize[i] = srcSize[i] * m_StepAOT;
            }

            destRegion.SetIndex(destIndex);
            destRegion.SetSize(destSize);
        }

    //****************************** GenerateOutputInformation ********************************
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        void
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();

            // Get the output pointers
            OutputImagePointerType outputPtr = this->GetIPAOT();
            OutputMaskPointerType maskPtr = this->GetOutputMask();
            OutputImagePointerType outputCamsPtr = this->GetIPAOTCams();
            // Get ths input pointers
            // LAIG-FA-MAC-1446-CNES
            InputImageConstPointerType inputPtr = this->GetIPTOACSub(); //this->GetInput();

            if ((inputPtr.IsNotNull() == true) && (outputPtr.IsNotNull() == true))
            {
                OutputImageRegionType newRegion;
                this->CallCopyInputRegionToOutputRegion(newRegion, inputPtr->GetLargestPossibleRegion());
                outputPtr->SetLargestPossibleRegion(newRegion);
                // LAIG-FA-MAC-701-CNES : Spacing = (input spacing * (and not /) m_StepAOT where
                // m_StepAOT is defined in GIP_L2COMM
                outputPtr->SetSpacing(inputPtr->GetSpacing() * static_cast<double>(m_StepAOT));
            }
            if ((inputPtr.IsNotNull() == true) && (outputCamsPtr.IsNotNull() == true))
            {
            	OutputImageRegionType newRegion;
            	this->CallCopyInputRegionToOutputRegion(newRegion, inputPtr->GetLargestPossibleRegion());
            	outputCamsPtr->SetLargestPossibleRegion(newRegion);
            	// LAIG-FA-MAC-701-CNES : Spacing = (input spacing * (and not /) m_StepAOT where
            	// m_StepAOT is defined in GIP_L2COMM
            	outputCamsPtr->SetSpacing(inputPtr->GetSpacing() * static_cast<double>(m_StepAOT));
            }
            if ((inputPtr.IsNotNull() == true) && (maskPtr.IsNotNull() == true))
            {
                OutputMaskRegionType newMaskRegion;
                this->CallCopyInputRegionToOutputRegion(newMaskRegion, inputPtr->GetLargestPossibleRegion());
                maskPtr->SetLargestPossibleRegion(newMaskRegion);
                // LAIG-FA-MAC-701-CNES : Spacing = (input spacing * (and not /) m_StepAOT where
                maskPtr->SetSpacing(inputPtr->GetSpacing() * static_cast<double>(m_StepAOT));
            }
        }

    //
    //******************* IPAOT image accessor  ****************
    //
    /** Get the IPAOT output image */
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        typename MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT,
                TOutputImage, TOutputMask>::OutputImageType *
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::GetIPAOT(void)
        {
            return itkDynamicCastInDebugMode<OutputImageType *>(itk::ProcessObject::GetOutput(0));
        }

    /** Get the IPAOTCams output image */
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
	class TOutputImage, class TOutputMask>
    typename MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT,
	TOutputImage, TOutputMask>::OutputImageType *
	MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
	TOutputMask>::GetIPAOTCams(void)
	{
    	return itkDynamicCastInDebugMode<OutputImageType *>(itk::ProcessObject::GetOutput(2));
	}

    //
    //******************* output mask accessor  ****************
    //
    /** Get the output mask */
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        typename MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT,
                TOutputImage, TOutputMask>::OutputMaskType *
        MultiTemporalComputeAOTImageFilter<TInputImage, TInputMask, TInputVectorMask, TInputDate, TInputDTM, TInputLUT, TOutputImage,
                TOutputMask>::GetOutputMask(void)
        {
            return itkDynamicCastInDebugMode<OutputMaskType *>(itk::ProcessObject::GetOutput(1));
        }

} // End namespace vns

#endif /* vnsMultiTemporalComputeAOTImageFilter_txx */
