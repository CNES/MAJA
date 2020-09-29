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
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-751-CNES : 17 février 2014 : Sélection des pixels pour l'inversion    *
 * 																	(Spec v2.2)								*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsRayleighCorrectionImageFilterTest.cxx 6888 2013-11-29 14:08:29Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsLoggers.h"
#include "vnsMultiTemporalAOTMethodThresholdsGenerator.h"
#include "vnsVectorLookUpTable.h"
#include "vnsDataManagerTestingLibrary.h"


int vnsMultiTemporalAOTMethodThresholdsGeneratorTest(int argc, char * argv [])
{
    if(argc!=12)
    {
    	std::cerr << argv[0] << " <LUT filename> <Blue band code>"
    			" <AOTMin> <AOTMax> <TOAReflMin> <TOAReflMax> <TOAReflStep> <Min Difference thresholds> <output filename>"<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    unsigned int cpt(1);

    const char * l_LUTFileName    = argv[cpt++];
    double l_AOTBand1 = atof(argv[cpt++]);
    double l_AOTBand2 = atof(argv[cpt++]);
    double l_FirstAOT = atof(argv[cpt++]);
    double l_SecondAOT = atof(argv[cpt++]);
    double l_TOAReflMin = atof(argv[cpt++]);
    double l_TOAReflMax = atof(argv[cpt++]);
    double l_TOAReflStep = atof(argv[cpt++]);
    double l_MinDiffTresholds = atof(argv[cpt++]);
    double l_AltitudeMean = atof(argv[cpt++]);
    
    const char * l_OutputFileName = argv[cpt++];

    /** Image typedefs */
    typedef vns::VectorLookUpTable<double, 3>           LUTType;
    typedef LUTType::ParameterValuesType                ParameterValuesType;

    typedef vns::MultiTemporalAOTMethodThresholdsGenerator<LUTType>  FilterType;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();

    /** Load the LUT */
    LUTType::Pointer l_LUT = LUTType::New();
    std::fstream lFile;
    lFile.open(l_LUTFileName, std::ios::in | std::ios::binary);
    if (lFile.fail())
      {
        std::cout << "vnsMultiTemporalAOTMethodThresholdsGeneratorTest : Failed to open LUT file ! "<<l_LUTFileName <<std::endl;
      }
    unsigned int lNbOfBands = 4;
    l_LUT->SetNumberOfComponentsPerPixel( lNbOfBands );

    // rappel : ordre du moins invariant au plus invariant
    // Add Parameters to the LUT
    std::string str1("0 1000 2000 3000");
    std::string str2("0.0000 0.0625 0.1250 0.1875 0.2500 0.3125 0.3750 0.4375 0.5000 0.5625 0.6250 0.6875 0.7500 0.8125 0.8750 0.9375 1.0000 1.0625 1.1250 1.1875 1.2500 1.3125 1.3750 1.4375 1.5000");
    std::string str3("-0.2000 -0.1300 -0.0600 0.0100 0.0800 0.1500 0.2200 0.2900 0.3600 0.4300 0.5000 0.5700 0.6400 0.7100 0.7800");

    const vns::Utilities::ListOfDoubles Altitude_Indexes = vns::Utilities::SplitStringAsDouble(str1);
    const vns::Utilities::ListOfDoubles AOT_Indexes = vns::Utilities::SplitStringAsDouble(str2);
    const vns::Utilities::ListOfDoubles TOA_Reflectance_Indexes = vns::Utilities::SplitStringAsDouble(str3);

    ParameterValuesType newParam1;
    newParam1.ParameterName="Altitude_Indexes";
    newParam1.ParameterValues=Altitude_Indexes;
    l_LUT->AddParameterValues(newParam1);

    ParameterValuesType newParam2;
    newParam2.ParameterName="AOT_Indexes";
    newParam2.ParameterValues=AOT_Indexes;
    l_LUT->AddParameterValues(newParam2);

    ParameterValuesType newParam3;
    newParam3.ParameterName="TOA_Reflectance_Indexes";
    newParam3.ParameterValues=TOA_Reflectance_Indexes;
    l_LUT->AddParameterValues(newParam3);

    // Read and load values
    LUTType::ParametersValuesType getValues;
    getValues = l_LUT->GetParametersValues();

    int size[getValues.size()];
    for (unsigned int i = 0; i < getValues.size(); i++)
      {
        size[i] = getValues[i].ParameterValues.size();
      }

    // Parcours de la LUT et chargement
    LUTType::IndexType lutIndex;
    double lVal;
    float var(0.);

    // number of components per pixel
    for (unsigned int lNumBand = 0; lNumBand < lNbOfBands; lNumBand++)
    {
        for (int i = 0; i < size[0]; i++)
        {
        	lutIndex[0] = i;
        	for (int j = 0; j < size[1]; j++)
            {
        		lutIndex[1] = j;
        		for (int k = 0; k < size[2]; k++)
                {
                    lutIndex[2] = k;

                    lFile.read((char*) (&var), 4);
                    lVal = var;
                    l_LUT->SetValue(lutIndex, lNumBand, lVal);
                }
            }
        }
    }
    lFile.close();

    std::vector<unsigned int> l_MTAOTBands;
    l_MTAOTBands.push_back(l_AOTBand1);
    l_MTAOTBands.push_back(l_AOTBand2);

    /** Link to the filter */
    l_Filter->SetLUT( l_LUT );

    l_Filter->SetMTAOTBands(l_MTAOTBands);
    l_Filter->SetFirstAOT(l_FirstAOT);
    l_Filter->SetSecondAOT(l_SecondAOT);
    l_Filter->SetTOAReflMin(l_TOAReflMin);
    l_Filter->SetTOAReflMax(l_TOAReflMax);
    l_Filter->SetTOAReflStep(l_TOAReflStep);
    l_Filter->SetMinDiffThreholds(l_MinDiffTresholds);
    l_Filter->SetAltitudeMean(l_AltitudeMean);

	l_Filter->ComputeReflectanceThresholds();

    std::cout<<"Parameters : "<<l_Filter<<std::endl;

    std::ofstream file;
    file.open(l_OutputFileName);
    for (unsigned int b=0; b<l_MTAOTBands.size(); b++)
    {
		file<< "Low threshold TOA :  " << l_Filter->GetLowReflectanceThreshold()[b] << std::endl;
		file<< "High threshold TOA :  " << l_Filter->GetHighReflectanceThreshold()[b] << std::endl;
    }
	file.close();


    return EXIT_SUCCESS;
}
