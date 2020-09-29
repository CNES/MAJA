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
 * VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Prise en compte de la DM incluant notamment   *
 *                                                            le MACCS generique (PM LAIG-NT-MAC-0239-CNES) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#if !defined(_MNTINTERSECTEUR_H_INCLUDED_)
#define _MNTINTERSECTEUR_H_INCLUDED_

#include "Verbeux.h"

#include <vector>

namespace Intersecteur
{
/*!
\class MntIntersecteur

  Mnt optimis� pour l'Intersecteur

	*/
	class MntIntersecteur : public Verbeux
	{
	public:

		//! Constructeur
		MntIntersecteur();

		//! Destructeur
		virtual ~MntIntersecteur();

		/*!
		* Charge le MNT en m�moire
		* @param Name : nom de fichier (avec path et sans extension) du MNT � charger
		* @return bool  : Status d'execution
		*/
		bool Charger(const std::string & Name);

        /*!
        * Charge le MNT existant en m�moire
        * @param x0 : LON_REF
        * @param y0 : LAT_REF
        * @param px : PAS_LON
        * @param py : PAS_LAT
        * @param typeconv : Type de conversion
        * @param ident : Identification modele REF
        * @param nc: NB_LON
        * @param nl: NB_LAT
        * @param z : pointer sur le MNT chargé en mémoire
        * @return bool  : Status d'execution
        */
	    bool Charger(                  const double x0, // LON_REF
	                                   const double y0, // LAT_REF
                                       const double px, // PAS_LON
	                                   const double py, // PAS_LAT
	                                   const int typeConv,
	                                   const int nc,
	                                   const int nl,
	                                         double * z);


		/*!
		* Indique si le MNT est charg� en memoire
		* @return bool  : Valeur
		*/
		bool GetIsLoaded() const { return _bIsLoaded; }

		/*!
		* Calcule le point terrain en fonction d'un point dans l'espace et d'un vecteur de vis�e
		* @param Point : Point 3D
		* @param Visee : Vecteur de vis�e "descendant" (z n�gatif)
		* @param PointR : Point terrain resultant de l'intersection
		* @param bValid : Indique si la vis�e intersecte le MNT
		* @return bool  : Status d'execution
		*/
		bool Intersection3D(const std::vector<double> & Point, const std::vector<double> & Visee, std::vector<double> & PointR, bool & bValid);

	protected:

		/*!
		* Verifie que la Vis�e intersecte la boite englobante du MNT et initialise le "point de depart"
		* @param Visee : Points terrain constituant la vis�e
		* @param PointB : "Point de d�part" calcul� en vue d'une intersection r�elle
		* @param dH3D : "Index" de ce "point de d�part" dans la liste constituant la vis�e
		* @param bTrouve : Indique si la vis�e intersecte la boite englobante du MNT
		* @return bool  : Status d'execution
		*/
		bool CheckCubeMnt(const std::vector< std::vector<double> > & Visee, std::vector<double> & PointB, double & dH3D, bool & bTrouve);

		/*!
		* Effectue le calcul d'intesection entre la vis�e et le MNT
		* @param Visee : Points terrain constituant la vis�e
		* @param H3D : Liste des index des points terrain constituant la vis�e
		* @param PointB : "Point de d�part" de l'intersection
		* @param dH3D : "Index" de ce "point de d�part" dans la liste constituant la vis�e
		* @param PointR : Point terrain resultant de l'intersection
		* @param bTrouve : Indique si la vis�e intersecte le MNT
		* @return bool  : Status d'execution
		*/
		bool Intersection(const std::vector< std::vector<double> > & Visee, const std::vector<double> & H3D, std::vector<double> & PointB, double & dH3D, std::vector<double> & PointR, bool & bTrouve);

		/*!
		* Transforme une liste de points terrain en une liste de points en "coordonn�es MNT"
		* @param Ters : Liste de points terrain
		* @param Mnts : Liste de points en "coordonn�es MNT"
		* @return void
		*/
		void TersToMnts(const std::vector< std::vector<double> > & Ters, std::vector< std::vector<double> > & Mnts);

		/*!
		* Transforme une liste de points en "coordonn�es MNT" en une liste de points terrain
		* @param Mnts : Liste de points en "coordonn�es MNT"
		* @param Ters : Liste de points terrain
		* @return void
		*/
		void MntsToTers(const std::vector< std::vector<double> > & Mnts, std::vector< std::vector<double> > & Ters);

		/*!
		* Transforme un point terrain en un point en "coordonn�es MNT"
		* @param Ter : Point terrain
		* @param Mnt : Point en "coordonn�es MNT"
		* @return void
		*/
		void TerToMnt(const std::vector<double> & Ter, std::vector<double> & Mnt);

		/*!
		* Transforme un point "coordonn�es MNT" en un point en terrain
		* @param Mnt : Point en "coordonn�es MNT"
		* @param Ter : Point terrain
		* @return void
		*/
		void MntToTer(const std::vector<double> & Mnt, std::vector<double> & Ter);

		/*!
		* Calcule une Altitude dans le MNT par interpolation bilineaire
		* @param dX, dY : "Coordonn�es MNT"
		* @return double  : Altitude
		*/
		double MakeAlti(double dX, double dY);

	public:

		long GetNC() const { return _nc; }
		long GetNL() const { return _nl; }

		const std::string & GetRepTer() const { return _Ident; }

		double GetPX() const { return _px; }
		double GetPY() const { return _py; }

		double GetX0() const { return _x0; }
		double GetY0() const { return _y0; }

		const double * GetZ() const { return _pZ; }

		double GetZMin() const { return _zMin; }
		double GetZMax() const { return _zMax; }

		const double * GetPZMin() const { return _pZMin; }
		const double * GetPZMax() const { return _pZMax; }

        //! Type d'encodage du MNT RAW
        enum ETypeVal
        {
            eTVChar = 0,
            eTVUChar = 1,
            eTVShort = 2,
            eTVUShort = 3,
            eTVInt = 4,
            eTVUInt = 5,
            eTVFloat = 6,
            eTVDouble = 7
        };

        //! Type de conversion d'altitude du MNT RAW
        enum ETypeConversion
        {
            eTCNone = 0,     // Altitude directe (signee) : hr = h
            eTCRelative = 1, // Altitude relative : hr = href + h * sh
            eTCIntervale = 2 // Intervalle d'altitude : hr = h0 + h*(h1-h0)/2^(8*bit_per_alt)
        };

	protected:


		//! Dimensions image
		long _nc, _nl;

		//! Type de donnes
		ETypeVal _TypeVal;

		//! Type de conversion (not implemented)
		ETypeConversion _TypeConv;

		//! Representation Terrain
		std::string _Ident;

		//! Coordonn�es dans la reference (terrain ou image) du premier noeud de la grille
		// double _x0, _y0;

		//! Pas dans la reference entre deux noeuds de grille
		double _px, _py;

		//! Coord Terrain
		double _x0, _y0;

		//! Calage inverse (Terrain==>Matrice)
		double _i1, _i2;

		//! Calage direct  (Matrice==>Terrain)
		double _d1, _d2;

		//! Altitudes
		double * _pZ;

		//! Bool to enable or disable free _pZ buffer
        bool _freepZ;

		//! H Min et Max du MNT
		double _zMin;
		double _zMax;

		//! H Min et Max des mailles du MNT
		double * _pZMin;
		double * _pZMax;

		//! Initialias�
		bool _bIsLoaded;

	private:
		// Calcule les altitudes Min et Max
		void CalculerALtitudesMinMax();

		void Print(std::ostream & s);

	};
}

#endif // _MNTINTERSECTEUR_H_INCLUDED_
