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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 20 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 mars 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef VNSLOOKUPTABLE_TXX_
#define VNSLOOKUPTABLE_TXX_

#include "vnsLookUpTable.h"
#include "vnsLoggers.h"
#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{

    template<class TValue, unsigned int VLUTDimension>
        LookUpTable<TValue, VLUTDimension>::LookUpTable()
        {
            // Interpolator creation
            SetInterpolator(DefaultInterpolatorType::New());
        }

    template<class TValue, unsigned int VLUTDimension>
        LookUpTable<TValue, VLUTDimension>::~LookUpTable()
        {
        }

    // Add parameter values
    template<class TValue, unsigned int VLUTDimension>
        void
        LookUpTable<TValue, VLUTDimension>::AddParameterValues(const ParameterValuesType &NewParamValues)
        {
            unsigned int lSize = m_ParametersValues.size();

            // If LUT already has the required number of params
            if (lSize == VLUTDimension)
            {
                // Error and Exit
                vnsExceptionDataMacro("Parameters values are all already defined")
            }

            // Store new parameter's name and values
            m_ParametersValues.push_back(NewParamValues);

            lSize = m_ParametersValues.size();
            //  If LUT now has the required number of params
            if (lSize == VLUTDimension)
            {
                // Allocate image
                this->AllocateLUT();
            }
        }

    // Get exact value
    template<class TValue, unsigned int VLUTDimension>
        TValue
        LookUpTable<TValue, VLUTDimension>::GetValue(const IndexType &lutIndex) const
        {
            // Forward to Image
            return this->GetPixel(lutIndex);
        }

    // Set value
    template<class TValue, unsigned int VLUTDimension>
        void
        LookUpTable<TValue, VLUTDimension>::SetValue(const IndexType &lutIndex, TValue value)
        {
            // Forward to Image
            this->SetPixel(lutIndex, value);
        }

    // Interpolate from physical point values
    template<class TValue, unsigned int VLUTDimension>
        TValue
        LookUpTable<TValue, VLUTDimension>::InterpolateValue(const PointType &PhysicalPoint) const
        {
            // TODO: use a scoped mutex the contention is much too important, this is bad for
            // performances
            // BTW, why a lock on const variables? Is it because the
            // interpolator has a state?
            m_Mutex.Lock();
            PointType imagePoint = ConvertPhysicalPointIntoImagePoint(PhysicalPoint);

            TValue ret = InterpolateIndex(imagePoint);
            m_Mutex.Unlock();
            return ret;
        }

    // Interpolate from physical point values
    template<class TValue, unsigned int VLUTDimension>
        TValue
        LookUpTable<TValue, VLUTDimension>::InterpolateIndex(const PointType &IndexPoint) const
        {
            // 4.2 TODO Check test LUT tempo
            if (m_Interpolator->IsInsideBuffer(IndexPoint) == false)
            {
                vnsLogDebugMacro("ATTENTION: the following index point is outside of LUT: " << IndexPoint);
            }
            try
            {
                return static_cast<TValue>(m_Interpolator->Evaluate(IndexPoint));
            }
            catch (...)
            {
                // TODO: if the catch is really redundant with the test at the
                // start of the function as the message implies, then, don't
                // do a try-catch here, and simplify the code!
                vnsExceptionDataMacro("ERROR : catch exception during interpolate point outside of LUT : " << IndexPoint);
            }
        }
    // Convert physical point into image point (index)
    template<class TValue, unsigned int VLUTDimension>
        typename LookUpTable<TValue, VLUTDimension>::PointType
        LookUpTable<TValue, VLUTDimension>::ConvertPhysicalPointIntoImagePoint(
                const LookUpTable<TValue, VLUTDimension>::PointType &realPoint) const
        {
            PointType imagePoint;
            imagePoint.Fill((PointValueType) 0);

            for (unsigned int dim = 0; dim < VLUTDimension; dim++)
            {
                imagePoint[dim] = FindIndexInParameterValues(dim, realPoint[dim]);
            }
            return imagePoint;
        }

    template<class TValue, unsigned int VLUTDimension>
        TValue
        LookUpTable<TValue, VLUTDimension>::FindIndexInParameterValues(unsigned int ParameterIndex, TValue ParameterValue) const
        {
            // Get the parameter values for the considered parameter index
            std::vector<TValue> const& valuesfordim = m_ParametersValues[ParameterIndex].ParameterValues;

            unsigned int index = 0;
            const unsigned int size = valuesfordim.size();

            // Get the index associated to the value
            for ( ; index < size - 1 ; ++index) 
            {
                if (valuesfordim[index] >= ParameterValue)
                {
                    break;
                }
            }

            // If value is outside of bounds, push a warning
            if (index == 0)
            {
                return 0;
            }
            // The value is greater than the max index value => Interpolation is an extrapolation
            if ((index == (size - 1)) && (ParameterValue > valuesfordim[index]))
            {
                return index;
            }

            // Check that the indices that surround the value are different
            TValue v2 = valuesfordim[index];
            --index ;
            const TValue v1 = valuesfordim[index];

            const TValue diff = static_cast<TValue>(v2 - v1);

            if (vnsEqualsDoubleMacro(diff,static_cast<TValue>(0)) == true)
            {
                vnsLogWarningMacro("Warning : Two consecutive indexes of the LUT are identical => return index 0 !!");
                return 0;
            }

            // Compute the final index
            const TValue k = (ParameterValue - v1) / diff;

            return index + k;
        }

    // Allocate Image
    template<class TValue, unsigned int VLUTDimension>
        void
        LookUpTable<TValue, VLUTDimension>::AllocateLUT()
        {
            vnsLogInfoMacro("Allocating LookUpTable image dimension = " << (int)(VLUTDimension))

            // Set Image Region
            typename Superclass::IndexType start;
            typename Superclass::SizeType size;

            for (unsigned int i = 0; i < VLUTDimension; i++)
            {
                start[i] = 0;
                size[i] = m_ParametersValues[i].ParameterValues.size();
            }
            typename Superclass::RegionType region;

            region.SetSize(size);
            region.SetIndex(start);

            this->SetRegions(region);

            // Set image spacing to 1.0
            typename Superclass::SpacingType spacing;
            spacing.Fill(1.0);
            this->SetSpacing(spacing);

            // Set image origin
            typename Superclass::PointType origin;
            origin.Fill(0.);
            this->SetOrigin(origin);

            // Allocate Image
            this->Allocate();

            // Update interpolator
            m_Interpolator->SetInputImage(this);
        }

} // End namespace vns

#endif
