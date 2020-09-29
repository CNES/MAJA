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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsExpandImageFilter_txx
#define __vnsExpandImageFilter_txx

#include "vnsExpandImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkMacro.h"
#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{

/**
 * Default constructor
 */
template <class TInputImage, class TOutputImage>
ExpandImageFilter<TInputImage,TOutputImage>
::ExpandImageFilter(void)
{
  // Set the vnsLinearInterpolateImageFunction interpolator as default interpolator
  this->SetInterpolator( LinearInterpolatorType::New() );
}

/**
 * ThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void
ExpandImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType /*threadId*/)
{
  int i;

  // Get the input and output pointers
  OutputImagePointer outputPtr = this->GetOutput();

  // Iterator for walking the output
  typedef itk::ImageRegionIteratorWithIndex<TOutputImage> OutputIterator;

  OutputIterator outIt( outputPtr, outputRegionForThread );

  // Define a few indices that will be used to translate from an input 
  // pixel to and output pixel
  typename TOutputImage::IndexType outputIndex;
  typename InterpolatorType::ContinuousIndexType inputIndex;

  // Support progress methods/callbacks
  
  // Walk the output region, and interpolate the input image
  for( i = 0; !outIt.IsAtEnd(); ++outIt, i++ )
    {
    // Determine the index of the output pixel
    outputIndex = outIt.GetIndex();

    // Determine the input pixel location associated with this output pixel.
    // Don't need to check for division by zero because the factors are
    // clamped to be minimum for 1.
    for( unsigned int j = 0; j < ImageDimension; j++ )
      {
#if 1 // MACCS 4.2 ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
        inputIndex[j] = ( static_cast<double>(outputIndex[j]) + 0.5 ) / static_cast<double>(this->GetExpandFactors()[j]) - 0.5;
#else
        inputIndex[j] = static_cast<double>(outputIndex[j]) / static_cast<double>(this->GetExpandFactors()[j]);
#endif
      }
    
    // interpolate value and write to output
    if( this->GetInterpolator()->IsInsideBuffer( inputIndex ) )
      {
        // Add CONST_EPSILON because error can occurs in cast : 254.9999999999963 -> 254 en int !!!
        outIt.Set( static_cast<OutputPixelType>( 
                   this->GetInterpolator()->EvaluateAtContinuousIndex( inputIndex )+CONST_EPSILON ) );
      }
    else
      {
        outIt.Set( this->GetEdgePaddingValue() );
      }
    } 
 
}


/**
 * GenerateInputRequesteRegion 
 */
template <class TInputImage, class TOutputImage>
void
ExpandImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  // Call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // Get pointers to the input and output
  InputImagePointer inputPtr = 
    const_cast< TInputImage * >( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // We need to compute the input requested region (size and start index)
  unsigned int i;
  const typename TOutputImage::SizeType& outputRequestedRegionSize
    = outputPtr->GetRequestedRegion().GetSize();
  const typename TOutputImage::IndexType& outputRequestedRegionStartIndex
    = outputPtr->GetRequestedRegion().GetIndex();

  typename TInputImage::SizeType  inputRequestedRegionSize;
  typename TInputImage::IndexType inputRequestedRegionStartIndex;

  /**
   * inputRequestedSize = (outputRequestedSize / ExpandFactor) + 1)
   * The extra 1 above is to take care of edge effects when streaming.
   */
  for (i = 0; i < TInputImage::ImageDimension; i++)
    {
#if 1 // MACCS 4.2 ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
      inputRequestedRegionSize[i]
        =  static_cast<long> ( vcl_ceil(static_cast<double> (outputRequestedRegionSize[i]) /
                                        static_cast<double> (this->GetExpandFactors()[i]) ) + 1 );
#else
      // Add one more line, to take care of edge effects when streaming
      inputRequestedRegionSize[i]
        =  static_cast<long> ( vcl_ceil(static_cast<double> (outputRequestedRegionSize[i]) /
                                        static_cast<double> (this->GetExpandFactors()[i]) ) + 1 + 1);
#endif

      inputRequestedRegionStartIndex[i]
        = static_cast<long> ( vcl_floor( static_cast<double> (outputRequestedRegionStartIndex[i]) /
                                         static_cast<double> (this->GetExpandFactors()[i]) ) );
      
    }


  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion.SetSize( inputRequestedRegionSize );
  inputRequestedRegion.SetIndex( inputRequestedRegionStartIndex );

  // Make sure the requested region is within largest possible.
  inputRequestedRegion.Crop( inputPtr->GetLargestPossibleRegion() );

  // Set the input requested region.
  inputPtr->SetRequestedRegion( inputRequestedRegion );

}


} // end namespace vns

#endif
