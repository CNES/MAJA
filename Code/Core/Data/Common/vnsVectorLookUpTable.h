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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 14 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0-3 : DM : 357 : 20 juil. 2011 : Correction AOT estimation.                                  *
 * 												Ajout d'un type dans la classe ParameterValue.              *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 mars 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef VNSVECTORLOOKUPTABLE_H_
#define VNSVECTORLOOKUPTABLE_H_

#include "otbVectorImage.h"
#include "vnsVectorLinearInterpolateImageFunction.h"
#include "itkPoint.h"
#include "itkImageRegionIterator.h"
#include "itkSimpleFastMutexLock.h"

namespace vns
{
    /** \class  VectorLookUpTable
     * \brief This class implements the multi-bands Look-Up Tables operations.
     *
     *  This class is used to deal with multi-bands Look-Up tables. It allows interpolation either
     *  in floating index values, or in real (physical) values.
     *
     * \sa LookUpTableExtractor
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
    template<class TValue, unsigned int VLUTDimension = 2>
    class VectorLookUpTable : public otb::VectorImage<TValue, VLUTDimension>
    {

    public:

        /** Types definition for ITK macros */
        typedef VectorLookUpTable    Self;
        typedef otb::VectorImage<TValue, VLUTDimension>     Superclass;
        typedef itk::SmartPointer<Self>                     Pointer;
        typedef itk::SmartPointer<const Self>               ConstPointer;

        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro( VectorImage, otb::VectorImage );

        /** Types definition for values */
        typedef typename Superclass::ValueType ValueType;

        itkStaticConstMacro(LUTDimension, unsigned int, VLUTDimension);

        typedef typename Superclass::IndexType              IndexType;
        typedef typename Superclass::PointType              PointType;
        typedef typename Superclass::SizeType               SizeType;
        typedef typename Superclass::PixelType              PixelType;
        typedef typename Superclass::InternalPixelType      InternalPixelType;
        typedef typename Superclass::IOPixelType            IOPixelType;

        typedef VectorInterpolateImageFunction<Superclass, TValue >  VectorInterpolatorType;
        typedef typename VectorInterpolatorType::OutputType OutputType;
        typedef VectorLinearInterpolateImageFunction
        <Superclass, TValue >                               DefaultInterpolatorType;
    
        typedef itk::ImageRegionIterator<Superclass> IteratorType;

        typedef class ParameterValues
        {
        public:
            typedef std::vector<TValue> ParamValuesType;
            std::string ParameterName;
            ParamValuesType ParameterValues;
        } ParameterValuesType;

        typedef std::vector<ParameterValuesType> ParametersValuesType;

        /** Getter and Setter for Parameters values */
        ParametersValuesType GetParametersValues () const
        {
            return m_ParametersValues;
        }

        ParameterValuesType GetParameterValues (unsigned int ind) const
        {
            return m_ParametersValues.at(ind);
        }

        void AddParameterValues(const ParameterValuesType &NewParamValues);

        /** Get exact value */
        PixelType GetValue(const IndexType &lutIndex) const;
        InternalPixelType GetValue(const IndexType &lutIndex, unsigned int pBandNum) const;

        /** Set value */
        void SetValue(const IndexType &lutIndex, PixelType pixel);
        void SetValue(const IndexType &lutIndex, unsigned int pBandNum, InternalPixelType pVal);

        /** Interpolate from physical point values */
        PixelType InterpolateValue(const PointType &PhysicalPoint) const;

        /** Interpolate from physical point values */
        PixelType InterpolateIndex(const PointType &IndexPoint) const;

        /** Find real index from Value */
        TValue FindIndexInParameterValues(unsigned int ParameterIndex, TValue ParameterValue) const;

        /** Invert LUT : Find parameter value from LUT value */
        bool InvertLUT(const PointType & pFixedLutIndex,unsigned int pNumBand, unsigned int pTargetedDim,TValue pVal, double & pResult) const;

        /** Copy the parameters of the input vectorLUT */
        void copyParametersValue(const VectorLookUpTable<TValue, VLUTDimension>::Pointer & p_other);

    protected:
        /** Constructor */
        VectorLookUpTable();

        /** Destructor */
        virtual ~VectorLookUpTable();

        /** Setter for Interpolator instance (Interpolator is itk::VectorLinearInterpolateImageFunction by default) */
        itkSetObjectMacro(Interpolator, VectorInterpolatorType)

        /** Convert physical point into image point (index) */
        PointType ConvertPhysicalPointIntoImagePoint(const PointType &realPoint) const;

    private:
        /** Allocate image when all variables values are set */
        void AllocateLUT();

        /** Interpolator on image */
        typename VectorInterpolatorType::Pointer m_Interpolator;

        /** Parameters names and values */
        ParametersValuesType m_ParametersValues;

    };

    typedef VectorLookUpTable<double, 6> VNSLUT6DType;
    typedef VectorLookUpTable<double, 3> VNSLUT3DType;
    typedef VectorLookUpTable<double, 2> VNSLUT2DType;

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsVectorLookUpTable.txx"
#endif

#endif /* VNSVECTORLOOKUPTABLE_H_ */
