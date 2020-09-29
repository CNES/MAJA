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
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 18 oct. 2011 : Méthode multi temporelle                         *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsMultiTemporalAOTCostFunction_h
#define __vnsMultiTemporalAOTCostFunction_h

#include "vnsAOTCostFunctionBase.h"
#include "vnsUtilities.h"
#include "vnsMacro.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkFixedArray.h"
#include <iostream>



namespace vns
{

/** \class MultiTemporalAOTCostFunction
 * \brief : This class minimize the equations
 *
 *   cost1 = Weq1 * sum l,p,b { [ Rsurf D-1(l,p,b,AOT(D-1)) - Rsurf D(l,p,b,AOT(D))]² }
 *
 *   cost2 = Weq2 * sum l,p,b { [ Rsurf D(l,p,b,AOT(D)) - Rsurf D(l,p,b)]² }
 *
 *   vector_cost =  [ cost1(pix1) ; cost2(pix1) ; cost1(pix2) ; cost2(pix2) ; cost_bound + cost_dark ]
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
template <class TInputImage, class TInputDTM, class TLUT>
class MultiTemporalAOTCostFunction : public AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>
{
public:
  /** Standard class typedefs. */
  typedef MultiTemporalAOTCostFunction Self;
  typedef AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>     Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(MultiTemporalAOTCostFunction, AOTCostFunctionBase );

  /** Some convenient typedefs. */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::Pointer           InputImagePointerType;
  typedef typename InputImageType::ConstPointer      InputImageConstPointer;
  typedef typename InputImageType::RegionType        RegionType;
  typedef typename InputImageType::IndexType         IndexType;
  typedef typename InputImageType::PixelType         PixelType;
  typedef typename InputImageType::InternalPixelType InternalPixelType;
  typedef typename InputImageType::SizeType          SizeType;
  typedef typename InputImageType::OffsetType        OffsetType;

  /** Look up table template typedefs */
  typedef TLUT                             LUTType;
  typedef typename LUTType::Pointer        LUTPointerType;
  typedef typename LUTType::PixelType      LUTPixelType;
  typedef typename LUTType::PointType      LUTPointType;
  typedef typename LUTPointType::ValueType LUTPointValueType;
  typedef typename LUTType::ParameterValuesType      LUTParameterValuesType;

  typedef TInputDTM                        DTMType;
  typedef typename DTMType::PixelType      DTMPixelType;
  typedef typename LUTType::Pointer        DTMPointerType;

    /* Neighborhood Iterator related type */
  typedef itk::ConstNeighborhoodIterator<InputImageType>        NeighborhoodIteratorType;
  typedef itk::ConstNeighborhoodIterator<DTMType>               DTMNeighborhoodIteratorType;

  typedef itk::FixedArray<double,4>     FixedArrayType;
  typedef Utilities::ListOfUIntegers                      ListOfUIntegers;
  typedef std::vector<OffsetType>       OffsetVectType;

  typedef typename Superclass::ParametersType              ParametersType;
  typedef typename Superclass::DerivativeType              DerivativeType;
  typedef typename Superclass::MeasureType                 MeasureType;


  /* Getters/Setters */

  itkSetObjectMacro(LUTDm1,LUTType);
  itkGetObjectMacro(LUTDm1,LUTType);

  void SetL2inRTA(NeighborhoodIteratorType & pL2inRTA)
  {
      m_L2inRTA = pL2inRTA;
      this->Modified();
  }
//  itkSetMacro(L2inRTA,NeighborhoodIteratorType);
  itkGetConstMacro(L2inRTA,NeighborhoodIteratorType);

  void SetL2inRTC(NeighborhoodIteratorType & pL2inRTC)
  {
      m_L2inRTC = pL2inRTC;
      this->Modified();
  }
  //itkSetMacro(L2inRTC,NeighborhoodIteratorType);
  itkGetConstMacro(L2inRTC,NeighborhoodIteratorType)

  itkSetMacro(Weq2,double)
  itkGetMacro(Weq2,double)


  vnsSetMacro(AOTbandsD,ListOfUIntegers)
  vnsGetMacro(AOTbandsD,ListOfUIntegers)
  vnsSetMacro(AOTbandsReflectanceCompositeDm1,ListOfUIntegers)
  vnsGetMacro(AOTbandsReflectanceCompositeDm1,ListOfUIntegers)
  vnsSetMacro(AOTbandsLUTCompositeDm1,ListOfUIntegers)
  vnsGetMacro(AOTbandsLUTCompositeDm1,ListOfUIntegers)

  void SetMeasureSize(unsigned int pSize)
   {

       unsigned int lNbOfBands = m_AOTbandsD.size();
       this->m_RangeDimension = pSize*2*lNbOfBands + 1;
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



  MeasureType GetValue( const ParametersType & parameters ) const;
  void EvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD, double pAOTcurrentDm1, MeasureType & pMTMeasure) const;

  void GetDerivative(const ParametersType & parameters, DerivativeType & derivative) const;
  void DerivativeEvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD, double pAOTcurrentDm1, DerivativeType & pMTDerivative) const;


protected:
  MultiTemporalAOTCostFunction( void );
  virtual ~MultiTemporalAOTCostFunction( void ) {};

  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const;



private:
  MultiTemporalAOTCostFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  NeighborhoodIteratorType m_L2inRTA;     // L2_in.R_toa*_sub(D-1,PixelList)
  NeighborhoodIteratorType m_L2inRTC;     // L2_in.R_surf_sub(D-1,PixelList)

  /** AOT bands definition for the date D */
  ListOfUIntegers m_AOTbandsD;

  /** AOT bands definition for the date D-1 for the reflectance composite */
  ListOfUIntegers m_AOTbandsReflectanceCompositeDm1;

  /** AOT bands definition for the date D-1 for the LUT (LTC) composite */
  ListOfUIntegers m_AOTbandsLUTCompositeDm1;

  /** Weight for inversion equation 2 */
  double m_Weq2;


  /** AOT LUT model for the date LPD */
  LUTPointerType  m_LUTDm1;


};

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMultiTemporalAOTCostFunction.txx"
#endif

#endif
