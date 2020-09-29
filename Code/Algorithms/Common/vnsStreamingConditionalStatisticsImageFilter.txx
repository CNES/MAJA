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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-153190-CS : 3 fevrier 2017 : Correction de "runtime error"  levés     *
 *                                              par MAJA compilé avec les options de sanaisation de clang++ *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-127944-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 8 novembre 2013 : Mise à jour du plugin SENTINEL2          *
 * 					Action interne 98664. Prise en compte  des modifications apportées à la classe   		*
 * 					otbStreamingStatisticsVectorImageFilter v3.13   										*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsStreamingConditionalStatisticsImageFilter.txx 10384 2015-06-05 16:23:02Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStreamingConditionalStatisticsImageFilter_txx
#define __vnsStreamingConditionalStatisticsImageFilter_txx

#include "vnsStreamingConditionalStatisticsImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsLoggers.h"
#include "vnsUtilities.h"

namespace vns
{

template<class TInputImage, class TInputMask, class TPrecision>
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::PersistentStreamingConditionalStatisticsImageFilter() :
m_EnableMinMax(true), m_EnableFirstOrderStats(true), m_EnableVariance(false), m_EnableExcludeValue(true), m_UseUnbiasedEstimator(false), m_ExcludedValue(
		static_cast<InternalPixelType>(0)), m_MaskForegroundValue(static_cast<MaskPixelType>(0)), m_IsValid(false), m_UseMaskInput(
				false)
				{
	// first output is a copy of the image, DataObject created by
	// superclass

	// allocate the data objects for the outputs which are
	// just decorators around vector/matrix types
	for (unsigned int i = 1; i < 9 ; ++i)
	{
		this->itk::ProcessObject::SetNthOutput(i, this->MakeOutput(i).GetPointer());
	}
				}

template<class TInputImage, class TInputMask, class TPrecision>
itk::DataObject::Pointer
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::MakeOutput(unsigned int output)
{
	switch (output)
	{
	// Input image
	case 0:
		return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
		// 1 and 2 outputs : pixel objects
	case 1:
	case 2:
		// min/max
		return static_cast<itk::DataObject*>(PixelObjectType::New().GetPointer());
		// 3 and 4 outputs : Real pixel objects
	case 3:
	case 4:
	case 5:
	case 6:
		// mean / sum / variance / standard deviation
		return static_cast<itk::DataObject*>(RealPixelObjectType::New().GetPointer());
	case 7:
	case 8:
		return static_cast<itk::DataObject*>(UIObjectType::New().GetPointer());
	default:
		// might as well make an image
		return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
	}
}

template<class TInputImage, class TInputMask, class TPrecision>
typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::PixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetMinimumOutput()
{
	return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TInputMask, class TPrecision>
const typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::PixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetMinimumOutput() const
{
	return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TInputMask, class TPrecision>
typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::PixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetMaximumOutput()
{
	return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TInputMask, class TPrecision>
const typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::PixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetMaximumOutput() const
{
	return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TInputMask, class TPrecision>
typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetMeanOutput()
{
	return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage, class TInputMask, class TPrecision>
const typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetMeanOutput() const
{
	return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage, class TInputMask, class TPrecision>
typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetSumOutput()
{
	return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage, class TInputMask, class TPrecision>
const typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetSumOutput() const
{
	return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage, class TInputMask, class TPrecision>
typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetVarianceOutput()
{
	return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(5));
}

template<class TInputImage, class TInputMask, class TPrecision>
const typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetVarianceOutput() const
{
	return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(5));
}

template<class TInputImage, class TInputMask, class TPrecision>
typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetStandardDeviationOutput()
{
	return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(6));
}

template<class TInputImage, class TInputMask, class TPrecision>
const typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetStandardDeviationOutput() const
{
	return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(6));
}

template<class TInputImage, class TInputMask, class TPrecision>
typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::UIObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfValidValuesOutput()
{
	return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(7));
}

template<class TInputImage, class TInputMask, class TPrecision>
const typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::UIObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfValidValuesOutput() const
{
	return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(7));
}

template<class TInputImage, class TInputMask, class TPrecision>
typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::UIObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfExcludedValuesOutput()
{
	return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(8));
}

template<class TInputImage, class TInputMask, class TPrecision>
const typename PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::UIObjectType*
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfExcludedValuesOutput() const
{
	return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(8));
}

template<class TInputImage, class TInputMask, class TPrecision>
void
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::GenerateOutputInformation()
{
	Superclass::GenerateOutputInformation();
	if (this->GetInput())
	{
		this->GetOutput()->CopyInformation(this->GetInput());
		this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

		if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
		{
			this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
		}
	}
}

template<class TInputImage, class TInputMask, class TPrecision>
void
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::AllocateOutputs()
{
	// This is commented to prevent the streaming of the whole image for the first stream strip
	// It shall not cause any problem because the output image of this filter is not intended to be used.
	//InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
	//this->GraftOutput( image );
	// Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage, class TInputMask, class TPrecision>
void
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::Reset()
{
	// Grab the input
	TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
	inputPtr->UpdateOutputInformation();

	// Get the number of threads
	const unsigned int numberOfThreads = this->GetNumberOfThreads();

	// Class parameters
	m_NumberOfValidPixels.SetSize(numberOfThreads);
	m_NumberOfValidPixels.Fill(itk::NumericTraits<unsigned long>::Zero);

	this->GetNbOfValidValuesOutput()->Set(itk::NumericTraits<unsigned long>::Zero);
	this->GetNbOfExcludedValuesOutput()->Set(itk::NumericTraits<unsigned long>::Zero);

	// If the Min and Max values are computed
	if (m_EnableMinMax)
	{
		const PixelType tempPixelMax(itk::NumericTraits<InternalPixelType>::max());
		const PixelType tempPixelNonpositiveMin(itk::NumericTraits<InternalPixelType>::NonpositiveMin());
		// Initialization of the min and max values
		this->GetMinimumOutput()->Set(tempPixelMax);
		this->GetMaximumOutput()->Set(tempPixelNonpositiveMin);

		// Initialization of the values per thread
		m_ThreadMin = std::vector<PixelType>(numberOfThreads, tempPixelMax);
		m_ThreadMax = std::vector<PixelType>(numberOfThreads, tempPixelNonpositiveMin);
	}

	if (m_EnableVariance)
	{
		m_EnableFirstOrderStats = true;
	}

	// Initialization of the first order statistics
	if (m_EnableFirstOrderStats)
	{
		RealPixelType zeroRealPixel(itk::NumericTraits<PrecisionType>::ZeroValue());
		this->GetMeanOutput()->Set(zeroRealPixel);
		this->GetSumOutput()->Set(zeroRealPixel);
		m_ThreadFirstOrderAccumulators.resize(numberOfThreads);
		std::fill(m_ThreadFirstOrderAccumulators.begin(), m_ThreadFirstOrderAccumulators.end(), zeroRealPixel);
	}

	// Initialization of the first order statistics
	if (m_EnableVariance)
	{
		RealPixelType zeroRealPixel(itk::NumericTraits<PrecisionType>::ZeroValue());
		this->GetVarianceOutput()->Set(zeroRealPixel);
		this->GetStandardDeviationOutput()->Set(zeroRealPixel);
		m_ThreadSumOfSquaresAccumulators.resize(numberOfThreads);
		std::fill(m_ThreadSumOfSquaresAccumulators.begin(), m_ThreadSumOfSquaresAccumulators.end(), zeroRealPixel);
	}

	// Boolean used to indicate if the number of pixels used to compute the statistics is not null
	m_IsValid = false;

}

template<class TInputImage, class TInputMask, class TPrecision>
void
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::Synthetize()
{
	TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
	const unsigned int nbPixels = inputPtr->GetLargestPossibleRegion().GetNumberOfPixels();

	// Initializations
	PixelType minimum(itk::NumericTraits<InternalPixelType>::max());
	PixelType maximum(itk::NumericTraits<InternalPixelType>::NonpositiveMin());
	RealPixelType streamFirstOrderAccumulator = itk::NumericTraits<RealPixelType>::Zero;
	RealPixelType streamSumOfSquaresAccumulator = itk::NumericTraits<RealPixelType>::Zero;
	unsigned long nbValidPixels(0);

	// Accumulate results from all threads
	const unsigned int numberOfThreads = this->GetNumberOfThreads();
	for (unsigned int threadId = 0; threadId < numberOfThreads; ++threadId)
	{
		if (m_EnableMinMax)
		{
			const PixelType& threadMin = m_ThreadMin[threadId];
			const PixelType& threadMax = m_ThreadMax[threadId];

			// Search the min and max value of all the threads and for each spectral band
			if (threadMin < minimum)
			{
				minimum = threadMin;
			}
			if (threadMax > maximum)
			{
				maximum = threadMax;
			}
			nbValidPixels += m_NumberOfValidPixels[threadId];
		}

		if (m_EnableFirstOrderStats)
		{
			streamFirstOrderAccumulator += m_ThreadFirstOrderAccumulators[threadId];
		}
		if (m_EnableVariance)
		{
			streamSumOfSquaresAccumulator += m_ThreadSumOfSquaresAccumulators[threadId];

		}
	}

	// Set null statistics if the number of valid pixels (not equal to the input excluded value) is null
	if (nbValidPixels == 0)
	{
		m_IsValid = false;

		vnsLogWarningMacro("PersistentStreamingConditionalStatisticsImageFilter::Synthetize."
				"No pixel is valid. Return null statistics");

		// Set default values
		this->GetMinimumOutput()->Set(itk::NumericTraits<InternalPixelType>::Zero);
		this->GetMaximumOutput()->Set(itk::NumericTraits<InternalPixelType>::Zero);
		this->GetSumOutput()->Set(static_cast<RealPixelType>(streamFirstOrderAccumulator));
		this->GetMeanOutput()->Set(static_cast<RealPixelType>(streamFirstOrderAccumulator));
		this->GetVarianceOutput()->Set(static_cast<RealPixelType>(itk::NumericTraits<RealPixelType>::Zero));
		this->GetStandardDeviationOutput()->Set(static_cast<RealPixelType>(itk::NumericTraits<RealPixelType>::Zero));
		// Set the number of used pixels to compute statistics and the number of excluded values
		this->GetNbOfValidValuesOutput()->Set(nbValidPixels);
		this->GetNbOfExcludedValuesOutput()->Set(nbPixels - nbValidPixels);
	}
	// Set the outputs
	else
	{
		m_IsValid = true;

		// Final calculations
		if (m_EnableMinMax)
		{
			this->GetMinimumOutput()->Set(minimum);
			this->GetMaximumOutput()->Set(maximum);
		}

		if (m_EnableFirstOrderStats)
		{
			// Compute the sum, the mean and the mean for all the components tacking into account the valid pixels (and not the excluded one)
			this->GetMeanOutput()->Set(streamFirstOrderAccumulator / static_cast<RealType>(nbValidPixels)); //nbPixels);
			this->GetSumOutput()->Set(streamFirstOrderAccumulator);
		}
		if (m_EnableVariance)
		{
			double /*RealPixelType*/l_Variance = (streamSumOfSquaresAccumulator
					- (streamFirstOrderAccumulator * streamFirstOrderAccumulator) / static_cast<double>(nbValidPixels));
			l_Variance /= static_cast<double>(nbValidPixels); // - 1);
			this->GetVarianceOutput()->Set(l_Variance);
			this->GetStandardDeviationOutput()->Set(std::sqrt(vcl_abs(l_Variance)));

		}

		// Set the number of used pixels to compute statistics and the number of excluded values
		this->GetNbOfValidValuesOutput()->Set(nbValidPixels);
		this->GetNbOfExcludedValuesOutput()->Set(nbPixels - nbValidPixels);

	} // if some pixels valid

}

template<class TInputImage, class TInputMask, class TPrecision>
void
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::ThreadedGenerateData(
		const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
		{
	// Grab the input
	InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
	itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);
	const double l_ExcludedValue = static_cast<double>(m_ExcludedValue);

	if (m_UseMaskInput == false)
	{
		for (it.GoToBegin(); !it.IsAtEnd(); ++it) //MACCS 4.3 - disable progress : progress.CompletedPixel())
		{
			const PixelType& vectorValue = it.Get();

			// If a pixel is set to the excluded value in one band, it is excluded
			// for all bands
			// Check with excluded value
			if ((m_EnableExcludeValue == false) || vnsEqualsDoubleMacro(static_cast<double>(vectorValue), l_ExcludedValue) == false)
			{
				// Compute min and max for the current thread
				if (m_EnableMinMax)
				{
					if (vectorValue < m_ThreadMin[threadId])
					{
						m_ThreadMin[threadId] = vectorValue;
					}
					if (vectorValue > m_ThreadMax[threadId])
					{
						m_ThreadMax[threadId] = vectorValue;
					}
				}
				// Compute the sum used to calculate the mean values for this thread
				if (m_EnableFirstOrderStats)
				{
					m_ThreadFirstOrderAccumulators[threadId] += vectorValue;
				}
				// If compute Variance
				if (m_EnableVariance)
				{
					m_ThreadSumOfSquaresAccumulators[threadId] += (vectorValue * vectorValue);
				}
				// Count the number of valid pixels per thread
				m_NumberOfValidPixels[threadId]++;
			}
		}

	}
	else
	{
		// Get the input mask
		InputMaskPointer maskPtr = dynamic_cast<TInputMask *>(this->itk::ProcessObject::GetInput(1));
		itk::ImageRegionConstIteratorWithIndex<TInputMask> maskIt(maskPtr, outputRegionForThread);

		const double l_DoubleForegroundValue = static_cast<double>(m_MaskForegroundValue);
		maskIt.GoToBegin();

		for (it.GoToBegin(); !it.IsAtEnd(); ++it) //MACCS 4.3 - disable progress : progress.CompletedPixel())
		{
			// Get the values of the mask pixels used for the selection of the pixels
			const MaskPixelType& maskValue = maskIt.Get();

			// If a pixel is set to the excluded value in one band, it is excluded
			// for all bands
			// Check with excluded value
			if (vnsEqualsDoubleMacro(static_cast<double>(maskValue), l_DoubleForegroundValue) == true)
			{
				const PixelType& vectorValue = it.Get();
				if ((m_EnableExcludeValue == false) || vnsEqualsDoubleMacro(static_cast<double>(vectorValue), l_ExcludedValue) == false)
				{
					// Compute min and max for the current thread
					if (m_EnableMinMax)
					{
						if (vectorValue < m_ThreadMin[threadId])
						{
							m_ThreadMin[threadId] = vectorValue;
						}
						if (vectorValue > m_ThreadMax[threadId])
						{
							m_ThreadMax[threadId] = vectorValue;
						}
					}
					// Compute the sum used to calculate the mean values for this thread
					if (m_EnableFirstOrderStats)
					{
						m_ThreadFirstOrderAccumulators[threadId] += vectorValue;
					}
					// If compute Variance
					if (m_EnableVariance)
					{
						m_ThreadSumOfSquaresAccumulators[threadId] += (vectorValue * vectorValue);
					}
					// Count the number of valid pixels per thread
					m_NumberOfValidPixels[threadId]++;
				}
			}// end if excluded value
			++maskIt;
		}

	}
		}

template<class TInputImage, class TInputMask, class TPrecision>
void
PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision>::PrintSelf(std::ostream& os,
		itk::Indent indent) const
		{
	Superclass::PrintSelf(os, indent);

	os << indent << "Min: " << this->GetMinimumOutput()->Get() << std::endl;
	os << indent << "Max: " << this->GetMaximumOutput()->Get() << std::endl;
	os << indent << "Mean: " << this->GetMeanOutput()->Get() << std::endl;
	os << indent << "Variance: " << this->GetVarianceOutput()->Get() << std::endl;
	os << indent << "UseMaskInput: " << Utilities::BoolToLowerString(this->m_UseMaskInput) << std::endl;
	os << indent << "MaskForegroundValue: " << m_MaskForegroundValue << std::endl;
	os << indent << "ExcludedValue: " << m_ExcludedValue << std::endl;
	os << indent << "StandardDeviation: " << this->GetStandardDeviationOutput()->Get() << std::endl;
	os << indent << "UseUnbiasedEstimator: " << Utilities::BoolToLowerString(this->m_UseUnbiasedEstimator) << std::endl;
	os << indent << "ValidVal: " << this->GetNbOfValidValuesOutput()->Get() << std::endl;
	os << indent << "ExcludedVal: " << this->GetNbOfExcludedValuesOutput()->Get() << std::endl;
		}

} // end namespace otb
#endif
