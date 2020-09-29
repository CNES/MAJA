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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 27 oct. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStereoAltitudeCorrectionImageFilter_h
#define __vnsStereoAltitudeCorrectionImageFilter_h

#include "itkImageToImageFilter.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  StereoAltitudeCorrectionImageFilter
     * \brief This class assigns an altitude to the cloudy pixel that have not been detected by
     * stereoscopy but with the radiometric test.
     * The average altitude within each label is assigned to this pixel and outliers are filtered.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TLabelImage, class TOutputImage>
        class StereoAltitudeCorrectionImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef StereoAltitudeCorrectionImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(StereoAltitudeCorrectionImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;

            // KsigmaAltitude parameter accesors
            itkSetMacro(KsigmaAltitude, unsigned short)
            ;itkGetConstReferenceMacro(KsigmaAltitude, unsigned short)

            // Mean parameter accesors
            //itkSetMacro(Mean, double);
            //itkGetConstReferenceMacro(Mean, double);

            // Sigma parameter accesors
            //itkSetMacro(Sigma, double);
            //itkGetConstReferenceMacro(Sigma, double);

            // Label parameter accesors
            //itkSetMacro(Label, LabelImagePixelType);
            //itkGetConstReferenceMacro(Label, LabelImagePixelType);

            // Sum the altitude of the pixel parameter accesors
            vnsSetMacro(LabelsAltitudeMean, double)
            vnsGetConstReferenceMacro(LabelsAltitudeMean, double)

            // Mean for each labels
            vnsSetMacro(LabelsMean, std::vector<double>)
            vnsGetConstReferenceMacro(LabelsMean, std::vector<double>);

        // Sigma for each label
        vnsSetMacro(LabelsSigma, std::vector<double>);
        vnsGetConstReferenceMacro(LabelsSigma, std::vector<double>);

        // List of labels
        vnsSetMacro(Labels, std::vector<unsigned long>);
        vnsGetConstReferenceMacro(Labels, std::vector<unsigned long>);

        // Number of labels
        vnsGetMacro(NumberOfLabels, unsigned int);

        /** Set the L2 stereoscopic cloud altitude image */
        vnsSetGetInputMacro( IPCLA , InputImageType , 0)
        /** Set the L2 stereoscopic cloud altitude image */
        vnsSetGetInputMacro( IPCLDAllLabel , LabelImageType , 1)

        /** Get the cloud altitude image */
        OutputImageType* GetIPCLAOutput()
        {
            return static_cast<OutputImageType*>( this->itk::ProcessObject::GetOutput(0) );
        }

    protected:
        /** Constructor */
        StereoAltitudeCorrectionImageFilter();
        /** Destructor */
        virtual ~StereoAltitudeCorrectionImageFilter();

        /** Before threaded generate data method */
        virtual void BeforeThreadedGenerateData();

        /** After threaded generate data method */
        virtual void AfterThreadedGenerateData();

        /** Composite Filter method. */
        virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

        /** PrintSelf method */
        virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
        StereoAltitudeCorrectionImageFilter(const Self&); //purposely not implemented
        void operator=(const Self&);//purposely not implemented

        /** Cloud altitudes are filtered to discard outliers.
         *  KsigmaAltitude is the factor that defines the strictness of this filter
         */
        unsigned short m_KsigmaAltitude;

        /** Mean and sigma parameters */
        double m_LabelsAltitudeMean;
        std::vector<double> m_LabelsAltitudeSumThreads;
        std::vector<unsigned long> m_LabelsAltitudeCountThreads;
        std::vector<double> m_LabelsMean;
        std::vector<double> m_LabelsSigma;
        std::vector<unsigned long> m_Labels;
        unsigned int m_NumberOfLabels;

    };

}
// End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsStereoAltitudeCorrectionImageFilter.txx"
#endif

#endif /* __vnsStereoAltitudeCorrectionImageFilter_h */
