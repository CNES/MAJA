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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 nov. 2017 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "vnsVectorLookUpTable.h"
#include "vnsMultiChannelsExtractImageFilter.h"
#include "vnsUtilities.h"
#include "vnsAtomicRegistry.h"
/** For LTC select channels */

#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class LutExtractChannels : public Application
{
public:
	/** Standard class typedefs. */
	typedef LutExtractChannels            Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(LutExtractChannels, otb::Wrapper::Application);

	/** Some convenient typedefs. */
    //LUT data
	//Reduced output lut typedef
	typedef VNSLUT3DType ReducedLutType;
	typedef ReducedLutType::ConstPointer ReducedLutConstPointer;
	typedef ReducedLutType::Pointer ReducedLutPointer;
	typedef ReducedLutType::ParameterValuesType ParameterValuesType;
    typedef MultiChannelsExtractImageFilter< ReducedLutType, ReducedLutType> LTCMultiChannelExtractROIType;
	typedef itk::ImageFileReader<ReducedLutType> ImageFileReaderType;
	typedef itk::ImageFileWriter<ReducedLutType> ImageFileWriterType;

private:
	void DoInit()
	{
		SetName("LutExtractChannels");
		SetDescription("LutExtractChannels algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application extract channels from a mha lut file");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputFilename,  "lutimage",   "Lut3D input");
		SetParameterDescription("lutimage", "Reduce LUT");
		AddParameter(ParameterType_OutputFilename,  "lut",   "Lut3D output with selected bands");
		SetParameterDescription("lut", "LUT");
		SetParameterRole("lut",Role_Output);
		// Channelist Parameters
		AddParameter(ParameterType_StringList,  "cl", "Output Image channels");
		SetParameterDescription("cl","Channels to write in the output image.");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_Reader = ImageFileReaderType::New();
		m_LTCSelectBandsOfTheComposite = LTCMultiChannelExtractROIType::New();
		//Load the lut image
		// ---------------------------------------------------------------------------------------------
		m_Reader->SetFileName(GetParameterString("lutimage"));
		m_LTCSelectBandsOfTheComposite->SetInput(m_Reader->GetOutput());
		Utilities::ListOfUIntegers l_ListOfBandIdx =
						vns::Utilities::StringListAsUnsigned(GetParameterStringList("cl"));
		for (unsigned int idx = 0; idx < l_ListOfBandIdx.size(); ++idx)
		{
			m_LTCSelectBandsOfTheComposite->SetChannel(l_ListOfBandIdx[idx] + 1 );
		}
		//Set outputs
		const std::string l_outFileName = GetParameterString("lut");
		itk::ImageFileWriter<ReducedLutType>::Pointer l_Writer = itk::ImageFileWriter<ReducedLutType>::New();
		l_Writer->SetInput(m_LTCSelectBandsOfTheComposite->GetOutput());
		l_Writer->SetFileName(l_outFileName);
		l_Writer->Update();

	}


	/** Filters declaration */
	ImageFileReaderType::Pointer m_Reader;
	LTCMultiChannelExtractROIType::Pointer m_LTCSelectBandsOfTheComposite;

  AtomicRegistry<itk::MetaImageIOFactory> m_MetaRegistry;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::LutExtractChannels)
