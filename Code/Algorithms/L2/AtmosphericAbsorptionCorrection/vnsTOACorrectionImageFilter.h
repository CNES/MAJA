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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsTOACorrectionImageFilter_h
#define __vnsTOACorrectionImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "vnsTOACorrectionImageFunctor.h"
#include "vnsSmacCoefContainer.h"

namespace vns
{

    /** \class TOACorrectionImageFilter
     * \brief This class implements a TOA correction using SMAC coefficients and translated method (smac_inverse_tg).
     *
     * Inputs are:
     * \begin{itemize}
     * \item Inputs;
     * \begin{itemize}
     *    \item TOA image : otbVectorImage
     *    \item Water image : otbImage
     *    \item Pressure image : otbmage
     * \end{itemize}
     * \item Output : TOA_surf : otbVectorImage (same number of channels than the input TOA).
     * \end{itemize}
     *
     * The filter needs other information:
     * \begin{itemize}
     * \item The solar zenithal angle : m_ThetaS
     * \item The view zenithal angle (one angle per band) : m_ThetaV
     * \item The ozone amount : m_OzoneAmount
     * \item The list of SMAC coefficents (49 value for each bands)
     * \end{itemize}
     *
     * \ingroup Radiometry
     *
     */

    template<class TInputTOAImage, class TInputPressureImage, class TOutputTOASurfImage, class TInputWaterImage = otb::Image<double> >
        class TOACorrectionImageFilter : public itk::InPlaceImageFilter<TInputTOAImage, TOutputTOASurfImage>
        {
        public:
            /** Standard class typedefs. */
            typedef TOACorrectionImageFilter Self;
            typedef itk::InPlaceImageFilter<TInputTOAImage, TOutputTOASurfImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Run-time type information (and related methods). */
            itkTypeMacro(TOACorrectionImageFilter, itk::InPlaceImageFilter)

            /** Some typedefs. */
            typedef TInputTOAImage InputTOAImageType;
            typedef typename InputTOAImageType::ConstPointer InputTOAImagePointer;
            typedef typename InputTOAImageType::RegionType InputTOAImageRegionType;
            typedef typename InputTOAImageType::PixelType InputTOAImagePixelType;
            typedef TInputWaterImage InputWaterImageType;
            typedef typename InputWaterImageType::ConstPointer InputWaterImagePointer;
            typedef typename InputWaterImageType::RegionType InputWaterImageRegionType;
            typedef typename InputWaterImageType::PixelType InputWaterImagePixelType;
            typedef TInputPressureImage InputPressureImageType;
            typedef typename InputPressureImageType::ConstPointer InputPressureImagePointer;
            typedef typename InputPressureImageType::RegionType InputPressureImageRegionType;
            typedef typename InputPressureImageType::PixelType InputPressureImagePixelType;
            typedef TOutputTOASurfImage OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImagePixelType::ValueType OutputInternalPixelType;

            typedef std::vector<double> DoubleVectorType;

            typedef vns::Functor::TOACorrectionImageFunctor<InputTOAImagePixelType, InputWaterImagePixelType, InputPressureImagePixelType,
                    OutputImagePixelType> FunctorType;

            /** Samc coefficient typedefs*/
            typedef typename FunctorType::SmacCoefContainerListType SmacCoefContainerListType;

            /** Get the functor object.  The functor is returned by reference.
             * (Functors do not have to derive from itk::LightObject, so they do
             * not necessarily have a reference count. So we cannot return a
             * SmartPointer). */
            FunctorType&
            GetFunctor(void)
            {
                return m_Functor;
            }

            /** Get the functor object.  The functor is returned by reference.
             * (Functors do not have to derive from itk::LightObject, so they do
             * not necessarily have a reference count. So we cannot return a
             * SmartPointer.) */
            const FunctorType&
            GetFunctor() const
            {
                return m_Functor;
            }

            /** Set the functor object.  This replaces the current Functor with a
             * copy of the specified Functor. This allows the user to specify a
             * functor that has ivars set differently than the default functor.
             * This method requires an operator==() be defined on the functor
             * (or the compiler's default implementation of operator==() being
             * appropriate). */
            void
            SetFunctor(const FunctorType& functor)
            {
                if (functor != m_Functor)
                {
                    m_Functor = functor;
                    this->Modified();
                }
            }

            /** Overload SetInput1 method. */
            vnsSetGetInputMacro( TOA, InputTOAImageType, 0)
            /** Overload SetInput2 method. */
            vnsSetGetInputMacro( Pressure, InputPressureImageType, 1)

            /** Overload SetInput2 method. */
            void
            SetWaterInput(const InputWaterImageType * image)
            {
                // The ProcessObject is not const-correct so the const_cast is required here
                this->SetNthInput(2, const_cast<InputWaterImageType *>(image));
                m_IsWaterImage = true;
            }

            /** Set the ozone amount value */
            void
            SetOzoneAmount(const double & oz)
            {
                this->GetFunctor().SetOzoneAmount(oz);
                this->Modified();
            }

            /** Get the ozone amount value */
            double
            GetOzoneAmount() const
            {
                return this->GetFunctor().GetOzoneAmount();
            }

            /** Set the solar zenithal angle value */
            void
            SetThetaS(const double & thetaS)
            {
                this->GetFunctor().SetThetaS(thetaS);
                this->Modified();
            }

            /** Get the view zenithal angle value */
            double
            GetThetaS() const
            {
                return this->GetFunctor().GetThetaS();
            }

            /** Set the view zenithal angle value */
            void
            SetThetaV(const DoubleVectorType & thetaV)
            {
                this->GetFunctor().SetThetaV(thetaV);
                this->Modified();
            }

            /** Get the view zenithal angle value */
            DoubleVectorType
            GetThetaV() const
            {
                return this->GetFunctor().GetThetaV();
            }

            /** Set Smac coefficients list */
            void
            SetSmacCoefContainerList(const SmacCoefContainerListType & coefList)
            {
                this->GetFunctor().SetSmacCoefsList(coefList);
                this->Modified();
            }

            /** Get Smac coefficients list */
            SmacCoefContainerListType
            GetSmacCoefContainerList() const
            {
                return this->GetFunctor().GetSmacCoefsList();
            }
            SmacCoefContainerListType
            GetSmacCoefContainerList()
            {
                return this->GetFunctor().GetSmacCoefsList();
            }

            /** Set the view zenithal angle value */
            void
            SetL2NoData(const RealNoDataType & val)
            {
                this->GetFunctor().SetL2NoData(val);
                this->Modified();
            }

            /** Add Smac coefficients to the list */
            void
            AddSmacCoefContainer(const SmacCoefContainer & coef);
            /** Add Smac coefficients to the list as a vector of double */
            void
            AddSmacCoefContainer(const std::vector<double> & vectCoef);

            /** Set water amount default value. */
            itkSetMacro( WaterAmountDefaultValue, double )

            /** Get water amount default value. */
            itkGetConstMacro( WaterAmountDefaultValue, double )

            /** Image dimensions */
            itkStaticConstMacro(InputTOAImageDimension, unsigned int,
                    TInputTOAImage::ImageDimension);
            itkStaticConstMacro(InputWaterImageDimension, unsigned int,
                    TInputWaterImage::ImageDimension);
            itkStaticConstMacro(InputPressureImageDimension, unsigned int,
                    TInputPressureImage::ImageDimension);
            itkStaticConstMacro(OutputImageDimension, unsigned int,
                    TOutputTOASurfImage::ImageDimension);

#ifdef ITK_USE_CONCEPT_CHECKING
                /** Begin concept checking */
                itkConceptMacro(SameDimensionCheck1,
                        (itk::Concept::SameDimension<InputTOAImageDimension, InputWaterImageDimension>));
                itkConceptMacro(SameDimensionCheck2,
                        (itk::Concept::SameDimension<InputTOAImageDimension, InputPressureImageDimension>));
                itkConceptMacro(SameDimensionCheck3,
                        (itk::Concept::SameDimension<InputTOAImageDimension, OutputImageDimension>));
                /** End concept checking */
#endif

            protected:
                TOACorrectionImageFilter();
                virtual
                ~TOACorrectionImageFilter()
                {
                }

                /** Overload GenerateOutputInformation method. */
                virtual void
                GenerateOutputInformation();

                /** Validate the presence of all three inputs. If one or more inputs
                 * are missing, throw an exception. */
                void
                BeforeThreadedGenerateData();

                /** TOACorrectionImageFilter can be implemented as a multithreaded filter.
                 * Therefore, this implementation provides a ThreadedGenerateData() routine
                 * which is called for each processing thread. The output image data is
                 * allocated automatically by the superclass prior to calling
                 * ThreadedGenerateData().  ThreadedGenerateData can only write to the
                 * portion of the output image specified by the parameter
                 * "outputRegionForThread"
                 *
                 * \sa ImageToImageFilter::ThreadedGenerateData(),
                 *     ImageToImageFilter::GenerateData()  */
                void
                ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

                /** Overload PrintSelf method. */
                virtual void
                PrintSelf(std::ostream& os, itk::Indent indent) const;

            private:
                TOACorrectionImageFilter(const Self&); //purposely not implemented
                void
                operator=(const Self&);//purposely not implemented

                FunctorType m_Functor;
                /** Water Image is optionnal, use this boolean to know if the input was set or not. */
                bool m_IsWaterImage;
                /** Water amount default value. */
                double m_WaterAmountDefaultValue;
                /** L2 No_Data value */
                RealNoDataType m_L2NoData;

            };

        }
        // end namespace itk

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsTOACorrectionImageFilter.txx"
#endif

#endif
