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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsValidShadowMaskImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "vnsLoggers.h"

#include <fstream>

int vnsValidShadowMaskImageFilterTest(int argc, char * argv [])
{
  if (argc != 5)
    {
      std::cerr << argv[0] << "<Shadow Label input filename> "
              "<Diff input filename> <image filename> <output text result filename>"<< std::endl;
      return EXIT_FAILURE;
    }

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;

    /** Pixel typedefs */
    typedef double       InputPixelType;
    typedef unsigned int InputMaskPixelType;
    /** Image typedefs */
    typedef otb::Image<InputMaskPixelType> MaskLabelImageType;
    typedef otb::Image<InputPixelType> DiffImageType;
    typedef otb::VectorImage<InputPixelType,Dimension> InputImageType;

    typedef otb::ImageFileReader<MaskLabelImageType>  LabelReaderType;
    typedef otb::ImageFileReader<DiffImageType>       DiffReaderType;
    typedef otb::ImageFileReader<InputImageType>       InputReaderType;

    

    typedef vns::ValidShadowMaskImageFilter<DiffImageType, InputImageType, MaskLabelImageType>  FilterType;

    LabelReaderType::Pointer readerShadLab  = LabelReaderType::New();
    DiffReaderType::Pointer   readerDiff = DiffReaderType::New();
    InputReaderType::Pointer  readerRefl = InputReaderType::New();
    
    readerShadLab->SetFileName(argv[1]);
    readerDiff->SetFileName(argv[2]);
    readerRefl->SetFileName(argv[3]);

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();

    filter->SetIPDiffInput( readerDiff->GetOutput() );
    filter->SetIPCLDShadLabel( readerShadLab->GetOutput() );
    filter->SetReflInput( readerRefl->GetOutput() );

    filter->SetExcludedValue(1);
    filter->SetExcludedLabelValue(0);
      

    filter->Update();

    // The ratio diff/refl = 1 (same image) => sum(ratio) = number of pixels
    std::fstream file(argv[4], std::ios::out);
    file<<filter;
    file.close();

    return EXIT_SUCCESS;
}
