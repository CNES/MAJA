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
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 27 jan. 2014 : Utilisation du filtre de calcul de l'angle  *
 *                                                              d'incidence solaire                         *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsInternalSlopeCorrectionImageFilter_h
#define __vnsInternalSlopeCorrectionImageFilter_h

#include "vnsMacro.h"
#include "vnsIncidenceAngleFunctor.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkImageToImageFilter.h"
#include "itkImageScanlineIterator.h"
#include "itkImageScanlineConstIterator.h"

namespace vns
{
/** \class  InternalSlopeCorrectionImageFilter
 * \brief This class compute the reflectance surface after atmospheric correction
 * (including environment) and topographic correction
 *
 * It computes incidence angle and exitence angle.
 * It tests several conditions on the computed angles and then uses them to compute
 * three variables :
 * -Fvisibleground
 * -Fvisiblesky
 * -Fbrdf
 *
 * Finally the ImageFilter produces the image of  surface reflectance with topographic correction.
 *
 * \author CS Systemes d'Information
 *
 * \ingroup L2
 *
 */

template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
class InternalSlopeCorrectionImageFilter : public itk::ImageToImageFilter<TInputVectorImage,TOutputVectorImage>
{
public:
    /** Standard class typedefs. */
    typedef InternalSlopeCorrectionImageFilter              Self;
    typedef itk::ImageToImageFilter<TInputVectorImage,TOutputVectorImage>
                                                            Superclass;
    typedef itk::SmartPointer<Self>                         Pointer;
    typedef itk::SmartPointer<const Self>                   ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(vnsInternalSlopeCorrectionImageFilter, ImageToImageFilter );

    /** Some convenient typedefs. */
    typedef TInputVectorImage                               InputVectorImageType;
    typedef typename InputVectorImageType::Pointer          InputVectorImagePointerType;
    typedef typename InputVectorImageType::ConstPointer     InputVectorImageConstPointer;
    typedef typename InputVectorImageType::RegionType       RegionType;
    typedef typename InputVectorImageType::IndexType        IndexType;
    typedef typename InputVectorImageType::PixelType        PixelType;
    typedef typename InputVectorImageType::InternalPixelType InternalPixelType;
    typedef typename InputVectorImageType::SizeType         SizeType;
    typedef typename InputVectorImageType::OffsetType       OffsetType;

    typedef TInputImage                                     InputImageType;
    typedef typename InputImageType::Pointer                InputImagePointerType;
    typedef typename InputImageType::ConstPointer           InputImageConstPointer;
    typedef typename InputImageType::RegionType             InputImageRegionType;
    typedef typename InputImageType::PixelType              InputImagePixelType;

    typedef TInputIncidenceAngleImage                               InternalIncidenceAngleImageType;
    typedef typename InternalIncidenceAngleImageType::Pointer       InternalIncidenceAngleImagePointerType;
    typedef typename InternalIncidenceAngleImageType::ConstPointer  InternalIncidenceAngleImageConstPointer;
    typedef typename InternalIncidenceAngleImageType::RegionType    InternalIncidenceAngleImageRegionType;
    typedef typename InternalIncidenceAngleImageType::PixelType     InternalIncidenceAngleImagePixelType;

    typedef TOutputVectorImage                              OutputVectorImageType;
    typedef typename OutputVectorImageType::Pointer         OutputVectorImagePointerType;
    typedef typename OutputVectorImageType::ConstPointer    OutputVectorImageConstPointer;
    typedef typename OutputVectorImageType::PixelType       OutputVectorImagePixelType;
    typedef typename OutputVectorImageType::RegionType      OutputVectorImageRegionType;


    typedef TOutputMask                                     MaskType;
    typedef typename MaskType::Pointer                      MaskPointerType;
    typedef typename MaskType::ConstPointer                 MaskConstPointer;
    typedef typename MaskType::PixelType                    MaskPixelType;
    typedef typename MaskType::RegionType                   MaskRegionType;

    /** Iterators */
    typedef typename itk::ImageScanlineIterator<MaskType>           	                MaskRegionIteratorType;
    typedef typename itk::ImageScanlineConstIterator<MaskType>                        MaskRegionConstIteratorType;
    typedef typename itk::ImageScanlineIterator<InputImageType>                       ImageRegionIteratorType;
    typedef typename itk::ImageScanlineConstIterator<InputImageType>                  ImageRegionConstIteratorType;
    typedef typename itk::ImageScanlineIterator<InternalIncidenceAngleImageType>      InternalIncidenceAngleRegionIteratorType;
    typedef typename itk::ImageScanlineConstIterator<InternalIncidenceAngleImageType> InternalIncidenceAngleRegionConstIteratorType;
    typedef typename itk::ImageScanlineIterator<InputVectorImageType>     	        VectorImageRegionIteratorType;
	typedef typename itk::ImageScanlineConstIterator<InputVectorImageType>   	        VectorImageRegionConstIteratorType;


	typedef std::vector<double> ListOfDoubles;

    /** Setters */
    itkSetMacro(ThetaS,double);
    itkSetMacro(PhiS,double);
    vnsSetMacro(ThetaV,ListOfDoubles);
    vnsSetMacro(PhiV,ListOfDoubles);
    itkSetMacro(MinCosI,double);
    itkSetMacro(MinCosE,double);
    itkSetMacro(MinCosRatio,double);
    itkSetMacro(NoData,double);


    /** Getters on outputs */
    OutputVectorImageType* GetL2FRE(); // R_surf_flat
    MaskType* GetL2STL();  // mask sun_too_low_flag
    MaskType* GetL2TGS();  // mask tangent_sun_flag

    /** Input images/mask accessors */
    vnsSetGetInputRawMacro( L2SRE,InputVectorImageType , 0)
    // Slope
    vnsSetGetInputRawMacro( S, InputImageType, 1)
    // Aspect
    vnsSetGetInputRawMacro( A, InputImageType, 2)
    // IncidenceAngle
    vnsSetGetInputRawMacro( IncidenceAngle, InternalIncidenceAngleImageType, 3)
    vnsSetGetInputRawMacro( Tdif, InputVectorImageType, 4)
    vnsSetGetInputRawMacro( Tdir, InputVectorImageType, 5)
    // Rho_env  : Mean of the image of surface reflectance after atmospheric correction
    vnsSetGetInputRawMacro( RhoEnv, InputVectorImageType, 6)

protected:
    /* Constructor */
    InternalSlopeCorrectionImageFilter();
    /** Destructor */
    virtual ~InternalSlopeCorrectionImageFilter();
    /** Validate the presence of all 6 inputs and correct sizes. If at least one input
     * is missing, or a size is not correct, throws an exception. */
    void BeforeThreadedGenerateData();
    /** Multi-thread version GenerateData. */
    void ThreadedGenerateData (const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
    InternalSlopeCorrectionImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    // Solar Angles
    double m_ThetaS;
    double m_PhiS;

    // Viewing Angles
    ListOfDoubles m_ThetaV;
    ListOfDoubles m_PhiV;

    // Incidence angle threshold
    double m_MinCosI;

    // Exitence angle threshold
    double m_MinCosE;

    // Ratio threshold
    double m_MinCosRatio;

    // NoData
    double m_NoData;


};


} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsInternalSlopeCorrectionImageFilter.txx"
#endif

#endif /* __vnsInternalSlopeCorrectionImageFilter_h */
