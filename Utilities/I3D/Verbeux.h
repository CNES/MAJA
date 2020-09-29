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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#if !defined(_VERBEUX_H_INCLUDED_)
#define _VERBEUX_H_INCLUDED_

#include <string>
#include <iostream>

#define I3D_OUT_STREAM  std::cout
#define I3D_ERR_STREAM  std::cerr


namespace Intersecteur
{
/*!

\class Verbeux
  
   Classe de base des Verbeuxs
   
	*/
	class Verbeux
	{
	public :

		//! Constructeur
		Verbeux();
		
		//! Destructeur
		virtual ~Verbeux();

		/*!
		* Positionne les streams de sortie et d'erreur
		* @param outstream, errstream : streams de sortie et d'erreur
		* @return void
		*/
		void SetStreams(std::ostream * outstream, std::ostream * errstream);

	public:

		std::ostream * GetOutStream() { return _outstream; }
		std::ostream * GetErrStream() { return _errstream; }

	protected:

		/*!
		* Positionne une erreur et l'affiche
		* @param method : nom de la methode affichant l'erreur
		* @param erreur : decriptif de l'erreur
		*/
        void SetError(const std::string & method, const std::string & erreur);

		/*!
		* Positionne un warning et l'affiche
		* @param method : nom de la methode affichant l'erreur
		* @param erreur : decriptif de l'erreur
		*/
        void SetWarning(const std::string & method, const std::string & erreur);

	protected:
		
		//! Stream de sortie
		std::ostream * _outstream;
		
		//! Stream d'erreur
		std::ostream * _errstream;
	} ;
}

#endif // _VERBEUX_H_INCLUDED_
