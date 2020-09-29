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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-354-CNES : 8 juin 2012 : Correction qualite (regle ou metrique)       *
 *                                                                                                          *
 * MOD : VERSION : 1-0-0  : 23 nov. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsZenithAndAzimuthAnglesToGridFunctor_h
#define __vnsZenithAndAzimuthAnglesToGridFunctor_h

#include "itkBinaryFunctorImageFilter.h"

namespace vns
{
    /** \class  ZenithAndAzimuthAnglesToGridFunctor
     * \brief This class implements .
     *
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
    namespace Functor
    {
        template<class TInputPixel1, class TInputPixel2, class TOutputPixel>
            class ZenithAndAzimuthAnglesToGridFunctor
            {
            public:
                typedef ZenithAndAzimuthAnglesToGridFunctor<TInputPixel1, TInputPixel2, TOutputPixel>
                        ZenithAndAzimuthAnglesToGridFunctorType;

                ZenithAndAzimuthAnglesToGridFunctor()
                {
                    m_OutputSize = 2;
                }

                virtual
                ~ZenithAndAzimuthAnglesToGridFunctor()
                {
                }

                typedef TInputPixel1 InputPixel1Type;
                typedef TInputPixel2 InputPixel2Type;
                typedef TOutputPixel OutputPixelType;
                typedef typename OutputPixelType::ValueType OutputValueType;

                /** Get the output size of the output mask */
                unsigned int
                GetOutputSize(void)
                {
                    return m_OutputSize;
                }

                inline OutputPixelType
                operator()(InputPixel1Type zen, InputPixel2Type azi) const
                {
                    OutputPixelType outPix;
                    outPix.SetSize(2);

                    const double l_zenRad = zen * otb::CONST_PI_180;
                    const double l_aziRad = azi * otb::CONST_PI_180;

                    outPix[0] = static_cast<OutputValueType> (std::tan(l_zenRad) * std::sin(l_aziRad));
                    outPix[1] = static_cast<OutputValueType> (std::tan(l_zenRad) * std::cos(l_aziRad));

                    return outPix;
                }

            private:
                unsigned int m_OutputSize;
            };
    }// end namespace functor


template<class TInputImage1, class TInputImage2, class TOutputImage>
class ZenithAndAzimuthAnglesToGridFilter : public itk::BinaryFunctorImageFilter<TInputImage1, TInputImage2,
TOutputImage, Functor::ZenithAndAzimuthAnglesToGridFunctor<typename TInputImage1::PixelType,
typename TInputImage2::PixelType, typename TOutputImage::PixelType> >
{
public:
    /** Standard class typedefs. */
    typedef ZenithAndAzimuthAnglesToGridFilter Self;
    typedef itk::BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage, Functor::ZenithAndAzimuthAnglesToGridFunctor<
    typename TInputImage1::PixelType, typename TInputImage2::PixelType, typename TOutputImage::PixelType> > Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self)

    /** Run-time type information (and related methods). */
    itkTypeMacro(ZenithAndAzimuthAnglesToGridFilter, itk::BinaryFunctorImageFilter)

protected:
    /** Constructor */
    ZenithAndAzimuthAnglesToGridFilter()
    {};
    /** Destructor */
    virtual ~ZenithAndAzimuthAnglesToGridFilter()
    {};
    /** Generate the output information missing */
    virtual void
    GenerateOutputInformation()
    {
        Superclass::GenerateOutputInformation();
        typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
        outputPtr->SetNumberOfComponentsPerPixel(this->GetFunctor().GetOutputSize());
    }

private:
    ZenithAndAzimuthAnglesToGridFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
};

} // End namespace vns


#endif /* __vnsZenithAndAzimuthAnglesToGridFunctor_h */
