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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsRunLevenbergMarquardtOptimization.h"
#include "vnsMultiTemporalAOTCostFunction.h"
#include "vnsLookUpTableFileReader.h"
#include "vnsLoggers.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "vnsVectorLookUpTable.h"
#include "otbImageFileReader.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "vnsUtilities.h"


int vnsRunLevenbergMarquardtOptimizationTest(int argc, char * argv [])
{

    if (argc != 15)
    {
        return EXIT_FAILURE;
    }

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);


    /********************/
    /** Get parameters **/
    /********************/

    // 3 images
    const char * IPTOACSubFileName = argv[1];
    const char * L2inRTASubFileName = argv[2];
    const char * L2inRTCSubFileName = argv[3];
    // 1 dtm image
    const char * L2inDTMSubFileName = argv[4];
    // 1 LUT
    const char * miniLUTFileNameD =  argv[5];
    const char * miniLUTFileNameDm1 =  argv[6];

    unsigned int lDarkestPixelAOT = static_cast<unsigned int> (atoi(argv[7]));
    double lWeq1 = static_cast<double> (atof(argv[8]));
    double lWeq2 = static_cast<double> (atof(argv[9]));
    double lWdark = static_cast<double> (atof(argv[10]));
    double lWbounds = static_cast<double> (atof(argv[11]));
    double lAOTmin = static_cast<double> (atof(argv[12]));
    unsigned int lNbOfUsedBands = static_cast<unsigned int> (atoi(argv[13]));
    const char * outputfilename(argv[14]);

    /**************/
    /** Typedefs **/
    /**************/

    typedef otb::VectorImage<double,2>                  InputImageType;
    typedef InputImageType::OffsetType                  OffsetType;
    typedef InputImageType::RegionType                  RegionType;
    typedef InputImageType::IndexType                   IndexType;
    typedef InputImageType::SizeType                    SizeType;

    typedef otb::Image<float,2>                         DTMType;
    typedef vns::VectorLookUpTable<double,3>            LUTType;

    typedef otb::ImageFileReader<InputImageType>        ReaderType;
    typedef ReaderType::Pointer                         ReaderPointerType;

    typedef otb::ImageFileReader<DTMType>               DTMReaderType;
    typedef DTMReaderType::Pointer                      DTMReaderPointerType;

    typedef LUTType::PixelType                          LUTPixelType;
    typedef LUTType::ParameterValuesType                ParameterValuesType;
    typedef vns::LookUpTableFileReader<LUTType>         LUTReaderType;
    typedef LUTReaderType::Pointer                      LUTReaderPointerType;

    typedef vns::MultiTemporalAOTCostFunction<InputImageType, DTMType, LUTType>  AOTCostFunctionType;

    typedef otb::Image<double,2>                        OutputImageType;

    typedef vns::RunLevenbergMarquardtOptimization<InputImageType,DTMType,LUTType,OutputImageType, AOTCostFunctionType> RunLMOptimizationType;

    /** Neighborhood Iterators */
    typedef itk::ConstNeighborhoodIterator<InputImageType>  ImageNeighborhoodIteratorType;
    typedef itk::ConstNeighborhoodIterator<DTMType>         DTMNeighborhoodIteratorType;

    typedef otb::MultiplyByScalarImageFilter<InputImageType,InputImageType>  MultiplyFilterType;
    typedef otb::MultiplyByScalarImageFilter<DTMType,DTMType>  DTMMultiplyFilterType;

      /********************/
      /** LUT Creation 1 **/
      /********************/

      std::cout << "Creation of AOT LUT D \n";
      LUTType::Pointer lLUTD = LUTType::New();
      LUTType::ParameterValuesType values;


      /** Read and fill the LUT */
      std::fstream lFile;
      std::string lFilename = miniLUTFileNameD;

      lFile.open(lFilename.c_str(), std::ios::in | std::ios::binary);
      if (lFile.fail())
      {
        std::cout << "ComputeAOTImageFilter : Failed to open LUT file ! "<<miniLUTFileNameD <<std::endl;
      }

      unsigned int lNbOfBands = 4;
      lLUTD->SetNumberOfComponentsPerPixel(lNbOfBands);

      // Add Parameters to the LUT
      std::string str4("0 1000 2000 3000");
      std::string str5("0.0000 0.0625 0.1250 0.1875 0.2500 0.3125 0.3750 0.4375 0.5000 0.5625 0.6250 0.6875 0.7500 0.8125 0.8750 0.9375 1.0000 1.0625 1.1250 1.1875 1.2500 1.3125 1.3750 1.4375 1.5000");
      std::string str6("-0.2000 -0.1300 -0.0600 0.0100 0.0800 0.1500 0.2200 0.2900 0.3600 0.4300 0.5000 0.5700 0.6400 0.7100 0.7800");

      const vns::Utilities::ListOfDoubles Altitude_Indexes = vns::Utilities::SplitStringAsDouble(str4);
      const vns::Utilities::ListOfDoubles AOT_Indexes = vns::Utilities::SplitStringAsDouble(str5);
      const vns::Utilities::ListOfDoubles TOA_Reflectance_Indexes = vns::Utilities::SplitStringAsDouble(str6);

      ParameterValuesType newParam4;
      newParam4.ParameterName="Altitude_Indexes";
      newParam4.ParameterValues=Altitude_Indexes;
      lLUTD->AddParameterValues(newParam4);

      ParameterValuesType newParam5;
      newParam5.ParameterName="AOT_Indexes";
      newParam5.ParameterValues=AOT_Indexes;
      lLUTD->AddParameterValues(newParam5);

      ParameterValuesType newParam6;
      newParam6.ParameterName="TOA_Reflectance_Indexes";
      newParam6.ParameterValues=TOA_Reflectance_Indexes;
      lLUTD->AddParameterValues(newParam6);

      // Read and load values
      LUTType::ParametersValuesType getValues;
      getValues = lLUTD->GetParametersValues();

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
                    lVal=static_cast<double>(var);
                    lLUTD->SetValue(lutIndex, lNumBand, lVal);
                }
            }
        }
      }
      lFile.close();



    /********************/
    /** LUT Creation 2 **/
    /********************/

    std::cout << "Creation of AOT LUT Dm1\n";
    std::fstream lFile2;
    LUTType::Pointer lLUTDm1 = LUTType::New();

    /** Read and fill the LUT */
    lFilename = miniLUTFileNameDm1;

    lFile2.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile2.fail())
    {
        std::cout << "ComputeAOTImageFilter : Failed to open LUT file ! "
                << miniLUTFileNameDm1 << std::endl;
    }

    lLUTDm1->SetNumberOfComponentsPerPixel(lNbOfBands);

    ParameterValuesType newParam1;
    newParam1.ParameterName="Altitude_Indexes";
    newParam1.ParameterValues=Altitude_Indexes;
    lLUTDm1->AddParameterValues(newParam1);

    ParameterValuesType newParam2;
    newParam2.ParameterName="AOT_Indexes";
    newParam2.ParameterValues=AOT_Indexes;
    lLUTDm1->AddParameterValues(newParam2);

    ParameterValuesType newParam3;
    newParam3.ParameterName="TOA_Reflectance_Indexes";
    newParam3.ParameterValues=TOA_Reflectance_Indexes;
    lLUTDm1->AddParameterValues(newParam3);


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

                    lFile2.read((char*) (&var), 4);
                    lVal = static_cast<double>(var);
                    lLUTDm1->SetValue(lutIndex, lNumBand, lVal);
                }
            }
        }
    }
    lFile2.close();


    /***********************/
    /** AOTbands Creation **/
    /***********************/

    std::vector<unsigned int> lAOTbands;
    for ( unsigned int i=0;i<lNbOfUsedBands;i++)
    {
        lAOTbands.push_back(i);
    }


    /********************************/
    /** lExtractedOffsets Creation **/
    /********************************/

    std::vector<OffsetType> lExtractedOffsets;
    OffsetType lOffset;

    for (int l=-3;l<=3;l++)
    {
        lOffset[0]=l;
        for (int p=-3;p<=3;p++)
        {
            lOffset[1]=p;
            lExtractedOffsets.push_back(lOffset);
        }
    }


    double F_Tolerance = 1e-5; // Function value tolerance
    double G_Tolerance = 1e-5; // Gradient magnitude tolerance
    double X_Tolerance = 1e-5; // Search space tolerance
    double Epsilon_Function = 1e-5; //1e-9; // Step
    int Max_Iterations = 500; // Maximum number of iterations
    double AOTinitialD = 0.2;
    double AOTinitialDm1 = 35.;



    /********************/
    /** Pipeline       **/
    /********************/

    /** Instantiating the filter */
   RunLMOptimizationType::Pointer lRunLMOptimization = RunLMOptimizationType::New();


    ReaderPointerType reader1 = ReaderType::New();
    ReaderPointerType reader2 = ReaderType::New();
    ReaderPointerType reader3 = ReaderType::New();

    DTMReaderPointerType DTMreader = DTMReaderType::New();

    MultiplyFilterType::Pointer lMultiplyFilter1 = MultiplyFilterType::New();
    MultiplyFilterType::Pointer lMultiplyFilter2 = MultiplyFilterType::New();
    MultiplyFilterType::Pointer lMultiplyFilter3 = MultiplyFilterType::New();
    DTMMultiplyFilterType::Pointer lDTMMultiplyFilter = DTMMultiplyFilterType::New();

    ImageNeighborhoodIteratorType lIPTOACSubIt;
    ImageNeighborhoodIteratorType lL2inRTASubIt;
    ImageNeighborhoodIteratorType lL2inRTCSubIt;
    DTMNeighborhoodIteratorType lDTMSubIt;

    IndexType lIndex;
    lIndex[0]=170;
    lIndex[1]=170;

    SizeType lSize;
    lSize[0]=7;
    lSize[1]=7;

    RegionType lRegion;
    lRegion.SetSize(lSize);
    lRegion.SetIndex(lIndex);

    reader1->SetFileName(IPTOACSubFileName);
    reader2->SetFileName(L2inRTASubFileName);
    reader3->SetFileName(L2inRTCSubFileName);
    DTMreader->SetFileName(L2inDTMSubFileName);


    lMultiplyFilter1->SetInput(reader1->GetOutput());
    lMultiplyFilter1->SetCoef(0.001);
    lMultiplyFilter2->SetInput(reader2->GetOutput());
    lMultiplyFilter2->SetCoef(0.001);
    lMultiplyFilter3->SetInput(reader3->GetOutput());
    lMultiplyFilter3->SetCoef(0.001);

    std::cout <<" Multiplying images and Reading DTM..." << std::endl;

    lMultiplyFilter1->Update();
    lMultiplyFilter2->Update();
    lMultiplyFilter3->Update();
    DTMreader->Update();

    std::cout <<" Multiplying images and Reading DTM done..." << std::endl;

    lIPTOACSubIt = ImageNeighborhoodIteratorType(lSize,lMultiplyFilter1->GetOutput(),lRegion);
    lL2inRTASubIt = ImageNeighborhoodIteratorType(lSize, lMultiplyFilter2->GetOutput(),lRegion);
    lL2inRTCSubIt = ImageNeighborhoodIteratorType(lSize, lMultiplyFilter3->GetOutput(),lRegion);
    lDTMSubIt = DTMNeighborhoodIteratorType(lSize, DTMreader->GetOutput(),lRegion);

    lIPTOACSubIt.GoToBegin();
    lL2inRTASubIt.GoToBegin();
    lL2inRTCSubIt.GoToBegin();
    lDTMSubIt.GoToBegin();

//    lRunLMOptimization->GetCostFunction()->SetAOTbands(lAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsD(lAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsReflectanceCompositeDm1(lAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsLUTCompositeDm1(lAOTbands);


    lRunLMOptimization->GetCostFunction()->SetLUTD(lLUTD);
    lRunLMOptimization->GetCostFunction()->SetLUTDm1(lLUTDm1);
    lRunLMOptimization->GetCostFunction()->SetDarkestPixelAOT(lDarkestPixelAOT);
    lRunLMOptimization->GetCostFunction()->SetWeq1(lWeq1);
    lRunLMOptimization->GetCostFunction()->SetWeq2(lWeq2);
    lRunLMOptimization->GetCostFunction()->SetWdark(lWdark);
    lRunLMOptimization->GetCostFunction()->SetWbounds(lWbounds);
    lRunLMOptimization->GetCostFunction()->SetAOTmin(lAOTmin);

    lRunLMOptimization->GetCostFunction()->SetOffsetVect(lExtractedOffsets);
    lRunLMOptimization->GetCostFunction()->SetIPTOACSub(lIPTOACSubIt);
    lRunLMOptimization->GetCostFunction()->SetL2inRTA(lL2inRTASubIt);
    lRunLMOptimization->GetCostFunction()->SetL2inRTC(lL2inRTCSubIt);
    lRunLMOptimization->GetCostFunction()->SetDTMSub(lDTMSubIt);

    lRunLMOptimization->GetCostFunction()->FindAOTmax();


    double AOT_value(0.);
    AOT_value = lRunLMOptimization->RunLMOptimization(
                F_Tolerance, G_Tolerance,X_Tolerance, Epsilon_Function,Max_Iterations,
                AOTinitialD, AOTinitialDm1);

    std::ofstream f(outputfilename);
    f << "AOT result: "<<AOT_value<<std::endl;
    f.close();
    return EXIT_SUCCESS;

}
