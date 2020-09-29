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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsInternalSlopeCorrectionImageFilterNew.cxx 3726 2012-01-26 08:20:23Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsLoggers.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsIncidenceAngleFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"


int vnsIncidenceAngleFunctorTest(int argc, char * argv [])
{
    if (argc != 6)
    {
        return EXIT_FAILURE;
    }

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const char * AFileName = argv[1];
    const char * SFileName = argv[2];
    double lThetaS = static_cast< double> (atof(argv[3]));
    double lPhiS   = static_cast< double> (atof(argv[4]));
    const char * IncidenceAngleFileName = argv[5];


    typedef otb::Image<double,2>                        InputImageType;
    typedef InputImageType::PixelType                   InputImagePixelType;

    typedef otb::ImageFileReader<InputImageType>        ReaderType;
    typedef ReaderType::Pointer                         ReaderPointerType;
    typedef otb::MultiplyByScalarImageFilter<InputImageType,InputImageType>  MultiplyFilterType;

    typedef otb::Image<double, 2>                       InternalIncidenceAngleImageType;
    typedef InternalIncidenceAngleImageType::PixelType  InternalIncidenceAnglePixelType;

    typedef otb::ImageFileWriter<InternalIncidenceAngleImageType>   InternalIncidenceAngleImageWriterType;
    typedef InternalIncidenceAngleImageWriterType::Pointer          InternalIncidenceAngleImageWriterPointerType;


    typedef vns::Functor::IncidenceAngleFunctor< InputImagePixelType, InputImagePixelType, InternalIncidenceAnglePixelType > IncidenceAngleFunctorType;
    typedef itk::BinaryFunctorImageFilter< InputImageType, InputImageType, InternalIncidenceAngleImageType, IncidenceAngleFunctorType> IncidenceAngleFilterType;
    typedef IncidenceAngleFilterType::Pointer IncidenceAngleFilterPointer;

    // instantiating the readers
    ReaderPointerType reader1 = ReaderType::New();
    ReaderPointerType reader2 = ReaderType::New();

    // pipeline
    reader1->SetFileName(AFileName);
    reader2->SetFileName(SFileName);

    MultiplyFilterType::Pointer lMultiplyFilter1 = MultiplyFilterType::New();
    MultiplyFilterType::Pointer lMultiplyFilter2 = MultiplyFilterType::New();
    // note : for A and S images coef 100
    lMultiplyFilter1->SetInput(reader1->GetOutput());
    lMultiplyFilter1->SetCoef(0.01);
    lMultiplyFilter2->SetInput(reader2->GetOutput());
    lMultiplyFilter2->SetCoef(0.01);

    IncidenceAngleFilterPointer l_IncidenceAngleFilter = IncidenceAngleFilterType::New();
    l_IncidenceAngleFilter->SetInput1( lMultiplyFilter1->GetOutput() );
    l_IncidenceAngleFilter->SetInput2( lMultiplyFilter2->GetOutput() );
    l_IncidenceAngleFilter->GetFunctor().SetPhiS( lPhiS * otb::CONST_PI_180 );
    l_IncidenceAngleFilter->GetFunctor().SetThetaS( lThetaS * otb::CONST_PI_180 );

    // instantiating the writers
    InternalIncidenceAngleImageWriterPointerType writer1 = InternalIncidenceAngleImageWriterType::New();
    writer1->SetFileName( IncidenceAngleFileName );
    writer1->SetInput( l_IncidenceAngleFilter->GetOutput() );
    writer1->Update();

    return EXIT_SUCCESS;
}
