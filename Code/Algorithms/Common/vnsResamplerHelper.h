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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 14 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 29 juin 2016 : Audit code - Correction du code            *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Modification pour corriger l'anomalie     *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 4-2-0 : DM : LAIG-DM-MAC-976-CNES : 10 mars 2015 : Creation                                    *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCopyImage.cxx 8544 2014-06-19 12:39:17Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsResamplerHelper_h
#define __vnsResamplerHelper_h

#include <iomanip> // std::setprecision
#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"

#include "vnsLinearInterpolateImageFunction.h"
#include "vnsFastLinearInterpolateImageFunction.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"

namespace vns
{
    /**
     * \class ResamplerHelper
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
    class ResamplerHelper
    {
    public:

        // Enum for interpolator
        typedef enum
        {
            RB_INTERPOLATOR_LINEAR, /**
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

            RB_INTERPOLATOR_LINEAR_WITH_RADIUS /* LINEAR with RADIUS: for Smoothy in sub sampling */,
            RB_INTERPOLATOR_BCO,
            RB_INTERPOLATOR_MAJA_FAST_LINEAR /* MAJA LINEAR reimplemnetd */,
            RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* MAJA LINEAR reimplemnetd */,
            RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR_WITH_RADIUS /* MAJA vector  LINEAR reimplemnetd with RADIUS */,
            RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA, /* CloudUndersampling computes CLA */
            RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLD_ALT, /* CloudUndersampling computes CLD Alt */
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
        template<class TResampler>
            static void
            BuildSampling(TResampler * resampler, const typename TResampler::InputImageType * inputPointer,
                    const InterpolatorType & interpolator, const typename TResampler::OutputImageType::PointType outOrigin,
                    const typename TResampler::OutputImageType::SpacingType outSpacing,
                    const typename TResampler::OutputImageType::SizeType outSize,
                    const typename TResampler::OutputImageType::PixelType edgePaddingValue)
            {
                // ---------------------------------------------------------------------------------------------
                typedef typename TResampler::InputImageType InputImageType;

                // ---------------------------------------------------------------------------------------------
                typedef itk::LinearInterpolateImageFunction<InputImageType, double> LinearInterpolatorType;
                typedef otb::BCOInterpolateImageFunction<InputImageType, double> BCOInterpolatorType;
                // ---------------------------------------------------------------------------------------------
                typedef LinearInterpolateImageFunction<InputImageType, double> MAJALinearWithRadiusInterpolatorType;
                typedef FastLinearInterpolateImageFunction<InputImageType, double> MAJAFastLinearInterpolatorType;

                // --------------------------------------------------------------------------------------
                // First, get input image parameters
                if (!inputPointer)
                {
                    // ---------------------------------------------------------------------------------------------
                    vnsStaticExceptionBusinessMacro("Internal error: In the ResamplerHelper, the input image pointer is null.")
                }
                // ---------------------------------------------------------------------------------------------
                InputImageType * l_NonConstinputPointer = const_cast<InputImageType*>(inputPointer);
                l_NonConstinputPointer->UpdateOutputInformation();

                // ---------------------------------------------------------------------------------------------
                vnsLogDebugMacro(
                        "ResamplerHelper: Input image origin : [" << std::setprecision(25) << inputPointer->GetOrigin()[0] << ";"<<inputPointer->GetOrigin()[1] << "]")
                vnsLogDebugMacro(
                        "                            spacing : [" << std::setprecision(25) << inputPointer->GetSpacing()[0] << ";"<<inputPointer->GetSpacing()[1] << "]")
                vnsLogDebugMacro(
                        "                               size : [" << inputPointer->GetLargestPossibleRegion().GetSize()[0] << ";"<<inputPointer->GetLargestPossibleRegion().GetSize()[1] << "]")
//                vnsLogDebugMacro("    (Input non const image)  origin : [" << l_NonConstinputPointer->GetOrigin()[0] << ";"<<l_NonConstinputPointer->GetOrigin()[1] << "]")
//                vnsLogDebugMacro("                            spacing : [" << l_NonConstinputPointer->GetSpacing()[0] << ";"<<l_NonConstinputPointer->GetSpacing()[1] << "]")
//                vnsLogDebugMacro("                               size : [" << l_NonConstinputPointer->GetLargestPossibleRegion().GetSize()[0] << ";"<<l_NonConstinputPointer->GetLargestPossibleRegion().GetSize()[1] << "]")
                // ---------------------------------------------------------------------------------------------
                vnsLogDebugMacro(
                        "                Output image origin : [" << std::setprecision(25) << outOrigin[0] << ";"<<outOrigin[1] << "]")
                vnsLogDebugMacro(
                        "                            spacing : [" << std::setprecision(25) << outSpacing[0] << ";"<<outSpacing[1] << "]")
                vnsLogDebugMacro("                               size : [" << outSize[0] << ";"<<outSize[1] << "]")

                // --------------------------------------------------------------------------------------
                // Check that the input and the output spacing are consistent
                if (((inputPointer->GetSpacing()[0] > 0) && (outSpacing[0] < 0))
                        || ((inputPointer->GetSpacing()[0] < 0) && (outSpacing[0] > 0))
                        || ((inputPointer->GetSpacing()[1] > 0) && (outSpacing[1] < 0))
                        || ((inputPointer->GetSpacing()[1] < 0) && (outSpacing[1] > 0)))
                {
                    vnsStaticExceptionBusinessMacro("For resampling, input and output spacing must have the same sign for each dimension!")
                }
                // --------------------------------------------------------------------------------------
                // Resampler set-up
                resampler->SetOutputOrigin(outOrigin);
                resampler->SetOutputSpacing(outSpacing);
                resampler->SetOutputSize(outSize);
                resampler->SetInput(inputPointer);
                resampler->SetDisplacementFieldSpacing(outSpacing);
                resampler->SetEdgePaddingValue(edgePaddingValue);

                // --------------------------------------------------------------------------------------
                // Interpolator set-up
                switch (interpolator)
                    {
                // --------------------------------------------------------------------------------------
                case RB_INTERPOLATOR_LINEAR:
                    {
                        // ---------------------------------------------------------------------------------------------
                        typename LinearInterpolatorType::Pointer f_interpolator = LinearInterpolatorType::New();
                        vnsLogDebugMacro("RB_INTERPOLATOR_LINEAR")
                        // ---------------------------------------------------------------------------------------------
                        resampler->SetInterpolator(f_interpolator);
                        break;
                    }
                    // --------------------------------------------------------------------------------------
                case RB_INTERPOLATOR_BCO:
                    {
                        // ---------------------------------------------------------------------------------------------
                        typename BCOInterpolatorType::Pointer f_interpolator = BCOInterpolatorType::New();
                        unsigned int radius = ComputeInterpolatorRadius(inputPointer->GetSpacing(), outSpacing, interpolator);
                        // ---------------------------------------------------------------------------------------------
                        vnsLogDebugMacro("RB_INTERPOLATOR_BCO - Radius = "<<radius)
                        f_interpolator->SetRadius(radius);
                        // ---------------------------------------------------------------------------------------------
                        resampler->SetInterpolator(f_interpolator);
                        break;
                    }
                    // --------------------------------------------------------------------------------------
                case RB_INTERPOLATOR_LINEAR_WITH_RADIUS:
                    {
                        // ---------------------------------------------------------------------------------------------
                        typename MAJALinearWithRadiusInterpolatorType::Pointer f_interpolator =
                                MAJALinearWithRadiusInterpolatorType::New();
                        unsigned int radius = ComputeInterpolatorRadius(inputPointer->GetSpacing(), outSpacing, interpolator);
                        vnsLogDebugMacro("RB_INTERPOLATOR_LINEAR_WITH_RADIUS - Radius = "<<radius)
                        f_interpolator->SetRadius(radius);
                        // ---------------------------------------------------------------------------------------------
                        resampler->SetInterpolator(f_interpolator);
                        break;
                    }
                    // --------------------------------------------------------------------------------------
                case RB_INTERPOLATOR_MAJA_FAST_LINEAR:
                    {
                        // ---------------------------------------------------------------------------------------------
                        typename MAJAFastLinearInterpolatorType::Pointer f_interpolator = MAJAFastLinearInterpolatorType::New();
                        vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_FAST_LINEAR")
                        // ---------------------------------------------------------------------------------------------
                        resampler->SetInterpolator(f_interpolator);
                        break;
                    }
                    // --------------------------------------------------------------------------------------
                case NOT_INTERPOLATOR_INITIALIZED:
                default:
                    {
                        // ---------------------------------------------------------------------------------------------
                        vnsStaticExceptionBusinessMacro("The interpolator is not initialized.")
                    }
                    }

            }
        static bool
        CheckSpacingAndGetIsSubSamplingCondition(const VNSSpacingType inSpacing, const VNSSpacingType outSpacing)
        {

            // --------------------------------------------------------------------------------------
            // Check that the input and the output spacing are consistent
            if (((inSpacing[0] > 0) && (outSpacing[0] < 0)) || ((inSpacing[0] < 0) && (outSpacing[0] > 0))
                    || ((inSpacing[1] > 0) && (outSpacing[1] < 0)) || ((inSpacing[1] < 0) && (outSpacing[1] > 0)))
            {
                vnsStaticExceptionBusinessMacro(
                        "For resampling, input spacing "<<inSpacing<<" and output spacing "<<outSpacing<<" must have the same sign for each dimension!")
            }
            // --------------------------------------------------------------------------------------
            // Get the resampling conditions
            // oversampling
            bool isSubSampling(false);
            if ((vcl_abs(inSpacing[0]) >= vcl_abs(outSpacing[0])) || (vcl_abs(inSpacing[1]) >= vcl_abs(outSpacing[1])) )
            {
                vnsLogDebugMacro(
                "ResamplerHelper: It's an 'OVER' sampling processing (input spacing=[" << std::setprecision(25) << inSpacing[0] << ";"<<inSpacing[1] <<"] and out spacing=[" << outSpacing[0] << ";"<<outSpacing[1] <<"])")
                isSubSampling = false;
            }
            // subsampling
            else
            {
                // Mean of the both sapcing dimension
                vnsLogDebugMacro(
                "ResamplerHelper: It's a 'SUB' sampling processing (input spacing=[" << std::setprecision(25) << inSpacing[0] << ";"<<inSpacing[1] <<"] and out spacing=[" << outSpacing[0] << ";"<<outSpacing[1] <<"])")
                isSubSampling = true;
            }
            return isSubSampling;
        }
        /**
         * Computes the dedicated radius from the interpolator and num / denom
         * \param fnum The size of the input image will be multiplied by fnum/fdenom
         * \param fdenom The size of the input image will be multiplied by fnum/fdenom
         * \param interpolator The interpolator type (can be
         *  RB_INTERPOLATOR_LINEAR or RB_INTERPOLATOR_BCO)
         * \return the radius value
         */
        static unsigned int
        ComputeInterpolatorRadius(const VNSSpacingType inSpacing, const VNSSpacingType outSpacing, const InterpolatorType & interpolator)
        {
            // --------------------------------------------------------------------------------------
            // Test if it is an oversampling or a subsampling
            const bool isSubSampling = CheckSpacingAndGetIsSubSamplingCondition(inSpacing, outSpacing);

            unsigned int l_int_inv_factor(1);
            if (isSubSampling == true)
            {
                // Mean of the both sapcing dimension
                const double inv_factor = (outSpacing[0] / inSpacing[0] + outSpacing[1] / inSpacing[1]) / 2.;
                l_int_inv_factor = static_cast<unsigned int>(inv_factor + 0.5);
                vnsLogDebugMacro("'SUB' sampling Factor computed : "<<l_int_inv_factor << " (real="<<inv_factor<<")")
            }
            // Instanciate a new instance of resampler

            unsigned int l_Radius(0);

            // --------------------------------------------------------------------------------------
            // Interpolator set-up
            switch (interpolator)
                {
            // --------------------------------------------------------------------------------------
            case RB_INTERPOLATOR_LINEAR:
            case RB_INTERPOLATOR_MAJA_FAST_LINEAR:
                {
                    l_Radius = 0;
                    break;
                }
                // --------------------------------------------------------------------------------------
            case RB_INTERPOLATOR_LINEAR_WITH_RADIUS:
                {
                    if (isSubSampling == true)
                    {
                        l_Radius = 1 * l_int_inv_factor;
                    }
                    // To be complient with the vnsExpandImageFilter that fixed the radius to 2 in the
                    // oversampling case
                    else
                    {
                        l_Radius = 2;
                    }
                    break;
                }
                // --------------------------------------------------------------------------------------
            case RB_INTERPOLATOR_BCO:
                {
                    if (isSubSampling == true)
                    {
                        l_Radius = 2 * l_int_inv_factor;
                    }
                    else
                    {
                        l_Radius = 2;
                    }
                    break;
                }
                // --------------------------------------------------------------------------------------
            case RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR_WITH_RADIUS:
                {
                    if (isSubSampling == true)
                    {
                        l_Radius = 1 * l_int_inv_factor;
                    }
                    // To be complient with the vnsExpandImageFilter that fixed the radius to 2 in the
                    // oversampling case
                    else
                    {
                        l_Radius = 2;
                    }
                    break;
                }
                // --------------------------------------------------------------------------------------
            case RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA:
            case RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLD_ALT:
                {
                    if (isSubSampling == true)
                    {
                        // --------------------------------------------------------------------------------------
                        // Spec algo - From CloudMaskUndersamplingImageFilter Deprecated class since V MACCS 5.0:
                        // # const double l_SubsampleFactor = vcl_abs(static_cast<double>(m_AreaToL2CoarseResolution.Spacing[0]) / static_cast<double>(m_AreaToCLDResolution.Spacing[0]));
                        // # const double l_RealRadius = ((l_SubsampleFactor - 1.) / 2.) + 0.5;
                        // --------------------------------------------------------------------------------------
                        const double l_RealRadius = ((static_cast<double>(l_int_inv_factor) - 1.) / 2.) + 0.5;
                        l_Radius = static_cast<unsigned int>(l_RealRadius);
                    }
                    // To be complient with the vnsExpandImageFilter that fixed the radius to 2 in the
                    // oversampling case
                    else
                    {
                        l_Radius = 1;
                    }

                    break;
                }
            case NOT_INTERPOLATOR_INITIALIZED:
            default:
                {
                    vnsStaticExceptionBusinessMacro("This interpolator don't need a radius parameter!!")
                }
                }
            vnsLogDebugMacro("ResamplerHelper::ComputeInterpolatorRadius:  Radius computed: " << l_Radius)

            return l_Radius;

        }

        static VNSOriginType
        ComputeOrigin(const VNSOriginType & inOrigin, const VNSSpacingType & inSpacing, const VNSSpacingType & outSpacing,
                ResamplerHelper::ModeType mode = ResamplerHelper::RB_MODE_UPPER_LEFT_CORNER_PIXEL);

        static void
        ComputeParameters(const VNSOriginType & inOrigin, const VNSSpacingType & inSpacing, const VNSSizeType & inSize,
                const unsigned int fnum, const unsigned int fdenom, VNSOriginType & outOrigin, VNSSpacingType & outSpacing,
                VNSSizeType & outSize, const ModeType & mode);

        static void
        ComputeParameters(const AreaType & inArea, const unsigned int fnum, const unsigned int fdenom, AreaType & outArea,
                const ModeType & mode)
        {
            ComputeParameters(inArea.Origin, inArea.Spacing, inArea.Size, fnum, fdenom, outArea.Origin, outArea.Spacing, outArea.Size,
                    mode);
        }

        static AreaType
        ComputeResamplingCenteredImageAreaFromInputImageAreaAndSizeOutputImageCriteria(const AreaType & pInputArea,
                const AreaType::SizeType & pOutputSize);

        // This class can not be instanciated
        ResamplerHelper();                //purposely not implemented
        ~ResamplerHelper();                //purposely not implemented

    private:
    };

} // end namespace vns

#endif

