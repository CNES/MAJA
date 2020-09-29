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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsComputeFinalResultEnvironmentCorrectionImageFilter.h 9316 2015-01-12 08:25:12Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputeFinalResultEnvironmentCorrectionImageFilter_h
#define __vnsComputeFinalResultEnvironmentCorrectionImageFilter_h

#include "itkImageToImageFilter.h"
#include "vnsMacro.h"

namespace vns
{

    /** \class ComputeFinalResultEnvironmentCorrectionImageFilter
     * \brief This class computes a water vapour computation.
     *
     * To proceed, we use a LUTh2o interpolation.
     *
     * Inputs are:
     * \begin{itemize}
     *    \item TOA image : otbVectorImage
     * \end{itemize}
     * Output is the water amount image (otbImage).
     *
     *
     * The filter needs other information:
     * \begin{itemize}
     * \item The 910nm band id : m_WaterVaporBand2Id
     * \item The 865nm band id : m_WaterVaporBand1Id
     * \item The LUT.
     * \end{itemize}
     *
     * \ingroup Radiometry
     *
     */

    template<class TInputImage, class TInputMask, class TOutputImage>
        class ComputeFinalResultEnvironmentCorrectionImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:

            /** Standard typedefs */
            typedef ComputeFinalResultEnvironmentCorrectionImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ComputeFinalResultEnvironmentCorrectionImageFilter,itk::ImageToImageFilter)

            /** Input TOA image template typedefs */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::PixelType InputPixelType;
            typedef typename InputImageType::ConstPointer InputImageConstPointerType;

            /** Output image template typedefs */
            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::PixelType OutputPixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputPixelType::ValueType InternalPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::PixelType InputMaskPixelType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointerType;

            vnsSetGetImageMacro(Toc, InputImageType, 0)
            vnsSetGetImageMacro(RhoEnv, InputImageType, 1)
            vnsSetGetImageMacro(Tdir, InputImageType, 2)
            vnsSetGetImageMacro(Tdif, InputImageType, 3)
            vnsSetGetImageMacro(Satm, InputImageType, 4)
            vnsSetGetImageMacro(Edg, InputMaskType, 5)

            /** Set the No_data value. */
            vnsMemberAndSetMacro(NoData, InternalPixelType)

        protected:
            ComputeFinalResultEnvironmentCorrectionImageFilter()
            {
                this->SetNumberOfRequiredInputs(6);
                this->SetNumberOfIndexedInputs(6);
                m_NoData = static_cast<InternalPixelType>(0);

            }
            virtual
            ~ComputeFinalResultEnvironmentCorrectionImageFilter()
            {
            }

            /** Overload ThreadedGenerateData method. */
            virtual void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** Overload PrintSelf method. */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const
            {
                Superclass::PrintSelf(os, indent);
            }

        private:
            ComputeFinalResultEnvironmentCorrectionImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

}
// end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsComputeFinalResultEnvironmentCorrectionImageFilter.txx"
#endif

#endif
