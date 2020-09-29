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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsStereoCorrectionImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsDataManagerTestingLibrary.h"


int vnsStereoCorrectionImageFilterTest(int argc, char * argv [])
{
    if(argc!=11)
    {
    	std::cerr << argv[0] << " <IPCLA filename> <IPCLDAll filename> "
    	        "<IPCLDExtLabel output filename> <IPCLA output filename>"
    	        "<Excluded altitude value> <dafault label value> <KSigam altitude> <Default altitude>"<< std::endl;
        return EXIT_FAILURE;
    }

    const unsigned int Dimension = 2;

    const char * l_IPinCLAFileName = argv[1];
    const char * l_IPCLDAllFileName  = argv[2];
    const char * l_IPCLDAllLabelFileName = argv[3];
    const char * l_IPCLAFileName = argv[4];
    unsigned short l_ExcludedValue = atoi(argv[5]);
    unsigned short l_DefalutValue = atoi(argv[6]);
    unsigned short l_KSigma = atoi(argv[7]);
    short l_DefaultAltitude = atoi(argv[8]);

    
    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned char InputMaskPixelType;
    typedef unsigned short LabelPixelType;
    typedef double OutputPixelType;

    /** Image typedefs */
    typedef otb::Image<InputPixelType,Dimension> InputImageType;
    typedef otb::Image<InputMaskPixelType,Dimension> InputMaskType;
    typedef otb::Image<LabelPixelType,Dimension> LabelImageType;
    typedef otb::Image<OutputPixelType,Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputImageType>            ReaderType;
    typedef otb::ImageFileReader<InputMaskType>             InputMaskReaderType;
    typedef otb::ImageFileWriter<LabelImageType>            LabelWriterType;
    typedef otb::ImageFileWriter<OutputImageType>           WriterType;

    typedef vns::StereoCorrectionImageFilter<InputImageType, InputMaskType, LabelImageType, OutputImageType>  FilterType;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_IPCLAReader  = ReaderType::New();
    InputMaskReaderType::Pointer l_IPCLDAllReader = InputMaskReaderType::New();
    LabelWriterType::Pointer l_IPCLDAllLabelWriter = LabelWriterType::New();
    WriterType::Pointer l_IPCLAWriter = WriterType::New();

    l_IPCLAReader->SetFileName(l_IPinCLAFileName);
    l_IPCLDAllReader->SetFileName(l_IPCLDAllFileName);

    l_IPCLDAllLabelWriter->SetFileName(l_IPCLDAllLabelFileName);
    l_IPCLAWriter->SetFileName(l_IPCLAFileName);

    l_Filter->SetIPCLAInput(l_IPCLAReader->GetOutput());
    l_Filter->SetIPCLDAllInput(l_IPCLDAllReader->GetOutput());

    l_Filter->SetExcludedValue(l_ExcludedValue);
    l_Filter->SetDefaultValue(l_DefalutValue);
    l_Filter->SetKsigmaAltitude(l_KSigma);
    l_Filter->SetDefaultAltitude(l_DefaultAltitude);

    l_Filter->UpdateWithCaching();

    l_IPCLDAllLabelWriter->SetInput(l_Filter->GetIPCLDAllLabelOutput());
    l_IPCLAWriter->SetInput(l_Filter->GetIPCLAOutput());

    l_IPCLDAllLabelWriter->Update();
    l_IPCLAWriter->Update();

    return EXIT_SUCCESS;
}
