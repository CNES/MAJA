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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 23 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsProcessingMode_h
#define __vnsProcessingMode_h

#include "itkObject.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  ProcessingMode
     * \brief This class implements the differentes values of L2 scientific processing mode.
     *
     * The Mode possibles values are INIT, NOMINAL and BACKWARD.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    class ProcessingMode
    {
    public:
        /** Constructor */
        ProcessingMode()
        {
            m_StandardMode = ProcessingMode::UNINITIALIZED;
            m_BackwardMode = false;
        }
        /** Destructor */
        virtual
        ~ProcessingMode()
        {
        }

        /** Definition of standards modes of processing. These modes will be used to configure
         * the scientifics processing algorithms (INIT or NOMINAL possible values)*/
        typedef enum
        {
            INIT = 0, NOMINAL = 1, UNINITIALIZED = 2,
        } StandardModeType;

        /** Set the standard mode of scientific processing */
        virtual void
        SetMode(const StandardModeType mode)
        {
            this->m_StandardMode = mode;
        }
        /** Set the standard mode to the INIT value */
        virtual void
        SetInitMode()
        {
            this->m_StandardMode = ProcessingMode::INIT;
        }
        /** Set the standard mode to the INIT value */
        virtual void
        SetNominalMode()
        {
            this->m_StandardMode = ProcessingMode::NOMINAL;
        }

        /** Set the backward mode (true or false) */
        virtual void
        SetBackwardMode(const bool mode)
        {
            this->m_BackwardMode = mode;
        }

        /** Enable the backward mode */
        virtual void
        EnableBackwardMode()
        {
            this->m_BackwardMode = true;
        }
        /** Disable the backward mode */
        virtual void
        DisableBackwardMode()
        {
            this->m_BackwardMode = false;
        }

        /** Check if the value of the mode is set to "INIT" */
        bool
        IsInit() const
        {
            return (this->m_StandardMode == ProcessingMode::INIT);
        }
        bool
        IsNotInit() const
        {
            return (this->IsInit() == false);
        }
        /** Check if the value of the mode is set to "NOMINAL" */
        bool
        IsNominal() const
        {
            return (this->m_StandardMode == ProcessingMode::NOMINAL);
        }
        bool
        IsNotNominal() const
        {
            return (this->IsNominal() == false);
        }
        /** Check if the value of the mode is set to "BACKWARD" */
        bool
        IsBackward() const
        {
            return (this->m_BackwardMode);
        }
        bool
        IsNotBackward() const
        {
            return (this->IsBackward() == false);
        }

        std::string
        GetStringProcessingMode() const
        {
            std::string val("");
            // Keep this order: test BACKWARD_MODE before Init or Nominal because in Backward mode, we can be combine in INIT or NOMINAL mode
            if (this->IsBackward() == true)
            {
                // Set val to BACKWARD
                val = "BACKWARD_MODE";
            }
            // INIT mode
            else if (this->IsInit() == true)
            {
                // Set val to INIT
                val = "INIT_MODE";
            }
            // NOMINAL (or STANDARD) mode
            else if (this->IsNominal() == true)
            {
                // Set val to NOMINAL
                val = "STANDARD_MODE";
            }
            // Mode not initialized
            else
            {
                // UNKNOWN
                val = "PROCESSING_MODE_UNINITIALIZED";
            }
            // return val
            return val;
        }

    private:
        /**  Standard Mode parameter (INIT or NOMINAL) */
        StandardModeType m_StandardMode;

        /**  Backward Mode parameter (TRUE or FALSE) */
        bool m_BackwardMode;
    };

} // End namespace vns

#endif /* __vnsProcessingMode_h */
