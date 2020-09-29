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
// (C) 2015, Julien Michel (CNES) <julien.michel@cnes.fr>

// Code ResamplerBuilder

/////////////////////////////////////////////////////////////////////////////////////////////////
// Test code
#include <string>
#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkMultiThreader.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsUtilities.h"
#include "itkTimeProbe.h"
#include "itkMeanImageFilter.h"
#include "vnsResamplerHelper.h"
#include "otbCommandLineArgumentParser.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "vnsLinearInterpolateImageFunction.h"
#include "vnsFastLinearInterpolateImageFunction.h"
#include "vnsLoggers.h"
#include "vnsLoggerBase.h"
#include "vnsPadAndResampleImageFilter.h"
#include "itkMultiThreader.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbImageList.h"

template<typename ImageType, typename ReaderType, typename ResamplerType>
    void
    setting_resampler(typename otb::CommandLineArgumentParseResult::Pointer parseResult, const ImageType* input_image,
            typename ResamplerType::Pointer resampler)
    {
        typedef itk::LinearInterpolateImageFunction<ImageType, double> LinearInterpolatorType;
        typedef otb::BCOInterpolateImageFunction<ImageType, double> BCOInterpolatorType;
        typedef vns::LinearInterpolateImageFunction<ImageType, double> MACCSLinearWithRadiusInterpolatorType;
        typedef vns::FastLinearInterpolateImageFunction<ImageType, double> MACCSFastLinearInterpolatorType;

        typedef itk::ImageBase<2>::SpacingType SpacingType;
        typedef itk::ImageBase<2>::PointType OriginType;
        typename ImageType::SizeType inSize, outSize;
        typename ImageType::PointType inOrigin, outOrigin;
        typename ImageType::SpacingType inSpacing, outSpacing;
        typedef vns::ResamplerHelper ImageResamplerBuilderType;

        const std::string inputFilename = parseResult->GetInputImage();

        typename ReaderType::Pointer reader = ReaderType::New();

        reader->SetFileName(inputFilename.c_str());
        reader->UpdateOutputInformation();
        bool grid(false);

        inOrigin = input_image /*reader->GetOutput()*/->GetOrigin();
        inSpacing = input_image /*reader->GetOutput()*/->GetSpacing();
        inSize = input_image /*reader->GetOutput()*/->GetLargestPossibleRegion().GetSize();

        typename ImageResamplerBuilderType::InterpolatorType l_InterpolValue = ImageResamplerBuilderType::NOT_INTERPOLATOR_INITIALIZED;
        std::string method("linear");
        if (parseResult->IsOptionPresent("--Interpolator") == true)
        {
            method = parseResult->GetParameterString("--Interpolator");
        }
        if (method == "bco-otb")
        {
            l_InterpolValue = ImageResamplerBuilderType::RB_INTERPOLATOR_BCO;
        }
        else if (method == "linear-itk")
        {
            l_InterpolValue = ImageResamplerBuilderType::RB_INTERPOLATOR_LINEAR;
        }
        else if (method == "linear-maccs")
        {
            l_InterpolValue = ImageResamplerBuilderType::RB_INTERPOLATOR_MAJA_FAST_LINEAR;
        }
        else if (method == "linear-maccs-with-radius")
        {
            l_InterpolValue = ImageResamplerBuilderType::RB_INTERPOLATOR_LINEAR_WITH_RADIUS;
        }
        else
        {
            std::cout << "The value for the parameter 'method' <" << method << "> is bad!" << std::endl;
            vnsStaticExceptionBusinessMacro("The interpolator is not initialized.")
        }
        std::cout << " - interpolator: " << method << std::endl;

        unsigned int radius(0);
        if (parseResult->IsOptionPresent("--Radius") == true)
        {
            radius = parseResult->GetParameterUInt("--Radius");
            std::cout << " - radius: " << radius << std::endl;
        }
        if (parseResult->IsOptionPresent("--PaddingRadius") == true)
        {
            //resampler->SetPaddingRadius(parseResult->GetParameterUInt("--PaddingRadius"));
        }
        //std::cout << " - paddingRadius: " << resampler->GetPaddingRadius() << std::endl;

        // Si grille
        if (parseResult->IsOptionPresent("--ReferenceGridImage") == true)
        {
            std::cout << "Resampling with a grid" << std::endl;
            grid = true;
            const std::string grid_inputFilename = parseResult->GetParameterString("--ReferenceGridImage");
            typename ReaderType::Pointer reader_grid = ReaderType::New();
            reader_grid->SetFileName(grid_inputFilename);
            reader_grid->UpdateOutputInformation();
            outOrigin = reader_grid->GetOutput()->GetOrigin();
            outSpacing = reader_grid->GetOutput()->GetSpacing();
            outSize = reader_grid->GetOutput()->GetLargestPossibleRegion().GetSize();
        }
        // Si factor + convention pixel
        else if (parseResult->IsOptionPresent("--InputOutputResols") == true)
        {
            std::cout << "Resampling with resolutions and convention." << std::endl;
            std::string mode = parseResult->GetParameterString("--Convention");
            typename ImageResamplerBuilderType::ModeType l_ModeValue = ImageResamplerBuilderType::NOT_MODE_INITIALIZED;
            if (mode == "centered")
            {
                l_ModeValue = vns::ResamplerHelper::RB_MODE_CENTERED_PIXEL;
            }
            else if (mode == "footprint")
            {
                l_ModeValue = vns::ResamplerHelper::RB_MODE_UPPER_LEFT_CORNER_PIXEL;
            }
            else
            {
                std::cout << "The value for the parameter 'mode' <" << mode << "> is bad!" << std::endl;
                vnsStaticExceptionBusinessMacro("The convention is bad.")
            }

            ImageResamplerBuilderType::ComputeParameters(inOrigin, inSpacing, inSize,
                    parseResult->GetParameterUInt("--InputOutputResols", 0), parseResult->GetParameterUInt("--InputOutputResols", 1),
                    outOrigin, outSpacing, outSize, l_ModeValue);
        }
        // Si emprise
        else
        {
            std::cout << "Resampling with footprint" << std::endl;
            outOrigin[0] = parseResult->GetParameterDouble("--Origin", 0);
            outOrigin[1] = parseResult->GetParameterDouble("--Origin", 1);
            outSpacing[0] = parseResult->GetParameterDouble("--Spacing", 0);
            outSpacing[1] = parseResult->GetParameterDouble("--Spacing", 1);
            outSize[0] = parseResult->GetParameterUInt("--Size", 0);
            outSize[1] = parseResult->GetParameterUInt("--Size", 1);
        }

        vns::AreaType l_Area;
        l_Area.Origin = outOrigin;
        l_Area.Spacing = outSpacing;
        l_Area.Size = outSize;

        // Resampler set-up
        resampler->SetAreaToOutputImageResolution(l_Area);
        resampler->SetInput0(input_image /*reader->GetOutput()*/);
        resampler->SetInterpolator(l_InterpolValue);
        std::cout << " Start resampler->UpdateWithCaching() ..." << std::endl;
        itk::TimeProbe l_clock;
        l_clock.Start();
        resampler->UpdateWithCaching();
        l_clock.Stop();

        std::cout << " -----   Time for resampler->UpdateWithCaching() is " << l_clock.GetTotal() << " seconds   ("
                << vns::Utilities::GetStringFormatedTime(l_clock.GetTotal()) << ")." << std::endl;

        std::cout << "    - Input image  origin centre pixel:         [" << inOrigin << "]" << std::endl;
        std::cout << "                   spacing:                     [" << inSpacing << "]" << std::endl;
        std::cout << "                   size:                        [" << inSize << "]" << std::endl;

        std::cout << "    - Output footprint origin centre pixel:     [" << outOrigin << "]" << std::endl;
        std::cout << "                   spacing:                     [" << outSpacing << "]" << std::endl;
        std::cout << "                   size:                        [" << outSize << "]" << std::endl;

    }
template<typename ImageType>
    void
    call_image_with_pad_and_resampler(typename otb::CommandLineArgumentParseResult::Pointer parseResult)
    {
        std::cout << " - call_image_with_pad_and_resampler ...." << std::endl;

        typedef otb::ImageFileReader<ImageType> ReaderType;
        typedef otb::ImageFileWriter<ImageType> WriterType;
        typedef vns::PadAndResampleImageFilter<ImageType, ImageType> ResamplerType;

        typename ReaderType::Pointer reader = ReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();
        typename ResamplerType::Pointer resampler = ResamplerType::New();

        const std::string inputFilename = parseResult->GetInputImage();
        reader->SetFileName(inputFilename.c_str());
        reader->UpdateOutputInformation();

        setting_resampler<ImageType, ReaderType, ResamplerType>(parseResult, reader->GetOutput(), resampler);

        const std::string outputFilename = parseResult->GetOutputImage();
        writer->SetFileName(outputFilename.c_str());

        unsigned int l_NumberOfLinesStrippedStreaming(200);
        if (parseResult->IsOptionPresent("--NumberOfLinesStrippedStreaming") == true)
        {
            l_NumberOfLinesStrippedStreaming = parseResult->GetParameterUInt("--NumberOfLinesStrippedStreaming");
        }
        std::cout << " - NumberOfLinesStrippedStreaming: " << l_NumberOfLinesStrippedStreaming << std::endl;

        itk::TimeProbe l_clock;
        l_clock.Start();
        writer->SetInput(resampler->GetOutput());
        l_clock.Start();
        //
        writer->SetNumberOfDivisionsStrippedStreaming(l_NumberOfLinesStrippedStreaming);
//        writer->SetAutomaticStrippedStreaming();
        writer->Update();
        l_clock.Stop();

        std::cout << " -----   Time is " << l_clock.GetTotal() << " seconds   ("
                << vns::Utilities::GetStringFormatedTime(l_clock.GetTotal()) << ")." << std::endl;

    }

template<typename PixelType>
    void
    call_list_of_images_with_pad_and_resampler(typename otb::CommandLineArgumentParseResult::Pointer parseResult)
    {
        std::cout << " - call_list_of_images_with_pad_and_resampler ...." << std::endl;

        typedef otb::VectorImage<PixelType, 2> VectorImageType;
        typedef otb::Image<PixelType, VectorImageType::ImageDimension> ImageType;

        typedef otb::ImageFileReader<VectorImageType> ReaderType;
        typedef otb::ImageFileWriter<VectorImageType> WriterType;

        typename ReaderType::Pointer reader = ReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();
        const std::string inputFilename = parseResult->GetInputImage();
        reader->SetFileName(inputFilename.c_str());
        reader->UpdateOutputInformation();

        typedef typename VectorImageType::Pointer VectorImagePointerType;

        typedef otb::ImageList<ImageType> ImageListType;
        typedef typename ImageListType::Pointer ImageListPointerType;

        typedef otb::VectorImageToImageListFilter<VectorImageType, ImageListType> DecompositionFilterType;
        typedef typename DecompositionFilterType::Pointer DecompositionFilterPointerType;
        typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType> RecompositionFilterType;
        typedef typename RecompositionFilterType::Pointer RecompositionFilterPointerType;

        DecompositionFilterPointerType decomposer = DecompositionFilterType::New();
        RecompositionFilterPointerType recomposer = RecompositionFilterType::New();

        typedef vns::PadAndResampleImageFilter<ImageType, ImageType> ResamplerType;

        decomposer->SetInput(reader->GetOutput());
        decomposer->UpdateOutputInformation();
        ImageListPointerType inputPtr = decomposer->GetOutput();
        ImageListPointerType outputPtr = ImageListType::New();

//        typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
//        while (inputListIt != inputPtr->End())
//        {
//            outputPtr->PushBack(OutputImageType::New());
//            ++inputListIt;
//        }

        std::vector<typename ResamplerType::Pointer> l_ListResamplerType;
// For each input image
        typename ImageListType::Iterator inputListIt = inputPtr->Begin();
        typename ImageListType::Iterator outputListIt = outputPtr->Begin();

        unsigned int cpt(0);
        while (inputListIt != inputPtr->End()) //&& outputListIt != outputPtr->End())
        {
            std::cout << " - Working band " << cpt++ << "..." << std::endl;

            l_ListResamplerType.push_back(ResamplerType::New());
            // Create the output image and set its information
            setting_resampler<ImageType, ReaderType, ResamplerType>(parseResult, inputListIt.Get(), l_ListResamplerType.back());

//            m_Filter->SetInput(inputListIt.Get());
//            m_Filter->UpdateOutputInformation();
//            outputListIt.Get()->CopyInformation(m_Filter->GetOutput(m_OutputIndex));
//            outputListIt.Get()->SetLargestPossibleRegion(m_Filter->GetOutput(m_OutputIndex)->GetLargestPossibleRegion());

            outputPtr->PushBack(l_ListResamplerType.back()->GetOutput());

            ++inputListIt;
//            ++outputListIt;
        }
        std::cout << " Nb bands " << cpt << std::endl;
        recomposer->SetInput(outputPtr);

        const std::string outputFilename = parseResult->GetOutputImage();
        writer->SetFileName(outputFilename.c_str());

        unsigned int l_NumberOfLinesStrippedStreaming(200);
        if (parseResult->IsOptionPresent("--NumberOfLinesStrippedStreaming") == true)
        {
            l_NumberOfLinesStrippedStreaming = parseResult->GetParameterUInt("--NumberOfLinesStrippedStreaming");
        }
        std::cout << " - NumberOfLinesStrippedStreaming: " << l_NumberOfLinesStrippedStreaming << std::endl;

        itk::TimeProbe l_clock;
        l_clock.Start();
        writer->SetInput(recomposer->GetOutput());
        l_clock.Start();
        //
        writer->SetNumberOfDivisionsStrippedStreaming(l_NumberOfLinesStrippedStreaming);
//        writer->SetAutomaticStrippedStreaming();
        writer->Update();
        l_clock.Stop();

        std::cout << " -----   Time is " << l_clock.GetTotal() << " seconds   ("
                << vns::Utilities::GetStringFormatedTime(l_clock.GetTotal()) << ")." << std::endl;

    }

int
vnsPadAndResamplerReSamplingTest(int argc, char * argv[])
{
// Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();
    std::string description =
            "MACCS Resampler tool [options]\n\n"
                    "  Three users cases:\n"
                    "     1. With a grid.                   Set option 'ReferenceGridImage'\n"
                    "     2. With a area.                   Set options 'Origin', 'Spacing' and 'Size''\n"
                    "     3. With factor and convention.    Set options 'InputOutputResols' for input and output resolutions and 'Convention' ('centered' or 'footprint').\n";
    unsigned int n = 1 << 1;
    std::cout << " 1 << 1 " << n << std::endl;
    n = 1 << 2;
    std::cout << " 1 << 2 " << n << std::endl;
    n = 2 << 1;
    std::cout << " 2 << 1 " << n << std::endl;
    n = 2 << 2;
    std::cout << " 2 << 2 " << n << std::endl;
    parser->SetProgramDescription(description);
    parser->AddInputImage(true);
    parser->AddOutputImage(true);
// Grif or footprint
    parser->AddOption("--ReferenceGridImage", "ReferenceGridImage", "-refgrid", 1, false);
    parser->AddOption("--Spacing", "Spacing", "-s", 2, false);
    parser->AddOption("--Origin", "Origin", "-o", 2, false);
    parser->AddOption("--Size", "Size", "-wh", 2, false);
    parser->AddOption("--InputOutputResols", "InputOutputResols", "", 2, false);
    parser->AddOption("--Convention", "Convention", "", 1, false);

    parser->AddOption("--Interpolator", "Interpolator", "-i", 1, true);
    parser->AddOption("--Radius", "Radius", "-r", 1, false);
    parser->AddOption("--PaddingRadius", "PaddingRadius", "-p", 1, false);
    parser->AddOption("--NbThreads", "NbThreads", "-t", 1, false);
    parser->AddOption("--NumberOfLinesStrippedStreaming", "NumberOfLinesStrippedStreaming", "", 1, false);
    parser->AddOption("--ImageType", "ImageType", "", 1, false);
    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer parseResult = ParserResultType::New();

    try
    {
        parser->ParseCommandLine(argc, argv, parseResult);
    }
    catch (itk::ExceptionObject & err)
    {
        std::string descriptionException = err.GetDescription();
        if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
        {
            std::cout << "WARNING : output file pixels are converted in 'unsigned char'" << std::endl;
            return EXIT_SUCCESS;
        }
        if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
        {
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }
    unsigned int nbThread(1);
    if (parseResult->IsOptionPresent("--NbThreads") == true)
    {
        nbThread = parseResult->GetParameterUInt("--NbThreads");
    }
    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(nbThread);

    std::cout << "NbThreads: " << itk::MultiThreader::GetGlobalDefaultNumberOfThreads() << std::endl;

    std::string l_ImageType = "image";
    if (parseResult->IsOptionPresent("--ImageType") == true)
    {
        l_ImageType = parseResult->GetParameterString("--ImageType");
    }
    bool l_VectorImage(false);
    if (l_ImageType == "vector-image")
    {
        l_VectorImage = true;
    }

    std::cout << "ImageType: " << l_ImageType << std::endl;
    std::cout << "VectorImage ? " << vns::Utilities::BoolToLowerString(l_VectorImage) << std::endl;

//    typedef otb::ImageFileReader<otb::Image<char> > RawReaderType;
//    RawReaderType::Pointer raw_reader = RawReaderType::New();
//    raw_reader->SetFileName(parseResult->GetInputImage());
//    raw_reader->UpdateOutputInformation();
//    std::cout << "PixelType detected: " << raw_reader->GetImageIO()->GetComponentTypeAsString(raw_reader->GetImageIO()->GetComponentType())
//            << " (" << raw_reader->GetImageIO()->GetComponentType() << ")" << std::endl;

    try
    {
        if (l_ImageType == "image")
        {
            call_image_with_pad_and_resampler<otb::Image<float, 2> >(parseResult);
//            if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::UCHAR)
//                call_image_with_pad_and_resampler<otb::Image<unsigned char, 2> >(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::CHAR)
////                call_image_with_pad_and_resampler<otb::Image<char, 2> >(parseResult);
//            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::SHORT)
//                call_image_with_pad_and_resampler<otb::Image<short, 2> >(parseResult);
//            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::USHORT)
//                call_image_with_pad_and_resampler<otb::Image<unsigned short, 2> >(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::FLOAT)
////                call_image_with_pad_and_resampler<otb::Image<float, 2> >(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::DOUBLE)
////                call_image_with_pad_and_resampler<otb::Image<double, 2> >(parseResult);
//            else
//            {
//                std::cout << "ERROR Pixel Type "
//                        << raw_reader->GetImageIO()->GetComponentTypeAsString(raw_reader->GetImageIO()->GetComponentType())
//                        << " not managed !! Modify the source code !!!" << std::endl;
//                return EXIT_FAILURE;
//            }
        }
        else if (l_ImageType == "vector-image")
        {
            call_image_with_pad_and_resampler<otb::VectorImage<float, 2> >(parseResult);
//            if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::UCHAR)
//                call_image_with_pad_and_resampler<otb::VectorImage<unsigned char, 2> >(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::CHAR)
////                call_image_with_pad_and_resampler<otb::VectorImage<char, 2> >(parseResult);
//            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::SHORT)
//                call_image_with_pad_and_resampler<otb::VectorImage<short, 2> >(parseResult);
//            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::USHORT)
//                call_image_with_pad_and_resampler<otb::VectorImage<unsigned short, 2> >(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::FLOAT)
////                call_image_with_pad_and_resampler<otb::VectorImage<float, 2> >(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::DOUBLE)
////                call_image_with_pad_and_resampler<otb::VectorImage<double, 2> >(parseResult);
//            else
//            {
//                std::cout << "ERROR Pixel Type "
//                        << raw_reader->GetImageIO()->GetComponentTypeAsString(raw_reader->GetImageIO()->GetComponentType())
//                        << " not managed !! Modify the source code !!!" << std::endl;
//                return EXIT_FAILURE;
//            }

        }
        else if (l_ImageType == "list-image")
        {
            call_list_of_images_with_pad_and_resampler<float>(parseResult);
//            if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::UCHAR)
//                call_list_of_images_with_pad_and_resampler<unsigned char>(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::CHAR)
////                call_image_with_pad_and_resampler<char>(parseResult);
//            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::SHORT)
//                call_list_of_images_with_pad_and_resampler<short>(parseResult);
//            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::USHORT)
//                call_list_of_images_with_pad_and_resampler<unsigned short>(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::FLOAT)
////                call_image_with_pad_and_resampler<float>(parseResult);
////            else if (raw_reader->GetImageIO()->GetComponentType() == otb::ImageIOBase::DOUBLE)
////                call_image_with_pad_and_resampler<double>(parseResult);
//            else
//            {
//                std::cout << "ERROR Pixel Type "
//                        << raw_reader->GetImageIO()->GetComponentTypeAsString(raw_reader->GetImageIO()->GetComponentType())
//                        << " not managed !! Modify the source code !!!" << std::endl;
//                return EXIT_FAILURE;
//            }
        }
    }
    catch (itk::ExceptionObject & err)
    {
        std::cout << "Processing error: " << err.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cout << "Processing error unknown !! " << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
