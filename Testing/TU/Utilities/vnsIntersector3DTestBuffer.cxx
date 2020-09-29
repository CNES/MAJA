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

#include "MntIntersecteur.h"
#include "vnsUtilities.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>

   ////////////////////////////////////////////////////////////////////////////////////////////////
    // fmygets
    ////////////////////////////////////////////////////////////////////////////////////////////////
    void
    VENUSfmygets(char * str, int n, FILE * file)
    {
        str[0] = 0;

        if (feof(file))
            return;
        int i = 0;
        long pos = -1;
        while (i < n && !feof(file))
        {
            char c;
            int len = fread(&c, sizeof(char), 1, file);
            if (len > 0)
            {
                if (c == '\n')
                {
                    str[i] = 0;
                    pos = ftell(file);
                    break;
                }
                if (c == '\r')
                {
                    str[i] = 0;
                    pos = ftell(file);
                    break;
                }

                str[i] = c;
            }
            else
            {
                str[i] = 0;
                return;
            }
            i++;
        }
        bool newline = false;
        while (i < n && !feof(file) && !newline)
        {
            char c;
            int len = fread(&c, sizeof(char), 1, file);
            if (len > 0)
            {
                newline = true;
                if (c == '\n')
                {
                    pos = ftell(file);
                    newline = false;
                }
                if (c == '\r')
                {
                    pos = ftell(file);
                    newline = false;
                }
            }
            else
            {
                return;
            }
        }
        if (pos != -1)
            fseek(file, pos, SEEK_SET);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // fmyread
    ////////////////////////////////////////////////////////////////////////////////////////////////
    bool
    VENUSfmyread(const std::string & key, std::string & val, FILE * file)
    {
        std::string readkey;
        val = "";

        bool bRetry = true;

        while (bRetry)
        {
            bRetry = false;

            char buf[1024];
            VENUSfmygets(buf, 1024, file);
            while (buf[0] == '/')
                VENUSfmygets(buf, 1024, file);
            if (strlen(buf) > 2)
            {
                if (buf[0] == '>' && buf[1] == '>' && buf[2] == '\t')
                {
                    unsigned int i;
                    for (i = 3; i < strlen(buf) && buf[i] != '\t'; ++i)
                    {
                        readkey += buf[i];
                    }
                    if (readkey == key)
                    {
                        ++i;
                        while (i < strlen(buf) && buf[i] != '\t')
                        {
                            val += buf[i];
                            ++i;
                        }
                        return true;
                    }
                    else
                    {
                        bRetry = true;
                    }
                }
            }
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // FromString
    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
        T
        VENUSFromString(const std::string & chaine)
        {
            T res;
            std::istringstream tmp(chaine);
            tmp >> res;
            return res;
        }
int vnsIntersector3DTestBuffer( int argc, char * argv[] )
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

    /*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
	double x0(0.), y0(0.), px(0.), py(0.);
	std::string Ident("");
	int _nc(0), _nl(0);
	double * _pZ(NULL);
	Intersecteur::MntIntersecteur::ETypeVal _TypeVal;

	// Lecture de tout le MNT
    std::string Name(argv[1]);
    if (Name.empty())
    {
        std::cout << "MntIntersecteur::Charger()   Pas de nom de fichier." << std::endl;
        return false;
    }
    std::string FileNameHD = Name + ".hd_babel";

    FILE * f = fopen(FileNameHD.c_str(), "rb");
    if (!f)
    {
        std::cout << "MntIntersecteur::Charger()   Echec d'ouverture fichier " + FileNameHD + "." << std::endl;
        return false;
    }

    // Lecture des attributs du MNT (contenus dans le fichier hd_babel)
    std::string Temp;
    bool bErrorReadingHeader = false;

    if (!VENUSfmyread("LON_REF", Temp, f))
        bErrorReadingHeader = true;
    x0 = VENUSFromString<double> (Temp);

    if (!VENUSfmyread("LAT_REF", Temp, f))
        bErrorReadingHeader = true;
    y0 = VENUSFromString<double> (Temp);

    if (!VENUSfmyread("NB_LON", Temp, f))
        bErrorReadingHeader = true;
    _nc = VENUSFromString<int> (Temp);

    if (!VENUSfmyread("NB_LAT", Temp, f))
        bErrorReadingHeader = true;
    _nl = VENUSFromString<int> (Temp);

    if (!VENUSfmyread("PAS_LON", Temp, f))
        bErrorReadingHeader = true;
    px = VENUSFromString<double> (Temp);

    if (!VENUSfmyread("PAS_LAT", Temp, f))
        bErrorReadingHeader = true;
    py = VENUSFromString<double> (Temp);

    if (!VENUSfmyread("TYPE_CODE", Temp, f))
        bErrorReadingHeader = true;
    int nTypeCode = VENUSFromString<int> (Temp);

    if (!VENUSfmyread("TYPE_CONV", Temp, f))
        bErrorReadingHeader = true;
    int nTypeConv = VENUSFromString<int> (Temp);

    switch (nTypeConv)
        {
    case Intersecteur::MntIntersecteur::eTCNone:
        /*
         _typeConv = eTCNone;
         */
        break;
    case Intersecteur::MntIntersecteur::eTCRelative:
        /*
         _typeConv = eTCRelative;
         if ( !VENUSfmyread("H_REF", Temp) ) bErrorReadingHeader = true;
         _specConv._relat._altitudeRef = VENUSFromString<double>(Temp);
         if ( !VENUSfmyread("S_H", Temp) ) bErrorReadingHeader = true;
         _specConv._relat._scale = VENUSFromString<double>(Temp);
         */
        {
            fclose(f);
            std::cout << "MntIntersecteur::Charger()   Seul le TypeConv 'None' est g�r�." << std::endl;
            return false;
        }
        break;
    case Intersecteur::MntIntersecteur::eTCIntervale:
        /*
         _typeConv = eTCIntervale;
         if ( !VENUSfmyread("H_MIN", Temp) ) bErrorReadingHeader = true;
         _specConv._inter._altitudeMin = VENUSFromString<double>(Temp);
         if ( !VENUSfmyread("H_MAX", Temp) ) bErrorReadingHeader = true;
         _specConv._inter._altitudeMax = VENUSFromString<double>(Temp);
         */
        {
            fclose(f);
            std::cout << "MntIntersecteur::Charger()   Seul le TypeConv 'None' est g�r�." << std::endl;
            return false;
        }
        break;
    default:
        break;
        }

    if (!VENUSfmyread("REF", Ident, f))
        bErrorReadingHeader = true;

    fclose(f);

    if (bErrorReadingHeader)
    {
        std::cout << "MntIntersecteur::Charger()   Erreur lors de la lecture du header." << std::endl;
        return false;
    }

    // Respect de l'existant SP5LIB...
    if (Ident == "GRS80_GEO")
    {
        Ident = "GRS80:G-D/GRS80:Z-M";
    }

    // Contr�les
    if (_nc < 2 || _nl < 2)
    {
        std::cout<<"MntIntersecteur::Charger()   Dimensions de grille incorrectes."<<std::endl;
        return false;
    }

    // Lecture des attributs du MNT (contenus dans le fichier hd)
    int nNbCan = 0;
    int nNbOctPix = 0;
    int nNbCol = 0;
    int nNbLig = 0;

    char pFilename[2048];
    strcpy(pFilename, Name.c_str());

    char pFilenameHD[2048];
    strcpy(pFilenameHD, pFilename);
    strcat(pFilenameHD, ".hd");
    f = fopen(pFilenameHD, "rb");
    if (!f)
    {
        std::cout << "MntIntersecteur::Charger()   Echec ouverture image." << std::endl;
        return false;
    }

    bool bNbCanFound = false;
    bool bNbOctPixFound = false;
    bool bNbColFound = false;
    bool bNbLigFound = false;

    while (!feof(f))
    {
        char buf[512];
        VENUSfmygets(buf, 512, f);
        if (strcmp(buf, "CHANNELS") == 0)
        {
            VENUSfmygets(buf, 512, f);
            nNbCan = atoi(buf);
            bNbCanFound = true;
        }
        if (strcmp(buf, "LINES") == 0)
        {
            VENUSfmygets(buf, 512, f);
            nNbLig = atoi(buf);
            bNbLigFound = true;
        }
        if (strcmp(buf, "COLUMNS") == 0)
        {
            VENUSfmygets(buf, 512, f);
            nNbCol = atoi(buf);
            bNbColFound = true;
        }
        if (strcmp(buf, "BITS PER PIXEL") == 0)
        {
            VENUSfmygets(buf, 512, f);
            nNbOctPix = atoi(buf) / 8;
            bNbOctPixFound = true;
        }
    }
    fclose(f);
    if (!bNbCanFound || !bNbOctPixFound || !bNbColFound || !bNbLigFound)
    {
        std::cout << "MntIntersecteur::Charger()   Echec lecture header." << std::endl;
        return false;
    }
    strcat(pFilename, ".c1");
    f = fopen(pFilename, "rb");
    if (f == NULL)
    {
        std::cout << "MntIntersecteur::Charger()   Echec ouverture image." << std::endl;
        return false;
    }

    // Controle des donnees de l'image
    if (nNbCan != 1)
    {
        fclose(f);
        std::cout << "MntIntersecteur::Charger()   Nombre de canaux incorrect." << std::endl;
        return false;
    }
    if ((nNbCol != _nc) || (nNbLig != _nl))
    {
        fclose(f);
        std::cout << "MntIntersecteur::Charger()   Incoherence sur les dimensions." << std::endl;
        return false;
    }
    if (((nTypeCode == Intersecteur::MntIntersecteur::eTVChar) && (nNbOctPix != sizeof(char))) || ((nTypeCode == Intersecteur::MntIntersecteur::eTVUChar) && (nNbOctPix
            != sizeof(unsigned char))) || ((nTypeCode == Intersecteur::MntIntersecteur::eTVShort) && (nNbOctPix != sizeof(short)))
            || ((nTypeCode == Intersecteur::MntIntersecteur::eTVUShort) && (nNbOctPix != sizeof(unsigned short))) || ((nTypeCode == Intersecteur::MntIntersecteur::eTVInt)
            && (nNbOctPix != sizeof(int))) || ((nTypeCode == Intersecteur::MntIntersecteur::eTVUInt)
            && (nNbOctPix != sizeof(unsigned int))) || ((nTypeCode == Intersecteur::MntIntersecteur::eTVFloat) && (nNbOctPix
            != sizeof(float))) || ((nTypeCode == Intersecteur::MntIntersecteur::eTVDouble) && (nNbOctPix != sizeof(double))))
    {
        fclose(f);
        std::cout << "MntIntersecteur::Charger()   Incoherence entre BITS PER PIXEL et TYPE_CODE." << std::endl;
        return false;
    }

    // allocation du buffer destination (buffer de floats)
    if (_pZ)
        free(_pZ);
    _pZ = (double*) malloc(_nc * _nl * sizeof(double));

    int nLig;
    int nCol;
    double * pZ;

    // suivant types de donn�es
    switch (nTypeCode)
        {
    //---------- char -------
    case Intersecteur::MntIntersecteur::eTVChar:
        _TypeVal = Intersecteur::MntIntersecteur::eTVChar;

        // allocation du buffer tampon
        char * pc_buf;
        char * pc_buf_tmp;
        pc_buf = (char *) malloc(_nc * _nl * sizeof(char));
        pc_buf_tmp = pc_buf;

        // Lecture d'un canal
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(pc_buf_tmp, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            pc_buf_tmp = pc_buf_tmp + nNbCol;
        }

        // recopie dans le buffer de floats
        pc_buf_tmp = pc_buf;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            for (nCol = 0; nCol < nNbCol; nCol++)
            {
                *pZ = (double) (*pc_buf_tmp);
                pZ++;
                pc_buf_tmp++;
            }
        }
        free(pc_buf);
        break;
        //---------- unsigned char -------
    case Intersecteur::MntIntersecteur::eTVUChar:
        _TypeVal = Intersecteur::MntIntersecteur::eTVUChar;

        // allocation du buffer tampon
        unsigned char * puc_buf;
        unsigned char * puc_buf_tmp;
        puc_buf = (unsigned char *) malloc(_nc * _nl * sizeof(unsigned char));
        puc_buf_tmp = puc_buf;

        // Lecture d'un canal
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(puc_buf_tmp, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            puc_buf_tmp = puc_buf_tmp + nNbCol;
        }

        // recopie dans le buffer de floats
        puc_buf_tmp = puc_buf;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            for (nCol = 0; nCol < nNbCol; nCol++)
            {
                *pZ = (double) (*puc_buf_tmp);
                pZ++;
                puc_buf_tmp++;
            }
        }
        free(puc_buf);
        break;
        //---------- short -------
    case Intersecteur::MntIntersecteur::eTVShort:
        _TypeVal = Intersecteur::MntIntersecteur::eTVShort;

        // allocation du buffer tampon
        short * ps_buf;
        short * ps_buf_tmp;
        ps_buf = (short *) malloc(_nc * _nl * sizeof(short));
        ps_buf_tmp = ps_buf;

        // Lecture d'un canal
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(ps_buf_tmp, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            ps_buf_tmp = ps_buf_tmp + nNbCol;
        }

        // recopie dans le buffer de floats
        ps_buf_tmp = ps_buf;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            for (nCol = 0; nCol < nNbCol; nCol++)
            {
                *pZ = (double) (*ps_buf_tmp);
                pZ++;
                ps_buf_tmp++;
            }
        }
        free(ps_buf);
        break;
        //---------- unsigned short -------
    case Intersecteur::MntIntersecteur::eTVUShort:
        _TypeVal = Intersecteur::MntIntersecteur::eTVUShort;

        // allocation du buffer tampon
        unsigned short * pus_buf;
        unsigned short * pus_buf_tmp;
        pus_buf = (unsigned short *) malloc(_nc * _nl * sizeof(unsigned short));
        pus_buf_tmp = pus_buf;

        // Lecture d'un canal
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(pus_buf_tmp, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            pus_buf_tmp = pus_buf_tmp + nNbCol;
        }

        // recopie dans le buffer de floats
        pus_buf_tmp = pus_buf;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            for (nCol = 0; nCol < nNbCol; nCol++)
            {
                *pZ = (double) (*pus_buf_tmp);
                pZ++;
                pus_buf_tmp++;
            }
        }
        free(pus_buf);
        break;
        //---------- int -------
    case Intersecteur::MntIntersecteur::eTVInt:
        _TypeVal = Intersecteur::MntIntersecteur::eTVInt;

        // allocation du buffer tampon
        int * pi_buf;
        int * pi_buf_tmp;
        pi_buf = (int *) malloc(_nc * _nl * sizeof(int));
        pi_buf_tmp = pi_buf;

        // Lecture d'un canal
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(pi_buf_tmp, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            pi_buf_tmp = pi_buf_tmp + nNbCol;
        }

        // recopie dans le buffer de floats
        pi_buf_tmp = pi_buf;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            for (nCol = 0; nCol < nNbCol; nCol++)
            {
                *pZ = (double) (*pi_buf_tmp);
                pZ++;
                pi_buf_tmp++;
            }
        }
        free(pi_buf);
        break;
        //---------- unsigned int -------
    case Intersecteur::MntIntersecteur::eTVUInt:
        _TypeVal = Intersecteur::MntIntersecteur::eTVUInt;

        // allocation du buffer tampon
        unsigned int * pui_buf;
        unsigned int * pui_buf_tmp;
        pui_buf = (unsigned int *) malloc(_nc * _nl * sizeof(unsigned int));
        pui_buf_tmp = pui_buf;

        // Lecture d'un canal
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(pui_buf_tmp, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            pui_buf_tmp = pui_buf_tmp + nNbCol;
        }

        // recopie dans le buffer de floats
        pui_buf_tmp = pui_buf;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            for (nCol = 0; nCol < nNbCol; nCol++)
            {
                *pZ = (double) (*pui_buf_tmp);
                pZ++;
                pui_buf_tmp++;
            }
        }
        free(pui_buf);
        break;
        //---------- float -------
    case Intersecteur::MntIntersecteur::eTVFloat:
        _TypeVal = Intersecteur::MntIntersecteur::eTVFloat;
        // allocation du buffer tampon
        float * pf_buf;
        float * pf_buf_tmp;
        pf_buf = (float *) malloc(_nc * _nl * sizeof(float));
        pf_buf_tmp = pf_buf;

        // Lecture d'un canal
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(pf_buf_tmp, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            pf_buf_tmp = pf_buf_tmp + nNbCol;
        }

        // recopie dans le buffer de floats
        pf_buf_tmp = pf_buf;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            for (nCol = 0; nCol < nNbCol; nCol++)
            {
                *pZ = (double) (*pf_buf_tmp);
                pZ++;
                pf_buf_tmp++;
            }
        }
        free(pf_buf);
        break;

        //---------- double -------
    case Intersecteur::MntIntersecteur::eTVDouble:
        _TypeVal = Intersecteur::MntIntersecteur::eTVDouble;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(pZ, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            pZ = pZ + nNbCol;
        }
        break;
/*
        // allocation du buffer tampon
        double * pd_buf;
        double * pd_buf_tmp;
        pd_buf = (double *) malloc(_nc * _nl * sizeof(double));
        pd_buf_tmp = pd_buf;

        // Lecture d'un canal
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            int r = fread(pd_buf_tmp, nNbOctPix, nNbCol, f);
            if (r != nNbCol)
            {
                std::cout << "MntIntersecteur::Charger()   Echec lecture canal." << std::endl;
                return false;
            }
            pd_buf_tmp = pd_buf_tmp + nNbCol;
        }

        // recopie dans le buffer de floats
        pd_buf_tmp = pd_buf;
        pZ = _pZ;
        for (nLig = 0; nLig < nNbLig; nLig++)
        {
            for (nCol = 0; nCol < nNbCol; nCol++)
            {
                *pZ = (float) (*pd_buf_tmp);
                pZ++;
                pd_buf_tmp++;
            }
        }
        free(pd_buf);
        break;
*/
        }

    // Fermeture de l'image
    fclose(f);



	/*******************************************************************************************************/
    /*******************************************************************************************************/
    /*******************************************************************************************************/
    std::cout << "x0: "<< x0 <<std::endl;
    std::cout << "y0: "<< y0 <<std::endl;
    std::cout << "px: "<< px <<std::endl;
    std::cout << "py: "<< py <<std::endl;
    std::cout << "nTypeConv: "<< nTypeConv <<std::endl;
    std::cout << "Ident: "<< Ident <<std::endl;
    std::cout << "_nc: "<< _nc <<std::endl;
    std::cout << "_nl: "<< _nl <<std::endl;
    std::cout << "_pZ: "<< _pZ <<std::endl;
    /*    if ( !I3D.Charger(argv[1]) )
        {
            std::cerr << "Erreur au chargement de " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }
    */
    if ( !I3D.Charger(                             x0, // LON_REF
                                                   y0, // LAT_REF
                                                   px, // PAS_LON
                                                   py, // PAS_LAT
                                                   nTypeConv,
                                                   _nc,
                                                   _nl,
                                                   _pZ
                                                   ) )
        {
            std::cerr << "Erreur au chargement du MNT buffurisé." << std::endl;
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
	if ( !I3D.Intersection3D(Point,Visee,PointR,bValid) )
	{
		std::cerr << "Erreur lors de l'intersection" << std::endl;
    	return EXIT_FAILURE;
	}
    std::ofstream flux(argv[8]);

    flux << "Point : " << Point[0] << ", " << Point[1] << ", " << Point[2] << std::endl;
    flux << "Visee : " << Visee[0] << ", " << Visee[1] << ", " << Visee[2] << std::endl;

	if ( bValid )
	{
	    flux << std::fixed <<std::setprecision(3) << "Resultat : " << PointR[0] << ", " << PointR[1] << ", " << PointR[2] << std::endl;
		if ( PointR[2] > Point[2] )
		{
		    flux << "Le point fourni en entree est cache !!!" << std::endl;
		}
	}
	else
	{
	    flux << "Pas d'interesction trouvee" << std::endl;
	}
	flux.close();

    return EXIT_SUCCESS;
}

