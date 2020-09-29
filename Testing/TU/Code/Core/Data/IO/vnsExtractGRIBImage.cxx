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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/


#include "itkMacro.h"

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"

#include "otbImageFileReader.h"
#include "vnsGRIBImageIOFactory.h"
#include "itkObjectFactoryBase.h"
#include "vnsLoggers.h"

#include <iostream>

int vnsExtractGRIBImage(int, char* argv[])
{  
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  int startX = atoi(argv[3]);
  int startY = atoi(argv[4]);
  int sizeX = atoi(argv[5]);
  int sizeY = atoi(argv[6]);

  // Configure Logger
  vns::Loggers::GetInstance()->Initialize(argv[0]);
  vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);


  typedef double                            PixelType;
  typedef otb::Image< PixelType >           ImageType;

  typedef otb::ExtractROI< PixelType, PixelType > ExtractorType;
  typedef otb::ImageFileReader< ImageType >       ReaderType;
  typedef otb::ImageFileWriter< ImageType >       WriterType;

  itk::ObjectFactoryBase::RegisterFactory( vns::GRIBImageIOFactory::New() );
  ExtractorType::Pointer extract = ExtractorType::New();
  ReaderType::Pointer    reader  = ReaderType::New();
  WriterType::Pointer    writer  = WriterType::New();

  reader->SetFileName( inputFilename );

  extract->SetInput( reader->GetOutput() );
  extract->SetStartX( startX);
  extract->SetStartY( startY);
  extract->SetSizeX( sizeX);
  extract->SetSizeY( sizeY);

  writer->SetFileName( outputFilename );
  writer->SetInput( extract->GetOutput() );

  writer->Update();
  

  return EXIT_SUCCESS;
}
