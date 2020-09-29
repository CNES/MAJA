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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 07 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStereoCorrectionImageFilter_h
#define __vnsStereoCorrectionImageFilter_h

#include "vnsMaskingImageFunctor.h"
#include "itkImageToImageFilter.h"

#include "itkConnectedComponentImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImageFileReader.h"
#include "itkBinaryThresholdImageFilter.h"

#include "vnsStereoAltitudeCorrectionImageFilter.h"
#include "vnsConstantImageSource.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  StereoCorrectionImageFilter
     * \brief This class aims to assign an altitude to each pixel within each cloud label.
     * Up to now, the only clouds that have an known altitude are the clouds detected by
     * stereoscopy. The cloud pixels detected by the radiometric tests do not have any altitude.
     * This class assigns an altitude to each cloud pixel.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        class StereoCorrectionImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef StereoCorrectionImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(StereoCorrectionImageFilter, ImageToImageFilter)

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::SpacingType SpacingType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            // Labelize filter type
            typedef itk::ConnectedComponentImageFilter<InputMaskType, LabelImageType> LabelFilterType;
            typedef typename LabelFilterType::Pointer LabelFilterPointer;

            // Reader types for chaching
            typedef otb::ImageFileReader<LabelImageType> LabelReaderType;
            typedef typename LabelReaderType::Pointer LabelReaderPointer;

            typedef otb::ImageFileReader<OutputImageType> OutputReaderType;
            typedef typename OutputReaderType::Pointer OutputReaderPointer;

            // Change value filter type
            typedef Functor::MaskingImageFunctor<LabelImagePixelType, InputImagePixelType, LabelImagePixelType> FunctorType;

            typedef itk::BinaryFunctorImageFilter<LabelImageType, InputImageType, LabelImageType, FunctorType> ChangeValueFilterType;
            typedef typename ChangeValueFilterType::Pointer ChangeValueFilterPointer;

            typedef Functor::MaskingImageFunctor<OutputImagePixelType, LabelImagePixelType, OutputImagePixelType> FunctorType2;

            typedef itk::BinaryFunctorImageFilter<OutputImageType, LabelImageType, OutputImageType, FunctorType2> CLAChangeValueFilterType;
            typedef typename CLAChangeValueFilterType::Pointer CLAChangeValueFilterPointer;

            // Label statistics filter
            typedef itk::LabelStatisticsImageFilter<InputImageType, LabelImageType> LabelStatFilterType;
            typedef typename LabelStatFilterType::Pointer LabelStatFilterPointer;

            /** StereoAltitudeCorrectionImageFilter type */
            typedef StereoAltitudeCorrectionImageFilter<InputImageType, LabelImageType, OutputImageType> StereoAltitudeCorrectionImageFilterType;
            typedef typename StereoAltitudeCorrectionImageFilterType::Pointer StereoAltitudeCorrectionImageFilterPointer;

            // Constant Image filter type
            typedef ConstantImageSource<OutputImageType> ConstantFilterType;
            typedef typename ConstantFilterType::Pointer ConstantFilterPointer;

            // threshold image filter
            typedef itk::BinaryThresholdImageFilter<LabelImageType, OutputImageType> ThresholderType;
            typedef typename ThresholderType::Pointer ThresholderPointer;

            // Excluded value parameter accesors
            itkSetMacro(ExcludedValue, InputImagePixelType)
            itkGetConstReferenceMacro(ExcludedValue, InputImagePixelType)

            // Default value parameter accesors
            itkSetMacro(DefaultValue, LabelImagePixelType)
            itkGetConstReferenceMacro(DefaultValue, LabelImagePixelType)

            // KSigmaAltitude parameter accesors
            itkSetMacro(KsigmaAltitude, unsigned short)
            itkGetConstReferenceMacro(KsigmaAltitude, unsigned short)

            // DefaultAltitude parameter accesors
            itkSetMacro(DefaultAltitude, short)
            itkGetConstReferenceMacro(DefaultAltitude, short)

            /** Set the L2 stereoscopic cloud altitude image */
            vnsSetGetInputMacro( IPCLA, InputImageType, 0)
            /** Set the synthesis of radiometric and stereoscopic cloud masks */
            vnsSetGetInputMacro( IPCLDAll, InputMaskType, 1)

            /** Get the cloud altitude image */
            OutputImageType*
            GetIPCLAOutput()
            {
                return m_IPCLAOutput;
            }

            /** Get the image of label of the cloud mask */
            LabelImageType*
            GetIPCLDAllLabelOutput()
            {
                return m_IPCLDAllLabelOutput;
            }

            /** Composite Filter method. */
            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            StereoCorrectionImageFilter();
            /** Destructor */
            virtual
            ~StereoCorrectionImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            StereoCorrectionImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Parameters declaration */
            /** Excluded value in the image of cloud altitude */
            InputImagePixelType m_ExcludedValue;
            /** Default value in the image of all clouds labels */
            LabelImagePixelType m_DefaultValue;
            /** Cloud altitudes are filtered to discard outliers.
             *  KsigmaAltitude is the factor that defines the strictness of this filter
             */
            unsigned short m_KsigmaAltitude;
            /** Default altitude attribute to cloud pixels
             * if stereoscopy has not detected any cloud
             */
            short m_DefaultAltitude;

            /** Filters declaration */
            LabelFilterPointer m_LabelFilter;
            LabelStatFilterPointer m_LabelStatFilter;
            ChangeValueFilterPointer m_ChangeValueFilter;
            LabelStatFilterPointer m_ChangeLabelStatFilter;

            StereoAltitudeCorrectionImageFilterPointer m_StereoAltitudeFilter;

            std::vector<CLAChangeValueFilterPointer> m_CLAChangeValueFilter;

            ConstantFilterPointer m_CLADefaultConstantImageFilter;

            OutputImagePointer m_IPCLAOutput;
            LabelImagePointer m_IPCLDAllLabelOutput;

            ThresholderPointer m_Threshold;
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsStereoCorrectionImageFilter.txx"
#endif

#endif /* __vnsStereoCorrectionImageFilter_h */
