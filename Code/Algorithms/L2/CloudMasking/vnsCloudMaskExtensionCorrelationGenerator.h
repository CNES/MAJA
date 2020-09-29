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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-1-2 : FA : LAIG-FA-MAC-1180-CNES : 19 mars 2015 : Pas de corrélation si date courante = date STO	*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskExtensionCorrelationGenerator_h
#define __vnsCloudMaskExtensionCorrelationGenerator_h

#include "itkProcessObject.h"
#include "vnsMacro.h"

#include "vnsCloudAlreadyDetectedImageFilter.h"
#include "vnsCloudMaskExtensionCorrelationImageFilter.h"

namespace vns
{
    /** \class  CloudMaskExtensionCorrelationGenerator
     * \brief This class implements .
     *
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        class CloudMaskExtensionCorrelationGenerator : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef CloudMaskExtensionCorrelationGenerator Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(CloudMaskExtensionCorrelationGenerator, itk::ProcessObject )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            typedef typename std::map<LabelImagePixelType, bool> BoolMapType;

            // Internal filters definition
            typedef CloudAlreadyDetectedImageFilter<InputMaskType, LabelImageType> CloudFilterType;
            typedef typename CloudFilterType::Pointer CloudFilterPointer;

            typedef CloudMaskExtensionCorrelationImageFilter<InputImageType, InputMaskType, LabelImageType, OutputImageType> CloudMaskExtensionFilterType;
            typedef typename CloudMaskExtensionFilterType::Pointer CloudMaskExtensionPointer;

            typedef typename itk::Size<InputImageType::ImageDimension> RadiusSizeType;

            typedef double CorrelationPixelType;

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

            // Label count accessors
            itkSetMacro(LabelCount, unsigned long)
            itkGetConstReferenceMacro(LabelCount, unsigned long)

            // Correlation Band parameter accesors band TOCR
            itkSetMacro(CorrelBandTOCR, unsigned int)
            itkGetConstReferenceMacro(CorrelBandTOCR, unsigned int)

            // Correlation Band parameter accesors band RCR
            itkSetMacro(CorrelBandRCR, unsigned int)
            itkGetConstReferenceMacro(CorrelBandRCR, unsigned int)

            // CorrelThreshold parameter accesors
            itkSetMacro(CorrelThreshold, double)
            itkGetConstReferenceMacro(CorrelThreshold, double)

            // MinPixelCorrelation parameter accesors
            itkSetMacro(MinPixelCorrelation, unsigned long)
            itkGetConstReferenceMacro(MinPixelCorrelation, unsigned long)

            // Product julian date accessors
            itkSetMacro(JDay, double)
            itkGetConstReferenceMacro(JDay, double)

            //STO list of dates
            void
            SetSTOListOfDates(Utilities::ListOfDoubles &listOfDates)
            {
                m_STOListOfDates = listOfDates;
                m_CloudMaskExtensionFilter->SetSTOListOfDates(listOfDates);
            }
            const Utilities::ListOfDoubles &
            GetSTOListOfDates(void)
            {
                return m_STOListOfDates;
            }

            /** Set the image of the cloud mask extension */
            vnsSetGetInputMacro( IPCLDExt, InputMaskType, 0)
            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro( IPTOCR, InputImageType, 1)
            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inRCR, InputImageType, 2)
            /** Set the stack of previous surface reflectance images corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inSTO, InputImageType, 3)
            /** Set the input cloud mask obtained by reflectance threshold */
            vnsSetGetInputMacro( IPCLDRefl, InputMaskType, 4)
            /** Set the input cloud mask obtained by reflectance variation threshold */
            vnsSetGetInputMacro( IPCLDReflVar, InputMaskType, 5)
            /** Set the image of labels of the cloud mask extension */
            vnsSetGetInputMacro( IPCLDExtLabel, LabelImageType, 6)
            /** Set the edge mask of the input product */
            vnsSetGetInputMacro( IPEDG, InputMaskType, 7)
            /** Set the no_data mask of the composite product */
            vnsSetGetInputMacro( L2inNDT, InputMaskType, 8)

            /* Outputs */
            /** Get the final extension of the cloud mask */
            itkGetConstObjectMacro(IPCLDExtOutput, OutputImageType)
            itkGetObjectMacro(IPCLDExtOutput, OutputImageType)


            itkGetConstObjectMacro(IPCLDExtCorelFlagOutput, OutputImageType)
            itkGetObjectMacro(IPCLDExtCorelFlagOutput, OutputImageType)

            void
            UpdateWithCaching(void);

        protected:
            /** Constructor */
            CloudMaskExtensionCorrelationGenerator();
            /** Destructor */
            virtual
            ~CloudMaskExtensionCorrelationGenerator();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            CloudMaskExtensionCorrelationGenerator(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            void
            CheckInputs();

            /** No_data value */
            RealNoDataType m_NoData;

            /** Declaration of the correlation band for TOCR */
            unsigned int m_CorrelBandTOCR;
            /** Declaration of the correlation band for RCR */
            unsigned int m_CorrelBandRCR;
            /** Radius of the neighborhood iterator declaration */
            RadiusSizeType m_NCorrel;
            /** Number of labels declaration */
            unsigned long m_LabelCount;

            /** Declaration of the threshold on correlation between images of date D and D-1 */
            double m_CorrelThreshold;
            /** Declaration of the minimum number of pixels to take correlation into account */
            unsigned long m_MinPixelCorrelation;

            /** Date of the input image declaration */
            double m_JDay;
            /** List of dates in STO image */
            Utilities::ListOfDoubles m_STOListOfDates;

            // Filter declarations
            CloudFilterPointer m_DetectedCloudFilter;
            CloudMaskExtensionPointer m_CloudMaskExtensionFilter;

            /* Outputs */
            OutputImagePointer m_IPCLDExtOutput;

            OutputImagePointer m_IPCLDExtCorelFlagOutput;

            // Pointer MapCloud
            BoolMapType m_CloudFoundMap;
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsCloudMaskExtensionCorrelationGenerator.txx"
#endif

#endif /* __vnsCloudMaskExtensionCorrelationGenerator_h */
