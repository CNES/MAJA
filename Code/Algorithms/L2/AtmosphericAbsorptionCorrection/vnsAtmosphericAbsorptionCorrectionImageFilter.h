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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 03 novembre 2011 : Modification pour prise en compte            *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsAtmosphericAbsorptionCorrectionImageFilter_h
#define __vnsAtmosphericAbsorptionCorrectionImageFilter_h

#include "vnsMacro.h"
#include "itkProcessObject.h"

#include "vnsLookUpTable.h"
#include "otbImage.h"
#include "otbObjectList.h"
#include "otbImageFileReader.h"

#include "otbFunctorImageFilter.h"
#include "vnsPressureFunctor.h"
#include "vnsWaterAmountImageGenerator.h"
#include "vnsTOACorrectionImageFilter.h"
#include "vnsSmacCoefContainer.h"
#include "vnsConstantImageSource.h"
#include "vnsPadAndResampleImageFilter.h"

#include "vnsUtilities.h"

namespace vns
{
    /** \class  AtmosphericAbsorptionCorrectionImageFilter
     * \brief This class implements a TOA correction using elevation information.
     *
     * It computes the correction with SMAC correction and integration of the water amount and the ozone amount.
     * Inputs are:
     * \begin{itemize}
     *    \item DTM image (otb::Image)
     *    \item TOA image (otb::VectorImage)
     *    \item LUT image (vns::LookUpTable) as an option
     *    \item The water amount value at the center of the image (double) as an option.
     *    \item The ozone amount value at the center of the image (double).
     *  \end{itemize}
     *
     * Output are:
     * \begin{itemize}
     *    \item Vapor amount image (otb::Image)
     *    \item corrected image (otb::VectorImage) as an option.
     *  \end{itemize}
     *
     * Please, notice that the LUT is optional and allows to compute a water amount image.
     * This input can be replaced by a default water amount value at the center of the image.
     * In this previous case, the Vapor amount image will not be computed and thus no
     * available as an output of the filter.
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
        class AtmosphericAbsorptionCorrectionImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef AtmosphericAbsorptionCorrectionImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(AtmosphericAbsorptionCorrectionImageFilter, ProcessObject)

            /** Some convenient typedefs. */
            /** TOA input image typedefs. */
            typedef VNSRealVectorImageType TOAImageType;
            typedef otb::ObjectList<TOAImageType> TOAImageListType;
            typedef TOAImageListType::ConstPointer TOAImageListConstPointer;
            typedef TOAImageType::ConstPointer TOAImageConstPointer;
            typedef TOAImageType::RegionType RegionType;
            typedef TOAImageType::PixelType TOAImagePixelType;
            typedef TOAImageType::InternalPixelType TOAImageInternalPixelType;
            typedef TOAImageType::SizeType SizeType;
            /** Altitude input image typedefs. */
            typedef VNSDTMImageType DTMImageType;
            typedef otb::ObjectList<DTMImageType> DTMImageListType;
            typedef DTMImageListType::ConstPointer DTMImageListConstPointer;
            typedef DTMImageType::ConstPointer DTMImageConstPointer;
            typedef DTMImageType::PixelType DTMImagePixelType;
            typedef DTMImageType::InternalPixelType DTMImageInternalPixelType;
            /** LUT image typedef */
            typedef VNSLUTMonoBand1DType LUTType;
            typedef LUTType::Pointer LUTPointer;
            typedef LUTType::ConstPointer LUTConstPointer;
            /** Corrected TOA output image typedefs. */
            typedef VNSRealVectorImageType TOACImageType;
            typedef otb::ObjectList<TOACImageType> TOACImageListType;
            typedef TOACImageListType::Pointer TOACImageListPointer;
            typedef TOACImageType::Pointer TOACImagePointer;
            typedef TOACImageType::ConstPointer TOACImageConstPointer;
            typedef TOACImageType::PixelType TOACImagePixelType;
            typedef TOACImageType::InternalPixelType TOACImageInternalPixelType;
            /** Vapor amount output image typedefs. */
            typedef VNSRealImageType VAPImageType;
            typedef otb::ObjectList<VAPImageType> VAPImageListType;
            typedef VAPImageListType::Pointer VAPImageListPointer;
            typedef VAPImageType::Pointer VAPImagePointer;
            typedef VAPImageType::ConstPointer VAPImageConstPointer;
            typedef VAPImageType::PixelType VAPImagePixelType;
            typedef VAPImageType::InternalPixelType VAPImageInternalPixelType;

            /** Temporary image typedefs */
            typedef otb::Image<VNSRealPixelType> TempImageType;
            typedef TempImageType::PixelType TempImagePixelType;

            typedef Utilities::ListOfDoubles ListOfDoubles;
            typedef Utilities::ListOfListOfDoubles ListOfListOfDoubles;

            /** Internal filters definition */
            /** Pressure image filter typedefs */
            typedef Functor::PressureFunctor<DTMImagePixelType, TempImagePixelType> PressureFunctorType;
            typedef itk::UnaryFunctorImageFilter<DTMImageType, TempImageType, PressureFunctorType> PressureFilterType;
            typedef PressureFilterType::Pointer PressureFilterPointer;
            typedef otb::ObjectList<PressureFilterType> PressureFilterListType;
            /** Water amount image generator typedefs */
            typedef WaterAmountImageGenerator<TOAImageType, VAPImageType, LUTType> WaterAmountImageGeneratorType;
            typedef WaterAmountImageGeneratorType::Pointer WaterAmountImageGeneratorPointer;
            /** TOA correction image filter typedefs */
            typedef TOACorrectionImageFilter<TOAImageType, TempImageType, TOACImageType, VAPImageType> TOACorrectionImageFilterType;
            typedef TOACorrectionImageFilterType::Pointer TOACorrectionImageFilterPointer;
            typedef otb::ObjectList<TOACorrectionImageFilterType> TOACorrectionImageFilterListType;
            typedef TOACorrectionImageFilterType::SmacCoefContainerListType SmacCoefContainerListType;
            typedef std::vector<SmacCoefContainerListType> ListOfSmacCoefContainerListType;
            /** Expand filter */
            typedef PadAndResampleImageFilter<VAPImageType, VAPImageType> PadAndResampleImageFilterType;

            typedef PadAndResampleImageFilterType::Pointer PadAndResampleImageFilterPointer;
            typedef otb::ObjectList<PadAndResampleImageFilterType> PadAndResampleImageFilterListType;
            /** Resize filter */
            /** Constant image for VAP */
            typedef ConstantImageSource<VAPImageType> VAPImageConstantImageSourceFilterType;
            typedef VAPImageConstantImageSourceFilterType::Pointer VAPImageConstantImageSourceFilterPointer;
            typedef otb::ObjectList<VAPImageConstantImageSourceFilterType> VAPImageConstantImageSourceFilterListType;

            // Readers for caching
            typedef otb::ImageFileReader<TOAImageType> ReaderTOAType;
            typedef ReaderTOAType::Pointer ReaderTOAPointer;

            /** Set the Internal Product TOA reflectance image at L2 coarse resolution*/
            vnsSetGetInputMacro( IPTOASub, TOAImageType, 0)
            /** Set the Internal Product TOA reflectance image at L2 resolution*/
            vnsSetGetInputMacro( IPTOA, TOAImageListType, 1)
            /** Set the DTM image at L2 coarse resolution*/
            vnsSetGetInputMacro( DTMSub, DTMImageType, 2)
            /** Set the DTM image at L2 resolution*/
            vnsSetGetInputMacro( DTM, DTMImageListType, 3)

            /** Set the LUT image */
            void
            SetLUT(LUTPointer lut);
            /** Get the DTM image */
            itkGetConstObjectMacro(LUT, LUTType)

            /** Get the internal product corrected TOA reflectance output image at L2Coarse */
            itkGetObjectMacro(IPTOACSubOutput, TOACImageType)
            itkGetConstObjectMacro(IPTOACSubOutput, TOACImageType)

            /** Get the internal product corrected TOA reflectance output image at L2 resolution */
            itkGetObjectMacro(IPTOACImageListOutput, TOACImageListType)
            itkGetConstObjectMacro(IPTOACImageListOutput, TOACImageListType)

            /** Get the vapor amount images @ L2Resolution */
            itkGetObjectMacro(L2VAPImageListOutput, VAPImageListType)
            itkGetConstObjectMacro(L2VAPImageListOutput, VAPImageListType)

            /** Get the vapor amount image @ L2Coarse */
            itkGetObjectMacro(VAPSubOutput, VAPImageType)
            itkGetConstObjectMacro(VAPSubOutput, VAPImageType)

            /**  Set the water amount default value. */
            itkSetMacro(L2NoData, RealNoDataType)

            /**  Get the water amount default value. */
            itkGetConstMacro(L2NoData, RealNoDataType)

            /**  Get the water amount default value. */
            itkGetConstMacro(UseWaterAmountValue, bool)

            /** Set the water alount defualt value. */
            void
            SetWaterAmountValue(double val)
            {
                m_WaterAmountValue = val;
                m_UseWaterAmountValue = true;
                this->Modified();
            }
            /** Get the water amount default value. */
            itkGetConstMacro(WaterAmountValue, double)

            /** Set the water amount maximum threshold. */
            itkSetMacro(WaterVaporMaxThreshold, double)
            /** Get the water amount maximum threshol. */
            itkGetConstMacro(WaterVaporMaxThreshold, double)

            /** Set the channel of the 865 nm wavelength. */
            itkSetMacro(WaterVaporReferenceBandId, unsigned int)

            /** Get the channel of the 865 nm wavelength. */
            itkGetConstMacro(WaterVaporReferenceBandId, unsigned int)

            /** Set the channel of the 910 nm wavelength. */
            itkSetMacro(WaterVaporBandId, unsigned int)

            /** Get the channel of the 910 nm wavelength. */
            itkGetConstMacro(WaterVaporBandId, unsigned int)

            /** Set the ozone value. */
            itkSetMacro(OzoneAmountValue, double)

            /** Get the ozone value. */
            itkGetConstMacro(OzoneAmountValue, double)

            /**  Set solar zenith angle value. */
            vnsSetMacro(ThetaS, double)

            /**  Get solar zenith angle value. */
            vnsGetConstMacro(ThetaS, double)

            /**  Set view zenith angle values at L2 coarse resolution (one value per band). */
            vnsSetMacro(ThetaVSub, ListOfDoubles)

            /**  Get view zenith angle values at L2 coarse resolution. */
            vnsGetConstMacro(ThetaVSub, ListOfDoubles)

            /**  Set view zenith angle value at L2 resolutions. */
            vnsSetMacro(ThetaV, ListOfListOfDoubles)

            /**  Get view zenith angle valueat L2 resolutions. */
            vnsGetConstMacro(ThetaV, ListOfListOfDoubles)

            /** Write L2 product to L2 resolution. */
            itkSetMacro(WriteL2Resolution, bool)
            itkGetConstReferenceMacro(WriteL2Resolution, bool)

            vnsProtectedMemberAndSetAndGetConstReferenceMacro(VAPNodataValue, unsigned char)

            /** Set Smac coefficients list at L2 coarse resolution */
            void
            SetSMACCoefficientsSubList(const SmacCoefContainerListType & coefList)
            {
                m_SMACCoefficientsSubList = coefList;
                this->Modified();
            }
            /** Const get Smac coefficients list */
            SmacCoefContainerListType
            GetSMACCoefficientsSubList() const
            {
                return m_SMACCoefficientsSubList;
            }
            /** Get Smac coefficients list */
            SmacCoefContainerListType
            GetSMACCoefficientsSubList()
            {
                return m_SMACCoefficientsSubList;
            }
            /** Add Smac coefficients to the list */
            void
            AddSmacCoefContainer(const SmacCoefContainer & coef)
            {
                m_SMACCoefficientsSubList.push_back(coef);
                this->Modified();
            }
            /** Add Smac coefficients to the list as a vector of double */
            void
            AddSmacCoefContainer(const std::vector<double> & vectCoef)
            {
                SmacCoefContainer coef(vectCoef);
                this->AddSmacCoefContainer(coef);
            }

            /** Set Smac coefficients list at L2 resolutions (per band and per resolution) */
            void
            SetSMACCoefficientsList(const ListOfSmacCoefContainerListType & coefList)
            {
                m_SMACCoefficientsList = coefList;
                this->Modified();
            }
            /** Const get Smac coefficients list */
            ListOfSmacCoefContainerListType // MACCS 5.0 SmacCoefContainerListType
            GetSMACCoefficientsList() const
            {
                return m_SMACCoefficientsList;
            }

            vnsMemberAndSetMacro(RealL2NoData,RealNoDataType)

            /** Main computation method. */
            virtual void
            UpdateWithCaching(void);

        protected:
            /** Constructor */
            AtmosphericAbsorptionCorrectionImageFilter();
            /** Destructor */
            virtual
            ~AtmosphericAbsorptionCorrectionImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            void
            operator=(const Self&); //purposely not implemented

            /** Parameters declaration */
            /** L2 No_data value */
            RealNoDataType m_L2NoData;
            /** Use water amount default value boolean */
            bool m_UseWaterAmountValue;
            /** Water amount default value */
            double m_WaterAmountValue;
            /** boolean to know if the LUT was was set */
            bool m_IsSetLUT;
            /** Channel of the 865 nm wavelength, first channel has the id 0. */
            unsigned int m_WaterVaporReferenceBandId;
            /** Channel of the 910 nm wavelength, first channel has the id 0. */
            unsigned int m_WaterVaporBandId;
            /** Maximum water vapour amount threshold */
            double m_WaterVaporMaxThreshold;
            /** Ozone value */
            double m_OzoneAmountValue;
            /**  Solar zenith angle in degree. */
            double m_ThetaS;
            /**  View zenith angle in degree (one per band) at L2 coarse resolution. */
            ListOfDoubles m_ThetaVSub;
            /**  View zenith angle in degree (one per band) at L2 resolutions. */
            ListOfListOfDoubles m_ThetaV;
            /** Write L2 product to L2 resolutions */
            bool m_WriteL2Resolution;
            /** SMAC coefficients at L2 coarse resolution */
            SmacCoefContainerListType m_SMACCoefficientsSubList;
            /** SMAC coefficients at L2 resolutions */
            ListOfSmacCoefContainerListType m_SMACCoefficientsList;
            /** LUT H2O */
            LUTType::Pointer m_LUT;

            /** Filters at L2 coarse resolution */
            /** Pressure calculator filter. */
            PressureFilterPointer m_PressureFilter;
            /** Water amount image generator */
            WaterAmountImageGeneratorPointer m_WaterAmountGenerator;
            /** TOA correction image filter */
            TOACorrectionImageFilterPointer m_TOACorrectionFilter;

            VAPImageConstantImageSourceFilterPointer m_WaterAmountValueConstantImageFilter;

            /** List of filter for L2 resolutions */
            PressureFilterListType::Pointer m_PressureFilterList;
            VAPImageConstantImageSourceFilterListType::Pointer m_WaterAmountValueConstantImageFilterList;
            TOACorrectionImageFilterListType::Pointer m_TOACorrectionFilterList;
            PadAndResampleImageFilterListType::Pointer m_ExpandList;

            //output at L2 Coarse resolution
            TOACImagePointer m_IPTOACSubOutput;
            VAPImagePointer  m_VAPSubOutput;

            //output at L2 resolution
            TOACImageListPointer m_IPTOACImageListOutput;
            VAPImageListPointer m_L2VAPImageListOutput;

        };

} // End namespace vns

#endif /* __vnsAtmosphericAbsorptionCorrectionImageFilter_h */
