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

#include "itkMultiThreader.h"


#include "vnsBCOSubSampleVectorImageFilter.h"
#include "vnsLinearSubSampleVectorImageFilter.h"
#include "vnsBCOSubSampleImageFilter.h"
#include "vnsLinearSubSampleImageFilter.h"
#include "vnsUtilities.h"
#include "itkTimeProbe.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int
vnsResampleBenchmarkImageFiltersTest(int argc, char * argv[])
{
    if (argc != 7)
    {
        std::cout << "inputimage outputimage nbthreads factor (>0) [method]" << std::endl;
        std::cout << "method :  linear                          vns::LinearSubSampleVectorImageFilter with otb::VectorImage<double, 2>  "
                << std::endl;
        std::cout << "          bco                             vns::BCOSubSampleVectorImageFilter with  otb::VectorImage<double>"
                << std::endl;
        std::cout << "          maccs_expand_vi                 vns::ExpandVectorImageFilter with otb::VectorImage<double>  " << std::endl;
        return EXIT_FAILURE;
    }
    unsigned int cpt(1);
    const std::string inputFilename = argv[cpt++];
    const std::string outputFilename = argv[cpt++];
    const unsigned int nbthreads = atoi(argv[cpt++]);
    const std::string method(argv[cpt++]);
    const std::string image_type(argv[cpt++]);
    const double factor(atof(argv[cpt++]));

    std::cout << "Parameters: " << std::endl;
    std::cout << " - inputFilename: " << inputFilename << std::endl;
    std::cout << " - outputFilename: " << outputFilename << std::endl;
    std::cout << " - nbthreads: " << nbthreads << std::endl;
    std::cout << " - method: '" << method << "'" << std::endl;
    std::cout << " - image_type: '" << image_type << "'" << std::endl;
    std::cout << " - factor: " << factor << std::endl;

    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(nbthreads);
    itk::TimeProbe l_clock;
    l_clock.Start();
    if (image_type == "image")
    {
        typedef otb::Image<float> ImageType;
        typedef otb::ImageFileReader<ImageType> VIReaderType;
        typedef otb::ImageFileWriter<ImageType> VIWriterType;

        VIReaderType::Pointer reader = VIReaderType::New();
        VIWriterType::Pointer writer = VIWriterType::New();
        reader->SetFileName(inputFilename);
        reader->GenerateOutputInformation();
        writer->SetFileName(outputFilename);

        // Instantiating object
        if (method == "bco")
        {
            std::cout << " Star MACCS BCOSubSampleVectorImageFilter..." << std::endl;
            typedef vns::BCOSubSampleImageFilter<ImageType, ImageType> BCOSubSampleFilterType;
            BCOSubSampleFilterType::Pointer filter = BCOSubSampleFilterType::New();
            filter->SetInput(reader->GetOutput());
            filter->SetSubsampleFactor(static_cast<unsigned int>(factor));
            std::cout << " BCORadius computed: " << filter->GetBCORadius() << std::endl;
            writer->SetInput(filter->GetOutput());
            writer->Update();

        }
        else if (method == "linear")
        {
            std::cout << " Star MACCS LinearSubSampleVectorImageFilter..." << std::endl;
            typedef vns::LinearSubSampleImageFilter<ImageType, ImageType> LinearSubSampleFilterType;
            LinearSubSampleFilterType::Pointer filter = LinearSubSampleFilterType::New();
            filter->SetInput(reader->GetOutput());
            filter->SetSubsampleFactor(static_cast<unsigned int>(factor));
            writer->SetInput(filter->GetOutput());
            writer->Update();
        }
        else
        {
            std::cout << "The value for the parameter method <" << method << "> is bad!" << std::endl;
            return EXIT_FAILURE;
        }
    } // if( image_type == "vector-image" )
    else if (image_type == "vector-image")
    {
        typedef otb::VectorImage<float> ImageType;
        typedef otb::ImageFileReader<ImageType> VIReaderType;
        typedef otb::ImageFileWriter<ImageType> VIWriterType;

        VIReaderType::Pointer reader = VIReaderType::New();
        VIWriterType::Pointer writer = VIWriterType::New();
        reader->SetFileName(inputFilename);
        reader->GenerateOutputInformation();
        writer->SetFileName(outputFilename);

        // Instantiating object
        if (method == "bco")
        {
            std::cout << " Star MACCS BCOSubSampleVectorImageFilter..." << std::endl;
            typedef vns::BCOSubSampleVectorImageFilter<ImageType, ImageType> BCOSubSampleFilterType;
            BCOSubSampleFilterType::Pointer filter = BCOSubSampleFilterType::New();
            filter->SetInput(reader->GetOutput());
            filter->SetSubsampleFactor(static_cast<unsigned int>(factor));
            std::cout << " BCORadius computed: " << filter->GetBCORadius() << std::endl;
            writer->SetInput(filter->GetOutput());
            writer->Update();

        }
        else if (method == "linear")
        {
            std::cout << " Star MACCS LinearSubSampleVectorImageFilter..." << std::endl;
            typedef vns::LinearSubSampleVectorImageFilter<ImageType, ImageType> LinearSubSampleFilterType;
            LinearSubSampleFilterType::Pointer filter = LinearSubSampleFilterType::New();
            filter->SetInput(reader->GetOutput());
            filter->SetSubsampleFactor(static_cast<unsigned int>(factor));
            writer->SetInput(filter->GetOutput());
            writer->Update();
        }
        else
        {
            std::cout << "The value for the parameter method <" << method << "> is bad!" << std::endl;
            return EXIT_FAILURE;
        }
    } // if( image_type == "vector-image" )
    else
    {
        std::cout << "The value for the parameter image_type <" << image_type << "> is bad!" << std::endl;
        return EXIT_FAILURE;
    }

        l_clock.Stop();

    std::cout << " -----   Time is " << l_clock.GetTotal() << " seconds   (" << vns::Utilities::GetStringFormatedTime(l_clock.GetTotal())
            << ")." << std::endl;

    // Instantiating object
#if 0
    else if (method == "maccs_expand_vi")
    {
        std::cout << " Star MACCS Expand... (old version)" << std::endl;
        typedef vns::ExpandVectorImageFilter<VectorImageType, VectorImageType> VNSExpandFilterType;
        VNSExpandFilterType::Pointer filter = VNSExpandFilterType::New();
        filter->SetInput(reader->GetOutput());
        filter->SetExpandFactors(static_cast<unsigned int>(factor));
        writer->SetInput(filter->GetOutput());
        writer->Update();
    }
    else if (method == "maccs_real_expand_i")
    {
        std::cout << " Star MACCS Raw Expand (real factor)..." << std::endl;
        typedef itk::Image<double, 2> RawImageType;
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
        typedef otb::VectorImage<double> RawImageType;
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
        typedef itk::Image<double, 2> ImageType;
        typedef otb::ImageFileReader<ImageType> ReaderType;
        typedef otb::ImageFileWriter<ImageType> WriterType;
        typedef itk::ExpandImageFilter<ImageType, ImageType> ITKExpandFilterType;

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();
        ITKExpandFilterType::Pointer filter = ITKExpandFilterType::New();
        reader->SetFileName(inputFilename);
        filter->SetInput(reader->GetOutput());
        filter->SetExpandFactors(static_cast<unsigned int>(factor));
        writer->SetInput(filter->GetOutput());
        writer->SetFileName(outputFilename);
        writer->Update();
    }
    else if (method == "itk_expand_vi")
    {
        std::cout << " Star ITK VectorExpand..." << std::endl;
        typedef itk::Vector<double, 2> VectorType;
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
#endif
    return EXIT_SUCCESS;

}
