/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbMultiChannelsExtractImageFilter_h
#define __otbMultiChannelsExtractImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkMacro.h"
#include <vector>
#include "vnsUtilities.h"

namespace vns
{

/** \class MultiChannelsExtractImageFilter
 * \brief Extract a spatial or spectral subset of a multi-channel image.
 *
 * It is possible to extract all the channels from the input image or only those specified by the user.
 * The SetChannel() method allows to select one channel.
 * The SetFirstChannel() and SetLastChannel() methods allow the user to define a list of channels.
 *
 * \note If no channels are specified, then all channels from the input image are selected.
 * \note The image manipulated inside this class are of type otb::VectorImage.
 */
template <class TInputImage, class TOutputImage>
class MultiChannelsExtractImageFilter :
//public otb::ExtractROIBase<otb::VectorImage<TInputImage, 3>, otb::VectorImage<TOutputImage, 3> >
public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelsExtractImageFilter                                                             Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
//  typedef otb::ExtractROIBase<otb::VectorImage<TInputImage, 3>, otb::VectorImage<TOutputImage, 3> > Superclass;
  typedef itk::SmartPointer<Self>                                                            Pointer;
  typedef itk::SmartPointer<const Self>                                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelsExtractImageFilter, itk::ImageToImageFilter);

  /** Image type information. */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** Pixel type information */
  typedef typename OutputImageType::InternalPixelType OutputValueType;

  /** Typedef to describe the output and input image region types. */
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::RegionType  InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename InputImageType::PixelType  InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename InputImageType::IndexType  InputImageIndexType;
  typedef typename OutputImageType::SizeType  OutputImageSizeType;
  typedef typename InputImageType::SizeType   InputImageSizeType;

  /** Typedef Liste des canaux */
  typedef Utilities::ListOfUIntegers ChannelsType;

  /** \return The list of processed channels */
  ChannelsType GetChannels() const
  {
    return (m_Channels);
  }

  /** \return The number of processed channels */
  unsigned int GetNbChannels() const
  {
    return (m_Channels.size());
  }

  /** Select a channel to process
   \param channel The channel to process */
  void SetChannel(unsigned int channel);

  /** Clear channels selection */
  void ClearChannels(void);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

protected:
  MultiChannelsExtractImageFilter();
  virtual ~MultiChannelsExtractImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** MultiChannelsExtractImageFilter can produce an image which is a different
   * resolution than its input image.  As such, MultiChannelsExtractImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** ExtractImageFilter can be implemented as a  multithreaded filter.
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

private:
  MultiChannelsExtractImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** List of channels to process  [1...] */
  ChannelsType m_Channels;
};

} // end namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMultiChannelsExtractImageFilter.txx"
#endif

#endif
