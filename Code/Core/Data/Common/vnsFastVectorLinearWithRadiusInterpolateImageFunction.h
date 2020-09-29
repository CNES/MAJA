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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 10 avril 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsFastVectorLinearWithRadiusInterpolateImageFunction.h 2406 2011-01-19 15:38:23Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsFastVectorLinearWithRadiusInterpolateImageFunction_h
#define __vnsFastVectorLinearWithRadiusInterpolateImageFunction_h

#include "vnsFastVectorInterpolateImageFunction.h"
#include "vnl/vnl_vector.h"

namespace vns
{

    /**
     * \class FastVectorLinearWithRadiusInterpolateImageFunction
     * \brief Linearly interpolate a vector image at specified positions.
     *
     * FastVectorLinearWithRadiusInterpolateImageFunction linearly interpolates a vector
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
        class FastVectorLinearWithRadiusInterpolateImageFunction : public FastVectorInterpolateImageFunction<
                TInputImage, TCoordRep>
        {
        public:
            /** Standard class typedefs. */
            typedef FastVectorLinearWithRadiusInterpolateImageFunction Self;
            typedef FastVectorInterpolateImageFunction<TInputImage, TCoordRep> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Run-time type information (and related methods). */
            itkTypeMacro(FastVectorLinearWithRadiusInterpolateImageFunction,
                    FastVectorInterpolateImageFunction)

            /** InputImageType typedef support. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename Superclass::PixelType PixelType;
            typedef typename Superclass::PixelType InputPixelType;
            typedef typename Superclass::ValueType ValueType;
            typedef typename Superclass::RealType RealType;
            typedef typename Superclass::PointType PointType;
            typedef typename Superclass::OutputType OutputType;
            typedef typename InputImageType::IndexValueType IndexValueType;

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

            /** Coeficients container type.*/
            typedef vnl_vector<double> CoefContainerType;

            /** Set/Get the window radius */
            virtual void SetRadius(unsigned int radius);
            virtual unsigned int GetRadius() const;

        protected:
            FastVectorLinearWithRadiusInterpolateImageFunction();
            virtual
            ~FastVectorLinearWithRadiusInterpolateImageFunction()
            {
            }
            void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Compute the BCO coefficients. */
            virtual CoefContainerType EvaluateCoef( const unsigned int & winsize );

              /** Used radius for the Linear */
            unsigned int           m_Radius;
            /** Used winsize for the Linear */
            unsigned int           m_WinSize;

            /** Used Linear coefficient */
            CoefContainerType       m_LinearCoef;

        private:
            FastVectorLinearWithRadiusInterpolateImageFunction(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented


        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsFastVectorLinearWithRadiusInterpolateImageFunction.txx"
#endif

#endif
