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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 21 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 mars 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef VNSLOOKUPTABLE_H_
#define VNSLOOKUPTABLE_H_

#include "otbImage.h"
#include "itkInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include <vector>
#include "itkPoint.h"
#include "itkImageRegionIterator.h"
#include "itkSimpleFastMutexLock.h"

namespace vns
{
    /** \class  LookUpTable
     * \brief This class implements the Look-Up Tables operations.
     *
     *  This class is used to deal with Look-Up tables. It allows interpolation either
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
        class LookUpTable : public otb::Image<TValue, VLUTDimension>
        {

        public:

            /** Types definition for ITK macros */
            typedef LookUpTable<TValue, VLUTDimension> Self;
            typedef otb::Image<TValue, VLUTDimension> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Run-time type information (and related methods). */
            itkTypeMacro( LookUpTable, otb::Image );
            itkNewMacro (Self);

            /** Types definition for values */
            typedef TValue ValueType;itkStaticConstMacro(LUTDimension, unsigned int, VLUTDimension);

            typedef typename Superclass::IndexType IndexType;
            typedef typename Superclass::IndexValueType IndexValueType;
            typedef typename Superclass::PointType PointType;
            typedef typename Superclass::ValueType PointValueType;

            typedef itk::InterpolateImageFunction<Superclass, TValue> InterpolatorType;

            typedef itk::LinearInterpolateImageFunction<Superclass, TValue> DefaultInterpolatorType;

            typedef itk::ImageRegionIterator<Superclass> IteratorType;

            class ParameterValuesType
            {
            public:
                std::string ParameterName;
                std::vector<TValue> ParameterValues;
            };

            typedef std::vector<ParameterValuesType> ParametersValuesType;

            /** Getter and Setter for Parameters values */
            //itkGetVectorMacro(ParametersValues, ParametersValuesType,0)
            //itkSetVectorMacro(ParametersValues, ParametersValuesType)
            ParametersValuesType
            GetParametersValues() const
            {
                return m_ParametersValues;
            }

            void
            AddParameterValues(const ParameterValuesType &NewParamValues);

            /** Get exact value */
            TValue
            GetValue(const IndexType &lutIndex) const;

            /** Set value */
            void
            SetValue(const IndexType &lutIndex, TValue value);

            /** Interpolate from physical point values */
            TValue
            InterpolateValue(const PointType &PhysicalPoint) const;

            /** Interpolate from physical point values */
            TValue
            InterpolateIndex(const PointType &IndexPoint) const;

            /** Find real index from Value */
            TValue
            FindIndexInParameterValues(unsigned int ParameterIndex, TValue ParameterValue) const;

        protected:
            /** Constructor */
            LookUpTable();

            /** Destructor */
            virtual
            ~LookUpTable();

            /** Setter for Interpolator instance (Interpolator is itk::LinearInterpolateImageFunction by default) */
            itkSetObjectMacro(Interpolator, InterpolatorType)

            /** Convert physical point into image point (index) */
            PointType ConvertPhysicalPointIntoImagePoint(const PointType &realPoint) const;

        private:
            /** Allocate image when all variables values are set */
            void
            AllocateLUT();

            /** Interpolator on image */
            typename InterpolatorType::Pointer m_Interpolator;

            /** Parameters names and values */
            ParametersValuesType m_ParametersValues;

            /** Mutex for interpolation synchronisation */
            itk::SimpleFastMutexLock m_Mutex;
        };

    typedef LookUpTable<double, 1> VNSLUTMonoBand1DType;
    typedef LookUpTable<double, 3> VNSLUTMonoBand3DType;

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsLookUpTable.txx"
#endif

#endif /* VNSLOOKUPTABLE_H_ */
