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
/*_____________________________________________________________________________

   Program:
       ___  ___     ____  ____  ____
      / __)(__ \   (_  _)(  _ \( ___)
      \__ \ / _/    _)(_  )___/ )__)
      (___/(____)  (____)(__)  (__)


   Group:           CS Systemes d'Information  (France)
                    GMV  (Spain)
                    GAEL Systems  (France)

   SW Component:    IDP-SC
   Author:          CS Systemes d'Information

   Language:        C++
  _____________________________________________________________________________

  HISTORY

  VERSION : 01.00.00 : <AR/CR> : <JIRA #> : <Date> : Creation of the file.

  END-HISTORY
  _____________________________________________________________________________

  $Id: vnsXmlFileHandlerTest.cxx 5019 2013-02-11 14:35:16Z tfeuvrie $
  _____________________________________________________________________________*/



#include <cstdlib>
#include <ctime>
#include "vnsUtilities.h"

#include "vnsXMLFileHandler.h"

int vnsXmlFileHandlerReadXPathTest(int /*argc*/, char * argv[])
{
	vns::XMLFileHandler::Pointer handler = vns::XMLFileHandler::New();
	handler->LoadFile(argv[1]);

	std::string result = handler->GetStringValue(argv[2]);

	std::cout<<"result= "<<result<<std::endl;

	return EXIT_SUCCESS;
}
