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
#include "otbImageFileWriter.h"
#include "itkImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "vnsUtilities.h"

int vnsRunLevenbergMarquardtOptimizationValid(int argc, char * argv [])
{

    if (argc != 26)
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
    const char * L2inRTASubFileName = argv[1];
    const char * IPTOACSubFileName = argv[2];
    const char * L2inRTCSubFileName = argv[3];
    // 1 dtm image
    const char * L2inDTMSubFileName = argv[4];
    // 1 LUT
    const char * miniLUTFileNameDm1 =  argv[5];
    const char * miniLUTFileNameD =  argv[6];

    const char * lNbPixFileName = argv[7];
    const char * lKpondFileName = argv[8];

    const char * lTauMaxFileName = argv[9];
    const char * lTauDm1FileName = argv[10];
    const char * lTauDFileName = argv[11];


    double lAOTmin = static_cast<double> (atof(argv[12]));
    unsigned int lNbOfUsedBands = static_cast<unsigned int> (atoi(argv[13]));
    double lWdark = static_cast<double> (atof(argv[14]));
    double lWbounds = static_cast<double> (atof(argv[15]));
   // unsigned int lSize1 = static_cast<unsigned int> (atoi(argv[16]));
    unsigned int lSize2 = static_cast<unsigned int> (atoi(argv[17]));

    double lF_Tolerance = static_cast<double> (atof(argv[18]));
    double lG_Tolerance  = static_cast<double> (atof(argv[19]));
    double lX_Tolerance = static_cast<double> (atof(argv[20]));
    double lEpsilon_Function = static_cast<double> (atof(argv[21]));
    unsigned int lMax_Iterations  = static_cast<unsigned int> (atoi(argv[22]));
    double lAOTinitialD = static_cast<double> (atof(argv[23]));
    double lAOTinitialDm1 = static_cast<double> (atof(argv[24]));

    const char * outputfilename(argv[25]);

    /**************/
    /** Typedefs **/
    /**************/

    typedef otb::VectorImage<double,2>                  InputImageType;
    typedef InputImageType::OffsetType                  OffsetType;
    typedef InputImageType::RegionType                  RegionType;
    typedef InputImageType::IndexType                   IndexType;
    typedef InputImageType::SizeType                    SizeType;
    typedef InputImageType::PixelType                   PixelType;

    typedef otb::ImageFileWriter<InputImageType>        WriterType;
    typedef WriterType::Pointer                         WriterPointerType;

    typedef otb::Image<float,2>                         DTMType;
    typedef vns::VectorLookUpTable<double,3>            LUTType;

    typedef otb::ImageFileReader<InputImageType>        ReaderType;
    typedef ReaderType::Pointer                         ReaderPointerType;

    typedef otb::ImageFileReader<DTMType>               DTMReaderType;
    typedef DTMReaderType::Pointer                      DTMReaderPointerType;

    typedef otb::ImageFileWriter<DTMType>               DTMWriterType;
    typedef DTMWriterType::Pointer                      DTMWriterPointerType;

    typedef LUTType::PixelType                          LUTPixelType;
    typedef LUTType::ParameterValuesType                ParameterValuesType;
    typedef vns::LookUpTableFileReader<LUTType>         LUTReaderType;
    typedef LUTReaderType::Pointer                      LUTReaderPointerType;

    typedef itk::ImageFileWriter<LUTType>         LUTWriterType;
    typedef LUTWriterType::Pointer                      LUTWriterPointerType;

    typedef vns::MultiTemporalAOTCostFunction<InputImageType, DTMType, LUTType> AOTCostFunction;

    typedef otb::Image<double,2>                        OutputImageType;

    typedef vns::RunLevenbergMarquardtOptimization<InputImageType,DTMType,LUTType,OutputImageType, AOTCostFunction> RunLMOptimizationType;

    /** Neighborhood Iterators */
    typedef itk::ConstNeighborhoodIterator<InputImageType>  ImageNeighborhoodIteratorType;
    typedef itk::ConstNeighborhoodIterator<DTMType>         DTMNeighborhoodIteratorType;

    typedef otb::MultiplyByScalarImageFilter<InputImageType,InputImageType>  MultiplyFilterType;
    typedef otb::MultiplyByScalarImageFilter<DTMType,DTMType>  DTMMultiplyFilterType;

    /********************/
    /** LUT Creation 1 **/
    /********************/

    std::cout << "Creation of AOT LUT 1\n";
    LUTType::Pointer lLUTD = LUTType::New();
    LUTType::ParameterValuesType values;

    /** Read and fill the LUT */
    std::fstream lFile;
    std::string lFilename = miniLUTFileNameD;

    lFile.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile.fail())
    {
        std::cout << "ComputeAOTImageFilter : Failed to open LUT file ! "
                << miniLUTFileNameD << std::endl;
    }

    unsigned int lNbOfBands = 1;
    lLUTD->SetNumberOfComponentsPerPixel(lNbOfBands);

    // Add Parameters to the LUT
    std::string str4("0 1000 2000 3000");
    std::string
            str5(
                    "0.0000 0.0625 0.1250 0.1875 0.2500 0.3125 0.3750 0.4375 0.5000 0.5625 0.6250 0.6875 0.7500 0.8125 0.8750 0.9375 1.0000 1.0625 1.1250 1.1875 1.2500 1.3125 1.3750 1.4375 1.5000");
    std::string
            str6(
                    "-0.2000 -0.1300 -0.0600 0.0100 0.0800 0.1500 0.2200 0.2900 0.3600 0.4300 0.5000 0.5700 0.6400 0.7100 0.7800");

    const vns::Utilities::ListOfDoubles Altitude_Indexes = vns::Utilities::SplitStringAsDouble(str4);
    const vns::Utilities::ListOfDoubles AOT_Indexes = vns::Utilities::SplitStringAsDouble(str5);
    const vns::Utilities::ListOfDoubles TOA_Reflectance_Indexes = vns::Utilities::SplitStringAsDouble(str6);

    ParameterValuesType newParam4;
    newParam4.ParameterName = "Altitude_Indexes";
    newParam4.ParameterValues = Altitude_Indexes;
    lLUTD->AddParameterValues(newParam4);

    ParameterValuesType newParam5;
    newParam5.ParameterName = "AOT_Indexes";
    newParam5.ParameterValues = AOT_Indexes;
    lLUTD->AddParameterValues(newParam5);

    ParameterValuesType newParam6;
    newParam6.ParameterName = "TOA_Reflectance_Indexes";
    newParam6.ParameterValues = TOA_Reflectance_Indexes;
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
                    lVal = var;
                    lLUTD->SetValue(lutIndex, lNumBand, lVal);
                }
            }
        }
    }
    lFile.close();
/*
    LUTWriterPointerType lLUTWriter = LUTWriterType::New();
    lLUTWriter->SetFileName("LUT1.tif");
    lLUTWriter->SetInput(lLUT);
    lLUTWriter->Update();*/

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
    lLUTDm1->AddParameterValues(newParam4);
    lLUTDm1->AddParameterValues(newParam5);
    lLUTDm1->AddParameterValues(newParam6);

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
                    lVal = var;
                    lLUTDm1->SetValue(lutIndex, lNumBand, lVal);
                }
            }
        }
    }
    lFile2.close();

    /*LUTWriterPointerType lLUTWriter2 = LUTWriterType::New();
    lLUTWriter2->SetFileName("LUT2.tif");
    lLUTWriter2->SetInput(lLUT2);
    lLUTWriter2->Update();*/

    /***********************/
    /** AOTbands Creation **/
    /***********************/

    std::vector<unsigned int> lAOTbands;
    for ( unsigned int i=0;i<lNbOfUsedBands;i++)
    {
        lAOTbands.push_back(i);
    }
   // std::cout << "AOTbands : " << lAOTbands << std::endl;

    /**************/
    /** NbOfPix  **/
    /**************/

    std::fstream lFile3;
    lFilename = lNbPixFileName;
    unsigned char octet(0);

    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
      std::cout << "Run LM : Failed to open :  "<<lNbPixFileName <<std::endl;
    }

    lFile3.read((char*) (&octet), 1);
    unsigned int lNbOfPix = static_cast<unsigned int>(octet);
    std::cout << "lNbOfPix : " << lNbOfPix << std::endl;
    lFile3.close();

    /************************/
    /** Kpond : weq2 weq1  **/
    /************************/

    std::vector<double> lKpond;
    lFilename = lKpondFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<lKpondFileName <<std::endl;
    }

    // weq2
    lFile3.read((char*) (&var), 4);
    lKpond.push_back(static_cast<double>(var));
    std::cout << "lKpond : " << lKpond.at(0) << std::endl;
    // weq1
    lFile3.read((char*) (&var), 4);
    lKpond.push_back(static_cast<double>(var));
    std::cout << "lKpond : " << lKpond.at(1) << std::endl;
    lFile3.close();

    /**********************/
    /** lectures images  **/
    /**********************/

    /** 1 **/
    lFilename = IPTOACSubFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<IPTOACSubFileName <<std::endl;
    }
    else
    {
        std::cout << "image1 : " << IPTOACSubFileName << std::endl;
    }

    PixelType lPix;
    lPix.SetSize(1);

    IndexType lIndex;
    lIndex.Fill(0);

    SizeType lSize;
    lSize[0]=lNbOfPix;
    lSize[1]=lSize2;

    RegionType lRegion;
    lRegion.SetIndex(lIndex);
    lRegion.SetSize(lSize);

    InputImageType::Pointer lIPTOACSubPtr = InputImageType::New();
    lIPTOACSubPtr->SetRegions(lRegion);
    lIPTOACSubPtr->SetNumberOfComponentsPerPixel(1);
    lIPTOACSubPtr->Allocate();

    for (unsigned int i = 0; i < lSize[0]; i++)
    {
        lIndex[0] = i;
        for (unsigned int j = 0; j < lSize[1]; j++)
        {
            lIndex[1] = j;

            lFile3.read((char*) (&var), 4);
            lPix[0] = var;
            lIPTOACSubPtr->SetPixel(lIndex,lPix); // 0 for band 0
            //std::cout<<"IPTOAC :"<< var<<std::endl;
        }
    }
    lFile3.close();

    /** 2 **/
    lFilename = L2inRTASubFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<L2inRTASubFileName <<std::endl;
    }
    else
    {
        std::cout << "image2 : " << L2inRTASubFileName << std::endl;
    }

    lIndex.Fill(0);
    lSize[0]=lNbOfPix;
    lSize[1]=lSize2;
    lRegion.SetIndex(lIndex);
    lRegion.SetSize(lSize);

    InputImageType::Pointer lL2inRTAPtr = InputImageType::New();
    lL2inRTAPtr->SetRegions(lRegion);
    lL2inRTAPtr->SetNumberOfComponentsPerPixel(1);
    lL2inRTAPtr->Allocate();


    for (unsigned int i = 0; i < lSize[0]; i++)
    {
        lIndex[0] = i;
        for (unsigned int j = 0; j < lSize[1]; j++)
        {
            lIndex[1] = j;

            lFile3.read((char*) (&var), 4);
            lPix[0] = var;
            lL2inRTAPtr->SetPixel(lIndex,lPix); // 0 for band 0
            //std::cout<<"L2inRTA :"<< var<<std::endl;
        }
    }
    lFile3.close();

    /** 3 **/
    lFilename = L2inRTCSubFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<L2inRTCSubFileName <<std::endl;
    }
    else
    {
        std::cout << "image3 : " << L2inRTCSubFileName << std::endl;
    }

    lIndex.Fill(0);
    lSize[0]=lNbOfPix;
    lSize[1]=lSize2;
    lRegion.SetIndex(lIndex);
    lRegion.SetSize(lSize);

    InputImageType::Pointer lL2inRTCPtr = InputImageType::New();
    lL2inRTCPtr->SetRegions(lRegion);
    lL2inRTCPtr->SetNumberOfComponentsPerPixel(1);
    lL2inRTCPtr->Allocate();


    for (unsigned int i = 0; i < lSize[0]; i++)
    {
        lIndex[0] = i;
        for (unsigned int j = 0; j < lSize[1]; j++)
        {
            lIndex[1] = j;

            lFile3.read((char*) (&var), 4);
            lPix[0] = var;
            lL2inRTCPtr->SetPixel(lIndex,lPix); // 0 for band 0
            //std::cout<<"L2inRTC :"<< var<<std::endl;
        }
    }
    lFile3.close();

    /** 4 **/
    lFilename = L2inDTMSubFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<L2inDTMSubFileName <<std::endl;
    }
    else
    {
        std::cout << "image4 : " << L2inDTMSubFileName << std::endl;
    }

    lIndex.Fill(0);
    lSize[0]=lNbOfPix;
    lSize[1]=lSize2;
    lRegion.SetIndex(lIndex);
    lRegion.SetSize(lSize);

    DTMType::Pointer lDTMSubPtr = DTMType::New();
    lDTMSubPtr->SetRegions(lRegion);
    lDTMSubPtr->SetNumberOfComponentsPerPixel(1);
    lDTMSubPtr->Allocate();


    for (unsigned int i = 0; i < lSize[0]; i++)
    {
        lIndex[0] = i;
        for (unsigned int j = 0; j < lSize[1]; j++)
        {
            lIndex[1] = j;

            lFile3.read((char*) (&var), 4);
            lDTMSubPtr->SetPixel(lIndex,var); // 0 for band 0
            //std::cout<<"alt :"<< var<<std::endl;
        }
    }
    lFile3.close();

/*
    DTMWriterPointerType DTMwriter = DTMWriterType::New();
    DTMwriter->SetFileName("DTM.tif");
    DTMwriter->SetInput(lDTMSubPtr);
    DTMwriter->Update();
*/



    /********************************/
    /** lExtractedOffsets Creation **/
    /********************************/

    std::vector<OffsetType> lExtractedOffsets;
    OffsetType lOffset;

    for (unsigned int l=0;l<lSize[0];l++)
    {
        lOffset[0]=l;
        for (unsigned int p=0;p<lSize[1];p++)
        {
            lOffset[1]=p;
            lExtractedOffsets.push_back(lOffset);
        }
    }


    /************************/
    /** AOT Dark !!        **/
    /************************/
    lFilename = lTauMaxFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
      std::cout << "Run LM : Failed to open :  "<<lTauMaxFileName <<std::endl;
    }

    lFile3.read((char*) (&var), 4);
    double lDarkestPixelAOT = static_cast<double>(var);
    std::cout << "DarkestPixelAOT : " << lDarkestPixelAOT << std::endl;
    lFile3.close();



    /********************/
    /**    Pipeline    **/
    /********************/

    /** Instantiating the filter */
    RunLMOptimizationType::Pointer lRunLMOptimization = RunLMOptimizationType::New();

    ImageNeighborhoodIteratorType lIPTOACSubIt = ImageNeighborhoodIteratorType(lSize,lIPTOACSubPtr,lRegion);
    ImageNeighborhoodIteratorType lL2inRTAIt = ImageNeighborhoodIteratorType(lSize, lL2inRTAPtr,lRegion);
    ImageNeighborhoodIteratorType lL2inRTCIt = ImageNeighborhoodIteratorType(lSize, lL2inRTCPtr,lRegion);
    DTMNeighborhoodIteratorType lDTMSubIt = DTMNeighborhoodIteratorType(lSize, lDTMSubPtr,lRegion);

    lIPTOACSubIt.GoToBegin();
    lL2inRTAIt.GoToBegin();
    lL2inRTCIt.GoToBegin();
    lDTMSubIt.GoToBegin();

    // Important : SetAOTbands must be done BEFORE SetMeasureSize
//    lRunLMOptimization->GetCostFunction()->SetAOTbands(lAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsD(lAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsReflectanceCompositeDm1(lAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsLUTCompositeDm1(lAOTbands);

    lRunLMOptimization->GetCostFunction()->SetSpaceDimension(2);
    lRunLMOptimization->GetCostFunction()->SetMeasureSize(lNbOfPix);

    lRunLMOptimization->GetCostFunction()->SetLUTD(lLUTD);
    lRunLMOptimization->GetCostFunction()->SetLUTDm1(lLUTDm1);
    lRunLMOptimization->GetCostFunction()->SetDarkestPixelAOT(lDarkestPixelAOT);
    lRunLMOptimization->GetCostFunction()->SetWeq1(lKpond.at(1));
    lRunLMOptimization->GetCostFunction()->SetWeq2(lKpond.at(0));
    lRunLMOptimization->GetCostFunction()->SetWdark(lWdark);
    lRunLMOptimization->GetCostFunction()->SetWbounds(lWbounds);
    lRunLMOptimization->GetCostFunction()->SetAOTmin(lAOTmin);

    std::cout << "lWdark:"<<lWdark<<" lWbounds:" <<lWbounds << " lAOTmin:"<<lAOTmin <<std::endl;
    lRunLMOptimization->GetCostFunction()->SetOffsetVect(lExtractedOffsets);
    lRunLMOptimization->GetCostFunction()->SetIPTOACSub(lIPTOACSubIt);
    lRunLMOptimization->GetCostFunction()->SetL2inRTA(lL2inRTAIt);
    lRunLMOptimization->GetCostFunction()->SetL2inRTC(lL2inRTCIt);
    lRunLMOptimization->GetCostFunction()->SetDTMSub(lDTMSubIt);


    lRunLMOptimization->GetCostFunction()->FindAOTmax();
    double lAOTmax = lRunLMOptimization->GetCostFunction()->GetAOTmax();
    std::cout << "lAOTmax found : " << lAOTmax << std::endl;

    double lAOTFinal = lRunLMOptimization->RunLMOptimization(
                lF_Tolerance, lG_Tolerance,lX_Tolerance, lEpsilon_Function,lMax_Iterations,
                lAOTinitialD, lAOTinitialDm1);



    std::cout<<lRunLMOptimization->GetCostFunction()<<std::endl;
    std::cout<<"lAOTFinal = "<<lAOTFinal<<std::endl;



    /************************/
    /** tau1    from CNES  **/
    /************************/
    lFilename = lTauDFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
      std::cout << "Run LM : Failed to open :  "<<lTauDFileName <<std::endl;
    }

    lFile3.read((char*) (&var), 4);
    double lTauD = static_cast<double>(var);
    lFile3.close();

    std::cout<<"CNES Solution D = "<<lTauD<<std::endl;

    /************************/
    /** tau2    from CNES  **/
    /************************/
    lFilename = lTauDm1FileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
      std::cout << "Run LM : Failed to open :  "<<lTauDm1FileName <<std::endl;
    }

    lFile3.read((char*) (&var), 4);
    double lTauDm1 = static_cast<double>(var);
    lFile3.close();

    std::cout<<"CNES Solution Dm1 = "<<lTauDm1<<std::endl;

    std::ofstream f(outputfilename);
    f << "lAOTmax found : " << lAOTmax << std::endl;
    f << "lAOTFinal compute : "<<lAOTFinal<< std::endl;
    f << "CNES Solution   = (" <<lTauD << ",  " << lTauDm1 << ")" <<std::endl;
    f << "                = ( AOTdate D,    AOT date Dm1) "<<std::endl;
    f.close();

    return EXIT_SUCCESS;

}
