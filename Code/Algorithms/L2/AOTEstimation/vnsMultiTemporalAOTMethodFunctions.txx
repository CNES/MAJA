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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-2157-CNES : 14 fevrier 2017 : Correction sur l'index utilise dans    *
 *                                       les tableaux LowReflectanceThresholds et HighReflectanceThresholds *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de détection *
 *                                                                  des ombres de nuages                    *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-751-CNES : 17 février 2014 : Sélection des pixels pour l'inversion    *
 *                                  (Spec v2.2)                                                             *
 * MOD : VERSION : 1-0-0  : 27 oct. 2011 : Creation                                                           *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$                        *
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsMultiTemporalAOTMethodFunctions_txx
#define vnsMultiTemporalAOTMethodFunctions_txx

#include "vnsMultiTemporalAOTMethodFunctions.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate>
        MultiTemporalAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask, TInputDate>::MultiTemporalAOTMethodFunctions()
        {
        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate>
        MultiTemporalAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask, TInputDate>::~MultiTemporalAOTMethodFunctions()
        {
        }

    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate>
        void
        MultiTemporalAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask, TInputDate>::AOTPixelSelection(
                const OffsetType & pOffset, const InputImagePixelType & pIPTOACSubPix, const InputImagePixelType & pL2inRTAPix,
                const InputMaskPixelType & pIPEDGPix, const InputMaskPixelType & pIPSNWSubPix, const InputMaskPixelType & pL2inNDTPix,
                const InputMaskPixelType & pL2CLDPix, const InputVectorMaskPixelType & pIPSATPix, const InputMaskPixelType & pIPSHDPix,
                const InputMaskPixelType & pL2WASSubPix, const InputDatePixelType & pL2inPXDPix,
                const std::vector<unsigned int> & pAOTbands, const unsigned int pVARBandD, const unsigned int pVARBandDm1,
                const double pRelativeVarMax, const DoubleVectType & pLowReflectanceThresholds,
                const DoubleVectType & pHighReflectanceThresholds, const double pMaxAge, const double pJDay, OffsetVectType & pValidOffsets,
                PixelVectType & pValidIPTOAC, PixelVectType & pValidL2inRTA, DateListType & pDates, bool & pNeighborhoodNotEmpty)
        {
            const unsigned int lNbOfBands = pAOTbands.size();

            // Super if
            if (
            // Discard all NoData pixels date D
            (pIPEDGPix == 0)
            // Discard all NoData pixels date D-1
                    && (pL2inNDTPix == 0)
                    // Discard all cloudy pixels
                    && (pL2CLDPix == 0)
                    // Discard all cast shadow pixels
                    && (pIPSHDPix == 0)
                    // Discard saturated pixels for the VARBand band
                    && (pIPSATPix[pVARBandD] == 0)
                    // Discard all water pixels
                    && (pL2WASSubPix == 0)
                    // Discard snow pixel if the SWIR band is available (S2 + L5/L7)
                    && (pIPSNWSubPix == 0))
            {
                // Check saturated pixels and reflectance thresholds band per band
                bool isSaturatedPixel = false;
                bool isReflThresholdOK = false;

                for (unsigned int i = 0; i < lNbOfBands; i++)
                {
                    const unsigned int lcurBand = pAOTbands[i];
                    if (pIPSATPix[lcurBand] != static_cast<InputVectorMaskInternalPixelType>(0))
                    {
                        isSaturatedPixel = true;
                    }

                    // LAIG-DM-MAC-751-CNES : new filtering of pixels
                    const InternalPixelType lrTOA_AOT = pIPTOACSubPix[lcurBand];

                    // LAIG-FA-MAJA-2157-CNES : AOT Multi temporel : erreur d'index dans les tableaux LowReflectanceThresholds et HighReflectanceThresholds
                    // Pixels are selected if the condition is true for only one of the MT_AOT_bands
                    if ((lrTOA_AOT <= pLowReflectanceThresholds[i]) || (lrTOA_AOT >= pHighReflectanceThresholds[i]))
                    {
                        isReflThresholdOK = true;
                    }
                }

                // Check Age
                const double lDifferenceDate = pJDay - static_cast<double>(pL2inPXDPix);
                const bool isAgeThresholdOK = lDifferenceDate < pMaxAge;

                // Discard all saturated or old pixels for AOTUsedBands
                if ((isSaturatedPixel == false) && (isReflThresholdOK == true) && (isAgeThresholdOK == true))
                {
                    // Discard pixels for which the reflectance in the "var_band" varies too much between the date D and D-1
                    const InternalPixelType lrTOA_D_VAR = pIPTOACSubPix[pVARBandD];
                    const InternalPixelType lrTOA_D1_VAR = pL2inRTAPix[pVARBandDm1];

                    if ((lrTOA_D_VAR + lrTOA_D1_VAR) != 0)
                    {
                        if (vcl_abs((lrTOA_D_VAR - lrTOA_D1_VAR) / (lrTOA_D_VAR + lrTOA_D1_VAR)) <= pRelativeVarMax)
                        {
                            pNeighborhoodNotEmpty = true;
                            pDates.push_back(pL2inPXDPix);
                            pValidOffsets.push_back(pOffset);
                            pValidIPTOAC.push_back(pIPTOACSubPix);
                            pValidL2inRTA.push_back(pL2inRTAPix);
                        }
                    }
                } // end saturated pixels
            } // end super if
        }

        //
        //******************* Computation of the most frequent date in the list *******************
        //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate>
        unsigned int
        MultiTemporalAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask, TInputDate>::ComputeGreaterFrequencyDate(
                const DateListType & pDateList)
        {
            // Check size validity
            if (pDateList.empty())
            {
                vnsStaticExceptionBusinessMacro("MultiTemporalAOTMethodFunctions::ComputeGreaterFrequencyDate : the list is empty.");
            }
            // Find the greater frequent date
            // Sort the list of dates to iterate
            DateListType lSortedDates = pDateList;
            lSortedDates.sort();
            //lSortedDates.reverse();

            // Compute frequency
            typename DateListType::const_reverse_iterator it = lSortedDates.rbegin();
            DateType lPrevDate = (*it);
            // LPD = Local Previous Date is the most frequent date in the neighborhood (listed in lSortedDates)
            DateType lLPD = lPrevDate;
            unsigned int lFreq = 1;
            unsigned int lBestFreq = 1;

            // Dates loop
            // MACCS 5.0.0 - change loop
            // element, fix it with:
            // NOTE: Spec algo : if two dates are equally frequent,  LPD is the date closest to D
            for (++it; it != lSortedDates.rend(); ++it)
            {
                // For a new date
                if ((*it) != lPrevDate)
                {
                    // If this new date is more frequent than the previous one => new LPD
                    lPrevDate = (*it);
                    lFreq = 1;
                }
                // The date has already been met => increase the frequency
                else
                {
                    lFreq++;
                    // If this new date is more frequent than the previous one => new LPD
                    if (lFreq > lBestFreq)
                    {
                        lBestFreq = lFreq;
                        lLPD = lPrevDate;
                    }
                }
            }
            return static_cast<unsigned int>(lLPD);
        }

    //
    //******************* Extract the values of the vector associated to the LPD date *******************
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate>
        template<typename TAnyVectType>
            TAnyVectType
            MultiTemporalAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask, TInputDate>::ExtractMoreFreqValues(
                    const DateListType & pDateList, const TAnyVectType & pVect, const unsigned int pGreaterFreqDate)
            {

                // Check that the list of values and the list of dates are consistent
                const unsigned int lSize1 = pDateList.size();
                // Check size validity
                if (lSize1 == 0)
                {
                    vnsStaticExceptionBusinessMacro("MultiTemporalAOTMethodFunctions::ExtractMoreFreqValues : the list is empty.");
                }

                const unsigned int lSize2 = pVect.size();

                // Check size validity
                if (lSize1 != lSize2)
                {
                    vnsStaticExceptionBusinessMacro(
                            "MultiTemporalComputeAOTImageFilter::ExtractMoreFreqValues : lists does not have the same size.");
                }

                // Extract the values of the vector associated to the most frequent date
                TAnyVectType extractedVect;
                typename DateListType::const_iterator it = pDateList.begin();
                typename TAnyVectType::const_iterator it2 = pVect.begin();
                while (it2 != pVect.end())
                {
                    // The current date is equal to the LPD
                    if ((*it) == static_cast<int>(pGreaterFreqDate))
                    {
                        // Add the value to the output vector
                        extractedVect.push_back(*it2);
                    }
                    ++it;
                    it2++;
                }
                return extractedVect;
            }

    //
    //******************* Standard deviation computation on an input vector *******************
    //
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate>
        bool
        MultiTemporalAOTMethodFunctions<TInputImage, TInputMask, TInputVectorMask, TInputDate>::ComputeWeight(const int lGreaterFreq,
                const DateListType & pDates, const PixelVectType & pValidIPTOAC, const PixelVectType & pValidL2inRTA,
                const std::vector<unsigned int> & pAOTbandsD, const std::vector<unsigned int> & pAOTbandsDm1, const unsigned int pSTDEVBand,
                const double pMinStdDevThreshold, const double pStdNoise, double & pWeq1)
        {
            PixelVectType lExtractedIPTOACSTDEV;
            double lsumTOACSTDEV(0.);
            double lsumTot(0.);

            const unsigned int lSize1 = pDates.size();

            const unsigned int lSize2 = pValidIPTOAC.size();
            const unsigned int lSize3 = pValidL2inRTA.size();

            // Check size validity
            if (lSize1 == 0)
            {
                vnsStaticExceptionBusinessMacro("MultiTemporalAOTMethodFunctions::ComputeGreaterFrequencyDate : the list is empty.");
            }
            // Check size validity
            if ((lSize1 != lSize2) && (lSize1 != lSize3))
            {
                vnsStaticExceptionBusinessMacro("MultiTemporalAOTMethodFunctions::ComputeWeight : lists doesn't have the same size.");
            }

            // Init the iterators
            DoubleVectType extractedTOACSTDEV;
            typename DateListType::const_iterator itDate = pDates.begin();
            typename PixelVectType::const_iterator itTOAC = pValidIPTOAC.begin();
            typename PixelVectType::const_iterator itRTA = pValidL2inRTA.begin();
            typename DateListType::const_iterator itEnd = pDates.end();

            unsigned int count(0);
            unsigned int countSTDEV(0);
            // Init the number of bands
            const unsigned int lNbOfBands = pAOTbandsD.size();

            while ((itDate != itEnd) && (countSTDEV <= lSize1))
            {
                if ((*itDate) == static_cast<int>(lGreaterFreq))
                {
                    InputImagePixelType toac = (*itTOAC);
                    InputImagePixelType rta = (*itRTA);
                    double toacValSTDEV = toac[pSTDEVBand];

                    // Sum of toac in stdev band
                    lsumTOACSTDEV += toacValSTDEV;
                    ++countSTDEV;

                    extractedTOACSTDEV.push_back(toacValSTDEV);
                    // Sum the difference in AOTBands
                    for (unsigned int i = 0; i < lNbOfBands; i++)
                    {
                        const unsigned int id_d = pAOTbandsD[i];
                        const unsigned int id_dm1 = pAOTbandsDm1[i];
                        lsumTot += toac[id_d] - rta[id_dm1];

                        // Increment the counter
                        ++count;
                    }
                }
                ++itDate;
                ++itTOAC;
                ++itRTA;
            }

            const double invCountSTDEV = 1. / static_cast<double>(countSTDEV);
            const double invCount = 1. / static_cast<double>(count);
            // Compute the TOAC standard deviation
            double lmeanTOACSTDEV = lsumTOACSTDEV * invCountSTDEV;
            double lStdDevTOAC(0.);
            typename DoubleVectType::const_iterator itExtTOACSTDEV = extractedTOACSTDEV.begin();
            typename DoubleVectType::const_iterator itExtTOACSTDEVEnd = extractedTOACSTDEV.end();
            while (itExtTOACSTDEV != itExtTOACSTDEVEnd)
            {
                lStdDevTOAC += ((*itExtTOACSTDEV) - lmeanTOACSTDEV) * ((*itExtTOACSTDEV) - lmeanTOACSTDEV);
                itExtTOACSTDEV++;
            }

            lStdDevTOAC = std::sqrt((lStdDevTOAC / (countSTDEV - 1))); //vcl_pow((lStdDevTOAC / (countSTDEV-1)), 0.5);

            bool res = false;
            if (lStdDevTOAC > pMinStdDevThreshold)
            {
                pWeq1 = lsumTot * invCount;
                pWeq1 = vcl_abs(pWeq1 / pStdNoise);
                res = true;
            }

            return res;
        }

} // End namespace vns

#endif /* vnsMultiTemporalAOTMethodFunctions_txx */
