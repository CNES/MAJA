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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-116796-CS : 20 fevrier 2013 : Valeur du No_Data dans le plan PXD 		*
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 19 décembre 2013 : Améliorations algorithmiques			*
 * 								Suppression de la recherche des zones de forte AOT près des nuages			*
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 03 novembre 2011 : Modification pour prise en compte            *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 6 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCompositeImageUpdateImageFilter_h
#define __vnsCompositeImageUpdateImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

#include "itkVariableLengthVector.h"

#include "vnsMacro.h"

namespace vns
{
    /** \class  CompositeImageUpdateImageFilter
     * \brief This class implements .
     *
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
    template<class TInputImage, class TInputMask, class TVectorMask, class TDateImage, class TOutputImage>
        class CompositeImageUpdateImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef CompositeImageUpdateImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(CompositeImageUpdateImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;
            typedef typename InputImageType::SizeType SizeType;

            typedef itk::ImageRegionConstIterator<InputImageType> InputImageConstIteratorType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef itk::ImageRegionConstIterator<InputMaskType> InputMaskConstIteratorType;
            typedef itk::ImageRegionIterator<InputMaskType> InputMaskIteratorType;

            typedef TVectorMask VectorMaskType;
            typedef typename VectorMaskType::Pointer VectorMaskPointer;
            typedef typename VectorMaskType::ConstPointer VectorMaskConstPointer;
            typedef typename VectorMaskType::PixelType VectorMaskPixelType;
            typedef typename VectorMaskType::InternalPixelType VectorMaskInternalPixelType;

            typedef TDateImage DateImageType;
            typedef typename DateImageType::Pointer DateImagePointer;
            typedef typename DateImageType::ConstPointer DateImageConstPointer;
            typedef typename DateImageType::PixelType DateImagePixelType;
            typedef typename DateImageType::InternalPixelType DateInternalPixelType;

            typedef itk::ImageRegionConstIterator<DateImageType> DateImageConstIteratorType;
            typedef itk::ImageRegionIterator<DateImageType> DateImageIteratorType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;

            typedef itk::ImageRegionIterator<OutputImageType> OutputImageIteratorType;

            typedef itk::VariableLengthVector<VectorMaskInternalPixelType> VectorMaskVectorType;
            typedef itk::VariableLengthVector<OutputInternalPixelType> OutputVectorType;

            // Init mode accessors
            itkSetMacro(InitMode, bool)

            itkGetConstReferenceMacro(InitMode, bool)

            // Real No_data pixel value accessors
            itkSetMacro(RealL2NoData, RealNoDataType)
            itkGetConstReferenceMacro(RealL2NoData, RealNoDataType)

            // No_data pixel value accessors
            itkSetMacro(L2NoData, NoDataType)

            itkGetConstReferenceMacro(L2NoData, NoDataType)

            // Maximum AOT accessors
            itkSetMacro(MaxAOT, double)

            itkGetConstReferenceMacro(MaxAOT, double)

            // Maximum Age accessors
            itkSetMacro(MaxAge, unsigned short)

            itkGetConstReferenceMacro(MaxAge, unsigned short)

            // Hotspot flag accessors
            itkSetMacro(HotspotFlag, bool)

            itkGetConstReferenceMacro(HotspotFlag, bool)

            // Cirrus flag accessors
            itkSetMacro(CirrusFlag, bool)

            itkGetConstReferenceMacro(CirrusFlag, bool)

            // Rain flag accessors
            itkSetMacro(RainFlag, bool)

            itkGetConstReferenceMacro(RainFlag, bool)

            // Acquisition date in julian days
            itkSetMacro(JDay, short)

            itkGetConstReferenceMacro(JDay, short)

            // AOT mean accessors
            itkSetMacro(AOTMean, double)

            itkGetConstReferenceMacro(AOTMean, double)

            // STO band number accessors
            itkSetMacro(STOBandNumber, unsigned int)

            itkGetConstReferenceMacro(STOBandNumber, unsigned int)

            // STO band index accessors
            itkSetMacro(STOBandIndex, unsigned int)

            itkGetConstReferenceMacro(STOBandIndex, unsigned int)

            /** Set the image of the TOA reflectance corrected by absorption */
            vnsSetGetInputMacro( IPTOAC, InputImageType, 0)
            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro( IPTOCR, InputImageType, 1)
            /** Set the image of the surface reflectance */
            vnsSetGetInputMacro( IPTOC, InputImageType, 2)
            /** Set the Mask containing not valid pixel (edge and snow) of the input product
             * See the vnsCompositeImageFilter.txx to know all the mask contained in this one.*/
            vnsSetGetInputMacro( IPNotValidMaskSub, InputMaskType, 3)
            /** Set the summary cloud mask */
            vnsSetGetInputMacro( IPCLDSum, InputMaskType, 4)
            /** Set the saturated image of the input product */
            vnsSetGetInputMacro( IPSAT, VectorMaskType, 5)
            /** Set the high AOT mask */
            vnsSetGetInputMacro( HighAOTMask, InputMaskType, 6)

            /** Set the image of the TOA reflectance corrected by absorption
             * of the composite product */
            vnsSetGetInputMacro( L2inRTA, InputImageType, 7)
            /** Get the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inRCR, InputImageType, 8)
            /** Set the image of the surface reflectance of the composite product */
            vnsSetGetInputMacro( L2inRTC, InputImageType, 9)
            /** Set the stack of previous surface reflectance images corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inSTO, InputImageType, 10)
            /** Set the image of date of the composite product */
            vnsSetGetInputMacro( L2inPXD, DateImageType, 11)
            /** Set the no_data mask of the composite product */
            vnsSetGetInputMacro( L2inNDT, InputMaskType, 12)

            /** Get the image of the TOA reflectance corrected by absorption
             * of the composite product */
            OutputImageType *
            GetL2RTAOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
            }

            /** Get the image of the surface reflectance corrected by absorption and rayleigh */
            OutputImageType *
            GetL2RCROutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(1));
            }

            /** Get the image of the surface reflectance of the composite product */
            OutputImageType *
            GetL2RTCOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(2));
            }

            /** Get the stack of previous surface reflectance images corrected by absorption and rayleigh
             * of the composite product */
            OutputImageType *
            GetL2STOOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(3));
            }

            /** Get the image of date of the composite product */
            DateImageType *
            GetL2PXDOutput()
            {
                return static_cast<DateImageType*>(this->itk::ProcessObject::GetOutput(4));
            }

            /** Get the no_data mask of the composite product */
            InputMaskType *
            GetL2NDTOutput()
            {
                return static_cast<InputMaskType*>(this->itk::ProcessObject::GetOutput(5));
            }

        protected:
            /** Constructor */
            CompositeImageUpdateImageFilter();
            /** Destructor */
            virtual
            ~CompositeImageUpdateImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Generate output information. */
            virtual void
            GenerateOutputInformation(void);

            /** Validate the presence of all ten inputs. If one or more inputs
             * are missing, throw an exception. */
            virtual void
            BeforeThreadedGenerateData();

            /** Composite Filter method. */
            virtual void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** Test if the pixel value is not too old in the composite product */
            void
            OldReflectanceTest(InputImageConstIteratorType& inIt1, InputImageConstIteratorType& inIt2, InputImageConstIteratorType& inIt3,
                    DateImageConstIteratorType& inPxdIt, const InputMaskPixelType& inMsk, OutputImageIteratorType& outIt1,
                    OutputImageIteratorType& outIt2, OutputImageIteratorType& outIt3, DateImageIteratorType& outPxdIt,
                    InputMaskIteratorType& outMskIt);

            /** FillPixels method */
            void
            FillPixels(InputImageConstIteratorType& inIt1, InputImageConstIteratorType& inIt2, InputImageConstIteratorType& inIt3,
                    const DateImagePixelType& inPxd, const InputMaskPixelType& inMsk, OutputImageIteratorType& outIt1,
                    OutputImageIteratorType& outIt2, OutputImageIteratorType& outIt3, DateImageIteratorType& outPxdIt,
                    InputMaskIteratorType& outMskIt);

            /** FillPixels method with L2 No_Data value */
            void
            FillPixels(OutputImageIteratorType& outIt1, OutputImageIteratorType& outIt2, OutputImageIteratorType& outIt3,
                    DateImageIteratorType& outPxdIt, InputMaskIteratorType& outMskIt);

            /** FillPixels method */
            void
            FillSTOPixels(InputImageConstIteratorType& inSTOIt, InputImageConstIteratorType& inTOCRIt, OutputImageIteratorType& outSTOIt);

        private:
            CompositeImageUpdateImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Init mode parameter */
            bool m_InitMode;

            /** Real L2 No_Data parameter */
            RealNoDataType m_RealL2NoData;
            /** L2 No_Data parameter */
            NoDataType m_L2NoData;

            /** AOT Threshold parameter */
            double m_MaxAOT;
            /** Maximum age for reflectancein the composite image */
            unsigned short m_MaxAge;

            /** Hotspot flag parameter */
            bool m_HotspotFlag;
            /** Cirrus flag parameter */
            bool m_CirrusFlag;
            /** Rain flag parameter */
            bool m_RainFlag;

            /** Acquisition date in julian days */
            short m_JDay;
            /** AOT mean parameter */
            double m_AOTMean;

            /** Number of bands of the STO images */
            unsigned int m_STOBandNumber;

            /** RCR band index set in the STO image */
            unsigned int m_STOBandIndex;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsCompositeImageUpdateImageFilter.txx"
#endif

#endif /* __vnsCompositeImageUpdateImageFilter_h */
