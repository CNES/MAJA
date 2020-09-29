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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 07 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 16 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 1-0-0-3 : DM : 357 : 19 sept. 2011 : Passage du parametre ShadowBand au filtre ValidShadowMaskImageFilter *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowMaskDeterminationImageFilter_h
#define __vnsShadowMaskDeterminationImageFilter_h

#include "vnsMaskUpdatingFunctor.h"
#include "vnsImageUpdatingWithMaskFunctor.h"
#include "itkProcessObject.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkTernaryFunctorImageFilter.h"

#include "vnsConditionalDifferenceImageFilter.h"
#include "vnsSolarDirectionForCloudAltitudeFunctor.h"
#include "vnsShadowMaskProjection.h"
#include "vnsShadowProjectionImageFilter.h"
#include "vnsAltitudeCorrectionFunctor.h"
#include "vnsThresholdImageFunctor.h"
#include "vnsAltitudeRefinementGenerator.h"
#include "vnsNoDataShadowImageFilter.h"
#include "vnsCloudLabelRemovalFunctor.h"

#include "otbImageFileReader.h"
#include "otbObjectList.h"

#include "vnsMacro.h"

#include "otbImage.h"

#include "vnsDEMDataManager.h"

namespace vns
{
    /** \class  ShadowMaskDeterminationImageFilter
     * \brief This class generates the shadow mask.
     *
     * The location of cloud shadows within the image is computed using the cloud
     * mask and the cloud altitude determined by stereoscopy and using the viewing
     * and sun angles and the DTM.
     * An iteractive algorithms is used to refine the altitude of clouds so that the
     * geometrical projection of shadows corresponds to the greatest decrrease of
     * reflectance.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        class ShadowMaskDeterminationImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef ShadowMaskDeterminationImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ShadowMaskDeterminationImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            typedef TGRIDImage GRIDImageType;
            typedef typename GRIDImageType::Pointer GRIDImagePointer;
            typedef typename GRIDImageType::ConstPointer GRIDImageConstPointer;
            typedef typename GRIDImageType::PixelType GRIDImagePixelType;
            typedef typename GRIDImageType::IndexType IndexType;
            typedef typename GRIDImageType::PointType PointType;
            typedef typename GRIDImageType::SpacingType SpacingType;

            typedef TCLAImage CLAImageType;
            typedef typename CLAImageType::Pointer CLAImagePointer;
            typedef typename CLAImageType::ConstPointer CLAImageConstPointer;
            typedef typename CLAImageType::PixelType CLAImagePixelType;

            typedef TDTMImage DTMImageType;
            typedef typename DTMImageType::Pointer DTMImagePointer;
            typedef typename DTMImageType::ConstPointer DTMImageConstPointer;
            typedef typename DTMImageType::PixelType DTMImagePixelType;

            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            /** Internal types */
            typedef otb::Image<double, 2> InternalImageType;
            typedef InternalImageType::Pointer InternalImagePointer;

            typedef otb::ImageFileReader<InternalImageType> InternalReaderType;
            typedef InternalReaderType::Pointer InternalReaderPointer;

            /** Filters typedefs */

            typedef vns::DEMDataManager<DTMImageType> Intersector3DType;

            typedef ConditionalDifferenceImageFilter<InputImageType, InputMaskType, InternalImageType> DifferenceFilterType;
            typedef typename DifferenceFilterType::Pointer DifferenceFilterPointerType;

            typedef AltitudeRefinementGenerator<InputImageType, InternalImageType, InputMaskType, LabelImageType, GRIDImageType,
                    CLAImageType, DTMImageType> AltitudeRefinementFilterType;
            typedef typename AltitudeRefinementFilterType::DoubleMapType DoubleMapType;
            typedef typename AltitudeRefinementFilterType::Pointer AltitudeRefinementFilterPointerType;

            typedef otb::ObjectList<AltitudeRefinementFilterType> AltitudeRefinementFilterListType;

            typedef Functor::ImageUpdatingWithMaskImageFunctor<CLAImagePixelType, CLAImagePixelType, CLAImagePixelType> UpdatingCLAFunctorType;
            typedef itk::TernaryFunctorImageFilter<CLAImageType, CLAImageType, CLAImageType, CLAImageType,UpdatingCLAFunctorType> UpdatingCLAFilterType;
            typedef typename UpdatingCLAFilterType::Pointer UpdatingCLAFilterPointerType;

            typedef otb::ObjectList<UpdatingCLAFilterType> UpdatingFilterListType;

            typedef NoDataShadowImageFilter<InternalImageType> NoDataShadowImageFilterType;
            typedef typename NoDataShadowImageFilterType::Pointer NoDataShadowImageFilterPointerType;

            typedef Functor::CloudLabelRemovalFunctor<LabelImagePixelType> CloudLabelRemovalFunctorType;
            typedef itk::UnaryFunctorImageFilter<LabelImageType, LabelImageType, CloudLabelRemovalFunctorType> CloudLabelRemovalFilterType;
            typedef typename CloudLabelRemovalFilterType::Pointer CloudLabelRemovalFilterPointerType;

            typedef Functor::ImageUpdatingWithMaskImageFunctor<CLAImagePixelType, LabelImagePixelType> UpdatingWithMaskFunctorType;
            typedef itk::TernaryFunctorImageFilter<CLAImageType, LabelImageType, CLAImageType, CLAImageType,UpdatingCLAFunctorType> UpdatingWithMaskFilterType;
            typedef typename UpdatingWithMaskFilterType::Pointer UpdatingWithMaskFilterPointerType;

            typedef Functor::SolarDirectionForCloudAltitudeFunctor<InputImagePixelType, InputImagePixelType, CLAImagePixelType,
                    InputImagePixelType> FunctorType;
            typedef itk::TernaryFunctorImageFilter<InputImageType, InputImageType, CLAImageType, InputImageType, FunctorType> SolarFilterType;
            typedef typename SolarFilterType::Pointer SolarFilterPointerType;

            typedef vns::ShadowProjectionImageFilter<InputMaskType, LabelImageType, DTMImageType, GRIDImageType, CLAImageType> ShadowProjectionFilterType;
            typedef typename ShadowProjectionFilterType::Pointer ShadowProjectionPointerFilterType;

            // Result map typedef
            typedef unsigned long LinearIndex;
            typedef typename std::map<LinearIndex, LabelImagePixelType> LabelMapType;
            typedef typename NoDataShadowImageFilterType::LabelListType LabelListType;

            typedef ShadowMaskProjection<LabelImageType> ShadowMaskProjectionFilterType;
            typedef typename ShadowMaskProjectionFilterType::Pointer ShadowMaskProjectionFilterPointerType;

            typedef Functor::AltitudeCorrectionFunctor<CLAImagePixelType, LabelImagePixelType, CLAImagePixelType> AltCorrFunctorType;
            typedef itk::BinaryFunctorImageFilter<CLAImageType, LabelImageType, CLAImageType, AltCorrFunctorType> AltCorrFilterType;
            typedef typename AltCorrFilterType::Pointer AltCorrFilterPointerType;

            typedef otb::ObjectList<AltCorrFilterType> AltCorrFilterListType;

            typedef Functor::ThresholdImageFunctor<LabelImagePixelType, OutputImagePixelType> ThresholdFunctorType;
            typedef typename itk::UnaryFunctorImageFilter<LabelImageType, OutputImageType, ThresholdFunctorType> ThresholdFilterType;
            typedef typename ThresholdFilterType::Pointer ThresholdFilterPointerType;

            typedef Functor::MaskUpdatingImageFunctor<LabelImagePixelType, CLAImagePixelType, LabelImagePixelType> LabelMaskUpdatingFunctorType;
            typedef itk::BinaryFunctorImageFilter<LabelImageType, CLAImageType, LabelImageType, LabelMaskUpdatingFunctorType> LabelMaskUpdatingImageFilterType;
            typedef typename LabelMaskUpdatingImageFilterType::Pointer LabelMaskUpdatingImageFilterPointerType;

            typedef Functor::MaskUpdatingImageFunctor<InputMaskPixelType, CLAImagePixelType, InputMaskPixelType> CloudMaskUpdatingFunctorType;
            typedef itk::BinaryFunctorImageFilter<InputMaskType, CLAImageType, InputMaskType, CloudMaskUpdatingFunctorType> CloudMaskUpdatingImageFilterType;
            typedef typename CloudMaskUpdatingImageFilterType::Pointer CloudMaskUpdatingImageFilterPointerType;

            // Reader for caching
            typedef otb::ImageFileReader<LabelImageType> ReaderLabelType;
            typedef typename ReaderLabelType::Pointer ReaderLabelPointer;

            // No_data pixel value accessors
            itkSetMacro(NoData, RealNoDataType)

            itkGetConstReferenceMacro(NoData, RealNoDataType)

            // Init mode parameter accessors
            itkSetMacro(InitMode, bool)

            itkGetConstReferenceMacro(InitMode, bool)

            // Refinement option accessors
            itkSetMacro(RefinementOption, bool)

            itkGetConstReferenceMacro(RefinementOption, bool)

            // DeltaHMax parameter accessors
            itkSetMacro(DeltaHMax, short)

            itkGetConstReferenceMacro(DeltaHMax, short)

            // DeltaHMin parameter accessors
            itkSetMacro(DeltaHMin, short)

            itkGetConstReferenceMacro(DeltaHMin, short)

            // DeltaHStep parameter accessors
            itkSetMacro(DeltaHStep, short)

            itkGetConstReferenceMacro(DeltaHStep, short)

            // First reference altitude parameter accessors
            itkSetMacro(SOLH1, double)

            itkGetConstReferenceMacro(SOLH1, double)

            // Second reference altitude parameter accessors
            itkSetMacro(SOLH2, double)

            itkGetConstReferenceMacro(SOLH2, double)

            // Reference altitude for sun direction accessors
            itkSetMacro(VIEHRef, double)

            itkGetConstReferenceMacro(VIEHRef, double)

            // Reference altitude for sun direction accessors
            itkSetMacro(SOLHRef, double)

            itkGetConstReferenceMacro(SOLHRef, double)

            // Minimum number of pixels in label to perform the altitude refinement accessors
            itkSetMacro(AbsNbpixThreshold, unsigned short)

            itkGetConstReferenceMacro(AbsNbpixThreshold, unsigned short)

            // Minimum darkening of image to decide to change the cloud altitude
            itkSetMacro(ThresholdDiffImage, double)

            itkGetConstReferenceMacro(ThresholdDiffImage, double)

            // Shadow band accessorsfor the TOCR
            itkSetMacro(ShadowBandTOCR, unsigned int)
            itkGetConstReferenceMacro(ShadowBandTOCR, unsigned int)

            // Shadow band accessorsfor the RCR
            itkSetMacro(ShadowBandRCR, unsigned int)
            itkGetConstReferenceMacro(ShadowBandRCR, unsigned int)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsMemberSetGetObjectMacro(IPTOCRInput, InputImageType)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsMemberSetGetObjectMacro(L2inRCRInput, InputImageType)

            /** Set the edge mask of the input product */
            vnsMemberSetGetObjectMacro(IPEDGInput, InputMaskType)

            /** Set the no_data mask of the composite product */
            vnsMemberSetGetObjectMacro(L2inNDTInput, InputMaskType)

            /** Set the water mask of the composite product */
            vnsMemberSetGetObjectMacro(L2inWASInput, InputMaskType)

            /** Set the image of the cloud mask */
            vnsMemberSetGetObjectMacro(L2CLDSubInput, InputMaskType)

            /** Set the image of label of the cloud mask */
            vnsMemberSetGetObjectMacro(L2CLDSubLabelInput, LabelImageType)

            /** Set the viewing direction grid */
            vnsMemberSetGetObjectMacro(IPVIEInput, GRIDImageType)

            /** Set the solar direction grid for the first reference altitude */
            vnsMemberSetGetObjectMacro(IPSOL1Input, GRIDImageType)
            /** Set the solar direction grid for the second reference altitude */
            vnsMemberSetGetObjectMacro(IPSOL2Input, GRIDImageType)

            /** Set the cloud altitude image */
            vnsMemberSetGetObjectMacro(IPCLAInput, CLAImageType)

            /** Set the DTM image */
            vnsMemberSetGetObjectMacro(DTMInput, DTMImageType)

            /** Get the IPCLDShad output mask */
            vnsMemberGetObjectMacro(IPCLDShadOutput, OutputImageType)

            /** Get the IPCLA output image */
            vnsMemberGetObjectMacro(IPCLAOutput, CLAImageType)

            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            ShadowMaskDeterminationImageFilter();
            /** Destructor */
            virtual
            ~ShadowMaskDeterminationImageFilter();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ShadowMaskDeterminationImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Global refinement for all the sensors */
            void
            GlobalRefinementProcess();

//            /** Local refinement for sensor other than Venus with only one label used
//             * for the global altitude refinement */
//            void
//            LocalRefinementProcess(unsigned short & iter, bool & pIsCLDMaskUpdated);
//
//            /** Last check for the clouds for which no altitude has been found */
//            void
//            LastCheck();

            /** Final projection of the cloud with last estimation of the cloud altitude */
            void
            FinalCloudProjection();

            /** Parameters declaration */
            /** No_data value declaration*/
            RealNoDataType m_NoData;
            /** Init mode declaration */
            bool m_InitMode;
            /** Refinement option declaration */
            bool m_RefinementOption;

            /** Altitude excursion around the L1 altitude */
            short m_DeltaHMax;
            short m_DeltaHMin;
            short m_DeltaHStep;
            /** First reference altitude for solar grid */
            double m_SOLH1;
            /** Second reference altitude for solar grid */
            double m_SOLH2;
            /** Reference altitude of viewing direction */
            double m_VIEHRef;
            /** Reference altitude of solar direction */
            double m_SOLHRef;
            /** Minimum number of usable pixels in a label to perform the altitude refinement */
            unsigned short m_AbsNbpixThreshold;
            /** Minimum darkening of image to decide to change the cloud altitude */
            double m_ThresholdDiffImage;
            /** Shadow band for TOCR */
            unsigned int m_ShadowBandTOCR;
            /** Shadow band for RCR */
            unsigned int m_ShadowBandRCR;

            /** Filters declaration */
            DifferenceFilterPointerType m_DifferenceFilter;
            typename AltitudeRefinementFilterListType::Pointer m_AltitudeRefinementFilterList;
            typename AltCorrFilterListType::Pointer m_AltitudeCorrectionFilterList;
            typename UpdatingFilterListType::Pointer m_ImageUpdatingFilterList;

            NoDataShadowImageFilterPointerType m_NoDataShadowFilter;
            CloudLabelRemovalFilterPointerType m_CloudLabelRemovalFilter;
            UpdatingWithMaskFilterPointerType m_LastCLAUpdatingFilter;

            SolarFilterPointerType m_SolarFilter;
            SolarFilterPointerType m_FinalSolarFilter;
            ShadowProjectionPointerFilterType m_ShadowProjectionFilter;
            ShadowProjectionPointerFilterType m_FinalShadowProjectionFilter;
            ShadowMaskProjectionFilterPointerType m_FinalShadowMaskProjectionFilter;

            ThresholdFilterPointerType m_ThresholdFilter;

            /** Intersector 3D object declaration */
            Intersector3DType m_I3D;
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowMaskDeterminationImageFilter.txx"
#endif

#endif /* __vnsShadowMaskDeterminationImageFilter_h */
