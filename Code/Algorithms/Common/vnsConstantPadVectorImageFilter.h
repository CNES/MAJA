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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 2 août 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConstantPadVectorImageFilter_h
#define __vnsConstantPadVectorImageFilter_h

#include "itkPadImageFilter.h"

namespace vns
{

/** \class ConstantPadVectorImageFilter
 * \brief Increase the vector image size by padding with a scalar constant value.
 *
 * ConstantPadVectorImageFilter changes the output image region.  If the output
 * image region is larger than the input image region, the extra pixels are
 * filled in by a constant value.  The output image region must be specified.
 *
 * This filter is implemented as a multithreaded filter.  It provides a 
 * ThreadedGenerateData() method for its implementation.
 * 
 * \ingroup GeometricTransforms
 * \sa ConstantPadImageFilter, WrapPadImageFilter, MirrorPadImageFilter
 */
template <class TInputImage, class TOutputImage>
class ConstantPadVectorImageFilter:
    public itk::PadImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ConstantPadVectorImageFilter                    Self;
  typedef itk::PadImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self); 

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConstantPadVectorImageFilter, PadImageFilter);

  /** Typedef to describe the output image region type. */
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::InputImageRegionType  InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename Superclass::OutputImagePixelType OutputImagePixelType;
  typedef typename TOutputImage::InternalPixelType  OutputImageInternalPixelType;
  typedef typename Superclass::InputImagePixelType  InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename Superclass::OutputImageIndexType OutputImageIndexType;
  typedef typename Superclass::InputImageIndexType  InputImageIndexType;
  typedef typename Superclass::OutputImageSizeType  OutputImageSizeType;
  typedef typename Superclass::InputImageSizeType   InputImageSizeType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Set/Get the pad value.  Default is Zero. */
//  itkSetMacro(Constant, OutputImagePixelType);
//  itkGetConstMacro(Constant, OutputImagePixelType);
  itkSetMacro(ScalarConstant, OutputImageInternalPixelType);
  itkGetConstMacro(ScalarConstant, OutputImageInternalPixelType);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputEqualityComparableCheck,
    (itk::Concept::EqualityComparable<OutputImagePixelType>));
  itkConceptMacro(InputConvertibleToOutputCheck,
    (itk::Concept::Convertible<InputImagePixelType, OutputImagePixelType>));
  itkConceptMacro(SameDimensionCheck,
    (itk::Concept::SameDimension<ImageDimension, OutputImageDimension>));
  itkConceptMacro(OutputOStreamWritableCheck,
    (itk::Concept::OStreamWritable<OutputImagePixelType>));
  /** End concept checking */
#endif

protected:
  ConstantPadVectorImageFilter();
  virtual ~ConstantPadVectorImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  /** PadImageFilter can be implemented as a multithreaded filter.  Therefore,
   * this implementation provides a ThreadedGenerateData() routine which
   * is called for each processing thread. The output image data is allocated
   * automatically by the superclass prior to calling ThreadedGenerateData().
   * ThreadedGenerateData can only write to the portion of the output image
   * specified by the parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId );

  /**
   * Given an n dimensional list of output region breakpoints in indices
   * and size (where the current region and maximum region for each dimension
   * is encoded in regIndices and regLimit), choose the next output region.
   */ 
  virtual int GenerateNextRegion(long *regIndices, long *regLimit,
                         OutputImageIndexType *indices, 
                         OutputImageSizeType *sizes, 
                         OutputImageRegionType& outputRegion);
  
private:
  ConstantPadVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  virtual void BeforeThreadedGenerateData();

  OutputImageInternalPixelType m_ScalarConstant;
  OutputImagePixelType m_Constant;
};

  
} // end namespace vns
  
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsConstantPadVectorImageFilter.txx"
#endif
  
#endif
