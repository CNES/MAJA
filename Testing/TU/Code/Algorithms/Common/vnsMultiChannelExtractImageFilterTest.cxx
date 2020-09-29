/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#include "itkMacro.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkTimeProbe.h"
#include "vnsUtilities.h"
#include "vnsMultiChannelsExtractImageFilter.h"
#include "otbStreamingImageVirtualWriter.h"
#include "otbVectorImage.h"
#include "vnsVectorLookUpTable.h"
#include "vnsGRIBImageIOFactory.h"
#include "itkObjectFactoryBase.h"

template<class TImage>
    int
    generic_otbMultiChannelExtractROI(int /*argc*/, char * argv[])
    {
        int cpt(2);
        std::string inputfilename = std::string(argv[cpt++]);
        std::string outputfilename = std::string(argv[cpt++]);

        typedef otb::ImageFileReader<TImage> ReaderType;
        typedef otb::ImageFileWriter<TImage> WriterType;
        typedef vns::MultiChannelsExtractImageFilter<TImage, TImage> MultiChannelExtractType;

        typename MultiChannelExtractType::Pointer extractChannel = MultiChannelExtractType::New();
        typename ReaderType::Pointer reader = ReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();

        reader->SetFileName(inputfilename);
        writer->SetFileName(outputfilename);

        extractChannel->SetInput(reader->GetOutput());
        while (argv[cpt] != NULL)
        {
            std::cout << "Selection of the channel index : " << argv[cpt] << std::endl;
            extractChannel->SetChannel(atoi(argv[cpt++]));
        }

        writer->SetInput(extractChannel->GetOutput());

        itk::TimeProbe clock;
        clock.Start();
        writer->Update();
        clock.Stop();

        std::cout << " Total Time : extract data " << std::endl;
        std::cout << vns::Utilities::GetStringFormatedTime(clock.GetTotal()) << std::endl;

        return EXIT_SUCCESS;
    }
template<class TImage>
    int
    generic_otbMultiChannelExtractROIWithITK(int /*argc*/, char * argv[])
    {
        int cpt(2);
        std::string inputfilename = std::string(argv[cpt++]);
        std::string outputfilename = std::string(argv[cpt++]);

        typedef itk::ImageFileReader<TImage> ReaderType;
        typedef itk::ImageFileWriter<TImage> WriterType;
        typedef vns::MultiChannelsExtractImageFilter<TImage, TImage> MultiChannelExtractType;

        typename MultiChannelExtractType::Pointer extractChannel = MultiChannelExtractType::New();
        typename ReaderType::Pointer reader = ReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();

        reader->SetFileName(inputfilename);
        writer->SetFileName(outputfilename);

        extractChannel->SetInput(reader->GetOutput());
        while (argv[cpt] != NULL)
        {
            std::cout << "Selection of the channel index : " << argv[cpt] << std::endl;
            extractChannel->SetChannel(atoi(argv[cpt++]));
        }

        writer->SetInput(extractChannel->GetOutput());

        itk::TimeProbe clock;
        clock.Start();
        writer->Update();
        clock.Stop();

        std::cout << " Total Time : extract data " << std::endl;
        std::cout << vns::Utilities::GetStringFormatedTime(clock.GetTotal()) << std::endl;

        return EXIT_SUCCESS;
    }

int
vnsMultiChannelExtractImageFilterTest(int argc, char * argv[])
{
    itk::ObjectFactoryBase::RegisterFactory( vns::GRIBImageIOFactory::New() );

    std::string linputFilter;
    int cpt(1);
    //Si le format n'est pas sp�cif�, alors celui par defaut
    linputFilter = std::string(argv[cpt++]);

    std::cout << "Selection dimensions: " << linputFilter << std::endl;
    if (linputFilter == "3D")
    {
//        return (generic_otbMultiChannelExtractROI<otb::VectorImage<double, 3> >(argc, argv));
        return (generic_otbMultiChannelExtractROIWithITK<vns::VectorLookUpTable<double, 3> >(argc, argv));
    }
    else
    {
        return (generic_otbMultiChannelExtractROI<otb::VectorImage<double, 2> >(argc, argv));
    }
    return EXIT_FAILURE;

}
