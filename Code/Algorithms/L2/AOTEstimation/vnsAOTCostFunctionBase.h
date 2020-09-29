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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 17 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 oct. 2011 : Création des différentes méthodes de calcul des AOT *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsAOTCostFunctionBase_h
#define __vnsAOTCostFunctionBase_h

#include "itkMultipleValuedCostFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkFixedArray.h"
#include <iostream>



namespace vns
{

/** \class AOTCostFunctionBase
 * \brief : This class implements the calculation of constraints to maintain AOT retrieval
 *  within [AOTmin, AOTmax] : Error_Bound and Dark_Error
 *
 *  The cost functions depend on the method and are implemented
 *  in the specific AOT cost functions that derive from this class.
 *
 *
 * \sa itkResampleImageFilter
 * \sa itkLinearInterpolationImageFilter
 *
 */ 
template <class TInputImage, class TInputDTM, class TLUT>
class AOTCostFunctionBase : public itk::MultipleValuedCostFunction
{
public:
  /** Standard class typedefs. */
  typedef AOTCostFunctionBase Self;
  typedef itk::MultipleValuedCostFunction Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory macro */
  itkTypeMacro(AOTCostFunctionBase, itkMultipleValuedCostFunction );

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

  typedef Superclass::ParametersType              ParametersType;
  typedef Superclass::DerivativeType              DerivativeType;
  typedef Superclass::MeasureType                 MeasureType;

    /* Neighborhood Iterator related type */
  typedef itk::ConstNeighborhoodIterator<InputImageType>        NeighborhoodIteratorType;
  typedef itk::ConstNeighborhoodIterator<DTMType>               DTMNeighborhoodIteratorType;

  typedef itk::FixedArray<double,4>     FixedArrayType;
  typedef std::vector<unsigned int>     UIVectType;
  typedef std::vector<OffsetType>       OffsetVectType;


  /* Getters/Setters */
  itkSetObjectMacro(LUTD,LUTType);
  itkGetObjectMacro(LUTD,LUTType);

  virtual void SetDTMSub(DTMNeighborhoodIteratorType & pDTMSub)
  {
      m_DTMSub = pDTMSub;
      this->Modified();
  }
  //itkSetMacro(DTMSub,DTMNeighborhoodIteratorType);
  itkGetConstMacro(DTMSub,DTMNeighborhoodIteratorType);

  virtual void SetIPTOACSub(NeighborhoodIteratorType & pIPTOACSub)
  {
      m_IPTOACSub = pIPTOACSub;
      this->Modified();
  }


//  itkSetMacro(IPTOACSub,NeighborhoodIteratorType);
  itkGetConstMacro(IPTOACSub,NeighborhoodIteratorType);


  itkSetMacro(DarkestPixelAOT,double);
  itkGetMacro(DarkestPixelAOT,double);

  itkSetMacro(Weq1,double);
  itkGetMacro(Weq1,double);

  itkSetMacro(Wdark,double);
  itkGetMacro(Wdark,double);

  itkSetMacro(Wbounds,double);
  itkGetMacro(Wbounds,double);

  itkSetMacro(AOTmin,double);
  itkGetMacro(AOTmin,double);

  itkSetMacro(AOTmax,double);
  itkGetMacro(AOTmax,double);

  //CAMS related attribute
  itkSetMacro(UseCamsData,bool);
  itkGetMacro(UseCamsData,bool);
  itkSetMacro(KPondCams,double);
  itkGetMacro(KPondCams,double);
  itkSetMacro(CAMSAot,double);
  itkGetMacro(CAMSAot,double);


  virtual void SetOffsetVect(const OffsetVectType & pOffsetVect)
  {
      m_OffsetVect = pOffsetVect;
      this->Modified();
  }

  virtual void SetSpaceDimension(const unsigned int pSpaceDimension)
  {
      m_SpaceDimension = pSpaceDimension;
      this->Modified();
  }

  virtual unsigned int GetSpaceDimension()
  {
      return m_SpaceDimension;
  }

  virtual void SetMeasureSize(unsigned int pSize) = 0;

  double EvaluateDarkError(double pAOTcurrentD) const;
  double EvaluateErrorBound(double pAOTmin, double pAOTmax, double pAOTcurrentT) const;

  double DerivativeEvaluateDarkError(double pAOTcurrentD) const;
  double DerivativeEvaluateErrorBound(double pAOTmin, double pAOTmax, double pAOTcurrentT) const;

  unsigned int GetNumberOfParameters(void) const;
  unsigned int GetNumberOfValues(void) const;
  void FindAOTmax(void);
  double LimitAOT(const double & pAOT) const;



protected:
  AOTCostFunctionBase( void );
  virtual ~AOTCostFunctionBase( void ) {};

  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const;

  mutable MeasureType       m_Measure;
  mutable DerivativeType    m_Derivative;

  NeighborhoodIteratorType m_IPTOACSub;   // R_toa*_sub(D,PixelList))
  DTMNeighborhoodIteratorType m_DTMSub; // mono channel neighborhood of DTM


  unsigned int m_DTMsub;
  double m_DarkestPixelAOT;
  double m_Weq1;
  double m_Wdark;
  double m_Wbounds;
  double m_AOTmin;
  double m_AOTmax;

  //CAMS related attribute
  bool m_UseCamsData;
  double m_KPondCams;
  double m_CAMSAot;

  /** List of offsets of the used pixels in the Neighborhood */
  OffsetVectType m_OffsetVect;

  /** AOT LUT model for the date D */
  LUTPointerType  m_LUTD;

  /** Nb Of Parameters */
  unsigned int m_SpaceDimension;

  /** Nb of expected values in the vector to minimize */
  unsigned int m_RangeDimension;



private:
  AOTCostFunctionBase( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

};

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsAOTCostFunctionBase.txx"
#endif

#endif
