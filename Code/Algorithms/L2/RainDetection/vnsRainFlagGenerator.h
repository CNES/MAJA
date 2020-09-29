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
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 24 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsRainFlagGenerator_h
#define __vnsRainFlagGenerator_h

#include "itkProcessObject.h"
#include "itkOrImageFilter.h"
#include "itkSimpleDataObjectDecorator.h"

#include "vnsMultiChannelExtractROI.h"
#include "vnsStreamingMaskConditionalStatisticsVectorImageFilter.h"

namespace vns
{
    /** \class  RainFlagGenerator
     * \brief This class raises the rain flag if a strongly decreasing of the surface reflectance is measured.
     *
     * The detection of heavy rain is based on the surface reflectance (Rayleigh corrected) variation in
     * the near-infrared. If the difference between the mean reflectance of the two input images is higher than
     * the m_ReflectanceDecreaseThreshold parameter, the flag is raised.
     * Cloudy, shadowed, water and no_data pixels are not taken into account in the computation of the mean
     * reflectance. Those information are contains in different masks that are given as inputs.
     *
     * This class is templated by two types :
     *  - otb::VectorImage for the input images of reflectance
     *  - otb::Image for the input masks
     *
     * \author CS Systemes d'Information
     *
     * \sa ProcessObject
     *
     * \ingroup L2
     *
     */
        class RainFlagGenerator : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef RainFlagGenerator Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(RainFlagGenerator, itk::ProcessObject )

            /** Some convenient typedefs. */
            typedef VNSRealVectorImageType InputImageType;
            typedef InputImageType::InternalPixelType InternalPixelType;
            typedef InputImageType::Pointer InputImagePointer;
            typedef InputImageType::ConstPointer InputImageConstPointer;

            typedef VNSMaskImageType InputMaskType;
            typedef InputMaskType::Pointer InputMaskPointer;
            typedef InputMaskType::ConstPointer InputMaskConstPointer;

            /** Type of DataObjects used for scalar outputs */
            typedef itk::SimpleDataObjectDecorator<bool> BoolObjectType;

            typedef itk::ProcessObject ProcessObjectType;

            typedef MultiChannelExtractROI<InternalPixelType, InternalPixelType> ChannelFilterType;
            typedef ChannelFilterType::Pointer ChannelFilterPointerType;
            typedef itk::OrImageFilter<InputMaskType, InputMaskType, InputMaskType> OrFilterType;
            typedef OrFilterType::Pointer OrFilterPointerType;
            typedef StreamingMaskConditionalStatisticsVectorImageFilter<InputImageType, InputMaskType> StatisticsFilterType;
            typedef StatisticsFilterType::Pointer StatisticsFilterPointerType;

            // Decrease reflectance threshold accessors
            itkSetMacro(ReflectanceDecreaseThreshold, double)

            itkGetConstReferenceMacro(ReflectanceDecreaseThreshold, double)

            // Water Band accessors for TOCR
            itkSetMacro(WaterBandTOCR, unsigned short)
            itkGetConstReferenceMacro(WaterBandTOCR, unsigned short)
            // Water Band accessors for RCR
            itkSetMacro(WaterBandRCR, unsigned short)
            itkGetConstReferenceMacro(WaterBandRCR, unsigned short)

            /** Return the rain flag. */
            bool
            GetRainFlag() const
            {
                return this->GetRainFlagOutput()->Get();
            }

            BoolObjectType*
            GetRainFlagOutput();
            const BoolObjectType*
            GetRainFlagOutput() const;

            /** Set the image of surface reflectance corrected from absorption and rayleigh. */
            vnsSetGetInputMacro( IPTOCR, InputImageType, 0)
            /** Set the input composite image of surface reflectance corrected from absorption and rayleigh. */
            vnsSetGetInputMacro( L2inRCR, InputImageType, 1)
            /** Set the cloud mask (summary mask) at L2_coarse_resolution. */
            vnsSetGetInputMacro( L2CLDSub, InputMaskType, 2)
            /** Set the mask of water pixels. */
            vnsSetGetInputMacro( L2WAS, InputMaskType, 3)
            /** Set the mask of no_data (included edges). */
            vnsSetGetInputMacro( IPEDG, InputMaskType, 4)
            /** Set the input mask of water pixels in the composite image (D-1). */
            vnsSetGetInputMacro( L2inWAS,InputMaskType , 5)
            /** Set the input mask of no_data in the composite image (D-1). */
            vnsSetGetInputMacro( L2inNDT, InputMaskType, 6)
            /** Set the input mask of snow pixels. */
            vnsSetGetInputMacro( IPSNWSub, InputMaskType, 7)


            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer
            MakeOutput(unsigned int idx);

            /** Update method declaration used to call GenerateData method. */
            virtual void
            Update(void);

            /** PrintResult method */
            std::string
            PrintResult();

        protected:
            /** Constructor */
            RainFlagGenerator();
            /** Destructor */
            virtual
            ~RainFlagGenerator()
            {
            }
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            virtual void
            GenerateData(void);

        private:
            RainFlagGenerator(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Decrease of the reflectance threshold declaration. */
            double m_ReflectanceDecreaseThreshold;

            /** Water Band declaration for the TOCR band */
            unsigned short m_WaterBandTOCR;
            /** Water Band declaration for the RCR band */
            unsigned short m_WaterBandRCR;

            // Internal values
            double m_L2inRCRMean;
            double m_IPTOCRMean;
            double m_DiffRefelectance;

            /** Filters declaration. */
            ChannelFilterPointerType m_ChannelFilter;
            ChannelFilterPointerType m_ChannelFilter2;
            OrFilterPointerType m_OrFilter1;
            OrFilterPointerType m_OrFilter2;
            OrFilterPointerType m_OrFilter3;
            OrFilterPointerType m_OrFilter4;
            OrFilterPointerType m_OrFilter5;
            StatisticsFilterPointerType m_StatisticsFilter;

        };

} // End namespace vns

#endif /* __vnsRainFlagGenerator_h */
