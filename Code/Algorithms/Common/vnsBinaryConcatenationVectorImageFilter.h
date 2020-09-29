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
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 21 novembre 2013 : Correction de l'information géographique*
 * 									dans les produits. Ajout de l'origine, du spacing et de la projection   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 6 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsBinaryConcatenationVectorImageFilter_h
#define __vnsBinaryConcatenationVectorImageFilter_h

#include "itkImageToImageFilter.h"

namespace vns
{
/** \class  BinaryConcatenationVectorImageFilter
 * \brief This class concatenates several masks, that are contains in a unique image
 * (one mask per band), in a mono channel image. Each bit corresponds to one band.
 *
 * The number of bands of the input image must be less than or equal to the number
 * of bits of the output image.
 * 
 * Whatever the type of the input image, the pixel values greater than or equal to one
 * are reduced to one and the pixel values lower than one are set to zero.
 *
 * \author CS Systemes d'Information
 *
 * \sa ImageToImageFilter
 *
 * \ingroup L2
 *
 */
template <class TInputImage, class TOutputImage>
class BinaryConcatenationVectorImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
    /** Standard class typedefs. */
    typedef BinaryConcatenationVectorImageFilter                                       Self;
    typedef itk::ImageToImageFilter<TInputImage,TOutputImage>      Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(BinaryConcatenationVectorImageFilter, ImageToImageFilter );

    /** Some convenient typedefs. */
    typedef typename Superclass::InputImageType   InputImageType;
    typedef typename InputImageType::ConstPointer InputImageConstPointer;
    typedef typename InputImageType::Pointer 	  InputImagePointer;
    typedef typename InputImageType::RegionType   RegionType;
    typedef typename InputImageType::PixelType    InputImagePixelType;
    typedef typename InputImageType::SizeType     SizeType;
    typedef typename InputImageType::InternalPixelType InputInternalPixelType;
    typedef typename Superclass::OutputImageType  OutputImageType;
    typedef typename OutputImageType::Pointer     OutputImagePointer;
    typedef typename OutputImageType::PixelType   OutputImagePixelType;
    typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;

protected:
    /** Constructor */
    BinaryConcatenationVectorImageFilter();
    /** Destructor */
    virtual ~BinaryConcatenationVectorImageFilter();
    /** Test if the number of bands is consistent with the type of the output image */
    virtual void BeforeThreadedGenerateData ();
    /** Multi-thread version GenerateData. */
    void ThreadedGenerateData (const RegionType& outputRegionForThread, itk::ThreadIdType threadId);
    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    /** Generate data method */
    virtual void GenerateOutputInformation();

private:
    BinaryConcatenationVectorImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

};

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsBinaryConcatenationVectorImageFilter.txx"
#endif

#endif /* __vnsBinaryConcatenationVectorImageFilter_h */
