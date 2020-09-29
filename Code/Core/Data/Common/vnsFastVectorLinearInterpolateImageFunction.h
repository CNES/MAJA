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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsFastVectorLinearInterpolateImageFunction.h 2406 2011-01-19 15:38:23Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsFastVectorLinearInterpolateImageFunction_h
#define __vnsFastVectorLinearInterpolateImageFunction_h

#include "vnsFastVectorInterpolateImageFunction.h"

namespace vns
{

    /**
     * \class FastVectorLinearInterpolateImageFunction
     * \brief Linearly interpolate a vector image at specified positions.
     *
     * FastVectorLinearInterpolateImageFunction linearly interpolates a vector
     * image intensity non-integer pixel position. This class is templated
     * over the input image type and the coordinate representation type.
     *
     * This function works for N-dimensional images.
     *
     * \warning This function work only for Vector images. For
     * scalar images use LinearInterpolateImageFunction.
     *
     * \ingroup ImageFunctions ImageInterpolators
     *
     */
    template<class TInputImage, class TCoordRep = double>
        class FastVectorLinearInterpolateImageFunction : public FastVectorInterpolateImageFunction<
                TInputImage, TCoordRep>
        {
        public:
            /** Standard class typedefs. */
            typedef FastVectorLinearInterpolateImageFunction Self;
            typedef FastVectorInterpolateImageFunction<TInputImage, TCoordRep> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Run-time type information (and related methods). */
            itkTypeMacro(FastVectorLinearInterpolateImageFunction,
                    FastVectorInterpolateImageFunction)

            /** InputImageType typedef support. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename Superclass::PixelType PixelType;
            typedef typename Superclass::ValueType ValueType;
            typedef typename Superclass::RealType RealType;
            typedef typename Superclass::PointType PointType;
            typedef typename Superclass::OutputType OutputType;

            /** Dimension underlying input image. */
            itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);

            /** Index typedef support. */
            typedef typename Superclass::IndexType IndexType;

            /** ContinuousIndex typedef support. */
            typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

            /** Evaluate the function at a ContinuousIndex position
             *
             * Returns the linearly interpolated image intensity at a
             * specified point position. No bounds checking is done.
             * The point is assume to lie within the image buffer.
             *
             * ImageFunction::IsInsideBuffer() can be used to check bounds before
             * calling the method. */
            /** Returns the interpolated image intensity at a
             * specified point position. No bounds checking is done.
             * The point is assume to lie within the image buffer.
             * ImageFunction::IsInsideBuffer() can be used to check bounds before
             * calling the method. */
            OutputType
            Evaluate(const PointType& point) const
            {
                // ---------------------------------------------------------------------------------------------
                ContinuousIndexType index;
                // ---------------------------------------------------------------------------------------------
                this->GetInputImage()->TransformPhysicalPointToContinuousIndex(point, index);
                return (this->EvaluateAtContinuousIndex(index));
            }

            OutputType
            EvaluateAtContinuousIndex(const ContinuousIndexType & index) const;

        protected:
            FastVectorLinearInterpolateImageFunction();
            virtual
            ~FastVectorLinearInterpolateImageFunction()
            {
            }
            void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            FastVectorLinearInterpolateImageFunction(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Number of neighbors used in the interpolation */
            static const unsigned long m_Neighbors;

        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsFastVectorLinearInterpolateImageFunction.txx"
#endif

#endif
