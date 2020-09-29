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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsFastVectorInterpolateImageFunction.h 9359 2015-01-19 16:21:14Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsFastVectorInterpolateImageFunction_h
#define __vnsFastVectorInterpolateImageFunction_h

#include "itkObject.h"
#include "itkFixedArray.h"
#include "itkContinuousIndex.h"
#include "itkPoint.h"

namespace vns
{

/** \class FastVectorInterpolateImageFunction
 * \brief Base class for all vector image interpolaters.
 *
 * FastVectorInterpolateImageFunction is the base for all ImageFunctions that
 * interpolates image with vector pixel types. This function outputs
 * a return value of type Vector<double,Dimension>.
 *
 * This class is templated input image type and the coordinate
 * representation type.
 *
 * \warning This hierarchy of functions work only for images
 * with Vector-based pixel types. For scalar images use
 * InterpolateImageFunction.
 *
 * \sa InterpolateImageFunction
 * \ingroup ImageFunctions ImageInterpolators
 */
template<class TInputImage, class TCoordRep = double>
class FastVectorInterpolateImageFunction : public itk::Object
{
public:

    /** Dimension underlying input image. */
    itkStaticConstMacro(ImageDimension, unsigned int,
            TInputImage::ImageDimension);

    /** Standard class typedefs. */
    typedef FastVectorInterpolateImageFunction Self;
    typedef itk::Object Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Run-time type information (and related methods). */
    itkTypeMacro(FastVectorInterpolateImageFunction, Object)

    /** InputImageType typedef support. */
    /** CoordRep typedef support. */
    typedef TCoordRep CoordRepType;

    typedef TInputImage InputImageType;

    /** InputImagePointer typedef support */
    typedef typename InputImageType::ConstPointer InputImageConstPointer;

    typedef typename InputImageType::PixelType PixelType;
    typedef typename PixelType::ValueType ValueType;
    typedef typename itk::NumericTraits<ValueType>::RealType RealType;

    /** Output type is RealType of TInputImage::PixelType. */
    typedef typename itk::NumericTraits< typename TInputImage::PixelType>::RealType OutputType;

    /** ContinuousIndex Type. */
    typedef itk::ContinuousIndex< TCoordRep,
    itkGetStaticConstMacro(ImageDimension) > ContinuousIndexType;

    /** Point Type. */
    typedef itk::Point< TCoordRep, itkGetStaticConstMacro(ImageDimension) > PointType;

    /** Index Type. */
    typedef typename InputImageType::IndexType IndexType;
    typedef typename InputImageType::IndexValueType IndexValueType;

    /** Get the input image. */
    const InputImageType * GetInputImage() const
    {   return m_Image.GetPointer();}

    /** Check if an index is inside the image buffer.
     * We take into account the fact that each voxel has its
     * center at the integer coordinate and extends half way
     * to the next integer coordinate.
     * \warning For efficiency, no validity checking of
     * the input image is done. */
    bool IsInsideBuffer(const IndexType & index) const
    {
      for ( unsigned int j = 0; j < ImageDimension; j++ )
        {
        if ( index[j] < m_StartIndex[j] )
          {
          return false;
          }
        if ( index[j] > m_EndIndex[j] )
          {
          return false;
          }
        }
      return true;
    }

    /** Check if a continuous index is inside the image buffer.
     * \warning For efficiency, no validity checking of
     * the input image is done. */
    bool IsInsideBuffer(const ContinuousIndexType & index) const
    {
        for ( unsigned int j = 0; j < ImageDimension; j++ )
        {
            /* Test for negative of a positive so we can catch NaN's. */
            if ( ! (index[j] >= m_StartContinuousIndex[j] &&
                            index[j] < m_EndContinuousIndex[j] ) )
            {
                return false;
            }
        }
        return true;
    }

    /** Check if a point is inside the image buffer.
     * \warning For efficiency, no validity checking of
     * the input image pointer is done. */
    bool IsInsideBuffer(const PointType & point) const
    {
      ContinuousIndexType index;
      m_Image->TransformPhysicalPointToContinuousIndex(point, index);
      /* Call IsInsideBuffer to test against BufferedRegion bounds.
       * TransformPhysicalPointToContinuousIndex tests against
       * LargestPossibleRegion */
      return IsInsideBuffer( index );
    }

    virtual void
    SetInputImage(const InputImageType * ptr)
    {
        // ---------------------------------------------------------------------------------------------
        // set the input image
        this->m_Image = ptr;

        // ---------------------------------------------------------------------------------------------
        // If the input pointer is valid
        if (ptr)
        {
            typename InputImageType::SizeType size = ptr->GetBufferedRegion().GetSize();
            // ---------------------------------------------------------------------------------------------
            // set the start index
            this->m_StartIndex = ptr->GetBufferedRegion().GetIndex();

            for (unsigned int j = 0; j < ImageDimension; j++)
            {
                // ---------------------------------------------------------------------------------------------
                // set the end index
                this->m_EndIndex[j] = this->m_StartIndex[j] + static_cast<IndexValueType> (size[j]) - 1;
                this->m_StartContinuousIndex[j] = static_cast<CoordRepType>( this->m_StartIndex[j] - 0.5 );
                this->m_EndContinuousIndex[j] = static_cast<CoordRepType>( this->m_EndIndex[j] + 0.5 );
            }
        }
    }
    itkGetConstReferenceMacro(StartIndex, IndexType);
    itkGetConstReferenceMacro(EndIndex, IndexType);

    itkGetConstReferenceMacro(StartContinuousIndex, ContinuousIndexType);
    itkGetConstReferenceMacro(EndContinuousIndex, ContinuousIndexType);

protected:
    FastVectorInterpolateImageFunction()
    {
    }
    virtual
    ~FastVectorInterpolateImageFunction()
    {
    }
    void
    PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
    }

    /** Const pointer to the input image. */
    InputImageConstPointer m_Image;

    /** Cache some values for testing if indices are inside buffered region. */
    IndexType m_StartIndex;
    IndexType m_EndIndex;

    ContinuousIndexType m_StartContinuousIndex;
    ContinuousIndexType m_EndContinuousIndex;

private:
    FastVectorInterpolateImageFunction(const Self&); //purposely not implemented
    void
    operator=(const Self&);//purposely not implemented

};

}
 // end namespace vns

#endif
