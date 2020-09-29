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
#include <vnsChangeValueWithMaskVectorFunctor.h>
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int vnsChangeValueWithMaskVectorFunctorTest(int /*argc*/, char * argv [])
{
    const unsigned int Dimension = 2;
    const char * inputName = argv[1];
    const char * inputMaskName = argv[2];
    const char * outputName = argv[3];

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef double OutputPixelType;
    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension> InputImageType;
    typedef otb::Image<InputPixelType,Dimension> InputMaskType;
    typedef otb::VectorImage<OutputPixelType,Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputImageType>                               ReaderType;
    typedef otb::ImageFileReader<InputMaskType>                               MaskReaderType;
    typedef otb::ImageFileWriter<OutputImageType>                              WriterType;
    typedef vns::Functor::ChangeValueWithMaskVectorFunctor<InputImageType::PixelType,InputMaskType::PixelType,
            OutputImageType::PixelType> ChangeValueVectorFunctorType;
    typedef itk::BinaryFunctorImageFilter<InputImageType, InputMaskType, OutputImageType,
                        ChangeValueVectorFunctorType> FilterType;

    /** instantiating the filter */
    ReaderType::Pointer reader = ReaderType::New();
    MaskReaderType::Pointer maskReader = MaskReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    FilterType::Pointer filter = FilterType::New();


    reader->SetFileName(inputName);
    reader->UpdateOutputInformation();

    OutputImageType::PixelType changeValue;
    changeValue.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
    changeValue.Fill(1);

    maskReader->SetFileName(inputMaskName);
    filter->SetInput1(reader->GetOutput());
    filter->SetInput2(maskReader->GetOutput());

    filter->GetFunctor().SetBackgroundMaskValue(0);
    filter->GetFunctor().SetChange(changeValue);
    writer->SetFileName(outputName);
    writer->SetInput(filter->GetOutput());
    writer->SetNumberOfDivisionsStrippedStreaming(30);
    writer->Update();

    return EXIT_SUCCESS;
}
