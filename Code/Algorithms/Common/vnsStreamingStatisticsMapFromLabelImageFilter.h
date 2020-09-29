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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 14 octobre 2016 : Audit code - Correction qualite :       *
 *                                                               Regle : Missing parameter name in function *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsStreamingStatisticsMapFromLabelImageFilter_h
#define __vnsStreamingStatisticsMapFromLabelImageFilter_h

#include "otbPersistentImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include <map>

namespace vns
{

/** \class PersistentStreamingStatisticsMapFromLabelImageFilter
 * \brief Computes population for each label of a label image
 *
 * This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the statitics of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the populations once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 */

template<class TLabelImage>
class PersistentStreamingStatisticsMapFromLabelImageFilter :
public otb::PersistentImageFilter<TLabelImage, TLabelImage>
{
public:
    /** Standard Self typedef */
    typedef PersistentStreamingStatisticsMapFromLabelImageFilter Self;
    typedef otb::PersistentImageFilter<TLabelImage, TLabelImage> Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self)

    /** Runtime information support. */
    itkTypeMacro(PersistentStreamingStatisticsMapFromLabelImageFilter, PersistentImageFilter);

    /** Image related typedefs. */
    typedef TLabelImage LabelImageType;
    typedef typename TLabelImage::Pointer LabelImagePointer;

    typedef typename LabelImageType::PixelType LabelPixelType;
    typedef std::map<LabelPixelType, unsigned long> LabelPopulationMapType;

    typedef itk::SimpleDataObjectDecorator<LabelPopulationMapType> LabelPopulationMapObjectType;

    itkStaticConstMacro(InputImageDimension, unsigned int,
            TLabelImage::ImageDimension);

    /** Image related typedefs. */
    itkStaticConstMacro(ImageDimension, unsigned int,
            TLabelImage::ImageDimension);

    /** Smart Pointer type to a DataObject. */
    typedef typename itk::DataObject::Pointer DataObjectPointer;

    typedef itk::ImageBase<InputImageDimension> ImageBaseType;
    typedef typename ImageBaseType::RegionType InputImageRegionType;

    /** Set input label image */
    virtual void SetInput( const LabelImageType *input);

    /** Get input label image */
    virtual const LabelImageType * GetInput();

    /** Return the computed number of labeled pixels for each label in the input label image */
    LabelPopulationMapType GetLabelPopulationMap() const;

    /** Make a DataObject of the correct type to be used as the specified
     * output. */
    virtual DataObjectPointer MakeOutput(unsigned int idx);

    /** Pass the input through unmodified. Do this by Grafting in the
     *  AllocateOutputs method.
     */
    void AllocateOutputs();

    virtual void GenerateOutputInformation();

    void Synthetize(void);

    void Reset(void);

    /** Due to heterogeneous input template GenerateInputRequestedRegion must be reimplemented using explicit cast **/
    /** This new implementation is inspired by the one of itk::ImageToImageFilter **/
    void GenerateInputRequestedRegion();

protected:
    PersistentStreamingStatisticsMapFromLabelImageFilter();
    virtual ~PersistentStreamingStatisticsMapFromLabelImageFilter()
    {}
    void PrintSelf(std::ostream& os, itk::Indent indent) const;

    /** GenerateData. */
    void GenerateData();

private:
    PersistentStreamingStatisticsMapFromLabelImageFilter(const Self &); //purposely not implemented
    void operator =(const Self&);//purposely not implemented

    LabelPopulationMapType m_LabelPopulation;
}; // end of class PersistentStreamingStatisticsMapFromLabelImageFilter

/*===========================================================================*/

/** \class StreamingStatisticsMapFromLabelImageFilter
 * \brief Computes mean radiometric value for each label of a label image, based on a support VectorImage
 *
 * Currently the class only computes the mean value.
 *
 * This class streams the whole input image through the PersistentStreamingStatisticsMapFromLabelImageFilter.
 *
 * This way, it allows to compute the first order global statistics of this image.
 * It calls the Reset() method of the PersistentStatisticsImageFilter before streaming
 * the image and the Synthetize() method of the PersistentStatisticsImageFilter
 * after having streamed the image to compute the statistics.
 * The accessor on the results are wrapping the accessors of the
 * internal PersistentStatisticsImageFilter.
 *
 */

template<class TLabelImage>
class StreamingStatisticsMapFromLabelImageFilter :
public otb::PersistentFilterStreamingDecorator<PersistentStreamingStatisticsMapFromLabelImageFilter<TLabelImage> >
{
public:
    /** Standard Self typedef */
    typedef StreamingStatisticsMapFromLabelImageFilter Self;
    typedef otb::PersistentFilterStreamingDecorator<PersistentStreamingStatisticsMapFromLabelImageFilter<TLabelImage> > Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Type macro */
    itkNewMacro(Self)

    /** Creation through object factory macro */
    itkTypeMacro(StreamingStatisticsMapFromLabelImageFilter, PersistentFilterStreamingDecorator);

    typedef TLabelImage LabelImageType;

    typedef typename Superclass::FilterType::LabelPopulationMapType LabelPopulationMapType;

    /** Set input multispectral image */
    void SetInput(const LabelImageType * input)
    {
        this->GetFilter()->SetInput(input);
    }

    /** Get input multispectral image */
    const LabelImageType * GetInput()
    {
        return this->GetFilter()->GetInput();
    }

    /** Return the computed number of labeled pixels for each label */
    LabelPopulationMapType GetLabelPopulationMap() const
    {
        return this->GetFilter()->GetLabelPopulationMap();
    }

protected:
    /** Constructor */
    StreamingStatisticsMapFromLabelImageFilter()
    {}
    /** Destructor */
    virtual ~StreamingStatisticsMapFromLabelImageFilter()
    {}

private:
    StreamingStatisticsMapFromLabelImageFilter(const Self &); //purposely not implemented
    void operator =(const Self&);//purposely not implemented
};

}
 // end namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsStreamingStatisticsMapFromLabelImageFilter.txx"
#endif

#endif
