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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 12 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsShadowMaskProjectionImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsUtilities.h"
#include "itkConnectedComponentImageFilter.h"

#include "vnsConstantVectorImageSource.h"
#include "vnsConstantImageSource.h"

int
vnsShadowMaskProjectionImageFilterTest(int argc, char * argv[])
{
    if (argc != 11)
    {
        std::cerr << argv[0] << " <L2CLDSub mask filename> <DTM filename> "
                "<IPCLDShad output filename> <VIEHRef> <SOLHRef>"
                "<VIE zenith angle> <VIE azimuth angle> "
                "<SOL zenith angle> <SOL azimuth angle> <default altitude>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_L2CLDSubFileName = argv[1];
    const char * l_DTMFileName = argv[2];
    const char * l_OutputFileName = argv[3];

    double l_VIEHRef = atof(argv[4]);
    // Not used with Formosat : l_SOLHref = -cos(theta_s) - see line 101
    double l_SOLHRef = atof(argv[5]);

    const double l_VIEZenithRad(vns::Utilities::AsDouble(argv[6]) * otb::CONST_PI_180);
    const double l_VIEAzimuthRad(vns::Utilities::AsDouble(argv[7]) * otb::CONST_PI_180);
    const double l_SOLZenithRad(vns::Utilities::AsDouble(argv[8]) * otb::CONST_PI_180);
    const double l_SOLAzimuthRad(vns::Utilities::AsDouble(argv[9]) * otb::CONST_PI_180);

    const double l_CLAValue(vns::Utilities::AsDouble(argv[10]));

    // x = h*tg(theta_v)cos(phi_v) - lin -> lat
    // y = h*tg(theta_v)sin(phi_v) - col -> long
    std::vector<double> l_VIEValue(2);
    l_VIEValue[0] = l_VIEHRef * vcl_tan(l_VIEZenithRad) * vcl_sin(l_VIEAzimuthRad);
    l_VIEValue[1] = l_VIEHRef * vcl_tan(l_VIEZenithRad) * vcl_cos(l_VIEAzimuthRad);

    // x = -sin(theta_s)cos(phi_s)
    // y = -sin(theta_s)sin(phi_s)
    // z = -cos(theta_s)
    // downward direction (opposite sign)

    // Grids are first generated in pixels and then converted in meters dividing by the spacing
    // in order to fit with venus grids
    // see lines 203:206
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
    typedef unsigned short OutputPixelType;

    /** Image typedefs */
    typedef otb::Image<InputMaskPixelType, Dimension> InputMaskType;
    typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
    typedef otb::Image<DTMPixelType, Dimension> DTMImageType;
    typedef otb::VectorImage<GRIDPixelType, Dimension> GRIDImageType;
    typedef otb::Image<CLAPixelType, Dimension> CLAImageType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputMaskType> InputMaskReaderType;
    typedef otb::ImageFileReader<DTMImageType> DTMReaderType;
    typedef otb::ImageFileReader<GRIDImageType> GRIDReaderType;
    typedef otb::ImageFileReader<CLAImageType> CLAReaderType;

    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef vns::ShadowMaskProjectionImageFilter<InputMaskType, LabelImageType, DTMImageType, GRIDImageType, CLAImageType, OutputImageType> FilterType;

    // Labelize filter type
    typedef itk::ConnectedComponentImageFilter<InputMaskType, LabelImageType> LabelizeFilterType;

    // Constant Image filter type
    typedef vns::ConstantVectorImageSource<GRIDImageType> ConstantVectorFilterType;
    typedef vns::ConstantImageSource<CLAImageType> ConstantFilterType;

    GRIDImageType::PixelType l_VIEPixel;
    GRIDImageType::PixelType l_SOLPixel;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    LabelizeFilterType::Pointer l_LabelFilter = LabelizeFilterType::New();
    ConstantVectorFilterType::Pointer l_VIEConstantFilter = ConstantVectorFilterType::New();
    ConstantVectorFilterType::Pointer l_SOLConstantFilter = ConstantVectorFilterType::New();
    ConstantFilterType::Pointer l_CLAConstantFilter = ConstantFilterType::New();
    ConstantFilterType::Pointer l_SOLHRefConstantFilter = ConstantFilterType::New();
    InputMaskReaderType::Pointer l_L2CLDSubReader = InputMaskReaderType::New();
    CLAReaderType::Pointer l_IPCLAReader = CLAReaderType::New();
    DTMReaderType::Pointer l_DTMReader = DTMReaderType::New();
    WriterType::Pointer l_IPCLDShadWriter = WriterType::New();

    // --------
    // Readers
    // --------
    l_L2CLDSubReader->SetFileName(l_L2CLDSubFileName);
    l_DTMReader->SetFileName(l_DTMFileName);

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

    GRIDImageType::SpacingValueType l_Spacing[2] =
        { 96., -96. };
    l_VIEConstantFilter->SetSpacing(l_Spacing);
    l_SOLConstantFilter->SetSpacing(l_Spacing);

    GRIDImageType::PointValueType l_Origin[2];
    l_Origin[0] = l_L2CLDSubOrigin[0];
    l_Origin[1] = l_L2CLDSubOrigin[1];
    l_VIEConstantFilter->SetOrigin(l_Origin);
    l_SOLConstantFilter->SetOrigin(l_Origin);

    l_VIEPixel.SetSize(l_NbBand);
    l_SOLPixel.SetSize(l_NbBand);
    /*    for (unsigned int i=0 ; i<l_NbBand ; i++)
     {
     l_VIEPixel.SetElement(i,l_VIEValue[i]/l_Spacing[i]);
     l_SOLPixel.SetElement(i,l_SOLValue[i]/l_Spacing[i]);
     }*/
    l_VIEPixel.SetElement(0, l_VIEValue[0] / l_Spacing[0]);
    l_VIEPixel.SetElement(1, -l_VIEValue[1] / l_Spacing[1]);
    l_SOLPixel.SetElement(0, l_SOLValue[0] / l_Spacing[0]);
    l_SOLPixel.SetElement(1, -l_SOLValue[1] / l_Spacing[1]);

    l_VIEConstantFilter->SetConstantValue(l_VIEPixel);
    l_SOLConstantFilter->SetConstantValue(l_SOLPixel);

//    l_VIEConstantFilter->Update();
//    l_SOLConstantFilter->Update();

    /*        typedef otb::ImageFileWriter<GRIDImageType>           GridWriterType;
     GridWriterType::Pointer l_GridWriter = GridWriterType::New();

     l_GridWriter->SetFileName("/MILO/VENUS/caroline/Test/SOLConstant.tif");
     l_GridWriter->SetInput(l_SOLConstantFilter->GetOutput());
     l_GridWriter->Update();*/

    // --------------------------------------------
    // Constant cloud altitude
    // --------------------------------------------
    CLAImageType::SizeValueType l_CLASize[2];

    l_CLASize[0] = 375;
    l_CLASize[1] = 334;
    l_CLAConstantFilter->SetSize(l_CLASize);

    CLAImageType::SpacingValueType l_CLASpacing[2] =
        { 96., -96. };
    l_CLAConstantFilter->SetSpacing(l_CLASpacing);

    CLAImageType::PointValueType l_CLAOrigin[2];
    l_CLAOrigin[0] = l_L2CLDSubOrigin[0];
    l_CLAOrigin[1] = l_L2CLDSubOrigin[1];
    l_CLAConstantFilter->SetOrigin(l_CLAOrigin);

    l_CLAConstantFilter->SetConstantValue(l_CLAValue);

    // --------------------------------------------
    // Constant solar reference altitude
    // --------------------------------------------
    l_SOLHRefConstantFilter->SetSize(l_CLASize);
    l_SOLHRefConstantFilter->SetSpacing(l_CLASpacing);
    l_SOLHRefConstantFilter->SetOrigin(l_CLAOrigin);

    l_SOLHRefConstantFilter->SetConstantValue(l_SOLHRef);

    // DTM Reader
    l_DTMReader->Update();
    l_DTMReader->GenerateOutputInformation();

    // ----------------------------------
    // Set input images
    // ----------------------------------
    l_Filter->SetL2CLDSubInput(l_L2CLDSubReader->GetOutput());
    l_Filter->SetDTMInput(l_DTMReader->GetOutput());
    l_Filter->SetIPVIEInput(l_VIEConstantFilter->GetOutput());
    l_Filter->SetIPSOLInput(l_SOLConstantFilter->GetOutput());
    l_Filter->SetIPCLAInput(l_CLAConstantFilter->GetOutput());
    l_Filter->SetIPSOLHRefInput(l_SOLHRefConstantFilter->GetOutput());

    l_Filter->SetVIEHRef(l_VIEHRef);

    // ----------------------------------
    // Generate image of labels
    // ----------------------------------
    l_LabelFilter->SetInput(l_L2CLDSubReader->GetOutput());

//    typedef otb::ImageFileWriter<LabelImageType>           LabelWriterType;
//    LabelWriterType::Pointer l_LabelWriter = LabelWriterType::New();

//    l_LabelWriter->SetFileName("/MILO/VENUS/caroline/Test/L2CLDSubLabel.tif");
//    l_LabelWriter->SetInput(l_LabelFilter->GetOutput());
//    l_LabelWriter->Update();

//    std::cout<<l_LabelFilter<<std::endl;
//    std::cout<<"Nombre de label " << l_LabelFilter->GetObjectCount()<<std::endl;

    l_Filter->SetL2CLDSubLabelInput(l_LabelFilter->GetOutput());

    // -------
    // Writer
    // -------
    l_IPCLDShadWriter->SetFileName(l_OutputFileName);
    l_IPCLDShadWriter->SetInput(l_Filter->GetIPCLDShadOutput());

    l_IPCLDShadWriter->Update();

    return EXIT_SUCCESS;
}
