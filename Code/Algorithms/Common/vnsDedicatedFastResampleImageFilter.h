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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsDedicatedFastResampleImageFilter.h 2656 2011-02-15 10:25:38Z cvallade $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsDedicatedFastResampleImageFilter_h
#define __vnsDedicatedFastResampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "vnsMacro.h"
#include "itkContinuousIndex.h"
#include "itkInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "vnsFastVectorLinearInterpolateImageFunction.h"
#include "vnsFastVectorLinearWithRadiusInterpolateImageFunction.h"
#include "vnsResamplerHelper.h"

namespace vns
{

    /** \class DedicatedFastResampleImageFilter
     * \brief Expand the size of an image by an integer factor in each
     * dimension.
     *
     * DedicatedFastResampleImageFilter increases the size of an image by an integer
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
     * For vector images use VectorDedicatedFastResampleImageFilter.
     *
     * This filter assumes that the input and output image has the same
     * number of dimensions.
     *
     * \sa InterpolateImageFunction
     * \sa LinearInterpolationImageFunction
     * \sa VectorDedicatedFastResampleImageFilter
     *
     * \ingroup GeometricTransform
     */

    template<class TInputImage, class TOutputImage>
        class DedicatedFastResampleImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef DedicatedFastResampleImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Run-time type information (and related methods). */
            itkTypeMacro(DedicatedFastResampleImageFilter, itk::ImageToImageFilter)

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
                typedef typename OutputImageType::SpacingType SpacingType;
                typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
                typedef typename InputImageType::Pointer InputImagePointer;
                typedef typename OutputImageType::Pointer OutputImagePointer;
                typedef typename OutputImageType::IndexValueType IndexValueType;

                /** Typedef support for the interpolation function. */
                typedef double CoordRepType;
                /** Point type */
                typedef itk::Point<CoordRepType, itkGetStaticConstMacro(ImageDimension)> PointType;

                /** Typedef support for the interpolation function. */
                typedef double IndexRepType;
                /** Point type */
                typedef itk::ContinuousIndex<IndexRepType, itkGetStaticConstMacro(ImageDimension)> ContinuousIndexType;

                typedef typename TOutputImage::SizeType OutputSizeType;
                typedef typename TOutputImage::IndexType OutputIndexType;
                typedef typename TInputImage::SizeType InputSizeType;
                typedef typename TInputImage::IndexType InputIndexType;
                typedef typename TInputImage::RegionType InputRegionType;

                typedef typename ResamplerHelper::InterpolatorType InterpolatorValueType;

                vnsMemberAndSetAndGetConstReferenceMacro(InterpolatorValue, InterpolatorValueType)

                /** Interpolator type */
//    typedef itk::InterpolateImageFunction<InputImageType,CoordRepType> InterpolatorType;
//    typedef itk::LinearInterpolateImageFunction<InputImageType,CoordRepType>  DefaultInterpolatorType;
//                typedef VectorLinearInterpolateImageFunction<InputImageType, CoordRepType> VectorLinearInterpolatorType;
//                typedef typename VectorLinearInterpolatorType::Pointer VectorLinearInterpolatorPointer;
                typedef FastVectorLinearInterpolateImageFunction<InputImageType, CoordRepType> VectorLinearInterpolatorType;
                typedef typename VectorLinearInterpolatorType::Pointer VectorLinearInterpolatorPointer;

                typedef FastVectorLinearWithRadiusInterpolateImageFunction<InputImageType, CoordRepType> FastLinearWithRadiusInterpolatorType;
                typedef typename FastLinearWithRadiusInterpolatorType::Pointer FastLinearWithRadiusInterpolatorPointer;

                /** Get a pointer to the interpolator function. */
                itkGetObjectMacro( FastLinearWithRadiusInterpolator, FastLinearWithRadiusInterpolatorType );

                typedef itk::InterpolateImageFunction<InputImageType,CoordRepType> InterpolatorType;
                typedef typename InterpolatorType::Pointer InterpolatorPointer;
                typedef itk::LinearInterpolateImageFunction<InputImageType, double> LinearInterpolatorType;

                /** Set the interpolator function. */
                itkSetObjectMacro( Interpolator, InterpolatorType );

                /** Get a pointer to the interpolator function. */
                itkGetObjectMacro( Interpolator, InterpolatorType );

                vnsMemberAndDirectSetAndGetConstReferenceMacro(AreaToOutputImageResolution, AreaType)

                /** DedicatedFastResampleImageFilter produces an image which is a different resolution and
                 * with a different pixel spacing than its input image.  As such,
                 * DedicatedFastResampleImageFilter needs to provide an implementation for
                 * UpdateOutputInformation() in order to inform the pipeline execution model.
                 * The original documentation of this method is below.
                 * \sa ProcessObject::GenerateOutputInformaton() */
                virtual void GenerateOutputInformation();

                /** DedicatedFastResampleImageFilter needs a smaller input requested region than the output
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
                DedicatedFastResampleImageFilter() : m_InterpolatorValue(ResamplerHelper::NOT_INTERPOLATOR_INITIALIZED)
                {    // Setup default interpolator
                    typename LinearInterpolatorType::Pointer interp = LinearInterpolatorType::New();
                    m_Interpolator = static_cast<InterpolatorType*>( interp.GetPointer() );
                    m_VectorLinearInterpolator = VectorLinearInterpolatorType::New();
                    m_FastLinearWithRadiusInterpolator = FastLinearWithRadiusInterpolatorType::New();
                }
                virtual ~DedicatedFastResampleImageFilter()
                {}
                void PrintSelf(std::ostream& os, itk::Indent indent) const
                {
                    Superclass::PrintSelf(os, indent);

                    // Print the Interpolator
                    os << indent << "Interpolator: ";
                    os << m_Interpolator.GetPointer() << std::endl;

                }

                /** DedicatedFastResampleImageFilter is implemented as a multithreaded filter.  Therefore,
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
//            {
//                vnsExceptionBusinessMacro("ThreadedGenerateData must be implemented in the sub classes !!")
//            }

                /** This method is used to set the state of the filter before
                 * multi-threading. */
                virtual void BeforeThreadedGenerateData();

            private:
                DedicatedFastResampleImageFilter(const Self&); //purposely not implemented
                void operator=(const Self&);//purposely not implemented

                InterpolatorPointer m_Interpolator;
                VectorLinearInterpolatorPointer m_VectorLinearInterpolator;
                FastLinearWithRadiusInterpolatorPointer m_FastLinearWithRadiusInterpolator;

            };

        }
        // end namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsDedicatedFastResampleImageFilter.txx"
#endif

#endif
