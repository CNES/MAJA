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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution                *
 * VERSION : 1-0-0-3 : DM : 357 : 25 juil. 2011 : Add a test on cloud proportion                              *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCirrusFlagGenerator_h
#define __vnsCirrusFlagGenerator_h

#include "itkProcessObject.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkSimpleDataObjectDecorator.h"

#include "otbVectorImage.h"

#include "vnsMacro.h"
#include "vnsCountCirrusPixelGenerator.h"
#include "vnsThresholdImageFunctor.h"
#include "vnsStreamingConditionalStatisticsImageFilter.h"

namespace vns
{
    /** \class  CirrusFlagGenerator
     * \brief This class raises the cirrus flag if the proportion of cirrus pixels within
     * the image is too high or if the standard deviation is high.
     *
     * A pixel is declared as cirrus if the altitude of the cloud given in the stereoscopic
     * cloud mask is higher a threshold.
     *
     * This class is templated by the otb::Image type used for the input image of cloud altitude.
     *
     * \author CS Systemes d'Information
     *
     * \sa ProcessObject
     *
     * \ingroup L2
     *
     */
    class CirrusFlagGenerator : public itk::ProcessObject
    {
    public:
        /** Standard class typedefs. */
        typedef CirrusFlagGenerator Self;
        typedef itk::ProcessObject Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)

        /** Creation through object factory macro */
        itkTypeMacro(CirrusFlagGenerator, itk::ProcessObject )

        /** Some convenient typedefs. */
        typedef VNSRealImageType InputImageType;
        typedef InputImageType::PixelType PixelType;
        typedef InputImageType::Pointer InputImagePointer;

        typedef VNSMaskImageType InputMaskType;
        typedef InputMaskType::Pointer InputMaskPointer;

        typedef otb::VectorImage<PixelType, 2> TemporaryImageType;
        typedef TemporaryImageType::PixelType TemporaryPixelType;
        typedef TemporaryImageType::Pointer TemporaryImagePointer;

        typedef itk::ProcessObject ProcessObjectType;

        /** Type of DataObjects used for scalar outputs */
        typedef itk::SimpleDataObjectDecorator<bool> BoolObjectType;

        /** Filters typedefs. */
        typedef CountCirrusPixelGenerator<InputImageType, InputMaskType> CountCirrusFilterType;
        typedef CountCirrusFilterType::Pointer CountCirrusFilterPointerType;

        typedef Functor::ThresholdImageFunctor<PixelType, PixelType> ThresholdFunctorType;
        typedef itk::UnaryFunctorImageFilter<InputImageType, InputImageType, ThresholdFunctorType> ThresholdFilterType;
        typedef ThresholdFilterType::Pointer ThresholdFilterPointerType;

//            typedef vns::StreamingMaskConditionalStatisticsVectorImageFilter<TemporaryImageType,
//                    InputImageType> StatisticsFilterType;
        typedef StreamingConditionalStatisticsImageFilter<InputImageType, InputImageType> StatisticsFilterType;
        typedef StatisticsFilterType::Pointer StatisticsFilterPointerType;

        // Altitude threshold accessors
        itkSetMacro(AltitudeThreshold, int)

        itkGetConstReferenceMacro(AltitudeThreshold, int)

        // Accessors of the minimum percentage of cirrus clouds within the image to raise the cirrus flag
        itkSetMacro(MinPercentCirrus, unsigned short)

        itkGetConstReferenceMacro(MinPercentCirrus, unsigned short)

        // Accessors of the minimum percentage of clouds within the image to raise the cirrus flag
        itkSetMacro(MinPercentCloud, unsigned short)

        itkGetConstReferenceMacro(MinPercentCloud, unsigned short)

        // Accessors of the minimum standard deviation for cirrus of thin clouds
        itkSetMacro(MinRstdCirrus, short)

        itkGetConstReferenceMacro(MinRstdCirrus, short)

        // Accessors of the minimum percentage for standard deviation for cirrus of thin clouds
        itkSetMacro(MinPercentStdCirrus, unsigned short)

        itkGetConstReferenceMacro(MinPercentStdCirrus, unsigned short)

        // No_data pixel value accessors
        itkSetMacro(L1NoData, RealNoDataType)

        itkGetConstReferenceMacro(L1NoData, RealNoDataType)

        /** Return the cirrus falg. */
        bool
        GetCirrusFlag() const
        {
            return this->GetCirrusFlagOutput()->Get();
        }
        BoolObjectType*
        GetCirrusFlagOutput();
        const BoolObjectType*
        GetCirrusFlagOutput() const;

        /** Set the stereoscopic cloud mask of level 1 product. */
        vnsSetGetInputMacro( L1CLA, InputImageType, 0)
        /** Set the stereoscopic cloud mask of level 1 product. */
        vnsSetGetInputMacro( EDGSub,InputMaskType , 1)

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
        CirrusFlagGenerator();
        /** Destructor */
        virtual
        ~CirrusFlagGenerator()
        {
        }
        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

        virtual void
        GenerateData(void);

    private:
        CirrusFlagGenerator(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented

        /** Altitude threshold declaration. */
        int m_AltitudeThreshold;
        /** Declaration of the minimum percentage of cirrus clouds within
         * the image to raise the cirrus flag. */
        unsigned short m_MinPercentCirrus;
        unsigned short m_MinPercentCloud;
        /** Declaration of the minimum standard deviation for cirrus of
         * thin clouds. */
        short m_MinRstdCirrus;
        /** Declaration of the minimum percentage for standard deviation
         * for cirrus of thin clouds. */
        unsigned short m_MinPercentStdCirrus;

        /** No_data of L1 product declaration. */
        RealNoDataType m_L1NoData;

        double m_CirrusProportion;
        double m_CloudProportion;
        double m_CloudRstd;
        double m_CloudStd;

        /** Filters declaration. */
        CountCirrusFilterPointerType m_CountCirrusFilter;
        ThresholdFilterPointerType m_ThresholdFilter;
        StatisticsFilterPointerType m_StatisticsFilter;

    };

} // End namespace vns

#endif /* __vnsCirrusFlagGenerator_h */
