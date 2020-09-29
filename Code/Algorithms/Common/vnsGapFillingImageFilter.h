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
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-1754-CNES : 10 janvier 2017 : Water Vapor Post-Processing             *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsGapFillingImageFilter_h
#define vnsGapFillingImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkNeighborhoodAlgorithm.h"
#include "vnsMacro.h"


namespace vns
{
/** \class  GapFillingImageFilter
 * \brief This class implements .
 *
 * 
 *
 * \author CS Systemes d'Information
 *
 * \sa ImageToImageFilter
 *
 * \ingroup L2
 * \ingroup L3
 * \ingroup Checktool
 *
 */
template <class TInputImage, class TOutputImage, class TOutputMask>
class GapFillingImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
    /** Standard class typedefs. */
    typedef GapFillingImageFilter                               Self;
    typedef itk::ImageToImageFilter<TInputImage,TOutputImage>      Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(GapFillingImageFilter, ImageToImageFilter );

    /** Some convenient typedefs. */
    typedef TInputImage                                     InputImageType;
    typedef typename InputImageType::Pointer                InputImagePointerType;
    typedef typename InputImageType::ConstPointer           InputImageConstPointerType;
    typedef typename InputImageType::RegionType             InputImageRegionType;
    typedef typename InputImageType::PixelType              InputImagePixelType;
    typedef typename InputImageType::InternalPixelType      InternalPixelType;
    typedef typename InputImageType::IndexType              IndexType;
    typedef typename InputImageType::SizeType               SizeType;
    typedef typename InputImageType::OffsetType             OffsetType;
    typedef typename InputImageType::InternalPixelType      NoDataType;

    typedef TOutputImage                                    OutputImageType;
    typedef typename OutputImageType::Pointer               OutputImagePointerType;
    typedef typename OutputImageType::ConstPointer          OutputImageConstPointerType;
    typedef typename OutputImageType::RegionType            OutputImageRegionType;
    typedef typename OutputImageType::InternalPixelType     OutputImagePixelType;
    typedef typename OutputImageType::IndexType             OutputImageIndexType;
    typedef typename OutputImageType::SizeType              OutputImageSizeType;

    typedef TOutputMask                                     OutputMaskType;
    typedef typename OutputMaskType::Pointer                OutputMaskPointerType;
    typedef typename OutputMaskType::ConstPointer           OutputMaskConstPointer;
    typedef typename OutputMaskType::PixelType              OutputMaskPixelType;

    typedef vnl_matrix<double>                              MatrixType;
    typedef std::vector<MatrixType>                         MatrixVectType;

    typedef std::vector<unsigned int>                       ListOfIndexes;
    typedef std::vector<ListOfIndexes>                      TableOfListOfIndex;

    /** Image Region Iterators */
    typedef typename itk::ImageRegionIterator<OutputImageType> OutputImageRegionIterator;
    typedef typename itk::ImageRegionIterator<OutputMaskType>  OutputMaskRegionIterator;

    /** Neighborhood Iterator */
    typedef itk::ConstNeighborhoodIterator<InputImageType>  ImageNeighborhoodIteratorType;

    /** Boundaries */
    typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>
                                                            ImageBoundaryFacesType;

    typedef typename itk::ConstantBoundaryCondition<InputImageType>
                                                            ConstantBCType;

    typedef typename ImageNeighborhoodIteratorType::RadiusType
                                                            RadiusType;


    typedef itk::ImageBase<2>                               ImageBaseType;

    /** No_data pixel value accessors for input image */
    itkSetMacro(NoData, NoDataType);
    itkGetConstReferenceMacro(NoData, NoDataType);

    /** Default value */
    itkSetMacro(DefaultValue, double);
    itkGetConstReferenceMacro(DefaultValue, double);

    /** Initial window radius */
    itkSetMacro(InitWindowRadius,unsigned int);
    itkGetConstMacro(InitWindowRadius,unsigned int);

    /** Maximum window radius */
    itkSetMacro(MaxWindowRadius,unsigned int);
    itkGetConstMacro(MaxWindowRadius,unsigned int);

    /** Has valid pixel */
    itkSetMacro(HasValidPixel,bool);
    itkGetConstMacro(HasValidPixel,bool);

    /** Vector of Matrixes */
    vnsSetMacro(Weights,MatrixVectType);
    vnsGetConstMacro(Weights,MatrixVectType);

    /** Mean value */
    itkSetMacro(MeanValue, double);
    itkGetConstMacro(MeanValue, double);

    /** Setters : Overload SetInput method. */
    /** Getters : Overload GetInput method. */
    vnsSetGetInputRawMacro( InputImage, InputImageType, 0)

    OutputImageType * GetOutputFilledImage(void);
    OutputMaskType * GetOutputMask(void);


protected:
    /** Constructor */
    GapFillingImageFilter();
    /** Destructor */
    virtual ~GapFillingImageFilter(){};
    /** Use a marging in the requested regions to be able to compute in a
     * changing currentRegion cf. ThreadedGenerateData(). */
    void GenerateInputRequestedRegion();
    /** Validate the presence of all ten inputs. If one or more inputs
     * are missing, throw an exception. */
    void BeforeThreadedGenerateData();
    /** Multi-thread version GenerateData. */
    void ThreadedGenerateData (const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
    GapFillingImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /** Add margings to the requested regions */
    void PadByRadiusRequestedRegion( ImageBaseType * pImagePtr);

    /** Fill the weights vector with the computed matrixes */
    void InitWeights();

    void PrecomputeIndexes();

    /** Compute a weight matrix */
    MatrixType ComputeWeight(unsigned int pSize);

    NoDataType m_NoData;

    double m_DefaultValue;

    double m_MeanValue;

    /** Radius is such as window_width = 1 + 2*radius */
    unsigned int m_InitWindowRadius;

    /** Radius is such as window_width = 1 + 2*radius */
    unsigned int m_MaxWindowRadius;

    bool m_HasValidPixel;

    MatrixVectType m_Weights;

    TableOfListOfIndex m_Indexes;

};

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsGapFillingImageFilter.txx"
#endif

#endif /* vnsGapFillingImageFilter_h */
