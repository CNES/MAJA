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
#include "Verbeux.h"

namespace Intersecteur
{
	Verbeux::Verbeux() : _outstream(NULL), _errstream(NULL)
	{
		
	}
	
	Verbeux::~Verbeux()
	{
		
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Positionne les streams de sortie et d'erreur
	////////////////////////////////////////////////////////////////////////////////////////////////
	void Verbeux::SetStreams(std::ostream * outstream, std::ostream * errstream)
	{
		_outstream = outstream;
		_errstream = errstream;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Positionne une erreur et l'affiche
	////////////////////////////////////////////////////////////////////////////////////////////////
    void Verbeux::SetError(const std::string & method, const std::string & erreur)
	{
		if ( _errstream ) *_errstream << "Erreur dans la methode " << method <<  " : " << erreur << std::endl;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Positionne un warning et l'affiche. TNT 030305
	////////////////////////////////////////////////////////////////////////////////////////////////
    void Verbeux::SetWarning(const std::string & method, const std::string & erreur)
	{
		if ( _errstream ) *_errstream << "Avertissement dans la methode " << method <<  " : " << erreur << std::endl;
	}
}
