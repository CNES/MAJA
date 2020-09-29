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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 27 jan. 2014 : Utilisation du filtre de calcul de l'angle  *
 *                                                              d'incidence solaire                         *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 19 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0  : 19 oct. 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskDeterminationImageFilter_h
#define __vnsSnowMaskDeterminationImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  SnowMaskDeterminationImageFilter
     * \brief This class is used to detect Snow in S2 and Landsat Images, using the SWIR bands.
     *
     * Caution: m_UseWaterMask is set to True is mode != Init and useWaterMask == True
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */

    template<class TInputVectorImage, class TInputMask, class TInputIncidenceAngleImage, class TOutputMask>
        class SnowMaskDeterminationImageFilter : public itk::ImageToImageFilter<TInputVectorImage, TOutputMask>
        {
        public:
            /** Standard class typedefs. */
            typedef SnowMaskDeterminationImageFilter Self;
            typedef itk::ImageToImageFilter<TInputVectorImage, TOutputMask> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(SnowMaskDeterminationImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType InputImageRegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename Superclass::OutputImageType OutputMaskType;
            typedef typename OutputMaskType::Pointer OutputMaskPointer;
            typedef typename OutputMaskType::PixelType OutputMaskPixelType;
            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;

            typedef TInputIncidenceAngleImage InputIncidenceAngleImageType;
            typedef typename InputIncidenceAngleImageType::Pointer       InputIncidenceAngleImagePointerType;
            typedef typename InputIncidenceAngleImageType::ConstPointer  InputIncidenceAngleImageConstPointer;
            typedef typename InputIncidenceAngleImageType::RegionType    InputIncidenceAngleImageRegionType;
            typedef typename InputIncidenceAngleImageType::PixelType     InputIncidenceAngleImagePixelType;

            typedef typename itk::ImageRegionConstIterator<InputMaskType> InputMaskConstIteratorType;
            typedef typename itk::ImageRegionConstIterator<InputImageType> InputImageConstIteratorType;
            typedef typename itk::ImageRegionConstIterator<InputIncidenceAngleImageType> InputIncidenceAngleImageConstIteratorType;
            typedef typename itk::ImageRegionIterator<OutputMaskType> OutputMaskIteratorType;

            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro(IPTOCR, InputImageType, 0)
            vnsSetGetInputMacro(IncidenceAngle, InputIncidenceAngleImageType, 2)
            vnsSetGetInputMacro(IPNotValidMaskSub, InputMaskType, 1)
            vnsSetGetInputMacro(L2inWAS, InputMaskType, 3)

            // Green Band parameter accesors
            itkSetMacro(RefBand, unsigned int)

            // SWIR Band parameter accesors
            itkSetMacro(AbsBand, unsigned int)

            // Red Band parameter accesors
            itkSetMacro(RedBand, unsigned int)

            // NDSI thresold parameter accesors
            itkSetMacro(NDSIThreshold, double)

            // Red reflectance threshold parameter accesors
            itkSetMacro(RedThreshold, double)

			// Is the swir band availble
			itkSetMacro(HasSWIR, bool)

            // SWIR reflectance threshold parameter accesors
            itkSetMacro(SWIRThreshold, double)

            // ThetaS parameter accesors
            itkSetMacro(ThetaS, double)

            // minimum incidence angle parameter accesors
            itkSetMacro(MinCosI, double)

            // UseWaterMask parameter accesors
            itkSetMacro(UseWaterMask, bool)

			// UseIncidenceAngles parameter accesors
			itkSetMacro(UseIncidencesAngles, bool)

        protected:
            /** Constructor */
            SnowMaskDeterminationImageFilter();
            /** Destructor */
            virtual
            ~SnowMaskDeterminationImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;
            /** Validate the presence of all inputs. If at least one input
             * is missing, it throws an exception. */
            void BeforeThreadedGenerateData();
            /** Multi-thread version GenerateData. */
            void
            ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

        private:
            SnowMaskDeterminationImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            unsigned int m_RefBand;
            unsigned int m_AbsBand;
            unsigned int m_RedBand;
            double m_NDSIThreshold;
            double m_RedThreshold;
            bool m_HasSWIR;
            double m_SWIRThreshold;
            double m_ThetaS;
            double m_MinCosI;
            bool m_UseWaterMask;
            bool m_UseIncidencesAngles;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsSnowMaskDeterminationImageFilter.txx"
#endif

#endif /* __vnsSnowMaskDeterminationImageFilter_h */
