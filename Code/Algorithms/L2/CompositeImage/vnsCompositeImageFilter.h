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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-DM-MAC-1757-CNES : 19 juillet 2016 : amélioration du calcul des statistiques *
 *                                                                  de l’AOT                                *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-116796-CS : 20 fevrier 2013 : Valeur du No_Data dans le plan PXD 		*
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 19 décembre 2013 : Améliorations algorithmiques			*
 * 								Suppression de la recherche des zones de forte AOT près des nuages			*
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 3 novembre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 10 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCompositeImageFilter_h
#define __vnsCompositeImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkConnectedComponentImageFilter.h"

#include "itkBinaryFunctorImageFilter.h"
#include "itkOrImageFilter.h"

#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "vnsOrVectorToScalarFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

#include "vnsBinaryLogicalOrFunctor.h"
#include "vnsStreamingConditionalStatisticsImageFilter.h"

#include "vnsHighAOTImageFilter.h"
#include "vnsCompositeImageUpdateImageFilter.h"
#include "otbImageFileReader.h"
#include "otbMultiChannelExtractROI.h"
#include "vnsMultiChannelsExtractImageFilter.h"

#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "vnsMacro.h"
#include "vnsUtilities.h"

namespace vns
{
    /** \class  CompositeImageFilter
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
    class CompositeImageFilter : public itk::ProcessObject
    {
    public:
        /** Standard class typedefs. */
        typedef CompositeImageFilter Self;
        typedef itk::ProcessObject Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)
        /** Creation through object factory macro */
        itkTypeMacro(CompositeImageFilter, itk::ProcessObject )

        /** Some convenient typedefs. */
        typedef VNSRealVectorImageType InputImageType;
        typedef InputImageType::ConstPointer InputImageConstPointer;
        typedef InputImageType::RegionType RegionType;
        typedef InputImageType::PixelType InputImagePixelType;
        typedef InputImageType::SizeType SizeType;
        typedef InputImageType::InternalPixelType InternalPixelType;

        typedef VNSMaskImageType InputMaskType;
        typedef InputMaskType::Pointer InputMaskPointer;
        typedef InputMaskType::ConstPointer InputMaskConstPointer;
        typedef InputMaskType::PixelType InputMaskPixelType;
        typedef InputMaskType::InternalPixelType InputMaskInternalPixelType;
        typedef otb::ImageList<InputMaskType> InputMaskListType;
        typedef InputMaskListType::Pointer InputMaskListPointerType;

        typedef VNSIntegerImageType DateImageType;
        typedef DateImageType::Pointer DateImagePointer;
        typedef DateImageType::ConstPointer DateImageConstPointer;
        typedef DateImageType::PixelType DateImagePixelType;
        typedef DateImageType::InternalPixelType DateInternalPixelType;

        typedef VNSMaskVectorImageType VectorMaskType;
        typedef VectorMaskType::Pointer VectorMaskPointer;
        typedef VectorMaskType::ConstPointer VectorMaskConstPointer;
        typedef VectorMaskType::PixelType VectorMaskPixelType;
        typedef VectorMaskType::InternalPixelType VectorMaskInternalPixelType;

        typedef VNSRealImageType AOTImageType;
        typedef AOTImageType::Pointer AOTImagePointer;
        typedef AOTImageType::ConstPointer AOTImageConstPointer;
        typedef AOTImageType::PixelType AOTImagePixelType;

        typedef VNSRealVectorImageType OutputImageType;
        typedef OutputImageType::Pointer OutputImagePointer;
        typedef OutputImageType::PixelType OutputImagePixelType;

        typedef Utilities::ListOfUIntegers ListOfUIntegers;

        // Internal image types
        typedef otb::VectorImage<AOTImagePixelType, 2> TemporaryImageType;
        typedef TemporaryImageType::PixelType TemporaryPixelType;
        typedef TemporaryImageType::Pointer TemporaryImagePointer;

        typedef unsigned short LabelPixelType;
        typedef otb::Image<LabelPixelType, AOTImageType::ImageDimension> InternalLabelType;
        typedef InternalLabelType::Pointer InternalLabelPointerType;
        typedef InternalLabelType::PixelType InternalLabelPixelType;
        typedef otb::ImageFileReader<InternalLabelType> InternalLabelReaderType;
        typedef InternalLabelReaderType::Pointer InternalLabelReaderPointerType;

        // Internal filters definition

        //Or Filter to concatenate edg mask and snow mask
        typedef itk::OrImageFilter<InputMaskType, InputMaskType, InputMaskType> OrFilterType;
        typedef OrFilterType::Pointer OrFilterPointer;

        // Compute AOT statistics tacking into account masks
        typedef Functor::BinaryLogicalOrFunctor<InputMaskPixelType, InputMaskPixelType, InputMaskPixelType> BinaryLogicalOrFunctorType;
        typedef itk::BinaryFunctorImageFilter<InputMaskType, InputMaskType, InputMaskType, BinaryLogicalOrFunctorType> BinaryLogicalOrFilterType;
        typedef BinaryLogicalOrFilterType::Pointer BinaryLogicalOrPointerFilterType;

        typedef otb::ImageListToVectorImageFilter<InputMaskListType, VectorMaskType> MaskListToVectorMaskFilterType;
        typedef MaskListToVectorMaskFilterType::Pointer MaskListToVectorMaskFilterPointerType;

        typedef otb::ConcatenateVectorImageFilter<VectorMaskType,VectorMaskType,VectorMaskType> ConcatenateVectorImageFilterType;
        typedef ConcatenateVectorImageFilterType::Pointer ConcatenateVectorImageFilterPointerType;

        typedef Functor::OrVectorToScalarFunctor<VectorMaskType::PixelType, InputMaskType::PixelType>   VectorMaskToMaskFunctorType;
        typedef itk::UnaryFunctorImageFilter<VectorMaskType,InputMaskType, VectorMaskToMaskFunctorType> VectorMaskToMaskFilterType;
        typedef VectorMaskToMaskFilterType::Pointer VectorMaskToMaskFilterPointerType;

        typedef StreamingConditionalStatisticsImageFilter<AOTImageType, InputMaskType> StatisticsFilterType;

        typedef StatisticsFilterType::Pointer StatisticsFilterPointerType;

        // High AOT filter type
        typedef HighAOTImageFilter<AOTImageType, InputMaskType, InputMaskType> HighAOTImageFilterType;
        typedef HighAOTImageFilterType::Pointer HighAOTImagePointerFilterType;

        // Labelize filter type
        typedef itk::ConnectedComponentImageFilter<InputMaskType, InternalLabelType> LabelizeFilterType;
        typedef LabelizeFilterType::Pointer LabelizePointerFilterType;

        // Composite image update filter type
        typedef CompositeImageUpdateImageFilter<InputImageType, InputMaskType, VectorMaskType, DateImageType, OutputImageType> CompositeUpdateFilterType;
        typedef CompositeUpdateFilterType::Pointer CompositeUpdatePointerFilterType;

        typedef otb::ImageFileReader<OutputImageType> ReaderType;
        typedef ReaderType::Pointer ReaderPointer;
        typedef otb::ImageFileReader<InputMaskType> ReaderMaskType;
        typedef ReaderMaskType::Pointer ReaderMaskPointer;

        typedef otb::MultiChannelExtractROI<InternalPixelType, InternalPixelType> MultiChannelExtractROIType;
//            typedef MultiChannelsExtractImageFilter<InputImageType,InputImageType> MultiChannelExtractROIType;
        typedef MultiChannelExtractROIType::Pointer MultiChannelExtractROIPointer;

        /** Input pointer accessors. */
        /** Set the image of the TOA reflectance corrected by absorption */
        itkSetObjectMacro(IPTOACInput, InputImageType)
        /** Get the image of the TOA reflectance corrected by absorption */
        itkGetConstObjectMacro(IPTOACInput, InputImageType)

        /** Set the image of the TOA reflectance corrected by absorption
         * of the composite product */
        itkSetObjectMacro(L2inRTAInput, InputImageType)
        /** Get the image of the TOA reflectance corrected by absorption
         * of the composite product */
        itkGetConstObjectMacro(L2inRTAInput, InputImageType)

        /** Set the image of the surface reflectance corrected by absorption and rayleigh */
        itkSetObjectMacro(IPTOCRInput, InputImageType)
        /** Get the image of the surface reflectance corrected by absorption and rayleigh */
        itkGetConstObjectMacro(IPTOCRInput, InputImageType)

        /** Set the image of the surface reflectance corrected by absorption and rayleigh
         * of the composite product */
        itkSetObjectMacro(L2inRCRInput, InputImageType)
        /** Get the image of the surface reflectance corrected by absorption and rayleigh
         * of the composite product */
        itkGetConstObjectMacro(L2inRCRInput, InputImageType)

        /** Set the image of the surface reflectance */
        itkSetObjectMacro(IPTOCInput, InputImageType)
        /** Get the image of the surface reflectance */
        itkGetConstObjectMacro(IPTOCInput, InputImageType)

        /** Set the image of the surface reflectance of the composite product */
        itkSetObjectMacro(L2inRTCInput, InputImageType)
        /** Get the image of the surface reflectance of the composite product */
        itkGetConstObjectMacro(L2inRTCInput, InputImageType)

        /** Set the stack of previous surface reflectance images corrected by absorption and rayleigh
         * of the composite product */
        itkSetObjectMacro(L2inSTOInput, InputImageType)
        /** Get the stack of previous surface reflectance images corrected by absorption and rayleigh
         * of the composite product */
        itkGetConstObjectMacro(L2inSTOInput, InputImageType)

        /** Set the image of date of the composite product */
        itkSetObjectMacro(L2inPXDInput, DateImageType)
        /** Get the image of date of the composite product */
        itkGetConstObjectMacro(L2inPXDInput, DateImageType)

        /** Set the edge mask of the input product */
        itkSetObjectMacro(IPEDGInput, InputMaskType)
        /** Get the the edge mask of the input product */
        itkGetConstObjectMacro(IPEDGInput, InputMaskType)

        /** Set the no_data mask of the composite product */
        itkSetObjectMacro(L2inNDTInput, InputMaskType)
        /** Get the the no_data mask of the composite product */
        itkGetConstObjectMacro(L2inNDTInput, InputMaskType)

        /** Set the summary cloud mask */
        itkSetObjectMacro(IPCLDSumInput, InputMaskType)
        /** Get the summary cloud mask */
        itkGetConstObjectMacro(IPCLDSumInput, InputMaskType)

        /** Set the saturated image of the input product for all bands */
        itkSetObjectMacro(IPSATSubInput, VectorMaskType)
        /** Get the saturated image of the input product for all bands */
        itkGetConstObjectMacro(IPSATSubInput, VectorMaskType)

        /** Set the AOT sub image */
        itkSetObjectMacro(IPAOTSubInput, AOTImageType)
        /** Get the AOT sub image */
        itkGetConstObjectMacro(IPAOTSubInput, AOTImageType)

        /** Set the snow mask sub */
        itkSetObjectMacro(IPSNWSubInput, InputMaskType)
        /** Get the snow mask sub */
        itkGetConstObjectMacro(IPSNWSubInput, InputMaskType)

        /* Outputs */
        /** Get the image of the TOA reflectance corrected by absorption
         * of the composite product */
        itkGetConstObjectMacro(L2RTAOutput, OutputImageType)
        itkGetObjectMacro(L2RTAOutput, OutputImageType)

        /** Get the image of the surface reflectance corrected by absorption and rayleigh */
        itkGetConstObjectMacro(L2RCROutput, OutputImageType)
        itkGetObjectMacro(L2RCROutput, OutputImageType)

        /** Get the image of the surface reflectance of the composite product */
        itkGetConstObjectMacro(L2RTCOutput, OutputImageType)
        itkGetObjectMacro(L2RTCOutput, OutputImageType)

        /** Get the stack of previous surface reflectance images corrected by absorption and rayleigh
         * of the composite product */
        itkGetConstObjectMacro(L2STOOutput, OutputImageType)
        itkGetObjectMacro(L2STOOutput, OutputImageType)

        /** Get the image of date of the composite product */
        itkGetConstObjectMacro(L2PXDOutput, DateImageType)
        itkGetObjectMacro(L2PXDOutput, DateImageType)

        /** Get the no_data mask of the composite product */
        itkGetConstObjectMacro(L2NDTOutput, InputMaskType)
        itkGetObjectMacro(L2NDTOutput, InputMaskType)

        // Parameters accessors
        // Init mode accessors
        itkSetMacro(InitMode, bool)
        itkGetConstReferenceMacro(InitMode, bool)

        // Real No_data pixel value accessors
        itkSetMacro(RealL2NoData, RealNoDataType)
        itkGetConstReferenceMacro(RealL2NoData, RealNoDataType)

        // No_data pixel value accessors
        itkSetMacro(L2NoData, NoDataType)
        itkGetConstReferenceMacro(L2NoData, NoDataType)

        // Coef sigma AOT accessors
        itkSetMacro(CoefSigmaAOT, unsigned short)
        itkGetConstReferenceMacro(CoefSigmaAOT, unsigned short)

        // Maximum high AOT std accessors
        itkSetMacro(MinimumHighAOTStd, double)
        itkGetConstReferenceMacro(MinimumHighAOTStd, double)

        // Maximum AOT accessors
        itkSetMacro(MaxAOT, double)
        itkGetConstReferenceMacro(MaxAOT, double)

		// Default AOT accessors
		itkSetMacro(DefaultAOT, double)
		itkGetConstReferenceMacro(DefaultAOT, double)

        // Maximum Age accessors
        itkSetMacro(MaxAge, unsigned short)
        itkGetConstReferenceMacro(MaxAge, unsigned short)

        // Hotspot flag accessors
        itkSetMacro(HotspotFlag, bool)
        itkGetConstReferenceMacro(HotspotFlag, bool)

        // Cirrus flag accessors
        itkSetMacro(CirrusFlag, bool)
        itkGetConstReferenceMacro(CirrusFlag, bool)

        // Rain flag accessors
        itkSetMacro(RainFlag, bool)
        itkGetConstReferenceMacro(RainFlag, bool)

        // Acquisition date in julian days
        itkSetMacro(JDay, short)
        itkGetConstReferenceMacro(JDay, short)

        // STO band number accessors
        itkSetMacro(STOBandNumber, unsigned int)
        itkGetConstReferenceMacro(STOBandNumber, unsigned int)

        // STO band index accessors
        itkSetMacro(STOBandIndex, unsigned int)
        itkGetConstReferenceMacro(STOBandIndex, unsigned int)

        // Dilate radius accessors
        itkSetMacro(SEAOTRadius, unsigned long)
        itkGetConstReferenceMacro(SEAOTRadius, unsigned long)

        // List of band index of the composite
        vnsSetMacro(ListOfBandIndexForComposite, ListOfUIntegers)

        void
        UpdateWithCaching(void);

    protected:
        /** Constructor */
        CompositeImageFilter();
        /** Destructor */
        virtual
        ~CompositeImageFilter();
        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
        void
        operator=(const Self&); //purposely not implemented

        void
        CheckInputs();

        /** Init mode parameter */
        bool m_InitMode;
        /** Hotspot flag parameter */
        bool m_HotspotFlag;
        /** Cirrus flag parameter */
        bool m_CirrusFlag;
        /** Rain flag parameter */
        bool m_RainFlag;

        /** Real L2 No_Data parameter */
        RealNoDataType m_RealL2NoData;

        /** L2 No_Data parameter */
        NoDataType m_L2NoData;

        // HighAOTImageFilter parameters
        /** Coefficient for the threshold to detect high AOT */
        unsigned short m_CoefSigmaAOT;
        /** Std max under which the std of the AOT image is not taking into
         * account in the calculation of the high AOT threshold
         */
        double m_MinimumHighAOTStd;
        /** Deault AOT Value */
        double m_DefaultAOT;

        // CompositeImageUpdateImageFilter parameters
        /** AOT Threshold parameter */
        double m_MaxAOT;
        /** Maximum age for reflectancein the composite image */
        unsigned short m_MaxAge;



        /** Acquisition date in julian days */
        short m_JDay;

        /** Number of bands of the STO images */
        unsigned int m_STOBandNumber;

        /** RCR band index set in the STO image */
        unsigned int m_STOBandIndex;

        /** SE AOT radius parameter */
        unsigned long m_SEAOTRadius;

        /** Filters declaration */
        InputMaskListPointerType m_MaskList;
        MaskListToVectorMaskFilterPointerType m_MaskListToVectorMaskFilter;
        ConcatenateVectorImageFilterPointerType m_ConcatenateVectorImageFilter;
        VectorMaskToMaskFilterPointerType m_VectorMaskToMaskFilter;
        StatisticsFilterPointerType m_StatisticsFilter;
        HighAOTImagePointerFilterType m_HighAOTFilter;
        LabelizePointerFilterType m_LabelizeFilter;
        CompositeUpdatePointerFilterType m_CompositeUpdateFilter;
        OrFilterPointer m_OrFilter;

        MultiChannelExtractROIPointer m_IPTOACExtractChannelsFilter;
        MultiChannelExtractROIPointer m_IPTOCRExtractChannelsFilter;
        MultiChannelExtractROIPointer m_IPTOCExtractChannelsFilter;

        /** Inputs */
        InputImageConstPointer m_IPTOACInput;
        InputImageConstPointer m_L2inRTAInput;
        InputImageConstPointer m_IPTOCRInput;
        InputImageConstPointer m_L2inRCRInput;
        InputImageConstPointer m_IPTOCInput;
        InputImageConstPointer m_L2inRTCInput;
        InputImageConstPointer m_L2inSTOInput;
        DateImageConstPointer  m_L2inPXDInput;
        InputMaskPointer  m_IPEDGInput;
        InputMaskConstPointer  m_L2inNDTInput;
        InputMaskPointer  m_IPCLDSumInput;
        VectorMaskPointer m_IPSATSubInput;
        AOTImageConstPointer   m_IPAOTSubInput;
        InputMaskPointer  m_IPSNWSubInput;

        /* Outputs */
        OutputImagePointer m_L2RTAOutput;
        OutputImagePointer m_L2RCROutput;
        OutputImagePointer m_L2RTCOutput;
        OutputImagePointer m_L2STOOutput;
        DateImagePointer m_L2PXDOutput;
        InputMaskPointer m_L2NDTOutput;

        InternalLabelPointerType m_LabelizeFilterOutput;
        InternalLabelReaderPointerType m_LabelizeReader;

        ListOfUIntegers m_ListOfBandIndexForComposite;

    };

} // End namespace vns

#endif /* __vnsCompositeImageFilter_h */
