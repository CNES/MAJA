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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 10 juin 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "MntIntersecteur.h"
#include "vnsUtilities.h"

#include <fstream>
#include <sstream>
#include "otbImageFileReader.h"
#include "otbImage.h"
#include <iomanip>
#include "vnsDEMDataManager.h"
#include "vnsLoggers.h"

int
vnsIntersector3DTestBufferWithOTB(int argc, char * argv[])
{
    if (argc != 9)
    {
        std::cout << "Usage : " << argv[0] << " <mnt> <x> <y> <z> <vx> <vy> <vz>" << std::endl;
        std::cout << "  <mnt> - chemin d'acces complet au MNT (sans extension)" << std::endl;
        std::cout << "  <x> <y> <z> - coordonnees du point de base (dans le referentiel du MNT)" << std::endl;
        std::cout << "  <vx> <vy> <vz> - vecteur de visee (dans le referentiel du MNT)" << std::endl;
        std::cout << "  [OutputAsciiFilename]" << std::endl;
        return EXIT_FAILURE;
    }
    Intersecteur::MntIntersecteur I3D;
    I3D.SetStreams(&std::cout, &std::cerr);

    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    double x0(0.), y0(0.), px(0.), py(0.);
    double * _pZ(NULL);
    int _nc(0), _nl(0);
    std::string Ident("");

    typedef otb::Image<double> DEMImageType;
    typedef otb::ImageFileReader<DEMImageType> ReaderType;

    int cpt(1);
    // Lecture de tout le MNT
    std::string Name(argv[cpt++]);

    if (Name.empty())
    {
        std::cout << "MntIntersecteur::Charger()   Pas de nom de fichier." << std::endl;
        return EXIT_FAILURE;
    }
    char pFilenameHDR[2048];
    strcpy(pFilenameHDR, Name.c_str());
    strcat(pFilenameHDR, ".hdr");

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(pFilenameHDR);
    reader->Update();
//    reader->GenerateOutputInformation();

    DEMImageType * output = reader->GetOutput();
    DEMImageType::Pointer ptroutput = reader->GetOutput();
    std::cout << "OTB: Image: " << ptroutput << std::endl;

    /*******************************************************************************************************/
//    x0 = output->GetUpperLeftCorner()[0];
//    y0 = output->GetUpperLeftCorner()[1];

    x0 = output->GetOrigin()[0];
    y0 = output->GetOrigin()[1];

    _nc = output->GetLargestPossibleRegion().GetSize(0);
    _nl = output->GetLargestPossibleRegion().GetSize(1);

    px = output->GetSignedSpacing()[0];
    py = output->GetSignedSpacing()[1];

    int nTypeConv = Intersecteur::MntIntersecteur::eTCNone;

    Ident = "GRS80:G-D/GRS80:Z-M";
    _pZ = output->GetBufferPointer();

    /*******************************************************************************************************/
    std::cout << "x0: " << x0 << std::endl;
    std::cout << "y0: " << y0 << std::endl;
    std::cout << "px: " << px << std::endl;
    std::cout << "py: " << py << std::endl;
    std::cout << "nTypeConv: " << nTypeConv << std::endl;
    std::cout << "Ident: " << Ident << std::endl;
    std::cout << "_nc: " << _nc << std::endl;
    std::cout << "_nl: " << _nl << std::endl;
    std::cout << "_pZ: " << _pZ << std::endl;
    if (!I3D.Charger(x0, // LON_REF
            y0, // LAT_REF
            px, // PAS_LON
            py, // PAS_LAT
            nTypeConv, _nc, _nl, _pZ))
    {
        std::cerr << "Erreur au chargement du MNT buffurisé." << std::endl;
        return EXIT_FAILURE;
    }
    vns::Utilities::ListOfDoubles Point(3);
    vns::Utilities::ListOfDoubles Visee(3);
    vns::Utilities::ListOfDoubles PointR(3);
    bool bValid;
    Point[0] = vns::Utilities::AsDouble(argv[cpt++]);
    Point[1] = vns::Utilities::AsDouble(argv[cpt++]);
    Point[2] = vns::Utilities::AsDouble(argv[cpt++]);
    Visee[0] = vns::Utilities::AsDouble(argv[cpt++]);
    Visee[1] = vns::Utilities::AsDouble(argv[cpt++]);
    Visee[2] = vns::Utilities::AsDouble(argv[cpt++]);
    std::cout << "Point : " << std::setprecision(25) << Point[0] << ", " << Point[1] << ", " << Point[2] << std::endl;
    std::cout << "Visee : " << std::setprecision(25) << Visee[0] << ", " << Visee[1] << ", " << Visee[2] << std::endl;
    bool l_ComputeIntersection = I3D.Intersection3D(Point, Visee, PointR, bValid);
    if (l_ComputeIntersection == false)
    {
        std::cerr << "Erreur lors de l'intersection" << std::endl;
        return EXIT_FAILURE;
    }
    std::ofstream flux(argv[cpt++]);

    flux << "Point : " << Point[0] << ", " << Point[1] << ", " << Point[2] << std::endl;
    flux << "Visee : " << Visee[0] << ", " << Visee[1] << ", " << Visee[2] << std::endl;


    if (bValid)
    {
        flux << std::fixed << std::setprecision(3) << "Resultat : " << PointR[0] << ", " << PointR[1] << ", " << PointR[2] << std::endl;
        std::cout << "Resultat : " << std::setprecision(3) << PointR[0] << ", " << PointR[1] << ", " << std::fixed << PointR[2] << std::endl;
        if (PointR[2] > Point[2])
        {
            flux << "Le point fourni en entree est cache !!!" << std::endl;
            std::cout << "Le point fourni en entree est cache !!!" << std::endl;
        }
    }
    else
    {
        flux << "Pas d'interesction trouvee" << std::endl;
        std::cout << "Pas d'interesction trouvee" << std::endl;
    }
    flux.close();

    return EXIT_SUCCESS;
}

