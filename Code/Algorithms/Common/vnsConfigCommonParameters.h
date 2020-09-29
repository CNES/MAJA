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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConfigCommonParameters_h
#define __vnsConfigCommonParameters_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace vns
{
/** \class  ConfigCommonParameters
 * \brief This class implement alls parameters contains in the L23ConfigCommon parameters file.
 *
 * 
 * \author CS Systemes d'Information
 *
 * \sa Object
 *
 * \ingroup L2
 * \ingroup L3
 * \ingroup Checktool
 *
 */
class ConfigCommonParameters : public itk::Object
{
public:
    /** Standard class typedefs. */
    typedef ConfigCommonParameters                                 Self;
    typedef itk::Object                                            Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(ConfigCommonParameters, Object );


    /** Set/Get the number of threads used by the processors */
    itkSetMacro(NumberOfThreads,int);
    itkGetConstMacro(NumberOfThreads,int);


protected:
    /** Constructor */
    ConfigCommonParameters();
    /** Destructor */
    virtual ~ConfigCommonParameters();
    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
    ConfigCommonParameters(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /** Number of threads used by the processors */
    int m_NumberOfThreads;

};

} // End namespace vns

#endif /* __vnsConfigCommonParameters_h */
