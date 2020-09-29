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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsResamplerHelper.h 3636 2011-12-13 17:44:04Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/

// (C) 2015, Julien Michel (CNES) <julien.michel@cnes.fr>
// Code ResamplerHelper
#include "vnsResamplerHelper.h"
namespace vns
{

    /** Constructor */
    ResamplerHelper::ResamplerHelper()
    {
    }

    /** Destructor */
    ResamplerHelper::~ResamplerHelper()
    {
    }

    VNSOriginType
    ResamplerHelper::ComputeOrigin(const VNSOriginType & inOrigin, const VNSSpacingType & inSpacing, const VNSSpacingType & outSpacing,
            ResamplerHelper::ModeType mode)
    {

        VNSOriginType outOrigin;
        VNSSpacingType factor;

        // --------------------------------------------------------------------------------------
        // Resampling factor in line and column
        factor[0] = static_cast<double>(inSpacing[0]) / static_cast<double>(outSpacing[0]);
        factor[1] = static_cast<double>(inSpacing[1]) / static_cast<double>(outSpacing[1]);

        vnsLogDebugMacro(
                "Resampling CheckComputeOrigin     - pixel mode:             "<<mode<< " (with CENTERED="<<vns::ResamplerHelper::RB_MODE_CENTERED_PIXEL<<" and UPPER_LEFT_CORNER="<<vns::ResamplerHelper::RB_MODE_UPPER_LEFT_CORNER_PIXEL<<")        ")
        vnsLogDebugMacro("                                  - Input image   origin:    ["<<inOrigin[0]<<";"<<inOrigin[1]<<"]        ")
        vnsLogDebugMacro("                                  -              spacing:    ["<<inSpacing[0]<<";"<<inSpacing[1]<<"]        ")
        vnsLogDebugMacro("                                  - Output image spacing:    ["<<outSpacing[0]<<";"<<outSpacing[1]<<"]        ")
        vnsLogDebugMacro("                                  - Factor:                  ["<<factor[0]<<";"<<factor[1]<<"]        ")

        // Check the sign of the input and output spacing
        if (((inSpacing[0] > 0) && (outSpacing[0] < 0)) || ((inSpacing[0] < 0) && (outSpacing[0] > 0))
                || ((inSpacing[1] > 0) && (outSpacing[1] < 0)) || ((inSpacing[1] < 0) && (outSpacing[1] > 0)))
        {
            vnsStaticExceptionBusinessMacro("For resampling, input and output spacing must have the same sign for each dimension!")
        }

        // --------------------------------------------------------------------------------------
        // Two convention are used to resample images depending on the satellite
        switch (mode)
            {
        // --------------------------------------------------------------------------------------
        case vns::ResamplerHelper::RB_MODE_CENTERED_PIXEL:
            {
                // In centered case, origin does not move
                outOrigin = inOrigin;
                break;
            }
            // --------------------------------------------------------------------------------------
        case vns::ResamplerHelper::RB_MODE_UPPER_LEFT_CORNER_PIXEL:
            {
                // In this case, the origin has to be shifted to preserve
                // footprint:
                // Formule CNES (J.Michel) et validee sur papier par CS
                outOrigin[0] = inOrigin[0] - (factor[0] - 1) * inSpacing[0] / (2 * factor[0]);
                outOrigin[1] = inOrigin[1] - (factor[1] - 1) * inSpacing[1] / (2 * factor[1]);
                break;
            }
            // --------------------------------------------------------------------------------------
        case vns::ResamplerHelper::NOT_MODE_INITIALIZED:
            // --------------------------------------------------------------------------------------
        default:
            {
                vnsStaticExceptionBusinessMacro("Unknown ResamplerHelper mode.")
                break;
            }
            }
        vnsLogDebugMacro("                          - Output image origin:    ["<<outOrigin[0]<<";"<<outOrigin[1]<<"]        ")
        return outOrigin;

    }

    void
    ResamplerHelper::ComputeParameters(const VNSOriginType & inOrigin, const VNSSpacingType & inSpacing, const VNSSizeType & inSize,
            const unsigned int fnum, const unsigned int fdenom, VNSOriginType & outOrigin, VNSSpacingType & outSpacing,
            VNSSizeType & outSize, const ModeType & mode)
    {

        // Compute resampling factor
        if (fnum == 0 || fdenom == 0)
        {
            vnsStaticExceptionBusinessMacro("At least one of fnum ("<<fnum<<") or fdenom ("<<fdenom<<") parameter is null")
        }
        const double factor = static_cast<double>(fnum) / static_cast<double>(fdenom);

        // Output spacing is always the same
        outSpacing[0] = inSpacing[0] / factor;
        outSpacing[1] = inSpacing[1] / factor;

        vnsLogDebugMacro(
                "Resampling information:   - pixel mode:             "<<mode<< " (with CENTERED="<<RB_MODE_CENTERED_PIXEL<<" and UPPER_LEFT_CORNER="<<RB_MODE_UPPER_LEFT_CORNER_PIXEL<<")        ")
        vnsLogDebugMacro("                          - num / denom -> factor:  "<<fnum<<"/"<<fdenom<<" -> "<<factor<<"        ")
        vnsLogDebugMacro("                          - Input image  origin:    ["<<inOrigin[0]<<";"<<inOrigin[1]<<"]        ")
        vnsLogDebugMacro("                                         spacing:   ["<<inSpacing[0]<<";"<<inSpacing[1]<<"]        ")
        vnsLogDebugMacro("                                         size:      ["<<inSize[0]<<";"<<inSize[1]<<"]        ")

        if (((inSpacing[0] > 0) && (outSpacing[0] < 0)) || ((inSpacing[0] < 0) && (outSpacing[0] > 0))
                || ((inSpacing[1] > 0) && (outSpacing[1] < 0)) || ((inSpacing[1] < 0) && (outSpacing[1] > 0)))
        {
            vnsStaticExceptionBusinessMacro("For resampling, input and output spacing must have the same sign for each dimension!")
        }

        switch (mode)
            {
        //------------------------------------------------------------------
        case RB_MODE_CENTERED_PIXEL:
            {
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
                vnsStaticExceptionBusinessMacro("Unknown ResamplerHelper mode.")
            }
            }
        vnsLogDebugMacro("                          - Output image origin:    ["<<outOrigin[0]<<";"<<outOrigin[1]<<"]        ")
        vnsLogDebugMacro("                                         spacing:   ["<<outSpacing[0]<<";"<<outSpacing[1]<<"]        ")
        vnsLogDebugMacro("                                         size:      ["<<outSize[0]<<";"<<outSize[1]<<"]        ")
    }

    AreaType
    ResamplerHelper::ComputeResamplingCenteredImageAreaFromInputImageAreaAndSizeOutputImageCriteria(const AreaType & pInputArea,
            const AreaType::SizeType & pOutputSize)
    {
        const double l_InputSizeX = static_cast<double>(pInputArea.Size[0]);
        const double l_InputSizeY = static_cast<double>(pInputArea.Size[1]);
        const double l_OutputSizeX = static_cast<double>(pOutputSize[0]);
        const double l_OutputSizeY = static_cast<double>(pOutputSize[1]);
        const double l_RatioX = l_InputSizeX / l_OutputSizeX;
        const double l_RatioY = l_InputSizeY / l_OutputSizeY;

        AreaType::OriginType l_InputNewOriginExpanded;
        AreaType l_OutputAreaComptuted;

        // ---------------------------------------------------------------------------------
        // A4 vertical case:
        if (l_RatioX > l_RatioY)
        {
            // Compute Black band size
            const double l_DemiBlackBandSize = (l_OutputSizeY - (l_InputSizeY / l_RatioX)) / 2.;
            vnsLogDebugMacro(" A4 vertical case (RatioX="<<l_RatioX<<" and RatioY="<< l_RatioY<<".  DemiBlackBandSize="<<l_DemiBlackBandSize<<".")
            // Same X Origin
            l_InputNewOriginExpanded[0] = pInputArea.Origin[0];
            l_InputNewOriginExpanded[1] = pInputArea.Origin[1] - l_DemiBlackBandSize * pInputArea.Spacing[1] * l_RatioX;

            // Computes the Output Area for the OutpuSize criteria
            // - Origin
            l_OutputAreaComptuted.Origin = l_InputNewOriginExpanded;
            // - Spacing
            l_OutputAreaComptuted.Spacing = pInputArea.Spacing * l_RatioX;
            // - Size
            l_OutputAreaComptuted.Size = pOutputSize;
        }
        else
        {
            // Compute Black band size
            const double l_DemiBlackBandSize = (l_OutputSizeX - (l_InputSizeX / l_RatioY)) / 2.;
            vnsLogDebugMacro(" A4 horizontal case (RatioX="<<l_RatioX<<" and RatioY="<< l_RatioY<<".  DemiBlackBandSize="<<l_DemiBlackBandSize<<".")
            l_InputNewOriginExpanded[0] = pInputArea.Origin[0] - l_DemiBlackBandSize * pInputArea.Spacing[0] * l_RatioY;
            // Same Y Origin
            l_InputNewOriginExpanded[1] = pInputArea.Origin[1];

            // Computes the Output Area for the OutpuSize criteria
            // - Origin
            l_OutputAreaComptuted.Origin = l_InputNewOriginExpanded;
            // - Spacing
            l_OutputAreaComptuted.Spacing = pInputArea.Spacing * l_RatioY;
            // - Size
            l_OutputAreaComptuted.Size = pOutputSize;
        }

        vnsLogDebugMacro("                          - Input area   origin:    ["<<pInputArea.Origin<<"]        ")
        vnsLogDebugMacro("                                         spacing:   ["<<pInputArea.Spacing<<"]        ")
        vnsLogDebugMacro("                                         size:      ["<<pInputArea.Size<<"]        ")

        vnsLogDebugMacro("                          - Output area  origin:    ["<<l_OutputAreaComptuted.Origin<<"]        ")
        vnsLogDebugMacro("                                         spacing:   ["<<l_OutputAreaComptuted.Spacing<<"]        ")
        vnsLogDebugMacro("                                         size:      ["<<l_OutputAreaComptuted.Size<<"]        ")

        return l_OutputAreaComptuted;
    }

}
