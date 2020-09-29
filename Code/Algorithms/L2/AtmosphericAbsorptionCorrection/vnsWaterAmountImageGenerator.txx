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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-2090-CNES : 12 decembre 2016 : Correction utilisation du VAP NoData   *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 21 novembre 2013 : Correction de l'information géographique*
 * 									dans les produits en sortie.											*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsWaterAmountImageGenerator_txx
#define __vnsWaterAmountImageGenerator_txx

#include "vnsWaterAmountImageGenerator.h"

#include "vnsMacro.h"
#include "vnsMath.h"
#include "itkImageScanlineIterator.h"
#include "itkImageScanlineConstIterator.h"

namespace vns
{

    /**
     * Constructor
     */
    template<class TTOAInputImage, class TOutputImage, class TLUT>
        WaterAmountImageGenerator<TTOAInputImage, TOutputImage, TLUT>::WaterAmountImageGenerator() :
                m_LUT(), m_WaterVaporReferenceBandId(10), m_WaterVaporBandId(11), m_MaxThreshold(8.0), m_VAPNodataValue(0)


        {
            this->SetNumberOfRequiredInputs(1);
            this->SetNumberOfIndexedInputs(1);
            this->SetNthInput(0, TOAInputImageType::New().GetPointer());
        }

    template<class TTOAInputImage, class TOutputImage, class TLUT>
        void
        WaterAmountImageGenerator<TTOAInputImage, TOutputImage, TLUT>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();

            const unsigned int nbPix = this->GetIPTOAInput()->GetNumberOfComponentsPerPixel();

            // Check the m_WaterVaporReferenceBandId validity
            if (nbPix < m_WaterVaporReferenceBandId)
            {
                vnsExceptionBusinessMacro("Invalid m_WaterVaporReferenceBandId. It should be in [0 ... "<<nbPix-1<<"].");
            }

            // Check the m_WaterVaporBandId validity
            if (nbPix < m_WaterVaporBandId)
            {
                vnsExceptionBusinessMacro("Invalid m_WaterVaporBandId. It should be in [0 ... "<<nbPix-1<<"].");
            }

            // Update the output image information : Origin, spacing, projection
            OutputImageType * outputPtr = this->GetOutput();

            outputPtr->SetOrigin(this->GetIPTOAInput()->GetOrigin());
            outputPtr->SetSpacing(this->GetIPTOAInput()->GetSpacing());

            // Get projection ref from input
            // Get the metadata dictionary of the image source
            itk::MetaDataDictionary & dict = this->GetIPTOAInput()->GetMetaDataDictionary();
            // Set the projection in the metadata dictionary
            std::string projectionRef;
            itk::ExposeMetaData<std::string>(dict, otb::MetaDataKey::ProjectionRefKey, projectionRef);
            // Get the metadata dictionary of the output image
            itk::MetaDataDictionary & outdict = outputPtr->GetMetaDataDictionary();
            // Set the projection
            itk::EncapsulateMetaData<std::string>(outdict, otb::MetaDataKey::ProjectionRefKey, projectionRef);
        }

    template<class TTOAInputImage, class TOutputImage, class TLUT>
        void
        WaterAmountImageGenerator<TTOAInputImage, TOutputImage, TLUT>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Get input
            TOAInputImageConstPointerType toaPtr = this->GetIPTOAInput();
            // Get the output pointer
            OutputImageType * outputPtr = this->GetOutput();

            // local iterators
            itk::ImageScanlineConstIterator<TOAInputImageType> toaIt(toaPtr, outputRegionForThread);
            itk::ImageScanlineIterator<OutputImageType> outIt(outputPtr, outputRegionForThread);

            // Init iterators to begin
            toaIt.GoToBegin();
            outIt.GoToBegin();

            // Set the water vapor band id
            const unsigned int lWaterVaporReferenceBandId(m_WaterVaporReferenceBandId);
            const unsigned int lWaterVaporBandId(m_WaterVaporBandId);

            // Progress reporter

            LUTPointType point;
            point.Fill((LUTPointValueType) 0);

            // Loop image
            while ((outIt.IsAtEnd() == false) && (toaIt.IsAtEnd() == false))
            {
                while ((outIt.IsAtEndOfLine() == false) && (toaIt.IsAtEndOfLine() == false))
                {
                    const TOAInputPixelType & lPix = toaIt.Get();
                    const double toa865 = static_cast<double>(lPix[lWaterVaporReferenceBandId]);

                    // test if no data
                    if (toa865 < CONST_EPSILON)
                    {
                        outIt.Set(static_cast<OutputPixelType>(m_VAPNodataValue));
                    }
                    else
                    {
                        const double toaRatio = static_cast<double>(lPix[lWaterVaporBandId]);
                        point[0] = static_cast<LUTPointValueType>(toaRatio / toa865);

                        // Compute the value with the LUT
                        LUTPixelType interpValue = m_LUT->InterpolateValue(point);

                        if (interpValue < static_cast<LUTPixelType>(0))
                        {
                            outIt.Set(m_VAPNodataValue);
                        }
                        else if (interpValue > m_MaxThreshold )
                        {
                        	outIt.Set(m_MaxThreshold);
                        }
                        else
                        {
                            outIt.Set(static_cast<OutputPixelType>(interpValue));
                        }
                    }
                    ++toaIt;
                    ++outIt;
                }
                toaIt.NextLine();
                outIt.NextLine();
            }
        }

    template<class TTOAInputImage, class TOutputImage, class TLUT>
        void
        WaterAmountImageGenerator<TTOAInputImage, TOutputImage, TLUT>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // end namespace vns

#endif
