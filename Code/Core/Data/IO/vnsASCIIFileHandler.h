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
 * VERSION : 4-4-0 : DM : LAIG-DM-MAC-1284-CNES : 27 mai 2015 : Création du plug-in Landsat8 natif          *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$                                                                                                     *
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsASCIIFileHandler_h
#define __vnsASCIIFileHandler_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include <string>
#include <list>
#include <map>
#include "vnsUtilities.h"

namespace vns
{
    /** \class  ASCIIFileHandler
     * \brief This class implements the basic functionnalities to read a ASCCI file.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa XMLFileHandler
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
    class ASCIIFileHandler : public itk::Object
    {
    public:
        /** Standard class typedefs. */
        typedef ASCIIFileHandler Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Creation through object factory macro */
        itkTypeMacro(ASCIIFileHandler, itk::Object )

        /** Type macro */
        itkNewMacro(Self)

        typedef Utilities::ListOfStrings ListOfStrings;

        /** Load file */
        void
        LoadFile(const std::string& filename);

        /** Get parameter value */
        std::string
        GetStringValue(const std::string& ParamName);

        double
        GetDoubleValue(const std::string& ParamName);

        float
        GetFloatValue(const std::string& ParamName);

        int
        GetIntValue(const std::string& ParamName);

        unsigned int
        GetUIntValue(const std::string& ParamName);

        std::string
        GetParameterValue(const std::string& ParamName);

        std::string
        GetRawParameterValue(const std::string& ParamName);

    protected:
        /** Constructor */
        ASCIIFileHandler();
        /** Destructor */
        virtual
        ~ASCIIFileHandler();
        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

        virtual bool
        CanHandle(const std::string & name);
        /** do the actual parsing of the input file */
        virtual void
        GenerateOutputInformation();

        std::string m_Filename;
        std::list<std::string> m_LinesOfFiles;
        bool m_IsLoaded;

    private:
        ASCIIFileHandler(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented


};

} // End namespace vns

#endif /* __vnsASCIIFileHandler_h */
