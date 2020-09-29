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

#include "otbStreamingResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "vnsLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"

namespace otb
{
    /**
     * \class ResamplerBuilder
     * \brief Helper class to build a ready to run instance of resampler
     *
     * This class can not be instanciated and only provides two static
     * methods:
     *
     * The ComputeParameters method will compute the output image
     * parameters (origin, spacing, size) for resampling according to the
     * input image parameters (origin, spacing, size), the resampling
     * ratio (k/l) and an ouptut grid mode (either pixel centers or
     * global footprint are preserved).
     *
     * The Build method will return a ready to run
     * StreamingResampleImageFilter instance based on an input image
     * pointer (the image to resample), the resampling ratio (k/l), the
     * output grid mode and the interpolator.
     *
     */
    template<class TInputImage, class TOutputImage = TInputImage>
        class ResamplerBuilder
        {
        public:

            typedef otb::StreamingResampleImageFilter<TInputImage, TOutputImage> ResamplerType;
            typedef itk::LinearInterpolateImageFunction<TInputImage, double> LinearInterpolatorType;
            typedef otb::BCOInterpolateImageFunction<TInputImage, double> BCOInterpolatorType;
            typedef vns::LinearInterpolateImageFunction<TInputImage, double> MACCSLinearInterpolatorType;

            // Enum for interpolator
            typedef enum
            {
                RB_INTERPOLATOR_LINEAR, RB_INTERPOLATOR_LINEAR_MACCS, RB_INTERPOLATOR_BCO, NOT_INTERPOLATOR_INITIALIZED
            } InterpolatorType;

            // Enum for output grid mode
            typedef enum
            {
                RB_MODE_CENTERED, RB_MODE_FP, NOT_MODE_INITIALIZED
            } ModeType;

            /**
             * Build a ready to run StreamingResampleImageFilter instance from
             * an input image pointer, the resampling ratio, interpolation type
             * and output grid mode.
             * \param inputPointer Pointer to the image to resample
             * \param fnum The size of the input image will be multiplied by fnum/fdenom
             * \param fdenom The size of the input image will be multiplied by fnum/fdenom
             * \param interpolator The interpolator type (can be
             *  RB_INTERPOLATOR_LINEAR or RB_INTERPOLATOR_BCO)
             * \param mode The output grid mode (can be RB_MODE_CENTERED or
             *  RB_MODE_FP)
             * \param strict If true an exception will be thrown if computation
             *  does not yield exact results
             * \return A pointer to a read to run instance of the StreamingResampleImageFilter
             */
            static typename ResamplerType::Pointer
            Build(TInputImage * inputPointer, const unsigned int & fnum, const unsigned int & fdenom, const InterpolatorType & interpolator,
                    const unsigned int & p_radius, const ModeType& mode, bool strict = true)
            {
                // Instanciate a new instance of resampler
                typename ResamplerType::Pointer resampler = ResamplerType::New();

                // First, get input image parameters
                if (!inputPointer)
                {
                    itkGenericExceptionMacro(<<"Input image pointer is null.");
                }

                // Update image pointer to ensure information is here
                inputPointer->UpdateOutputInformation();

                // Retrieve origin, spacing, size
                typename TInputImage::PointType inOrigin = inputPointer->GetOrigin();
                typename TInputImage::SpacingType inSpacing = inputPointer->GetSpacing();
                typename TInputImage::SizeType inSize = inputPointer->GetLargestPossibleRegion().GetSize();

                // Now, compute output image parameters
                typename TOutputImage::PointType outOrigin;
                typename TOutputImage::SpacingType outSpacing;
                typename TOutputImage::SizeType outSize;

                // Compute output parameters
                ComputeParameters(inOrigin, inSpacing, inSize, fnum, fdenom, outOrigin, outSpacing, outSize, mode, strict);

                // Resampler set-up
                resampler->SetOutputOrigin(outOrigin);
                resampler->SetOutputSpacing(outSpacing);
                resampler->SetOutputSize(outSize);
//                resampler->SetInput(inputPointer);
                resampler->SetDisplacementFieldSpacing(outSpacing);

                // Interpolator set-up
                switch (interpolator)
                    {
                case RB_INTERPOLATOR_LINEAR:
                    {
                        typename LinearInterpolatorType::Pointer interpolator = LinearInterpolatorType::New();
                        resampler->SetInterpolator(interpolator);
                        break;
                    }
                case RB_INTERPOLATOR_LINEAR_MACCS:
                    {
                        typename MACCSLinearInterpolatorType::Pointer interpolator = MACCSLinearInterpolatorType::New();
                        interpolator->SetRadius(p_radius);
                        resampler->SetInterpolator(interpolator);
                        break;
                    }
                case RB_INTERPOLATOR_BCO:
                    {
                        typename BCOInterpolatorType::Pointer interpolator = BCOInterpolatorType::New();
                        interpolator->SetRadius(p_radius);
                        resampler->SetInterpolator(interpolator);
                        break;
                    }
                case NOT_INTERPOLATOR_INITIALIZED:
                    {
                        break;
                    }
                    }

                // Return the resampler we created
                return resampler;
            }
            static unsigned int
            ComputeRadius(const unsigned int & fnum, const unsigned int & fdenom, const InterpolatorType & interpolator)
            {
                // Test if sous-ech or sur-ech
                double inv_factor(0.);
                std::cout << " ComputeRadius - num '" << fnum << "' and denom '" << fdenom << "'" << std::endl;
                if (fnum >= fdenom)
                {
                    std::cout << " -> SUR echantillonnage" << std::endl;
                    inv_factor = 1.;
                }
                else
                {
                    std::cout << " -> SOUS echantillonnage" << std::endl;
                    inv_factor = static_cast<double>(fdenom) / static_cast<double>(fnum);
                }
                // Instanciate a new instance of resampler
                unsigned int l_Radius(0);

                // Interpolator set-up
                switch (interpolator)
                    {
                case RB_INTERPOLATOR_LINEAR:
                    {
                        l_Radius = static_cast<unsigned int>(1. * inv_factor);
                        break;
                    }
                case RB_INTERPOLATOR_LINEAR_MACCS:
                    {
                        l_Radius = static_cast<unsigned int>(1. * inv_factor);
                        break;
                    }
                case RB_INTERPOLATOR_BCO:
                    {
                        l_Radius = static_cast<unsigned int>(2. * inv_factor);
                        break;
                    }
                case NOT_INTERPOLATOR_INITIALIZED:
                    {
                        break;
                    }
                    }

                // Return the resampler we created
                return l_Radius;
            }
            /**
             * Compute the output image parameters from the input parameter,
             * resampling ratio and output image grid mode.
             * \param inOrigin Origin of the input image
             * \param inSpacing Spacing of the input image
             * \param inSize Size of the input image
             * \param fnum The size of the input image will be multiplied by fnum/fdenom
             * \param fdenom The size of the input image will be multiplied by fnum/fdenom
             * \param outOrigin A pointer to the variable holding output origin (will be
             *  updated during call)
             * \param outSpacing A pointer to the variable holding output spacing (will be
             *  updated during call)
             * \param outSize A pointer to the variable holding output size (will be
             *  updated during call)
             * \param mode The output grid mode (can be RB_MODE_CENTERED or RB_MODE_FP)
             * \param strict If true an exception will be thrown if computation
             */
            static void
            ComputeParameters(const typename TInputImage::PointType & inOrigin, const typename TInputImage::SpacingType & inSpacing,
                    const typename TInputImage::SizeType & inSize, const unsigned int fnum, const unsigned int fdenom,
                    typename TOutputImage::PointType & outOrigin, typename TOutputImage::SpacingType & outSpacing,
                    typename TOutputImage::SizeType & outSize, const ModeType & mode, bool strict = true)
            {

                // Compute resampling factor
                if (fnum == 0 || fdenom == 0)
                {
                    itkGenericExceptionMacro(<<"At least one of fnum ("<<fnum<<") or fdenom ("<<fdenom<<") parameter is null");
                }
                double factor = static_cast<double>(fnum) / static_cast<double>(fdenom);

                // Output spacing is always the same
                outSpacing[0] = inSpacing[0] / factor;
                outSpacing[1] = inSpacing[1] / factor;

                switch (mode)
                    {
                case RB_MODE_CENTERED:
                    {
                        if (strict && ((inSize[0] - 1) % fdenom != 0 || (inSize[1] - 1) % fdenom != 0))
                        {
                            itkGenericExceptionMacro(<<"Strict mode is enabled, and exact resampling is impossible.");
                        }

                        // In centered case, origin does not move
                        outOrigin = inOrigin;

                        // In centered case, the output size needs to be adjusted
                        outSize[0] = static_cast<unsigned long>(vcl_floor(factor * inSize[0] + 1 - factor + 0.5));
                        outSize[1] = static_cast<unsigned long>(vcl_floor(factor * inSize[1] + 1 - factor + 0.5));
                        break;
                    }
                case RB_MODE_FP:
                    {
                        if (strict && ((inSize[0] % fdenom != 0 || inSize[1] % fdenom != 0)))
                        {
                            itkGenericExceptionMacro(<<"Strict mode is enabled, and exact resampling is impossible.");
                        }

                        // In this case, the origin has to be shifted to preserve
                        // footprint:
                        outOrigin[0] = inOrigin[0] - (factor - 1) * inSpacing[0] / (2 * factor);
                        outOrigin[1] = inOrigin[1] - (factor - 1) * inSpacing[1] / (2 * factor);

                        // Size is straightforward
                        outSize[0] = static_cast<unsigned long>(vcl_floor(factor * inSize[0] + 0.5));
                        outSize[1] = static_cast<unsigned long>(vcl_floor(factor * inSize[1] + 0.5));
                        break;
                    }
                case NOT_MODE_INITIALIZED:
                default:
                    {
                        itkGenericExceptionMacro(<<"Unknown ResamplerBuilder mode.");
                        break;
                    }
                    }
            }
        private:
            // This class can not be instanciated
            ResamplerBuilder();                //purposely not implemented
            ~ResamplerBuilder();                //purposely not implemented
            ResamplerBuilder(const ResamplerBuilder&);                //purposely not implemented
            void
            operator =(const ResamplerBuilder&);                //purposely not implemented
        };

}
// end namespace otb

/////////////////////////////////////////////////////////////////////////////////////////////////
// Test code

#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkMultiThreader.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsUtilities.h"
#include "itkTimeProbe.h"
#include "itkMeanImageFilter.h"
#include "otbPerBandVectorImageFilter.h"

template<class TImage>
    int
    generic_function(int argc, char * argv[])
    {
        std::cout << "argc: " << argc << std::endl;
        if (argc < 8)
        {
            std::cerr << "Usage: " << argv[0]
                    << " inOrigin inSpacing inSize fnum fdenom method [bco / linear] smooth [null, mean + radius , gauss + radius]"
                    << std::endl;
            return EXIT_FAILURE;
        }
        typedef TImage ImageType;
        typedef otb::ResamplerBuilder<ImageType> ImageResamplerBuilderType;
        typename ImageType::SizeType inSize, outSize;
        typename ImageType::PointType inOrigin, outOrigin;
        typename ImageType::SpacingType inSpacing, outSpacing;
        typedef otb::Image<typename ImageType::InternalPixelType> SingleImageType;

        typedef otb::ImageFileReader<ImageType> ReaderType;
        typedef otb::ImageFileWriter<ImageType> WriterType;
        typedef itk::MeanImageFilter<SingleImageType, SingleImageType> MeanFilterType;
        typedef otb::PerBandVectorImageFilter<ImageType, ImageType, MeanFilterType> PerBandVectorImageFilterType;

        unsigned int cpt(1);
        const std::string inputFilename = argv[cpt++];
        const std::string outputFilename = argv[cpt++];
        const unsigned int nbthreads = atoi(argv[cpt++]);
//  std::string smooth(argv[cpt++]);
//  unsigned int smooth_radius(0);
//  if ((method == "mean") || (method == "gauss"))
//  {
//      smooth_radius = atoi(argv[cpt++]);
//  }

        itk::MultiThreader::SetGlobalDefaultNumberOfThreads(nbthreads);

        typename ReaderType::Pointer reader = ReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();
        reader->SetFileName(inputFilename);
        reader->GenerateOutputInformation();
        writer->SetFileName(outputFilename);
        inOrigin = reader->GetOutput()->GetOrigin();
        inSpacing = reader->GetOutput()->GetSpacing();
        inSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
//  inOrigin.Fill(atof(argv[cpt++]));
//  inSpacing.Fill(atof(argv[cpt++]));
//  inSize.Fill(atoi(argv[cpt++]));
        unsigned int num = atoi(argv[cpt++]);
        unsigned int denom = atoi(argv[cpt++]);
        bool sous_ech(true);
        if (num >= denom)
        {
            std::cout << " SUR echantillonnage" << std::endl;
            sous_ech = false;
        }

        std::string mode(argv[cpt++]);
        std::string method(argv[cpt++]);

        typename ImageResamplerBuilderType::InterpolatorType l_InterpolValue = ImageResamplerBuilderType::NOT_INTERPOLATOR_INITIALIZED;
        typename ImageResamplerBuilderType::ModeType l_ModeValue = ImageResamplerBuilderType::NOT_MODE_INITIALIZED;
        unsigned int radius(0);
        std::cout << "Parameters: " << std::endl;
        std::cout << " - inputFilename: " << inputFilename << std::endl;
        std::cout << " - outputFilename: " << outputFilename << std::endl;
        std::cout << " - nbthreads: " << nbthreads << std::endl;
        std::cout << " - method: " << method << std::endl;
        std::cout << " - mode: " << mode << std::endl;

        if (method == "bco")
        {
            l_InterpolValue = ImageResamplerBuilderType::RB_INTERPOLATOR_BCO;
        }
        else if (method == "linear_maccs")
        {
            l_InterpolValue = ImageResamplerBuilderType::RB_INTERPOLATOR_LINEAR_MACCS;
        }
        else if (method == "linear")
        {
            l_InterpolValue = ImageResamplerBuilderType::RB_INTERPOLATOR_LINEAR;
        }
        else
        {
            std::cout << "The value for the parameter 'method' <" << method << "> is bad!" << std::endl;
            return EXIT_FAILURE;
        }

        if (mode == "centered")
        {
            l_ModeValue = ImageResamplerBuilderType::RB_MODE_CENTERED;
        }
        else if (mode == "footprint")
        {
            l_ModeValue = ImageResamplerBuilderType::RB_MODE_FP;
        }
        else
        {
            std::cout << "The value for the parameter 'mode' <" << mode << "> is bad!" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << num << "/" << denom << " resampling a " << inSize << " image with origin " << inOrigin << " and spacing " << inSpacing
                << " (centered): " << std::endl;

        try
        {
            ImageResamplerBuilderType::ComputeParameters(inOrigin, inSpacing, inSize, num, denom, outOrigin, outSpacing, outSize,
                    ImageResamplerBuilderType::RB_MODE_CENTERED);
        }
        catch (itk::ExceptionObject&)
        {
            std::cout << "Strict resampling impossible, retrying without strict constraint ..." << std::endl;
            ImageResamplerBuilderType::ComputeParameters(inOrigin, inSpacing, inSize, num, denom, outOrigin, outSpacing, outSize,
                    ImageResamplerBuilderType::RB_MODE_CENTERED, false);
        }

        std::cout << "origin: " << outOrigin << ", spacing: " << outSpacing << ", size: " << outSize << std::endl;

        std::cout << num << "/" << denom << " resampling a " << inSize << " image with origin " << inOrigin << " and spacing " << inSpacing
                << " (footprint): " << std::endl;

        radius = ImageResamplerBuilderType::ComputeRadius(num, denom, l_InterpolValue);
        std::cout << " Radius computed in main: " << radius << std::endl;
        std::cout << " - Radius: " << radius << std::endl;

        try
        {
            ImageResamplerBuilderType::ComputeParameters(inOrigin, inSpacing, inSize, num, denom, outOrigin, outSpacing, outSize,
                    ImageResamplerBuilderType::RB_MODE_FP);
        }
        catch (itk::ExceptionObject&)
        {
            std::cout << "Strict resampling impossible, retrying without strict constraint ..." << std::endl;
            ImageResamplerBuilderType::ComputeParameters(inOrigin, inSpacing, inSize, num, denom, outOrigin, outSpacing, outSize,
                    ImageResamplerBuilderType::RB_MODE_FP, false);
        }

        std::cout << "origin: " << outOrigin << ", spacing: " << outSpacing << ", size: " << outSize << std::endl;
        typename ImageResamplerBuilderType::ResamplerType::Pointer resampler = ImageResamplerBuilderType::Build(reader->GetOutput(), num,
                denom, l_InterpolValue, radius, l_ModeValue, false);

//  typedef itk::MeanImageFilter<ImageType, ImageType>         MeanFilterType;
//  typename MeanFilterType::Pointer smooth_filter = MeanFilterType::New();

        std::cout << "VectorImage resamplers allocation ok." << std::endl;
        // Instantiating object
        typename PerBandVectorImageFilterType::Pointer meanfilter = PerBandVectorImageFilterType::New();
        if ((method == "linear") && (sous_ech == true))
        {
            std::cout << "Using MeanImageFIlter in pre processing with radius: " << radius << std::endl;
            meanfilter->SetInput(reader->GetOutput());
            meanfilter->GetFilter()->SetRadius(radius);
            resampler->SetInput(meanfilter->GetOutput());
        }
        else
        {
            resampler->SetInput(reader->GetOutput());
        }
        writer->SetInput(resampler->GetOutput());
        itk::TimeProbe l_clock;
        l_clock.Start();
        writer->Update();
        l_clock.Stop();

        std::cout << " -----   Time is " << l_clock.GetTotal() << " seconds   ("
                << vns::Utilities::GetStringFormatedTime(l_clock.GetTotal()) << ")." << std::endl;

        return EXIT_SUCCESS;
    }

int
vnsCNESJulienMichelResamplerBuilder(int argc, char * argv[])
{
    if (argc < 9)
    {
        std::cerr << "ICI Usage: " << argv[0] << " inOrigin inSpacing inSize fnum fdenom method [bco / linear]" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "argc: " << argc << std::endl;
    const std::string image_type = argv[1];

    /*    if (image_type == "image")
     {
     return generic_function<otb::Image<float> >(argc - 1, argv + 1);
     }
     else */if (image_type == "vector-image")
    {
        return generic_function<otb::VectorImage<float> >(argc - 1, argv + 1);
    }
    else
    {
        std::cout << "Imagetype bad value: 'image' or 'vector-image'" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}
