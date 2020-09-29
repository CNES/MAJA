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

#include "vnsImageListPixelIterator.h"

#include <iostream>
#include <cmath>

#include "itkMacro.h"
#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"
#include "otbImageList.h"

namespace itk
{
    template <typename TIterator>
    struct FixedIterator
    {
        typedef TIterator IteratorType;
        typedef typename IteratorType::PixelType ValueType;
        typedef vns::PixelReference<IteratorType> ReferenceType;

        explicit FixedIterator(TIterator const& it) : m_it(it) {}

        // FixedIterator& operator=(PixelType const& pix) { m_it.Set(pix); }
        ValueType operator*() const                    { return m_it.Get(); }
        ReferenceType operator*()                      { return ReferenceType(m_it); }
        FixedIterator & operator++()                   { ++m_it; return *this; }

        friend bool operator!=(FixedIterator const& lhs_, FixedIterator const& rhs_)
        { return lhs_.m_it != rhs_.m_it; }
        friend bool operator==(FixedIterator const& lhs_, FixedIterator const& rhs_)
        { return lhs_.m_it == rhs_.m_it; }
    private:
        IteratorType m_it;
    };

    template <typename TIterator, typename TImage, typename TRegion> inline
    FixedIterator<TIterator> begin(TImage const& i_, TRegion const& r_)
    {
        TIterator it(i_, r_);
        return FixedIterator<TIterator>(it);
    }
    template <typename TIterator, typename TImage, typename TRegion> inline
    FixedIterator<TIterator> end(TImage const& i_, TRegion const& r_)
    {
        TIterator it(i_, r_);
        it.GoToEnd();
        return FixedIterator<TIterator>(it);
    }

}// namespace itk


const unsigned int Dimension = 2;
typedef unsigned short                              LabelType;
typedef float                                       PixelType;

typedef itk::Image<LabelType, Dimension>            LabelImageType;
typedef itk::ImageRegionIterator<LabelImageType>    LabelIteratorType;
// typedef itk::VectorImage<LabelType, Dimension>      LabelVectorImageType;
typedef otb::ImageList<LabelImageType>              LabelImageListType;

typedef itk::Image<PixelType, Dimension>            ImageType;
typedef itk::ImageRegionIterator<ImageType>         IteratorType;

typedef itk::Image<LabelType, Dimension>            OutLabelImageType;
typedef otb::ImageList<OutLabelImageType>           OutLabelImageListType;


static LabelImageListType::Pointer imageList;

static
LabelImageListType::Pointer make_image_list(bool verbose)
{
    LabelImageType::IndexType start;
    start[0] =   0;  // first index on X
    start[1] =   0;  // first index on Y

    LabelImageType::SizeType size;
    size[0]  = 200;  // size along X
    size[1]  = 200;  // size along Y

    imageList = LabelImageListType::New();
    for (unsigned short i=0; i!=4 ; ++i)
    {
        LabelImageType::RegionType region;
        region.SetSize(size);
        region.SetIndex(start);

        LabelImageType::Pointer image = LabelImageType::New();
        image->SetRegions(region);
        image->Allocate();

        itk::FixedIterator<LabelIteratorType> it  = itk::begin<LabelIteratorType>(image, region);
        itk::FixedIterator<LabelIteratorType> end = itk::end  <LabelIteratorType>(image, region);
        for (std::size_t n=0; it != end; ++it, ++n)
        {
            // i = 0 => [0:100  , 0:100]   <- i
            // i = 1 => [0:100  , 100:200] <- i
            // i = 2 => [100:200, 0:100]   <- i
            // i = 3 => [100:200, 100:200] <- i
            // =>
            // col <- i:even -> [0:100],  i:odd -> [100:200]
            // lig <- i<2    -> [0:100],  i>=2  -> [100:200]
            const std::size_t col = n % 200;
            const std::size_t lig = n / 200;
            const bool        set
                =  ((i % 2 == 0) ^ (col >= 100))
                && ((i     <= 1) ^ (lig >= 100))
                ;
            *it = set ? i+1 : 0;
        }

        imageList->PushBack(image);

        if (verbose) {
            typedef itk::ImageFileWriter< LabelImageType  > WriterType;
            WriterType::Pointer writer = WriterType::New();

            char filename[512];
            (void)snprintf(filename, sizeof(filename), "input-%d.tiff", i);
            writer->SetFileName(filename);
            writer->SetInput(image);
            writer->Update();
            std::cout << filename << " written" << std::endl;
        }
    }

    return imageList;
}

static
int test_image_list(LabelImageListType & images)
{
    std::cout << "Testing images produced..." << std::flush;
    LabelImageType::RegionType region;
    region.SetSize(images.GetNthElement(0)->GetLargestPossibleRegion().GetSize());

    vns::ImageListPixelIterator<LabelIteratorType> it(images, region);

    bool isOK = true;
    for ( std::size_t n=0; !it.IsAtEnd() ; ++it, ++n)
    {
        const std::size_t col = n % 200;
        const std::size_t lig = n / 200;
        const bool        set0 = (col <  100) && (lig <  100);
        const bool        set1 = (col >= 100) && (lig <  100);
        const bool        set2 = (col <  100) && (lig >= 100);
        const bool        set3 = (col >= 100) && (lig >= 100);
        if ((*it)[0] != (set0 ? 1 : 0)) {
            if (isOK) { std::cout << "\n"; }
            std::cout << " - Pixel0[" << lig << "][" << col << "]=" << ((*it)[0]) << "  != expected " << (set0 ? 1 : 0) << "\n";
            isOK = false;
        }
        if ((*it)[1] != (set1 ? 2 : 0)) {
            if (isOK) { std::cout << "\n"; }
            std::cout << " - Pixel1[" << lig << "][" << col << "]=" << ((*it)[1]) << "  != expected " << (set1 ? 2 : 0) << "\n";
            isOK = false;
        }
        if ((*it)[2] != (set2 ? 3 : 0)) {
            if (isOK) { std::cout << "\n"; }
            std::cout << " - Pixel2[" << lig << "][" << col << "]=" << ((*it)[2]) << "  != expected " << (set2 ? 3 : 0) << "\n";
            isOK = false;
        }
        if ((*it)[3] != (set3 ? 4 : 0)) {
            if (isOK) { std::cout << "\n"; }
            std::cout << " - Pixel3[" << lig << "][" << col << "]=" << ((*it)[3]) << "  != expected " << (set3 ? 4 : 0) << "\n";
            isOK = false;
        }
    }
    if (isOK) std::cout << " OK";
    std::cout << std::endl;

    return isOK ? EXIT_SUCCESS : EXIT_FAILURE;
}


int vnsImageListPixelIterator(int /*argc*/, char* /*argv*/[])
{
    try
    {
        bool verbose = false;
        LabelImageListType::Pointer masks = make_image_list(verbose);
        return test_image_list(*masks);
    }
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return EXIT_FAILURE;
}
