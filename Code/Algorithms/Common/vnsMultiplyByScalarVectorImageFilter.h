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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMultiplyByScalarImageFilter_h
#define __vnsMultiplyByScalarImageFilter_h

#include "itkImageToImageFilter.h"
#include "vnsMacro.h"

namespace vns
{
/** \class  MultiplyByScalarToVectorImageFilter
 * \brief Multiply each component of a vector image by a given scalar
 *
 *
 * \author CS Systemes d'Information
 *
 * \sa ImageToImageFilter
 * \sa MultiplyByScalarVectorImageFilter
 *
 *
 */
template <class TInputImage, class TOutputImage>
class MultiplyByScalarVectorImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
    /** Standard class typedefs. */
    typedef MultiplyByScalarVectorImageFilter                              Self;
    typedef itk::ImageToImageFilter<TInputImage,TOutputImage>      Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(MultiplyByScalarVectorImageFilter, ImageToImageFilter );

    /** Some convenient typedefs. */
    typedef typename Superclass::InputImageType   InputImageType;
    typedef typename InputImageType::ConstPointer InputImageConstPointer;
    typedef typename InputImageType::RegionType   RegionType;
    typedef typename InputImageType::PixelType    InputImagePixelType;
    typedef typename InputImageType::SizeType     SizeType;
    typedef typename Superclass::OutputImageType  OutputImageType;
    typedef typename OutputImageType::Pointer     OutputImagePointer;
    typedef typename OutputImageType::PixelType   OutputImagePixelType;
    typedef typename OutputImageType::InternalPixelType   OutputImageInternalPixelType;

    itkSetMacro(Coeff, double);

    virtual void GenerateOutputInformation();

protected:
    /** Constructor */
    MultiplyByScalarVectorImageFilter();
    /** Destructor */
    virtual ~MultiplyByScalarVectorImageFilter();
    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    /** Multi-thread version GenerateData. */
    void ThreadedGenerateData (const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
    MultiplyByScalarVectorImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /** Scalar factor top apply */
    double m_Coeff;

};

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMultiplyByScalarVectorImageFilter.txx"
#endif

#endif /* __vnsMultiplyByScalarVectorImageFilter_h */
