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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowVariationCorrelationImageFilter_h
#define __vnsShadowVariationCorrelationImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNeighborhoodAlgorithm.h"

#include "vnsMacro.h"
#include "vnsCrossCorrelationFunctor.h"
#include "vnsUtilities.h"

namespace vns
{
    /** \class  ShadowVariationCorrelationImageFilter
     * \brief This class detects shadows caused by clouds outside the image.
     *
     * WWhen a ratio of the reflectance of date D and the composite image of date D-1 is below
     * a threshold (for the red band), the pixel is considered as a shadow.
     * The threshold increase with time delay between the image acquisition and the reference
     * reflectance date.
     * The detection is not activated above water surfaces to avoid false detection of shadows.
     * It is only activated in the zone where clouds outside the image could cast shadows within image.
     *
     * The input images are otb::VectorImage (images of reflectance).
     * The input mask and the image of date are otb::Image.
     * The output mask is an otb::Image.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TOutputImage>
        class ShadowVariationCorrelationImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ShadowVariationCorrelationImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(ShadowVariationCorrelationImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            typedef itk::ConstNeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
            typedef itk::ConstNeighborhoodIterator<InputMaskType> MaskNeighborhoodIteratorType;
            typedef typename NeighborhoodIteratorType::RadiusType RadiusType;
            typedef typename itk::Size<InputImageType::ImageDimension> RadiusSizeType;

            typedef typename itk::ImageRegionIterator<InputMaskType> IteratorType;

            typedef double CorrelationPixelType;
            typedef vns::Functor::CrossCorrelationFunctor<NeighborhoodIteratorType, MaskNeighborhoodIteratorType, CorrelationPixelType> FunctorType;

            // Radius accesors
            itkSetMacro(NCorrel, RadiusSizeType)

            /** Set unsigned int radius */
            void
            SetNCorrel(unsigned int radius)
            {
                m_NCorrel.Fill(radius);
            }

            // No_data pixel value accessors
            itkSetMacro(NoData, RealNoDataType)
            itkGetConstReferenceMacro(NoData, RealNoDataType)

            // Red band index accessors for TOCR
            itkSetMacro(RedBandTOCR, unsigned int)
            itkGetConstReferenceMacro(RedBandTOCR, unsigned int)

            // Red band index accessors for RCR
            itkSetMacro(RedBandRCR, unsigned int)
            itkGetConstReferenceMacro(RedBandRCR, unsigned int)

            // Product julian date accessors
            itkSetMacro(JDay, double)
            itkGetConstReferenceMacro(JDay, double)

            //STO list of dates
            void
            SetSTOListOfDates(Utilities::ListOfDoubles &listOfDates)
            {
                m_STOListOfDates = listOfDates;
            }
            const Utilities::ListOfDoubles &
            GetSTOListOfDates(void)
            {
                return m_STOListOfDates;
            }
            // Forgetting duration accessors
            itkSetMacro(ForgettingDuration, double)
            itkGetConstReferenceMacro(ForgettingDuration, double)

            // Minimum shadow variation accessors
            itkSetMacro(MinShadowVariation, double)
            itkGetConstReferenceMacro(MinShadowVariation, double)

            // MinPixelCorrelation parameter accesors
            itkSetMacro(MinPixelCorrelation, unsigned long)
            itkGetConstReferenceMacro(MinPixelCorrelation, unsigned long)

            // Correlation Band parameter accesors for TOCR
            itkSetMacro(CorrelBandTOCR, unsigned int)
            itkGetConstReferenceMacro(CorrelBandTOCR, unsigned int)

            // Correlation Band parameter accesors for RCR
            itkSetMacro(CorrelBandRCR, unsigned int)
            itkGetConstReferenceMacro(CorrelBandRCR, unsigned int)

            // CorrelThreshold parameter accesors
            itkSetMacro(CorrelThreshold, double)
            itkGetConstReferenceMacro(CorrelThreshold, double)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro(IPTOCR , InputImageType, 0)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro(L2inRCR , InputImageType, 1)

            /** Set the stack of previous surface reflectance images corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inSTO,InputImageType , 2)

            /** Set the edge mask of the input product */
            vnsSetGetInputMacro( IPEDG, InputMaskType, 3)

            /** Set the no_data mask of the composite product */
            vnsSetGetInputMacro( L2inNDT, InputMaskType, 4)

            /** Set the water mask of the composite product */
            vnsSetGetInputMacro( L2inWAS, InputMaskType, 5)

            /** Set the input shadow var cloud mask */
            vnsSetGetInputMacro(IPCLDShadVar , InputMaskType, 6)

            /** Get the shadows mask from clouds outside image */
            OutputImageType*
            GetIPCLDShadVarOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
            }

        protected:
            /** Constructor */
            ShadowVariationCorrelationImageFilter();
            /** Destructor */
            virtual
            ~ShadowVariationCorrelationImageFilter();

            /** Pad the input requested region by radius */
            virtual void
            GenerateInputRequestedRegion(void);

            /** Set output image to 0 brfore processing */
            virtual void
            BeforeThreadedGenerateData();

            /** Multi-thread version GenerateData. */
            void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ShadowVariationCorrelationImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** No_data value */
            RealNoDataType m_NoData;

            /** Red band definition for TOCR */
            unsigned int m_RedBandTOCR;

            /** Red band definition for RCR */
            unsigned int m_RedBandRCR;

            /** Date of the input image declaration */
            double m_JDay;
            /** List of dates in STO image */
            Utilities::ListOfDoubles m_STOListOfDates;
            /** Declaration of time delay after which the reflectance variation threshold is doubled */
            double m_ForgettingDuration;
            /** Minimum shadow variation declaration */
            double m_MinShadowVariation;
            /** Declaration of the minimum number of pixels to take correlation into account */
            unsigned long m_MinPixelCorrelation;
            /** Declaration of the correlation band for TOCR */
            unsigned int m_CorrelBandTOCR;
            /** Declaration of the correlation band for RCR */
            unsigned int m_CorrelBandRCR;
            /** Declaration of the threshold on correlation between images of date D and D-1 */
            double m_CorrelThreshold;

            /** Radius of the neighborhood iterator declaration */
            RadiusSizeType m_NCorrel;

            // Vector used to define one functor by thread
            std::vector<FunctorType> m_FunctorVector;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowVariationCorrelationImageFilter.txx"
#endif

#endif /* __vnsShadowVariationCorrelationImageFilter_h */
