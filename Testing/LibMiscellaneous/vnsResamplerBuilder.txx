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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsResamplerBuilder.h 3636 2011-12-13 17:44:04Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsResamplerBuilder_txx
#define __vnsResamplerBuilder_txx

#include "vnsResamplerBuilder.h"
#include "vnsLoggers.h"

namespace vns
{



    template<class TInputImage, class TOutputImage>
    typename ResamplerBuilder<TInputImage, TOutputImage>::ResamplerPointer
    ResamplerBuilder<TInputImage, TOutputImage>::BuildSampling(TInputImage * inputPointer, const unsigned int & fnum, const unsigned int & fdenom, const InterpolatorType & interpolator, const ModeType& mode, bool strict)
     {
         // Instanciate a new instance of resampler
         typename ResamplerType::Pointer resampler = ResamplerType::New();

         // First, get input image parameters
         if (!inputPointer)
         {
             itkGenericExceptionMacro(<<"Input image pointer is null.");
         }

         // Update image pointer to ensure information is here
         inputPointer->UpdateOutputInformation();

         // Retrieve origin, spacing, size
         typename TInputImage::PointType inOrigin = inputPointer->GetOrigin();
         typename TInputImage::SpacingType inSpacing = inputPointer->GetSpacing();
         typename TInputImage::SizeType inSize = inputPointer->GetLargestPossibleRegion().GetSize();

         // Now, compute output image parameters
         typename TOutputImage::PointType outOrigin;
         typename TOutputImage::SpacingType outSpacing;
         typename TOutputImage::SizeType outSize;

         // Compute output parameters
         ComputeParameters(inOrigin, inSpacing, inSize, fnum, fdenom, outOrigin, outSpacing, outSize, mode, strict);

         // Resampler set-up
         resampler->SetOutputOrigin(outOrigin);
         resampler->SetOutputSpacing(outSpacing);
         resampler->SetOutputSize(outSize);
//                resampler->SetInput(inputPointer);
         resampler->SetDisplacementFieldSpacing(outSpacing);


         // Interpolator set-up
         switch (interpolator)
             {
         case RB_INTERPOLATOR_LINEAR:
             {
                 typename LinearInterpolatorType::Pointer f_interpolator = LinearInterpolatorType::New();
                 resampler->SetInterpolator(f_interpolator);
                 break;
             }
         case RB_INTERPOLATOR_LINEAR_MACCS:
             {
                 typename MACCSLinearWithRadiusInterpolatorType::Pointer f_interpolator = MACCSLinearWithRadiusInterpolatorType::New();
                 f_interpolator->SetRadius(ComputeInterpolatorRadius(fnum, fdenom, interpolator));
                 resampler->SetInterpolator(f_interpolator);
                 break;
             }
         case RB_INTERPOLATOR_BCO:
             {
                 typename BCOInterpolatorType::Pointer f_interpolator = BCOInterpolatorType::New();
                 f_interpolator->SetRadius(ComputeInterpolatorRadius(fnum, fdenom, interpolator));
                 resampler->SetInterpolator(f_interpolator);
                 break;
             }
         case NOT_INTERPOLATOR_INITIALIZED:
             {
                 break;
             }
             }

         // Return the resampler we created
         return resampler;
     }

    template<class TInputImage, class TOutputImage>
        unsigned int
        ResamplerBuilder<TInputImage, TOutputImage>::ComputeInterpolatorRadius(const unsigned int & fnum, const unsigned int & fdenom,
                const InterpolatorType & interpolator)
        {
    		//------------------------------------------------------------------
            // Test if it's subsampling or oversampling
            double inv_factor(0.);
            if (fnum >= fdenom)
            {
                vnsLogDebugMacro("ResamplerBuilder: It's an 'OVER' sampling treatments (num='" << fnum << "' and denom='" << fdenom << "')")
                inv_factor = 1.;
            }
            else
            {
                vnsLogDebugMacro("ResamplerBuilder: It's a 'SUB' sampling treatments (num='" << fnum << "' and denom='" << fdenom << "')")
                inv_factor = static_cast<double>(fdenom) / static_cast<double>(fnum);
            }
            //------------------------------------------------------------------
            // Instanciate a new instance of resampler
            unsigned int l_Radius(0);

            //------------------------------------------------------------------
            // Interpolator set-up
            switch (interpolator)
                {
            case RB_INTERPOLATOR_LINEAR:
            case RB_INTERPOLATOR_LINEAR_MACCS:
                {
                    l_Radius = static_cast<unsigned int>(1. * inv_factor);
                    break;
                }
            // With BCO interpolator, the radius is multiplied per 2
            case RB_INTERPOLATOR_BCO:
                {
                    l_Radius = static_cast<unsigned int>(2. * inv_factor);
                    break;
                }
            case NOT_INTERPOLATOR_INITIALIZED:
            default:
                {
                    vnsStaticExceptionBusinessMacro("This interpolator don't need a radius parameter!!")
                    break;
                }
                }
            vnsLogDebugMacro("ResamplerBuilder::ComputeInterpolatorRadius:  Radius computed: " << l_Radius)

            return l_Radius;
        }


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
    template<class TInputImage, class TOutputImage>
        void
        ResamplerBuilder<TInputImage, TOutputImage>::ComputeParameters(const typename TInputImage::PointType & inOrigin,
                const typename TInputImage::SpacingType & inSpacing, const typename TInputImage::SizeType & inSize, const unsigned int fnum,
                const unsigned int fdenom, typename TOutputImage::PointType & outOrigin, typename TOutputImage::SpacingType & outSpacing,
                typename TOutputImage::SizeType & outSize, const ModeType & mode, bool strict)
        {

    		//------------------------------------------------------------------
            // Compute resampling factor
            if (fnum == 0 || fdenom == 0)
            {
                vnsStaticExceptionBusinessMacro("At least one of fnum ("<<fnum<<") or fdenom ("<<fdenom<<") parameter is null")
            }
            const double factor = static_cast<double>(fnum) / static_cast<double>(fdenom);

            //------------------------------------------------------------------
            // Output spacing is always the same
            outSpacing[0] = inSpacing[0] / factor;
            outSpacing[1] = inSpacing[1] / factor;

            vnsLogDebugMacro(
                    "Resampling information:   - pixel mode:             "<<mode<< " (with CENTERED="<<RB_MODE_CENTERED_PIXEL<<" and UPPER_LEFT_CORNER="<<RB_MODE_UPPER_LEFT_CORNER_PIXEL<<")        ")
            vnsLogDebugMacro("                          - num / denom -> factor:  "<<fnum<<"/"<<fdenom<<" -> "<<factor<<"        ")
            vnsLogDebugMacro("                          - Input image  origin:    ["<<inOrigin[0]<<";"<<inOrigin[1]<<"]        ")
            vnsLogDebugMacro("                                         spacing:   ["<<inSpacing[0]<<";"<<inSpacing[1]<<"]        ")
            vnsLogDebugMacro("                                         size:      ["<<inSize[0]<<";"<<inSize[1]<<"]        ")

            // Specify the resampling convention
            switch (mode)
                {
            //------------------------------------------------------------------
            case RB_MODE_CENTERED_PIXEL:
                {
                    if (strict && (((inSize[0] - 1) % fdenom != 0) || ((inSize[1] - 1) % fdenom != 0)))
                    {
                        vnsStaticExceptionBusinessMacro("Strict mode is enabled, and exact resampling is impossible.")
                    }

                    // In centered case, origin does not move
                    outOrigin = inOrigin;

                    // In centered case, the output size needs to be adjusted
                    // Formule CNES (J.Michel) et validee sur papier par CS : outSize = factor * inSize + 1 - factor
                    // -> ajout de 0.5 pour arrondi a l'entier le plus proche
                    outSize[0] = static_cast<unsigned long>(vcl_floor(factor * inSize[0] + 1 - factor + 0.5));
                    outSize[1] = static_cast<unsigned long>(vcl_floor(factor * inSize[1] + 1 - factor + 0.5));
                    break;

                }

            //------------------------------------------------------------------
            case RB_MODE_UPPER_LEFT_CORNER_PIXEL:
                {
                    if (strict && (((inSize[0] % fdenom) != 0) || ((inSize[1] % fdenom) != 0)))
                    {
                        vnsStaticExceptionBusinessMacro("Strict mode is enabled, and exact resampling is impossible.")
                    }

                    // In this case, the origin has to be shifted to preserve
                    // footprint:
                    // Formule CNES (J.Michel) et validee sur papier par CS
                    outOrigin[0] = inOrigin[0] - (factor - 1) * inSpacing[0] / (2 * factor);
                    outOrigin[1] = inOrigin[1] - (factor - 1) * inSpacing[1] / (2 * factor);

                    // Size is straightforward
                    outSize[0] = static_cast<unsigned long>(vcl_floor(factor * inSize[0] + 0.5));
                    outSize[1] = static_cast<unsigned long>(vcl_floor(factor * inSize[1] + 0.5));
                    break;
                }
            //------------------------------------------------------------------
            case NOT_MODE_INITIALIZED:
            //------------------------------------------------------------------
            default:
                {
                    vnsStaticExceptionBusinessMacro("Unknown ResamplerBuilder mode.")
                    break;
                }
                }
            vnsLogDebugMacro("                          - Output image origin:    ["<<outOrigin[0]<<";"<<outOrigin[1]<<"]        ")
            vnsLogDebugMacro("                                         spacing:   ["<<outSpacing[0]<<";"<<outSpacing[1]<<"]        ")
            vnsLogDebugMacro("                                         size:      ["<<outSize[0]<<";"<<outSize[1]<<"]        ")

        }
}
// namespace vns

#endif
