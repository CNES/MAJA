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
#ifndef __vnsBinaryToVectorImageFilter_h
#define __vnsBinaryToVectorImageFilter_h

#include "itkImageToImageFilter.h"
#include "vnsMacro.h"

namespace vns
{
/** \class  BinaryToVectorImageFilter
 * \brief This class read a mask (mono image with each bit corresponds to one value) and extract each value
 * to a multi band image.
 *
 * The number of values extracted from the mask image is set with the SetNumberOfComponentsPerPixel() method.
 *
 * \author CS Systemes d'Information
 *
 * \sa ImageToImageFilter
 * \sa BinaryConcatenationVectorImageFilter
 *
 * \ingroup L2
 * \ingroup L3
 * \ingroup Checktool
 *
 */
template <class TInputImage, class TOutputImage>
class BinaryToVectorImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
    /** Standard class typedefs. */
    typedef BinaryToVectorImageFilter                              Self;
    typedef itk::ImageToImageFilter<TInputImage,TOutputImage>      Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(BinaryToVectorImageFilter, ImageToImageFilter );

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

    itkSetMacro(NumberOfComponentsPerPixel, unsigned int)
    itkGetConstReferenceMacro(NumberOfComponentsPerPixel, unsigned int)

    virtual void GenerateOutputInformation();

protected:
    /** Constructor */
    BinaryToVectorImageFilter();
    /** Destructor */
    virtual ~BinaryToVectorImageFilter();
    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    /** Check the pixel type size */
    void BeforeThreadedGenerateData();
    /** Multi-thread version GenerateData. */
    void ThreadedGenerateData (const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
    BinaryToVectorImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /** Number of value in the input image file (number of component by pixel for the output vector image) */
    unsigned int m_NumberOfComponentsPerPixel;

    /** Pixel byte weight */
    std::vector<unsigned short> m_BitWeight;
};

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsBinaryToVectorImageFilter.txx"
#endif

#endif /* __vnsBinaryToVectorImageFilter_h */
