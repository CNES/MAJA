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
 * VERSION : 4-0-0 : <TypeFT> : <NumFT> : 6 janvier 2014 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsShadowMaskDeterminationImageFilterNew.cxx 4915 2012-11-30 17:34:21Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "vnsSnowMaskCorrectionImageFilter.h"

#include "vnsDataManagerTestingLibrary.h"
#include "vnsLoggers.h"

int vnsSnowMaskCorrectionImageFilterTest(int argc, char * argv[])
{
	if (argc != 10)
	{
		std::cerr << argv[0] << "<Snow label border mask filename> <Edge mask filename> "
			  "<Cloud refl mask filename> <Cloud refl var mask filename> <dilate radius> "
			  "<Updated cloud refl mask> <Updated snow mask> "<< std::endl;
		return EXIT_FAILURE;
	}

	unsigned int jcpt1(0);
	unsigned int jcpt2(0);
	jcpt1 = argc-1;
	jcpt2 = argc-2;

	// Configure Logger

    const unsigned int Dimension = 2;
    /** Pixel typedefs */
    typedef unsigned char MaskPixelType;
    /** Image typedefs */
    typedef otb::Image<MaskPixelType,Dimension> MaskType;

    typedef otb::ImageFileReader<MaskType>  MaskReaderType;
    typedef otb::ImageFileWriter<MaskType> WriterType;

    typedef vns::SnowMaskCorrectionImageFilter<MaskType>  FilterType;
    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();

    MaskReaderType::Pointer snwReader  = MaskReaderType::New();
	MaskReaderType::Pointer edgReader  = MaskReaderType::New();
	MaskReaderType::Pointer cldReflReader  = MaskReaderType::New();
	MaskReaderType::Pointer cldReflVarReader  = MaskReaderType::New();

	WriterType::Pointer cldWriter = WriterType::New();
	WriterType::Pointer snwWriter = WriterType::New();

	snwReader->SetFileName(argv[1]);
	edgReader->SetFileName(argv[2]);
	cldReflReader->SetFileName(argv[3]);
	cldReflVarReader->SetFileName(argv[4]);

	unsigned short radius(atoi(argv[5]));

	filter->SetIPSNWSubInput( snwReader->GetOutput() );
	filter->SetIPEDGSubInput( edgReader->GetOutput() );
	filter->SetIPCLDReflSubInput( cldReflReader->GetOutput() );
	filter->SetIPCLDReflVarSubInput( cldReflVarReader->GetOutput() );

	filter->SetDilateRadius(radius);
	filter->SetOutputMaskValue(static_cast<MaskPixelType>(255));

	filter->UpdateWithCaching();

	cldWriter->SetInput( filter->GetIPCLDReflSubOutput() );
	cldWriter->SetFileName( argv[6] );
	cldWriter->Update();

	snwWriter->SetInput( filter->GetIPSNWSubOutput() );
	snwWriter->SetFileName( argv[7] );
	snwWriter->Update();

    return EXIT_SUCCESS;
}
