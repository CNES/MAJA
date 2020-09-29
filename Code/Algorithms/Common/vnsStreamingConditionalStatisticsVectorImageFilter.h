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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 8 novembre 2013 : Mise à jour du plugin SENTINEL2          *
 * 					Action interne 98664. Prise en compte  des modifications apportées à la classe   		*
 * 					otbStreamingStatisticsVectorImageFilter v3.13   										*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 avr. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStreamingConditionalStatisticsVectorImageFilter_h
#define __vnsStreamingConditionalStatisticsVectorImageFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"

namespace vns
{

/** \class PersistentStreamingConditionalStatisticsVectorImageFilter
 * \brief Compute covariance & correlation of a large image using streaming excluding
 * the pixels equal to a value specified by the input parameter m_ExcludedValue.
 *
 *  This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the statitics of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 * Note that if a pixel is equal to m_ExcludedValue in one band, it is considered equal to
 * m_ExcludedValue in all bands.
 *
 * \sa PersistentImageFilter
 * \sa PersistentConditionalStatisticsVectorImageFilter
 *
 * \ingroup L2
 *
 */
template<class TInputImage, class TPrecision >
class PersistentStreamingConditionalStatisticsVectorImageFilter :
  public otb::PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentStreamingConditionalStatisticsVectorImageFilter           Self;
  typedef otb::PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStreamingConditionalStatisticsVectorImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                           ImageType;
  typedef typename ImageType::Pointer           InputImagePointer;
  typedef typename ImageType::RegionType        RegionType;
  typedef typename ImageType::SizeType          SizeType;
  typedef typename ImageType::IndexType         IndexType;
  typedef typename ImageType::PixelType         PixelType;
  typedef typename ImageType::InternalPixelType InternalPixelType;

  typedef TPrecision                            PrecisionType;
  typedef PrecisionType                         RealType;

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  /** Type to use for computations. */
  typedef itk::VariableSizeMatrix<PrecisionType>        MatrixType;
  typedef itk::VariableLengthVector<PrecisionType>      RealPixelType;

  /** Type of DataObjects used for outputs */
  typedef itk::SimpleDataObjectDecorator<RealType>      RealObjectType;
  typedef itk::SimpleDataObjectDecorator<IndexType>     IndexObjectType;
  typedef itk::SimpleDataObjectDecorator<PixelType>     PixelObjectType;
  typedef itk::SimpleDataObjectDecorator<RealPixelType> RealPixelObjectType;
  typedef itk::SimpleDataObjectDecorator<MatrixType>    MatrixObjectType;

  // Used to count the number of valid pixels
  typedef itk::SimpleDataObjectDecorator<unsigned int> UIObjectType;

  /** Set the excluded value in the statistic computation. */
  itkSetMacro(ExcludedValue, InternalPixelType);
  /** Get the excluded value in the statistic computation. */
  itkGetConstReferenceMacro(ExcludedValue, InternalPixelType);

  /** Return the computed Min */
  PixelType GetMinimum() const
  {
    return this->GetMinOutput()->Get();
  }
  PixelObjectType* GetMinimumOutput();
  const PixelObjectType* GetMinimumOutput() const;

  /** Return the computed Max index */
  PixelType GetMaximum() const
  {
    return this->GetMaxOutput()->Get();
  }
  PixelObjectType* GetMaximumOutput();
  const PixelObjectType* GetMaximumOutput() const;

  /** Return the global mean of all the internal pixel values
   * (flattening the multispectral image as a 1D-vector) */
  RealType GetComponentMean() const
  {
    return this->GetComponentMeanOutput()->Get();
  }
  RealObjectType* GetComponentMeanOutput();
  const RealObjectType* GetComponentMeanOutput() const;

  /** Return the global correlation of all the internal pixel values
   * (flattening the multispectral image as a 1D-vector) */
  RealType GetComponentCorrelation() const
  {
    return this->GetComponentCorrelationOutput()->Get();
  }
  RealObjectType* GetComponentCorrelationOutput();
  const RealObjectType* GetComponentCorrelationOutput() const;

  /** Return the global covariance of all the internal pixel values
   * (flattening the multispectral image as a 1D-vector) */
  RealType GetComponentCovariance() const
  {
    return this->GetComponentCovarianceOutput()->Get();
  }
  RealObjectType* GetComponentCovarianceOutput();
  const RealObjectType* GetComponentCovarianceOutput() const;

  /** Return the computed Mean. */
  RealPixelType GetMean() const
  {
    return this->GetMeanOutput()->Get();
  }
  RealPixelObjectType* GetMeanOutput();
  const RealPixelObjectType* GetMeanOutput() const;

  /** Return the computed Sum. */
  RealPixelType GetSum() const
  {
    return this->GetSumOutput()->Get();
  }
  RealPixelObjectType* GetSumOutput();
  const RealPixelObjectType* GetSumOutput() const;

  /** Return the computed Covariance. */
  MatrixType GetCorrelation() const
  {
    return this->GetCorrelation()->Get();
  }
  MatrixObjectType* GetCorrelationOutput();
  const MatrixObjectType* GetCorrelationOutput() const;

  /** Return the computed Covariance. */
  MatrixType GetCovariance() const
  {
    return this->GetCovarianceOutput()->Get();
  }
  MatrixObjectType* GetCovarianceOutput();
  const MatrixObjectType* GetCovarianceOutput() const;

  /** Return the number of valid values. */
  unsigned int GetNbOfValidValues() const
  {
	  return this->GetNbOfValidValuesOutput()->Get();
  }
  UIObjectType* GetNbOfValidValuesOutput();
  const UIObjectType* GetNbOfValidValuesOutput() const;

  /** Return the number of excluded values. */
  unsigned int GetNbOfExcludedValues() const
  {
	  return this->GetNbOfExcludedValuesOutput()->Get();
  }
  UIObjectType* GetNbOfExcludedValuesOutput();
  const UIObjectType* GetNbOfExcludedValuesOutput() const;

  /** Check the validity of the statistics */
  itkGetConstMacro(IsValid,bool)

  /** Make a DataObject of the correct type to be used as the specified
   * output.
   */
  virtual DataObjectPointer MakeOutput(unsigned int idx);

  virtual void Reset(void);

  virtual void Synthetize(void);

  itkSetMacro(EnableMinMax, bool);
  itkGetMacro(EnableMinMax, bool);

  itkSetMacro(EnableFirstOrderStats, bool);
  itkGetMacro(EnableFirstOrderStats, bool);

  itkSetMacro(EnableSecondOrderStats, bool);
  itkGetMacro(EnableSecondOrderStats, bool);

  itkSetMacro(UseUnbiasedEstimator, bool);
  itkGetMacro(UseUnbiasedEstimator, bool);

protected:
  PersistentStreamingConditionalStatisticsVectorImageFilter();

  virtual ~PersistentStreamingConditionalStatisticsVectorImageFilter() {}

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  virtual void AllocateOutputs();

  virtual void GenerateOutputInformation();


  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
  PersistentStreamingConditionalStatisticsVectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  bool m_EnableMinMax;
  bool m_EnableFirstOrderStats;
  bool m_EnableSecondOrderStats;

  /* use an unbiased estimator to compute the covariance */
  bool m_UseUnbiasedEstimator;

  std::vector<PixelType>     m_ThreadMin;
  std::vector<PixelType>     m_ThreadMax;
  std::vector<RealType>      m_ThreadFirstOrderComponentAccumulators;
  std::vector<RealType>      m_ThreadSecondOrderComponentAccumulators;
  std::vector<RealPixelType> m_ThreadFirstOrderAccumulators;
  std::vector<MatrixType>    m_ThreadSecondOrderAccumulators;

  itk::Array<long>	m_NumberOfValidPixels;
  InternalPixelType m_ExcludedValue;

  /** Check is the statistics are valid */
  bool m_IsValid;

}; // end of class PersistentStreamingConditionalStatisticsVectorImageFilter

/**===========================================================================*/

/** \class StreamingConditionalStatisticsVectorImageFilter
 * \brief This class streams the whole input image through the PersistentStatisticsImageFilter.
 *
 * This way, it allows to compute the first and second order global statistics of this image. It calls the
 * Reset() method of the PersistentStreamingConditionalStatisticsVectorImageFilter before streaming the image and the
 * Synthetize() method of the PersistentStreamingConditionalStatisticsVectorImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentStreamingConditionalStatisticsVectorImageFilter.
 *
 * \sa PersistentStreamingConditionalStatisticsVectorImageFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */

template<class TInputImage, class TPrecision = typename itk::NumericTraits<typename TInputImage::InternalPixelType>::RealType>
class StreamingConditionalStatisticsVectorImageFilter :
  public otb::PersistentFilterStreamingDecorator<PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision> >
{
public:
  /** Standard Self typedef */
  typedef StreamingConditionalStatisticsVectorImageFilter Self;
  typedef otb::PersistentFilterStreamingDecorator
  <PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingConditionalStatisticsVectorImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputImage                                 InputImageType;
  typedef typename Superclass::FilterType             StatFilterType;

  /** Type of DataObjects used for outputs */
  typedef typename StatFilterType::PixelType           PixelType;
  typedef typename StatFilterType::InternalPixelType   InternalPixelType;
  typedef typename StatFilterType::RealType            RealType;
  typedef typename StatFilterType::RealObjectType      RealObjectType;
  typedef typename StatFilterType::RealPixelType       RealPixelType;
  typedef typename StatFilterType::RealPixelObjectType RealPixelObjectType;
  typedef typename StatFilterType::MatrixType          MatrixType;
  typedef typename StatFilterType::MatrixObjectType    MatrixObjectType;

  // Used to count the number of valid pixels
  typedef typename StatFilterType::UIObjectType UIObjectType;

  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Return the computed Mean. */
  RealPixelType GetMinimum() const
  {
    return this->GetFilter()->GetMinimumOutput()->Get();
  }
  RealPixelObjectType* GetMinimumOutput()
  {
    return this->GetFilter()->GetMinimumOutput();
  }
  const RealPixelObjectType* GetMinimumOutput() const
  {
    return this->GetFilter()->GetMinimumOutput();
  }

  /** Return the computed Mean. */
  RealPixelType GetMaximum() const
  {
    return this->GetFilter()->GetMaximumOutput()->Get();
  }
  RealPixelObjectType* GetMaximumOutput()
  {
    return this->GetFilter()->GetMaximumOutput();
  }
  const RealPixelObjectType* GetMaximumOutput() const
  {
    return this->GetFilter()->GetMaximumOutput();
  }

  /** Return the computed Mean. */
  RealPixelType GetMean() const
  {
    return this->GetFilter()->GetMeanOutput()->Get();
  }
  RealPixelObjectType* GetMeanOutput()
  {
    return this->GetFilter()->GetMeanOutput();
  }
  const RealPixelObjectType* GetMeanOutput() const
  {
    return this->GetFilter()->GetMeanOutput();
  }

  /** Return the computed Mean. */
  RealPixelType GetSum() const
  {
    return this->GetFilter()->GetSumOutput()->Get();
  }
  RealPixelObjectType* GetSumOutput()
  {
    return this->GetFilter()->GetSumOutput();
  }
  const RealPixelObjectType* GetSumOutput() const
  {
    return this->GetFilter()->GetSumOutput();
  }

  /** Return the computed Covariance. */
  MatrixType GetCovariance() const
  {
    return this->GetFilter()->GetCovarianceOutput()->Get();
  }
  MatrixObjectType* GetCovarianceOutput()
  {
    return this->GetFilter()->GetCovarianceOutput();
  }
  const MatrixObjectType* GetCovarianceOutput() const
  {
    return this->GetFilter()->GetCovarianceOutput();
  }

  /** Return the computed Covariance. */
  MatrixType GetCorrelation() const
  {
    return this->GetFilter()->GetCorrelationOutput()->Get();
  }
  MatrixObjectType* GetCorrelationOutput()
  {
    return this->GetFilter()->GetCorrelationOutput();
  }
  const MatrixObjectType* GetCorrelationOutput() const
  {
    return this->GetFilter()->GetCorrelationOutput();
  }

  /** Return the computed Mean. */
  RealType GetComponentMean() const
  {
    return this->GetFilter()->GetComponentMeanOutput()->Get();
  }
  RealObjectType* GetComponentMeanOutput()
  {
    return this->GetFilter()->GetComponentMeanOutput();
  }
  const RealObjectType* GetComponentMeanOutput() const
  {
    return this->GetFilter()->GetComponentMeanOutput();
  }

  /** Return the computed Covariance. */
  RealType GetComponentCovariance() const
  {
    return this->GetFilter()->GetComponentCovarianceOutput()->Get();
  }
  RealObjectType* GetComponentCovarianceOutput()
  {
    return this->GetFilter()->GetComponentCovarianceOutput();
  }
  const RealObjectType* GetComponentCovarianceOutput() const
  {
    return this->GetFilter()->GetComponentCovarianceOutput();
  }

  /** Return the computed Covariance. */
  RealType GetComponentCorrelation() const
  {
    return this->GetFilter()->GetComponentCorrelationOutput()->Get();
  }
  RealObjectType* GetComponentCorrelationOutput()
  {
    return this->GetFilter()->GetComponentCorrelationOutput();
  }
  const RealObjectType* GetComponentCorrelationOutput() const
  {
    return this->GetFilter()->GetComponentCorrelationOutput();
  }

  /** Set the excluded value of the filter.*/
  void SetExcludedValue(const InternalPixelType val)
  {
	  this->GetFilter()->SetExcludedValue(val);
  }
  /** Get the excluded value of the filter.*/
  InternalPixelType GetExcludedValue() const
  {
	  return this->GetFilter()->GetExcludedValue();
  }

  /** Return the number of valid values. */
  unsigned int GetNbOfValidValues() const
  {
	  return this->GetFilter()->GetNbOfValidValuesOutput()->Get();
  }
  UIObjectType* GetNbOfValidValuesOutput()
  {
	  return this->GetFilter()->GetNbOfValidValues();
  }
  const UIObjectType* GetNbOfValidValueOutput() const
  {
	  return this->GetFilter()->GetNbOfValidValues();
  }

  /** Return the number of excluded values. */
  unsigned int GetNbOfExcludedValues() const
  {
	  return this->GetFilter()->GetNbOfExcludedValuesOutput()->Get();
  }
  UIObjectType* GetNbOfExcludedValuesOutput()
  {
	  return this->GetFilter()->GetNbOfExcludedValues();
  }
  const UIObjectType* GetNbOfExcludedValueOutput() const
  {
	  return this->GetFilter()->GetNbOfExcludedValues();
  }

  bool
  GetIsValid() const
  {
	return this->GetFilter()->GetIsValid();
  }

  otbSetObjectMemberMacro(Filter, EnableMinMax, bool);
  otbGetObjectMemberMacro(Filter, EnableMinMax, bool);

  otbSetObjectMemberMacro(Filter, EnableFirstOrderStats, bool);
  otbGetObjectMemberMacro(Filter, EnableFirstOrderStats, bool);

  otbSetObjectMemberMacro(Filter, EnableSecondOrderStats, bool);
  otbGetObjectMemberMacro(Filter, EnableSecondOrderStats, bool);

  otbSetObjectMemberMacro(Filter, UseUnbiasedEstimator, bool);
  otbGetObjectMemberMacro(Filter, UseUnbiasedEstimator, bool);

protected:
  /** Constructor */
  StreamingConditionalStatisticsVectorImageFilter() {}

  /** Destructor */
  virtual ~StreamingConditionalStatisticsVectorImageFilter() {}

private:
  StreamingConditionalStatisticsVectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsStreamingConditionalStatisticsVectorImageFilter.txx"
#endif

#endif /* __vnsStreamingConditionalStatisticsVectorImageFilter_h */
