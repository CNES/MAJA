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
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2299-CNES : 27 juillet 2017 : Seuillage WaterVapor                   *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-2090-CNES : 12 decembre 2016 : Correction utilisation du VAP NoData   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsWaterAmountImageGenerator_h
#define __vnsWaterAmountImageGenerator_h

#include "itkImageToImageFilter.h"
#include "vnsLookUpTable.h"
#include "vnsMacro.h"

namespace vns
{

    /** \class WaterAmountImageGenerator
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

    template<class TTOAInputImage, class TOutputImage, class TLUT = LookUpTable<double, 2> >
        class WaterAmountImageGenerator : public itk::ImageToImageFilter<TTOAInputImage, TOutputImage>
        {
        public:

            /** Standard typedefs */
            typedef WaterAmountImageGenerator Self;
            typedef itk::ImageToImageFilter<TTOAInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(WaterAmountImageGenerator,itk::ImageToImageFilter)

            /** Input TOA image template typedefs */
            typedef TTOAInputImage TOAInputImageType;
            typedef typename TOAInputImageType::PixelType TOAInputPixelType;
            typedef typename TOAInputImageType::Pointer TOAInputImagePointerType;
            typedef typename TOAInputImageType::ConstPointer TOAInputImageConstPointerType;

            /** Output image template typedefs */
            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::PixelType OutputPixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;

            /** Look up table template typedefs */
            typedef TLUT LUTType;
            typedef typename LUTType::PixelType LUTPixelType;
            typedef typename LUTType::PointType LUTPointType;
            typedef typename LUTPointType::ValueType LUTPointValueType;

            /** TOA input setter. */
            vnsSetGetInputMacro( IPTOA, TOAInputImageType, 0)

            /** Overload GetOutput() */
            OutputImageType *
            GetIPVAP()
            {
                return this->GetOutput();
            }
            OutputImageType *
            GetIPVAP() const
            {
                return this->GetOutput();
            }

            /** Set the reduced look up table. */
            itkSetObjectMacro(LUT, LUTType)

            /** Get the reduced look up table. */
            itkGetObjectMacro(LUT, LUTType)

            /** Set the channel of the 865 nm wavelength. */
            itkSetMacro(WaterVaporReferenceBandId, unsigned int)

            /** Get the channel of the 865 nm wavelength. */
            itkGetConstMacro(WaterVaporReferenceBandId, unsigned int)

            /** Set the channel of the 910 nm wavelength. */
            itkSetMacro(WaterVaporBandId, unsigned int)

            /** Get the channel of the 910 nm wavelength. */
            itkGetConstMacro(WaterVaporBandId, unsigned int)

            /** Set the No_data value. */
            itkSetMacro(VAPNodataValue, OutputPixelType)

            /** Set the No_data value. */
            itkGetConstMacro(VAPNodataValue, OutputPixelType)

			/** Set the No_data value. */
			itkSetMacro(MaxThreshold, OutputPixelType)

			/** Set the No_data value. */
			itkGetConstMacro(MaxThreshold, OutputPixelType)


        protected:
            WaterAmountImageGenerator();
            virtual
            ~WaterAmountImageGenerator()
            {
            }

            /** Overload GenerateOutputInformation method. */
            virtual void
            GenerateOutputInformation();

            /** Overload ThreadedGenerateData method. */
            virtual void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** Overload PrintSelf method. */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            WaterAmountImageGenerator(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Look up table */
            typename LUTType::Pointer m_LUT;
            /** Channel of the 865 nm wavelength, first channel has the id 0. */
            unsigned int m_WaterVaporReferenceBandId;
            /** Channel of the 910 nm wavelength, first channel has the id 0. */
            unsigned int m_WaterVaporBandId;
            /** Maximum threshold for water amount */
            OutputPixelType m_MaxThreshold;


            /** VAP No_Data value */
            OutputPixelType m_VAPNodataValue;

        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsWaterAmountImageGenerator.txx"
#endif

#endif
