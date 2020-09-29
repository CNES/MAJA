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
 * VERSION : 5.1.0 : DM : LAIG-FA-MAC-145739-CS : 9 aout 2016 : Correction qualité sur MACCS 5.1            *
 * VERSION : 5.1.0 : <TypeFT> : <NumFT> : 22 juillet 2016 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsFastLinearInterpolateImageFunction.h 12703 2016-07-22 14:12:29Z abricier $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsFastLinearInterpolateImageFunction_h
#define __vnsFastLinearInterpolateImageFunction_h

#include "itkInterpolateImageFunction.h"
#include "vnl/vnl_vector.h"
#include "otbMath.h"

#include "otbVectorImage.h"

namespace vns
{
/** \class FastLinearInterpolateImageFunction
 *  \brief Interpolate an image at specified positions using bilinear interpolation.
 *
 * FastLinearInterpolateImageFunction interpolates image intensity at
 * a non-integer pixel position. This class is templated
 * over the input image type and the coordinate representation type
 * (e.g. float or double).
 *
 * This function works for 2-dimensional images.
 *
 * This function works with both Images and VectorImages.
 *
 * Parameters are the interpolation window radius and the bicubic
 * optimisation coefficient alpha.
 * Alpha is usually set to -0.5, -0.75 or -1 (-0.5 by default).
 * The case alpha = -0.5 (which corresponds to the cubic Hermite
 * spline) is known to produce the best approximation of the original
 * function.
 *
 * \ingroup ImageFunctions ImageInterpolators
 */
template< class TInputImage, class TCoordRep = double >
class FastLinearInterpolateImageFunctionBase :
  public itk::InterpolateImageFunction<TInputImage, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef FastLinearInterpolateImageFunctionBase                      Self;
  typedef itk::InterpolateImageFunction<TInputImage, TCoordRep> Superclass;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FastLinearInterpolateImageFunctionBase, InterpolateImageFunction);

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** InputImageType typedef support. */
  typedef typename Superclass::InputImageType InputImageType;

  /** InputPixelType typedef support. */
  typedef typename Superclass::InputPixelType InputPixelType;

  /** RealType typedef support. */
  typedef typename Superclass::RealType RealType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Index typedef support. */
  typedef typename Superclass::IndexType      IndexType;
  typedef typename Superclass::IndexValueType IndexValueType;

  /** Point typedef support. */
  typedef typename Superclass::PointType      PointType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef TCoordRep                                ContinuousIndexValueType;

  /** Evaluate the function at a ContinuousIndex position
   *
   * Returns the linearly interpolated image intensity at a
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const = 0;

protected:
  FastLinearInterpolateImageFunctionBase() {};
  virtual ~FastLinearInterpolateImageFunctionBase() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Number of neighbors used in the interpolation */
  static const unsigned long m_Neighbors;

private:
  FastLinearInterpolateImageFunctionBase( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

};


template < class TInputImage, class TCoordRep = double >
class FastLinearInterpolateImageFunction :
    public FastLinearInterpolateImageFunctionBase< TInputImage, TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef FastLinearInterpolateImageFunction                              Self;
  typedef FastLinearInterpolateImageFunctionBase<TInputImage, TCoordRep>   Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  itkTypeMacro(FastLinearInterpolateImageFunction, FastLinearInterpolateImageFunctionBase);
  itkNewMacro(Self);
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  typedef typename Superclass::OutputType            OutputType;
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputPixelType        InputPixelType;
  typedef typename Superclass::RealType              RealType;
  typedef typename Superclass::IndexType             IndexType;
  typedef typename Superclass::IndexValueType        IndexValueType;
  typedef typename Superclass::PointType             PointType;
  typedef typename Superclass::ContinuousIndexType   ContinuousIndexType;

  virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const;

protected:
  FastLinearInterpolateImageFunction() {};
  virtual ~FastLinearInterpolateImageFunction() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  FastLinearInterpolateImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented
};


template < typename TPixel, unsigned int VImageDimension, class TCoordRep >
class FastLinearInterpolateImageFunction< otb::VectorImage<TPixel, VImageDimension> , TCoordRep > :
    public FastLinearInterpolateImageFunctionBase< otb::VectorImage<TPixel, VImageDimension> , TCoordRep >
{
public:
  /** Standard class typedefs.*/
  typedef FastLinearInterpolateImageFunction                              Self;
  typedef FastLinearInterpolateImageFunctionBase
      < otb::VectorImage<TPixel, VImageDimension>, TCoordRep >      Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  itkTypeMacro(FastLinearInterpolateImageFunction, FastLinearInterpolateImageFunctionBase);
  itkNewMacro(Self);
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  typedef typename Superclass::OutputType            OutputType;
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputPixelType        InputPixelType;
  typedef typename Superclass::RealType              RealType;
  typedef typename Superclass::IndexType             IndexType;
  typedef typename Superclass::IndexValueType        IndexValueType;
  typedef typename Superclass::PointType             PointType;
  typedef typename Superclass::ContinuousIndexType   ContinuousIndexType;

  virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const;

protected:
  FastLinearInterpolateImageFunction() {};
  virtual ~FastLinearInterpolateImageFunction() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  FastLinearInterpolateImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented
};

} // end namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsFastLinearInterpolateImageFunction.txx"
#endif

#endif
