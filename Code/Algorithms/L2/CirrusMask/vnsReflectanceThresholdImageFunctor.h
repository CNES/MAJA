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
 * VERSION : 3.3.0 : DM : LAIG-DM-MAJA-3044-CNES : 15 mars 2019 : Integration modifications oliver hagolle  *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 27 jan. 2014 : Prise en compte du gain et de l'offset pour *
 *                                                              le calcul du masque du Cirrus               *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsBinaryThresholdVectorImageFilter.h 2406 2011-01-19 15:38:23Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsReflectanceThresholdImageFilter_h
#define __vnsReflectanceThresholdImageFilter_h

#include "vnsMacro.h"


namespace vns
{
    /** \class  ReflectanceThresholdImageFunctor
     * \brief This class applies a threshold to a reflectance image and generate the associated mask.
     *
     * The threshold value is computed for each pixel
     * It tests if the value of the input pixel is greater than a threshold value.
     * If it is true, this functor sets the value of the output pixel to m_OutputValue,
     * else it sets the value of the output pixel to m_InsideValue.
     * The input image and the output mask are otb::Image.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputImagePixel, class TInputDTMPixel, class TOutputPixel>
        class ReflectanceThresholdImageFunctor
        {
            public:
                typedef TInputImagePixel InputImagePixelType;
                typedef TInputDTMPixel InputDTMPixelType;
                typedef TOutputPixel OutputPixelType;
                typedef ReflectanceThresholdImageFunctor<InputImagePixelType, InputDTMPixelType, OutputPixelType> ReflectanceThresholdImageFunctorType;


                ReflectanceThresholdImageFunctor()
                {
                    m_ThresholdOffset = static_cast<OutputPixelType>(0);
                    m_ThresholdGain = static_cast<OutputPixelType>(0);
                    m_AltRef = static_cast<OutputPixelType>(0);
                    m_InsideValue = static_cast<OutputPixelType>(0);
                    m_OutputValue = static_cast<OutputPixelType>(1);
                }


                virtual ~ReflectanceThresholdImageFunctor()
                {
                    // nothing to do
                }


                virtual void Modified() const
                {
                    // nothing to do
                }


                /** Declare m_ThresholdOffset variables and generate get and set functions on it. */
                vnsMemberAndSetAndGetConstReferenceMacro( ThresholdOffset, InputImagePixelType );
                /** Declare m_ThresholdGain variables and generate get and set functions on it. */
                vnsMemberAndSetAndGetConstReferenceMacro( ThresholdGain, InputImagePixelType );
                /** Declare m_InsideValue variables and generate get and set functions on it. */
                vnsMemberAndSetAndGetConstReferenceMacro( InsideValue, OutputPixelType );
                /** Declare m_OutputValue variables and generate get and set functions on it. */
                vnsMemberAndSetAndGetConstReferenceMacro( OutputValue, OutputPixelType );
                /** Declare m_ThresholdOffset variables and generate get and set functions on it. */
                vnsMemberAndSetAndGetConstReferenceMacro( AltRef, InputImagePixelType );


                inline OutputPixelType
                operator()( InputImagePixelType inPix, InputDTMPixelType inDtm ) const
                {
                    OutputPixelType outPix;

                    // Set the inside value to the pixel of the output mask
                    outPix = m_InsideValue;
		    //Modif OH, use a quadratic fomula instead of linear. Division by 2000 to keep same threshold value  at that altitude.
		    //The threshold will be lower for altitudes below 2000, and higher for altitudes above 2000

                    if(m_AltRef!=0)
                    {
                    	if( inPix > ( m_ThresholdGain * inDtm * inDtm / m_AltRef + m_ThresholdOffset ))
                    	 {
                    	 	outPix = m_OutputValue;
                    	 }
                    }
                    else
                    {
                    	vnsLogWarningMacro("The reference altitude value is not correct : it cannot be equal to zero !")
                    }


                    return outPix;
                }

            protected:
            };

    } // end namespace functor
  
} // end namespace vns

  
#endif
