/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Creation pour corriger l'anomalie         *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudMaskUndersamplingWithoutResamplingImageFilter.h 11465 2016-01-28 15:51:04Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsCloudMaskUndersamplingInterpolateImageFunction_txx
#define __vnsCloudMaskUndersamplingInterpolateImageFunction_txx

#include "vnsCloudMaskUndersamplingInterpolateImageFunction.h"

#include "itkNumericTraits.h"
#include "itkMath.h"
#include "vnsMath.h"

namespace vns
{


    template<class TInputImage, class TCoordRep>
        void
        CloudMaskUndersamplingInterpolateImageFunctionBase<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << indent << "Radius: " << m_Radius << std::endl;
            os << indent << "WinSize: " << m_WinSize << std::endl;
            os << indent << "NoData: " << m_NoData << std::endl;
        }



    template<class TInputImage, class TCoordRep>
        typename CloudMaskUndersamplingCLAInterpolateImageFunction<TInputImage, TCoordRep>::OutputType
        CloudMaskUndersamplingCLAInterpolateImageFunction<TInputImage, TCoordRep>::EvaluateAtContinuousIndex(const ContinuousIndexType & index) const
        {

            IndexType baseIndex;
            IndexType neighIndex;
            // Compute base index = closet index
            for (unsigned int dim = 0; dim < ImageDimension; dim++)
            {
                baseIndex[dim] = itk::Math::Floor<IndexValueType>(index[dim] + 0.5);
            }

            // Init var
            RealType l_Sum(0.);
            unsigned long l_Number(0);

            // Loops under image size
            for (unsigned int i = 0; i < this->m_WinSize; ++i)
            {
                for (unsigned int j = 0; j < this->m_WinSize; ++j)
                {
                    // get neighbor index
                    neighIndex[0] = baseIndex[0] + i - this->m_Radius;
                    neighIndex[1] = baseIndex[1] + j - this->m_Radius;

                    if (neighIndex[0] > this->m_EndIndex[0])
                    {
                        neighIndex[0] = this->m_EndIndex[0];
                    }
                    if (neighIndex[0] < this->m_StartIndex[0])
                    {
                        neighIndex[0] = this->m_StartIndex[0];
                    }
                    if (neighIndex[1] > this->m_EndIndex[1])
                    {
                        neighIndex[1] = this->m_EndIndex[1];
                    }
                    if (neighIndex[1] < this->m_StartIndex[1])
                    {
                        neighIndex[1] = this->m_StartIndex[1];
                    }
                    const RealType l_InputPixelValue = static_cast<RealType>(this->GetInputImage()->GetPixel(neighIndex));
                    if (( vnsDifferentDoubleMacro(l_InputPixelValue, 0.) == true) && ( vnsDifferentDoubleMacro(l_InputPixelValue, this->m_NoData) == true))
                    {
                        l_Sum += l_InputPixelValue;
                        l_Number++;
                   }
                }
            }

            // Check the value of nb number
            if (l_Number != 0)
            {
                return static_cast<OutputType>(l_Sum / static_cast<double>(l_Number));
            }
            else
            {
                return static_cast<OutputType>(0.);
            }
        }

    template<class TInputImage, class TCoordRep>
        void
        CloudMaskUndersamplingCLAInterpolateImageFunction<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    template<typename TPixel, unsigned int VImageDimension, class TCoordRep>
        typename CloudMaskUndersamplingCLAInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::OutputType
        CloudMaskUndersamplingCLAInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::EvaluateAtContinuousIndex(
                const ContinuousIndexType & /*index*/) const
        {
            itkExceptionMacro("CloudMaskUndersamplingCLAInterpolateImageFunction with VectorImage<> EvaluateAtContinuousIndex not implemented!!!!")
            OutputType output;
            return (output);
        }
    template<typename TPixel, unsigned int VImageDimension, class TCoordRep>
         void
         CloudMaskUndersamplingCLAInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::PrintSelf(std::ostream& os,
                 itk::Indent indent) const
         {
             Superclass::PrintSelf(os, indent);
         }



    template<class TInputImage, class TCoordRep>
        typename CloudMaskUndersamplingCLDAltInterpolateImageFunction<TInputImage, TCoordRep>::OutputType
        CloudMaskUndersamplingCLDAltInterpolateImageFunction<TInputImage, TCoordRep>::EvaluateAtContinuousIndex(const ContinuousIndexType & index) const
        {

            IndexType baseIndex;
            IndexType neighIndex;

            // Compute base index = closet index
            for (unsigned int dim = 0; dim < ImageDimension; dim++)
            {
                baseIndex[dim] = itk::Math::Floor<IndexValueType>(index[dim] + 0.5);
            }

            // Loops under image size
            for (unsigned int i = 0; i < this->m_WinSize; ++i)
            {
                for (unsigned int j = 0; j < this->m_WinSize; ++j)
                {
                    // get neighbor index
                    neighIndex[0] = baseIndex[0] + i - this->m_Radius;
                    neighIndex[1] = baseIndex[1] + j - this->m_Radius;

                    if (neighIndex[0] > this->m_EndIndex[0])
                    {
                        neighIndex[0] = this->m_EndIndex[0];
                    }
                    if (neighIndex[0] < this->m_StartIndex[0])
                    {
                        neighIndex[0] = this->m_StartIndex[0];
                    }
                    if (neighIndex[1] > this->m_EndIndex[1])
                    {
                        neighIndex[1] = this->m_EndIndex[1];
                    }
                    if (neighIndex[1] < this->m_StartIndex[1])
                    {
                        neighIndex[1] = this->m_StartIndex[1];
                    }
                    const RealType l_InputPixelValue = static_cast<RealType>(this->GetInputImage()->GetPixel(neighIndex));
                    if (( vnsDifferentDoubleMacro(l_InputPixelValue, 0.) == true) && ( vnsDifferentDoubleMacro(l_InputPixelValue, this->m_NoData) == true))
                    {
                        return this->m_SuccesOutputValue;
                    }
                }
            }

            return this->m_FailureOutputValue;
        }

    template<class TInputImage, class TCoordRep>
        void
        CloudMaskUndersamplingCLDAltInterpolateImageFunction<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << indent << "SuccesOutputValue: " << m_SuccesOutputValue << std::endl;
            os << indent << "FailureOutputValue: " << m_FailureOutputValue << std::endl;
        }
    template<typename TPixel, unsigned int VImageDimension, class TCoordRep>
        typename CloudMaskUndersamplingCLDAltInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::OutputType
        CloudMaskUndersamplingCLDAltInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::EvaluateAtContinuousIndex(
                const ContinuousIndexType & /*index*/) const
        {
            itkExceptionMacro("CloudMaskUndersamplingCLDAltInterpolateImageFunction with VectorImage<> EvaluateAtContinuousIndex not implemented!!!!")
            OutputType output;
            return (output);
        }
    template<typename TPixel, unsigned int VImageDimension, class TCoordRep>
         void
         CloudMaskUndersamplingCLDAltInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::PrintSelf(std::ostream& os,
                 itk::Indent indent) const
         {
             Superclass::PrintSelf(os, indent);
             os << indent << "SuccesOutputValue: " << m_SuccesOutputValue << std::endl;
             os << indent << "FailureOutputValue: " << m_FailureOutputValue << std::endl;
         }


} //namespace vns

#endif
