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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 oct. 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsRunLevenbergMarquardtOptimization.h"
#include "vnsSpectroTemporalAOTCostFunction.h"
#include "vnsLookUpTableFileReader.h"
#include "vnsLoggers.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "vnsVectorLookUpTable.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"


int vnsRunLevenbergMarquardtOptimizationSTValid(int argc, char * argv [])
{
    std::cout<<"========="<< argc<<std::endl;
    if (argc != 38)
    {
        return EXIT_FAILURE;
    }

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);


    /********************/
    /** Get parameters **/
    /********************/

    unsigned int cpt(1);

    // 4 images
    const char * L2inRTASubFileName = argv[cpt++];
    const char * IPTOACSubFileName = argv[cpt++];
    const char * IPTOACSubMSFileName = argv[cpt++];
    const char * L2inRTCSubFileName = argv[cpt++];
    // 2 dtm image
    const char * DTMSubFileName = argv[cpt++];
    const char * DTMSubMSFileName = argv[cpt++];
    // 3 LUTs
    const char * miniLUTFileNameDm1 =  argv[cpt++];
    const char * miniLUTFileNameD =  argv[cpt++];
    const char * miniLUTMSFileNameD =  argv[cpt++];

    const char * lNbPixFileName = argv[cpt++];
    const char * lNbPixMSFileName = argv[cpt++];
    const char * lKpondFileName = argv[cpt++];
    const char * lKpondMSFileName = argv[cpt++];

    const char * lTauMaxFileName = argv[cpt++];
    const char * lTauDm1FileName = argv[cpt++];
    const char * lTauDFileName = argv[cpt++];


    double lAOTmin = static_cast<double> (atof(argv[cpt++]));
    unsigned int lNbOfUsedBands = static_cast<unsigned int> (atoi(argv[cpt++]));
    unsigned int lNbOfMSUsedBands = static_cast<unsigned int> (atoi(argv[cpt++]));
    unsigned int lBlueBand = static_cast<unsigned int> (atoi(argv[cpt++]));
    unsigned int lRedBand = static_cast<unsigned int> (atoi(argv[cpt++]));
    unsigned int lNDVIAOTBand1 = static_cast<unsigned int> (atoi(argv[cpt++]));
    unsigned int lNDVIAOTBand2 = static_cast<unsigned int> (atoi(argv[cpt++]));
    double lWdark = static_cast<double> (atof(argv[cpt++]));
    double lWbounds = static_cast<double> (atof(argv[cpt++]));
    double lSlope = static_cast<double> (atof(argv[cpt++]));
    double lYIntercept = static_cast<double> (atof(argv[cpt++]));
    double lSlopeNDVI = static_cast<double> (atof(argv[cpt++]));
    double lYInterceptNDVI = static_cast<double> (atof(argv[cpt++]));

    double lF_Tolerance = static_cast<double> (atof(argv[cpt++]));
    double lG_Tolerance  = static_cast<double> (atof(argv[cpt++]));
    double lX_Tolerance = static_cast<double> (atof(argv[cpt++]));
    double lEpsilon_Function = static_cast<double> (atof(argv[cpt++]));
    unsigned int lMax_Iterations  = static_cast<unsigned int> (atoi(argv[cpt++]));
    double lAOTinitialD = static_cast<double> (atof(argv[cpt++]));
    double lAOTinitialDm1 = static_cast<double> (atof(argv[cpt++]));

    const char * outputfilename(argv[cpt++]);

    unsigned int lSize2 = static_cast<unsigned int> (1);

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

    typedef vns::SpectroTemporalAOTCostFunction<InputImageType, DTMType, LUTType> AOTCostFunction;

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

    std::cout << "Creation of AOT LUT 1 "<<miniLUTFileNameD<<std::endl;
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

    lLUTD->SetNumberOfComponentsPerPixel(lNbOfUsedBands);

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
    for (unsigned int lNumBand = 0; lNumBand < lNbOfUsedBands; lNumBand++)
    {

        for (int i = 0; i < size[0]; i++)
        {
            std::cout<<"================ "<<i<<std::endl;
            lutIndex[0] = i;
            for (int j = 0; j < size[1]; j++)
            {
                std::cout<<"+++++++++++++++++++ "<<j<<std::endl;
                lutIndex[1] = j;
                for (int k = 0; k < size[2]; k++)
                {
                    lutIndex[2] = k;

                    lFile.read((char*) (&var), 4);
                    lVal = var;
                    std::cout<<var<<std::endl;
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

    std::cout << "Creation of AOT LUT Dm1 "<<miniLUTFileNameDm1<<std::endl;;;
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

    lLUTDm1->SetNumberOfComponentsPerPixel(lNbOfUsedBands);
    lLUTDm1->AddParameterValues(newParam4);
    lLUTDm1->AddParameterValues(newParam5);
    lLUTDm1->AddParameterValues(newParam6);

    // number of components per pixel
    for (unsigned int lNumBand = 0; lNumBand < lNbOfUsedBands; lNumBand++)
    {
        std::cout<<"******************* "<<lNumBand<<std::endl;
        for (int i = 0; i < size[0]; i++)
        {
            std::cout<<"================ "<<i<<std::endl;
            lutIndex[0] = i;
            for (int j = 0; j < size[1]; j++)
            {
                std::cout<<"+++++++++++++++++++ "<<j<<std::endl;
                lutIndex[1] = j;
                for (int k = 0; k < size[2]; k++)
                {
                    lutIndex[2] = k;

                    lFile2.read((char*) (&var), 4);
                    lVal = var;
                    std::cout<<var<<std::endl;
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

    /********************/
    /** LUT Creation MS **/
    /********************/

    std::cout << "Creation of MS AOT LUT 1\n";
    LUTType::Pointer lMSLUTD = LUTType::New();

    /** Read and fill the LUT */
    lFilename = miniLUTMSFileNameD;

    lFile.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile.fail())
    {
        std::cout << "ComputeAOTImageFilter : Failed to open LUT file ! "
                << miniLUTMSFileNameD << std::endl;
    }

    lMSLUTD->SetNumberOfComponentsPerPixel(lNbOfMSUsedBands);


    lMSLUTD->AddParameterValues(newParam4);
    lMSLUTD->AddParameterValues(newParam5);
    lMSLUTD->AddParameterValues(newParam6);

    // Parcours de la LUT et chargement
    // number of components per pixel
    for (unsigned int lNumBand = 0; lNumBand < lNbOfMSUsedBands; lNumBand++)
    {
        //std::cout<<"******************* "<<lNumBand<<std::endl;
        for (int i = 0; i < size[0]; i++)
        {

            //std::cout<<"================ "<<i<<std::endl;
            lutIndex[0] = i;
            for (int j = 0; j < size[1]; j++)
            {
                //std::cout<<"+++++++++++++++++++ "<<j<<std::endl;
                lutIndex[1] = j;
                for (int k = 0; k < size[2]; k++)
                {
                    lutIndex[2] = k;

                    lFile.read((char*) (&var), 4);
                   // std::cout<<var<<std::endl;
                    lVal = var;
                    lMSLUTD->SetValue(lutIndex, lNumBand, lVal);
                }
            }
        }
    }
    lFile.close();

    /***********************/
    /** AOTbands Creation **/
    /***********************/

    std::vector<unsigned int> lMTAOTbands;

    lMTAOTbands.push_back(lBlueBand);

    std::vector<unsigned int> lMSAOTbands;

    lMSAOTbands.push_back(lBlueBand);
    lMSAOTbands.push_back(lRedBand);

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

  //  lNbOfPix = 0;

    /**************/
    /** NbOfPixMS  **/
    /**************/

    std::fstream lFile4;
    lFilename = lNbPixMSFileName;

    lFile4.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile4.fail())
    {
      std::cout << "Run LM : Failed to open :  "<<lNbPixMSFileName <<std::endl;
    }

    lFile4.read((char*) (&octet), 1);
    unsigned int lNbOfPixMS = static_cast<unsigned int>(octet);
    std::cout << "lNbOfPixMS : " << lNbOfPixMS << std::endl;
    lFile4.close();

   // lNbOfPixMS=0;

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

    double lWMS(0.);
    lFilename = lKpondMSFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<lKpondMSFileName <<std::endl;
    }

    // WeqMS
    lFile3.read((char*) (&var), 4);
    lWMS = static_cast<double>(var);
    std::cout << "lWMS : " << lWMS << std::endl;
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
    lSize[0] = lNbOfPix;
    lSize[1] = lSize2;

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

    lFilename = IPTOACSubMSFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<IPTOACSubMSFileName <<std::endl;
    }
    else
    {
        std::cout << "image3 : " << IPTOACSubMSFileName << std::endl;
    }

    SizeType lSizeMS;
    lSizeMS[0] = lNbOfPixMS;
    lSizeMS[1] = lSize2;

    lIndex.Fill(0);
    RegionType lRegionMS;
    lRegionMS.SetIndex(lIndex);
    lRegionMS.SetSize(lSizeMS);

    InputImageType::Pointer lIPTOACSubMSPtr = InputImageType::New();
    lIPTOACSubMSPtr->SetRegions(lRegionMS);
    lIPTOACSubMSPtr->SetNumberOfComponentsPerPixel(lNbOfMSUsedBands);
    lIPTOACSubMSPtr->Allocate();

    std::vector<double> lImage;

    for (unsigned int lNumBand = 0; lNumBand < lNbOfMSUsedBands; lNumBand++)
    {
        for (unsigned int i = 0; i < lSizeMS[0]; i++)
        {
            for (unsigned int j = 0; j < lSizeMS[1]; j++)
            {
                lFile3.read((char*) (&var), 4);
                lImage.push_back(var);
                //std::cout << "xxxxxxxxxxxxxxxx IPTOAC MS: " << lNumBand << " "<< i << " " << j << " " << var << std::endl;
            }
        }
    }
    lFile3.close();

    const unsigned int lnbCol = lSizeMS[0];
    const unsigned int lnbLine = lSizeMS[1];
    const unsigned int lnbPix = lnbCol*lnbLine;

    for (unsigned int i = 0; i < lnbLine; i++)
    {
        lIndex[1] = i;
        for (unsigned int j = 0; j < lnbCol; j++)
        {
            unsigned int lRefIndex = i*lnbCol + j;
            lIndex[0] = j;
            lPix[0] = lImage[lRefIndex];
            lPix[1] = lImage[lnbPix + lRefIndex];
            //std::cout << "xxxxxxxxxxxxxxxx lPix : " << lIndex <<" "<< lPix[0] << " "<< lPix[1] << std::endl;
            lIPTOACSubMSPtr->SetPixel(lIndex, lPix); // 0 for band 0
        }
    }


    /** 4 **/
    lFilename = L2inRTCSubFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<L2inRTCSubFileName <<std::endl;
    }
    else
    {
        std::cout << "image4 : " << L2inRTCSubFileName << std::endl;
    }

    lIndex.Fill(0);
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

    /** 5 **/
    lFilename = DTMSubFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  "<<DTMSubFileName <<std::endl;
    }
    else
    {
        std::cout << "image5 : " << DTMSubFileName << std::endl;
    }

    lIndex.Fill(0);
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

    /** 6 **/
     lFilename = DTMSubMSFileName;
     lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
     if (lFile3.fail())
     {
         std::cout << "Run LM : Failed to open :  "<<DTMSubMSFileName <<std::endl;
     }
     else
     {
         std::cout << "image6 : " << DTMSubMSFileName << std::endl;
     }

     lIndex.Fill(0);
     lRegionMS.SetIndex(lIndex);
     lRegionMS.SetSize(lSizeMS);

     DTMType::Pointer lDTMSubMSPtr = DTMType::New();
     lDTMSubMSPtr->SetRegions(lRegionMS);
     lDTMSubMSPtr->SetNumberOfComponentsPerPixel(1);
     lDTMSubMSPtr->Allocate();


     for (unsigned int i = 0; i < lSizeMS[0]; i++)
     {
         lIndex[0] = i;
         for (unsigned int j = 0; j < lSizeMS[1]; j++)
         {
             lIndex[1] = j;

             lFile3.read((char*) (&var), 4);
             lDTMSubMSPtr->SetPixel(lIndex,var); // 0 for band 0
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

    std::vector<OffsetType> lExtractedOffsetsMS;


    for (unsigned int l = 0; l < lSizeMS[0]; l++)
    {
        lOffset[0] = l;
        for (unsigned int p = 0; p < lSizeMS[1]; p++)
        {
            lOffset[1] = p;
            lExtractedOffsetsMS.push_back(lOffset);
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
    ImageNeighborhoodIteratorType lIPTOACSubMSIt = ImageNeighborhoodIteratorType(lSizeMS,lIPTOACSubMSPtr,lRegionMS);
    ImageNeighborhoodIteratorType lL2inRTAIt = ImageNeighborhoodIteratorType(lSize, lL2inRTAPtr,lRegion);
    ImageNeighborhoodIteratorType lL2inRTCIt = ImageNeighborhoodIteratorType(lSize, lL2inRTCPtr,lRegion);
    DTMNeighborhoodIteratorType lDTMSubIt = DTMNeighborhoodIteratorType(lSize, lDTMSubPtr,lRegion);
    DTMNeighborhoodIteratorType lDTMSubMSIt = DTMNeighborhoodIteratorType(lSizeMS, lDTMSubMSPtr,lRegionMS);

    lIPTOACSubIt.GoToBegin();
    lIPTOACSubMSIt.GoToBegin();
    lL2inRTAIt.GoToBegin();
    lL2inRTCIt.GoToBegin();
    lDTMSubIt.GoToBegin();
    lDTMSubMSIt.GoToBegin();

    // Important : SetMTAOTbands must be done BEFORE SetMeasureSize
//    lRunLMOptimization->GetCostFunction()->SetAOTbands(lMTAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsD(lMTAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsReflectanceCompositeDm1(lMTAOTbands);
    lRunLMOptimization->GetCostFunction()->SetAOTbandsLUTCompositeDm1(lMTAOTbands);

    lRunLMOptimization->GetCostFunction()->SetMSAOTbands(lMSAOTbands);
    lRunLMOptimization->GetCostFunction()->SetNDVIAOTBand1(lNDVIAOTBand1);
    lRunLMOptimization->GetCostFunction()->SetNDVIAOTBand2(lNDVIAOTBand2);
    lRunLMOptimization->GetCostFunction()->SetSpaceDimension(2);
    lRunLMOptimization->GetCostFunction()->SetSTMeasureSize(lNbOfPix, lNbOfPixMS);

    lRunLMOptimization->GetCostFunction()->SetLUTD(lLUTD);
    lRunLMOptimization->GetCostFunction()->SetLUTDm1(lLUTDm1);
    lRunLMOptimization->GetCostFunction()->SetMSLUTD(lMSLUTD);
    lRunLMOptimization->GetCostFunction()->SetDarkestPixelAOT(lDarkestPixelAOT);
    lRunLMOptimization->GetCostFunction()->SetWeq1(lKpond.at(1));
    lRunLMOptimization->GetCostFunction()->SetWeq2(lKpond.at(0));
    lRunLMOptimization->GetCostFunction()->SetWeqMS(lWMS);
    lRunLMOptimization->GetCostFunction()->SetWdark(lWdark);
    lRunLMOptimization->GetCostFunction()->SetWbounds(lWbounds);
    lRunLMOptimization->GetCostFunction()->SetAOTmin(lAOTmin);

    lRunLMOptimization->GetCostFunction()->SetSlopeMS(lSlope);
    lRunLMOptimization->GetCostFunction()->SetYInterceptMS(lYIntercept);
    lRunLMOptimization->GetCostFunction()->SetSlopeNDVI(lSlopeNDVI);
    lRunLMOptimization->GetCostFunction()->SetYInterceptNDVI(lYInterceptNDVI);

    std::cout << "lWdark:"<<lWdark<<" lWbounds:" <<lWbounds << " lAOTmin:"<<lAOTmin <<std::endl;
    lRunLMOptimization->GetCostFunction()->SetOffsetVect(lExtractedOffsets);
    lRunLMOptimization->GetCostFunction()->SetMSOffsetVect(lExtractedOffsetsMS);
    lRunLMOptimization->GetCostFunction()->SetIPTOACSub(lIPTOACSubIt);
    lRunLMOptimization->GetCostFunction()->SetMSIPTOACSub(lIPTOACSubMSIt);
    lRunLMOptimization->GetCostFunction()->SetL2inRTA(lL2inRTAIt);
    lRunLMOptimization->GetCostFunction()->SetL2inRTC(lL2inRTCIt);
    lRunLMOptimization->GetCostFunction()->SetDTMSub(lDTMSubIt);
    lRunLMOptimization->GetCostFunction()->SetMSDTMSub(lDTMSubMSIt);


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
