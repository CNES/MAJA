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

#include <iostream>
#include "vnsLoggers.h"

static int TestCharge()
{
	vns::Loggers* log = vns::Loggers::GetInstance();
    	
	return (log == NULL)?EXIT_FAILURE:EXIT_SUCCESS;
}

static int TestConfigure()
{
	vns::Loggers* log = vns::Loggers::GetInstance();
    log->Initialize("Test Logger");
	return EXIT_SUCCESS;
}

static int TestMessageSort()
{

	vns::Loggers* log = vns::Loggers::GetInstance();
	log->SetMinLevel(vns::LoggerBase::INFO);

	vnsLogInfoMacro("Message INFO : should appear");

	vnsLogDebugMacro("Message DEBUG : should not appear");

	return EXIT_SUCCESS;

}

int vnsLoggerTU(int argc, char* argv[])
{

	if (TestCharge() != EXIT_SUCCESS)
	{
		std::cout << "TestCharge failed !" << std::endl;
		return EXIT_FAILURE;
	}

	if (TestConfigure() != EXIT_SUCCESS)
	{
		std::cout << "TestConfigure failed !" << std::endl;
		return EXIT_FAILURE;
	}

	if (TestMessageSort() != EXIT_SUCCESS)
	{
		std::cout << "TestMessageSort failed !" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}


