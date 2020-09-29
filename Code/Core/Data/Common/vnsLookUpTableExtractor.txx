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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 5 aout 2016 : Audit code - Correction du code             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 mars 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsLookUpTableExtractor_txx
#define __vnsLookUpTableExtractor_txx

#include "vnsLookUpTableExtractor.h"

namespace vns
{

    template<class TFullLUT, class TReducedLUT>
        typename TReducedLUT::Pointer
        LookUpTableExtractor<TFullLUT, TReducedLUT>::ExtractLUT(
                typename TFullLUT::Pointer FullLUT,
                const ExtractPointType & ReducedPoint)
        {
            typename TReducedLUT::Pointer ret = TReducedLUT::New();

            // Dimension of the init LUT
            unsigned int FullDimension = TFullLUT::LUTDimension;
            // Dimension of the "miniLUT" in which several dimensions were lost by setting
            // fixed values for those parameters
            unsigned int ReducedDimension = TReducedLUT::LUTDimension;
            unsigned int DimensionLost = FullDimension - ReducedDimension;

            // Set kept parameters values */
            typename TFullLUT::ParametersValuesType ParametersValues =
                    FullLUT->GetParametersValues();


			typename TReducedLUT::SizeType size;
			typename TReducedLUT::IndexType orig;
			unsigned int count = 0;

			// ----------------------------------------------------------------
			// For each kept dimension
			// ----------------------------------------------------------------
            for (unsigned int idim = DimensionLost; idim < FullDimension; idim++)
            {
				// Copy dimension size and origin
				size[count] = FullLUT->GetLargestPossibleRegion().GetSize()[idim];
				orig[count] = FullLUT->GetLargestPossibleRegion().GetIndex()[idim];
				count++;

				// Load the name of the parameter
                typename TFullLUT::ParameterValuesType values =
                        ParametersValues[idim];

                typename TReducedLUT::ParameterValuesType convertedValues;
                convertedValues.ParameterName = values.ParameterName;

                // Load the associated indices for each parameter (dimension) of the LUT
                const unsigned int NbValues = values.ParameterValues.size();
                for (unsigned int j = 0; j < NbValues; j++)
                {
                    convertedValues.ParameterValues.push_back(
                            values.ParameterValues[j]);
                }

                ret->AddParameterValues(convertedValues);
            }

            // ----------------------------------------------------------------
			// Allocate output image
            // ----------------------------------------------------------------
			typename TReducedLUT::RegionType region;
			region.SetSize(size);
			region.SetIndex(orig);

			ret->SetRegions(region);
			ret->Allocate();

            // Copy kept part of the buffer */
            typename TFullLUT::PointType interpPoint;

            // Convert physical values into float index,
            // as we do it only once... We save time after ! (instead of interpolating each point on physical values) */
            for (unsigned int i = 0; i < ExtractPointType::PointDimension; i++)
            {
                interpPoint[i] = FullLUT->FindIndexInParameterValues(i,
                        ReducedPoint[i]);
            }

            // Run over all the sub-LUT
            typename TReducedLUT::IteratorType iterator(ret, ret->GetLargestPossibleRegion());

            iterator.GoToBegin();
            while (iterator.IsAtEnd()==false)
            {
                // Get coordinates
                typename TReducedLUT::IndexType index = iterator.GetIndex();

                // Complete interpolation point in Full LUT
                for (unsigned int i = DimensionLost; i
                        < FullDimension; i++)
                {
                    interpPoint[i] = index[i - DimensionLost];
                }

                // Interpolate in full LUT and set value in reduced one
                iterator.Set(FullLUT->InterpolateIndex(interpPoint));

                ++iterator;
            }

            return ret;
        }

} // End namespace vns

#endif // __vnsLookUpTableExtractor_txx
