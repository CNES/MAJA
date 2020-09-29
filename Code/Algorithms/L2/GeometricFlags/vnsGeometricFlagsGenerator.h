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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsGeometricFlagsGenerator_h
#define __vnsGeometricFlagsGenerator_h

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "vnsMacro.h"
#include "otbMath.h"

namespace vns
{
/** \class  GeometricFlagsGenerator
 * \brief This class provides two flags:
 * \begin{itemize}
 *      \item a sunglint flag which is raiseed if the geometry is close to
 *      the sunglint direction. Indeed, above water surfaces, near sun glint direction,
 *      reflectances are very dependent on wind speed and viewing and solar angles.
 *      \item a hot-spot flag which is raised if the geometry is close tothe hot-spot
 *      direction. Indeed, above land surfaces, near the backscattering direction,
 *      quick variation of reflectances happen.
 *  \end{itemize}
 * 
 * \author CS Systemes d'Information
 *
 * \sa Object
 *
 * \ingroup L2
 *
 */
class GeometricFlagsGenerator : public itk::Object
{
public:
    /** Standard class typedefs. */
    typedef GeometricFlagsGenerator                                Self;
    typedef itk::Object                                            Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    typedef std::vector<double>                                    DoubleVectorType;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(GeometricFlagsGenerator, Object );

    /**  Set solar zenith angle value. */
    vnsSetMacro(ThetaS, double)
    /**  Get solar zenith angle value. */
    vnsGetConstMacro(ThetaS, double)

    /**  Set solar azimuth angle value. */
    vnsSetMacro(PhiS, double)
    /**  Get solar azimuth angle value. */
    vnsGetConstMacro(PhiS, double)

    /**  Set viewing zenith angle value. */
    vnsSetMacro(ThetaV, DoubleVectorType)
    /**  Get viewing zenith angle value. */
    vnsGetConstMacro(ThetaV, DoubleVectorType)

    /**  Set viewing azimuth angle value. */
    vnsSetMacro(PhiV, DoubleVectorType)
    /**  Get viewing azimuth angle value. */
    vnsGetConstMacro(PhiV, DoubleVectorType)

    /**  Set the sunglint threshold value. */
    vnsSetMacro(SunglintThreshold, double)
    /**  Get the sunglint threshold  value. */
    vnsGetConstMacro(SunglintThreshold, double)

    /**  Set the hot-spot threshold value. */
    vnsSetMacro(HotSpotThreshold, double)
    /**  Get the hot-spot threshold  value. */
    vnsGetConstMacro(HotSpotThreshold, double)

    bool IsSunglint();
    bool IsHotSpot();
    
    std::string PrintResult();


protected:
    /** Constructor */
    GeometricFlagsGenerator();
    /** Destructor */
    virtual ~GeometricFlagsGenerator();

    DoubleVectorType GetSunglintDistance();
    DoubleVectorType GetHotSpotDistance();

    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
    GeometricFlagsGenerator(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /**  Solar zenith angle in degree. */
    double m_ThetaS;
    /**  Solar azimuth angle in degree. */
    double m_PhiS;
    /**  View zenith angle in degree (one angle per band). */
    DoubleVectorType m_ThetaV;
    /**  View azimuth angle in degree (one angle per band). */
    DoubleVectorType m_PhiV;

    /** Maximum angular distance to sunglint under which a viewing direction
     *  is declared close to sunglint
     */
    double m_SunglintThreshold;
    /** Maximum angular distance to hot-spot under which a veiwing direction
     *  is declared close to hot-spot
     */
    double m_HotSpotThreshold;

};

} // End namespace vns

#endif /* __vnsGeometricFlagsGenerator_h */
