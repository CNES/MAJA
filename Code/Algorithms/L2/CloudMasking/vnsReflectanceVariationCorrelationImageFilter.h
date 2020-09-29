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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 avril 2016 : Modification de l’algorithme de detection   *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 1-0-0-3 : DM : 357 : 31 août 2011 : pas de correlation avec la date courante                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsReflectanceVariationCorrelationImageFilter_h
#define __vnsReflectanceVariationCorrelationImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNeighborhoodAlgorithm.h"

#include "vnsMacro.h"
#include "vnsCrossCorrelationFunctor.h"
#include "vnsUtilities.h"

namespace vns
{
    /** \class  ReflectanceVariationCorrelationImageFilter
     * \brief This class generates the reflectance variation threshold cloud mask.
     *
     * For each pixel, this class compares the surface reflectance of date D in the 443nm band to
     * the composite image of cloud free reflectance.
     * First, a pixel is declared cloudy if the reflectance variation is greater than a thershold that
     * depends on time.
     * Second, a correlation between a neighborhood around a pixel for date D and the same window for
     * the composite of date D-1 is computed. The neighborhood is also correlated with the stack of
     * previous images. If one of the correlation coefficients is above a threshold, the neighborhood
     * looks like one of the previous images. So, the pixel is not declared cloudy.
     * This filter is templated by four types:
     * - otbVectorImage for images of reflectance
     * - otbImage for input masks
     * - otbImage for image of date
     * - otbImage for output masks
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     */
    template<class TInputImage, class TInputMask, class TDateImage, class TOutputImage>
        class ReflectanceVariationCorrelationImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ReflectanceVariationCorrelationImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ReflectanceVariationCorrelationImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::IndexType InputImageIndexType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TDateImage DateImageType;
            typedef typename DateImageType::Pointer DateImagePointer;
            typedef typename DateImageType::ConstPointer DateImageConstPointer;
            typedef typename DateImageType::PixelType DateImagePixelType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            /*
             typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>   ImageBoundaryFacesType;
             typedef typename ImageBoundaryFacesType::FaceListType   ImageFaceListType;
             typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputMaskType>    MaskBoundaryFacesType;
             typedef typename MaskBoundaryFacesType::FaceListType    MaskFaceListType;
             */
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

            // Blue band index accessors for TOCR
            itkSetMacro(BlueBandTOCR, unsigned int)
            itkGetConstReferenceMacro(BlueBandTOCR, unsigned int)

            // Red band index accessors for TOCR
            itkSetMacro(RedBandTOCR, unsigned int)
            itkGetConstReferenceMacro(RedBandTOCR, unsigned int)

            // NIR band index accessors for TOCR
            itkSetMacro(NIRBandTOCR, unsigned int)
            itkGetConstReferenceMacro(NIRBandTOCR, unsigned int)

            // Blue band index accessors for RCR
            itkSetMacro(BlueBandRCR, unsigned int)
            itkGetConstReferenceMacro(BlueBandRCR, unsigned int)

            // Red band index accessors for RCR
            itkSetMacro(RedBandRCR, unsigned int)
            itkGetConstReferenceMacro(RedBandRCR, unsigned int)

            // NIR band index accessors for RCR
            itkSetMacro(NIRBandRCR, unsigned int)
            itkGetConstReferenceMacro(NIRBandRCR, unsigned int)

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

            // Minimum variation threshold accessors
            itkSetMacro(MinVarThreshold, double)
            itkGetConstReferenceMacro(MinVarThreshold, double)

            // Minimum variation threshold accessors
            itkSetMacro(MaxVarThreshold, double)
            itkGetConstReferenceMacro(MaxVarThreshold, double)

            // KNIRBlue parameter accessors
            itkSetMacro(KNIRBlue, double)
            itkGetConstReferenceMacro(KNIRBlue, double)

            // KRedBlue parameter accessors
            itkSetMacro(KRedBlue, double)
            itkGetConstReferenceMacro(KRedBlue, double)

            // DeltaThreshold parameter accesors
            itkSetMacro(DeltaThreshold, double)
            itkGetConstReferenceMacro(DeltaThreshold, double)

            // MinPixelCorrelation parameter accesors
            itkSetMacro(MinPixelCorrelation, unsigned long)
            itkGetConstReferenceMacro(MinPixelCorrelation, unsigned long)

            // Correlation Band parameter accesors band TOCR
            itkSetMacro(CorrelBandTOCR, unsigned int)
            itkGetConstReferenceMacro(CorrelBandTOCR, unsigned int)

            // Correlation Band parameter accesors band RCR
            itkSetMacro(CorrelBandRCR, unsigned int)
            itkGetConstReferenceMacro(CorrelBandRCR, unsigned int)

            // CorrelThreshold parameter accesors
            itkSetMacro(CorrelThreshold, double)
            itkGetConstReferenceMacro(CorrelThreshold, double)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro( IPTOCR , InputImageType , 0)
            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inRCR , InputImageType , 1 )
            /** Set the stack of previous surface reflectance images corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inSTO , InputImageType , 2 )
            /** Set the image of date of the composite product */
            vnsSetGetInputMacro( L2inPXD , DateImageType , 3 )
            /** Set the edge mask of the input product */
            vnsSetGetInputMacro( IPEDG , InputMaskType , 4)
            /** Set the no_data mask of the composite product */
            vnsSetGetInputMacro( L2inNDT , InputMaskType , 5 )
            /** Set the water and snow masks of the composite product */
            vnsSetGetInputMacro( ConditionalMask , InputMaskType , 6)
            /** Set the reflectance threshold cloud mask */
            vnsSetGetInputMacro( IPCLDRefl , InputMaskType , 7)

            /** Get the cloud mask of reflectance variation */
            OutputImageType*
            GetIPCLDReflVarOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
            }

            /** Get the extension of the cloud mask */
            OutputImageType*
            GetIPCLDExtOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(1));
            }

            /** Get the extension of the cloud mask */
            OutputImageType*
            GetIPCLDReflOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(2));
            }



        protected:
            /** Constructor */
            ReflectanceVariationCorrelationImageFilter();
            /** Destructor */
            virtual
            ~ReflectanceVariationCorrelationImageFilter();

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
            ReflectanceVariationCorrelationImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** No_data value */
            RealNoDataType m_NoData;

            /** Blue band definition for TOCR*/
            unsigned int m_BlueBandTOCR;
            /** Red band definition  for TOCR*/
            unsigned int m_RedBandTOCR;
            /** NIR band definition  for TOCR*/
            unsigned int m_NIRBandTOCR;

            /** Blue band definition for RCR*/
            unsigned int m_BlueBandRCR;
            /** Red band definition  for RCR*/
            unsigned int m_RedBandRCR;
            /** NIR band definition  for RCR*/
            unsigned int m_NIRBandRCR;

            /** Date of the input image declaration */
            double m_JDay;
            /** List of dates in STO image */
            Utilities::ListOfDoubles m_STOListOfDates;
            /** Declaration of time delay after which the reflectance variation threshold is doubled */
            double m_ForgettingDuration;
            /** Declaration of the minimum threshold on reflectance variation between date D and D-1 */
            double m_MinVarThreshold;
            /** Declaration of the maximum threshold on reflectance variation between date D and D-1 */
            double m_MaxVarThreshold;
            /** Declaration of parameter for comparison of relative variations in the blue and in the NIR */
            double m_KNIRBlue;
            /** Declaration of parameter for comparison of variations in the blue and in the red */
            double m_KRedBlue;
            /** Declaration of the difference between the threshold used for the initial variation detection step
             * and the one used for the extension step */
            double m_DeltaThreshold;
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
#include "vnsReflectanceVariationCorrelationImageFilter.txx"
#endif

#endif /* __vnsReflectanceVariationCorrelationImageFilter_h */

