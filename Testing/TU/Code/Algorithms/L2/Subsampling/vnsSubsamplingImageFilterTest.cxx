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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsSubsamplingImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsDataManagerTestingLibrary.h"
#include "itkTimeProbe.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "vnsTestingMacro.h"

int
vnsSubsamplingImageFilterTest(int argc, char * argv[])
{
    if (argc != 18)
    {
      std::cout<<argc<<std::endl;
        std::cerr << argv[0]
                << " <L1 TOA input filename> <L1 SAT input filename> "
                << "<TOAS filename> <TOA filename> <EDS filename> <EDG filename> <SATS filename> <SAT filename>"
                << "<L2SatThresholdValue> <IPSatThresholdValue> <L1 no data value> <L2 no data value> <L1L2ratio> <L2L2coarseration> <TOA coef>" << std::endl;
        return EXIT_FAILURE;
    }

    vns::DMTesting::LOAD_TU_CONTEXT_WITH_INSTALL_MAJA_DIR(argv[16],argv[17],vns::LoggerBase::INFO);

    // Inputs
    const char * l1TOAFileName = argv[1];
    const char * l1SATFileName = argv[2];
    // Outputs
    const char * IPTOAFileName = argv[3];
    const char * IPTOASubFileName = argv[4];
    const char * l2EDGFileName = argv[5];
    const char * IPEDSFileName = argv[6];
    const char * l2SATFileName = argv[7];
    const char * IPSATFileName = argv[8];
    // Parameters
    double l_L2SatThresholdValue = atof(argv[9]);
    double l_IPSatThresholdValue = atof(argv[10]);
    double l_L1NoData = atof(argv[11]);
    double l_L2NoData = atof(argv[12]);

    /** Pixel typedefs */
    typedef double PixelType;
    typedef unsigned char MaskPixelType;

    /** Image typedefs */
    typedef otb::VectorImage<PixelType> VectorImageType;
    typedef otb::VectorImage<MaskPixelType> VectorMaskImageType;
    typedef otb::VectorImage<int>             IntVectorImageType;
    typedef otb::Image<MaskPixelType> MaskImageType;
    typedef otb::ImageFileReader<VectorImageType> ReaderType;
    typedef otb::ImageFileReader<VectorMaskImageType> ReaderVectorMaskReaderType;
    typedef otb::ImageFileWriter<MaskImageType> WriterMaskType;
    typedef otb::ImageFileWriter<VectorMaskImageType> WriterVectorMaskType;
    typedef otb::ImageFileWriter<IntVectorImageType> WriterType;
    typedef otb::MultiplyByScalarImageFilter<VectorImageType, VectorImageType> MultiplierType;

    typedef vns::SubsamplingImageFilter<VectorImageType, VectorMaskImageType, MaskImageType, VectorImageType, VectorMaskImageType, MaskImageType> FilterType;
    typedef FilterType::TempImagePixelType TempImagePixelType;

    typedef vns::tu::Functor::myFloorFunctor<VectorImageType::PixelType, VectorImageType::PixelType> FunctorType;
    typedef itk::UnaryFunctorImageFilter<VectorImageType, VectorImageType, FunctorType>     FloorerType;

    typedef otb::MultiplyByScalarImageFilter<VectorImageType, IntVectorImageType>    VectorIntMultiplierType;

    FilterType::Pointer filter = FilterType::New();
    MultiplierType::Pointer multi = MultiplierType::New();

    ReaderType::Pointer l1TOAreader = ReaderType::New();
    ReaderVectorMaskReaderType::Pointer l1SATreader = ReaderVectorMaskReaderType::New();
 
    WriterType::Pointer IPTOASubwriter = WriterType::New();
    WriterType::Pointer IPTOAwriter = WriterType::New();
    WriterMaskType::Pointer IPEDSwriter = WriterMaskType::New();
    WriterMaskType::Pointer l2EDGwriter = WriterMaskType::New();
    WriterVectorMaskType::Pointer l2SATwriter = WriterVectorMaskType::New();
    WriterVectorMaskType::Pointer IPSATwriter = WriterVectorMaskType::New();
 
    l1TOAreader->SetFileName(l1TOAFileName);
    l1SATreader->SetFileName(l1SATFileName);

    multi->SetInput( l1TOAreader->GetOutput() );
    multi->SetCoef( atof(argv[15]) );

    IPTOASubwriter->SetFileName(IPTOASubFileName);
    IPTOAwriter->SetFileName(IPTOAFileName);
    IPEDSwriter->SetFileName(IPEDSFileName);
    l2EDGwriter->SetFileName(l2EDGFileName);
    l2SATwriter->SetFileName(l2SATFileName);
    IPSATwriter->SetFileName(IPSATFileName);

    // Set inputs
    filter->SetL1TOAInput(multi->GetOutput());
    filter->SetL1SATInput(l1SATreader->GetOutput());

    // Set filters parameters
    filter->SetEDGOneBandThresholdValue(l_L1NoData);
    filter->SetEDGOneBandEqualValue(itk::NumericTraits<MaskImageType::PixelType>::max());
    filter->SetEDGOneBandOutsideValue(0);

    // Set ratios
    filter->SetL1L2Ratio( atoi(argv[13]) );
    filter->SetL2L2CoarseRatio( atoi(argv[14]));

    filter->SetSatThresholdInsideValue(0);
    filter->SetSatThresholdOutputValue(1);
    filter->SetL2SatThresholdValue(static_cast<TempImagePixelType>(l_L2SatThresholdValue));
    filter->SetIPSatThresholdValue(static_cast<TempImagePixelType>(l_IPSatThresholdValue));
 
    filter->SetEDGThresholdValue(0.);
    filter->SetEDGThresholdOutsideValue(1.);
    filter->SetTOAGreaterThresholdMinInputValue(0);
    filter->SetTOAGreaterThresholdOutputValue(l_L2NoData);
    

    itk::TimeProbe clock;
    clock.Start();
    filter->UpdateWithCaching();

    clock.Stop();
    std::cout<<"-------------------------------- Filter Time : "<<clock.GetTotal()<<std::endl;
    clock.Start();

     // ----------------------------------------
     // Write EDG and SAT masks at L2 resolution
     // ----------------------------------------

     l2SATwriter->SetInput(filter->GetL2SATOutput());
     l2SATwriter->Update();
    
//     l2EDGwriter->SetInput(filter->GetL2EDGOutput());
//     l2EDGwriter->Update();

     // ----------------------------------------------------
     // Write TOA reflectance images at L2 coarse resolution
     // ----------------------------------------------------

     // Cast the output
     FloorerType::Pointer l_IPTOAfloorer = FloorerType::New();
     FloorerType::Pointer l_IPTOASubfloorer = FloorerType::New();

     l_IPTOAfloorer->SetInput(filter->GetIPTOAOutput());
     l_IPTOAfloorer->SetNumberOfThreads(1);

     l_IPTOASubfloorer->SetInput(filter->GetIPTOASubOutput());
     l_IPTOASubfloorer->SetNumberOfThreads(1);

     // Multiply the output
     VectorIntMultiplierType::Pointer l_IPTOAMultiOut = VectorIntMultiplierType::New();
     VectorIntMultiplierType::Pointer l_IPTOASubMultiOut = VectorIntMultiplierType::New();

     l_IPTOAMultiOut->SetInput(l_IPTOAfloorer->GetOutput());
     l_IPTOAMultiOut->SetCoef(1000);
    
     IPTOAwriter->SetInput(l_IPTOAMultiOut->GetOutput());
     IPTOAwriter->Update();

     l_IPTOASubMultiOut->SetInput(l_IPTOASubfloorer->GetOutput());
     l_IPTOASubMultiOut->SetCoef(1000);

     IPTOASubwriter->SetInput(l_IPTOASubMultiOut->GetOutput());
     IPTOASubwriter->Update();

     // -----------------------------------------------
     // Write EDG and SAT masks at L2 coarse resolution
     // -----------------------------------------------

     IPEDSwriter->SetInput(filter->GetIPEDGOutput());
     IPEDSwriter->Update();
    
     IPSATwriter->SetInput(filter->GetIPSATOutput());
     IPSATwriter->Update();

    clock.Stop();
    std::cout<<"-------------------------------- Filter finish in : "<<clock.GetTotal()<<std::endl;

    return EXIT_SUCCESS;
}
