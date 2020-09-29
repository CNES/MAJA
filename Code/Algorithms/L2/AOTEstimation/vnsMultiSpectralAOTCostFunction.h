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
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 17 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 2 mai 2016 : Correction warning, qualite, etc             *
 * MOD : VERSION : 1-0-0  : 14 oct. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMultiSpectralAOTCostFunction_h
#define __vnsMultiSpectralAOTCostFunction_h

#include "otbVegetationIndicesFunctor.h"

#include "vnsAOTCostFunctionBase.h"
#include "vnsMacro.h"

#include "itkMultipleValuedCostFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkFixedArray.h"

#include <iostream>

namespace vns
{

    /** \class MultiSpectralAOTCostFunction
     * \brief : This class minimizes the equation
     *
     *   cost1 = Weq1 * sum l,p { [ Rsurf_est (l,p,blue,AOT(D)) - Rsurf_sim (l,p,blue,AOT(D))]² }
     *
     *   vector_cost =  [ cost1(pix1) ; cost1(pix2) ; cost_bound + cost_dark ]
     *
     *   for the (AOT(D)) parameters
     *
     *   (l,p,b) values are sampled over a rectangular region
     *   whose size is defined by XRange and YRange
     *
     * \sa itkResampleImageFilter
     * \sa itkLinearInterpolationImageFilter
     *
     */
    template<class TInputImage, class TInputDTM, class TLUT>
        class MultiSpectralAOTCostFunction : public AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>
        {
        public:
            /** Standard class typedefs. */
            typedef MultiSpectralAOTCostFunction Self;
            typedef AOTCostFunctionBase<TInputImage, TInputDTM, TLUT> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(MultiSpectralAOTCostFunction, AOTCostFunctionBase )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::Pointer InputImagePointerType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::IndexType IndexType;
            typedef typename InputImageType::PixelType PixelType;
            typedef typename InputImageType::InternalPixelType InternalPixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::OffsetType OffsetType;

            /** Look up table template typedefs */
            typedef TLUT LUTType;
            typedef typename LUTType::Pointer LUTPointerType;
            typedef typename LUTType::PixelType LUTPixelType;
            typedef typename LUTType::PointType LUTPointType;
            typedef typename LUTPointType::ValueType LUTPointValueType;
            typedef typename LUTType::ParameterValuesType LUTParameterValuesType;

            typedef TInputDTM DTMType;
            typedef typename DTMType::PixelType DTMPixelType;
            typedef typename LUTType::Pointer DTMPointerType;

            typedef typename Superclass::ParametersType ParametersType;
            typedef typename Superclass::DerivativeType DerivativeType;
            typedef typename Superclass::MeasureType MeasureType;

            typedef otb::Functor::NDVI<InternalPixelType, InternalPixelType> NDVIFunctorType;

            /* Neighborhood Iterator related type */
            typedef itk::ConstNeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
            typedef itk::ConstNeighborhoodIterator<DTMType> DTMNeighborhoodIteratorType;

            typedef itk::FixedArray<double, 4> FixedArrayType;
            typedef std::vector<unsigned int> UIVectType;
            typedef std::vector<OffsetType> OffsetVectType;


            /* The first band is the blue channel and the second one is the red channel */
            void
            SetAOTbands(const UIVectType & pAOTbands)
            {
                m_AOTbands = pAOTbands;
                this->Modified();
            }
            vnsGetMacro(AOTbands,UIVectType)

            /** Red Band used for the NDVI computation */
            itkSetMacro(NDVIAOTBand1, unsigned int)
            itkGetConstReferenceMacro(NDVIAOTBand1, unsigned int)

            /** NIR Band used for the NDVI computation */
            itkSetMacro(NDVIAOTBand2, unsigned int)
            itkGetConstReferenceMacro(NDVIAOTBand2, unsigned int)

            /** Slope of the linear relationship between blue and red surface reflectances */
            itkSetMacro(SlopeMS,double)
            itkGetConstReferenceMacro(SlopeMS, double)

            /** YIntercept of the linear relationship between blue and red surface reflectances */
            itkSetMacro(YInterceptMS,double)
            itkGetConstReferenceMacro(YInterceptMS, double)

            /** Slope for multi spectral weight that depends on NDVI */
            itkSetMacro(SlopeNDVI,double)
            itkGetConstReferenceMacro(SlopeNDVI, double)

            /** YIntercept for multi spectral weight that depends on NDVI */
            itkSetMacro(YInterceptNDVI,double)
            itkGetConstReferenceMacro(YInterceptNDVI, double)

            void
            SetMeasureSize(unsigned int pSize)
            {
                this->m_RangeDimension = pSize + 1;
                // Note :  + 1 because we add of the 2 bounds errors  and the dark errors
                //Wae add one since cams add one dims
                if(this->m_UseCamsData){
                	this->m_RangeDimension++;
                }
                this->m_Measure.SetSize(this->m_RangeDimension);
                this->m_Measure.fill(0);
                this->m_Derivative.SetSize(this->m_SpaceDimension, this->m_RangeDimension);
                this->m_Derivative.fill(0);
                this->Modified();
            }

            MeasureType
            GetValue(const ParametersType & parameters) const;
            void
            EvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD, MeasureType & pMSMeasure) const;

            void
            GetDerivative(const ParametersType & parameters, DerivativeType & derivative) const;
            void
            DerivativeEvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD, DerivativeType & pMSDerivative) const;

        protected:
            MultiSpectralAOTCostFunction(void);
            virtual
            ~MultiSpectralAOTCostFunction(void)
            {
            }
            ;

            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            MultiSpectralAOTCostFunction(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Red Band used for the NDVI computation */
            unsigned int m_NDVIAOTBand1;
            /** NIR Band used for the NDVI computation */
            unsigned int m_NDVIAOTBand2;

            UIVectType m_AOTbands;

            /** Slope of the MS linear relaationship between the red and the ble surface reflectances */
            double m_SlopeMS;

            /** Y-intercept of the MS linear relationship between the ble and the red surface reflectances */
            double m_YInterceptMS;

            /** Slope for multi spectral weight that depends on NDVI */
            double m_SlopeNDVI;

            /** Y-intercept for multi spectral weight that depends on NDVI */
            double m_YInterceptNDVI;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMultiSpectralAOTCostFunction.txx"
#endif

#endif /* __vnsMultiSpectralAOTCostFunction_h */
