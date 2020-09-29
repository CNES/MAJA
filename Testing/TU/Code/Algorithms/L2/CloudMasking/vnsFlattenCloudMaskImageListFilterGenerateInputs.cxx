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
 * VERSION : 5.0 : <TypeFT> : <NumFT> : Creation *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include <cmath>
#include "vnsFlattenCloudMaskImageListFilter.h"
#include <iostream>
#include <sstream>
#include "itkMacro.h"
#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "otbImageFileWriter.h"
#include "otbImageList.h"

template<typename II>
    std::string
    join(II first, II last)
    {
        std::ostringstream oss;
        for (; first != last; ++first)
            oss << *first << ", ";
        return oss.str();
    }

template<typename C>
    std::string
    join(C const& c)
    {
        return join(c.begin(), c.end());
    }

namespace itk
{
    template<typename TIterator>
        struct FixedIterator
        {
            typedef TIterator IteratorType;
            typedef typename IteratorType::PixelType ValueType;
            typedef vns::PixelReference<IteratorType> ReferenceType;

            explicit
            FixedIterator(TIterator const& it) :
                    m_it(it)
            {
            }

            // FixedIterator& operator=(PixelType const& pix) { m_it.Set(pix); }
            ValueType
            operator*() const
            {
                return m_it.Get();
            }
            ReferenceType
            operator*()
            {
                return ReferenceType(m_it);
            }
            FixedIterator &
            operator++()
            {
                ++m_it;
                return *this;
            }

            friend bool
            operator!=(FixedIterator const& lhs_, FixedIterator const& rhs_)
            {
                return lhs_.m_it != rhs_.m_it;
            }
            friend bool
            operator==(FixedIterator const& lhs_, FixedIterator const& rhs_)
            {
                return lhs_.m_it == rhs_.m_it;
            }
        private:
            IteratorType m_it;
        };

    template<typename TIterator, typename TImage, typename TRegion>
        inline FixedIterator<TIterator>
        begin(TImage const& i_, TRegion const& r_)
        {
            TIterator it(i_, r_);
            return FixedIterator<TIterator>(it);
        }
    template<typename TIterator, typename TImage, typename TRegion>
        inline FixedIterator<TIterator>
        end(TImage const& i_, TRegion const& r_)
        {
            TIterator it(i_, r_);
            it.GoToEnd();
            return FixedIterator<TIterator>(it);
        }
} // namespace itk

const unsigned int Dimension = 2;
typedef unsigned char MaskPixelType;

typedef itk::Image<MaskPixelType, Dimension> MaskImageType;
typedef itk::ImageRegionIterator<MaskImageType> MaskIteratorType;
typedef otb::ImageList<MaskImageType> MaskImageListType;

typedef itk::Image<MaskPixelType, Dimension> ImageType;
typedef itk::ImageRegionIterator<ImageType> IteratorType;

typedef itk::Image<MaskPixelType, Dimension> OutMaskImageType;
typedef otb::ImageList<OutMaskImageType> OutMaskImageListType;

static MaskImageListType::Pointer
make_image_list(bool verbose)
{
    MaskImageType::IndexType start;
    start[0] = 0;  // first index on X
    start[1] = 0;  // first index on Y

    MaskImageType::SizeType size;
    size[0] = 200;  // size along X
    size[1] = 200;  // size along Y

    MaskImageListType::Pointer imageList = MaskImageListType::New();
    for (unsigned short i = 0; i != 4; ++i)
    {
        MaskImageType::RegionType region;
        region.SetSize(size);
        region.SetIndex(start);

        MaskImageType::Pointer image = MaskImageType::New();
        image->SetRegions(region);
        image->Allocate();

        itk::FixedIterator<MaskIteratorType> it = itk::begin<MaskIteratorType>(image, region);
        itk::FixedIterator<MaskIteratorType> end = itk::end<MaskIteratorType>(image, region);
        for (std::size_t n = 0; it != end; ++it, ++n)
        {
            // i = 0 => [0:50  ,  0:50]    <- 1
            // i = 1 => [0:50  ,  150:200] <- 1
            // i = 2 => [150:200, 0:50]    <- 1
            // i = 3 => [150:200, 150:200] <- 1
            // =>
            // col <- i:even -> [0:50],  i:odd -> [150:200]
            // lig <- i<2    -> [0:50],  i>=2  -> [150:200]
            const int col = n % 200;
            const int lig = n / 200;
            const bool set = (((i % 2 == 0) && (col < 50)) || ((i % 2 == 1) && (col >= 150)))
                    && (((i <= 1) && (lig < 50)) || ((i > 1) && (lig >= 150)));
            *it = set ? 1 : 0;
        }

        imageList->PushBack(image);

        if (verbose)
        {
            typedef otb::ImageFileWriter<MaskImageType> WriterType;
            WriterType::Pointer writer = WriterType::New();

            char filename[512];
            (void) snprintf(filename, sizeof(filename), "input-mask-to-flatten-%d.tiff", i);
            writer->SetFileName(filename);
            writer->SetInput(image);
            writer->Update();
            std::cout << filename << " written" << std::endl;
        }
    }

    return imageList;
}

static
int
test_image_list(MaskImageListType & images)
{
    std::cout << "Testing images produced..." << std::flush;
    vns::ImageListPixelIterator<MaskIteratorType> it(images, images.Front()->GetLargestPossibleRegion());

    bool isOK = true;
    for (std::size_t n = 0; !it.IsAtEnd(); ++it, ++n)
    {
        const std::size_t col = n % 200;
        const std::size_t lig = n / 200;
        const bool set0 = (col < 50) && (lig < 50);
        const bool set1 = (col >= 150) && (lig < 50);
        const bool set2 = (col < 50) && (lig >= 150);
        const bool set3 = (col >= 150) && (lig >= 150);
        if ((*it)[0] != (set0 ? 1 : 0))
        {
            if (isOK)
            {
                std::cout << "\n";
            }
            std::cout << " - Pixel0[" << lig << "][" << col << "]=" << int((*it)[0]) << "  != expected " << (set0 ? 1 : 0) << "\n";
            isOK = false;
        }
        if ((*it)[1] != (set1 ? 1 : 0))
        {
            if (isOK)
            {
                std::cout << "\n";
            }
            std::cout << " - Pixel1[" << lig << "][" << col << "]=" << int((*it)[1]) << "  != expected " << (set1 ? 1 : 0) << "\n";
            isOK = false;
        }
        if ((*it)[2] != (set2 ? 1 : 0))
        {
            if (isOK)
            {
                std::cout << "\n";
            }
            std::cout << " - Pixel2[" << lig << "][" << col << "]=" << int((*it)[2]) << "  != expected " << (set2 ? 1 : 0) << "\n";
            isOK = false;
        }
        if ((*it)[3] != (set3 ? 1 : 0))
        {
            if (isOK)
            {
                std::cout << "\n";
            }
            std::cout << " - Pixel3[" << lig << "][" << col << "]=" << int((*it)[3]) << "  != expected " << (set3 ? 1 : 0) << "\n";
            isOK = false;
        }
    }
    if (isOK)
        std::cout << " OK";
    std::cout << std::endl;

    return isOK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
apply_filter(MaskImageListType & images, bool verbose)
{
    typedef OutMaskImageType OutputKeptMasksMasksType;
    typedef vns::FlattenCloudMaskImageListFilter<MaskImageType, OutputKeptMasksMasksType> FilterType;

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(&images);

    if (verbose)
    {
        typedef otb::ImageFileWriter<OutputKeptMasksMasksType> WriterType;
        WriterType::Pointer writer = WriterType::New();

        std::string filename = "output-flatten-mask.tiff";
        writer->SetFileName(filename);
        writer->SetInput(filter->GetOutput());
        writer->Update();
        std::cout << filename << " written" << std::endl;
    }
    else
    {
        filter->Update();
    }

    return EXIT_SUCCESS;
}

int
vnsFlattenCloudMaskImageListFilterGenerateInputs(int /*argc*/, char **/*argv*/)
{
    bool verbose = true;
    MaskImageListType::Pointer masks = make_image_list(verbose);
    if (test_image_list(*masks) == EXIT_FAILURE)
        return EXIT_FAILURE;
    return apply_filter(*masks, verbose);
    return EXIT_FAILURE;
}

