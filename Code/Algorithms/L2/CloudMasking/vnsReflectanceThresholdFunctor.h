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
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-1991-CNES : 29 novembre 2016 : Update CloudMasking ATCOR              *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsReflectanceThresholdFunctor_h
#define __vnsReflectanceThresholdFunctor_h

#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{
    /** \class  ReflectanceThresholdFunctor
     * \brief This class applies a threshold on blue reflectance to detect thick clouds.
     *
     * This functor takes into account an input mask.
     *
     * It tests if the pixel value of the mask is equal to m_BackgroundValue.
     * If it is true, this functor sets the value of the output pixel mask to 1.
     * else the value of the output pixel keeps 0.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TVectorMaskPixel, class TMaskPixel,
                class TOutputMaskPixel>
            class ReflectanceThresholdFunctor
            {
            public:
                typedef ReflectanceThresholdFunctor<TInputPixel,
                        TVectorMaskPixel, TMaskPixel, TOutputMaskPixel>
                        ReflectanceThresholdFunctorType;

                ReflectanceThresholdFunctor() : m_BlueBand(0),m_WaterBand(0), m_RedBand(0),
                        m_NirBand(0), m_SwirBand(0), m_hasSwirBand(false)
                {
                    m_ForegroundValue = static_cast<VectorMaskInternalPixelType>(1);
                    m_BlueReflectanceThreshold = static_cast<InputInternalPixelType>(0);
                    m_WaterReflectanceThreshold = static_cast<InputInternalPixelType>(0);
                    m_RedReflectanceThreshold = static_cast<InputInternalPixelType>(0);
                    m_NirRedRatioMaxThreshold = static_cast<InputInternalPixelType>(0);
                    m_NirRedRatioMinThreshold = static_cast<InputInternalPixelType>(0);
                    m_NirSwirRatioMinThreshold = static_cast<InputInternalPixelType>(0);
                }


                virtual ~ReflectanceThresholdFunctor()
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TVectorMaskPixel VectorMaskPixelType;
                typedef TMaskPixel MaskPixelType;
                typedef TOutputMaskPixel OutputMaskPixelType;

                typedef typename InputPixelType::ValueType
                                        InputInternalPixelType;
                typedef typename VectorMaskPixelType::ValueType
                        VectorMaskInternalPixelType;

                /** Get blue band index */
                unsigned int
                GetBlueBand()
                {
                    return m_BlueBand;
                }
                /** Set blue band index */
                void
                SetBlueBand(unsigned int val)
                {
                    m_BlueBand = val;
                }

                /** Get water band index */
                unsigned int
                GetWaterBand()
                {
                    return m_WaterBand;
                }
                /** Set water band index */
                void
                SetWaterBand(unsigned int val)
                {
                    m_WaterBand = val;
                }

                /** Get red band index */
                unsigned int
                GetRedBand()
                {
                    return m_RedBand;
                }
                /** Set red band index */
                void
                SetRedBand(unsigned int val)
                {
                    m_RedBand = val;
                }

                /** Get nir band index */
                unsigned int
                GetNirBand()
                {
                    return m_NirBand;
                }
                /** Set nir band index */
                void
                SetNirBand(unsigned int val)
                {
                    m_NirBand = val;
                }

                /** Get swir band index */
                unsigned int
                GetSwirBand()
                {
                    return m_SwirBand;
                }
                /** Set Swir band index */
                void
                SetSwirBand(unsigned int val)
                {
                    m_SwirBand = val;
                    m_hasSwirBand = true;
                }


                /** Get the foreground value of the mask */
                VectorMaskInternalPixelType
                GetForegroundValue()
                {
                    return m_ForegroundValue;
                }
                /** Set the foreground value of the mask */
                void
                SetForegroundValue(VectorMaskInternalPixelType val)
                {
                    m_ForegroundValue = val;
                }

                /** Get the blue reflectance threshold value */
                InputInternalPixelType
                GetBlueReflectanceThreshold()
                {
                    return m_BlueReflectanceThreshold;
                }
                /** Set the blue reflectance threshold value */
                void
                SetBlueReflectanceThreshold(InputInternalPixelType val)
                {
                    m_BlueReflectanceThreshold = val;
                }

                /** Get the water reflectance threshold value */
				InputInternalPixelType
				GetWaterReflectanceThreshold()
				{
					return m_WaterReflectanceThreshold;
				}
				/** Set the water reflectance threshold value */
				void
				SetWaterReflectanceThreshold(InputInternalPixelType val)
				{
					m_WaterReflectanceThreshold = val;
				}

                /** Get the red reflectance threshold value */
                const InputInternalPixelType& GetRedReflectanceThreshold() const {
                    return m_RedReflectanceThreshold;
                }

                /** Set the red reflectance threshold value */
                void SetRedReflectanceThreshold(
                        const InputInternalPixelType& redReflectanceThreshold) {
                    m_RedReflectanceThreshold = redReflectanceThreshold;
                }

				/** Get the NirRedRatio max threshold value */
				const double& GetNirRedRatioMaxThreshold() const {
					return m_NirRedRatioMaxThreshold;
				}

				/** Set the NirRedRatio max threshold value */
				void SetNirRedRatioMaxThreshold(
						const double nirRedRatioMaxThreshold) {
					m_NirRedRatioMaxThreshold = nirRedRatioMaxThreshold;
				}

				/** Get the NirRedRatio min threshold value */
				const double& GetNirRedRatioMinThreshold() const {
					return m_NirRedRatioMinThreshold;
				}

				/** Set the NirRedRatio min threshold value */
				void SetNirRedRatioMinThreshold(
						const double& nirRedRatioMinThreshold) {
					m_NirRedRatioMinThreshold = nirRedRatioMinThreshold;
				}

				/** Get the NirSwirRatio min threshold value */
				const double& GetNirSwirRatioMinThreshold() const {
					return m_NirSwirRatioMinThreshold;
				}

				/** Set the NirSwirRatio min threshold value */
				void SetNirSwirRatioMinThreshold(
						const double& nirSwirRatioMinThreshold) {
					m_NirSwirRatioMinThreshold = nirSwirRatioMinThreshold;
				}

                inline OutputMaskPixelType
                operator()(InputPixelType inPix, VectorMaskPixelType satPix, MaskPixelType ndtPix, MaskPixelType watPix) const
                {
                    const double l_Epsilon = CONST_EPSILON_20;

                    OutputMaskPixelType outPix;

                    // Initialization
                    outPix = static_cast<OutputMaskPixelType>(0);

                    // Test if the pixel value is null in the mask (not edge)
                    if (ndtPix == 0)
                    {
                        bool hasSaturated = (satPix[m_BlueBand] == m_ForegroundValue) ||
                                            (satPix[m_RedBand] == m_ForegroundValue) ||
                                            (satPix[m_NirBand] == m_ForegroundValue);
                        if (m_hasSwirBand) {
                            hasSaturated = hasSaturated || (satPix[m_SwirBand] == m_ForegroundValue);
                        }

                        if (hasSaturated ) {
                            // Set pixel to DefaultValue
                            outPix = static_cast<OutputMaskPixelType>(1);
                        } else {
                            bool hasThreshold = vnsStrictlySupDoubleMacro(inPix[m_BlueBand],m_BlueReflectanceThreshold,l_Epsilon) &&
                                    vnsStrictlySupDoubleMacro(inPix[m_RedBand],m_RedReflectanceThreshold,l_Epsilon) &&
                                    vnsStrictlyInfDoubleMacro(inPix[m_NirBand],m_NirRedRatioMaxThreshold*inPix[m_RedBand],l_Epsilon) &&
                                    vnsStrictlySupDoubleMacro(inPix[m_NirBand],m_NirRedRatioMinThreshold*inPix[m_RedBand],l_Epsilon);
                            if (m_hasSwirBand) {
                                hasThreshold = hasThreshold && vnsStrictlySupDoubleMacro(inPix[m_NirBand],m_NirSwirRatioMinThreshold*inPix[m_SwirBand],l_Epsilon);
                            }

                            if (hasThreshold)
                            {
                                // Set pixel to DefaultValue
                                outPix = static_cast<OutputMaskPixelType>(1);
                            }
                        }
                        if (vnsStrictlySupDoubleMacro(inPix[m_WaterBand],m_WaterReflectanceThreshold,l_Epsilon) && (watPix > 0))
						{
                        	// Set pixel to DefaultValue
                        	outPix = static_cast<OutputMaskPixelType>(1);
						}

                    }

                    return outPix;
                }


            protected:
                /** Blue band definition */
				unsigned int m_BlueBand;
				/** Blue band definition */
				unsigned int m_WaterBand;
				/** Red band definition */
				unsigned int m_RedBand;
				/** Nir band definition */
				unsigned int m_NirBand;
				/** Swir band definition */
				unsigned int m_SwirBand;
				/** Swir band testing  */
				bool m_hasSwirBand;
                /** Foreground value declaration */
                VectorMaskInternalPixelType m_ForegroundValue;
                /** Reflectance Threshold value declaration */
                InputInternalPixelType m_BlueReflectanceThreshold;
                /** Water Reflectance Threshold value declaration */
                InputInternalPixelType m_WaterReflectanceThreshold;
                /** RedReflectance Threshold value declaration */
                InputInternalPixelType m_RedReflectanceThreshold;
                /** NirRedRatio max Threshold value declaration */
                double m_NirRedRatioMaxThreshold;
                /** NirRedRatio min Threshold value declaration */
                double m_NirRedRatioMinThreshold;
                /** NirSwirRatio min Threshold value declaration */
                double m_NirSwirRatioMinThreshold;




            };

    } // End namespace functor

} // End namespace vns

#endif /* __vnsReflectanceThresholdFunctor_h */
