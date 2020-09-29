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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsChangeValueVectorFunctor.h"
#include "otbVectorImage.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int vnsChangeValueVectorImageFilterTest(int /*argc*/, char * argv [])
{
    const unsigned int Dimension = 2;
    const char * inputName = argv[1];
    const char * outputName = argv[2];

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef double OutputPixelType;
    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension> InputImageType;
    typedef otb::VectorImage<OutputPixelType,Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputImageType>                               ReaderType;
    typedef otb::ImageFileWriter<OutputImageType>                              WriterType;
    typedef vns::Functor::VectorChangeValue<InputImageType::PixelType, OutputImageType::PixelType> VectorChangeFunctorType;
    typedef itk::UnaryFunctorImageFilter<InputImageType, OutputImageType, VectorChangeFunctorType> FilterType;


    /** instantiating the filter */
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    FilterType::Pointer filter = FilterType::New();

    InputImageType::PixelType inputValue;
    OutputImageType::PixelType changeValue;
    /*inputValue.SetSize(4);
    inputValue[0]= -10000.025;
    inputValue[1]= -10000.022;
    inputValue[2]= -10000.085;
    inputValue[3]= -10000.003;
    changeValue.SetSize(4);
    changeValue[0]=1;
    changeValue[1]=1;
    changeValue[2]=1;
    changeValue[3]=1;*/

    inputValue.SetSize(1);
    inputValue[0]= 0;
    changeValue.SetSize(1);
    changeValue[0]=100;


    reader->SetFileName(inputName);
    reader->UpdateOutputInformation();
    filter->SetInput(reader->GetOutput());
    filter->GetFunctor().SetNumberOfComponentsPerPixel(reader->GetOutput()->GetNumberOfComponentsPerPixel());
    filter->GetFunctor().SetChange(inputValue,changeValue);
    writer->SetFileName(outputName);
    writer->SetInput(filter->GetOutput());
    writer->SetNumberOfDivisionsStrippedStreaming(30);
    writer->Update();

    return EXIT_SUCCESS;
}
