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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 14 oct. 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsRunLevenbergMarquardtOptimization.h"
#include "vnsMultiSpectralAOTCostFunction.h"
#include "vnsLookUpTableFileReader.h"
#include "vnsLoggers.h"
#include "vnsUtilities.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "vnsVectorLookUpTable.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"

int
vnsRunLevenbergMarquardtOptimizationMSValid(int argc, char * argv[])
{

    if (argc != 24)
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

    // 1 images
    const char * IPTOACSubFileName = argv[cpt++];
    // 1 dtm image
    const char * L2DTMSubFileName = argv[cpt++];
    // 1 LUT
    const char * miniLUTFileNameD = argv[cpt++];

    const char * lNbPixFileName = argv[cpt++];

    const char * lTauMaxFileName = argv[cpt++];
    const char * lTauDFileName = argv[cpt++];

    double lAOTmin = static_cast<double>(atof(argv[cpt++]));
    unsigned int lBlueBand = static_cast<unsigned int>(atoi(argv[cpt++]));
    unsigned int lRedBand = static_cast<unsigned int>(atoi(argv[cpt++]));
    double lWMS = static_cast<double>(atof(argv[cpt++]));
    double lWdark = static_cast<double>(atof(argv[cpt++]));
    double lWbounds = static_cast<double>(atof(argv[cpt++]));
    double lSlope = static_cast<double>(atof(argv[cpt++]));
    double lYIntercept = static_cast<double>(atof(argv[cpt++]));
    double lSlopeNDVI = static_cast<double>(atof(argv[cpt++]));
    double lYInterceptNDVI = static_cast<double>(atof(argv[cpt++]));

    double lF_Tolerance = static_cast<double>(atof(argv[cpt++]));
    double lG_Tolerance = static_cast<double>(atof(argv[cpt++]));
    double lX_Tolerance = static_cast<double>(atof(argv[cpt++]));
    double lEpsilon_Function = static_cast<double>(atof(argv[cpt++]));
    unsigned int lMax_Iterations = static_cast<unsigned int>(atoi(argv[cpt++]));
    double lAOTinitialD = static_cast<double>(atof(argv[cpt++]));

    const char * outputfilename(argv[cpt++]);

    unsigned int lSize2 = static_cast<unsigned int>(1);

    /**************/
    /** Typedefs **/
    /**************/

    typedef otb::VectorImage<double, 2> InputImageType;
    typedef InputImageType::OffsetType OffsetType;
    typedef InputImageType::RegionType RegionType;
    typedef InputImageType::IndexType IndexType;
    typedef InputImageType::SizeType SizeType;
    typedef InputImageType::PixelType PixelType;

    typedef otb::ImageFileWriter<InputImageType> WriterType;
    typedef WriterType::Pointer WriterPointerType;

    typedef otb::Image<float, 2> DTMType;
    typedef vns::VectorLookUpTable<double, 3> LUTType;

    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef ReaderType::Pointer ReaderPointerType;

    typedef otb::ImageFileReader<DTMType> DTMReaderType;
    typedef DTMReaderType::Pointer DTMReaderPointerType;

    typedef otb::ImageFileWriter<DTMType> DTMWriterType;
    typedef DTMWriterType::Pointer DTMWriterPointerType;

    typedef LUTType::PixelType LUTPixelType;
    typedef LUTType::ParameterValuesType ParameterValuesType;
    typedef vns::LookUpTableFileReader<LUTType> LUTReaderType;
    typedef LUTReaderType::Pointer LUTReaderPointerType;

    typedef itk::ImageFileWriter<LUTType> LUTWriterType;
    typedef LUTWriterType::Pointer LUTWriterPointerType;

    typedef vns::MultiSpectralAOTCostFunction<InputImageType, DTMType, LUTType> AOTCostFunction;

    typedef otb::Image<double, 2> OutputImageType;

    typedef vns::RunLevenbergMarquardtOptimization<InputImageType, DTMType, LUTType, OutputImageType, AOTCostFunction> RunLMOptimizationType;

    /** Neighborhood Iterators */
    typedef itk::ConstNeighborhoodIterator<InputImageType> ImageNeighborhoodIteratorType;
    typedef itk::ConstNeighborhoodIterator<DTMType> DTMNeighborhoodIteratorType;

    typedef otb::MultiplyByScalarImageFilter<InputImageType, InputImageType> MultiplyFilterType;
    typedef otb::MultiplyByScalarImageFilter<DTMType, DTMType> DTMMultiplyFilterType;

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
        std::cout << "ComputeAOTImageFilter : Failed to open LUT file ! " << miniLUTFileNameD << std::endl;
    }

    unsigned int lNbOfBands = 2;
    lLUTD->SetNumberOfComponentsPerPixel(lNbOfBands);

    // Add Parameters to the LUT
    std::string str4("0 1000 2000 3000");
    std::string str5(
            "0.0000 0.0625 0.1250 0.1875 0.2500 0.3125 0.3750 0.4375 0.5000 0.5625 0.6250 0.6875 0.7500 0.8125 0.8750 0.9375 1.0000 1.0625 1.1250 1.1875 1.2500 1.3125 1.3750 1.4375 1.5000");
    std::string str6("-0.2000 -0.1300 -0.0600 0.0100 0.0800 0.1500 0.2200 0.2900 0.3600 0.4300 0.5000 0.5700 0.6400 0.7100 0.7800");

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

    std::cout << "size LUT =" << size[0] << " " << size[1] << " " << size[2] << std::endl;

    // Parcours de la LUT et chargement
    LUTType::IndexType lutIndex;
    double lVal;
    float var(0.);

    // number of components per pixel
    for (unsigned int lNumBand = 0; lNumBand < lNbOfBands; lNumBand++)
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
                    //std::cout<<var<<std::endl;
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

    /**************/
    /** NbOfPix  **/
    /**************/

    std::fstream lFile3;
    lFilename = lNbPixFileName;
    unsigned char octet(0);

    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  " << lNbPixFileName << std::endl;
    }

    lFile3.read((char*) (&octet), 1);
    unsigned int lNbOfPix = static_cast<unsigned int>(octet);
    std::cout << "lNbOfPix : " << lNbOfPix << std::endl;
    lFile3.close();

    /**********************/
    /** lectures images  **/
    /**********************/

    /** 1 **/
    lFilename = IPTOACSubFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  " << IPTOACSubFileName << std::endl;
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
    lIPTOACSubPtr->SetNumberOfComponentsPerPixel(lNbOfBands);
    lIPTOACSubPtr->Allocate();

    std::vector<double> lImage;

    for (unsigned int lNumBand = 0; lNumBand < lNbOfBands; lNumBand++)
    {
        for (unsigned int i = 0; i < lSize[0]; i++)
        {
            for (unsigned int j = 0; j < lSize[1]; j++)
            {
                lFile3.read((char*) (&var), 4);
                lImage.push_back(var);
                std::cout << "xxxxxxxxxxxxxxxx IPTOAC : " << lNumBand << " " << i << " " << j << " " << var << std::endl;
            }
        }
    }
    lFile3.close();

    const unsigned int lnbCol = lSize[0];
    const unsigned int lnbLine = lSize[1];
    const unsigned int lnbPix = lnbCol * lnbLine;

    for (unsigned int i = 0; i < lnbLine; i++)
    {
        lIndex[1] = i;
        for (unsigned int j = 0; j < lnbCol; j++)
        {
            unsigned int lRefIndex = i * lnbCol + j;
            lIndex[0] = j;
            lPix[0] = lImage[lRefIndex];
            lPix[1] = lImage[lnbPix + lRefIndex];
            std::cout << "xxxxxxxxxxxxxxxx lPix : " << lIndex << " " << lPix[0] << " " << lPix[1] << std::endl;
            lIPTOACSubPtr->SetPixel(lIndex, lPix); // 0 for band 0
        }
    }

    /** 4 **/
    lFilename = L2DTMSubFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  " << L2DTMSubFileName << std::endl;
    }
    else
    {
        std::cout << "image4 : " << L2DTMSubFileName << std::endl;
    }

    lIndex.Fill(0);
    lSize[0] = lNbOfPix;
    lSize[1] = lSize2;
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
            lDTMSubPtr->SetPixel(lIndex, var); // 0 for band 0
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

    for (unsigned int l = 0; l < lSize[0]; l++)
    {
        lOffset[0] = l;
        for (unsigned int p = 0; p < lSize[1]; p++)
        {
            lOffset[1] = p;
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
        std::cout << "Run LM : Failed to open :  " << lTauMaxFileName << std::endl;
    }

    lFile3.read((char*) (&var), 4);
    double lDarkestPixelAOT = static_cast<double>(var);
    std::cout << "DarkestPixelAOT : " << lDarkestPixelAOT << std::endl;
    lFile3.close();

    /***********************/
    /** AOTbands Creation **/
    /***********************/

    std::vector<unsigned int> lAOTbands;

    lAOTbands.push_back(lBlueBand);
    lAOTbands.push_back(lRedBand);

    std::cout << "bands =" << lBlueBand << " " << lRedBand << std::endl;

    /********************/
    /**    Pipeline    **/
    /********************/

    /** Instantiating the filter */
    RunLMOptimizationType::Pointer lRunLMOptimization = RunLMOptimizationType::New();

    ImageNeighborhoodIteratorType lIPTOACSubIt = ImageNeighborhoodIteratorType(lSize, lIPTOACSubPtr, lRegion);
    DTMNeighborhoodIteratorType lDTMSubIt = DTMNeighborhoodIteratorType(lSize, lDTMSubPtr, lRegion);

    lIPTOACSubIt.GoToBegin();
    lDTMSubIt.GoToBegin();

    lRunLMOptimization->GetCostFunction()->SetSpaceDimension(1);

    lRunLMOptimization->GetCostFunction()->SetMeasureSize(lNbOfPix);

    lRunLMOptimization->GetCostFunction()->SetLUTD(lLUTD);
    lRunLMOptimization->GetCostFunction()->SetAOTbands(lAOTbands);
    lRunLMOptimization->GetCostFunction()->SetDarkestPixelAOT(lDarkestPixelAOT);
    lRunLMOptimization->GetCostFunction()->SetNDVIAOTBand1(0);
    lRunLMOptimization->GetCostFunction()->SetNDVIAOTBand2(1);
    lRunLMOptimization->GetCostFunction()->SetWeq1(lWMS);
    lRunLMOptimization->GetCostFunction()->SetWdark(lWdark);
    lRunLMOptimization->GetCostFunction()->SetWbounds(lWbounds);
    lRunLMOptimization->GetCostFunction()->SetAOTmin(lAOTmin);
    lRunLMOptimization->GetCostFunction()->SetSlopeMS(lSlope);
    lRunLMOptimization->GetCostFunction()->SetYInterceptMS(lYIntercept);
    lRunLMOptimization->GetCostFunction()->SetSlopeNDVI(lSlopeNDVI);
    lRunLMOptimization->GetCostFunction()->SetYInterceptNDVI(lYInterceptNDVI);

    std::cout << "lWdark:" << lWdark << " lWbounds:" << lWbounds << " lAOTmin:" << lAOTmin << std::endl;
    std::cout << " lExtractedOffsets.size " << lExtractedOffsets.size() << std::endl;
    lRunLMOptimization->GetCostFunction()->SetOffsetVect(lExtractedOffsets);
    lRunLMOptimization->GetCostFunction()->SetIPTOACSub(lIPTOACSubIt);
    lRunLMOptimization->GetCostFunction()->SetDTMSub(lDTMSubIt);

    lRunLMOptimization->GetCostFunction()->FindAOTmax();
    double lAOTmax = lRunLMOptimization->GetCostFunction()->GetAOTmax();
    std::cout << "lAOTmax found : " << lAOTmax << std::endl;

    double lAOTFinal = lRunLMOptimization->RunLMOptimization(lF_Tolerance, lG_Tolerance, lX_Tolerance, lEpsilon_Function, lMax_Iterations,
            lAOTinitialD, 0.);

    std::cout << lRunLMOptimization->GetCostFunction() << std::endl;
    std::cout << "lAOTFinal = " << lAOTFinal << std::endl;

    /************************/
    /** tau1    from CNES  **/
    /************************/
    lFilename = lTauDFileName;
    lFile3.open(lFilename.c_str(), std::ios::in | std::ios::binary);
    if (lFile3.fail())
    {
        std::cout << "Run LM : Failed to open :  " << lTauDFileName << std::endl;
    }

    lFile3.read((char*) (&var), 4);
    double lTauD = static_cast<double>(var);
    lFile3.close();

    std::cout << "CNES Solution = " << lTauD << std::endl;

    std::ofstream f(outputfilename);
    f << "lAOTmax found : " << lAOTmax << std::endl;
    f << "lAOTFinal compute : " << lAOTFinal << std::endl;
    f << "CNES Solution   = (" << lTauD << ")" << std::endl;
    f << "                = ( AOTdate D) " << std::endl;
    f.close();

    return EXIT_SUCCESS;

}
