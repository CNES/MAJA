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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 12 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "MntIntersecteur.h"
#include "vnsUtilities.h"
#include <fstream>
#include <cstdlib>
#include <iomanip>

int vnsIntersector3DTest( int argc, char * argv[] )
{
	if ( argc != 9 )
	{
		std::cout << "Usage : " << argv[0] << " <mnt> <x> <y> <z> <vx> <vy> <vz>" << std::endl;
		std::cout << "  <mnt> - chemin d'acces complet au MNT (sans extension)" << std::endl;
		std::cout << "  <x> <y> <z> - coordonnees du point de base (dans le referentiel du MNT)" << std::endl;
		std::cout << "  <vx> <vy> <vz> - vecteur de visee (dans le referentiel du MNT)" << std::endl;
		std::cout << "  [OutputAsciiFilename]" << std::endl;
    	return EXIT_FAILURE;
	}
	Intersecteur::MntIntersecteur I3D;
	I3D.SetStreams(&std::cout,&std::cerr);
	if ( !I3D.Charger(argv[1]) )
	{
		std::cerr << "Erreur au chargement de " << argv[1] << std::endl;
    	return EXIT_FAILURE;
	}
	vns::Utilities::ListOfDoubles Point(3);
	vns::Utilities::ListOfDoubles Visee(3);
	vns::Utilities::ListOfDoubles PointR(3);
	bool bValid;
    Point[0] = vns::Utilities::AsDouble(argv[2]);
    Point[1] = vns::Utilities::AsDouble(argv[3]);
    Point[2] = vns::Utilities::AsDouble(argv[4]);
    Visee[0] = vns::Utilities::AsDouble(argv[5]);
    Visee[1] = vns::Utilities::AsDouble(argv[6]);
    Visee[2] = vns::Utilities::AsDouble(argv[7]);
    std::cout << std::setprecision(20)<< "Point : " << Point[0] << ", " << Point[1] << ", " << Point[2] << std::endl;
    std::cout << std::setprecision(20)<< "Visee : " << Visee[0] << ", " << Visee[1] << ", " << Visee[2] << std::endl;
	if ( !I3D.Intersection3D(Point,Visee,PointR,bValid) )
	{
		std::cerr << "Erreur lors de l'intersection" << std::endl;
    	return EXIT_FAILURE;
	}
    std::ofstream f(argv[8]);

    f << "Point : " << Point[0] << ", " << Point[1] << ", " << Point[2] << std::endl;
    f << "Visee : " << Visee[0] << ", " << Visee[1] << ", " << Visee[2] << std::endl;

	if ( bValid )
	{
        f << std::fixed << std::setprecision(3) << "Resultat : " << PointR[0] << ", " << PointR[1] << ", " << PointR[2] << std::endl;
        std::cout << std::setprecision(3) << "Resultat : " << PointR[0] << ", " << PointR[1] << ", " << PointR[2] << std::endl;
		if ( PointR[2] > Point[2] )
		{
            f << "Le point fourni en entree est cache !!!" << std::endl;
            std::cout << "Le point fourni en entree est cache !!!" << std::endl;
		}
	}
	else
	{
        f << "Pas d'interesction trouvee" << std::endl;
        std::cout << "Pas d'interesction trouvee" << std::endl;
	}
    f.close();

    return EXIT_SUCCESS;
}

