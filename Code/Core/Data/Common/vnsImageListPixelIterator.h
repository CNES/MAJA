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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 25 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsImageListPixelIterator_h__
#define vnsImageListPixelIterator_h__

#include "otbImageList.h"
#include "otbImageListToImageListFilter.h"

namespace vns
{

    /** Proxy class to emulate a reference to a pixel behind an ITK iterator.
     * \tparam TIterator an ITK pixel iterator
     * \author CS Systemes d'Information
     * \ingroup Core
     */
    template<typename TIterator>
        struct PixelReference
        {
            typedef TIterator IteratorType;
            typedef typename IteratorType::PixelType PixelType;
            explicit
            PixelReference(IteratorType & it) :
                    m_Iterator(it)
            {
            }
            PixelReference&
            operator=(PixelType const& p)
            {
                m_Iterator.Set(p);
                return *this;
            }

            operator PixelType() const
            {
                return m_Iterator.Get();
            }
        private:
            IteratorType & m_Iterator;
        };

// Forward declaration
    template<typename TImage>
        class ImageListPixelIterator;

    /** Proxy class to emulate a pixel in a list of images.
     * This class is not meant to be created manually, but from a \c
     * ImageListPixelIterator.
     *
     * \warning The pixel is bound to an iterator. Moving the iterator will move the pixel.
     *
     * \tparam TIterator an ITK pixel iterator
     * \invariant All images from the list shall have the same size
     *
     * \author CS Systemes d'Information
     * \ingroup Core
     * \sa \c ImageListPixelIterator
     */
    template<typename TImageIterator>
        class ImageListPixel
        {
        public:
            typedef TImageIterator SubIteratorType;
            typedef typename SubIteratorType::ImageType ImageType;
            typedef otb::ImageList<ImageType> ImageListType;

            typedef ImageListPixelIterator<SubIteratorType> PixelIteratorType;
            typedef typename ImageType::PixelType SubPixelType;
            typedef PixelReference<SubIteratorType> SubPixelReference;

            /** Init constructor. */
            explicit
            ImageListPixel(PixelIteratorType &iterator) :
                    m_Iterator(iterator)
            {
            }

            /** Assigns a new value to the pixel. */
            ImageListPixel&
            operator=(ImageListPixel const& rhs)
            {
                const std::size_t N = Size();
                itkAssertInDebugAndIgnoreInReleaseMacro(N == rhs.Size());
                for (std::size_t i = 0; i != N; ++i)
                {
                    (*this)[i] = rhs[i];
                    itkAssertInDebugAndIgnoreInReleaseMacro((*this)[i] == rhs[i]);
                }
                return *this;
            }
            /** Assigns a same value to all component of the pixel. */
            ImageListPixel&
            operator=(SubPixelType const& rhs)
            {
                const std::size_t N = Size();
                for (std::size_t i = 0; i != N; ++i)
                {
                    (*this)[i] = rhs;
                    itkAssertInDebugAndIgnoreInReleaseMacro((*this)[i] == rhs);
                }
                return *this;
            }
            /** Assigns and convert on-the-fly a new value to the pixel. */
            template<typename TImageIterator2>
                ImageListPixel&
                operator=(ImageListPixel<TImageIterator2> const& rhs)
                {
                    const std::size_t N = Size();
                    itkAssertInDebugAndIgnoreInReleaseMacro(N == rhs.Size());
                    for (std::size_t i = 0; i != N; ++i)
                    {
                        (*this)[i] = static_cast<typename TImageIterator2::PixelType>(rhs[i]);
                        itkAssertInDebugAndIgnoreInReleaseMacro((*this)[i] == rhs[i]);
                    }
                    return *this;
                }
            // TODO add assignement from VariableLengthPixel, etc

            /// Size accessor.
            std::size_t
            Size() const
            {
                return m_Iterator.Size();
            }

            // TODO: enable if TImage in not const
            /// R/W accessor to n-th component of the pixel
            SubPixelReference
            operator[](std::size_t idx)
            {
                return SubPixelReference(m_Iterator.m_Iterators[idx]);
            }
            /// Read-Only accessor to n-th component of the pixel
            SubPixelType
            operator[](std::size_t idx) const
            {
                return m_Iterator.m_Iterators[idx].Get();
            }

        private:
            /// Link to bound iterator.
            PixelIteratorType & m_Iterator;
        };

    /** Pixels iterator over ImageList.
     * This class assumes that all images in the ImageList have the
     * exact same size.
     *
     * \invariant There must be subimages
     * \invariant All subimages have the same size.
     * \invariant There are as many sub-iterator and sub-images
     * \author CS Systemes d'Information
     * \ingroup Core
     * \sa \c ImageListPixel
     */
    template<typename TImageIterator>
        class ImageListPixelIterator
        {
        public:
            /**@name Typedefs */
            //@{
            typedef TImageIterator SubIteratorType;
            typedef typename SubIteratorType::ImageType ImageType;
            typedef typename ImageType::RegionType RegionType;
            typedef otb::ImageList<ImageType> ImageListType;
            typedef ImageListPixel<SubIteratorType> PixelReferenceType;
            //@}

            friend class ImageListPixel<SubIteratorType> ;

            // template <typename TRegionType>
            ImageListPixelIterator(ImageListType & imageList, RegionType const& region) :
                    m_ImageList(imageList), m_Pixel(*this)
            {
                itkAssertInDebugAndIgnoreInReleaseMacro(AreInvariantsChecked() == true);

                // build the list of iterators
                // each sub-iterator will iterate of a sub-image
                typename ImageListType::ConstIterator b = m_ImageList.Begin();
                typename ImageListType::ConstIterator e = m_ImageList.End();
                while (b != e)
                {
                    typename ImageType::Pointer subimage = b.Get();
                    itkAssertInDebugAndIgnoreInReleaseMacro(
                            subimage->GetBufferPointer() && "Cannot create an iterator to an image w/ null buffer");
                    SubIteratorType it(subimage, region);
                    m_Iterators.push_back(it);
                    // Increment iterator
                    ++b;
                }
                // the number of components shall match for iterators and images
                itkAssertInDebugAndIgnoreInReleaseMacro(m_Iterators.size() == m_ImageList.Size());
            }

            /// Size accessor.
            std::size_t
            Size() const
            {
                return m_Iterators.size();
            }

            // TODO: enable if ImageType in not const
            /// R/W Accessor to current pixel -- STL syntax
            PixelReferenceType &
            operator*()
            {
                return m_Pixel;
            }
            /// Read-Only Accessor to current pixel -- STL syntax
            PixelReferenceType const&
            operator*() const
            {
                return m_Pixel;
            }

            // TODO: add accessors à la ITK: Set/Get, need a ImageListPixelType (likely
            // a VariableLengthVector)

            // TODO: add IsAtEndOfLine(), NextLine() for
            // itk::ImageScanLineConstIterator
            /// Tells whether all iterators have reach the end of the images
            bool
            IsAtEnd() const
            {
                const bool res = m_Iterators.front().IsAtEnd();
#if ! defined(NDEBUG) // invariant check in debug mode
                // All iterators shall reach the end of their respective images at the
                // same time
                typename IteratorSubListType::const_iterator b = m_Iterators.begin();
                typename IteratorSubListType::const_iterator e = m_Iterators.end();
                while (b != e)
                {
                    itkAssertInDebugAndIgnoreInReleaseMacro(res == b->IsAtEnd());
                    // Increment iterator
                    ++b;
                }
#endif
                return res;
            }

            /// Pre-increment operator => we don't want post-increment
            ImageListPixelIterator &
            operator++()
            {
                Increment();
                return *this;
            }

            /** Equality test.
             * \pre Sub iterators shall either be all different or all equal.
             */
            friend bool
            operator==(ImageListPixelIterator const& lhs, ImageListPixelIterator const& rhs)
            {
                itkAssertInDebugAndIgnoreInReleaseMacro(lhs.size() == rhs.size());
                const bool res = lhs.m_Iterators.Front() == rhs.m_Iterators.Front();
#if ! defined(NDEBUG) // invariant check in debug mode
                typename ImageListType::ConstIterator lb = lhs.m_ImageList.begin();
                typename ImageListType::ConstIterator le = lhs.m_ImageList.end();
                typename ImageListType::ConstIterator rb = rhs.m_ImageList.begin();
                typename ImageListType::ConstIterator re = rhs.m_ImageList.end();
                while (lb != le)
                {
                    itkAssertInDebugAndIgnoreInReleaseMacro(res == (*lb == *rb));
                    ++lb;
                    ++rb;
                }
#endif
                return res;
            }

            /// Difference test.
            friend bool
            operator!=(ImageListPixelIterator const& lhs, ImageListPixelIterator const& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            typedef std::vector<SubIteratorType> IteratorSubListType;

            /// Internal function to check class invariants
            bool
            AreInvariantsChecked() const
            {
                const std::size_t N = m_ImageList.Size();
                if (N == 0)
                {
                    return false;
                }

                const typename ImageType::SizeType refSize = m_ImageList.Front()->GetLargestPossibleRegion().GetSize();
                typename ImageListType::ConstIterator b = m_ImageList.Begin();
                typename ImageListType::ConstIterator e = m_ImageList.End();
                while (b != e)
                {
                    if (refSize != b.Get()->GetLargestPossibleRegion().GetSize())
                    {
                        return false;
                    }
                    // Increment iterator
                    ++b;
                }
                return true;
            }

            /// Increment the sub-iterators to the next pixel.
            void
            Increment()
            {
                typename IteratorSubListType::iterator b = m_Iterators.begin();
                typename IteratorSubListType::iterator e = m_Iterators.end();
                while (b != e)
                {
                    SubIteratorType & itPix = *b;
                    ++itPix;
                    // Increment iterator
                    ++b;
                }
            }

            /// Reference to the image list iterated
            ImageListType & m_ImageList;
            /// Internal all of sub-iterators
            IteratorSubListType m_Iterators;
            /// Fast way to access to a pixel
            PixelReferenceType m_Pixel;
        };

} // vns namespace

#endif // vnsImageListPixelIterator_h__
