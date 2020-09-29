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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 01 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsExpandThresholdVectorImageFilter_h
#define __vnsExpandThresholdVectorImageFilter_h

#include "itkCastImageFilter.h"
#include "vnsPadAndResampleImageFilter.h"
#include "vnsBinaryThresholdVectorImageFilter.h"
#include "vnsMacro.h"

namespace vns
{

/** \class ExpandThresholdVectorImageFilter
 * \brief chain caster -> expander -> thresholder
 * To produce expanded vector image of unsigned int inputs for example
 *
 * \sa CastImageFilter
 * \sa ExpandImageFilter
 * \sa BinaryThresholdImageFilter
 *
 * \ingroup Common
 */
template <class TInputVectorImage, class TOutputVectorImage>
class ExpandThresholdVectorImageFilter :
    public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ExpandThresholdVectorImageFilter                       Self;
  typedef itk::ProcessObject                                     Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ExpandThresholdVectorImageFilter, ProcessObject );


  /** Some convenient typedefs. */
    typedef TInputVectorImage                               InputVectorImageType;
    typedef typename InputVectorImageType::ConstPointer     InputVectorImageConstPointer;
    typedef typename InputVectorImageType::Pointer          InputVectorImagePointer;
    typedef typename InputVectorImageType::PixelType        InputPixelType;

    typedef TOutputVectorImage                              OutputVectorImageType;
    typedef typename OutputVectorImageType::Pointer         OutputVectorImagePointer;
    typedef typename OutputVectorImageType::PixelType       OutputPixelType;
    typedef typename OutputVectorImageType::InternalPixelType       OutputInternalPixelType;


    typedef VNSRealVectorImageType::PixelType VNSRealVectorPixelType;

    /** CastFilter used to manipulate real data */
    typedef itk::CastImageFilter<InputVectorImageType, VNSRealVectorImageType> RealCasterImageFilterType;
    typedef typename RealCasterImageFilterType::Pointer RealCasterImageFilterPointer;

    /** Expand only real images */
    typedef PadAndResampleImageFilter<VNSRealVectorImageType, VNSRealVectorImageType> RealExpandFilterType;
    typedef typename RealExpandFilterType::Pointer  RealExpandFilterPointer;

    /* Binary Thresholder */
    typedef vns::BinaryThresholdVectorImageFilter<VNSRealVectorImageType,OutputVectorImageType> BinaryThresholdFilterType;
    typedef typename BinaryThresholdFilterType::Pointer BinaryThresholdFilterPointer;

    vnsMemberAndSetAndGetConstReferenceMacro(EdgePaddingValue, VNSRealPixelType)

    otbSetObjectMemberMacro(Thresholder, Upper, VNSRealPixelType)
    otbSetObjectMemberMacro(Thresholder, InsideValue, OutputInternalPixelType)
    otbSetObjectMemberMacro(Thresholder, OutsideValue, OutputInternalPixelType)

    otbGetObjectMemberMacro(Thresholder, Upper, VNSRealPixelType)
    // V 4.2 - ITK4
    otbGetObjectMemberMacro(Thresholder, InsideValue, OutputInternalPixelType)
    otbGetObjectMemberMacro(Thresholder, OutsideValue, OutputInternalPixelType)

    // Input 0
    vnsSetGetInputRawMacro(Input0 , InputVectorImageType, 0)

    // Output0
    itkGetObjectMacro(Output0,OutputVectorImageType);

    vnsSetDirectMacro(AreaToOutputImageResolution, AreaType)

    /** Composite Filter method. */
    virtual void UpdateWithCaching();

protected:
  ExpandThresholdVectorImageFilter();
  virtual ~ExpandThresholdVectorImageFilter() {};


private:
  ExpandThresholdVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



  RealCasterImageFilterPointer m_Caster;
  RealExpandFilterPointer m_Expander;
  BinaryThresholdFilterPointer m_Thresholder;

  OutputVectorImagePointer m_Output0;

  AreaType m_AreaToOutputImageResolution;


};

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsExpandThresholdVectorImageFilter.txx"
#endif

#endif

