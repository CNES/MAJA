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
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 29 fevrier 2012 : Creation du functor pour MACCS                *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "itkImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkGradientMagnitudeImageFilter.h"

int vnsGradientMagnitudeImageFilterTest(int argc, char * argv [])
{

    if(argc!=4)
    {
        std::cerr << argv[0] << " <input filename> <output filename> use spacing" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int cpt(1);
    const char * inputFileName = argv[cpt++];
    const char * outputFileName = argv[cpt++];
    bool l_UseImageSpacing = static_cast<bool>(atoi(argv[cpt++]));
    const unsigned int Dimension = 2;
    /** Pixel typedefs */
    typedef double PixelType;
    /** Image typedefs */
    typedef itk::Image<PixelType,Dimension> InputImageType;
    typedef itk::Image<PixelType,Dimension> OutputImageType;

    /** Reader typedef */
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef ReaderType::Pointer ReaderPointerType;

    /* Writer typedef */
    typedef otb::ImageFileWriter<OutputImageType> WriterType;
    typedef WriterType::Pointer WriterPointerType;

    /** Filter typedef */
    //typedef itk::ZeroFluxNeumannBoundaryCondition<InputImageType> BoundaryConditionType;
    typedef itk::GradientMagnitudeImageFilter<InputImageType,OutputImageType> FilterType;

    /** Instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    ReaderPointerType reader = ReaderType::New();
    WriterPointerType writer = WriterType::New();

    filter->SetUseImageSpacing(l_UseImageSpacing);
    reader->SetFileName(inputFileName);
    writer->SetFileName(outputFileName);
    filter->SetInput(reader->GetOutput());
    writer->SetInput(filter->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;
}
