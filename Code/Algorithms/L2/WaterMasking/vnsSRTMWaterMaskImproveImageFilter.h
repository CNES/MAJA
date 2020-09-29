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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSRTMWaterMaskImproveImageFilter_h
#define __vnsSRTMWaterMaskImproveImageFilter_h

#include "itkImageToImageFilter.h"
#include "vnsMacro.h"

namespace vns
{
/** \class  SRTMWaterMaskImproveImageFilter
 * \brief This class implements .
 *
 * 
 *
 * \author CS Systemes d'Information
 *
 * \sa ImageToImageFilter
 *
 * \ingroup L2
 *
 */
template <class TInputMask, class TVectorMask, class TSRTMMask, class TOutputImage>
class SRTMWaterMaskImproveImageFilter : public itk::ImageToImageFilter<TInputMask,TOutputImage>
{
public:
    /** Standard class typedefs. */
    typedef SRTMWaterMaskImproveImageFilter Self;
    typedef itk::ImageToImageFilter<TInputMask,TOutputImage> Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(SRTMWaterMaskImproveImageFilter, ImageToImageFilter );

    /** Some convenient typedefs. */
    typedef typename Superclass::InputImageType InputMaskType;
    typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
    typedef typename InputMaskType::RegionType RegionType;
    typedef typename InputMaskType::PixelType InputMaskPixelType;
    typedef typename InputMaskType::SizeType SizeType;

    typedef TVectorMask VectorMaskType;
    typedef typename VectorMaskType::Pointer VectorMaskPointer;
    typedef typename VectorMaskType::ConstPointer VectorMaskConstPointer;
    typedef typename VectorMaskType::PixelType VectorMaskPixelType;
    typedef typename VectorMaskType::InternalPixelType VectorMaskInternalPixelType;

    typedef TSRTMMask SRTMMaskType;
    typedef typename SRTMMaskType::Pointer SRTMMaskPointer;
    typedef typename SRTMMaskType::ConstPointer SRTMMaskConstPointer;
    typedef typename SRTMMaskType::PixelType SRTMMaskPixelType;

    typedef typename Superclass::OutputImageType OutputImageType;
    typedef typename OutputImageType::Pointer OutputImagePointer;
    typedef typename OutputImageType::PixelType OutputImagePixelType;
    typedef typename OutputImageType::RegionType OutputImageRegionType;

    // Init mode parameter accessors
    itkSetMacro(InitMode, bool);
    itkGetConstReferenceMacro(InitMode, bool);

    // Bonus SRTM water mask accessors
    itkSetMacro(SRTMPercent, double);
    itkGetConstReferenceMacro(SRTMPercent, double);

    // Number of water mask accessors
    itkSetMacro(WaterMaskNumber, unsigned int);
    itkGetConstReferenceMacro(WaterMaskNumber, unsigned int);

    // Accessors of minimum percent to declare a pixel as water
    itkSetMacro(MinPercentPossibleWater, double);
    itkGetConstReferenceMacro(MinPercentPossibleWater, double);

    /** Set the possible water mask */
    vnsSetGetInputMacro( IPPWA, InputMaskType, 0)
    /** Set the tested pixel mask */
    vnsSetGetInputMacro( IPTWA, InputMaskType, 1)
    /** Set the SRTM water mask */
    vnsSetGetInputMacro( SRTMWAT, SRTMMaskType, 2)
    /** Set the possible water mask */
    vnsSetGetInputMacro( L2inPWA, VectorMaskType, 3)
    /** Set the tested pixel mask */
    vnsSetGetInputMacro( L2inTWA, VectorMaskType, 4)

    /** Get the final water mask */
    OutputImageType* GetL2WASOutput()
    {
        return static_cast<OutputImageType*>( this->itk::ProcessObject::GetOutput(0) );
    }

    /** Get the updated possible water flag */
    VectorMaskType* GetL2PWAOutput()
    {
        return static_cast<VectorMaskType*>( this->itk::ProcessObject::GetOutput(1) );
    }

    /** Get the updated tested pixel flag */
    VectorMaskType* GetL2TWAOutput()
    {
        return static_cast<VectorMaskType*>( this->itk::ProcessObject::GetOutput(2) );
    }

protected:
    /** Constructor */
    SRTMWaterMaskImproveImageFilter();
    /** Destructor */
    virtual ~SRTMWaterMaskImproveImageFilter();
    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    /** Generate output information. */
    virtual void GenerateOutputInformation(void);

    /** BeforeThreaded generate data. */
    virtual void BeforeThreadedGenerateData(void);

    /** Main Filter method. */
    virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
    SRTMWaterMaskImproveImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&);//purposely not implemented

    /** Init_mode option declaration */
    bool m_InitMode;
    /** Bonus SRTM water mask declaration */
    double m_SRTMPercent;
    /** Number of mask available in the PWA and TWA masks (N_WM) */
    unsigned int m_WaterMaskNumber;
    /** Minimum percent of tested times to declare a pixel as water */
    double m_MinPercentPossibleWater;

};

}
 // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsSRTMWaterMaskImproveImageFilter.txx"
#endif

#endif /* __vnsSRTMWaterMaskImproveImageFilter_h */
