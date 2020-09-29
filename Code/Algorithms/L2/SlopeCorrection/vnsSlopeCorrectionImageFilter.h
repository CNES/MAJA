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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 27 jan. 2014 : Utilisation du filtre de calcul de l'angle  *
 *                                                              d'incidence solaire                         *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 07 novembre 2011 : Modification pour prise en compte            *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSlopeCorrectionImageFilter_h
#define __vnsSlopeCorrectionImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkProcessObject.h"
#include "vnsDedicatedPadAndFastResampleImageFilter.h"

#include "vnsVectorLookUpTable.h"
#include "vnsInternalSlopeCorrectionImageFilter.h"
#include "vnsComputeTransmissionImageFilter.h"
#include "otbObjectList.h"
#include "vnsBandsDefinitions.h"
#include "otbMultiChannelExtractROI.h"
#include "vnsUtilities.h"
#include "vnsIncidenceAngleFunctor.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  SlopeCorrectionImageFilter
     * \brief This class implements the algorithms to compute the reflectance image after slope correction.
     *
     * Note that the size of the output images are expanded at L2_resolutionis set by the member m_OutputImageSize.
     * If this size is higher than the input image, the image is completed by the ScalarConstant value.
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
        class SlopeCorrectionImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef SlopeCorrectionImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(SlopeCorrectionImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef VNSRealVectorImageType InputVectorImageType;
            typedef InputVectorImageType::ConstPointer InputVectorImageConstPointerType;
            typedef InputVectorImageType::Pointer InputVectorImagePointerType;
            typedef InputVectorImageType::RegionType InputVectorImageRegionType;
            typedef InputVectorImageType::PixelType InputVectorImagePixelType;
            typedef InputVectorImageType::InternalPixelType InternalPixelType;
            typedef InputVectorImageType::IndexType IndexType;
            typedef InputVectorImageType::SizeType SizeType;
            typedef InputVectorImageType::OffsetType OffsetType;
            typedef otb::ObjectList<InputVectorImageType> InputVectorImageListType;
            typedef InputVectorImageListType::ConstPointer InputVectorImageListConstPointer;

            typedef VNSRealImageType InputImageType;
            typedef InputImageType::Pointer InputImagePointerType;
            typedef InputImageType::ConstPointer InputImageConstPointerType;
            typedef InputImageType::PixelType InputImagePixelType;
            typedef InputImageType::InternalPixelType InputImageInternalPixelType;
            typedef otb::ObjectList<InputImageType> InputImageListType;
            typedef InputImageListType::ConstPointer InputImageListConstPointer;

            typedef VNSDTMImageType InputDTMType;
            typedef InputDTMType::Pointer InputDTMPointerType;
            typedef InputDTMType::ConstPointer InputDTMConstPointerType;
            typedef InputDTMType::PixelType InputDTMPixelType;
            typedef InputDTMType::InternalPixelType InputDTMInternalPixelType;

            typedef VNSLUT3DType LUTType;
            typedef LUTType::Pointer LUTPointerType;
            typedef LUTType::PixelType LUTPixelType;
            typedef LUTType::ParameterValuesType ParameterValuesType;
            typedef std::map<unsigned int, LUTPointerType> LUTMapType;

            typedef VNSRealVectorImageType OutputVectorImageType;
            typedef OutputVectorImageType::Pointer OutputVectorImagePointerType;
            typedef OutputVectorImageType::ConstPointer OutputVectorImageConstPointerType;
            typedef OutputVectorImageType::RegionType OutputVectorImageRegionType;
            typedef OutputVectorImageType::InternalPixelType OutputVectorImageInternalPixelType;
            typedef OutputVectorImageType::PixelType OutputVectorImagePixelType;
            typedef OutputVectorImageType::IndexType OutputVectorImageIndexType;
            typedef OutputVectorImageType::SizeType OutputVectorImageSizeType;
            typedef otb::ObjectList<OutputVectorImageType> OutputVectorImageListType;
            typedef OutputVectorImageListType::Pointer OutputVectorImageListPointer;

            typedef VNSMaskImageType InputMaskType;
            typedef InputMaskType::Pointer InputMaskPointerType;
            typedef InputMaskType::ConstPointer InputMaskConstPointerType;
            typedef otb::ObjectList<InputMaskType> InputMaskListType;
            typedef VNSMaskImageType OutputMaskType;
            typedef OutputMaskType::Pointer OutputMaskPointerType;
            typedef OutputMaskType::ConstPointer OutputMaskConstPointerType;
            typedef OutputMaskType::PixelType OutputMaskPixelType;
            typedef OutputMaskType::RegionType OutputMaskRegionType;
            typedef otb::ObjectList<OutputMaskType> OutputMaskListType;
            typedef OutputMaskListType::Pointer OutputMaskListPointer;

            typedef otb::Image<double, 2> InternalIncidenceAngleImageType;
            typedef InternalIncidenceAngleImageType::Pointer InternalIncidenceAngleImageTypePointer;
            typedef InternalIncidenceAngleImageType::PixelType InternalIncidenceAnglePixelType;

            /** Filters */
            typedef InternalSlopeCorrectionImageFilter<InputVectorImageType, InputImageType, InternalIncidenceAngleImageType, OutputVectorImageType, OutputMaskType>
                    InternalSlopeCorrectionImageFilterType;
            typedef InternalSlopeCorrectionImageFilterType::Pointer InternalSlopeCorrectionImageFilterPointerType;
            typedef otb::ObjectList<InternalSlopeCorrectionImageFilterType> InternalSlopeCorrectionImageFilterListType;
            typedef InternalSlopeCorrectionImageFilterListType::Pointer InternalSlopeCorrectionImageFilterListPointer;

            typedef DedicatedPadAndFastResampleImageFilter<InputVectorImageType, OutputVectorImageType> DedicatedPadAndFastResampleImageFilterType;

            typedef DedicatedPadAndFastResampleImageFilterType::Pointer DedicatedPadAndFastResampleImageFilterPointerType;
            typedef otb::ObjectList<DedicatedPadAndFastResampleImageFilterType> DedicatedPadAndFastResampleImageFilterListType;
            typedef DedicatedPadAndFastResampleImageFilterListType::Pointer DedicatedPadAndFastResampleImageFilterListPointer;

            typedef ComputeTransmissionImageFilter<InputImageType, InputMaskType, InputDTMType, LUTType, OutputVectorImageType>
                    ComputeTransmissionImageFilterType;
            typedef ComputeTransmissionImageFilterType::Pointer ComputeTransmissionImageFilterPointerType;

            typedef otb::MultiChannelExtractROI<OutputVectorImageInternalPixelType, OutputVectorImageInternalPixelType> MultiChannelExtractROIType;
            typedef MultiChannelExtractROIType::Pointer MultiChannelExtractROIPointer;
            typedef otb::ObjectList<MultiChannelExtractROIType> MultiChannelExtractROIListType;
            typedef MultiChannelExtractROIListType::Pointer MultiChannelExtractROIListPointer;

            typedef Functor::IncidenceAngleFunctor< InputImagePixelType, InputImagePixelType, InternalIncidenceAnglePixelType > IncidenceAngleFunctorType;
            typedef itk::BinaryFunctorImageFilter< InputImageType, InputImageType, InternalIncidenceAngleImageType, IncidenceAngleFunctorType> IncidenceAngleFilterType;
            typedef IncidenceAngleFilterType::Pointer IncidenceAngleFilterPointer;
            typedef otb::ObjectList<IncidenceAngleFilterType> IncidenceAngleFilterListType;
            typedef IncidenceAngleFilterListType::Pointer IncidenceAngleFilterListPointer;

            typedef Utilities::ListOfListOfDoubles ListOfListOfDoubles;

            vnsSetGetInputRawMacro( L2SRE, InputVectorImageListType, 0)
            vnsSetGetInputRawMacro( IPAOTSub, InputImageType, 1)
            vnsSetGetInputRawMacro( DTMSub, InputDTMType, 2)
            vnsSetGetInputRawMacro( S, InputImageListType, 3)
            vnsSetGetInputRawMacro( A, InputImageListType, 4)
            vnsSetGetInputRawMacro( IPEDGSub, InputMaskType, 5)
            vnsSetGetInputRawMacro( L2EDG,InputMaskListType , 6)
            vnsSetGetInputRawMacro( RhoEnv, InputVectorImageListType, 7)

            // L2FRE output
            itkGetObjectMacro(L2FREList,OutputVectorImageListType)
            // L2STL output
            itkGetObjectMacro(L2STLList,OutputMaskListType)
            //L2TGS output
            itkGetObjectMacro(L2TGSList,OutputMaskListType)

            // ComputeTransmission parameters
            void
            SetLUT_Tdir(LUTPointerType pParam)
            {
                m_ComputeTransmission->SetLUT_Tdir(pParam);
                this->Modified();
            }
            void
            SetLUT_Tdif(LUTPointerType pParam)
            {
                m_ComputeTransmission->SetLUT_Tdif(pParam);
                this->Modified();
            }

            // Angles
            itkSetMacro(ThetaS, double)
            itkGetMacro(ThetaS, double)

            itkSetMacro(PhiS, double)
            itkGetMacro(PhiS, double)

            // Set view zenith angles per band at L2 resolutions
            void
            SetThetaV(ListOfListOfDoubles pParam)
            {
                const unsigned int lnbResol = pParam.size();
                for (unsigned int r = 0; r < lnbResol; r++)
                {
                    const unsigned int lSize = pParam.at(r).size();
                    for (unsigned int i = 0; i < lSize; i++)
                    {
                        // Conversion from degrees to radians
                        pParam.at(r).at(i) = pParam.at(r).at(i) * otb::CONST_PI_180;
                    }
                }
                m_ThetaV = pParam;
            }
            // Set view azimuth angles per band at L2 resolutions
            void
            SetPhiV(ListOfListOfDoubles pParam)
            {
                const unsigned int lnbResol = pParam.size();
                for (unsigned int r = 0; r < lnbResol; r++)
                {
                    const unsigned int lSize = pParam.at(r).size();
                    for (unsigned int i = 0; i < lSize; i++)
                    {
                        // Conversion from degrees to radians
                        pParam.at(r).at(i) = pParam.at(r).at(i) * otb::CONST_PI_180;
                    }
                }
                m_PhiV = pParam;
            }
            // Thresholds
            itkSetMacro(MinCosI, double)
            itkGetMacro(MinCosI, double)

            itkSetMacro(MinCosE, double)
            itkGetMacro(MinCosE, double)

            itkSetMacro(MinCosRatio, double)
            itkGetMacro(MinCosRatio, double)

            vnsMemberAndSetMacro(RealL2NoData, RealNoDataType)

            /** Set Image Information */
            vnsMemberAndDirectSetAndGetConstReferenceMacro(BandsDefinitions, BandsDefinitions)

            void
            UpdateWithCaching();

        protected:
            /** Constructor */
            SlopeCorrectionImageFilter();
            /** Destructor */
            virtual
            ~SlopeCorrectionImageFilter();
            /** Validate the presence of all inputs. If at least one input
             * is missing, throws an exception. */
            //void BeforeThreadedGenerateData();
            void
            CheckInputs();

            /** Generate data method */
            //virtual void GenerateData();
            /** GenerateInputRequestedRegion */
            //virtual void GenerateInputRequestedRegion();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            SlopeCorrectionImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented


            // Expand images from L2 coarse resolution to L2 resolution
            DedicatedPadAndFastResampleImageFilterListPointer m_ExpandTdirList;
            DedicatedPadAndFastResampleImageFilterListPointer m_ExpandTdifList;

//            ResizeVectorImageFilterListPointer m_ResizeTdirList;
//            ResizeVectorImageFilterListPointer m_ResizeTdifList;

//            AddImageFilterListPointer m_AddTdirTdifList;

            IncidenceAngleFilterListPointer m_IncidenceAngleList;

            MultiChannelExtractROIListPointer m_TdirExtractList;
            MultiChannelExtractROIListPointer m_TdifExtractList;

            InternalSlopeCorrectionImageFilterListPointer m_InternalSlopeCorrectionList;
            ComputeTransmissionImageFilterPointerType m_ComputeTransmission;

            /** Ratio to convert L2 to L2 coarse resolution */
            double m_ThetaS;
            double m_PhiS;
            /** Viewing zenital angles per band at L2 resolutions */
            ListOfListOfDoubles m_ThetaV;
            ListOfListOfDoubles m_PhiV;
            double m_MinCosI;
            double m_MinCosE;
            double m_MinCosRatio;

            //outputs
            OutputVectorImageListPointer m_L2FREList;
            OutputMaskListPointer m_L2STLList;
            OutputMaskListPointer m_L2TGSList;
        };

} // End namespace vns

#endif /* __vnsSlopeCorrectionImageFilter_h */

