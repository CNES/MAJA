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
#include "vnsLoggers.h"
#include "vnsUtilities.h"
#include <string>


#define CAST_AND_ADD_IMAGE_BASE(T, image_base)     \
  {                                                \
    T* img = dynamic_cast<T*>(image_base);         \
                                                   \
    if (img)                                       \
    {                                              \
      l_multi_writer->AddInputImage(img, l_filename); \
	  otb::RAMDrivenStrippedStreamingManager<T>::Pointer streamingManager = \
							otb::RAMDrivenStrippedStreamingManager< \
									T>::New(); \
					streamingManager->SetAvailableRAMInMB(availableRAM); \
					streamingManager->PrepareStreaming(img, \
							img->GetLargestPossibleRegion()); \
					l_nb_strip += streamingManager->GetNumberOfSplits(); \
					l_nb_strip_square += streamingManager->GetNumberOfSplits()*streamingManager->GetNumberOfSplits(); \
	  vnsLogDebugMacro("Nb split for file "<<l_filename<<" : "<<streamingManager->GetNumberOfSplits());\
	  continue;                      \
                                                   \
    }                                              \
  } \


namespace vns {

namespace Wrapper {

using namespace otb::Wrapper;

class ImageListWriter: public Application {
public:
	/** Standard class typedefs. */
	typedef ImageListWriter Self;
	typedef otb::Wrapper::Application Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self)
	;

	itkTypeMacro(ImageListWriter, otb::Wrapper::Application)
	;

	/** Some convenient typedefs. */
	typedef DoubleImageType ImageType;
	typedef ImageType::ConstPointer ImageConstPointer;
	typedef ImageType::Pointer ImagePointer;
	typedef ImageType::PixelType PixelType;
	typedef DoubleVectorImageType VectorImageType;
	typedef VectorImageType::ConstPointer VectorImageConstPointer;
	typedef VectorImageType::Pointer VectorImagePointer;
	typedef ImageType::SizeType SizeType;
	typedef ImageType::SpacingType SpacingType;
	typedef ImageType::PointType PointType;

private:

	  template <typename TInputImage>
	  void AddInput(TInputImage*)
	  {

	  }

	void DoInit() {
		SetName("ImageListWriter");
		SetDescription("ImageListWriter.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription(
				"This application write a list of image to files, this is for in memory python use");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Writer");

		AddParameter(ParameterType_InputImageList, "il", "Images");
		SetParameterDescription("il", "Images to write");
		AddParameter(ParameterType_StringList, "filenames",
				"Filenames to write to");
		SetParameterRole("filenames", Role::Role_Input);
		SetParameterDescription("filenames", "filenames to writeimages to");

		AddParameter(ParameterType_Int, "nblinesperstrip",
				"number of lines per strip");
		SetParameterDescription("nblinesperstrip", "number of lines per strip");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram", 2048);
	}

	void DoUpdateParameters() {
	}

	void DoExecute() {
		// Init filters
		vns::Utilities::ListOfStrings l_filename_list(
				GetParameterStringList("filenames"));

		if (GetNumberOfElementsInParameterInputImageList("il")
				!= l_filename_list.size()) {
			vnsExceptionDataMacro(
					"Not the correct number of filenames for images");
		}
		vnsLogDebugMacro(
				"RAM for writing: "<<GetParameterInt("ram"));
		otb::MultiImageFileWriter::Pointer l_multi_writer =
				otb::MultiImageFileWriter::New();
		unsigned int l_nb_strip = 0;
		unsigned int l_nb_strip_square = 0;
		const unsigned int availableRAM = GetParameterInt("ram");
		for (unsigned int i = 0;
				i < GetNumberOfElementsInParameterInputImageList("il"); i++) {
			ImageBaseType* imgBase = GetParameterImageBase("il", i);
			const std::string l_filename = l_filename_list.at(i);
			vnsLogDebugMacro(
					"Filename added : "<<l_filename<<" "<<imgBase->GetNameOfClass());
			// Guess the image type
			std::string className(imgBase->GetNameOfClass());
			if (className == "VectorImage") {
				CAST_AND_ADD_IMAGE_BASE(UInt8VectorImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(Int16VectorImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(UInt16VectorImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(Int32VectorImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(UInt32VectorImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(FloatVectorImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(DoubleVectorImageType,imgBase);
			} else {
				CAST_AND_ADD_IMAGE_BASE(UInt8ImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(Int16ImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(UInt16ImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(Int32ImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(UInt32ImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(FloatImageType,imgBase);
				CAST_AND_ADD_IMAGE_BASE(DoubleImageType,imgBase);
			}
			itkExceptionMacro("Unknown image type");
		}

		//Setup and execute the multiwriter
		double l_deviation = l_nb_strip_square - (1.0*l_nb_strip*l_nb_strip)/static_cast<double>(GetNumberOfElementsInParameterInputImageList("il"));
		l_deviation = std::sqrt(std::abs(l_deviation/static_cast<double>(GetNumberOfElementsInParameterInputImageList("il"))));
		l_nb_strip = std::ceil(1.0 * l_nb_strip / GetNumberOfElementsInParameterInputImageList("il") + 1.0*l_deviation);
		vnsLogInfoMacro("Number of split computed : "<<l_nb_strip);
		l_multi_writer->SetNumberOfDivisionsStrippedStreaming(l_nb_strip);
		l_multi_writer->Update();
		for (unsigned int i = 0;
						i < GetNumberOfElementsInParameterInputImageList("il"); i++) {
			ImageBaseType* imgBase = GetParameterImageBase("il", i);
			imgBase->ReleaseData();
		}
	}

	/** Filters declaration */

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ImageListWriter)
