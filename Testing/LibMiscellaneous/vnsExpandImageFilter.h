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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsExpandImageFilter_h
#define __vnsExpandImageFilter_h

#include "itkExpandImageFilter.h"
#include "vnsLinearInterpolateImageFunction.h"

namespace vns
{

/** \class ExpandImageFilter
 * \brief Simple overload of the GeneratenputRequestedRegion 
 * of the itkEExpandImageFilter.
 *
 * \sa InterpolateImageFunction
 * \sa LinearInterpolationImageFunction
 * \sa VectorExpandImageFilter
 *
 * \ingroup GeometricTransform
 */
template <class TInputImage, class TOutputImage>
class ExpandImageFilter:
  public itk::ExpandImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ExpandImageFilter                             Self;
  typedef itk::ExpandImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExpandImageFilter, itk::ExpandImageFilter);

  /** Typedef to describe the output image region type. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** ImageDimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Inherit some types from superclass. */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename Superclass::InputImagePointer     InputImagePointer;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;

  /** Typedef support for the interpolation function. */
  typedef typename Superclass::CoordRepType CoordRepType;
  typedef typename Superclass::InterpolatorType  InterpolatorType;
  typedef typename Superclass::InterpolatorPointer  InterpolatorPointertypename;
  typedef typename Superclass::DefaultInterpolatorType DefaultInterpolatorType;

  /** Linear Interpolator used as default value */
  typedef LinearInterpolateImageFunction<InputImageType, CoordRepType> LinearInterpolatorType;
  typedef typename LinearInterpolatorType::Pointer LinearInterpolatorPointer;

  /** ExpandImageFilter needs a smaller input requested region than the output
   * requested region.  As such, ShrinkImageFilter needs to provide an 
   * implementation for GenerateInputRequestedRegion() in order to inform 
   * the pipeline execution model.  
   * \sa ProcessObject::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion();    
    
  
  /** Set the edge padding value. The default is zero. */
  itkSetMacro( EdgePaddingValue, OutputPixelType );
  /** Get the edge padding value */
  itkGetConstMacro( EdgePaddingValue, OutputPixelType );


protected:
  ExpandImageFilter();
  virtual ~ExpandImageFilter() {};

  /** ExpandVectorImageFilter is implemented as a multithreaded filter.  Therefore,
   * this implementation provides a ThreadedGenerateData() routine which
   * is called for each processing thread. The output image data is allocated
   * automatically by the superclass prior to calling ThreadedGenerateData().
   * ThreadedGenerateData can only write to the portion of the output image
   * specified by the parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  virtual
    void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                              itk::ThreadIdType threadId );

private:
  ExpandImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputPixelType        m_EdgePaddingValue;
 
};

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsExpandImageFilter.txx"
#endif

#endif
