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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-2168-CNES : 28 fevrier 2017 : Correction I3D car sortie en erreur    *
 *                                                                                         boite englobante *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 8 juillet 2016 : Audit code - suppresion de macros ID3    *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-748-CNES : 4 mars 2014 : Optimisation de la suppression des DBL après *
 *                                  génération du DBL.DIR afin de réduire le volume du répertoire d'entrée  *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-747-CNES : 3 mars 2014 : Vidage repertoire Caching en PreProcessing.  *
 * VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Creation pour la prise en compte de la DM du  *
 *                                                            MACCS generique (PM LAIG-NT-MAC-0239-CNES)    *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsDEMDataManager.h 9319 2015-01-12 13:57:29Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef vns_DEMDataManager
#define vns_DEMDataManager

#include "MntIntersecteur.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsUtilities.h"
#include <iomanip>
#include <vector>

namespace vns
{
    /** \class DEMDataManager
     *   \brief DEM data manager (use I3D).
     *
     */
    template<class TDTMImage>
        class DEMDataManager : public Intersecteur::MntIntersecteur
        {
        public:
            typedef DEMDataManager Self;
            typedef Intersecteur::MntIntersecteur Superclass;

            typedef TDTMImage DTMImageType;
            typedef typename DTMImageType::Pointer DTMImagePointer;
            typedef typename DTMImageType::ConstPointer DTMImageConstPointer;

            /** Constructeur */
            DEMDataManager()
            {
                this->SetStreams(&(I3D_OUT_STREAM),&(I3D_ERR_STREAM));
        }

        /** Destructeur */
        virtual
        ~DEMDataManager()
        {
        }

        virtual void
        Load(TDTMImage * p_DTMPtr)
        {
            DTMImagePointer l_DTMPtr = const_cast<DTMImageType *>(p_DTMPtr);
            // Update to load the DTM in memory before setting to I3D
                        l_DTMPtr->Update();

                        vnsLogDebugMacro("Load I3D with the following DTM parameters: \nSize: "<< l_DTMPtr->GetLargestPossibleRegion() << "\nSpacing: "<<l_DTMPtr->GetSignedSpacing() << "\nOrigin: "<< l_DTMPtr->GetOrigin() << "\nUpperLeftCorner: ["<<l_DTMPtr->GetUpperLeftCorner()[0]<<", "<<l_DTMPtr->GetUpperLeftCorner()[1]<<"]\nLowerRightCorner: ["<<l_DTMPtr->GetLowerRightCorner()[0]<<", "<<l_DTMPtr->GetLowerRightCorner()[1]<<"]")

                        const int nc = l_DTMPtr->GetLargestPossibleRegion().GetSize(0);
                        const int nl = l_DTMPtr->GetLargestPossibleRegion().GetSize(1);

                        const double px = l_DTMPtr->GetSignedSpacing()[0];
                        const double py = l_DTMPtr->GetSignedSpacing()[1];

                        const double x0 = l_DTMPtr->GetOrigin()[0];
                        const double y0 = l_DTMPtr->GetOrigin()[1];

                        //Ident = "GRS80:G-D/GRS80:Z-M";
                        double * pZ = const_cast<double *>(l_DTMPtr->GetBufferPointer());

                        const bool l_LoadI3D = this->Charger(x0,// LON_REF
                        y0,// LAT_REF
                        px,// PAS_LON
                        py,// PAS_LAT
                        Intersecteur::MntIntersecteur::eTCNone, nc, nl, pZ);
                        if (l_LoadI3D == false)
                        {
                            vnsStaticExceptionDataMacro("Error of downloading of the buffered DTM.");
                        }
                    }

                    virtual bool
                    LaunchesIntersection3D(const std::string & comment, const std::vector<double> & Point, const std::vector<double> & Visee, std::vector<double> & PointR, const bool pWithoutError, const char *file, const char *function, const unsigned long line)
                    {
                        bool l_Valid(true);
                        const bool l_ComputeIntersection = this->Intersection3D(Point, Visee, PointR, l_Valid);
                        if (l_ComputeIntersection == false)
                        {
                            if (vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG)
                            {
                                std::ostringstream message;
                                message << comment << " - call Intersection3D (force execution without error=" << Utilities::BoolToLowerString(pWithoutError) << " with input parameters:\n"
                                "   Input Point: [" << std::setprecision(30) << Point[0] << ", " << Point[1] << ", " << Point[2]<< "]\n"
                                "   Input Visee: [" << std::setprecision(30) << Visee[0] << ", " << Visee[1] << ", " << Visee[2]<< "]\n"
                                "   -> Error Intersector3D ("<<comment<<"): No intersection for the Point [" << std::setprecision(15) << Point[0] << ", " << Point[1] << ", " << Point[2] << "]!";
                                vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,
                                 file, function, line);
                                if (vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::SUPER_DEBUG)
                                {
                                    std::ostringstream message;
                                    message << "\n Intersection3D details: \n"
                                    "    -  [PX;PY]      : [" << this->GetPX() << ";"<<this->GetPY() <<"]\n"
                                    "    -  [X0;Y0]      : [ "<< this->GetX0() << ";"<<this->GetY0() <<"]\n"
                                    "    -  [ZMin;ZMax]  : [" << this->GetZMin() << ";"<<this->GetZMax() <<"]\n"
                                    "    -  [NC;NL]      : [" << this->GetNC() << ";"<<this->GetNL() <<"]\n";
                                    vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::SUPER_DEBUG,
                                     file, function, line);
                                }
                            }
                            if ( pWithoutError == true )
                            {
                                if (vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::WARNING)
                                {
                                    std::ostringstream message;
                                    message << "Attention: I3D problem to check the cube with Input Point: [" << std::setprecision(30) << Point[0] << ", " << Point[1] << ", " << Point[2]<< "]; Input Visee: [" << std::setprecision(30) << Visee[0] << ", " << Visee[1] << ", " << Visee[2]<< "]";
                                    vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::WARNING,
                                     file, function, line);
                                }
                                l_Valid = false;
                            }
                            else
                            {
                                vnsStaticExceptionBusinessMacro("Error Intersector3D ("<<comment<<"): No intersection for the Point [" << std::setprecision(30) << Point[0] << ", " << Point[1] << ", " << Point[2] << "]!")
                            }
                        }
                        return l_Valid;
                    }

                };
            }

#endif
