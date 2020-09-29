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
// (C) 2015, Julien Michel (CNES) <julien.michel@cnes.fr>

// Code ResamplerBuilder

#ifndef __vnsResamplerBuilder_h
#define __vnsResamplerBuilder_h

#include "otbStreamingResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "vnsLinearInterpolateImageFunction.h"
#include "vnsMacro.h"

namespace vns
{
    /**
     * \class ResamplerBuilder
     * \brief Helper class to build a ready to run instance of resampler
     *
     * This class can not be instanciated and only provides two static
     * methods:
     *
     * The ComputeParameters method will compute the output image
     * parameters (origin, spacing, size) for resampling according to the
     * input image parameters (origin, spacing, size), the resampling
     * ratio (k/l) and an ouptut grid mode (either pixel centers or
     * global footprint are preserved).
     *
     * The Build method will return a ready to run
     * StreamingResampleImageFilter instance based on an input image
     * pointer (the image to resample), the resampling ratio (k/l), the
     * output grid mode and the interpolator.
     *
     */
    template<class TInputImage, class TOutputImage = TInputImage>
        class ResamplerBuilder
        {
        public:

            typedef otb::StreamingResampleImageFilter<TInputImage, TOutputImage> ResamplerType;
            typedef typename ResamplerType::Pointer ResamplerPointer;
            typedef itk::LinearInterpolateImageFunction<TInputImage, double> LinearInterpolatorType;
            typedef otb::BCOInterpolateImageFunction<TInputImage, double> BCOInterpolatorType;
            typedef LinearInterpolateImageFunction<TInputImage, double> MACCSLinearWithRadiusInterpolatorType;

            // Enum for interpolator
            typedef enum
            {
                RB_INTERPOLATOR_LINEAR,
                RB_INTERPOLATOR_LINEAR_MACCS /* LINEAR with RADIUS: for Smoothy in sub sampling */,
                RB_INTERPOLATOR_BCO,
                NOT_INTERPOLATOR_INITIALIZED
            } InterpolatorType;

            // Enum for output grid mode
            typedef enum
            {
                RB_MODE_CENTERED_PIXEL, RB_MODE_UPPER_LEFT_CORNER_PIXEL /* */, NOT_MODE_INITIALIZED
            } ModeType;

            /**
             * Build a ready to run StreamingResampleImageFilter instance from
             * an input image pointer, the resampling ratio, interpolation type
             * and output grid mode.
             * \param inputPointer Pointer to the image to resample
             * \param fnum The size of the input image will be multiplied by fnum/fdenom
             * \param fdenom The size of the input image will be multiplied by fnum/fdenom
             * \param interpolator The interpolator type (can be
             *  RB_INTERPOLATOR_LINEAR or RB_INTERPOLATOR_BCO)
             * \param mode The output grid mode (can be RB_MODE_CENTERED_PIXEL or
             *  RB_MODE_UPPER_LEFT_CORNER_PIXEL)
             * \param strict If true an exception will be thrown if computation
             *  does not yield exact results
             * \return A pointer to a read to run instance of the StreamingResampleImageFilter
             */

            static ResamplerPointer
            BuildSampling(TInputImage * inputPointer, const unsigned int & fnum, const unsigned int & fdenom, const InterpolatorType & interpolator,
                         const ModeType& mode, bool strict = true);


            /**
             * Computes the dedicated radius from the interpolator and num / denom
             * \param fnum The size of the input image will be multiplied by fnum/fdenom
             * \param fdenom The size of the input image will be multiplied by fnum/fdenom
             * \param interpolator The interpolator type (can be
             *  RB_INTERPOLATOR_LINEAR or RB_INTERPOLATOR_BCO)
             * \return the radius value
             */
            static unsigned int
            ComputeInterpolatorRadius(const unsigned int & fnum, const unsigned int & fdenom, const InterpolatorType & interpolator);

            /**
             * Compute the output image parameters from the input parameter,
             * resampling ratio and output image grid mode.
             * \param inOrigin Origin of the input image
             * \param inSpacing Spacing of the input image
             * \param inSize Size of the input image
             * \param fnum The size of the input image will be multiplied by fnum/fdenom
             * \param fdenom The size of the input image will be multiplied by fnum/fdenom
             * \param outOrigin A pointer to the variable holding output origin (will be
             *  updated during call)
             * \param outSpacing A pointer to the variable holding output spacing (will be
             *  updated during call)
             * \param outSize A pointer to the variable holding output size (will be
             *  updated during call)
             * \param mode The output grid mode (can be RB_MODE_CENTERED_PIXEL or RB_MODE_UPPER_LEFT_CORNER_PIXEL)
             * \param strict If true an exception will be thrown if computation
             */
            static void
            ComputeParameters(const typename TInputImage::PointType & inOrigin, const typename TInputImage::SpacingType & inSpacing,
                    const typename TInputImage::SizeType & inSize, const unsigned int fnum, const unsigned int fdenom,
                    typename TOutputImage::PointType & outOrigin, typename TOutputImage::SpacingType & outSpacing,
                    typename TOutputImage::SizeType & outSize, const ModeType & mode, bool strict = true);

        private:
            // This class can not be instanciated
            ResamplerBuilder();                //purposely not implemented
            ~ResamplerBuilder();                //purposely not implemented
            ResamplerBuilder(const ResamplerBuilder&);                //purposely not implemented
            void
            operator =(const ResamplerBuilder&);                //purposely not implemented
        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsResamplerBuilder.txx"
#endif

#endif

