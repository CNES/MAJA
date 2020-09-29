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
 * MOD : VERSION : 1-0-0  : 27 oct. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMultiSpectralAOTMethodFunctions_txx
#define __vnsMultiSpectralAOTMethodFunctions_txx

#include "vnsMultiSpectralAOTMethodFunctions.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TInputVectorMask>
        MultiSpectralAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask>::MultiSpectralAOTMethodFunctions()
        {
        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TInputVectorMask>
        MultiSpectralAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask>::~MultiSpectralAOTMethodFunctions()
        {
        }

    template<class TInputImage, class TInputMask, class TInputVectorMask>
        void
        MultiSpectralAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask>::AOTPixelSelection(const OffsetType & pOffset,
                const InputImagePixelType & pIPTOACSubPix, const InputMaskPixelType & pIPEDGPix, const InputMaskPixelType & pIPSNWSubPix,
                const InputMaskPixelType & pL2CLDSubPix, const InputVectorMaskPixelType & pIPSATPix,
                const InputMaskPixelType & pIPSHDSubPix, const InputMaskPixelType & pL2WASSubPix,
                const std::vector<unsigned int> & pAOTbands, const unsigned int & pRedChannel, const unsigned int & pNDVIAOTBand1,
                const unsigned int & pNDVIAOTBand2, const double & pRedTOAThreshold, const double & pNDVIThreshold,
                OffsetVectType & pValidOffsets, PixelVectType & pValidIPTOAC, bool & pNeighborhoodNotEmpty)
        {

            const unsigned int lNbOfBands = pAOTbands.size();

            // Compute NDVI
            NDVIFunctorType lNDVIFunctor;

            // Super if
            if (
            // Discard all NoData pixels date D
                    (pIPEDGPix == 0)
            // Discard all cloudy pixels
                    && (pL2CLDSubPix == 0)
            // Discard all cast shadow pixels
                    && (pIPSHDSubPix == 0)
            // Discard saturated pixels for the red band
                    && (pIPSATPix[pRedChannel] == 0)
            // Discard all water pixels
                    && (pL2WASSubPix == 0)
            // Discard snow pixel if the SWIR band is available (S2 + L5/L7)
                    && (pIPSNWSubPix == 0))
            {
                // Discard saturated pixels for all used bands
                bool IsSaturatedPixel = false;
                if ((pIPSATPix[pNDVIAOTBand1] != static_cast<InputVectorMaskInternalPixelType>(0))
                        || (pIPSATPix[pNDVIAOTBand2] != static_cast<InputVectorMaskInternalPixelType>(0)))
                {
                    IsSaturatedPixel = true;
                }
                for (unsigned int i = 0; i < lNbOfBands; i++)
                {
                    if (pIPSATPix[pAOTbands[i]] != static_cast<InputVectorMaskInternalPixelType> (0))
                    {
                        IsSaturatedPixel = true;
                    }
                }

                // Discard all saturated pixels for AOTUsedBands
                if (IsSaturatedPixel == false)
                {
                    // Discard pixels for which the red reflectance and the NDVI are greater than thresholds

                    const InternalPixelType lrTOA_RED = pIPTOACSubPix[pRedChannel];
                    InternalPixelType l_NDVIValue;

                    // Discard pixels for which red reflectance > pRedTOAThreshold
                    if (lrTOA_RED <= pRedTOAThreshold)
                    {
                        const InternalPixelType lrTOA_NDVI1 = pIPTOACSubPix[pNDVIAOTBand1];
                        const InternalPixelType lrTOA_NDVI2 = pIPTOACSubPix[pNDVIAOTBand2];
                        itk::VariableLengthVector<InternalPixelType> v(2);
                        v[0] = lrTOA_NDVI2 ;
                        v[1] = lrTOA_NDVI1 ;

                        lNDVIFunctor.SetBandIndex(CommonBandNames::RED, 1);
                        lNDVIFunctor.SetBandIndex(CommonBandNames::NIR, 2);
                        // Compute NDVI indice
                        l_NDVIValue = lNDVIFunctor(v);

                        // Discard pixels for which NDVI < m_NDVIThreshold
                        if (l_NDVIValue >= pNDVIThreshold)
                        {
                            pNeighborhoodNotEmpty = true;
                            pValidOffsets.push_back(pOffset);
                            pValidIPTOAC.push_back(pIPTOACSubPix);
                        } // end if NDVI
                    } // end if red reflectance

                } // end saturated pixels

            } // end super if

        }



    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TInputVectorMask>
        void
        MultiSpectralAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            this->Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsMultiSpectralAOTMethodFunctions_txx */
