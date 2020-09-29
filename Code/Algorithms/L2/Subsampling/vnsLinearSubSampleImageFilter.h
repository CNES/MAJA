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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsLinearSubSampleImageFilter_h
#define __vnsLinearSubSampleImageFilter_h

#include "vnsMacro.h"
#include "itkImageRegionIterator.h"
#include "itkImageToImageFilter.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNeighborhoodIterator.h"
#include "vnl/vnl_vector.h"


namespace vns
{

/** \class LinearSubSampleImageFilter
 * \brief Resample an image via a coordinate transform
 *
 * LinearSubSampleImageFilter resamples an existing image
 * interpolating via a Linear Interpolation. The class is templated
 * over the types of the input and output images.
 *
 * Input and output image are otb::Image.
 * The sample ratio is given through the method SetSubsampleFactor(unsigned int ratio);
 * It has to be an unsigned integer. A value of 1 returns the input image, 0 generates an exception.
 * Output image information (spacing, size and direction) are deducted
 * usign the sample ratio. Origin and largest possible region index is a
 * copy of the input image one.
 *
 * This filter is implemented as a multithreaded filter.  It provides a 
 * ThreadedGenerateData() method for its implementation.
 *
 * \sa itkResampleImageFilter
 * \sa itkLinearInterpolationImageFilter
 */

template <class TInputImage, class TOutputImage>
  class LinearSubSampleImageFilter: public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LinearSubSampleImageFilter                           Self;
  typedef itk::ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  typedef TInputImage                             InputImageType;
  typedef TOutputImage                            OutputImageType;
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef typename OutputImageType::Pointer       OutputImagePointer;
  typedef typename InputImageType::RegionType     InputImageRegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(LinearSubSampleImageFilter, itk::ImageToImageFilter);

  /** Number of dimensions. */
  itkStaticConstMacro(ImageDimension, unsigned int, TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);


  /** Image size typedef. */
  typedef typename InputImageType::SizeType SizeType;

  /** Image index typedef. */
  typedef typename TOutputImage::IndexType  IndexType;

  /** Image pixel value typedef. */
  typedef typename TOutputImage::PixelType         PixelType;
  typedef typename TInputImage::PixelType          InputPixelType;
  
  /** Typedef to describe the output image region type. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** Image spacing,origin and direction typedef */
  typedef typename TOutputImage::SpacingType   SpacingType;
  typedef typename TOutputImage::PointType     OriginPointType;
  typedef typename TOutputImage::DirectionType DirectionType;

  /** Face list typedefs */
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> NeighborhoodAlgorithmType;
  typedef typename NeighborhoodAlgorithmType::FaceListType                                  FaceListType;
  typedef typename FaceListType::iterator                                                   FaceListIteratorType;
  typedef typename itk::NeighborhoodIterator<InputImageType>                                NeighborhoodTypeIterator;
  typedef typename NeighborhoodTypeIterator::OffsetType                                     OffsetType;

  /** List typedefs */
  typedef typename std::vector<OffsetType>   OffsetListType;
  typedef typename std::vector<double>       DoubleVectorType;
  typedef typename std::vector<unsigned int> UIVectorType;

  /** Coeficients container type.*/
  typedef vnl_vector<double> CoefContainerType;

  /** base type for images of the current ImageDimension */
  typedef itk::ImageBase<itkGetStaticConstMacro(ImageDimension)> ImageBaseType;
  
  /** Set the size of the output image. */
  itkGetConstReferenceMacro( Size, SizeType );

  /** Get the output image spacing. */
  itkGetConstReferenceMacro( OutputSpacing, SpacingType );

  /** Get the output image origin. */
  itkGetConstReferenceMacro( OutputOrigin, OriginPointType );

  /** Set the output direciton cosine matrix. */
  itkGetConstReferenceMacro( OutputDirection, DirectionType );

  /** Get the start index of the output largest possible region. */
  itkGetConstReferenceMacro( OutputStartIndex, IndexType );

  /** Set/Get Sample ratio. */
  itkSetMacro( SubsampleFactor, unsigned int );
  itkGetConstReferenceMacro( SubsampleFactor, unsigned int );

  /** Get the used Linear coefficients. */
  vnsGetConstMacro(LinearCoef, CoefContainerType );


  /** LinearSubSampleImageFilter produces an image which is a different size
   * than its input.  As such, it needs to provide an implementation
   * for GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below. \sa ProcessObject::GenerateOutputInformaton() */
  virtual void GenerateOutputInformation( void );


protected:
  LinearSubSampleImageFilter( void );
  virtual ~LinearSubSampleImageFilter( void ) {};

  void PrintSelf( std::ostream& os, itk::Indent indent ) const;

  virtual void BeforeThreadedGenerateData();

  /** LinearSubSampleImageFilter can be implemented as a multithreaded filter.  Therefore,
   * this implementation provides a ThreadedGenerateData() routine which
   * is called for each processing thread. The output image data is allocated
   * automatically by the superclass prior to calling ThreadedGenerateData().
   * ThreadedGenerateData can only write to the portion of the output image
   * specified by the parameter "outputRegionForThread"
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread,
                             itk::ThreadIdType threadId );


  /** Since input and output image are very likely to be of different size.
   * Region estimation functions has to be reimplemented
   */
  virtual void CallCopyOutputRegionToInputRegion
    ( InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion );
  virtual void CallCopyInputRegionToOutputRegion
    ( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion );

  /** LinearSubSampleImageFilter needs a different input requested region than
   * the output requested region.  As such, LinearSubSampleImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   * \sa ProcessObject::GenerateInputRequestedRegion() */
    virtual void GenerateInputRequestedRegion(void);

    /** Compute the Linear coefficients. */
    void EvaluateCoef();


private:
  LinearSubSampleImageFilter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  /** Size of the output image */
  SizeType                m_Size;         
  /** Output image spacing */
  SpacingType             m_OutputSpacing;   
  /** Output image origin */
  OriginPointType         m_OutputOrigin;     
  /** Output image direction cosines */
  DirectionType           m_OutputDirection;   
  // Output image start index */
  IndexType               m_OutputStartIndex;  
   /** Used Linear coefficiet */   
  CoefContainerType       m_LinearCoef;
  /** Sample Ratio */
  unsigned int            m_SubsampleFactor; 
};

  
} // end namespace vns
  
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsLinearSubSampleImageFilter.txx"
#endif
  
#endif

