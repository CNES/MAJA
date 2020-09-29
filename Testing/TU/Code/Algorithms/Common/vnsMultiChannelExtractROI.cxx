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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "vnsMultiChannelExtractROI.h"
#include "itkTimeProbe.h"
#include "vnsUtilities.h"
#include "vnsConstantVectorImageSource.h"
#include "otbStreamingImageVirtualWriter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

template<class IExtractROIFilterType>
    int
    generic_otbMultiChannelExtractROI(int /*argc*/, char * argv[])
    {
        int cpt(2);
        std::string value;

        const int nbline(atoi(argv[cpt++]));
        const int nbcol(atoi(argv[cpt++]));
        const int nbchannel(atoi(argv[cpt++]));
        std::vector<double> listValueBand;

        for (int i = 0; i < nbchannel; i++)
        {
            std::string value(argv[cpt++]);
            listValueBand.push_back(vns::Utilities::AsDouble(value));
        }
        value = argv[cpt++];
        const unsigned long nb_stream_division(vns::Utilities::AsInt(value));
        value = argv[cpt++];
        const unsigned int first_channel(vns::Utilities::AsInt(value));
        value = argv[cpt++];
        const unsigned int last_channel(vns::Utilities::AsInt(value));

        std::cout << "               Image size : NbLine      : " << nbline << std::endl;
        std::cout << "                            NbColo      : " << nbcol << std::endl;
        std::cout << "                            NbBand      : " << nbchannel << std::endl;
        std::cout << "               NbStreamDivision         : " << nb_stream_division << std::endl;

        typedef IExtractROIFilterType ExtractROIFilterType;

        typename ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

        const unsigned int Dimension = 2;
        typedef otb::VectorImage<double,Dimension>   RefImageType;
        typedef typename RefImageType::PixelType RefPixelType;
        typedef vns::ConstantVectorImageSource<RefImageType>  FilterType;

        /** instantiating the filter */
        typename FilterType::Pointer filter = FilterType::New();

        RefPixelType pixel;

        typename RefImageType::SizeValueType randomSize[2];

        randomSize[0] = nbline;
        randomSize[1] = nbcol;
        filter->SetSize(randomSize);

        typename RefImageType::SpacingValueType spacing[2] = {1., 1.};
        filter->SetSpacing( spacing );

        typename RefImageType::PointValueType origin[2] = {0, 0};
        filter->SetOrigin( origin );

        pixel.SetSize(nbchannel);
        for (int i=0 ; i<nbchannel ; i++)
        {
            pixel.SetElement(i,static_cast<double>(listValueBand[i]));
        }
        filter->SetConstantValue(pixel);
        extractROIFilter->SetFirstChannel(first_channel);
        extractROIFilter->SetLastChannel(last_channel);

        extractROIFilter->SetInput(filter->GetOutput());
        std::cout << "PERF MULTIEXTARCT"<<std::endl;
        std::cout << "               Image size : NbLine      : "<<nbline<<std::endl;
        std::cout << "                            NbColo      : "<<nbcol<<std::endl;
        std::cout << "                            NbBand      : "<<nbchannel<<std::endl;
        std::cout << "               NbStreamDivision         : "<<nb_stream_division<<std::endl;

        typename otb::StreamingImageVirtualWriter<RefImageType>::Pointer l_PerfVirtualWriter = otb::StreamingImageVirtualWriter<RefImageType>::New();
        l_PerfVirtualWriter->SetInput(extractROIFilter->GetOutput());
        l_PerfVirtualWriter->SetNumberOfDivisionsStrippedStreaming(nb_stream_division);
        itk::TimeProbe clock;
        clock.Start();
        l_PerfVirtualWriter->Update();
        clock.Stop();

        std::cout << " Total Time : extract data " << std::endl;
        std::cout << vns::Utilities::GetStringFormatedTime(clock.GetTotal()) << std::endl;

        return EXIT_SUCCESS;
    }

int
vnsMultiChannelExtractROI(int argc, char * argv[])
{
    std::string linputFilter;
    int cpt(1);
    //Si le format n'est pas sp�cif�, alors celui par defaut
    linputFilter = std::string(argv[cpt++]);

    if (linputFilter == "OTB")
    {
        std::cout << "Extract Channel OTB" << std::endl;
        typedef otb::MultiChannelExtractROI<double, double> ExtractROIFilterType;
        return (generic_otbMultiChannelExtractROI<ExtractROIFilterType> (argc, argv));
    }
    else
    {
        std::cout << "Extract Channel VNS" << std::endl;
        typedef vns::MultiChannelExtractROI<double, double> ExtractROIFilterType;
        return (generic_otbMultiChannelExtractROI<ExtractROIFilterType> (argc, argv));
    }
    return EXIT_FAILURE;

}
