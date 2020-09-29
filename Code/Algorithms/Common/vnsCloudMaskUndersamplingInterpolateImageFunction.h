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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Creation pour corriger l'anomalie         *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudMaskUndersamplingWithoutResamplingImageFilter.h 11465 2016-01-28 15:51:04Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskUndersamplingInterpolateImageFunction_h
#define __vnsCloudMaskUndersamplingInterpolateImageFunction_h

#include "itkInterpolateImageFunction.h"
#include "itkMacro.h"
#include "otbVectorImage.h"

namespace vns
{
    /** \class CloudMaskUndersamplingInterpolateImageFunctionBase
     *  \brief Set 1 if a specific value is found in an image.
     *
     * This function works for 2-dimensional images.
     *
     * This function works with both Images and VectorImages.
     *
     * \ingroup ImageFunctions ImageInterpolators
     */
    template<class TInputImage, class TCoordRep = double>
        class CloudMaskUndersamplingInterpolateImageFunctionBase : public itk::InterpolateImageFunction<TInputImage, TCoordRep>
        {
        public:
            /** Standard class typedefs. */
            typedef CloudMaskUndersamplingInterpolateImageFunctionBase Self;
            typedef itk::InterpolateImageFunction<TInputImage, TCoordRep> Superclass;

            /** Run-time type information (and related methods). */
            itkTypeMacro(CloudMaskUndersamplingInterpolateImageFunctionBase, InterpolateImageFunction)
            itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

            /** OutputType typedef support. */
            typedef typename Superclass::OutputType OutputType;

            /** InputImageType typedef support. */
            typedef typename Superclass::InputImageType InputImageType;

            /** InputPixelType typedef support. */
            typedef typename Superclass::InputPixelType InputPixelType;

            /** RealType typedef support. */
            typedef typename Superclass::RealType RealType;

            /** Index typedef support. */
            typedef typename Superclass::IndexType IndexType;
            typedef typename Superclass::IndexValueType IndexValueType;

            /** Point typedef support. */
            typedef typename Superclass::PointType PointType;

            /** ContinuousIndex typedef support. */
            typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
            typedef TCoordRep ContinuousIndexValueType;

            /** Coeficients container type.*/
            typedef vnl_vector<double> CoefContainerType;

            /** Set/Get the window radius */
            virtual void SetRadius(unsigned int radius)
            {
                if (radius < 1)
                {
                    itkExceptionMacro(<< "Radius must be strictly greater than 0");
                }
                else
                {
                    m_Radius = radius;
                    m_WinSize = 2 * m_Radius + 1;
                }
            }
            itkGetConstMacro(Radius, unsigned int)
            itkSetMacro(NoData, InputPixelType)
            itkGetConstMacro(NoData, InputPixelType)

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
            CloudMaskUndersamplingInterpolateImageFunctionBase() : m_Radius(1), m_WinSize(3), m_NoData(0)
            {
            }
            virtual ~CloudMaskUndersamplingInterpolateImageFunctionBase()
            {
            }
            void PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Used radius for the BCO */
            unsigned int m_Radius;
            unsigned int m_WinSize;

            /** NoData value */
            InputPixelType m_NoData;

        private:
            CloudMaskUndersamplingInterpolateImageFunctionBase( const Self& ); //purposely not implemented
            void operator=( const Self& );//purposely not implemented

        };

    template<class TInputImage, class TCoordRep = double>
        class CloudMaskUndersamplingCLAInterpolateImageFunction : public CloudMaskUndersamplingInterpolateImageFunctionBase<
                TInputImage, TCoordRep>
        {
        public:
            /** Standard class typedefs. */
            typedef CloudMaskUndersamplingCLAInterpolateImageFunction Self;
            typedef CloudMaskUndersamplingInterpolateImageFunctionBase<TInputImage, TCoordRep> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            itkTypeMacro(CloudMaskUndersamplingCLAInterpolateImageFunction, CloudMaskUndersamplingInterpolateImageFunctionBase)
            itkNewMacro(Self)
            itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

            typedef typename Superclass::OutputType OutputType;
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename Superclass::InputPixelType InputPixelType;
            typedef typename Superclass::RealType RealType;
            typedef typename Superclass::IndexType IndexType;
            typedef typename Superclass::IndexValueType IndexValueType;
            typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

            virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const;

        protected:
            CloudMaskUndersamplingCLAInterpolateImageFunction()
            {
            }
            virtual ~CloudMaskUndersamplingCLAInterpolateImageFunction()
            {
            }
            void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            CloudMaskUndersamplingCLAInterpolateImageFunction( const Self& ); //purposely not implemented
            void operator=( const Self& );//purposely not implemented
        };

    template<typename TPixel, unsigned int VImageDimension, class TCoordRep>
        class CloudMaskUndersamplingCLAInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep> : public CloudMaskUndersamplingInterpolateImageFunctionBase<
                otb::VectorImage<TPixel, VImageDimension>, TCoordRep>
        {
        public:
            /** Standard class typedefs.*/
            typedef CloudMaskUndersamplingCLAInterpolateImageFunction Self;
            typedef CloudMaskUndersamplingInterpolateImageFunctionBase<otb::VectorImage<TPixel, VImageDimension>, TCoordRep> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            itkTypeMacro(CloudMaskUndersamplingCLAInterpolateImageFunction, CloudMaskUndersamplingInterpolateImageFunctionBase)
            itkNewMacro(Self)
            itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

            typedef typename Superclass::OutputType OutputType;
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename Superclass::InputPixelType InputPixelType;
            typedef typename Superclass::RealType RealType;
            typedef typename Superclass::IndexType IndexType;
            typedef typename Superclass::IndexValueType IndexValueType;
            typedef typename Superclass::PointType PointType;
            typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
            typedef typename Superclass::CoefContainerType CoefContainerType;

            virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const;

        protected:
            CloudMaskUndersamplingCLAInterpolateImageFunction()
            {
            }
            virtual ~CloudMaskUndersamplingCLAInterpolateImageFunction()
            {
            }
            void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            CloudMaskUndersamplingCLAInterpolateImageFunction( const Self& ); //purposely not implemented
            void operator=( const Self& );//purposely not implemented
        };

    template<class TInputImage, class TCoordRep = double>
        class CloudMaskUndersamplingCLDAltInterpolateImageFunction : public CloudMaskUndersamplingInterpolateImageFunctionBase<
                TInputImage, TCoordRep>
        {
        public:
            /** Standard class typedefs. */
            typedef CloudMaskUndersamplingCLDAltInterpolateImageFunction Self;
            typedef CloudMaskUndersamplingInterpolateImageFunctionBase<TInputImage, TCoordRep> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            itkTypeMacro(CloudMaskUndersamplingCLDAltInterpolateImageFunction, CloudMaskUndersamplingInterpolateImageFunctionBase)
            itkNewMacro(Self)
            itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

            typedef typename Superclass::OutputType OutputType;
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename Superclass::InputPixelType InputPixelType;
            typedef typename Superclass::RealType RealType;
            typedef typename Superclass::IndexType IndexType;
            typedef typename Superclass::IndexValueType IndexValueType;
            typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

            virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const;

            itkSetMacro(SuccesOutputValue, OutputType)
            itkGetConstMacro(SuccesOutputValue, OutputType)
            itkSetMacro(FailureOutputValue, OutputType)
            itkGetConstMacro(FailureOutputValue, OutputType)

        protected:
            CloudMaskUndersamplingCLDAltInterpolateImageFunction(): m_SuccesOutputValue(1), m_FailureOutputValue(itk::NumericTraits<OutputType>::Zero)
            {
            }
            virtual ~CloudMaskUndersamplingCLDAltInterpolateImageFunction()
            {
            }
            void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            CloudMaskUndersamplingCLDAltInterpolateImageFunction( const Self& ); //purposely not implemented

                void operator=( const Self& );//purposely not implemented

                OutputType m_SuccesOutputValue;
                OutputType m_FailureOutputValue;
            };

    template<typename TPixel, unsigned int VImageDimension, class TCoordRep>
        class CloudMaskUndersamplingCLDAltInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep> : public CloudMaskUndersamplingInterpolateImageFunctionBase<
                otb::VectorImage<TPixel, VImageDimension>, TCoordRep>
        {
        public:
            /** Standard class typedefs.*/
            typedef CloudMaskUndersamplingCLDAltInterpolateImageFunction Self;
            typedef CloudMaskUndersamplingInterpolateImageFunctionBase<otb::VectorImage<TPixel, VImageDimension>, TCoordRep> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            itkTypeMacro(CloudMaskUndersamplingCLDAltInterpolateImageFunction, CloudMaskUndersamplingInterpolateImageFunctionBase)
            itkNewMacro(Self)
            itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

            typedef typename Superclass::OutputType OutputType;
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename Superclass::InputPixelType InputPixelType;
            typedef typename Superclass::RealType RealType;
            typedef typename Superclass::IndexType IndexType;
            typedef typename Superclass::IndexValueType IndexValueType;
            typedef typename Superclass::PointType PointType;
            typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
            typedef typename Superclass::CoefContainerType CoefContainerType;

            virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const;

            itkSetMacro(SuccesOutputValue, OutputType)
            itkGetConstMacro(SuccesOutputValue, OutputType)
            itkSetMacro(FailureOutputValue, OutputType)
            itkGetConstMacro(FailureOutputValue, OutputType)

        protected:
            CloudMaskUndersamplingCLDAltInterpolateImageFunction(): m_SuccesOutputValue(1), m_FailureOutputValue(0)
            {
            }
            virtual ~CloudMaskUndersamplingCLDAltInterpolateImageFunction()
            {
            }
            void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            CloudMaskUndersamplingCLDAltInterpolateImageFunction( const Self& ); //purposely not implemented
            void operator=( const Self& );//purposely not implemented

            OutputType m_SuccesOutputValue;
            OutputType m_FailureOutputValue;
        };

}// end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsCloudMaskUndersamplingInterpolateImageFunction.txx"
#endif

#endif
