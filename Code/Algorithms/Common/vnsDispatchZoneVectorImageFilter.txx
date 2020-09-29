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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * MOD : VERSION : 1-0-0  : 24 nov. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDispatchZoneVectorImageFilter_txx
#define __vnsDispatchZoneVectorImageFilter_txx

#include "vnsDispatchZoneVectorImageFilter.h"
#include "itkImageScanlineConstIterator.h"
#include "itkImageScanlineIterator.h"
#include "vnsLoggers.h"

namespace vns {

/** Constructor */
template<class TInputImage, class TOutputImage>
DispatchZoneVectorImageFilter<TInputImage, TOutputImage>::DispatchZoneVectorImageFilter() {

	m_NumberOfComponentsPerPixel = 1;
	// This number will be incremented each time an image
	// is added over the two minimum required
	this->SetNumberOfRequiredInputs(2);
	this->InPlaceOff();
}

/** Destructor */
template<class TInputImage, class TOutputImage>
DispatchZoneVectorImageFilter<TInputImage, TOutputImage>::~DispatchZoneVectorImageFilter() {
}

template<class TInputImage, class TOutputImage>
void DispatchZoneVectorImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation() {
	// Call to the superclass implementation
	Superclass::GenerateOutputInformation();

	typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
	typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

	// initialize the number of channels of the output image
	outputPtr->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
}

/** PrintSelf method */
template<class TInputImage, class TOutputImage>
void DispatchZoneVectorImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData() {
	if (m_NumberOfComponentsPerPixel > (sizeof(InputImageInternalPixelType) * 8)) {
		vnsExceptionBusinessMacro(
				"The pixel type used to read the input image in not valid.\n"
						"Its size must be greater than the number of component extracted from the pixel.")
	}

	const unsigned int numberOfInputImages =
			static_cast<unsigned int>(this->GetNumberOfIndexedInputs());
	if (m_ImageOutputValueList.size() != numberOfInputImages)
	{
		vnsExceptionBusinessMacro("The number of output values does  not match the number of input images");
	}
	if (m_BitWeights.size() != numberOfInputImages)
	{
		vnsExceptionBusinessMacro("The number of bit weights does  not match the number of input images");
	}
	for (unsigned int i = 0; i<m_BitWeights.size();i++)
	{
		if (m_BitWeights[i].size() != m_NumberOfComponentsPerPixel)
		{
			vnsExceptionBusinessMacro("The number of bit weights does  not match the number of output components");
		}
	}

}

template<class TInputImage, class TOutputImage>
void DispatchZoneVectorImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(
		const RegionType& outputRegionForThread,
		itk::ThreadIdType /*threadId*/) {

	const SizeValueType size0 = outputRegionForThread.GetSize(0);
	if (size0 == 0) {
		return;
	}
	const unsigned int numberOfInputImages =
			static_cast<unsigned int>(this->GetNumberOfIndexedInputs());

	typedef itk::ImageScanlineConstIterator<TInputImage> ImageScanlineConstIteratorType;
	std::vector<ImageScanlineConstIteratorType *> inputItrVector;
	inputItrVector.reserve(numberOfInputImages);

	// support progress methods/callbacks.
	// count the number of inputs that are non-null
	for (unsigned int i = 0; i < numberOfInputImages; ++i) {
		InputImageConstPointer inputPtr =
				dynamic_cast<TInputImage *>(itk::ProcessObject::GetInput(i));

		if (inputPtr) {
			inputItrVector.push_back(
					new ImageScanlineConstIteratorType(inputPtr,
							outputRegionForThread));
		}
	}

	const unsigned int numberOfValidInputImages =
			static_cast<const unsigned int>(inputItrVector.size());

	if (numberOfValidInputImages == 0) {
		//No valid regions in the thread
		//(and no region iterators to delete)
		return;
	}

	NaryArrayType naryInputArray(numberOfValidInputImages);

	OutputImagePointer outputPtr = this->GetOutput(0);
	itk::ImageScanlineIterator<TOutputImage> outputIt(outputPtr,
			outputRegionForThread);

	typename std::vector<ImageScanlineConstIteratorType *>::iterator regionIterators;
	const typename std::vector<ImageScanlineConstIteratorType *>::const_iterator regionItEnd =
			inputItrVector.end();

	typename NaryArrayType::iterator arrayIt;
	//Pre allocate output vector pixel
	OutputImagePixelType outputVectorValue;
	outputVectorValue.SetSize(m_NumberOfComponentsPerPixel);
	outputVectorValue.Fill(
			itk::NumericTraits<OutputImageInternalPixelType>::Zero);
	const unsigned int nbValue = outputVectorValue.GetSize();

	while (!outputIt.IsAtEnd()) {
		while (!outputIt.IsAtEndOfLine()) {
			outputVectorValue.Fill(
					itk::NumericTraits<OutputImageInternalPixelType>::Zero);
			arrayIt = naryInputArray.begin();
			regionIterators = inputItrVector.begin();
			while (regionIterators != regionItEnd) {
				*arrayIt++ = static_cast<OutputImageInternalPixelType>((*regionIterators)->Get()[0]);
				++(*(*regionIterators));
				++regionIterators;
			}
			//Do the job
			//Image in loop
			for (unsigned int i = 0; i < naryInputArray.size(); i++) {
				if (naryInputArray[i] != 0)
				{
					// Band loop
					for (unsigned int j = 0; j < nbValue; j++) {
						if (outputVectorValue[j] < m_ImageOutputValueList[i]) {
							// Set to one the bit to the associated band
							const bool isSet = (naryInputArray[i] & m_BitWeights[i][j]) != 0;
							if (isSet)
							{
								outputVectorValue[j] = static_cast<OutputImageInternalPixelType>(m_ImageOutputValueList[i]);
							}
						}
					}
				}
			}
			// Set the output pixel value
			outputIt.Set(outputVectorValue);
			++outputIt;
		}

		regionIterators = inputItrVector.begin();
		while (regionIterators != regionItEnd) {
			(*regionIterators)->NextLine();
			++regionIterators;
		}
		outputIt.NextLine();
	}

	// Free memory
	regionIterators = inputItrVector.begin();
	while (regionIterators != regionItEnd) {
		delete (*regionIterators++);
	}
}

/** PrintSelf method */
template<class TInputImage, class TOutputImage>
void DispatchZoneVectorImageFilter<TInputImage, TOutputImage>::PrintSelf(
		std::ostream& os, itk::Indent indent) const {
	Superclass::PrintSelf(os, indent);
}
} // End namespace vns

#endif /* __vnsConcatenatePerZoneVectorImageFilter_txx */
