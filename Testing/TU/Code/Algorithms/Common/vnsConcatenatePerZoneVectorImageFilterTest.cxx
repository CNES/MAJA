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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * MOD : VERSION : 1-0-0  : 24 nov. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsConcatenatePerZoneVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbObjectList.h"

int
vnsConcatenatePerZoneVectorImageFilterTest(int /*argc*/, char * argv[])
{

    const char * anglesFilename1 = argv[1];
    const char * anglesFilename2 = argv[2];
    const char * maskFilename = argv[3];
    const char * outputFilename = argv[4];


    const unsigned int Dimension = 2;

    typedef otb::VectorImage<double, Dimension> InputImageType;
    typedef otb::Image<unsigned int, Dimension> MaskImageType;
    typedef otb::ImageFileReader<InputImageType> ImageReaderType;
    typedef otb::ImageFileReader<MaskImageType>  MaskReaderType;
    typedef otb::ImageFileWriter<InputImageType> WriterType;

    typedef vns::ConcatenatePerZoneVectorImageFilter<InputImageType, MaskImageType, InputImageType > FilterType;
    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    ImageReaderType::Pointer reader1 = ImageReaderType::New();
    ImageReaderType::Pointer reader2 = ImageReaderType::New();
    MaskReaderType::Pointer maskReader = MaskReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    otb::ObjectList<InputImageType>::Pointer inputList = otb::ObjectList<InputImageType>::New();

    reader1->SetFileName(anglesFilename1);
    reader1->UpdateOutputInformation();
    reader2->SetFileName(anglesFilename2);
    reader2->UpdateOutputInformation();
    maskReader->SetFileName(maskFilename);
    maskReader->UpdateOutputInformation();

    inputList->PushBack(reader1->GetOutput());
    inputList->PushBack(reader2->GetOutput());

    std::vector<unsigned int> lisftOfZones;
    lisftOfZones.push_back(4);
    lisftOfZones.push_back(6);

    filter->SetInputList(inputList);
    filter->SetInputMask(maskReader->GetOutput());
    filter->SetListOfZones(lisftOfZones);
    //filter->UpdateData();

    writer->SetFileName(outputFilename);
    writer->SetInput(filter->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;
}
