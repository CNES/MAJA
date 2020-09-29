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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 juil. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsExpandVectorImageFilter_h
#define __vnsExpandVectorImageFilter_h

#include "itkImageToImageFilter.h"
#include "vnsVectorLinearInterpolateImageFunction.h"

namespace vns
{

/** \class ExpandVectorImageFilter
 * \brief Expand the size of an image by an integer factor in each
 * dimension.
 *
 * ExpandVectorImageFilter increases the size of an image by an integer
 * factor in each dimension using a interpolation method.
 * The output image size in each dimension is given by:
 *
 * OutputSize[j] = InputSize[j] * ExpandFactors[j]
 *
 * The output values are obtained by interpolating the input image.
 * The default interpolation type used is the LinearInterpolateImageFunction.
 * The user can specify a particular interpolation function via
 * SetInterpolator(). Note that the input interpolator must derive
 * from base class InterpolateImageFunction.
 *
 * \warning: The following is valid only when the flag
 * ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY is OFF.
 * When the LargestPossibleRegion is requested, the output image will contain
 * padding at the upper edge of each dimension. The width of padding in the
 * i'th dimension is (ExpandFactors[i] - 1). Users can specify the padding
 * value used by setting the EdgePaddingValue.
 *
 * \warning: The following is valid only when the flag
 * ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY is ON 
 * The output image will not contain any padding, and therefore the
 * EdgePaddingValue will not be used.
 *
 * This filter will produce an output with different pixel spacing
 * that its input image such that:
 *
 * OutputSpacing[j] = InputSpacing[j] / ExpandFactors[j]
 *
 * The filter is templated over the input image type and the output 
 * image type.
 *
 * This filter is implemented as a multithreaded filter and supports
 * streaming.
 *
 * \warning This filter only works for image with scalar pixel types.
 * For vector images use VectorExpandVectorImageFilter.
 *
 * This filter assumes that the input and output image has the same
 * number of dimensions.
 *
 * \sa InterpolateImageFunction
 * \sa LinearInterpolationImageFunction
 * \sa VectorExpandVectorImageFilter
 *
 * \ingroup GeometricTransform
 */

template <class TInputImage, class TOutputImage>
class ExpandVectorImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
    /** Standard class typedefs. */
    typedef ExpandVectorImageFilter Self;
    typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ExpandVectorImageFilter, itk::ImageToImageFilter);

    /** Typedef to describe the output image region type. */
    typedef typename TOutputImage::RegionType OutputImageRegionType;

    /** ImageDimension enumeration. */
    itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

    /** Inherit some types from superclass. */
    typedef typename Superclass::InputImageType InputImageType;
    //typedef typename InputImageType::PixelType InputPixelType;
    //typedef typename InputImageType::InternamPixelType InputInternalPixelType;
    typedef typename Superclass::OutputImageType OutputImageType;

    typedef typename OutputImageType::PixelType OutputPixelType;
    typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
    typedef typename InputImageType::Pointer InputImagePointer;
    typedef typename OutputImageType::Pointer OutputImagePointer;

    /** Typedef support for the interpolation function. */
    typedef double CoordRepType;
    typedef VectorLinearInterpolateImageFunction<InputImageType, CoordRepType> InterpolatorType;
    typedef typename InterpolatorType::OutputType  InterpolatorOutputType;
    typedef typename InterpolatorType::Pointer InterpolatorPointer;


    /** Set/Get the interpolator function. */
    //itkSetObjectMacro( Interpolator, InterpolatorType );
    //itkGetObjectMacro( Interpolator, InterpolatorType );

    /** Set the expand factors. Values are clamped to
     * a minimum value of 1. Default is 1 for all dimensions. */
    itkLegacyMacro( virtual void SetExpandFactors( const unsigned int factors[] ); )
    itkLegacyMacro( virtual void SetExpandFactors( const unsigned int factor ); )

    /** Get the expand factors. */
    virtual const unsigned int * GetExpandFactors() const
    {   return m_ExpandFactors;}

    /** Set the edge padding value. The default is zero. */
    itkLegacyMacro( itkSetMacro( EdgePaddingValue, OutputPixelType ) )
    /** Set a constant edge padding value precising the value and the image number of components.
     The default is zero. */
    void SetEdgePaddingValue(OutputInternalPixelType scal, unsigned int nbComp)
    {
        m_EdgePaddingValue.SetSize(nbComp);
        m_EdgePaddingValue.Fill(scal);
        m_IsSetEdgePaddingValue = true;
        this->Modified();
    }
    /** Get the edge padding value */
    itkGetConstMacro( EdgePaddingValue, OutputPixelType );

    /** ExpandVectorImageFilter produces an image which is a different resolution and
     * with a different pixel spacing than its input image.  As such,
     * ExpandVectorImageFilter needs to provide an implementation for
     * UpdateOutputInformation() in order to inform the pipeline execution model.
     * The original documentation of this method is below.
     * \sa ProcessObject::GenerateOutputInformaton() */
    virtual void GenerateOutputInformation();

    /** ExpandVectorImageFilter needs a smaller input requested region than the output
     * requested region.  As such, ShrinkImageFilter needs to provide an
     * implementation for GenerateInputRequestedRegion() in order to inform
     * the pipeline execution model.
     * \sa ProcessObject::GenerateInputRequestedRegion() */
    virtual void GenerateInputRequestedRegion();

#ifdef ITK_USE_CONCEPT_CHECKING
    /** Begin concept checking */
    itkConceptMacro(InputHasNumericTraitsCheck,
            (itk::Concept::HasNumericTraits<typename TInputImage::InternalPixelType>));
    itkConceptMacro(OutputHasNumericTraitsCheck,
            (itk::Concept::HasNumericTraits<typename TOutputImage::InternalPixelType>));
    /** End concept checking */
#endif

protected:
    ExpandVectorImageFilter();
    virtual ~ExpandVectorImageFilter()
    {}
    void PrintSelf(std::ostream& os, itk::Indent indent) const;

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

    /** This method is used to set the state of the filter before
     * multi-threading. */
    virtual void BeforeThreadedGenerateData();

private:
    ExpandVectorImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    unsigned int m_ExpandFactors[ImageDimension];
    InterpolatorPointer m_Interpolator;
    OutputPixelType m_EdgePaddingValue;
    /** bool used to know if the user has set EdgePaddingValue using SetEdgePaddingValue(OutputInternalPixelType scal, unsigned int nbComp) method */
    bool m_IsSetEdgePaddingValue;

};

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsExpandVectorImageFilter.txx"
#endif

#endif
