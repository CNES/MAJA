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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 11 février 2014 : Améliorations algorithmiques (Spec v2.1)	*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsMultiTemporalAOTMethodThresholdsGenerator.cxx 5749 2013-05-29 14:44:02Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMultiTemporalAOTMethodThresholdsGenerator_txx
#define __vnsMultiTemporalAOTMethodThresholdsGenerator_txx

#include "vnsMultiTemporalAOTMethodThresholdsGenerator.h"
#include "vnsLoggers.h"

namespace vns
{

    // Constructor
	template<class TLUTInput>
    MultiTemporalAOTMethodThresholdsGenerator<TLUTInput>::MultiTemporalAOTMethodThresholdsGenerator() :
    	m_FirstAOT(0.), m_SecondAOT(0.), m_TOAReflMin(0.), m_TOAReflMax(0.), m_TOAReflStep(0.),
    	m_AltitudeMean(0.), m_MinDiffThreholds(0.)
    {
		m_MTAOTBands.clear();
		m_LowReflectanceThreshold.clear();
		m_HighReflectanceThreshold.clear();
    }

    // Destructor
	template<class TLUTInput>
    MultiTemporalAOTMethodThresholdsGenerator<TLUTInput>::~MultiTemporalAOTMethodThresholdsGenerator()
    {
    }

    // Get sunglint distance
	template<class TLUTInput>
    void
    MultiTemporalAOTMethodThresholdsGenerator<TLUTInput>::ComputeReflectanceThresholds()
    {

		if (vnsEqualsDoubleMacro(m_TOAReflStep, 0.) == true)
		{
			vnsStaticExceptionBusinessMacro("MultiTemporalAOTMethodThresholdsGenerator::ComputeReflectanceThresholds : the reflectance step is null !");
		}

    	// Check LUT availability
		if (m_LUT.IsNull() == true)
		{
			vnsExceptionBusinessMacro("Waiting for a LUT, but no one specified. Please set one.");
		}

		LUTPointType point1;
		LUTPointType point2;

		// Set the constant point value of the LUT
		point1[0] = static_cast<LUTPointValueType> (m_AltitudeMean);
		point1[1] = static_cast<LUTPointValueType> (m_FirstAOT);

		point2[0] = static_cast<LUTPointValueType> (m_AltitudeMean);
		point2[1] = static_cast<LUTPointValueType> (m_SecondAOT);

		DoubleVectType invalidRefl;

		const unsigned int nbBands = m_MTAOTBands.size();

		for (unsigned int band=0; band<nbBands; band++)
		{
			const unsigned int curBand = m_MTAOTBands[band];

			//	Two lists of surface reflectances are built according the LUT, the image angles, the mean altitude of the site and the TOA reflectances varying from 0.0 to 1.0 with a step of 0.01:
			//	LUT(list(rho_toa ), AOT ,h_DTM, thetaS, phiS, thetaV, phiV) =  list(rho_surf )

			//	-	The first list is for AOTMin
			//	-	The second list is for AOTMax

			double reflTOA(m_TOAReflMin);

			double lowThreshold(m_TOAReflMax);
			double highThreshold(m_TOAReflMin);

			invalidRefl.clear();

			const double reflMax = m_TOAReflMax + m_TOAReflStep;

			while (reflTOA < reflMax)
			{
				// Get the surface reflectance for a given TOA reflectance
				point1[2] = static_cast<LUTPointValueType> (reflTOA);
				const LUTPixelType reflSurf1 = m_LUT->InterpolateValue(point1);

				point2[2] = static_cast<LUTPointValueType> (reflTOA);
				const LUTPixelType reflSurf2 = m_LUT->InterpolateValue(point2);

				// Then the thresholds are computed in order to give the TOA reflectances for which the surface reflectance
				// difference is larger than Min_difference_thresholds_calculation

				const double reflSurfDiff = vcl_abs(reflSurf1[curBand] - reflSurf2[curBand]);

				if (reflSurfDiff <= m_MinDiffThreholds)
				{
					invalidRefl.push_back(reflTOA);
				}
				reflTOA += m_TOAReflStep;
			} // end while

			// If some TOA reflectances for which the difference of the surface reflectances is lower than
			// a threshold was found
			if (invalidRefl.size() != 0)
			{
				lowThreshold = *(std::min_element(invalidRefl.begin(), invalidRefl.end()));
				highThreshold = *(std::max_element(invalidRefl.begin(), invalidRefl.end()));
			}

			m_LowReflectanceThreshold.push_back(lowThreshold);
			m_HighReflectanceThreshold.push_back(highThreshold);

			vnsLogDebugMacro("AOT Estimation - Low TOA Reflectance Threshold - band " <<curBand << " : "<< m_LowReflectanceThreshold.back())
			vnsLogDebugMacro("AOT Estimation - High TOA Reflectance Threshold - band " <<curBand << " : "<< m_HighReflectanceThreshold.back())

		} // end MT AOT bands loop

    }

	// PrintSelf method
	template<class TLUTInput>
	void
	MultiTemporalAOTMethodThresholdsGenerator<TLUTInput>::PrintSelf(std::ostream& os, itk::Indent indent) const
	{
		Superclass::PrintSelf(os, indent);

		os << "First AOT value: " << m_FirstAOT << std::endl;
		os << "Second AOT value:  " << m_SecondAOT << std::endl;

		os << "TOA reflectance min:  " << m_TOAReflMin << std::endl;
		os << "TOA reflectance max:  " << m_TOAReflMax << std::endl;
		os << "TOA reflectance step:  " << m_TOAReflStep << std::endl;

		os << "Mean altitude :  " << m_AltitudeMean << std::endl;

		os << "Min difference thresholds : " << m_MinDiffThreholds<<std::endl;


	}

} // End namespace vns

#endif /* __vnsMultiTemporalAOTMethodThresholdsGenerator_txx */
