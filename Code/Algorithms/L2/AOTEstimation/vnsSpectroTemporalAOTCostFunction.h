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
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 17 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 2 mai 2016 : Correction warning, qualite, etc             *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 21 oct. 2011 : Méthode spectro temporelle                  *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsSpectroTemporalAOTCostFunction_h
#define __vnsSpectroTemporalAOTCostFunction_h

#include "vnsMultiTemporalAOTCostFunction.h"
#include "vnsMultiSpectralAOTCostFunction.h"
#include "vnsUtilities.h"
#include "vnsMacro.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkFixedArray.h"
#include <iostream>

namespace vns
{

    /** \class SpectroTemporalAOTCostFunction
     * \brief : This class minimize the equations
     *
     *   cost1 = Weq1 * sum l,p,b { [ Rsurf D-1(l,p,b,AOT(D-1)) - Rsurf D(l,p,b,AOT(D))]² }
     *
     *   cost2 = Weq2 * sum l,p,b { [ Rsurf D(l,p,b,AOT(D)) - Rsurf D(l,p,b)]² }
     *
     *   cost3 = W_MS * sum l,p,b { [ Rsurf_est (l,p,blue,AOT(D)) - Rsurf_sim (l,p,blue,AOT(D))]² }
     *
     *   vector_cost =  [ cost1(pix1) ; cost2(pix1) ; cost3(pix1) ; cost2(pix2) ; cost2(pix2) ; cost3(pix2) ; cost_bound + cost_dark ]
     *
     *   for the (AOT(D),AOT(D-1)) parameters
     *
     *   (l,p,b) values are sampled over a rectangular region
     *   and with several bands b
     *   whose size is defined by XRange and YRange
     *
     * \sa itkResampleImageFilter
     * \sa itkLinearInterpolationImageFilter
     *
     */
    template<class TInputImage, class TInputDTM, class TLUT>
        class SpectroTemporalAOTCostFunction : public MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT>
        {
        public:
            /** Standard class typedefs. */
            typedef SpectroTemporalAOTCostFunction Self;
            typedef MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(SpectroTemporalAOTCostFunction, MultiTemporalAOTCostFunction )

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

            /* Neighborhood Iterator related type */
            typedef itk::ConstNeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
            typedef itk::ConstNeighborhoodIterator<DTMType> DTMNeighborhoodIteratorType;

            typedef itk::FixedArray<double, 4> FixedArrayType;
            typedef Utilities::ListOfUIntegers                      ListOfUIntegers;
            typedef std::vector<OffsetType> OffsetVectType;

            typedef typename Superclass::ParametersType ParametersType;
            typedef typename Superclass::DerivativeType DerivativeType;
            typedef typename Superclass::MeasureType MeasureType;


            /** Multi Spectral cost function filter */
            typedef MultiSpectralAOTCostFunction<InputImageType, DTMType, LUTType> MultiSpectralAOTCostFunctionType;
            typedef typename MultiSpectralAOTCostFunctionType::Pointer MultiSpectralAOTCostFunctionPointerType;

            /* Getters/Setters */
            void SetLUTD(LUTType * pLUTD)
            {
                m_MSAOTCostFunction->SetLUTD(pLUTD);
                this->m_LUTD = pLUTD;
                this->Modified();
            }

            // Method added only for TU
             void SetMSLUTD(LUTType * pLUTD)
             {
                 m_MSAOTCostFunction->SetLUTD(pLUTD);
                 this->Modified();
             }


            void SetDTMSub(DTMNeighborhoodIteratorType & pDTMSub)
            {
                m_MSAOTCostFunction->SetDTMSub(pDTMSub);
                this->m_DTMSub = pDTMSub;
                this->Modified();
            }

            // Method added only for TU
            void SetMSDTMSub(DTMNeighborhoodIteratorType & pDTMSub)
            {
                m_MSAOTCostFunction->SetDTMSub(pDTMSub);
                this->Modified();
            }


            void SetIPTOACSub(NeighborhoodIteratorType & pIPTOACSub)
            {
                m_MSAOTCostFunction->SetIPTOACSub(pIPTOACSub);
                this->m_IPTOACSub = pIPTOACSub;
                this->Modified();
            }

            // Method added only for TU
            void SetMSIPTOACSub(NeighborhoodIteratorType & pIPTOACSub)
            {
                m_MSAOTCostFunction->SetIPTOACSub(pIPTOACSub);
                this->Modified();
            }

            void SetWeqMS(const double & pWeqMS)
            {
                m_MSAOTCostFunction->SetWeq1(pWeqMS);
                this->Modified();
            }

            double GetWeqMS() const
            {
                return m_MSAOTCostFunction->GetWeq1();
            }

            void SetSlopeMS(const double & pSlopeMS)
            {
                m_MSAOTCostFunction->SetSlopeMS(pSlopeMS);
                this->Modified();
            }
            double GetSlopeMS() const
            {
                return m_MSAOTCostFunction->GetSlopeMS();
            }

            void SetYInterceptMS(const double & pYInterceptMS)
            {
                m_MSAOTCostFunction->SetYInterceptMS(pYInterceptMS);
                this->Modified();
            }

            double GetYInterceptMS() const
            {
                return m_MSAOTCostFunction->GetYInterceptMS();
            }

            void SetMSOffsetVect(const OffsetVectType & pOffsetVect)
            {
                m_MSAOTCostFunction->SetOffsetVect(pOffsetVect);
                this->Modified();
            }
            void SetSpaceDimension(const unsigned int pSpaceDimension)
            {
                m_MSAOTCostFunction->SetSpaceDimension(1);
                this->m_SpaceDimension = pSpaceDimension;
                this->Modified();
            }

            void SetMSAOTbands(const ListOfUIntegers & pAOTbands)
            {
                m_MSAOTCostFunction->SetAOTbands(pAOTbands);
                this->Modified();
            }
            ListOfUIntegers GetMSAOTbands() const
            {
                return m_MSAOTCostFunction->GetAOTbands();
            }

            void SetNDVIAOTBand1(const unsigned int & pBand)
            {
                m_MSAOTCostFunction->SetNDVIAOTBand1(pBand);
                this->Modified();
            }
            unsigned int GetNDVIAOTBand1()
            {
                return m_MSAOTCostFunction->GetNDVIAOTBand1();
            }

            void SetNDVIAOTBand2(const unsigned int & pBand)
            {
                 m_MSAOTCostFunction->SetNDVIAOTBand2(pBand);
                 this->Modified();
            }
            unsigned int GetNDVIAOTBand2()
            {
                return m_MSAOTCostFunction->GetNDVIAOTBand2();
            }

            void SetSlopeNDVI(const double & pParam)
            {
                m_MSAOTCostFunction->SetSlopeNDVI(pParam);
                this->Modified();
            }
            double GetSlopeNDVI() const
            {
                return m_MSAOTCostFunction->GetSlopeNDVI();
            }


            void SetYInterceptNDVI(const double & pParam)
            {
                m_MSAOTCostFunction->SetYInterceptNDVI(pParam);
                this->Modified();
            }
            double GetYInterceptNDVI() const
            {
                return m_MSAOTCostFunction->GetYInterceptNDVI();
            }

            void SetSTMeasureSize(unsigned int pMTSize, unsigned int pMSSize)
            {
            	// Number of bands for the product of date D
                unsigned int lNbOfBands = this->GetAOTbandsD().size();
                // MT Pixels * 2 cost functions * MT AOT bands + MS pixels * 1 cost function * 1 MS AOT band
                this->m_RangeDimension = pMTSize * 2 * lNbOfBands + pMSSize + 1;
                // Note :  + 1 because we add of the 2 bounds errors  and the dark errors
                //Wae add one since cams add one dims
                if(this->m_UseCamsData){
                	this->m_RangeDimension++;
                }
                this->m_Measure.SetSize(this->m_RangeDimension);
                this->m_Measure.fill(0);
                this->m_Derivative.SetSize(this->m_SpaceDimension, this->m_RangeDimension);
                this->m_Derivative.fill(0);

                // Initialize the multi spectral method
                m_MSAOTCostFunction->SetMeasureSize(pMSSize);

                // Initialize the booleans that indicate if the MT method or the MS one is valid
                m_MTIsValid = false;
                if (pMTSize > 0)
                {
                    m_MTIsValid = true;
                }

                m_MSIsValid = false;
                if (pMSSize > 0)
                {
                    m_MSIsValid = true;
                }

                this->Modified();
            }

            MeasureType GetValue( const ParametersType & parameters ) const;

            void EvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD, double pAOTcurrentDm1, MeasureType & pSTMeasure) const;

            void DerivativeEvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD, double pAOTcurrentDm1, DerivativeType & pSTDerivative) const;

        protected:
            SpectroTemporalAOTCostFunction(void);
            virtual ~SpectroTemporalAOTCostFunction(void){};

            virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            SpectroTemporalAOTCostFunction(const Self&); //purposely not implemented
            void operator=(const Self&); //purposely not implemented

            MultiSpectralAOTCostFunctionPointerType m_MSAOTCostFunction;

            // Booleans that define if some pixels are valid for the two methods.
            bool m_MTIsValid;
            bool m_MSIsValid;

        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsSpectroTemporalAOTCostFunction.txx"
#endif

#endif
