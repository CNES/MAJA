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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Classe ThesholdVector dispo dans l'otb   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 07 novembre 2011 : Modification pour prise en compte            *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0-3 : DM : 237 : 02 aout 2011 : Modification expand dans Environnement Correction            *
 *                                             et mise à jour du masque de bord en sortie                   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 6 septembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsEnvironmentCorrectionImageFilter_h
#define __vnsEnvironmentCorrectionImageFilter_h

#include "vnsChangeValueWithMaskVectorFunctor.h"
#include "vnsMacro.h"

#include "itkProcessObject.h"
#include "vnsVectorLookUpTable.h"
#include "vnsNaryEnvironmentCorrectionImageFilter.h"
#include "vnsInterpolateForEnvCorrectionImageFilter.h"
#include "otbConvolutionImageFilter.h"
#include "vnsConvolutionVectorImageFilter.h"
#include "otbImageFileReader.h"
#include "itkAddImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "vnsComputeEnvironmentCorrectionImageFilter.h"
#include "otbObjectList.h"
#include "vnsBandsDefinitions.h"
#include "otbMultiChannelExtractROI.h"

namespace vns
{
    /** \class  EnvironmentCorrectionImageFilter
     * \brief This class implements the algorithms to compute the reflectance image after Environment correction.
     *
     * Note that the size of the output images are expanded at L2_resolution an eventually rearranged
     * with ResizeVectorImageFilter.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
        class EnvironmentCorrectionImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef EnvironmentCorrectionImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(EnvironmentCorrectionImageFilter, ProcessObject )

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

            typedef VNSDTMImageType InputDTMType;
            typedef InputDTMType::Pointer InputDTMPointerType;
            typedef InputDTMType::ConstPointer InputDTMConstPointerType;
            typedef InputDTMType::PixelType InputDTMPixelType;
            typedef InputDTMType::InternalPixelType InputDTMInternalPixelType;

            typedef VNSLUT3DType LUT3DType;
            typedef LUT3DType::Pointer LUT3DPointerType;
            typedef LUT3DType::PixelType LUT3DPixelType;
            typedef LUT3DType::ParameterValuesType ParameterValuesType;

            typedef VNSLUT2DType LUT2DType;
            typedef LUT2DType::Pointer LUT2DPointerType;
            typedef LUT2DType::PixelType LUT2DPixelType;

            typedef VNSRealVectorImageType OutputVectorImageType;
            typedef OutputVectorImageType::Pointer OutputVectorImagePointerType;
            typedef OutputVectorImageType::ConstPointer OutputVectorImageConstPointerType;
            typedef OutputVectorImageType::RegionType OutputVectorImageRegionType;
            typedef OutputVectorImageType::InternalPixelType OutputVectorImageInternalPixelType;
            typedef OutputVectorImageInternalPixelType NoDataType;
            typedef OutputVectorImageType::IndexType OutputVectorImageIndexType;
            typedef OutputVectorImageType::SizeType OutputVectorImageSizeType;
            typedef OutputVectorImageType::PixelType OutputVectorPixelType;
            typedef OutputVectorImageType::InternalPixelType OutputVectorInternalPixelType;
            typedef otb::ObjectList<OutputVectorImageType> OutputVectorImageListType;
            typedef OutputVectorImageListType::Pointer OutputVectorImageListPointer;

            typedef VNSMaskImageType InputMaskType;
            typedef InputMaskType::Pointer InputMaskPointerType;
            typedef InputMaskType::ConstPointer InputMaskConstPointerType;
            typedef InputMaskType::PixelType   InputMaskPixelType;
            typedef otb::ObjectList<InputMaskType> InputMaskListType;
            typedef InputMaskListType::ConstPointer InputMaskListConstPointer;

            /** Filters */
            typedef ComputeEnvironmentCorrectionImageFilter<InputVectorImageType, OutputVectorImageType, InputMaskType> ComputeEnvironmentCorrectionImageFilterType;
            typedef ComputeEnvironmentCorrectionImageFilterType::Pointer ComputeEnvironmentCorrectionImageFilterPointer;
            typedef otb::ObjectList<ComputeEnvironmentCorrectionImageFilterType> ComputeEnvironmentCorrectionImageFilterListType;
            typedef ComputeEnvironmentCorrectionImageFilterListType::Pointer ComputeEnvironmentCorrectionImageFilterListPointer;

            typedef InterpolateForEnvCorrectionImageFilter<InputImageType, InputMaskType, InputDTMType, LUT3DType, LUT2DType,
                    OutputVectorImageType> InterpolateForEnvCorrectionImageFilterType;
            typedef InterpolateForEnvCorrectionImageFilterType::Pointer InterpolateForEnvCorImageFilterPointerType;

            typedef ConvolutionVectorImageFilter<InputVectorImageType, InputMaskType, OutputVectorImageType> ConvolutionVectorImageFilterType;
            typedef ConvolutionVectorImageFilterType::Pointer ConvolutionVectorImageFilterPointerType;
            typedef ConvolutionVectorImageFilterType::ArrayType ArrayType;

            typedef Functor::ChangeValueWithMaskVectorFunctor<OutputVectorPixelType, InputMaskPixelType, OutputVectorPixelType>
                    ChangeValueVectorFunctorType;
            typedef itk::BinaryFunctorImageFilter<OutputVectorImageType, InputMaskType, OutputVectorImageType,
                    ChangeValueVectorFunctorType> ChangeValueWithMaskVectorImageFilterType;
            typedef ChangeValueWithMaskVectorImageFilterType::Pointer ChangeValueWithMaskVectorImageFilterPointer;


            typedef otb::MultiChannelExtractROI<OutputVectorInternalPixelType, OutputVectorInternalPixelType> MultiChannelExtractROIType;
            typedef MultiChannelExtractROIType::Pointer MultiChannelExtractROIPointer;
            typedef otb::ObjectList<MultiChannelExtractROIType> MultiChannelExtractROIListType;
            typedef MultiChannelExtractROIListType::Pointer MultiChannelExtractROIListPointer;

            /* Reader filter */
            typedef otb::ImageFileReader<OutputVectorImageType> VectorImageReaderType;
            typedef VectorImageReaderType::Pointer VectorImageReaderPointerType;
            typedef otb::ImageFileReader<InputMaskType> MaskReaderType;
            typedef MaskReaderType::Pointer MaskReaderPointer;

            /* Useful typedef */
            typedef InterpolateForEnvCorrectionImageFilterType::ListOfDoubles ListOfDoubles;

            vnsSetGetInputRawMacro( IPTOC, InputVectorImageListType, 0)
            vnsSetGetInputRawMacro( IPTOCSub, InputVectorImageType, 1)
            vnsSetGetInputRawMacro( IPAOTSub, InputImageType, 2)
            vnsSetGetInputRawMacro( DTMSub, InputDTMType, 3)
            vnsSetGetInputRawMacro( IPEDGSub, InputMaskType, 4)
            vnsSetGetInputRawMacro( L2EDG, InputMaskListType, 5)

            // RhoEnv output at L2_resolution
            itkGetObjectMacro(RhoEnvList,OutputVectorImageListType)
            // L2SRE output at L2_resolution
            itkGetObjectMacro(L2SREList,OutputVectorImageListType)

            // InterpolateForEnvCor parameters
            otbSetObjectMemberMacro(InterpolateForEnvCor,LUT_Tdir,LUT3DPointerType)
            otbSetObjectMemberMacro(InterpolateForEnvCor,LUT_Tdif,LUT3DPointerType)
            otbSetObjectMemberMacro(InterpolateForEnvCor,LUT_Satm,LUT2DPointerType)
            otbSetObjectMemberMacro(Convolution,Radius,SizeType)
            otbSetObjectMemberMacro(Convolution,Filter,ArrayType)
            // Not implemented yet
            //otbSetObjectMemberMacro(Convolution,NormalizeFilter,bool);
            // Angles ( cannot use macro because of std::vector )
            void
            SetThetaV(const ListOfDoubles & pParam)
            {
                // expected in degree
                m_InterpolateForEnvCor->SetThetaV(pParam);
                this->Modified();
            }

            /** Set Image Information */
            vnsMemberAndDirectSetAndGetConstReferenceMacro(BandsDefinitions, BandsDefinitions)

            vnsMemberAndSetMacro(RealL2NoData, RealNoDataType)

            /** Generate data method */
            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            EnvironmentCorrectionImageFilter();
            /** Destructor */
            virtual
            ~EnvironmentCorrectionImageFilter();

            /*    virtual void GenerateInputRequestedRegion();*/

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            EnvironmentCorrectionImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Validate the presence of all inputs and their sizes. If at least one input
             * is missing, throws an exception. */
            void
            CheckInputs();

            ComputeEnvironmentCorrectionImageFilterListPointer m_ComputeEnvironmentCorrectionImageFilterList;
            InterpolateForEnvCorImageFilterPointerType m_InterpolateForEnvCor;
            ConvolutionVectorImageFilterPointerType m_Convolution;
            ChangeValueWithMaskVectorImageFilterPointer m_VectorChangeValueWithMaskFilter;

            /** Outputs */
            OutputVectorImageListPointer m_RhoEnvList;
            OutputVectorImageListPointer m_L2SREList;

            /* Associated Readers */
            VectorImageReaderPointerType m_RhoEnvReader;
            VectorImageReaderPointerType m_L2SREReader;
            MaskReaderPointer m_L2EDGOutputReader;

            MultiChannelExtractROIListPointer m_RhoEnvExtractList;
            MultiChannelExtractROIListPointer m_TdirExtractList;
            MultiChannelExtractROIListPointer m_TdifExtractList;
            MultiChannelExtractROIListPointer m_SatmExtractList;

//            typename OutputVectorImageType::Pointer m_IdRhoEnvOutput;
//            VectorImageReaderPointerType m_IdRhoEnvOutputReader;

        };

} // End namespace vns

#endif /* __vnsEnvironmentCorrectionImageFilter_h */

