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
 * VERSION : 3.3.0 : DM : LAIG-DM-MAJA-3044-CNES : 15 mars 2019 : Integration modifications oliver hagolle  *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 12 octobre 2016 : Audit code - Correction qualite :       *
 *                                               Regle : 0-1-3 A project shall not contain unused variables *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 avril 2016 : Modification de l’algorithme de detection   *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 3 mars 2014 : Améliorations algorithmiques                 *
 * VERSION : 1-0-0-3 : DM : 357 : 02 sep. 2011 : pas de correlation avec la date courante                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsReflectanceVariationCorrelationImageFilter_txx
#define __vnsReflectanceVariationCorrelationImageFilter_txx

#include "vnsReflectanceVariationCorrelationImageFilter.h"

#include "otbImage.h"

#include "itkImageRegionIterator.h"

#include "vnsMath.h"


namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TDateImage, class TOutputImage>
        ReflectanceVariationCorrelationImageFilter<TInputImage, TInputMask, TDateImage, TOutputImage>::ReflectanceVariationCorrelationImageFilter() :
                m_NoData(static_cast<RealNoDataType>(0)), m_BlueBandTOCR(0), m_RedBandTOCR(0), m_NIRBandTOCR(0), m_BlueBandRCR(0), m_RedBandRCR(0), m_NIRBandRCR(0), m_JDay(0.), m_ForgettingDuration(0.), m_MinVarThreshold(
                        0.), m_MaxVarThreshold(0.), m_KNIRBlue(0.), m_KRedBlue(0.), m_DeltaThreshold(0.), m_MinPixelCorrelation(0), m_CorrelBandTOCR(0),m_CorrelBandRCR(0), m_CorrelThreshold(
                        0.)

        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(8);
            this->SetNumberOfRequiredInputs(8);
            this->SetNumberOfIndexedOutputs(3);

            // Allocate the outputs pointers
            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, OutputImageType::New());
            this->SetNthOutput(2, OutputImageType::New());

            // Initialize the ncorrel tab
            m_NCorrel.Fill(3);

        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TDateImage, class TOutputImage>
        ReflectanceVariationCorrelationImageFilter<TInputImage, TInputMask, TDateImage, TOutputImage>::~ReflectanceVariationCorrelationImageFilter()
        {
        }

    //****************************** GenerateInputRequestedRegion ********************************/
    /** GenerateInputRequestedRegion method */
    template<class TInputImage, class TInputMask, class TDateImage, class TOutputImage>
        void
        ReflectanceVariationCorrelationImageFilter<TInputImage, TInputMask, TDateImage, TOutputImage>::GenerateInputRequestedRegion()
        {

            // call the superclass' implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // get pointers to the input and output images
            InputImagePointer l_IPTOCRPtr = const_cast<InputImageType *>(this->GetIPTOCRInput());
            InputImagePointer l_L2inRCRPtr = const_cast<InputImageType *>(this->GetL2inRCRInput());
            InputImagePointer l_L2inSTOPtr = const_cast<InputImageType *>(this->GetL2inSTOInput());
            DateImagePointer l_L2inPXDPtr = const_cast<DateImageType *>(this->GetL2inPXDInput());
            InputMaskPointer l_IPEDGPtr = const_cast<InputMaskType *>(this->GetIPEDGInput());
            InputMaskPointer l_L2inNDTPtr = const_cast<InputMaskType *>(this->GetL2inNDTInput());
            InputMaskPointer l_L2inWASandSNWPtr = const_cast<InputMaskType *>(this->GetConditionalMaskInput());
            InputMaskPointer l_IPCLDReflPtr = const_cast<InputMaskType *>(this->GetIPCLDReflInput());

            OutputImagePointer l_IPCLDReflVarPtrOutput = this->GetIPCLDReflVarOutput();
            OutputImagePointer l_IPCLDExtPtrOutput = this->GetIPCLDExtOutput();
            OutputImagePointer l_IPCLDReflOutput = this->GetIPCLDReflOutput();

            if ((l_IPTOCRPtr.IsNull() == true) || (l_L2inRCRPtr.IsNull() == true) || (l_L2inSTOPtr.IsNull() == true)
                    || (l_L2inPXDPtr.IsNull() == true) || (l_IPEDGPtr.IsNull() == true) || (l_L2inNDTPtr.IsNull() == true)
                    || (l_L2inWASandSNWPtr.IsNull() == true) || (l_IPCLDReflPtr.IsNull() == true) || (l_IPCLDReflVarPtrOutput.IsNull() == true)
                    || (l_IPCLDExtPtrOutput.IsNull() == true) || (l_IPCLDReflOutput.IsNull() == true))
            {
                vnsExceptionBusinessMacro(
                        "ReflectanceVariationCorrelationImageFilter: Internal error: One (or more) Input pointer is NULL !")
            }
            // get a copy of the input requested region (should equal the output
            // requested region)
            RegionType l_IPTOCRRequestedRegion;
            RegionType l_L2inRCRRequestedRegion;
            RegionType l_L2inSTORequestedRegion;
            RegionType l_L2inPXDRequestedRegion;
            RegionType l_IPEDGRequestedRegion;
            RegionType l_L2inNDTRequestedRegion;
            RegionType l_L2inWASandSNWRequestedRegion;
            RegionType l_IPCLDReflRequestedRegion;
            RegionType l_IPCLDVarReflRequestedRegion;
            RegionType l_IPCLDExtRequestedRegion;

            // pad the input requested region by the operator radius
            SizeType maxRad;
            maxRad[0] = m_NCorrel[0];
            maxRad[1] = m_NCorrel[1];

            // IPTOCR Region
            l_IPTOCRRequestedRegion = l_IPTOCRPtr->GetRequestedRegion();
            l_IPTOCRRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPTOCRRequestedRegion.Crop(l_IPTOCRPtr->GetLargestPossibleRegion()))
            {
                l_IPTOCRPtr->SetRequestedRegion(l_IPTOCRRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.
                // store what we tried to request (prior to trying to crop)
                l_IPTOCRPtr->SetRequestedRegion(l_IPTOCRRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(l_IPTOCRPtr);
                throw e;
            }

            // L2inRCR Region
            l_L2inRCRRequestedRegion = l_L2inRCRPtr->GetRequestedRegion();
            l_L2inRCRRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inRCRRequestedRegion.Crop(l_L2inRCRPtr->GetLargestPossibleRegion()))
            {
                l_L2inRCRPtr->SetRequestedRegion(l_L2inRCRRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.
                // store what we tried to request (prior to trying to crop)
                l_L2inRCRPtr->SetRequestedRegion(l_L2inRCRRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(l_L2inRCRPtr);
                throw e;
            }

            // L2inSTO Region
            l_L2inSTORequestedRegion = l_L2inSTOPtr->GetRequestedRegion();
            l_L2inSTORequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inSTORequestedRegion.Crop(l_L2inSTOPtr->GetLargestPossibleRegion()))
            {
                l_L2inSTOPtr->SetRequestedRegion(l_L2inSTORequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.
                // store what we tried to request (prior to trying to crop)
                l_L2inSTOPtr->SetRequestedRegion(l_L2inSTORequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(l_L2inSTOPtr);
                throw e;
            }

            // L2inPXD Region
            l_L2inPXDRequestedRegion = l_L2inPXDPtr->GetRequestedRegion();
            l_L2inPXDRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inPXDRequestedRegion.Crop(l_L2inPXDPtr->GetLargestPossibleRegion()))
            {
                l_L2inPXDPtr->SetRequestedRegion(l_L2inPXDRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.
                // store what we tried to request (prior to trying to crop)
                l_L2inPXDPtr->SetRequestedRegion(l_L2inPXDRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(l_L2inPXDPtr);
                throw e;
            }

            // IPEDG Region
            l_IPEDGRequestedRegion = l_IPEDGPtr->GetRequestedRegion();

            l_IPEDGRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPEDGRequestedRegion.Crop(l_IPEDGPtr->GetLargestPossibleRegion()))
            {
                l_IPEDGPtr->SetRequestedRegion(l_IPEDGRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.
                // store what we tried to request (prior to trying to crop)
                l_IPEDGPtr->SetRequestedRegion(l_IPEDGRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(l_IPEDGPtr);
                throw e;
            }

            // L2inNDT Region
            l_L2inNDTRequestedRegion = l_L2inNDTPtr->GetRequestedRegion();

            l_L2inNDTRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inNDTRequestedRegion.Crop(l_L2inNDTPtr->GetLargestPossibleRegion()))
            {
                l_L2inNDTPtr->SetRequestedRegion(l_L2inNDTRequestedRegion);

            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                l_L2inNDTPtr->SetRequestedRegion(l_L2inNDTRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(l_L2inNDTPtr);
                throw e;

            }

            // L2inWAS and SNW mask Region
            l_L2inWASandSNWRequestedRegion = l_L2inWASandSNWPtr->GetRequestedRegion();
            l_L2inWASandSNWRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_L2inWASandSNWRequestedRegion.Crop(l_L2inWASandSNWPtr->GetLargestPossibleRegion()))
            {
                l_L2inWASandSNWPtr->SetRequestedRegion(l_L2inWASandSNWRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.
                // store what we tried to request (prior to trying to crop)
                l_L2inWASandSNWPtr->SetRequestedRegion(l_L2inWASandSNWRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(l_L2inWASandSNWPtr);
                throw e;
            }

            // IPCLDRefl Region
            l_IPCLDReflRequestedRegion = l_IPCLDReflPtr->GetRequestedRegion();

            l_IPCLDReflRequestedRegion.PadByRadius(maxRad);

            // crop the input requested region at the input's largest possible region
            if (l_IPCLDReflRequestedRegion.Crop(l_IPCLDReflPtr->GetLargestPossibleRegion()))
            {
                l_IPCLDReflPtr->SetRequestedRegion(l_IPCLDReflRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.
                // store what we tried to request (prior to trying to crop)
                l_IPCLDReflPtr->SetRequestedRegion(l_IPCLDReflRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(l_IPCLDReflPtr);
                throw e;
            }
        }

    /** BeforeThreadedGenerateData method */
    template<class TInputImage, class TInputMask, class TDateImage, class TOutputImage>
        void
        ReflectanceVariationCorrelationImageFilter<TInputImage, TInputMask, TDateImage, TOutputImage>::BeforeThreadedGenerateData()
        {
            // Conversion of the correlation threshold parameter that is in percent
            // MACCS 5.0 : Already /100.  m_CorrelThreshold = m_CorrelThreshold / static_cast<CorrelationPixelType>(100.); // Initial parameter in percent

            // First, clear the functor list
            m_FunctorVector.clear();

            // Get the number of Threads
            const unsigned int l_NumberOfThreads(this->GetNumberOfThreads());

            // Loop on threads
            for (unsigned int i = 0; i < l_NumberOfThreads; i++)
            {
                FunctorType l_Functor;
                l_Functor.SetNoData(m_NoData);
                l_Functor.SetMinPixelCorrelation(m_MinPixelCorrelation);
                l_Functor.SetCorrelBandA(m_CorrelBandTOCR);
                l_Functor.SetCorrelBandB(m_CorrelBandRCR);
                m_FunctorVector.push_back(l_Functor);
            }

        }

    //****************************** ThreadedGenerateData ********************************/
    /** ThreadedGenerateData method */
    template<class TInputImage, class TInputMask, class TDateImage, class TOutputImage>
        void
        ReflectanceVariationCorrelationImageFilter<TInputImage, TInputMask, TDateImage, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {

            // Variable threshold value
            double l_Threshold(0.);
            //Epsilon for comparison
            const double l_Epsilon = CONST_EPSILON;

	    //Haze Optmized transform variables
        double l_IPHOT(0.);
	    double l_L2inRCRHOT(0.);

	    // Reflectance variation parameters
            double l_BlueReflVar(0.);
            double l_RedReflVar(0.);
            double l_BlueReflRelativVar(0.);
            double l_NIRReflRelativVar(0.);
            double l_Sum(0.);
	    double l_HOTVar(0.);


            itk::ZeroFluxNeumannBoundaryCondition < InputImageType > l_IPTOCRnbC;
            itk::ZeroFluxNeumannBoundaryCondition < InputImageType > l_L2inRCRnbC;
            itk::ZeroFluxNeumannBoundaryCondition < InputImageType > l_L2inSTOnbC;
            itk::ZeroFluxNeumannBoundaryCondition < InputMaskType > l_IPEDGnbC;
            itk::ZeroFluxNeumannBoundaryCondition < InputMaskType > l_L2inNDTnbC;


            // Get pointers to input images
            InputImageConstPointer l_IPTOCRPtr = this->GetIPTOCRInput();
            InputImageConstPointer l_L2inRCRPtr = this->GetL2inRCRInput();
            InputImageConstPointer l_L2inSTOPtr = this->GetL2inSTOInput();
            DateImageConstPointer l_L2inPXDPtr = this->GetL2inPXDInput();
            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
            InputMaskConstPointer l_L2inNDTPtr = this->GetL2inNDTInput();
            InputMaskConstPointer l_L2inWASandSNWPtr = this->GetConditionalMaskInput();
            InputMaskConstPointer l_IPCLDReflPtr = this->GetIPCLDReflInput();

            // Get the output mask
            OutputImagePointer l_IPCLDReflVarPtrOutput = this->GetIPCLDReflVarOutput();
            OutputImagePointer l_IPCLDExtPtrOutput = this->GetIPCLDExtOutput();
            OutputImagePointer l_IPCLDReflPtrOutput = this->GetIPCLDReflOutput();

            // Get the coefficient of correlation
            CorrelationPixelType l_CorrelationCoef;
            CorrelationPixelType l_StackCorrelationCoef;

            RadiusType l_Radius;
            l_Radius[0] = m_NCorrel[0];
            l_Radius[1] = m_NCorrel[1];

            // Define the input neighborhood iterators
            NeighborhoodIteratorType l_IPTOCRIt(l_Radius, l_IPTOCRPtr, outputRegionForThread);
            NeighborhoodIteratorType l_L2inRCRIt(l_Radius, l_L2inRCRPtr, outputRegionForThread);
            NeighborhoodIteratorType l_L2inSTOIt(l_Radius, l_L2inSTOPtr, outputRegionForThread);
            MaskNeighborhoodIteratorType l_IPEDGIt(l_Radius, l_IPEDGPtr, outputRegionForThread);
            MaskNeighborhoodIteratorType l_L2inNDTIt(l_Radius, l_L2inNDTPtr, outputRegionForThread);

           // Define the input image region iterators
            // Those iterators are used to perform the correlation between the
            // surface reflectance images of D and D-1
            itk::ImageRegionConstIterator < DateImageType > l_L2inPXDIt(l_L2inPXDPtr, outputRegionForThread);
            itk::ImageRegionConstIterator < InputMaskType > l_L2inWASandSNWIt(l_L2inWASandSNWPtr, outputRegionForThread);
            itk::ImageRegionConstIterator < InputMaskType > l_IPCLDReflIt(l_IPCLDReflPtr, outputRegionForThread);

            // Define the output image iterators
            // (Images not used for the correlation)
            itk::ImageRegionIterator < OutputImageType > l_IPCLDExtItOutput(l_IPCLDExtPtrOutput, outputRegionForThread);
            itk::ImageRegionIterator < OutputImageType > l_IPCLDReflVarItOutput(l_IPCLDReflVarPtrOutput, outputRegionForThread);
            // MACCS 5.0.0 : DM 982 : new output because updating the Input l_IPCLDReflIt -> scandalous !!!
            itk::ImageRegionIterator < OutputImageType > l_IPCLDReflItOutput(l_IPCLDReflPtrOutput, outputRegionForThread);

           // Iterator initializations
            l_IPTOCRIt.GoToBegin();
            l_IPTOCRIt.OverrideBoundaryCondition(&l_IPTOCRnbC);

            l_L2inRCRIt.GoToBegin();
            l_L2inRCRIt.OverrideBoundaryCondition(&l_L2inRCRnbC);

            l_L2inSTOIt.GoToBegin();
            l_L2inSTOIt.OverrideBoundaryCondition(&l_L2inSTOnbC);

            l_IPEDGIt.GoToBegin();
            l_IPEDGIt.OverrideBoundaryCondition(&l_IPEDGnbC);

            l_L2inNDTIt.GoToBegin();
            l_L2inNDTIt.OverrideBoundaryCondition(&l_L2inNDTnbC);

            // Set the image region iterators to the same region of the neighborhood iterators
            l_L2inPXDIt.GoToBegin();
            l_L2inWASandSNWIt.GoToBegin();
            l_IPCLDReflIt.GoToBegin();

            l_IPCLDReflVarItOutput.GoToBegin();
            l_IPCLDExtItOutput.GoToBegin();
            l_IPCLDReflItOutput.GoToBegin();

            // Functor reference
            FunctorType & l_ThreadFunc = m_FunctorVector[threadId];

            const double l_Ratio = m_MinVarThreshold / m_ForgettingDuration;

            // Pixel loop
            while (l_IPCLDReflVarItOutput.IsAtEnd() == false)
            {
                const InputImagePixelType l_IPTOCRPix = l_IPTOCRIt.GetCenterPixel();
                const InputImagePixelType l_L2inRCRPix = l_L2inRCRIt.GetCenterPixel();

                // Get the input pixel values
                const InputInternalPixelType l_IPTOCRBlueValue = l_IPTOCRPix[m_BlueBandTOCR];
                const InputInternalPixelType l_IPTOCRRedValue = l_IPTOCRPix[m_RedBandTOCR];
                const InputInternalPixelType l_IPTOCRNIRValue = l_IPTOCRPix[m_NIRBandTOCR];

                const InputInternalPixelType l_L2inRCRBlueValue = l_L2inRCRPix[m_BlueBandRCR];
                const InputInternalPixelType l_L2inRCRRedValue = l_L2inRCRPix[m_RedBandRCR];
                const InputInternalPixelType l_L2inRCRNIRValue = l_L2inRCRPix[m_NIRBandRCR];

                const DateImagePixelType l_L2inPXDValue = l_L2inPXDIt.Get();
                const InputMaskPixelType l_L2inWASandSNWValue = l_L2inWASandSNWIt.Get();
                const InputMaskPixelType l_IPCLDReflValue = l_IPCLDReflIt.Get();

                // Init the value with input image
                l_IPCLDReflItOutput.Set(l_IPCLDReflIt.Get());


                // Get the no_data mask values
                const InputMaskPixelType l_IPEDGValue = l_IPEDGIt.GetCenterPixel();
                const InputMaskPixelType l_L2inNDTValue = l_L2inNDTIt.GetCenterPixel();

                // Output mask initialization
                l_IPCLDReflVarItOutput.Set(itk::NumericTraits < OutputImagePixelType > ::Zero);
                l_IPCLDExtItOutput.Set(itk::NumericTraits < OutputImagePixelType > ::Zero);


                // Test on reflectance variation except for edges pixels (No_Data)
                if ((l_IPEDGValue == static_cast<InputMaskPixelType>(0)) && (l_L2inNDTValue == static_cast<InputMaskPixelType>(0)))
                {

                    bool l_ReflectanceVariationTest = true;
                    // Initialization
                    l_BlueReflRelativVar = static_cast<double>(m_NoData);
                    l_NIRReflRelativVar = static_cast<double>(m_NoData);

                    // Compute the variable threshold dependent on the duration between the
                    // input product and the composite product
                    // Modification of the calculation of the threshold (FTs OGM 114432)
                    l_Threshold = m_MinVarThreshold + l_Ratio * (vcl_abs(m_JDay - static_cast<double>(l_L2inPXDValue)));
                    if (l_Threshold >= m_MaxVarThreshold)
                    {
                        l_Threshold = m_MaxVarThreshold;
                    }
                    // Detection not activated for water and snow surfaces and if the values of reflectance are equals to No_Data
                    if (l_L2inWASandSNWValue == static_cast<InputMaskPixelType>(0))
                    {

                        // Compute the absolute variation in the blue band
                        l_BlueReflVar = l_IPTOCRBlueValue - l_L2inRCRBlueValue;

                        // Compute the absolute variation in the red band
                        l_RedReflVar = l_IPTOCRRedValue - l_L2inRCRRedValue;
                        l_Sum = (l_IPTOCRBlueValue + l_L2inRCRBlueValue);

			//Compute the  Haze Optimized Transformation (HOT) 
			l_IPHOT =  l_IPTOCRBlueValue -  l_IPTOCRRedValue/2.;
			l_L2inRCRHOT = l_L2inRCRBlueValue -   l_L2inRCRRedValue/2.;
			l_HOTVar= l_IPHOT - l_L2inRCRHOT ;


                        // Compute the relative variation of reflectance in the blue and NIR bands
                        if (vcl_abs(l_Sum) > CONST_EPSILON)
                        {
                            l_BlueReflRelativVar = (l_IPTOCRBlueValue - l_L2inRCRBlueValue) / l_Sum;
                        }
                        l_Sum = (l_IPTOCRNIRValue + l_L2inRCRNIRValue);

                        if (vcl_abs(l_Sum) > CONST_EPSILON)
                        {
                            l_NIRReflRelativVar = (l_IPTOCRNIRValue - l_L2inRCRNIRValue) / l_Sum;
                        }
                        // Post Condition checking
                        if ((vnsEqualsDoubleMacro(l_BlueReflRelativVar,m_NoData) == true)
                                || (vnsEqualsDoubleMacro(l_NIRReflRelativVar,m_NoData) == true))
                        {
                            l_ReflectanceVariationTest = false;
                        }
                    }
                    else
                    {
                        l_BlueReflVar = m_NoData;
                        l_RedReflVar = m_NoData;
                        l_BlueReflRelativVar = m_NoData;
                        l_NIRReflRelativVar = m_NoData;
			l_HOTVar = m_NoData;		
                        l_ReflectanceVariationTest = false;
                    }

                    // First detection of cloud pixel due to the variation of reflectance
                    if (l_ReflectanceVariationTest == true)
                    {
                        // Coefficients used to test the reflectance variation
                        const double l_KNIRBlueCoef = m_KNIRBlue * l_BlueReflRelativVar;
                        const double l_KRedBlueCoef = m_KRedBlue * l_BlueReflVar;

                        if (vnsStrictlyInfDoubleMacro(l_NIRReflRelativVar,l_KNIRBlueCoef,l_Epsilon) &&
                                vnsStrictlyInfDoubleMacro(l_RedReflVar,l_KRedBlueCoef,l_Epsilon))
                        {
                            if (vnsStrictlySupDoubleMacro(l_HOTVar,l_Threshold,l_Epsilon))
                            {
                                // The pixel is cloudy
                                l_IPCLDReflVarItOutput.Set(static_cast<OutputImagePixelType>(1));
                            }
                            if (vnsStrictlySupDoubleMacro(l_HOTVar, (l_Threshold - m_DeltaThreshold), l_Epsilon))
                            {
                                // The pixel is cloudy in the cloud mask extension
                                l_IPCLDExtItOutput.Set(static_cast<OutputImagePixelType>(1));
                            }

                        }
                    } // end if l_NIRReflRelativVar != m_NoData*/

                } // end if no_data

                // Correlation

                if ((l_IPCLDReflVarItOutput.Get() == static_cast<OutputImagePixelType>(1))
                        || (l_IPCLDReflValue == static_cast<OutputImagePixelType>(1)))
                {

                    // Correlation between the input surface reflectance image and the composite product
                    l_ThreadFunc.SetCorrelBandB(m_CorrelBandRCR);
                    l_CorrelationCoef = l_ThreadFunc(l_IPTOCRIt, l_L2inRCRIt, l_IPEDGIt);

                    // Previous images loop
                    unsigned int l_band(0);

                    // Number of bands
                    const unsigned int l_BandNumber(l_L2inSTOIt.GetCenterPixel().Size());

                    // The correlation is stopped as soon as a good correlation point is obtained
                    while (vnsInfOrEqualDoubleMacro(l_CorrelationCoef, m_CorrelThreshold, l_Epsilon) && (l_band < l_BandNumber))
                    {
                        if (vnsDifferentDoubleMacro(m_STOListOfDates[l_band],m_JDay) == true)
                        {
                            // Set the correlation band of the stack of images
                            l_ThreadFunc.SetCorrelBandB(l_band);
                            // Correlation between the input surface reflectance image and the previous image of date D-1-bd
                            l_StackCorrelationCoef = l_ThreadFunc(l_IPTOCRIt, l_L2inSTOIt, l_IPEDGIt);

                            // If the correlation was successful
                            if ( vnsDifferentDoubleMacro(l_StackCorrelationCoef,m_NoData) == true)
                            {
                                l_CorrelationCoef = l_StackCorrelationCoef;
                            }
                        }
                        l_band++;

                    }

                    // Final decision to set the pixel cloudy or not
                    if (vnsStrictlySupDoubleMacro(l_CorrelationCoef, m_CorrelThreshold,l_Epsilon))
                    {
                        // The pixel is not cloudy since it correlated with a previous image
                        l_IPCLDReflVarItOutput.Set(static_cast<OutputImagePixelType>(0));
                        l_IPCLDReflItOutput/*l_IPCLDReflIt*/.Set(static_cast<OutputImagePixelType>(0));
                    }

                } // end if cloud pixel


                ++l_IPTOCRIt;
                ++l_L2inRCRIt;
                ++l_L2inSTOIt;
                ++l_L2inPXDIt;
                ++l_IPEDGIt;
                ++l_L2inNDTIt;
                ++l_L2inWASandSNWIt;
                ++l_IPCLDReflIt;
                ++l_IPCLDReflVarItOutput;
                ++l_IPCLDExtItOutput;
                ++l_IPCLDReflItOutput;

            } // end pixel loop

        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TDateImage, class TOutputImage>
        void
        ReflectanceVariationCorrelationImageFilter<TInputImage, TInputMask, TDateImage, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            // Displays the values of all the parameters after initialisation
            os << "m_NoData:    " << m_NoData << std::endl;
            os << "m_BlueBandTOCR:    " << m_BlueBandTOCR << std::endl;
            os << "m_RedBandTOCR:    " << m_RedBandTOCR << std::endl;
            os << "m_NIRBandTOCR:    " << m_NIRBandTOCR << std::endl;
            os << "m_BlueBandRCR:    " << m_BlueBandRCR << std::endl;
            os << "m_RedBandRCR:    " << m_RedBandRCR << std::endl;
            os << "m_NIRBandRCR:    " << m_NIRBandRCR << std::endl;
            os << "m_CorrelBandTOCR:    " << m_CorrelBandTOCR << std::endl;
            os << "m_CorrelBandRCR:    " << m_CorrelBandRCR << std::endl;
            os << "m_JDay:    " << m_JDay << std::endl;
            os << "m_ForgettingDuration:    " << m_ForgettingDuration << std::endl;
            os << "m_MinVarThreshold:    " << m_MinVarThreshold << std::endl;
            os << "m_MaxVarThreshold:    " << m_MaxVarThreshold << std::endl;
            os << "m_KRedBlue:    " << m_KRedBlue << std::endl;
            os << "m_KNIRBlue:    " << m_KNIRBlue << std::endl;
            os << "m_DeltaThreshold:    " << m_DeltaThreshold << std::endl;
            os << "m_MinPixelCorrelation:    " << m_MinPixelCorrelation << std::endl;
            os << "m_CorrelThreshold:    " << m_CorrelThreshold << std::endl;
            os << "m_NCorrel:   " << m_NCorrel << std::endl;
        }

} // End namespace vns

#endif /* __vnsReflectanceVariationCorrelationImageFilter_txx */

