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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-1355-CNES : 16 juillet 2015 : Suppression sortie en erreur si 		*
 * 																	green refl + swir refl = 0            	*
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mars 2014 : Améliorations algorithmiques                *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 19 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0  : 19 oct. 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskDeterminationImageFilter_txx
#define __vnsSnowMaskDeterminationImageFilter_txx

#include "vnsSnowMaskDeterminationImageFilter.h"
#include "vnsMath.h"

namespace vns
{

    /** Constructor */
    template<class TInputVectorImage, class TInputMask, class TInputIncidenceAngleImage, class TOutputMask>
        SnowMaskDeterminationImageFilter<TInputVectorImage, TInputMask, TInputIncidenceAngleImage, TOutputMask>::SnowMaskDeterminationImageFilter() :
                m_RefBand(0), m_AbsBand(0), m_RedBand(0), m_NDSIThreshold(0.), m_RedThreshold(0.), m_SWIRThreshold(0.),
				m_HasSWIR(true),m_ThetaS(0.), m_MinCosI(0.), m_UseWaterMask(false), m_UseIncidencesAngles(true)
        {
            this->SetNumberOfIndexedInputs(4);
            this->SetNumberOfRequiredInputs(2);
            this->SetNumberOfIndexedOutputs(1);

            OutputMaskPointer output = static_cast<OutputMaskType*>(this->MakeOutput(0).GetPointer());
            this->itk::ProcessObject::SetNthOutput(0, output.GetPointer());
            //this->SetNthOutput(0, OutputMaskType::New());
        }

    /** Destructor */
    template<class TInputVectorImage, class TInputMask, class TInputIncidenceAngleImage, class TOutputMask>
        SnowMaskDeterminationImageFilter<TInputVectorImage, TInputMask, TInputIncidenceAngleImage, TOutputMask>::~SnowMaskDeterminationImageFilter()
        {
        }

    /** PrintSelf method */
    template<class TInputVectorImage, class TInputMask, class TInputIncidenceAngleImage, class TOutputMask>
        void
        SnowMaskDeterminationImageFilter<TInputVectorImage, TInputMask, TInputIncidenceAngleImage, TOutputMask>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            this->Superclass::PrintSelf(os, indent);
        }

    //
    //******************* ThreadedGenerateData function *******************
    //
    template<class TInputVectorImage, class TInputMask, class TInputIncidenceAngleImage, class TOutputMask>
        void
        SnowMaskDeterminationImageFilter<TInputVectorImage, TInputMask, TInputIncidenceAngleImage, TOutputMask>::BeforeThreadedGenerateData()
        {
            // Get input images
            InputImageConstPointer l_TOCRPtr = this->GetIPTOCRInput();
            InputMaskConstPointer l_NotValidPtr = this->GetIPNotValidMaskSubInput();
            InputIncidenceAngleImageConstPointer l_IncidenceAnglePtr = this->GetIncidenceAngleInput();
            InputMaskConstPointer l_WaterMaskPtr = this->GetL2inWASInput();

            // Check if all the input images are available
            if ((l_TOCRPtr.IsNull() == true) || (l_NotValidPtr.IsNull() == true)
                    || (m_UseWaterMask && l_WaterMaskPtr.IsNull() == true)|| (m_UseIncidencesAngles && l_IncidenceAnglePtr.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

            // Check the input sizes
            SizeType lSize1 = l_TOCRPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize2 = l_NotValidPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize3 = lSize2;
            SizeType lSize4 = lSize3;
            // Options
            if (m_UseWaterMask)
            {
                lSize4 = l_WaterMaskPtr->GetLargestPossibleRegion().GetSize();
            }
            if (m_UseIncidencesAngles)
            {
            	lSize3 = l_IncidenceAnglePtr->GetLargestPossibleRegion().GetSize();
            }

            if ((lSize1 != lSize2) || (lSize1 != lSize3) || (lSize1 != lSize4))
            {
                itkExceptionMacro(<< "At least one size is not correct.");
            }

        }

    /** Threaded Generate data */
    template<class TInputVectorImage, class TInputMask, class TInputIncidenceAngleImage, class TOutputMask>
        void
        SnowMaskDeterminationImageFilter<TInputVectorImage, TInputMask, TInputIncidenceAngleImage, TOutputMask>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {

            // Get the input image pointers
            InputImageConstPointer l_TOCRPtr = this->GetIPTOCRInput();
            InputMaskConstPointer l_NotValidPtr = this->GetIPNotValidMaskSubInput();
            InputIncidenceAngleImageConstPointer l_IncidenceAnglePtr = this->GetIncidenceAngleInput();
            InputMaskConstPointer l_WaterMaskPtr = this->GetL2inWASInput();

            // Get outputMasks
            OutputMaskPointer l_OutputPtr = this->GetOutput(0);

            // Define input Iterators
            InputImageConstIteratorType l_TOCRIt = InputImageConstIteratorType(l_TOCRPtr, outputRegionForThread);
            InputMaskConstIteratorType l_NotValidIt = InputMaskConstIteratorType(l_NotValidPtr, outputRegionForThread);
            InputIncidenceAngleImageConstIteratorType l_IncidenceAngleIt;
            InputMaskConstIteratorType l_WaterMaskIt;

            // Define output Iterators
            OutputMaskIteratorType l_OutputIt = OutputMaskIteratorType(l_OutputPtr, outputRegionForThread);

            l_TOCRIt.GoToBegin();
            l_NotValidIt.GoToBegin();
            l_OutputIt.GoToBegin();

            // Define water mask Iterator
            if (m_UseWaterMask)
            {
                if (l_WaterMaskPtr.IsNull())
                {
                    vnsExceptionBusinessMacro("vnsSnowMaskDeterminationImageFilter : must use water mask but no one is given");
                }
                l_WaterMaskIt = InputMaskConstIteratorType(l_WaterMaskPtr, outputRegionForThread);
                l_WaterMaskIt.GoToBegin();
            }

            // Define incidence angles Iterator
            if (m_UseIncidencesAngles)
            {
            	if (l_IncidenceAnglePtr.IsNull())
            	{
            		vnsExceptionBusinessMacro("vnsSnowMaskDeterminationImageFilter : must use incidence angles nut none given");
            	}
            	l_IncidenceAngleIt = InputIncidenceAngleImageConstIteratorType(l_IncidenceAnglePtr, outputRegionForThread);
            	l_IncidenceAngleIt.GoToBegin();
            }

            double l_NDSI = 0.;

            // Loop
            while (l_OutputIt.IsAtEnd() == false)
            {
                const InputImagePixelType & pixel = l_TOCRIt.Get();
                //compute coefficient of NDSI correction
                InputIncidenceAngleImagePixelType correctionCoef = 1.0;
                if (m_UseIncidencesAngles)
                {
                	const InputIncidenceAngleImagePixelType & incidenceAngle = l_IncidenceAngleIt.Get();
                	InputIncidenceAngleImagePixelType correctedIncidenceAngle = incidenceAngle;
                	if (correctedIncidenceAngle < m_MinCosI)
                	{
                		correctedIncidenceAngle = m_MinCosI;
                	}
                	if (vnsEqualsDoubleMacro(correctedIncidenceAngle, 0.0) == true)
                	{
                		vnsExceptionBusinessMacro(
                				"vnsSnowMaskDeterminationImageFilter : divide by 0, correctedIncidenceAngle = " << correctedIncidenceAngle << " (maybe m_MinCosI is not set?)");
                	}
                	correctionCoef = std::cos( m_ThetaS ) / correctedIncidenceAngle;
                }

                //compute NDSI
                const double l_refRefl = correctionCoef * pixel[m_RefBand];
                const double l_absRefl = correctionCoef * pixel[m_AbsBand];
                const double l_redRefl = correctionCoef * pixel[m_RedBand];

                const double l_denom = l_refRefl + l_absRefl;

                // LAIG-FA-MAC-1355-CNES : no exception thrown if l_denom is null
                if (vnsEqualsDoubleMacro(l_denom, 0.0) == true)
                {
                    l_OutputIt.Set(0);
                }
                else
                {
                    l_NDSI = (l_refRefl - l_absRefl) / l_denom;
                    // Test threshold
                    if ((l_NDSI > m_NDSIThreshold) && (l_redRefl > m_RedThreshold) && (!m_HasSWIR || l_absRefl < m_SWIRThreshold)
                            && (l_NotValidIt.Get() == 0))
                    {
                        if (m_UseWaterMask && l_WaterMaskIt.Get() == 0)
                        {
                            l_OutputIt.Set(1);
                        }
                        else if (!m_UseWaterMask)
                        {
                            l_OutputIt.Set(1);
                        }
                        else
                        {
                            l_OutputIt.Set(0);
                        }
                    }
                    else
                    {
                        l_OutputIt.Set(0);
                    }
                }

                // Increments iterators
                ++l_TOCRIt;
                ++l_NotValidIt;
                if (m_UseIncidencesAngles)
                {
                	++l_IncidenceAngleIt;
                }
                //++l_WaterMaskIt;

                if (m_UseWaterMask)
                {
                    ++l_WaterMaskIt;
                }
                ++l_OutputIt;
            }
        }

} // End namespace vns

#endif /* __vnsSnowMaskDeterminationImageFilter_txx */
