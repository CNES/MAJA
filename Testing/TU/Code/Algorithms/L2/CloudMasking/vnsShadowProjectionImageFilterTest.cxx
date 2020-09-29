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
#include "vnsUtilities.h"

int vnsShadowProjectionImageFilterTest(int argc, char * argv [])
{
    if(argc!=13)
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
    const char * l_L2CLDSubFileName  = argv[1];
    const char * l_DTMFileName  = argv[2];
    const char * l_OutputFileName  = argv[3];

    double l_VIEHRef = atof(argv[4]);
    // Not used with Formosat : l_SOLHref = -cos(theta_s) - see line 101
    double l_SOLHRef = atof(argv[5]);

    const double l_VIEZenithRad(vns::Utilities::AsDouble(argv[6]) * otb::CONST_PI_180);
    const double l_VIEAzimuthRad(vns::Utilities::AsDouble(argv[7]) * otb::CONST_PI_180);
    const double l_SOLZenithRad(vns::Utilities::AsDouble(argv[8]) * otb::CONST_PI_180);
    const double l_SOLAzimuthRad(vns::Utilities::AsDouble(argv[9]) * otb::CONST_PI_180);

    const double l_CLAValue(vns::Utilities::AsDouble(argv[10]));

    // x = h*tg(theta_v)sin(phi_v) - col -> lon
    // y = h*tg(theta_v)cos(phi_v) - lin -> lat
    // z = h

    std::vector<double> l_VIEValue(2);
    l_VIEValue[0] = l_VIEHRef * vcl_tan(l_VIEZenithRad) * vcl_sin(l_VIEAzimuthRad);
    l_VIEValue[1] = l_VIEHRef * vcl_tan(l_VIEZenithRad) * vcl_cos(l_VIEAzimuthRad);

    // x = -sin(theta_s)sin(phi_s)
    // y = -sin(theta_s)cos(phi_s)
    // z = -cos(theta_s)
    // downward direction (opposite sign)

    // Grids are first generated in pixels and then converted in meters dividing by the spacing
    // in order to fit with venus grids
    // see lines 194:197
    // An arbitrary value of SOLHref is used for formosat
    std::vector<double> l_SOLValue(2);
    l_SOLValue[0] = l_SOLHRef * vcl_tan(l_SOLZenithRad) * vcl_sin(l_SOLAzimuthRad);
    l_SOLValue[1] = l_SOLHRef * vcl_tan(l_SOLZenithRad) * vcl_cos(l_SOLAzimuthRad);


    std::cout << l_VIEValue[0] << " " << l_VIEValue[1] << std::endl;
    std::cout << l_SOLValue[0] << " " << l_SOLValue[1] << std::endl;


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

    typedef vns::DEMDataManager<DTMImageType> Intersector3DType;



    typedef vns::ShadowProjectionImageFilter<InputMaskType,
                                             LabelImageType,
                                             DTMImageType,
                                             GRIDImageType,
                                             CLAImageType>  FilterType;

    // Labelize filter type
    typedef itk::ConnectedComponentImageFilter<InputMaskType, LabelImageType> LabelizeFilterType;

    // Constant Image filter type
    typedef vns::ConstantVectorImageSource<GRIDImageType>  ConstantVectorFilterType;
    typedef vns::ConstantImageSource<CLAImageType>         ConstantFilterType;

    GRIDImageType::PixelType l_VIEPixel;
    GRIDImageType::PixelType l_SOLPixel;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    LabelizeFilterType::Pointer l_LabelFilter = LabelizeFilterType::New();
    ConstantVectorFilterType::Pointer l_VIEConstantFilter = ConstantVectorFilterType::New();
    ConstantVectorFilterType::Pointer l_SOLConstantFilter = ConstantVectorFilterType::New();
    ConstantFilterType::Pointer l_CLAConstantFilter = ConstantFilterType::New();
    InputMaskReaderType::Pointer l_L2CLDSubReader  = InputMaskReaderType::New();
    CLAReaderType::Pointer l_IPCLAReader = CLAReaderType::New();
    DTMReaderType::Pointer l_DTMReader = DTMReaderType::New();

    // --------
    // Readers
    // --------
    l_L2CLDSubReader->SetFileName(l_L2CLDSubFileName);
    l_DTMReader->SetFileName(l_DTMFileName);
    l_DTMReader->Update();

    // ------------------------------------
    // Load the DTM image in intersector 3D
    // ------------------------------------
    Intersector3DType l_I3D;

    try
    {
    	l_I3D.Load(l_DTMReader->GetOutput());
    }
    catch(...)
    {
        std::cout << "Error of downloading of the buffered DTM." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout<<"Apres LoadI3D" <<std::endl;


    // --------------------------------------------
    // Constant viewing and solar directions images
    // --------------------------------------------

    // Get the physical coordinates of the upper left corner
    l_L2CLDSubReader->GenerateOutputInformation();
    InputMaskType::PointType l_L2CLDSubOrigin;
    l_L2CLDSubOrigin = l_L2CLDSubReader->GetOutput()->GetOrigin();

    GRIDImageType::SizeValueType l_Size[2];

    l_Size[0] = 375;
    l_Size[1] = 334;
    l_VIEConstantFilter->SetSize(l_Size);
    l_SOLConstantFilter->SetSize(l_Size);

    GRIDImageType::SpacingValueType l_Spacing[2] = {96., -96.};
    l_VIEConstantFilter->SetSpacing( l_Spacing );
    l_SOLConstantFilter->SetSpacing( l_Spacing );
    GRIDImageType::SpacingType l_L1Spacing;
    l_L1Spacing[0] = atof(argv[11]);
    l_L1Spacing[1] = atof(argv[12]);

    GRIDImageType::PointValueType l_Origin[2];
    l_Origin[0] = l_L2CLDSubOrigin[0];
    l_Origin[1] = l_L2CLDSubOrigin[1];
    l_VIEConstantFilter->SetOrigin( l_Origin );
    l_SOLConstantFilter->SetOrigin( l_Origin );

    l_VIEPixel.SetSize(l_NbBand);
    l_SOLPixel.SetSize(l_NbBand);

    l_VIEPixel.SetElement(0,l_VIEValue[0]);
    l_VIEPixel.SetElement(1,l_VIEValue[1]);
    l_SOLPixel.SetElement(0,l_SOLValue[0]);
    l_SOLPixel.SetElement(1,l_SOLValue[1]);


    l_VIEConstantFilter->SetConstantValue(l_VIEPixel);
    l_SOLConstantFilter->SetConstantValue(l_SOLPixel);

    // --------------------------------------------
    // Constant cloud altitude
    // --------------------------------------------
    CLAImageType::SizeValueType l_CLASize[2];

    l_CLASize[0] = 375;
    l_CLASize[1] = 334;
    l_CLAConstantFilter->SetSize(l_CLASize);

    CLAImageType::SpacingValueType l_CLASpacing[2] = {96., -96.};
    l_CLAConstantFilter->SetSignedSpacing( l_CLASpacing );


    CLAImageType::PointValueType l_CLAOrigin[2];
    l_CLAOrigin[0] = l_L2CLDSubOrigin[0];
    l_CLAOrigin[1] = l_L2CLDSubOrigin[1];
    l_CLAConstantFilter->SetOrigin( l_CLAOrigin );

    l_CLAConstantFilter->SetConstantValue(l_CLAValue);


    // DTM Reader
    l_DTMReader->Update();
    l_DTMReader->GenerateOutputInformation();

    std::cout <<  "DTM = " << l_DTMReader << std::endl;

    std::cout <<  "====Spacing l_L2CLDSubReader = " <<l_L2CLDSubReader->GetOutput()->GetSpacing()<<std::endl;
    std::cout <<  "====Spacing l_DTMReader = " <<l_DTMReader->GetOutput()->GetSpacing()<<std::endl;

    std::cout <<  "====SignedSpacing l_L2CLDSubReader = " <<l_L2CLDSubReader->GetOutput()->GetSignedSpacing()<<std::endl;
    std::cout <<  "====SignedSpacing l_DTMReader = " <<l_DTMReader->GetOutput()->GetSignedSpacing()<<std::endl;




    // ----------------------------------
    // Set input images
    // ----------------------------------
    l_Filter->SetL2CLDSubInput(l_L2CLDSubReader->GetOutput());
    l_Filter->SetDTMInput(l_DTMReader->GetOutput());
    l_Filter->SetIPVIEInput(l_VIEConstantFilter->GetOutput());
    l_Filter->SetIPSOLInput(l_SOLConstantFilter->GetOutput());
    l_Filter->SetIPCLAInput(l_CLAConstantFilter->GetOutput());

    l_Filter->SetI3D( & l_I3D);

    l_Filter->SetVIEHRef(l_VIEHRef);
    l_Filter->SetSOLHRef(l_SOLHRef);

    // ----------------------------------
    // Generate image of labels
    // ----------------------------------
    l_LabelFilter->SetInput(l_L2CLDSubReader->GetOutput());

    l_Filter->SetL2CLDSubLabelInput(l_LabelFilter->GetOutput());
    l_Filter->Update();

    std::fstream file(l_OutputFileName, std::ios::out);
    file<<l_Filter;
    file.close();

    return EXIT_SUCCESS;
}
