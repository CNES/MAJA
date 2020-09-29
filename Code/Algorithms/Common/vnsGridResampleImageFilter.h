/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 19 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-1638-CNES : 1 mars 2016 : Nouveau filtre reechantillonnage issu OTB   *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudMaskingImageFilter.h 11465 2016-01-28 15:51:04Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsGridResampleImageFilter_h
#define __vnsGridResampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkDefaultConvertPixelTraits.h"

#include "otbMacro.h"

namespace vns
{

    /** \class GridResampleImageFilter
     *  \brief Resample input image on a new origin/spacing/size grid
     *
     *  This filter resample the input image on a new grid defined by
     *  OutputOrigin, OutputSpacing and OutputSize, using the provided
     *  interpolator.
     *
     *  This is equivalent to a itk::ResampleImageFilter using an
     *  itk::IdentityTransform, except that it is slightly more efficient
     *  and that in this simplified case it is possible to explicitely
     *  compute the input requested region. The GridResampleImageFilter
     *  therefore supports streaming, contrary to the
     *  itk::ResampleImageFilter.
     *
     *  When grid position is outside of the input image domain, the
     *  default EdgePaddingValue is used.
     *
     *  If CheckOutputBounds flag is set to true (default value), the
     *  interpolated value will be checked for output pixel type range
     *  prior to casting.
     *
     * \ingroup OTBImageManipulation
     * \ingroup Streamed
     * \ingroup Threaded
     **/

    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision = double>
        class GridResampleImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef GridResampleImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Run-time type information (and related methods). */
            itkTypeMacro(GridResampleImageFilter, itk::ImageToImageFilter)

            /** Number of dimensions. */
            itkStaticConstMacro(ImageDimension, unsigned int,
                    TOutputImage::ImageDimension);
            itkStaticConstMacro(InputImageDimension, unsigned int,
                    TInputImage::ImageDimension);

            /** Typedef parameters*/
            typedef TInputImage InputImageType;
            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename TOutputImage::PixelType OutputPixelType;

            typedef itk::DefaultConvertPixelTraits<OutputPixelType> OutputPixelConvertType;
            typedef typename OutputPixelConvertType::ComponentType OutputPixelComponentType;

            /** Interpolator type */
            typedef itk::InterpolateImageFunction<InputImageType,
            TInterpolatorPrecision> InterpolatorType;
            typedef typename InterpolatorType::Pointer InterpolatorPointerType;
            typedef itk::LinearInterpolateImageFunction<InputImageType,
            TInterpolatorPrecision> DefaultInterpolatorType;
            typedef typename InterpolatorType::OutputType InterpolatorOutputType;
            typedef itk::DefaultConvertPixelTraits< InterpolatorOutputType > InterpolatorConvertType;
            typedef typename InterpolatorConvertType::ComponentType InterpolatorComponentType;

            /** Input pixel continuous index typdef */
            typedef typename itk::ContinuousIndex<double,InputImageDimension > ContinuousInputIndexType;

            /** ImageBase typedef */
            typedef itk::ImageBase<OutputImageType::ImageDimension> ImageBaseType;
            typedef typename ImageBaseType::SpacingType SpacingType;
            typedef typename ImageBaseType::SizeType SizeType;
            typedef typename ImageBaseType::PointType PointType;
            typedef typename ImageBaseType::IndexType IndexType;
            typedef typename ImageBaseType::IndexValueType IndexValueType;
            typedef typename ImageBaseType::SizeValueType SizeValueType;

            itkSetMacro(OutputStartIndex,IndexType);
            itkGetConstReferenceMacro(OutputStartIndex,IndexType);

            itkSetMacro(OutputSize,SizeType);
            itkGetConstReferenceMacro(OutputSize,SizeType);

            itkSetMacro(OutputOrigin,PointType);
            itkGetConstReferenceMacro(OutputOrigin,PointType);

            itkSetMacro(OutputSpacing,SpacingType);
            itkGetConstReferenceMacro(OutputSpacing,SpacingType);

            itkSetMacro(EdgePaddingValue,OutputPixelType);
            itkGetConstReferenceMacro(EdgePaddingValue,OutputPixelType);

            itkSetMacro(InterpolatorRadius,unsigned int);
            itkGetConstReferenceMacro(InterpolatorRadius,unsigned int);

            itkSetMacro(CheckOutputBounds,bool);
            itkGetMacro(CheckOutputBounds,bool);
            itkBooleanMacro(CheckOutputBounds);

            itkSetObjectMacro(Interpolator,InterpolatorType);
            itkGetObjectMacro(Interpolator,InterpolatorType);

            /** Import output parameters from a given image */
            void SetOutputParametersFromImage(const ImageBaseType * image);

            /** Method Compute the Modified Time based on changed to the components. */
            itk::ModifiedTimeType GetMTime(void) const;

        protected:
            GridResampleImageFilter();

            /** Destructor */
            virtual ~GridResampleImageFilter()
            {};

            virtual void GenerateOutputInformation();

            virtual void GenerateInputRequestedRegion();

            virtual void BeforeThreadedGenerateData();

            virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            virtual void AfterThreadedGenerateData();

            inline OutputPixelType CastPixelWithBoundsChecking( const InterpolatorOutputType& value,
                    const InterpolatorComponentType& minComponent,
                    const InterpolatorComponentType& maxComponent) const
            {
                // Method imported from itk::ResampleImageFilter
                const unsigned int nComponents = InterpolatorConvertType::GetNumberOfComponents(value);
                OutputPixelType outputValue;

                itk::NumericTraits<OutputPixelType>::SetLength( outputValue, nComponents );

                for (unsigned int n=0; n<nComponents; n++)
                {
                    InterpolatorComponentType component = InterpolatorConvertType::GetNthComponent( n, value );

                    if ( m_CheckOutputBounds && component < minComponent )
                    {
                        OutputPixelConvertType::SetNthComponent( n, outputValue, static_cast<OutputPixelComponentType>( minComponent ) );
                    }
                    else if ( m_CheckOutputBounds && component > maxComponent )
                    {
                        OutputPixelConvertType::SetNthComponent( n, outputValue, static_cast<OutputPixelComponentType>( maxComponent ) );
                    }
                    else
                    {
                        OutputPixelConvertType::SetNthComponent(n, outputValue,
                                static_cast<OutputPixelComponentType>( component ) );
                    }
                }

                return outputValue;
            }

            void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            GridResampleImageFilter(const Self &); //purposely not implemented
            void operator =(const Self&);//purposely not implemented

            IndexType m_OutputStartIndex;// output image start index
            SizeType m_OutputSize;// output image size
            PointType m_OutputOrigin;// output image origin
            SpacingType m_OutputSpacing;// output image spacing

            OutputPixelType m_EdgePaddingValue;// Default pixel value

            bool m_CheckOutputBounds;// Shall we check
                                     // output bounds when
                                     // casting?

            InterpolatorPointerType m_Interpolator;// Interpolator used
                                     // for resampling

            OutputImageRegionType m_ReachableOutputRegion;// Internal
                                     // variable for
                                     // speed-up. Computed
                                     // in BeforeThreadedGenerateData

            unsigned int m_InterpolatorRadius;
        };

    }
    // namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsGridResampleImageFilter.txx"
#endif

#endif
