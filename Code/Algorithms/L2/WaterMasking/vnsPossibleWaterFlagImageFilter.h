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
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1514-CNES : 29 juillet 2016 : Ajout NoData                            *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 19 décembre 2013 : Améliorations algorithmiques			*
 * 										Ajout d'un seuillage sur la pente et la réflectance dans le rouge   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsPossibleWaterFlagImageFilter_h
#define __vnsPossibleWaterFlagImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

#include "otbVegetationIndicesFunctor.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  PossibleWaterFlagImageFilter
     * \brief This class compute the possible water flag for image for date D.
     *
     *  Water is first detected by its very low NDVI. But a pixel may be dark in
     *  the NIR band either because of water or because  it is shadowed
     *  (by a cloud or by topogrpahy).
     *
     *  Then, near the sunglint direction, the reflectance of water pixels is changing
     *  from day to day because the reflectance is very sensitive to small alngle
     *  variations and to water surface roughness due to wind. A pixel observed in
     *  this direction will then be detected as "possible water" if its reflectance
     *  changes quickly from day to day.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TImage, class TOutputImage>
        class PossibleWaterFlagImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef PossibleWaterFlagImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(PossibleWaterFlagImageFilter, ImageToImageFilter )


            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::InternalPixelType InputImageInternalPixelType;
            typedef typename InputImageType::SizeType SizeType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TImage ImageType;
            typedef typename ImageType::Pointer ImageTypePointer;
            typedef typename ImageType::ConstPointer ImageTypeConstPointer;
            typedef typename ImageType::PixelType ImageTypePixelType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;

            typedef itk::ImageRegionConstIterator<InputImageType> InputImageConstIterator;
            typedef itk::ImageRegionConstIterator<InputMaskType> MaskConstIterator;
            typedef itk::ImageRegionConstIterator<ImageType> ImageConstIterator;

            typedef itk::ImageRegionIterator<OutputImageType> OutputMaskIterator;

            typedef otb::Functor::NDVI<InputImageInternalPixelType, InputImageInternalPixelType> NDVIFunctorType;

            // Init mode parameter accessors
            itkSetMacro(InitMode, bool)
            itkGetConstReferenceMacro(InitMode, bool)

            // Sunglint flag parameter accessors
            itkSetMacro(SunglintFlag, bool)
            itkGetConstReferenceMacro(SunglintFlag, bool)

            // Red band index accessors for the TOCR
            itkSetMacro(RedBandTOCR, unsigned int)
            itkGetConstReferenceMacro(RedBandTOCR, unsigned int)

            // NIR band index accessors for TOCR
            itkSetMacro(NIRBandTOCR, unsigned int)
            itkGetConstReferenceMacro(NIRBandTOCR, unsigned int)

            // NIR band index accessors for RCR
            itkSetMacro(NIRBandRCR, unsigned int)
            itkGetConstReferenceMacro(NIRBandRCR, unsigned int)


            // Water NDVI threshold accessors
            itkSetMacro(WaterNDVIThreshold, double)
            itkGetConstReferenceMacro(WaterNDVIThreshold, double)

            // Water reflectance variation threshold accessors
            itkSetMacro(WaterReflectanceVariationThreshold, double)
            itkGetConstReferenceMacro(WaterReflectanceVariationThreshold, double)

            // Maximum sunglint reflectance accessors
            itkSetMacro(MaximumSunglintReflectance, double)
            itkGetConstReferenceMacro(MaximumSunglintReflectance, double)

            // DEM slope threshold accessors
            itkSetMacro(WaterSlopeThreshold, double)
            itkGetConstReferenceMacro(WaterSlopeThreshold, double)

            // Red reflectance threshold accessors
            itkSetMacro(WaterRedReflectanceThreshold, double)
            itkGetConstReferenceMacro(WaterRedReflectanceThreshold, double)

            vnsMemberAndSetMacro(RealL2NoData, RealNoDataType)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro( IPTOCR, InputImageType, 0)
            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inRCR, InputImageType, 5)
            /** Set the edge mask of the input product */
            vnsSetGetInputMacro( IPEDG, InputMaskType, 1)
            /** Set the cast shadow mask */
            vnsSetGetInputMacro( IPSHDSub, InputMaskType, 2)

            /** Set the cloud mask (summary mask) */
            vnsSetGetInputMacro( L2CLDSubSummary, InputMaskType, 3)

            /** Set the terrain slope in degrees at L2 coarse resolution */
            vnsSetGetInputMacro( SlopeSub, ImageType, 4)

            /** Get the possible water mask */
            OutputImageType*
            GetIPPWAOutput()
            {
                return static_cast<OutputImageType*>(this->GetOutput(0));
            }

            /** Get the tested pixel mask */
            OutputImageType*
            GetIPTWAOutput()
            {
                return static_cast<OutputImageType*>(this->GetOutput(1));
            }

        protected:
            /** Constructor */
            PossibleWaterFlagImageFilter();
            /** Destructor */
            virtual
            ~PossibleWaterFlagImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Composite Filter method. */
            virtual void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

        private:
            PossibleWaterFlagImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            void
            LowNDVIDetection(const InputImageConstIterator & p_IPTOCRIt, OutputMaskIterator & p_IPPWAIt, OutputMaskIterator & p_IPTWAIt);

            /** Init_mode option declaration */
            bool m_InitMode;
            /** Sunglint Flag */
            bool m_SunglintFlag;

            /** Red band definition for TOCR */
            unsigned int m_RedBandTOCR;
            /** NIR band definition for TOCR */
            unsigned int m_NIRBandTOCR;
            /** NIR band definition for RCR */
            unsigned int m_NIRBandRCR;

            /** Water NDVI Threshold under which the pixel is probably covered by water */
            double m_WaterNDVIThreshold;
            /** Water reflectance variation threshold over which the pixel is probably covered by water */
            double m_WaterReflectanceVariationThreshold;
            /** Maximum reflectance parameter for which the variation threshold is used */
            double m_MaximumSunglintReflectance;
            /** DEM slope threshold */
            double m_WaterSlopeThreshold;
            /** Red reflectance threshold */
            double m_WaterRedReflectanceThreshold;

            NDVIFunctorType m_NDVIFunctor;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsPossibleWaterFlagImageFilter.txx"
#endif

#endif /* __vnsPossibleWaterFlagImageFilter_h */
