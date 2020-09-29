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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Supression Observer                       *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 oct. 2011 : Création des différentes méthodes de calcul des AOT *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsRunLevenbergMarquardtOptimization_txx
#define __vnsRunLevenbergMarquardtOptimization_txx

#include "vnsRunLevenbergMarquardtOptimization.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TInputDTM, class TInputLUT, class TOutputImage, class TCostFunction>
        RunLevenbergMarquardtOptimization<TInputImage, TInputDTM, TInputLUT, TOutputImage, TCostFunction>::RunLevenbergMarquardtOptimization()
        {
            // Declaration of a itkOptimizer
            m_Optimizer = OptimizerType::New();
            // Declaration of the CostFunction adaptor
            m_CostFunction = AOTCostFunctionType::New();
        }

    // Destructor
    template<class TInputImage, class TInputDTM, class TInputLUT, class TOutputImage, class TCostFunction>
        typename RunLevenbergMarquardtOptimization<TInputImage, TInputDTM, TInputLUT, TOutputImage, TCostFunction>::OutputImagePixelType
        RunLevenbergMarquardtOptimization<TInputImage, TInputDTM, TInputLUT, TOutputImage, TCostFunction>::RunLMOptimization(double fTolerance,
                double gTolerance, double xTolerance, double epsilonFunction, int maxIterations, double AOTinitialD, double AOTinitialDm1)
        {
            try
            {
                m_Optimizer->SetCostFunction(m_CostFunction.GetPointer());
            }
            catch (itk::ExceptionObject & e)
            {
                std::string lError("RunLevenbergMarquardtOptimization::RunLMOptimization :\n An error occurred during Optimization.\n");
                lError = lError + e.GetDescription();
                vnsExceptionBusinessMacro(lError);
            }

            // Automatic computation of the gradient
            // Note that Derivative functions developed in the cost function have not been validated
            // The derivative function of the spectro temporal method is not developed.
            m_Optimizer->UseCostFunctionGradientOff();

            // Use derivative function implemented in AOTCostFunction
            //m_Optimizer->UseCostFunctionGradientOn();

            vnlOptimizerType * vnlOptimizer = m_Optimizer->GetOptimizer();

            vnlOptimizer->set_f_tolerance(fTolerance);
            vnlOptimizer->set_g_tolerance(gTolerance);
            vnlOptimizer->set_x_tolerance(xTolerance);
            vnlOptimizer->set_epsilon_function(epsilonFunction);
            vnlOptimizer->set_max_function_evals(maxIterations);

            // We start not so far from the solution
            //typedef LMCostFunction::ParametersType ParametersType;
            const unsigned int lSpaceDimension = m_CostFunction->GetSpaceDimension();
            ParametersType initialValue(lSpaceDimension);

            // Values to find
            initialValue[0] = AOTinitialD;

            if (lSpaceDimension == 2)
            {
                initialValue[1] = AOTinitialDm1;
            }

            // Set initial position for the values to find (AOT(D) and AOT (D-1) for MT method)
            m_Optimizer->SetInitialPosition(initialValue);

            try
            {
                m_Optimizer->StartOptimization();
            }
            catch (itk::ExceptionObject & e)
            {
                std::string lError("RunLevenbergMarquardtOptimization::RunLMOptimization :\n An error occurred during Optimization.");
                lError = lError + "\nLocation    = " + e.GetLocation();
                lError = lError + "\nDescription = " + e.GetDescription();
                vnsExceptionBusinessMacro(lError);
            }

            ParametersType finalPosition;
            finalPosition = m_Optimizer->GetCurrentPosition();

            // USeless here, and uncommented, it generates an infinite loop !!!
            //vnsLogDebugMacro("RunLevenbergMarquardtOptimization, result for RunLMOptimization : "<< finalPosition);

            // Return the final AOT value (last evenbergMarquardt iteration)
            return static_cast<OutputImagePixelType> (finalPosition[0]);
        }

    // PrintSelf method
    template<class TInputImage, class TInputDTM, class TInputLUT, class TOutputImage, class TCostFunction>
        void
        RunLevenbergMarquardtOptimization<TInputImage, TInputDTM, TInputLUT, TOutputImage, TCostFunction>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsRunLevenbergMarquardtOptimization_txx */
