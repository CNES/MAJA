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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 30 mars 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsBinaryThresholdVectorImageFilter_h
#define __vnsBinaryThresholdVectorImageFilter_h

#include "itkImageToImageFilter.h"

namespace vns
{

/** \class BinaryThresholdVectorImageFilter
 * \brief Set image values to a user-specified value if they are below, 
 * above, or between simple threshold values.
 *
 * BinaryThresholdVectorImageFilter sets image values to a user-specified "outside"
 * value (by default, "black") if the image values are below, above, or
 * between simple threshold values. 
 *
 * The pixels must support the operators >= and <=.
 * 
 * \ingroup IntensityImageFilters Multithreaded
 */
  template <class TInputImage, class TOutputImage=TInputImage>
  class BinaryThresholdVectorImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryThresholdVectorImageFilter               Self;
  typedef itk::ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryThresholdVectorImageFilter, itk::ImageToImageFilter);


  /** Some additional typedefs.  */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::InternalPixelType    InputImageInternalPixelType;

  /** Some additional typedefs.  */
  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType   OutputImageInternalPixelType;

  /** Set the "outside" pixel value. The default value 
   * NumericTraits<PixelType>::Zero. */
  itkSetMacro(OutsideValue,OutputImageInternalPixelType);
  
  /** Get the "outside" pixel value. */
  itkGetConstMacro(OutsideValue,OutputImageInternalPixelType);

  /** Set the "inside" pixel value. The default value
   * NumericTraits<PixelType>::One. */
  itkSetMacro(InsideValue,OutputImageInternalPixelType);

  /** Get the "outside" pixel value. */
  itkGetConstMacro(InsideValue,OutputImageInternalPixelType);

                 
  /** The values greater than or equal to the value are set to OutsideValue. */
  void ThresholdAbove(const InputImageInternalPixelType &thresh);
  
  /** The values less than or equal to the value are set to OutsideValue. */
  void ThresholdBelow(const InputImageInternalPixelType &thresh);

  /** The values outside the range are set to OutsideValue. */
  void ThresholdOutside(const InputImageInternalPixelType &lower, const InputImageInternalPixelType &upper);
  
  /** Set/Get methods to set the lower threshold */
  itkSetMacro(Lower, InputImageInternalPixelType);
  itkGetConstMacro(Lower, InputImageInternalPixelType);

  /** Set/Get methods to set the upper threshold */
  itkSetMacro(Upper, InputImageInternalPixelType);
  itkGetConstMacro(Upper, InputImageInternalPixelType);


protected:
  BinaryThresholdVectorImageFilter();
  virtual ~BinaryThresholdVectorImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** BinaryThresholdVectorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId );

  void GenerateOutputInformation(void)
  {
    Superclass::GenerateOutputInformation();
    
    this->GetOutput()->SetNumberOfComponentsPerPixel( this->GetInput()->GetNumberOfComponentsPerPixel() );
  }

private:
  BinaryThresholdVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImageInternalPixelType m_InsideValue;
  OutputImageInternalPixelType m_OutsideValue;
  InputImageInternalPixelType m_Lower;
  InputImageInternalPixelType m_Upper;
};

  
} // end namespace vns
  
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsBinaryThresholdVectorImageFilter.txx"
#endif
  
#endif
