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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 10 avril 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsVectorInterpolateImageFunction_h
#define __vnsVectorInterpolateImageFunction_h

#include "itkImageFunction.h"
#include "itkFixedArray.h"

namespace vns
{

    /**
     * Due to a bug in MSVC, an enum value cannot be accessed out of a template
     * parameter until the template class opens.  In order for templated classes
     * to access the dimension of a template parameter in defining their
     * own dimension, this class is needed as a work-around.
     */
    template<typename T>
        struct GetDimension
        {
            itkStaticConstMacro(Dimension, int, T::Dimension);
        };

    /** \class VectorInterpolateImageFunction
     * \brief Base class for all vector image interpolaters.
     *
     * VectorInterpolateImageFunction is the base for all ImageFunctions that
     * interpolates image with vector pixel types. This function outputs
     * a return value of type Vector<double,Dimension>.
     *
     * This class is templated input image type and the coordinate
     * representation type.
     *
     * \warning This hierarchy of functions work only for images
     * with Vector-based pixel types. For scalar images use
     * InterpolateImageFunction.
     *
     * \sa InterpolateImageFunction
     * \ingroup ImageFunctions ImageInterpolators
     */
    template<class TInputImage, class TCoordRep = double>
        class VectorInterpolateImageFunction : public itk::ImageFunction<TInputImage, typename /*ITK_TYPENAME*/ itk::NumericTraits<
                typename TInputImage::PixelType>::RealType, TCoordRep>
        {
        public:

            /** Dimension underlying input image. */
            itkStaticConstMacro(ImageDimension, unsigned int,
                    TInputImage::ImageDimension);

            /** Standard class typedefs. */
            typedef VectorInterpolateImageFunction Self;
            typedef itk::ImageFunction<TInputImage, typename /*ITK_TYPENAME*/ itk::NumericTraits<typename TInputImage::PixelType>::RealType, TCoordRep>
                    Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Run-time type information (and related methods). */
            itkTypeMacro(VectorInterpolateImageFunction, ImageFunction)

            /** InputImageType typedef support. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::PixelType PixelType;
            typedef typename PixelType::ValueType ValueType;
            typedef typename itk::NumericTraits<ValueType>::RealType RealType;

            /** Point typedef support. */
            typedef typename Superclass::PointType PointType;

            /** Index typedef support. */
            typedef typename Superclass::IndexType IndexType;

            /** Index value type support */
            typedef typename Superclass::IndexValueType IndexValueType;

            /** ContinuousIndex typedef support. */
            typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

            /** Output type is RealType of TInputImage::PixelType. */
            typedef typename Superclass::OutputType OutputType;

            /** CoordRep typedef support. */
            typedef TCoordRep CoordRepType;

            /** Returns the interpolated image intensity at a
             * specified point position. No bounds checking is done.
             * The point is assume to lie within the image buffer.
             * ImageFunction::IsInsideBuffer() can be used to check bounds before
             * calling the method. */
            virtual OutputType
            Evaluate(const PointType& point) const
            {
                // ---------------------------------------------------------------------------------------------
                ContinuousIndexType index;
                // ---------------------------------------------------------------------------------------------
                this->GetInputImage()->TransformPhysicalPointToContinuousIndex(point, index);
                return (this->EvaluateAtContinuousIndex(index));
            }

            /** Interpolate the image at a continuous index position
             *
             * Returns the interpolated image intensity at a
             * specified index position. No bounds checking is done.
             * The point is assume to lie within the image buffer.
             *
             * Subclasses must override this method.
             *
             * ImageFunction::IsInsideBuffer() can be used to check bounds before
             * calling the method. */
            virtual OutputType
            EvaluateAtContinuousIndex(const ContinuousIndexType & index) const = 0;

            /** Interpolate the image at an index position.
             * Simply returns the image value at the
             * specified index position. No bounds checking is done.
             * The point is assume to lie within the image buffer.
             *
             * ImageFunction::IsInsideBuffer() can be used to check bounds before
             * calling the method. */
            virtual OutputType
            EvaluateAtIndex(const IndexType & index) const
            {
                // ---------------------------------------------------------------------------------------------
                OutputType output;
                // ---------------------------------------------------------------------------------------------
                // Get the input pixel
                const PixelType & input = this->GetInputImage()->GetPixel(index);
                // ---------------------------------------------------------------------------------------------
                // Get the size
                const unsigned int size = input.GetSize();
                output.SetSize(size);
                // ---------------------------------------------------------------------------------------------
                for (unsigned int k = 0; k < size; k++)
                {
                    output[k] = static_cast<double> (input[k]);
                }
                return (output);
            }

            virtual void
            SetInputImage(const InputImageType * ptr)
            {
                // ---------------------------------------------------------------------------------------------
                // set the input image
                this->m_Image = ptr;

                // ---------------------------------------------------------------------------------------------
                // If the input pointer is valid
                if (ptr)
                {
                    typename InputImageType::SizeType size = ptr->GetBufferedRegion().GetSize();
                    // ---------------------------------------------------------------------------------------------
                    // set the start index
                    this->m_StartIndex = ptr->GetBufferedRegion().GetIndex();

                    for (unsigned int j = 0; j < ImageDimension; j++)
                    {
                        // ---------------------------------------------------------------------------------------------
                        // set the end index
                        this->m_EndIndex[j] = this->m_StartIndex[j] + static_cast<IndexValueType> (size[j]) - 1;
                        this->m_StartContinuousIndex[j] = static_cast<CoordRepType>( this->m_StartIndex[j] - 0.5 );
                        this->m_EndContinuousIndex[j] = static_cast<CoordRepType>( this->m_EndIndex[j] + 0.5 );
                    }
                }
            }

        protected:
            VectorInterpolateImageFunction()
            {
            }
            virtual
            ~VectorInterpolateImageFunction()
            {
            }
            void
            PrintSelf(std::ostream& os, itk::Indent indent) const
            {
                Superclass::PrintSelf(os, indent);
            }

        private:
            VectorInterpolateImageFunction(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

} // end namespace vns

#endif
