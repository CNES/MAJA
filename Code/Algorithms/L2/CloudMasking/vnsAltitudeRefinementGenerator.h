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
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 16 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 4-0-0 : <TypeFT> : <NumFT> : 16 janvier 2014 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsAltitudeRefinementGenerator.h 3545 2011-11-29 14:21:48Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsAltitudeRefinementGenerator_h
#define __vnsAltitudeRefinementGenerator_h

#include "itkProcessObject.h"
#include "itkTernaryFunctorImageFilter.h"
#include "itkAddImageFilter.h"

#include "vnsSolarDirectionForCloudAltitudeFunctor.h"
#include "vnsShadowMaskProjection.h"
#include "vnsShadowProjectionImageFilter.h"
#include "vnsShadowCriterionGenerator.h"
#include "vnsAltitudeCorrectionFunctor.h"
#include "vnsThresholdImageFunctor.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbObjectList.h"

#include "vnsMacro.h"

#include "vnsDEMDataManager.h"

namespace vns
{
    /** \class  AltitudeRefinementGenerator
     * \brief This class computes the altitude refinement of shadows.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TDiffImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        class AltitudeRefinementGenerator : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef AltitudeRefinementGenerator Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(AltitudeRefinementGenerator, ProcessObject )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;

            typedef TDiffImage DiffImageType;
            typedef typename DiffImageType::Pointer DiffImagePointer;
            typedef typename DiffImageType::ConstPointer DiffImageConstPointer;
            typedef typename DiffImageType::PixelType DiffImagePixelType;

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

            /** Filters typedefs */

            typedef DEMDataManager<DTMImageType> Intersector3DType;

            typedef Functor::SolarDirectionForCloudAltitudeFunctor<InputImagePixelType, InputImagePixelType, CLAImagePixelType,
                    InputImagePixelType> FunctorType;
            typedef itk::TernaryFunctorImageFilter<InputImageType, InputImageType, CLAImageType, InputImageType, FunctorType> SolarFilterType;
            typedef typename SolarFilterType::Pointer SolarFilterPointerType;

            typedef ShadowProjectionImageFilter<InputMaskType, LabelImageType, DTMImageType, GRIDImageType, CLAImageType> ShadowProjectionFilterType;
            typedef typename ShadowProjectionFilterType::Pointer ShadowProjectionPointerFilterType;

            // Result map typedef
            typedef unsigned long LinearIndex;
            typedef typename std::map<LinearIndex, LabelImagePixelType> LabelMapType;

            typedef ShadowMaskProjection<LabelImageType> ShadowMaskProjectionFilterType;
            typedef typename ShadowMaskProjectionFilterType::Pointer ShadowMaskProjectionFilterPointerType;

// V 4.2 - ITK 4 class n'existe plus: itk::AddConstantToImageFilter
//            typedef itk::AddConstantToImageFilter<CLAImageType, short, CLAImageType> AddConstantFilterType;
            typedef itk::AddImageFilter<CLAImageType, CLAImageType, CLAImageType> AddConstantFilterType;
            typedef typename AddConstantFilterType::Pointer AddConstantFilterPointerType;

            typedef ShadowCriterionGenerator<InputImageType, DiffImageType, LabelImageType> ShadowCriterionFilterType;
            typedef typename ShadowCriterionFilterType::DoubleMapType DoubleMapType;
            typedef typename ShadowCriterionFilterType::UIMapType UIMapType;
            typedef typename ShadowCriterionFilterType::Pointer ShadowCriterionFilterPointerType;

            typedef otb::ObjectList<ShadowCriterionFilterType> ShadowCriterionGeneratorListType;

            // No_data pixel value accessors
            itkSetMacro(NoData, RealNoDataType)

            itkGetConstReferenceMacro(NoData, RealNoDataType)

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

            // Shadow band accessors
            itkSetMacro(ShadowBand, unsigned int)

            itkGetConstReferenceMacro(ShadowBand, unsigned int)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsMemberSetGetObjectMacro(L2inRCRInput, InputImageType)

            /** Set the difference reflectance image between the current image and the composite product */
            vnsMemberSetGetObjectMacro(DiffInput, DiffImageType)

            /** Set the image of the cloud mask */
            vnsMemberSetGetObjectMacro(IPCLDSubInput, InputMaskType)

            /** Set the image of label of the cloud mask */
            vnsMemberSetGetObjectMacro(IPCLDSubLabelInput, LabelImageType)

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

            /** Intersector 3D object accessor */
            virtual void
            SetI3D(Intersector3DType* i3d)
            {
                m_I3D = i3d;
                if (m_I3D == NULL)
                {
                    vnsExceptionBusinessMacro("I3D pointer is NULL!");
                }
            }

            /** Get the altitude correction map output */
            vnsGetMacro(AltitudeCorrectionMap, DoubleMapType)

            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            AltitudeRefinementGenerator();
            /** Destructor */
            virtual
            ~AltitudeRefinementGenerator();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            AltitudeRefinementGenerator(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            bool
            LoadI3D(void);

            /** Parameters declaration */
            /** No_data value declaration*/
            RealNoDataType m_NoData;

            /** Altitude excursion around the L1 altitude */
            short m_DeltaHMax;
            short m_DeltaHMin;
            short m_DeltaHStep;
            /** First reference altitude for solar grid */
            double m_SOLH1;
            /** Second reference altitude for solar grid */
            double m_SOLH2;
            /** Reference altitude of solar direction */
            double m_SOLHRef;
            /** Reference altitude of viewing direction */
            double m_VIEHRef;
            /** Minimum number of usable pixels in a label to perform the altitude refinement */
            unsigned short m_AbsNbpixThreshold;
            /** Minimum darkening of image to decide to change the cloud altitude */
            double m_ThresholdDiffImage;
            /** Shadow band */
            unsigned int m_ShadowBand;

            /** Filters declaration */
            SolarFilterPointerType m_SolarFilter;
            SolarFilterPointerType m_FinalSolarFilter;
            ShadowProjectionPointerFilterType m_ShadowProjectionFilter;
            ShadowMaskProjectionFilterPointerType m_ShadowMaskProjectionFilter;
            AddConstantFilterPointerType m_AddConstantFilter;

            typename ShadowCriterionGeneratorListType::Pointer m_ShadowCriterionFilterList;

            /** Internal parameters declaration */
            UIMapType m_LevelMap;
            DoubleMapType m_ShadowCriterionMap;
            DoubleMapType m_AltitudeCorrectionMap;

            /** Intersector 3D object declaration */
            Intersector3DType* m_I3D;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsAltitudeRefinementGenerator.txx"
#endif

#endif /* __vnsAltitudeRefinementGenerator_h */
