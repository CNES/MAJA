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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 2 mai 2016 : Correction warning, qualite, etc             *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 août 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputeTransmissionImageFilter_h
#define __vnsComputeTransmissionImageFilter_h

#include "itkImageToImageFilter.h"
#include "vnsMacro.h"
#include "itkImageScanlineIterator.h"



namespace vns
{
/** \class  ComputeTransmissionImageFilter
 * \brief This class computes Tdir, Tdif and T, images of direct transmission, diffuse transmission
 *  and the sum of these two.
 *
 * To do this, we proceed interpolations on two LUTs with h_dtm_sub, thetaS and AOT smoothed image.
 * \author CS Systemes d'Information
 *
 * \sa ImageToImageFilter
 *
 * \ingroup L2
 *
 */
template <class TInputImage, class TInputMask, class TInputDTM, class TInputLUT, class TOutputVectorImage>
class ComputeTransmissionImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputVectorImage>
{
public:
    /** Standard class typedefs. */
    typedef ComputeTransmissionImageFilter                             Self;
    typedef itk::ImageToImageFilter<TInputImage,TOutputVectorImage>      Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(ComputeTransmissionImageFilter, ImageToImageFilter );

    /** Some convenient typedefs. */
    typedef TInputImage                               InputImageType;
    typedef typename InputImageType::ConstPointer     InputImageConstPointerType;
    typedef typename InputImageType::Pointer          InputImagePointerType;
    typedef typename InputImageType::RegionType       InputImageRegionType;
    typedef typename InputImageType::PixelType        InputImagePixelType;
    typedef typename InputImageType::InternalPixelType InternalPixelType;
    typedef typename InputImageType::IndexType        IndexType;
    typedef typename InputImageType::SizeType         SizeType;
    typedef typename InputImageType::OffsetType       OffsetType;

    typedef TInputMask                                      InputMaskType;
    typedef typename InputMaskType::Pointer                 InputMaskPointerType;
    typedef typename InputMaskType::ConstPointer            InputMaskConstPointerType;
    typedef typename InputMaskType::PixelType               InputMaskPixelType;
    typedef typename InputMaskType::InternalPixelType       InputMaskInternalPixelType;

    typedef TInputDTM                                       InputDTMType;
    typedef typename InputDTMType::Pointer                  InputDTMPointerType;
    typedef typename InputDTMType::ConstPointer             InputDTMConstPointerType;
    typedef typename InputDTMType::PixelType                InputDTMPixelType;
    typedef typename InputDTMType::InternalPixelType        InputDTMInternalPixelType;

    typedef TInputLUT                                       LUTType;
    typedef typename LUTType::Pointer                       LUTPointerType;
    typedef typename LUTType::PixelType                     LUTPixelType;
    typedef typename LUTType::ParameterValuesType           ParameterValuesType;
    typedef typename LUTType::PointType                     LUTPointType;
    typedef typename LUTPointType::ValueType                LUTPointValueType;
    typedef std::map<unsigned int, LUTPointerType>          LUTMapType;

    typedef TOutputVectorImage                              OutputVectorImageType;
    typedef typename OutputVectorImageType::Pointer         OutputVectorImagePointerType;
    typedef typename OutputVectorImageType::RegionType      OutputVectorImageRegionType;
    typedef typename OutputVectorImageType::PixelType       OutputVectorImagePixelType;
    typedef typename OutputVectorImageType::IndexType       OutputVectorImageIndexType;
    typedef typename OutputVectorImageType::SizeType        OutputVectorImageSizeType;


    /** Iterators */
    typedef typename itk::ImageScanlineConstIterator<InputDTMType>            DTMRegionConstIteratorType;
    typedef typename itk::ImageScanlineConstIterator<InputImageType>          ImageRegionConstIteratorType;
    typedef typename itk::ImageScanlineConstIterator<InputMaskType>           MaskRegionConstIteratorType;

    typedef typename itk::ImageScanlineIterator<OutputVectorImageType>        VectorImageRegionIteratorType;


    // Input 0 -> transmitted to InternalSlopeCorrection
    vnsSetGetInputRawMacro( IPAOTSub, InputImageType, 0)
    vnsSetGetInputRawMacro( DTMSub, InputDTMType, 1)
    vnsSetGetInputRawMacro( IPEDG, InputMaskType, 2)

    // Output
    OutputVectorImageType* GetTdir();

    // Output 1
    OutputVectorImageType* GetTdif();


    // LUTs accessors
    itkSetObjectMacro(LUT_Tdir,LUTType);
    itkSetObjectMacro(LUT_Tdif,LUTType);

    // Angle
    itkSetMacro(ThetaS, double);
    itkGetConstReferenceMacro(ThetaS, double);

    // NbOfBands
    itkSetMacro(NbOfBands, unsigned int);
    itkGetConstReferenceMacro(NbOfBands, unsigned int);


protected:
    /** Constructor */
    ComputeTransmissionImageFilter();
    /** Destructor */
    virtual ~ComputeTransmissionImageFilter();
    /** Validate the presence of all inputs. If at least one input
     * is missing, it throws an exception. */
    void BeforeThreadedGenerateData();
    /** Multi-thread version GenerateData. */
    void ThreadedGenerateData (const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    virtual void GenerateOutputInformation(void)
    {
        Superclass::GenerateOutputInformation();
        this->GetTdir()->SetNumberOfComponentsPerPixel(m_NbOfBands);
        this->GetTdif()->SetNumberOfComponentsPerPixel(m_NbOfBands);
    }

private:
    ComputeTransmissionImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented



    // Direct Transmission LUT (3D)
    LUTPointerType m_LUT_Tdir;
    // Diffuse Transmission LUT (3D)
    LUTPointerType m_LUT_Tdif;

    double m_ThetaS;

    unsigned int m_NbOfBands;


};

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsComputeTransmissionImageFilter.txx"
#endif

#endif /* __vnsComputeTransmissionImageFilter_h */
