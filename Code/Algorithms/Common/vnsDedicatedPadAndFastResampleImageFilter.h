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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 24 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsDedicatedPadAndFastResampleImageFilter.h 9591 2015-03-20 17:29:33Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDedicatedPadAndFastResampleImageFilter_h
#define __vnsDedicatedPadAndFastResampleImageFilter_h

#include "vnsDedicatedFastResampleImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkZeroFluxNeumannPadImageFilter.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsResamplerHelper.h"
#include "itkProcessObject.h"

namespace vns
{

/** \class DedicatedPadAndFastResampleImageFilter
 * \brief chain pad -> resample
 *
 * \sa PadImageFilter
 * \sa StreamingResampleImageFilter
 *
 * \ingroup Common
 */
template <class TInputImage, class TOutputImage>
class DedicatedPadAndFastResampleImageFilter :
    public /*itk::ImageToImageFilter<TInputImage, TOutputImage> */ itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef DedicatedPadAndFastResampleImageFilter                             Self;
  typedef itk::ProcessObject                                     Superclass;
//  typedef itk::ImageToImageFilter<TInputImage,TOutputImage>      Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(DedicatedPadAndFastResampleImageFilter, ProcessObject );


  /** Some convenient typedefs. */
    typedef TInputImage                                     InputImageType;
    typedef typename InputImageType::ConstPointer           InputImageConstPointer;
    typedef typename InputImageType::Pointer                InputImagePointer;
    typedef typename InputImageType::PixelType              InputPixelType;

    typedef TOutputImage                                    OutputImageType;
    typedef typename OutputImageType::Pointer               OutputImagePointer;
    typedef typename OutputImageType::PixelType             OutputPixelType;

    /** PadFilter used to manipulate real data */
    typedef itk::ZeroFluxNeumannPadImageFilter<InputImageType, InputImageType> PadFilterType;
    typedef typename PadFilterType::Pointer PadFilterPointer;

    /** Expand only real images */
    typedef DedicatedFastResampleImageFilter<InputImageType, OutputImageType> ResamplerFilterType;
    typedef typename ResamplerFilterType::Pointer  ResamplerFilterPointer;

    // Input 0
    vnsSetGetInputRawMacro(Input0, InputImageType, 0)

    virtual OutputImageType * GetOutput(void)
    {
        return m_OutputResampler; //m_Resampler->GetOutput();
    }

    typedef typename ResamplerHelper::InterpolatorType InterpolatorValueType;

    vnsMemberAndSetAndGetConstReferenceMacro(InterpolatorValue, InterpolatorValueType)

    vnsMemberAndDirectSetAndGetConstReferenceMacro(AreaToOutputImageResolution, AreaType)

    /** Composite Filter method. */
    virtual void UpdateWithCaching();

protected:
  DedicatedPadAndFastResampleImageFilter();
  virtual ~DedicatedPadAndFastResampleImageFilter() {};

  /** Composite Filter method. */
//  virtual void GenerateData();


private:
  DedicatedPadAndFastResampleImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  PadFilterPointer m_Pader;
  ResamplerFilterPointer m_Resampler;
  OutputImagePointer m_OutputResampler;

};


#define vnsImagePadAndFastDedicatedResamplerMacro(comment, resampler, inputPointer, interpolator, output_area )  \
        { \
            /* First, get input image parameters */ \
            if (!(inputPointer)) \
            { \
                vnsStaticExceptionBusinessMacro("Internal error: In the ResamplerHelper, the input image pointer is null.") \
            } \
			(resampler)->SetInput0(inputPointer); \
            (resampler)->SetAreaToOutputImageResolution(output_area); \
            (resampler)->SetInterpolatorValue(interpolator); \
            (resampler)->UpdateWithCaching(); \
        }

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsDedicatedPadAndFastResampleImageFilter.txx"
#endif

#endif

