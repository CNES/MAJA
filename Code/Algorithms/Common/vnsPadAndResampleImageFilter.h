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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 24 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Modification pour corriger l'anomalie     *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-8-0 : DM : LAIG-DM-MAC-1638-CNES : 1 mars 2016 : Nouveau filtre reechantillonnage issu OTB   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 01 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsPadAndResampleImageFilter.h 9591 2015-03-20 17:29:33Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsPadAndResampleImageFilter_h
#define __vnsPadAndResampleImageFilter_h

#include "vnsGridResampleImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkZeroFluxNeumannPadImageFilter.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsResamplerHelper.h"
#include "itkProcessObject.h"
#include "itkImageToImageFilter.h"
#include "itkIdentityTransform.h"

namespace vns
{

    /** \class PadAndResampleImageFilter
     * \brief chain pad -> resample
     *
     * \sa PadImageFilter
     * \sa StreamingResampleImageFilter
     *
     * \ingroup Common
     */
    template<class TInputImage, class TOutputImage>
        class PadAndResampleImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef PadAndResampleImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(PadAndResampleImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::PixelType InputPixelType;

            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::PixelType OutputPixelType;

            /** PadFilter used to manipulate real data */
            typedef itk::ZeroFluxNeumannPadImageFilter<InputImageType, InputImageType> PadFilterType;
            typedef typename PadFilterType::Pointer PadFilterPointer;

            /** Expand only real images */
            //typedef otb::StreamingResampleImageFilter<InputImageType, OutputImageType> ResamplerFilterType;
            typedef GridResampleImageFilter<InputImageType, OutputImageType> ResamplerFilterType;
            typedef typename ResamplerFilterType::Pointer ResamplerFilterPointer;
            typedef typename ResamplerHelper::InterpolatorType InterpolatorType;

            typedef itk::IdentityTransform<double, InputImageType::ImageDimension> IdentityTransformType;

            vnsMemberAndSetAndGetConstReferenceMacro(Interpolator, InterpolatorType)

            // Input 0
            vnsSetGetInputRawMacro(Input0, InputImageType , 0)

			//Set the radius of the padding (0 desactivated)
			vnsSetMacro(PadRadius, unsigned int);
            vnsGetConstMacro(PadRadius, unsigned int);

            virtual OutputImageType *
            GetOutput(void)
            {
                return m_OutputResampler; //m_Resampler->GetOutput();
            }

            vnsMemberAndDirectSetAndGetConstReferenceMacro(AreaToOutputImageResolution, AreaType)

            /** Composite Filter method. */
            virtual void
            UpdateWithCaching();

            // Modifiable !!
            itkGetObjectMacro(Resampler, ResamplerFilterType)

        protected:
            PadAndResampleImageFilter();
            virtual
            ~PadAndResampleImageFilter()
            {
            }

        private:
            PadAndResampleImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            PadFilterPointer m_Pader;
            ResamplerFilterPointer m_Resampler;
            OutputImagePointer m_OutputResampler;
            unsigned int m_PadRadius;

        };

#define vnsImagePadAndResamplerMacro(comment, resampler, inputPointer, interpolator, output_area )  \
        { \
            /* First, get input image parameters */ \
            if (!(inputPointer) )\
            { \
                vnsStaticExceptionBusinessMacro("Internal error: In the ResamplerHelper, the input image pointer is null.") \
            } \
            vnsLogDebugMacro("Setting padding and resampling for " << (comment) <<"...") \
            (resampler)->SetInput0(inputPointer); \
            (resampler)->SetInterpolator(interpolator); \
            (resampler)->SetAreaToOutputImageResolution(output_area); \
            (resampler)->UpdateWithCaching(); \
            vnsLogDebugMacro("Setting padding and resampling for " << (comment) <<" done.") \
        }

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsPadAndResampleImageFilter.txx"
#endif

#endif

