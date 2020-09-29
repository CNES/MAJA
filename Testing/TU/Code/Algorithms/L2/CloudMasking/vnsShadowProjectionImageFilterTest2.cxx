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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsShadowProjectionImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkConnectedComponentImageFilter.h"

#include "vnsConstantVectorImageSource.h"
#include "vnsConstantImageSource.h"

#include "vnsDEMDataManager.h"
#include <fstream>


// This test is used to launch a test only with input images coming from others tests
// This test is not launched systematically

int vnsShadowProjectionImageFilterTest2(int argc, char * argv [])
{
    if(argc!=10)
    {
    	std::cerr << argv[0] << " <L2CLDSub mask filename> <DTM filename> "
    	        "<output filename> <VIEHRef> <SOLHRef>"
    	        "<VIE zenith angle> <VIE azimuth angle> "
                "<SOL zenith angle> <SOL azimuth angle> <default altitude> <L1SpacingX> <L1SpacingY>"<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_IPSOLInputFileName  = argv[1];
    const char * l_L2CLDSubInputFileName  = argv[2];
    const char * l_L2CLDSubLabelInputFileName  = argv[3];
    const char * l_DTMInputFileName  = argv[4];
    const char * l_IPVIEInputFileName  = argv[5];
    const char * l_IPCLAInputFileName  = argv[6];
    const char * l_OutputFileName = argv[7];
    double l_VIEHRef = atof(argv[8]);
    double l_SOLHRef = atof(argv[9]);

    unsigned int l_NbBand = 2;

    /** Pixel typedefs */
    typedef unsigned char InputMaskPixelType;
    typedef unsigned short LabelPixelType;
    typedef double DTMPixelType;
    typedef double GRIDPixelType;
    typedef double CLAPixelType;

    /** Image typedefs */
    typedef otb::Image<InputMaskPixelType,Dimension> InputMaskType;
    typedef otb::Image<LabelPixelType,Dimension> LabelImageType;
    typedef otb::Image<DTMPixelType,Dimension> DTMImageType;
    typedef otb::VectorImage<GRIDPixelType,Dimension> GRIDImageType;
    typedef otb::Image<CLAPixelType,Dimension> CLAImageType;

    typedef otb::ImageFileReader<InputMaskType>             InputMaskReaderType;
    typedef otb::ImageFileReader<DTMImageType>              DTMReaderType;
    typedef otb::ImageFileReader<GRIDImageType>             GRIDReaderType;
    typedef otb::ImageFileReader<CLAImageType>              CLAReaderType;
    typedef otb::ImageFileReader<LabelImageType>            LabelReaderType;

    typedef vns::DEMDataManager<DTMImageType>          Intersector3DType;

    typedef vns::ShadowProjectionImageFilter<InputMaskType,
                                             LabelImageType,
                                             DTMImageType,
                                             GRIDImageType,
                                             CLAImageType>  FilterType;
    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();

    GRIDReaderType::Pointer l_IPSOLInput = GRIDReaderType::New();
    l_IPSOLInput->SetFileName(l_IPSOLInputFileName);

    InputMaskReaderType::Pointer l_L2CLDSubInput = InputMaskReaderType::New();
    l_L2CLDSubInput->SetFileName(l_L2CLDSubInputFileName);

    LabelReaderType::Pointer l_L2CLDSubLabelInput = LabelReaderType::New();
    l_L2CLDSubLabelInput->SetFileName(l_L2CLDSubLabelInputFileName);
    DTMReaderType::Pointer l_DTMInput = DTMReaderType::New();
    l_DTMInput->SetFileName(l_DTMInputFileName);
    GRIDReaderType::Pointer l_IPVIEInput = GRIDReaderType::New();
    l_IPVIEInput->SetFileName(l_IPVIEInputFileName);
    CLAReaderType::Pointer l_IPCLAInput = CLAReaderType::New();
    l_IPCLAInput->SetFileName(l_IPCLAInputFileName);

    std::cout<<"Before LoadI3D" <<std::endl;

    // ------------------------------------
    // Load the DTM image in intersector 3D
    // ------------------------------------

    Intersector3DType l_I3D;

    l_DTMInput->Update();

    try
    {
    	l_I3D.Load(l_DTMInput->GetOutput());
    }
    catch(...)
    {
        std::cout << "Error of downloading of the buffered DTM." << std::endl;
    }

    std::cout<<"Apres LoadI3D" <<std::endl;


    // --------------------------------------------
    // Constant viewing and solar directions images
    // --------------------------------------------

    // DTM Reader
    l_DTMInput->Update();
    l_DTMInput->GenerateOutputInformation();

    std::cout <<  "DTM = " << l_DTMInput << std::endl;

    // ----------------------------------
    // Set input images
    // ----------------------------------
    l_Filter->SetL2CLDSubInput(l_L2CLDSubInput->GetOutput());
    l_Filter->SetL2CLDSubLabelInput(l_L2CLDSubLabelInput->GetOutput());
    l_Filter->SetDTMInput(l_DTMInput->GetOutput());
    l_Filter->SetIPVIEInput(l_IPVIEInput->GetOutput());
    l_Filter->SetIPSOLInput(l_IPSOLInput->GetOutput());
    l_Filter->SetIPCLAInput(l_IPCLAInput->GetOutput());

    l_Filter->SetI3D( & l_I3D);

    l_Filter->SetVIEHRef(l_VIEHRef);
    l_Filter->SetSOLHRef(l_SOLHRef);

    l_Filter->Update();

    std::fstream file(l_OutputFileName, std::ios::out);
    file<<l_Filter;
    file.close();

    return EXIT_SUCCESS;
}
