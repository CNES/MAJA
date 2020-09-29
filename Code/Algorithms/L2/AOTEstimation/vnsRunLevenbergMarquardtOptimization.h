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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 10 avril 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsRunLevenbergMarquardtOptimization_h
#define __vnsRunLevenbergMarquardtOptimization_h

#include "vnsMultiTemporalAOTCostFunction.h"
#include "vnsMultiSpectralAOTCostFunction.h"
#include "vnsLookUpTable.h"
#include "otbImage.h"
#include "itkFixedArray.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkLevenbergMarquardtOptimizer.h"
#include "vnsCommandIterationUpdateLevenbergMarquardt.h"
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_math.h>
#include "itkProcessObject.h"

#include <iostream>


namespace vns
{
/** \class  RunLevenbergMarquardtOptimization
 * \brief This class implements .
 *
 * Use Levenberg Marquardt Optimization and AOT cost function to compute the AOT value for each pixel.
 *
 * \author CS Systemes d'Information
 *
 * \sa AOTCostFunction
 *
 * TInputImage  : vector image of double expected
 * TInputMask   : Monochannel image of short expected
 * TInputLUT    : Vector LUT of double
 *
 * \ingroup L2
 *
 */
template <class TInputImage, class TInputDTM, class TInputLUT, class TOutputImage, class TCostFunction>
class RunLevenbergMarquardtOptimization : public itk::ProcessObject
{
public:

    /** Standard class typedefs. */
     typedef RunLevenbergMarquardtOptimization                      Self;
     typedef itk::ProcessObject                                     Superclass;
     typedef itk::SmartPointer<Self>                                Pointer;
     typedef itk::SmartPointer<const Self>                          ConstPointer;

     /** Type macro */
     itkNewMacro(Self);
     /** Creation through object factory macro */
     itkTypeMacro(RunLevenbergMarquardtOptimization, ProcessObject );

    typedef TInputLUT                                               LUTType;
    typedef typename LUTType::Pointer                               LUTPointerType;

    typedef TInputImage                                             VectorImageType;
    typedef typename VectorImageType::Pointer                       VectorImagePointerType;
    typedef typename VectorImageType::IndexType                     IndexType;
    typedef typename VectorImageType::PixelType                     PixelType;
    typedef typename VectorImageType::SizeType                      SizeType;
    typedef typename IndexType::IndexValueType                      IndexValueType;
    typedef typename VectorImageType::OffsetType                    OffsetType;

    typedef TInputDTM                                               DTMType;
    typedef typename DTMType::PixelType                             DTMPixelType;
    typedef typename DTMType::Pointer                               DTMPointerType;

    typedef TOutputImage                                            OutputImageType;
    typedef typename OutputImageType::Pointer                       OutputImagePointerType;
    typedef typename OutputImageType::PixelType                     OutputImagePixelType;

    typedef itk::ConstNeighborhoodIterator<VectorImageType>         NeighborhoodIteratorType;
    typedef itk::ConstNeighborhoodIterator<DTMType>                 DTMNeighborhoodIteratorType;

    typedef itk::FixedArray<double,4>                               FixedArrayType;
    typedef itk::LevenbergMarquardtOptimizer                        OptimizerType;
    typedef typename OptimizerType::Pointer                         OptimizerPointerType;
    typedef typename OptimizerType::InternalOptimizerType           vnlOptimizerType;


    typedef TCostFunction                                           AOTCostFunctionType;
    typedef typename AOTCostFunctionType::Pointer                   AOTCostFunctionPointerType;
    typedef typename AOTCostFunctionType::ParametersType            ParametersType;

    typedef std::vector<IndexType>                                  IndexListType;
    typedef std::vector<PixelType>                                  PixelVectType;
    typedef std::vector<OffsetType>                                 OffsetVectType;
    typedef std::vector<unsigned int>                               UIVectType;

    /** Getters/Setters */

    AOTCostFunctionPointerType GetCostFunction()
    {
        return m_CostFunction;
    }

    OutputImagePixelType RunLMOptimization(   double fTolerance,
                                         double gTolerance,
                                         double xTolerance,
                                         double epsilonFunction,
                                         int    maxIterations,
                                         double AOTinitialD,
                                         double AOTinitialDm1);


protected:
  RunLevenbergMarquardtOptimization( void );
  virtual ~RunLevenbergMarquardtOptimization( void ) {};

  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const;



private:
  RunLevenbergMarquardtOptimization( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

    /** AOT Cost Function */
    AOTCostFunctionPointerType m_CostFunction;

    /** Optimizer */
    OptimizerPointerType  m_Optimizer;
};

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsRunLevenbergMarquardtOptimization.txx"
#endif

#endif /* __vnsRunLevenbergMarquardtOptimization_h */
