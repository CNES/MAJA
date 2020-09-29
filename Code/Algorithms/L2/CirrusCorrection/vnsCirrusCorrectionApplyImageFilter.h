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
 *                                        __  __    __     ____   __                                        *
 *                                       (  \/  )  /__\   (_  _) /__\                                       *
 *                                        )    (  /(__)\ .-_)(  /(__)\                                      *
 *                                       (_/\/\_)(__)(__)\____)(__)(__)                                     *
 *                                                                                                          *
 *                                                                                                          *
 ************************************************************************************************************
 *                                                                                                          *
 * Author: CS Systemes d'Information (France)                                                               *
 *                                                                                                          *
 ************************************************************************************************************
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 3.3.0 : DM : LAIG-DM-MAJA-3044-CNES : 15 mars 2019 : Integration modifications oliver hagolle  *
 * VERSION : 2.1.0 : FA : LAIG-FA-MAJA-157185-CS : 9 mai 2017 : Correction de règles qualité                *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2149-CNES : 24 fevrier 2017 : Correction Cirrus - création           *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$                                                                                                     *
 *                                                                                                          *
 ************************************************************************************************************/


#ifndef __vnsCirrusCorrectionApplyImageFilter_h
#define __vnsCirrusCorrectionApplyImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "vnsMacro.h"
#include "vnsUtilities.h"

namespace vns
{

/** \class CirrusCorrectionApplyImageFilter
 * \brief Implements cirrus correction application on TOA image.
 *
 *
 *
 */
template <class TInputImageTOA, class TInputImageRefl,
    class TInputImageMask, class TInputImageDist,
    class TOutputImage>
class CirrusCorrectionApplyImageFilter :
  public itk::InPlaceImageFilter<TInputImageTOA, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef CirrusCorrectionApplyImageFilter                        Self;
  typedef itk::InPlaceImageFilter<TInputImageTOA, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CirrusCorrectionApplyImageFilter, InPlaceImageFilter);

  /** Some typedefs. */
  typedef TInputImageTOA                           InputTOAImageType;
  typedef typename InputTOAImageType::ConstPointer InputTOAImagePointer;
  typedef typename InputTOAImageType::RegionType   InputTOAImageRegionType;
  typedef typename InputTOAImageType::PixelType    InputTOAImagePixelType;
  typedef TInputImageRefl                          InputReflImageType;
  typedef typename InputReflImageType::ConstPointer InputReflImagePointer;
  typedef typename InputReflImageType::RegionType   InputReflImageRegionType;
  typedef typename InputReflImageType::PixelType    InputReflImagePixelType;
  typedef TInputImageMask                           InputMaskImageType;
  typedef typename InputMaskImageType::ConstPointer InputMaskImagePointer;
  typedef typename InputMaskImageType::RegionType   InputMaskImageRegionType;
  typedef typename InputMaskImageType::PixelType    InputMaskImagePixelType;
  typedef TInputImageDist                           InputDistImageType;
  typedef typename InputDistImageType::ConstPointer InputDistImagePointer;
  typedef typename InputDistImageType::RegionType   InputDistImageRegionType;
  typedef typename InputDistImageType::PixelType    InputDistImagePixelType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType OutputImageInternalPixelType;

  /** TOA Corrected image. */
  vnsSetGetInputMacro( TOA, InputTOAImageType, 0)

  /** Cirrus Reflectance on center. */
  vnsSetGetInputMacro( ReflCenter, InputReflImageType, 1)

  /** Cirrus reflectance on edge. */
  vnsSetGetInputMacro( ReflEdge, InputReflImageType, 2)

  /** Cirrus Mask. */
  vnsSetGetInputMacro( CirrusMask, InputMaskImageType, 3)

  /** Cirrus Maurer Dist. */
  vnsSetGetInputMacro( CirrusDist, InputDistImageType, 4)


  /** Set gamma */
  itkSetMacro(Gamma, double);

  /** Set SWIR factor */
  itkSetMacro(SWIRGammaFactor, double);




  /** Set full list of band code    */
  vnsSetDirectMacro(ListOfBandCode, Utilities::ListOfStrings);

  /** Set SWIR list of band code    */
  vnsSetDirectMacro(ListOfSWIRBandCode, Utilities::ListOfStrings);

  itkSetMacro(ForegroundMaskValue,InputMaskImagePixelType)
  itkGetMacro(ForegroundMaskValue,InputMaskImagePixelType)

  itkSetMacro(MaxReflectance, double)

  /** Image dimensions */
  itkStaticConstMacro(InputTOAImageDimension, unsigned int,
                      TInputImageTOA::ImageDimension);
  itkStaticConstMacro(InputReflImageDimension, unsigned int,
                      TInputImageRefl::ImageDimension);
  itkStaticConstMacro(InputMaskImageDimension, unsigned int,
                      TInputImageMask::ImageDimension);
  itkStaticConstMacro(InputDistImageDimension, unsigned int,
                      TInputImageDist::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);



protected:
  CirrusCorrectionApplyImageFilter();
  virtual ~CirrusCorrectionApplyImageFilter() {}

  /** Validate the presence of all three inputs. If one or more inputs
   * are missing, throw an exception. */
  void BeforeThreadedGenerateData();

  /** QuaternaryFunctorImageFilter can be implemented as a multithreaded filter.
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
                            itk::ThreadIdType threadId);

  double m_Gamma;
  double m_SWIRGammaFactor;

  Utilities::ListOfStrings m_ListOfBandCode;
  Utilities::ListOfStrings m_ListOfSWIRBandCode;
  InputMaskImagePixelType m_ForegroundMaskValue;

  double m_MaxReflectance;


private:
  CirrusCorrectionApplyImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented



};

} // end namespace vns

#ifndef OTB_MANUAL_INSTANTIATION
#include "vnsCirrusCorrectionApplyImageFilter.txx"
#endif

#endif
