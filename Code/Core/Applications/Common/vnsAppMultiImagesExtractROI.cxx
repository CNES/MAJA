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
#include "otbMultiImageFileWriter.h"
#include "vnsMacro.h"
#include "vnsUtilities.h"
#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class MultiImagesExtractROI : public Application
{
public:
	/** Standard class typedefs. */
	typedef MultiImagesExtractROI               Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(MultiImagesExtractROI, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleImageType ImageType;
	typedef ImageType::ConstPointer ImageConstPointer;
	typedef ImageType::Pointer ImagePointer;
	typedef ImageType::PixelType PixelType;
	typedef DoubleVectorImageType                      VectorImageType;
	typedef VectorImageType::ConstPointer     VectorImageConstPointer;
	typedef VectorImageType::Pointer          VectorImagePointer;
	typedef ImageType::SizeType SizeType;
	typedef ImageType::SpacingType SpacingType;
	typedef ImageType::PointType PointType;



private:
	void DoInit()
	{
		SetName("MultiImagesExtractROI");
		SetDescription("MultiImagesExtractROI.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application extract multiple images from multiple images");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Writer");

		AddParameter(ParameterType_InputImageList,  "il",   "Images");
		SetParameterDescription("il", "Images to write");
		AddParameter(ParameterType_StringList,  "filenames",   "Filenames to write to");
		SetParameterRole("filenames",Role::Role_Input);
		SetParameterDescription("filenames", "filenames to writeimages to");
		AddParameter(ParameterType_StringList,  "bandstoextract",   "Band to extract");
		SetParameterRole("bandstoextract",Role::Role_Input);
		SetParameterDescription("bandstoextract", "Description of bands to extact: 4:2,5:3 will extract band2 of file 4 along with band 3 of file 5 in the same output file");
		AddParameter(ParameterType_StringList, "coefs","Coeff to multiply input files");
		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{
	}


	void DoExecute()
	{
		// Init filters
		vns::Utilities::ListOfStrings l_filename_list(GetParameterStringList("filenames"));

		Utilities::ListOfDoubles l_CoefsV= Utilities::StringListAsDouble(
				this->GetParameterStringList("coefs"));
		// Get input image pointers
		FloatVectorImageListType::Pointer inList = GetParameterImageList("il");

		Utilities::ListOfStrings l_Files = Utilities::SplitString(this->GetParameterStringList("bandstoextract"), " ");
		for (unsigned int i = 0;i < l_Files.size(); i++)
		{
			Utilities::ListOfStrings l_Bands = Utilities::SplitString(l_Files[i], ",");
			for (unsigned int b = 0; b < l_Bands.size(); b++)
			{
				Utilities::ListOfStrings l_Dets = Utilities::SplitString(l_Bands[i], ",");
				std::string detFname = bandDetFnames[l_ListOfZone[det]];
				MaskReaderPointer l_ZoneMaskReader = MaskReaderType::New();
				l_ZoneMaskReader->SetFileName(detFname);
				this->m_L2ZoneMaskReaders.push_back(l_ZoneMaskReader);

				BinaryMaskToVectorImageFilterPointerType l_BinaryImageToVectorImage = BinaryMaskToVectorImageFilterType::New();
				l_BinaryImageToVectorImage->SetInput(l_ZoneMaskReader->GetOutput());
				l_BinaryImageToVectorImage->SetNumberOfComponentsPerPixel(8);
				l_BinaryImageToVectorImage->UpdateOutputInformation();
				m_DTFBinaryImageToVectorImageList->PushBack(l_BinaryImageToVectorImage);

				MaskMultiToMonoChannelExtractROIPointerType l_MaskMultiChannelExtractROI = MaskMultiToMonoChannelExtractROIType::New();
				l_MaskMultiChannelExtractROI->SetInput(l_BinaryImageToVectorImage->GetOutput());
				l_MaskMultiChannelExtractROI->SetChannel(m_headerHandler->GetL1DTFImageIndex(l1BandId, l_ListOfZone[det]).at(0));
				l_MaskMultiChannelExtractROI->UpdateOutputInformation();
				m_DTFMaskMultiChannelExtractROIList->PushBack(l_MaskMultiChannelExtractROI);

				l_ZoneMaskByBandfilter->SetNthInput(det, l_MaskMultiChannelExtractROI->GetOutput());

				sstm << ", b" << det + 1 << " * " << l_ListOfZone[det] << " ";
			}
			sstm << ")";
			std::string expr = sstm.str();
			vnsLogDebugMacro("expression: "<< expr)
			l_ZoneMaskByBandfilter->SetExpression(expr);
			m_L2ZoneMaskByBandfilterList.push_back(l_ZoneMaskByBandfilter);

			vnsResourceProbeLogDebugRam()
			MaskReaderPointer l_ZoneMaskByBandCachingReader = MaskReaderType::New();
			vnsCachingMacro(l_ZoneMaskByBandCachingReader, MaskImageType, l_ZoneMaskByBandfilter->GetOutput(), "ZoneMaskByBand_" + Utilities::AsString(bd))
			m_ZoneMaskByBandReadersList.push_back(l_ZoneMaskByBandCachingReader);
			this->m_L2ZoneMaskListVector.at(l2res)->PushBack(l_ZoneMaskByBandCachingReader->GetOutput());

			vnsLogDebugMacro("band loop: " << bd+1 << " / " << nbBand << " ("<< curRes << ")")
		} // band loop
		vnsLogDebugMacro("band loop: END")

		//*********************************************************************************************************
		// L2 Zone mask pipeline connection
		//*********************************************************************************************************
		MaskListToVectorImageFilterPointer l_L2ZoneMaskListToVectorImageFilter = MaskListToVectorImageFilterType::New();
		l_L2ZoneMaskListToVectorImageFilter->SetInput(this->m_L2ZoneMaskListVector.at(l2res));
		this->m_L2ZoneMaskListToVectorImageFilterList.push_back(l_L2ZoneMaskListToVectorImageFilter);
		this->m_L2ZoneOutputList->PushBack(this->m_L2ZoneMaskListToVectorImageFilterList.at(l2res)->GetOutput());

	}




	otb::MultiImageFileWriter::Pointer l_multi_writer = otb::MultiImageFileWriter::New();

	//Setup and execute the multiwriter
	l_multi_writer->SetAutomaticStrippedStreaming(GetParameterInt("ram"));
	l_multi_writer->Update();

}


/** Filters declaration */

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::MultiImagesExtractROI)
