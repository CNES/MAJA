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
 * MOD : VERSION : 1-0-0  : 19 oct. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsSnowMaskDeterminationImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkOrImageFilter.h"

int vnsSnowMaskDeterminationImageFilterTest(int argc, char * argv [])
{
    if(argc!=11)
    {
    	std::cerr << argv[0] << " <tocr filename> <edg filename> <cirrus mask filename> <ref band> \
    	        <abs band> <red band> <NDSI threshold> \
    	        <red threshold> <SWIR threshold> <output filename> "<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    unsigned int cpt(1);

    const char * TOCRFileName  = argv[cpt++];
    const char * EDGFileName  = argv[cpt++];
    const char * CirrusMaskFileName  = argv[cpt++];
    unsigned int GreenBand = static_cast<unsigned int>(atoi(argv[cpt++]));
    unsigned int SWIRBand = static_cast<unsigned int>(atoi(argv[cpt++]));
    unsigned int RedBand = static_cast<unsigned int>(atoi(argv[cpt++]));
    double NDSIThreshold = static_cast<double>(atof(argv[cpt++]));
    double RedThreshold = static_cast<double>(atof(argv[cpt++]));
    double SWIRThreshold = static_cast<double>(atof(argv[cpt++]));
    const char * outputFileName = argv[cpt++];

    
    /** Pixel typedefs */
    const unsigned int Dimension = 2;
    typedef double InputPixelType;
    typedef unsigned char MaskPixelType;
    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension>    InputImageType;
    typedef otb::Image<double, 2>                       InternalIncidenceAngleImageType;
    typedef otb::Image<MaskPixelType,Dimension>   MaskImageType;
    typedef otb::ImageFileReader<InputImageType>    ReaderType;
    typedef otb::ImageFileReader<MaskImageType>    MaskReaderType;
    typedef otb::ImageFileWriter<MaskImageType>   WriterType;

    typedef itk::OrImageFilter<MaskImageType,MaskImageType,MaskImageType> OrFilterType;
    typedef vns::SnowMaskDeterminationImageFilter<InputImageType,MaskImageType,InternalIncidenceAngleImageType,MaskImageType>  FilterType;

    /** instantiating the filter */
    OrFilterType::Pointer orFilter = OrFilterType::New();
    FilterType::Pointer filter = FilterType::New();
    ReaderType::Pointer TOCRreader  = ReaderType::New();
    MaskReaderType::Pointer EDGreader  = MaskReaderType::New();
    MaskReaderType::Pointer CirrusMaskreader  = MaskReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    TOCRreader->SetFileName(TOCRFileName);
    EDGreader->SetFileName(EDGFileName);
    CirrusMaskreader->SetFileName(CirrusMaskFileName);
    writer->SetFileName(outputFileName);

    orFilter->SetInput1(EDGreader->GetOutput());
    orFilter->SetInput2(CirrusMaskreader->GetOutput());

    filter->SetIPTOCRInput(TOCRreader->GetOutput());
    filter->SetIPNotValidMaskSubInput(orFilter->GetOutput());
///////filter->SetIncidenceAngleInput()
    filter->SetRefBand(GreenBand);
    filter->SetAbsBand(SWIRBand);
    filter->SetRedBand(RedBand);
    filter->SetNDSIThreshold(NDSIThreshold);
    filter->SetRedThreshold(RedThreshold);
    filter->SetSWIRThreshold(SWIRThreshold);
    filter->SetHasSWIR(true);

    writer->SetInput(filter->GetOutput());

    writer->Update();

    return EXIT_SUCCESS;
}
