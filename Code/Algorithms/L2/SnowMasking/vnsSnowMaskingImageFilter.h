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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mars 2014 : Améliorations algorithmiques                *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 19 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0  : 19 oct. 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskingImageFilter_h
#define __vnsSnowMaskingImageFilter_h

#include "itkProcessObject.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "vnsMacro.h"
#include "vnsSnowMaskDeterminationImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkOrImageFilter.h"
#include "vnsIncidenceAngleFunctor.h"
#include "otbObjectList.h"

namespace vns
{
    /** \class  SnowMaskingImageFilter
     * \brief This class is used to detect Snow in S2 and Landsat Images, using the SWIR bands.
     *
     * Caution: We use water mask only if mode != Init and useWaterMask == True
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */

        class SnowMaskingImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef SnowMaskingImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(SnowMaskingImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef VNSRealVectorImageType InputImageType;
            typedef InputImageType::ConstPointer InputImageConstPointer;
            typedef InputImageType::RegionType InputImageRegionType;
            typedef InputImageType::PixelType InputImagePixelType;
            typedef InputImageType::SizeType SizeType;
            typedef VNSMaskImageType OutputMaskType;
            typedef OutputMaskType::Pointer OutputMaskPointer;
            typedef OutputMaskType::PixelType OutputMaskPixelType;

            typedef VNSDTMImageType InputSimpleImageType;
            typedef InputSimpleImageType::Pointer InputSimpleImagePointer;
            typedef InputSimpleImageType::ConstPointer InputSimpleImageConstPointer;
            typedef InputSimpleImageType::PixelType InputSimpleImagePixelType;

            typedef VNSMaskImageType InputMaskType;
            typedef InputMaskType::Pointer InputMaskPointer;
            typedef InputMaskType::ConstPointer InputMaskConstPointer;

            typedef otb::Image<double, 2> InternalIncidenceAngleImageType;
            typedef InternalIncidenceAngleImageType::Pointer InternalIncidenceAngleImageTypePointer;
            typedef InternalIncidenceAngleImageType::PixelType InternalIncidenceAnglePixelType;

            typedef itk::ImageRegionConstIterator<InputMaskType> InputMaskConstIteratorType;
            typedef itk::ImageRegionConstIterator<InputImageType> InputImageConstIteratorType;
            typedef itk::ImageRegionIterator<OutputMaskType> OutputMaskIteratorType;

            typedef Functor::IncidenceAngleFunctor< InputSimpleImagePixelType, InputSimpleImagePixelType, InternalIncidenceAnglePixelType > IncidenceAngleFunctorType;
            typedef itk::BinaryFunctorImageFilter< InputSimpleImageType, InputSimpleImageType, InternalIncidenceAngleImageType, IncidenceAngleFunctorType> IncidenceAngleFilterType;
            typedef IncidenceAngleFilterType::Pointer IncidenceAngleFilterPointer;
            typedef otb::ObjectList<IncidenceAngleFilterType> IncidenceAngleFilterListType;
            typedef IncidenceAngleFilterListType::Pointer IncidenceAngleFilterListPointer;

            typedef SnowMaskDeterminationImageFilter<InputImageType, InputMaskType, InternalIncidenceAngleImageType, OutputMaskType> SnowMaskDeterminationImageFilterType;
            typedef SnowMaskDeterminationImageFilterType::Pointer SnowMaskDeterminationImageFilterPointer;

            typedef itk::BinaryBallStructuringElement<OutputMaskPixelType, 2> KernelType;
            typedef itk::BinaryMorphologicalClosingImageFilter<OutputMaskType, OutputMaskType, KernelType>
                    BinaryMorphologicalClosingImageFilterType;
            typedef BinaryMorphologicalClosingImageFilterType::Pointer BinaryMorphologicalClosingImageFilterPointer;

            typedef itk::OrImageFilter<InputMaskType, InputMaskType, InputMaskType> OrFilterType;
            typedef OrFilterType::Pointer OrFilterPointer;


            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro( IPTOCR, InputImageType, 0)
            /** Set the edge mask sub */
            vnsSetGetInputMacro( EDGSub, InputMaskType, 1)
            /** Set the cirrus mask sub */
            vnsSetGetInputMacro( IPCirrusMaskSub, InputMaskType, 2)
            /** Set slope image */
            vnsSetGetInputMacro( Slope, InputSimpleImageType, 3)
            /** Set aspect image */
            vnsSetGetInputMacro( Aspect, InputSimpleImageType, 4)
            /** Set water mask sub */
            vnsSetGetInputMacro( L2inWAS, InputMaskType, 5)


            // Green Band parameter accesors
            itkSetMacro(GreenBand, unsigned int)

            // SWIR Band parameter accesors
            itkSetMacro(SWIRBand, unsigned int)

            // Red Band parameter accesors
            itkSetMacro(RedBand, unsigned int)

            // SE Radius for morphological closure parameter accesors
            itkSetMacro(SERadius, unsigned short)

            // NDSI thresold parameter accesors
            itkSetMacro(NDSIThreshold, double)

            // Red reflectance threshold parameter accesors
            itkSetMacro(RedThreshold, double)

            // SWIR reflectance threshold parameter accesors
            itkSetMacro(SWIRThreshold, double)

            // ThetaS parameter accesors
            itkSetMacro(ThetaS, double)

            // PhiS parameter accesors
            itkSetMacro(PhiS, double)

            // minimum incidence angle parameter accesors
            itkSetMacro(MinCosI, double)

            // UseWaterMask parameter accesors
            itkSetMacro(UseWaterMask, bool)

            // IsInitMode parameter accesors
            itkSetMacro(IsInitMode, bool)

            //Output accessor
            itkGetObjectMacro(SnowMaskOutput, OutputMaskType)
            itkGetConstObjectMacro(SnowMaskOutput, OutputMaskType)

            /** Main computation method */
            void
            UpdateWithCaching();

        protected:
            /** Constructor */
            SnowMaskingImageFilter();
            /** Destructor */
            virtual
            ~SnowMaskingImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            SnowMaskingImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            unsigned int m_GreenBand;
            unsigned int m_SWIRBand;
            unsigned int m_RedBand;
            unsigned short m_SERadius;
            double m_NDSIThreshold;
            double m_RedThreshold;
            double m_SWIRThreshold;
            double m_ThetaS;
            double m_PhiS;
            double m_MinCosI;
            bool m_UseWaterMask;
            bool m_IsInitMode;

            OutputMaskPointer m_SnowMaskOutput;
            IncidenceAngleFilterPointer m_IncidenceAngleFilter;
            SnowMaskDeterminationImageFilterPointer m_SnowMaskDeterminationImageFilter;
            BinaryMorphologicalClosingImageFilterPointer m_BinaryClosingImageFilter;
            OrFilterPointer m_OrFilter;

        };

} // End namespace vns

#endif /* __vnsSnowMaskingImageFilter_h */
