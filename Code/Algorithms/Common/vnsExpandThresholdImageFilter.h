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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsExpandThresholdImageFilter_h
#define __vnsExpandThresholdImageFilter_h

#include "itkCastImageFilter.h"
#include "vnsPadAndResampleImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "vnsMacro.h"

namespace vns
{

/** \class ExpandThresholdImageFilter
 * \brief chain caster -> expander -> thresholder
 * To produce expanded image of unsigned int inputs for example
 *
 * \sa CastImageFilter
 * \sa BinaryThresholdImageFilter
 *
 * \ingroup Common
 */
template <class TInputImage, class TOutputImage>
class ExpandThresholdImageFilter :
    public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ExpandThresholdImageFilter                             Self;
  typedef itk::ProcessObject                                     Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ExpandThresholdImageFilter, ProcessObject );


  /** Some convenient typedefs. */
    typedef TInputImage                                     InputImageType;
    typedef typename InputImageType::ConstPointer           InputImageConstPointer;
    typedef typename InputImageType::Pointer                InputImagePointer;
    typedef typename InputImageType::PixelType              InputPixelType;

    typedef TOutputImage                                    OutputImageType;
    typedef typename OutputImageType::Pointer               OutputImagePointer;
    typedef typename OutputImageType::PixelType             OutputPixelType;

    typedef VNSRealImageType::PixelType                      VNSRealPixelType;

    /** CastFilter used to manipulate real data */
    typedef itk::CastImageFilter<InputImageType, VNSRealImageType> RealCasterImageFilterType;
    typedef typename RealCasterImageFilterType::Pointer RealCasterImageFilterPointer;

    /** Expand only real images */
    typedef PadAndResampleImageFilter<VNSRealImageType, VNSRealImageType> RealExpandFilterType;
    typedef typename RealExpandFilterType::Pointer  RealExpandFilterPointer;

    /* Binary Thresholder */
    typedef itk::BinaryThresholdImageFilter<VNSRealImageType,OutputImageType> BinaryThresholdFilterType;
    typedef typename BinaryThresholdFilterType::Pointer BinaryThresholdFilterPointer;

    vnsMemberAndSetAndGetConstReferenceMacro(EdgePaddingValue, VNSRealPixelType)

    otbSetObjectMemberMacro(Thresholder, UpperThreshold, VNSRealPixelType)
    otbSetObjectMemberMacro(Thresholder, InsideValue, OutputPixelType)
    otbSetObjectMemberMacro(Thresholder, OutsideValue, OutputPixelType)

    otbGetObjectMemberMacro(Thresholder, UpperThreshold, VNSRealPixelType)
    otbGetObjectMemberMacro(Thresholder, InsideValue, OutputPixelType)
    otbGetObjectMemberMacro(Thresholder, OutsideValue, OutputPixelType)

    // Input 0
    vnsSetGetInputRawMacro(Input0 , InputImageType, 0)

    // Output0
    itkGetObjectMacro(Output0,OutputImageType);

    vnsSetDirectMacro(AreaToOutputImageResolution, AreaType)

    /** Composite Filter method. */
    virtual void UpdateWithCaching();

protected:
  ExpandThresholdImageFilter();
  virtual ~ExpandThresholdImageFilter() {};


private:
  ExpandThresholdImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

 

  RealCasterImageFilterPointer m_Caster;
  RealExpandFilterPointer m_Expander;
  BinaryThresholdFilterPointer m_Thresholder;

  OutputImagePointer m_Output0;

  AreaType m_AreaToOutputImageResolution;


};

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsExpandThresholdImageFilter.txx"
#endif

#endif

