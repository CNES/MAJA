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
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction données des CAMS             *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 20 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 mars 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef VNSVECTORLOOKUPTABLE_TXX_
#define VNSVECTORLOOKUPTABLE_TXX_

#include "vnsVectorLookUpTable.h"
#include "vnsLoggers.h"
#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{

    template<class TValue, unsigned int VLUTDimension>
    VectorLookUpTable<TValue, VLUTDimension>::VectorLookUpTable()
    {
        // Interpolator creation
        SetInterpolator(DefaultInterpolatorType::New());
        this->SetNumberOfComponentsPerPixel(1);
    }

    template<class TValue, unsigned int VLUTDimension>
    VectorLookUpTable<TValue, VLUTDimension>::~VectorLookUpTable()
    {

    }

    // Add parameter values
    template<class TValue, unsigned int VLUTDimension>
    void
    VectorLookUpTable<TValue, VLUTDimension>::AddParameterValues(const ParameterValuesType &NewParamValues)
    {
        unsigned int lSize = m_ParametersValues.size();

        // If LUT already has the required number of params
        if (lSize == VLUTDimension)
        {
            // Error and Exit
            itkExceptionMacro("Parameters values are all already defined")
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
    typename VectorLookUpTable<TValue, VLUTDimension>::PixelType
    VectorLookUpTable<TValue, VLUTDimension>::GetValue(const IndexType &lutIndex) const
    {
        // Forward to Image
        return this->GetPixel(lutIndex);
    }

    // Get exact value for only one band
    template<class TValue, unsigned int VLUTDimension>
    typename VectorLookUpTable<TValue, VLUTDimension>::InternalPixelType
    VectorLookUpTable<TValue, VLUTDimension>::GetValue(const IndexType &lutIndex, unsigned int pBandNum) const
    {
        const PixelType lPix = this->GetPixel(lutIndex);
        return lPix[pBandNum];
    }

    // Set value
    template<class TValue, unsigned int VLUTDimension>
    void
    VectorLookUpTable<TValue, VLUTDimension>::SetValue(const IndexType &lutIndex, PixelType pixel)
    {
        // Forward to VectorImage
        this->SetPixel(lutIndex, pixel);
    }

    // Set value for only one band
    template<class TValue, unsigned int VLUTDimension>
    void
    VectorLookUpTable<TValue, VLUTDimension>::SetValue(const IndexType &lutIndex, unsigned int pBandNum, InternalPixelType pVal)
    {
        PixelType lPix = this->GetPixel(lutIndex);
        lPix[pBandNum] = pVal;
        this->SetPixel(lutIndex, lPix);
    }

    // Interpolate from physical point values
    template<class TValue, unsigned int VLUTDimension>
    typename VectorLookUpTable<TValue, VLUTDimension>::PixelType
    VectorLookUpTable<TValue, VLUTDimension>::InterpolateValue(const PointType &PhysicalPoint) const
    {
        return InterpolateIndex(ConvertPhysicalPointIntoImagePoint(PhysicalPoint));
    }

    // Interpolate from physical point values
    template<class TValue, unsigned int VLUTDimension>
    typename VectorLookUpTable<TValue, VLUTDimension>::PixelType
    VectorLookUpTable<TValue, VLUTDimension>::InterpolateIndex(const PointType &IndexPoint) const
    {
        // 4.2 TODO Check test LUT tempo
        if (m_Interpolator->IsInsideBuffer(IndexPoint) == false)
        {
            vnsLogSuperDebugMacro("ATTENTION: the following index point is outside of LUT: " << IndexPoint);
        }
        try
        {
            // Store interpolate value
            const OutputType interpVal = m_Interpolator->Evaluate(IndexPoint);
            // TODO optimization: check if template OutputType::PixelType == InternalPixelType, so return interpVal; else do actual code

            // Cast the interpolate value that is in RealType
            // into the LUT type
            const unsigned int l_Size(interpVal.Size());
            unsigned int l(0);
            PixelType outVal;
            outVal.SetSize(l_Size);
            outVal.Fill(0);
            for (l = 0; l < l_Size; l++)
            {
                outVal[l] = static_cast<InternalPixelType>(interpVal[l]);
            }

            return outVal;
        }
        catch (...)
        {
            vnsExceptionDataMacro(
                    "ERROR : point outside of LUT : " << IndexPoint << "\nRegion index: Start: " << m_Interpolator->GetStartIndex() << "; End: " << m_Interpolator->GetEndIndex() << "\nRegion Continuous index: Start: " << m_Interpolator->GetStartContinuousIndex() << "; End: " << m_Interpolator->GetEndContinuousIndex() << "\n m_Origin: "<< this->m_Origin << "\n m_Spacing: "<< this->m_Spacing << "\n m_LargestPossibleRegion: "<< this->GetLargestPossibleRegion())
        }

    }

    // Convert physical point into image point (index)
    template<class TValue, unsigned int VLUTDimension>
    typename VectorLookUpTable<TValue, VLUTDimension>::PointType
    VectorLookUpTable<TValue, VLUTDimension>::ConvertPhysicalPointIntoImagePoint(
            const VectorLookUpTable<TValue, VLUTDimension>::PointType &realPoint) const
            {
        PointType imagePoint;
        for (unsigned int dim = 0; dim < VLUTDimension; dim++)
        {
            imagePoint[dim] = FindIndexInParameterValues(dim, realPoint[dim]);
        }
        return imagePoint;
            }

    template<class TValue, unsigned int VLUTDimension>
    TValue
    VectorLookUpTable<TValue, VLUTDimension>::FindIndexInParameterValues(unsigned int ParameterIndex, TValue ParameterValue) const
    {

        // Get the parameter values for the considered parameter index
        const std::vector<TValue> valuesfordim = m_ParametersValues[ParameterIndex].ParameterValues;

        const unsigned int lSize_moins_1 = valuesfordim.size() - 1;
        unsigned int index = 0;

        bool lValueIsInside = true;
        while (lValueIsInside && (index < lSize_moins_1))
        {
            if (valuesfordim[index] >= ParameterValue)
            {
                lValueIsInside = false;
            }
            else
            {
                index = index + 1;
            }
        }

        // If value is outside of bounds, push a warning
        if (index == 0)
        {
            return 0;
        }

        if ((index == lSize_moins_1) && (ParameterValue > valuesfordim[index]))
        {
            return index;
        }

        // Check that the indices that surround the value are different
        const TValue v2 = valuesfordim[index];
        index = index - 1;
        const TValue v1 = valuesfordim[index];

        const TValue diff = static_cast<TValue>(v2 - v1);

        if (vnsEqualsDoubleMacro(diff,static_cast<TValue>(0)) == true)
        {
            vnsLogWarningMacro("Warning : Two consecutive indexes of the LUT are identical => return index 0 !!");
            return 0;
        }

        const TValue k = (ParameterValue - v1) / diff;

        return (index + k);

    }

    // Allocate Image
    template<class TValue, unsigned int VLUTDimension>
    void
    VectorLookUpTable<TValue, VLUTDimension>::AllocateLUT()
    {
        vnsLogDebugMacro("Allocating VectorLookUpTable image dimension = " << (int)(VLUTDimension))

        // Set Image Region
                    typename Superclass::IndexType start;
        typename Superclass::SizeType size;

        if (m_ParametersValues.size() != VLUTDimension)
        {
            itkExceptionMacro(
                    "AllocateLUT: Internal error: The size m_ParametersValues ("<<m_ParametersValues.size()<<") is different than the dimension of the LUT ("<<VLUTDimension<<") !")
        }

        for (unsigned int i = 0; i < VLUTDimension; i++)
        {
            start[i] = 0;
            size[i] = m_ParametersValues.at(i).ParameterValues.size();
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

        // TODO: Mem: free LUT allocated!
        // Allocate Image
        this->Allocate();

        // Update interpolator
        m_Interpolator->SetInputImage(this);

    }

    // Invert LUT : return false if the result is not found
    template<class TValue, unsigned int VLUTDimension>
    bool
    VectorLookUpTable<TValue, VLUTDimension>::InvertLUT(const PointType & pFixedLutIndex, unsigned int pNumBand,
            unsigned int pTargetedDim, TValue pVal, double & pResult) const
            {

        // Initialization
        ParameterValues lParamVal;
        PointType lVariableLutIndex;
        PixelType lPix;
        InternalPixelType lVal1(0);
        InternalPixelType lVal2(0);
        InternalPixelType lPoids(1);
        unsigned int lIndex1(0);
        unsigned int lIndex2(1);
        unsigned int i(0);
        bool lFound(false);
        pResult = 0.;

        // Copy FixedLutIndex into VariableLutIndex, two dimensions are fixed,
        // the third is variable
        for (i = 0; i < VLUTDimension; i++)
        {
            lVariableLutIndex[i] = pFixedLutIndex[i];
        }

        // Get the parameter values and its size
        lParamVal = this->GetParameterValues(pTargetedDim);
        std::vector<TValue> lVectorOfVal = lParamVal.ParameterValues;
        const unsigned int lSize = lVectorOfVal.size();

        lVariableLutIndex[pTargetedDim] = lIndex1;
        lPix = this->InterpolateIndex(lVariableLutIndex);
        lVal2 = lPix[pNumBand];

        // lVal2 = dark surface reflectance mimimum associated to the AOT index min
        // Test if the dark surface reflectance associated to the value of AOT index min is lower than the
        // dark surface reflectance threshold
        if (lVal2 < pVal)
        {
            vnsLogWarningMacro(
                    "Warning : The dark surface reflectance associated to the value of AOT index min is" <<" lower than the dark surface reflectance threshold. AOT dark is set to the AOT index min : " <<pResult<<".");

            this->FindIndexInParameterValues(pTargetedDim, pResult);
            return true;
        }

        // ------------------------------------------------------------------------------------------
        // Search the two indices in the parameters of the LUT that surround the fixed output value
        // of the LUT (surface reflectance)
        // ------------------------------------------------------------------------------------------

        // Loop on the Targeted (variable) Dimension Value
        while ((lFound == false) && (lIndex2 < lSize))
        {
            lVal1 = lVal2;

            // Get the interpolated value
            lVariableLutIndex[pTargetedDim] = lIndex2;
            lPix = this->InterpolateIndex(lVariableLutIndex);
            lVal2 = lPix[pNumBand];

            // Check if the value is include in the interval
            if (((lVal1 <= pVal) && (pVal <= lVal2)) || ((lVal1 >= pVal) && (pVal >= lVal2)))
            {
                lFound = true;
            }
            else
            {
                lIndex1++;
                lIndex2++;
            }
        }

        // If value is outside of bounds, push a warning
        if (lFound == false)
        {
            vnsLogWarningMacro("Warning : Interpolation is an extrapolation, value out of bound : no result found");
        }
        else
        {
            // Compute the weight for the interpolation
            if ((lVal1 < lVal2) && (lVal1 != lVal2))
            {
                lPoids = 1 - (pVal - lVal1) / (lVal2 - lVal1);
            }
            else
            {
                lPoids = 1 - (pVal - lVal2) / (lVal1 - lVal2);
            }

            // Linear interpolation between the two found indices
            pResult = lVectorOfVal.at(lIndex1) + lPoids * (lVectorOfVal.at(lIndex2) - lVectorOfVal.at(lIndex1));

            // Shows the results in debug mode
            vnsLogDebugMacro("Indexes: [" << lIndex1 << ";" << lIndex2 << "]");
            vnsLogDebugMacro("Val1   : " << lVal1);
            vnsLogDebugMacro("pVal   : " << pVal);
            vnsLogDebugMacro("Val2   : " << lVal2);
            vnsLogDebugMacro("Poids  : " << lPoids);
            vnsLogDebugMacro("Result : " << pResult);

        }
        return lFound;

            }
    template<class TValue, unsigned int VLUTDimension>
    void VectorLookUpTable<TValue, VLUTDimension>::copyParametersValue(const VectorLookUpTable<TValue, VLUTDimension>::Pointer& p_other)
    {
        // Set kept parameters values */
        ParametersValuesType ParametersValues = p_other->GetParametersValues();
        if (ParametersValues.size() == 0)
        {
            vnsExceptionAlgorithmsProcessingMacro("Input Lut has no parameters value to copy")
        }
        // ----------------------------------------------------------------
        // For each kept dimension
        // ----------------------------------------------------------------
        SizeType size;
        IndexType orig;
        unsigned int count = 0;
        for (unsigned int idim = 0; idim < LUTDimension; idim++)
        {
            // Copy dimension size and origin
            size[count] =  p_other->GetLargestPossibleRegion().GetSize()[idim];
            orig[count] = p_other->GetLargestPossibleRegion().GetIndex()[idim];
            count++;

            // Load the name of the parameter
            ParameterValuesType values = ParametersValues[idim];

            ParameterValuesType convertedValues;
            convertedValues.ParameterName = values.ParameterName;

            // Load the associated indices for each parameter (dimension) of the LUT
            unsigned int NbValues = values.ParameterValues.size();
            for (unsigned int j = 0; j < NbValues; j++)
            {
                convertedValues.ParameterValues.push_back(values.ParameterValues[j]);
            }
            this->AddParameterValues(convertedValues);
        }
    }

} // End namespace vns

#endif
