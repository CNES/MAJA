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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConvolutionVectorImageFilter_h
#define __vnsConvolutionVectorImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class ConvolutionVectorImageFilter
     * \brief Applies a convolution filter to each channel of a vector image.
     *
     * Computes vector image which is the convolution of the input vector image
     * with a filter. (Band per band)
     *
     * This filter can be used with a mask.
     *
     * The radius of the input filter is provided by the \code SetInput() \endcode
     * method and the filters coefficients are given by an itk::Array passed to the
     * \code SetFilter() \endcode method.
     *
     * By default, the input filter is not normalized but it can be using the
     * NormalizeFilterOn() method.
     *
     * This filter allows the user to choose the boundary condtions in the template parameters.
     Default boundary conditions are zero flux Neumann boundary conditions.
     *
     * An optimized version of this filter using FFTW is available in the Orfeo ToolBox and
     * will significantly improves performances especially for large kernels
     * (see OverlapSaveConvolutionVectorImageFilter).
     *
     * \sa Image
     * \sa Neighborhood
     * \sa NeighborhoodOperator
     * \sa NeighborhoodIterator
     * \sa ImageBoundaryCondition
     * \sa ZeroFluxNeumannBoundaryCondition
     * \sa OverlapSaveConvolutionVectorImageFilter
     *
     * \ingroup IntensityImageFilters
     * \ingroup Streamed
     * \ingroup MultiThreaded
     */
    template<class TVectorInputImage, class TMask, class TVectorOutputImage,
            class TBoundaryCondition = itk::ZeroFluxNeumannBoundaryCondition<TVectorInputImage> >
        class ConvolutionVectorImageFilter : public itk::ImageToImageFilter<TVectorInputImage, TVectorOutputImage>
        {
        public:
            /** Extract dimension from input and output image. */
            itkStaticConstMacro(InputImageDimension, unsigned int,
                    TVectorInputImage::ImageDimension);
            itkStaticConstMacro(OutputImageDimension, unsigned int,
                    TVectorOutputImage::ImageDimension);

            /** Convenient typedefs for simplifying declarations. */
            typedef TVectorInputImage InputVectorImageType;
            typedef TVectorOutputImage OutputVectorImageType;

            /** Standard class typedefs. */
            typedef ConvolutionVectorImageFilter Self;
            typedef itk::ImageToImageFilter< InputVectorImageType, OutputVectorImageType> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self);

            /** Run-time type information (and related methods). */
            itkTypeMacro(ConvolutionVectorImageFilter, ImageToImageFilter);

            /** Image typedef support. */
            typedef TMask MaskType;
            typedef typename MaskType::Pointer MaskPointer;
            typedef typename MaskType::RegionType MaskRegionType;
            typedef typename InputVectorImageType::PixelType InputVectorPixelType;
            typedef typename InputVectorImageType::InternalPixelType InputVectorInternalPixelType;
            typedef typename OutputVectorImageType::PixelType OutputVectorPixelType;
            typedef typename itk::NumericTraits<InputVectorInternalPixelType>::RealType InputRealType;
            typedef typename itk::NumericTraits<InputVectorPixelType>::RealType InputVectorRealType;
            typedef typename InputVectorImageType::RegionType InputVectorImageRegionType;
            typedef typename OutputVectorImageType::RegionType OutputVectorImageRegionType;
            typedef typename OutputVectorImageType::InternalPixelType OutputVectorInternalPixelType;
            typedef typename InputVectorImageType::SizeType InputSizeType;
            typedef typename InputVectorImageType::OffsetType InputOffsetType;
            typedef typename itk::Array<InputRealType> ArrayType;
            typedef TBoundaryCondition BoundaryConditionType;

            typedef itk::ZeroFluxNeumannBoundaryCondition<MaskType> MaskBoundaryConditionType;

            /** Set the radius of the neighborhood of the filter */
            virtual void SetRadius (const InputSizeType rad)
            {
                itkDebugMacro("setting radius to " << rad);
                if (this->m_Radius != rad)
                {
                    this->m_Radius = rad;
                    unsigned int arraySize=1;
                    for (unsigned int i=0; i<m_Radius.GetSizeDimension(); ++i)
                    {
                        arraySize = arraySize * (2 * this->m_Radius[i] + 1);
                    }
                    this->m_Filter.SetSize(arraySize);
                    this->m_Filter.Fill(1);
                    this->Modified();
                }
            }

            /** Get the radius of the neighborhood of the filter*/
            itkGetConstReferenceMacro(Radius, InputSizeType);

            /** Set the input filter */
            virtual void SetFilter( ArrayType filter )
            {
                if (filter.Size()!= m_Filter.Size())
                {
                    itkExceptionMacro("Error in SetFilter, invalid filter size:"<< filter.Size()<<" instead of (2*m_Radius[0]+1)*(2*m_Radius[1]+1): "<<m_Filter.Size());
    }
    else
    {
      m_Filter = filter;
    }
    this->Modified();
  }
  itkGetConstReferenceMacro(Filter, ArrayType);

  /** Set/Get methods for the input mask  */
  vnsSetGetInputRawMacro(Mask, TMask, 1)

  /**
   * Set/Get methods for the normalization of the filter
   * Not used here for performance reasons
  */
  //itkSetMacro(NormalizeFilter, bool);
  //itkGetMacro(NormalizeFilter, bool);
  //itkBooleanMacro(NormalizeFilter);

  itkSetMacro(NoData, OutputVectorInternalPixelType);
  itkGetMacro(NoData, OutputVectorInternalPixelType);



protected:
  ConvolutionVectorImageFilter();
  virtual ~ConvolutionVectorImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ConvolutionVectorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputVectorImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId );

  /** ConvolutionVectorImageFilter needs a larger input requested region than
   * the output requested region.  As such, ConvolutionVectorImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

private:
  ConvolutionVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Radius of the filter */
  InputSizeType m_Radius;
  /** Array containing the filter values */
  ArrayType m_Filter;
  /** Flag for filter coefficients normalization. Not used here for performance reasons. */
  //bool m_NormalizeFilter;
  /** NoData value for the output */
  OutputVectorInternalPixelType m_NoData;

};

} // end namespace itk

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsConvolutionVectorImageFilter.txx"
#endif

#endif

