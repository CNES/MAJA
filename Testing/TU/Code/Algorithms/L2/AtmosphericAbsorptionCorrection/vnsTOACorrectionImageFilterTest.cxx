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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsLoggers.h"

#include "vnsTOACorrectionImageFilter.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsUtilities.h"
int
vnsTOACorrectionImageFilterTest(int argc, char * argv[])
{
    if (argc != 9)
    {
        std::cout << "Invalid input arguments. It should be:" << std::endl;
        std::cout
                << "<input TOA image path> \n <input water image path> \n <input pressure image path> \n <smac coefficient file path> \n <output corrected TOA image path> \n <ozone amount value> \n <solar zenithal angle> \n <view zenithal angle> "
                << std::endl;
        std::cout << argc << std::endl;
        return EXIT_FAILURE;
    }

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const char * toaFilename = argv[1];
    const char * waterFilename = argv[2];
    const char * pressureFilename = argv[3];
    const char * smacFilename = argv[4];
    const char * toaSurfFilename = argv[5];
    double ozoneAmount = atof(argv[6]);
    double thetaS = atof(argv[7]);
    double thetaV = atof(argv[8]);

    typedef double PixelType;
    typedef otb::VectorImage<PixelType> VectorInpuImageType;
    typedef otb::Image<PixelType> ImageType;
    typedef otb::ImageFileReader<VectorInpuImageType> VectorImageFileReaderType;
    typedef otb::ImageFileReader<ImageType> ImageFileReaderType;
    typedef otb::ImageFileWriter<VectorInpuImageType> VectorImageFileWriterType;
    typedef std::vector<double> DoubleVectorType;

    typedef vns::TOACorrectionImageFilter<VectorInpuImageType, ImageType, VectorInpuImageType, ImageType> TOACorrectionImageFilterType;

    // Load the SMAC coefficients
    std::ifstream file(smacFilename);

    std::vector<std::string> splittedLine;
    std::vector<double> vect1, vect2, vect3, vect4;
    if (file)
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::string line2(line);
            splittedLine = vns::Utilities::SplitString(line2, '\t');
            if (splittedLine.size() < 4)
            {
                file.close();
                std::cout << "Invalid input smac file." << std::endl;
                return EXIT_FAILURE;
            }
            vect1.push_back(vns::Utilities::AsDouble(splittedLine.at(0)));
            vect2.push_back(vns::Utilities::AsDouble(splittedLine.at(1)));
            vect3.push_back(vns::Utilities::AsDouble(splittedLine.at(2)));
            vect4.push_back(vns::Utilities::AsDouble(splittedLine.at(3)));
        }
    }

    else
    {
        file.close();
        std::cout << "Invalid input smac file name " << smacFilename << std::endl;
        return EXIT_FAILURE;
    }

    // Set the input readers
    VectorImageFileReaderType::Pointer toaReader = VectorImageFileReaderType::New();
    ImageFileReaderType::Pointer waterReader = ImageFileReaderType::New();
    ImageFileReaderType::Pointer presReader = ImageFileReaderType::New();
    toaReader->SetFileName(toaFilename);
    toaReader->GenerateOutputInformation();
    waterReader->SetFileName(waterFilename);
    presReader->SetFileName(pressureFilename);

    // init and link filter
    TOACorrectionImageFilterType::Pointer filter = TOACorrectionImageFilterType::New();
    filter->SetTOAInput(toaReader->GetOutput());
    filter->SetWaterInput(waterReader->GetOutput());
    filter->SetPressureInput(presReader->GetOutput());

    // set smac coef to the filter
    filter->AddSmacCoefContainer(vect1);
    filter->AddSmacCoefContainer(vect2);
    filter->AddSmacCoefContainer(vect3);
    filter->AddSmacCoefContainer(vect4);

    // set ozone amount
    filter->SetOzoneAmount(ozoneAmount);

    // set angles
    filter->SetThetaS(thetaS);
    DoubleVectorType vThetaV(toaReader->GetOutput()->GetNumberOfComponentsPerPixel(), thetaV);
    filter->SetThetaV(vThetaV);

    // Init the writer
    VectorImageFileWriterType::Pointer writer = VectorImageFileWriterType::New();
    writer->SetFileName(toaSurfFilename);
    writer->SetInput(filter->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;
}
