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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 juil. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsExpandImageFilterTest.cxx 2406 2011-01-19 15:38:23Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include "itkMacro.h"

#include "itkExpandImageFilter.h"
#include "itkVectorExpandImageFilter.h"
#include "vnsExpandImageFilter.h"
#include "vnsExpandVectorImageFilter.h"
#include "itkMultiThreader.h"
#include "vnsRawExpandImageFilter.h"
#include "vnsRawExpandVectorImageFilter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkVectorImage.h"


int
vnsMainGlobalTestAllResampleImageFiltersTest(int argc, char * argv[])
{
    if (argc < 4 )
    {
        std::cout << "inputimage outputimage nbthreads method"<<std::endl;
        std::cout << "method :  itk_expand_vi                   Real factor      ; itk::VectorExpandImageFilter with itk::Vector<double, 2>  " <<std::endl;
        std::cout << "                                          -> parameters :   factor"<<std::endl;
        std::cout << "          itk_expand_i                    Integer Factor   ; itk::ExpandImageFilter with  itk::Image<double>" <<std::endl;
        std::cout << "                                          -> parameters :   factor"<<std::endl;
        std::cout << "          maccs_expand                    Integer factor   ; vns::ExpandImageFilter with otb::VectorImage<double>  " <<std::endl;
        std::cout << "                                          -> parameters :   factor"<<std::endl;
        std::cout << "          maccs_expand_vi                 Integer factor   ; vns::ExpandVectorImageFilter with otb::VectorImage<double>  " <<std::endl;
        std::cout << "                                          -> parameters :   factor"<<std::endl;
        std::cout << "          maccs_real_expand_i             Real factor   ; vns::RawExpandImageFilter withitk::Image<double>  " <<std::endl;
        std::cout << "                                          -> parameters :   factor"<<std::endl;
        std::cout << "          maccs_real_expand_vi            Real factor   ; vns::RawExpandVectorImageFilter with otb::VectorImage<double>  " <<std::endl;
        std::cout << "                                          -> parameters :   factor"<<std::endl;
    }
    unsigned int cpt(1);
    const char* inputFilename = argv[cpt++];
    std::string outputFilename = argv[cpt++];
    const unsigned int nbthreads = atoi(argv[cpt++]);
    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(nbthreads);
    std::string method(argv[cpt++]);

    typedef otb::VectorImage<unsigned short> VectorImageType;
    typedef otb::ImageFileReader<VectorImageType> VIReaderType;
    typedef otb::ImageFileWriter<VectorImageType> VIWriterType;

    typedef otb::Image<unsigned short> ImageType;
    typedef otb::ImageFileReader<ImageType> IReaderType;
    typedef otb::ImageFileWriter<ImageType> IWriterType;

    // Instantiating object
    if (method == "maccs_expand")
    {
        std::cout << " Star MACCS Expand... (old version)" << std::endl;
        const unsigned int factor = (atoi(argv[cpt++]));
        std::cout << " Factor : " << factor << std::endl;
        typedef vns::ExpandImageFilter<ImageType, ImageType> ExpandFilterType;
        IReaderType::Pointer reader = IReaderType::New();
        IWriterType::Pointer writer = IWriterType::New();
        ExpandFilterType::Pointer filter = ExpandFilterType::New();
        reader->SetFileName(inputFilename);
        reader->GenerateOutputInformation();
        filter->SetInput(reader->GetOutput());
        filter->SetExpandFactors(factor);
        writer->SetInput(filter->GetOutput());
        writer->SetFileName(outputFilename);
        writer->Update();
    }
    else if (method == "maccs_expand_vi")
    {
        std::cout << " Star MACCS Expand VectorImage... (old version)" << std::endl;
        const unsigned int factor = (atoi(argv[cpt++]));
        std::cout << " Factor : " << factor << std::endl;
        typedef vns::ExpandVectorImageFilter<VectorImageType, VectorImageType> VNSExpandFilterType;
        VIReaderType::Pointer reader = VIReaderType::New();
        VIWriterType::Pointer writer = VIWriterType::New();
        VNSExpandFilterType::Pointer filter = VNSExpandFilterType::New();
        reader->SetFileName(inputFilename);
        reader->GenerateOutputInformation();
        filter->SetInput(reader->GetOutput());
        filter->SetExpandFactors(factor);
        writer->SetInput(filter->GetOutput());
        writer->SetFileName(outputFilename);
        writer->Update();
    }
    // Instantiating object
    else if (method == "maccs_real_expand_i")
    {
        std::cout << " Star MACCS Raw Expand (real factor)..." << std::endl;
        const float factor(atof(argv[cpt++]));
        std::cout << " Factor : " << factor << std::endl;
        typedef itk::Image<unsigned short, 2> RawImageType;
        typedef vns::RawExpandImageFilter<RawImageType, RawImageType> VNSRawExpandFilterType;
        typedef otb::ImageFileReader<RawImageType> RReaderType;
        typedef otb::ImageFileWriter<RawImageType> RWriterType;

        RReaderType::Pointer reader = RReaderType::New();
        RWriterType::Pointer writer = RWriterType::New();

        VNSRawExpandFilterType::Pointer filter = VNSRawExpandFilterType::New();
        reader->SetFileName(inputFilename);
        reader->GenerateOutputInformation();
        filter->SetInput(reader->GetOutput());
        filter->SetExpandFactors(factor);
        writer->SetInput(filter->GetOutput());
        writer->SetFileName(outputFilename);
        writer->Update();
    }
    // Instantiating object
    else if (method == "maccs_real_expand_vi")
    {
        std::cout << " Star MACCS Raw VectorImage Expand (real factor)..." << std::endl;
        const float factor(atof(argv[cpt++]));
        std::cout << " Factor : " << factor << std::endl;
        typedef otb::VectorImage<unsigned short> RawImageType;
        typedef vns::RawExpandVectorImageFilter<RawImageType, RawImageType> VNSRawExpandFilterType;
        typedef otb::ImageFileReader<RawImageType> RReaderType;
        typedef otb::ImageFileWriter<RawImageType> RWriterType;

        RReaderType::Pointer wreader = RReaderType::New();
        RWriterType::Pointer wwriter = RWriterType::New();

        VNSRawExpandFilterType::Pointer filter = VNSRawExpandFilterType::New();
        wreader->SetFileName(inputFilename);
        wreader->GenerateOutputInformation();
        filter->SetInput(wreader->GetOutput());
        filter->SetExpandFactors(factor);
        wwriter->SetInput(filter->GetOutput());
        wwriter->SetFileName(outputFilename);
        wwriter->Update();
    }
    else if (method == "itk_expand_i")
    {
        std::cout << " Star ITK Expand..." << std::endl;
        const unsigned int factor = (atoi(argv[cpt++]));
        std::cout << " Factor : " << factor << std::endl;
        typedef itk::Image<unsigned short, 2> ImageType;
        typedef otb::ImageFileReader<ImageType> ReaderType;
        typedef otb::ImageFileWriter<ImageType> WriterType;
        typedef itk::ExpandImageFilter<ImageType, ImageType> ITKExpandFilterType;

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();
        ITKExpandFilterType::Pointer filter = ITKExpandFilterType::New();
        reader->SetFileName(inputFilename);
        filter->SetInput(reader->GetOutput());
        filter->SetExpandFactors(factor);
        writer->SetInput(filter->GetOutput());
        writer->SetFileName(outputFilename);
        writer->Update();
    }
    else if (method == "itk_expand_vi")
    {
        std::cout << " Star ITK VectorExpand..." << std::endl;
        const float factor(atof(argv[cpt++]));
        std::cout << " Factor : " << factor << std::endl;
        typedef itk::Vector<unsigned short, 2> VectorType;
        typedef itk::Image<VectorType, 2> ImageVectorType;
        typedef otb::ImageFileReader<ImageVectorType> IVReaderType;
        typedef otb::ImageFileWriter<ImageVectorType> IVWriterType;
        typedef itk::VectorExpandImageFilter<ImageVectorType, ImageVectorType> ITKVectorEpandFilterType;

        IVReaderType::Pointer reader = IVReaderType::New();
        IVWriterType::Pointer writer = IVWriterType::New();
        ITKVectorEpandFilterType::Pointer filter = ITKVectorEpandFilterType::New();

        reader->SetFileName(inputFilename);
        reader->GenerateOutputInformation();
        filter->SetInput(reader->GetOutput());

//        typedef itk::VectorLinearInterpolateImageFunction<ImageType,double> InterpolatorType;
//        typedef itk::VectorNearestNeighborInterpolateImageFunction<ImageType,double> InterpolatorType;
//        InterpolatorType::Pointer interpolator = InterpolatorType::New();

//        expander->SetInterpolator( interpolator );

        filter->SetExpandFactors(factor);
        float factors[2] =
        {   factor,factor};
        filter->SetExpandFactors(factors);
        writer->SetInput(filter->GetOutput());
        writer->SetFileName(outputFilename);
        writer->Update();
    }
    return EXIT_SUCCESS;

}
