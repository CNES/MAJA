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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 2 mai 2016 : Correction warning, qualite, etc             *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 10 nov. 2011 : Modification pour prise en compte                *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsScatteringCorrectionImageFilter_h
#define __vnsScatteringCorrectionImageFilter_h

#include "itkProcessObject.h"

#include "vnsMacro.h"
#include "vnsBandsDefinitions.h"
#include "vnsComputeScatteringCorrectionImageFilter.h"

#include "otbImage.h"
#include "otbObjectList.h"
#include <map>


namespace vns
{
    /** \class  ScatteringCorrectionImageFilter
     * \brief This class computes the surface reflectance using the atmospheric LUT..
     *
     * Inputs are:
     * \begin{itemize}
     *    \item TOAC image (otb::VectorImage)
     *    \item AOT image (otb::Image)
     *    \item EDG image (otb::Image)
     *    \item DTM image (otb::Image)     
     *    \item LUT (vns::VectorLookUpTable)
     *  \end{itemize}
     *
     * Output is TOC (or NSL) image (otb::VectorImage).
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
    class ScatteringCorrectionImageFilter : public itk::ProcessObject
    {
    public:
        /** Standard class typedefs. */
        typedef ScatteringCorrectionImageFilter Self;
        typedef itk::ProcessObject Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)

        /** Creation through object factory macro */
        itkTypeMacro(ScatteringCorrectionImageFilter, ProcessObject)

        /** Some convenient typedefs. */
        /** TOAC input image typedefs. */
        typedef VNSRealVectorImageType TOACImageType;
        typedef TOACImageType::ConstPointer TOACImageConstPointer;
        typedef TOACImageType::PixelType TOACImagePixelType;
        typedef TOACImageType::InternalPixelType TOACImageInternalPixelType;
        typedef otb::ObjectList<TOACImageType> TOACImageListType;
        typedef TOACImageListType::ConstPointer TOACImageListConstPointer;

        /** AOT input image typedefs. */
        typedef VNSRealImageType AOTImageType;
        typedef AOTImageType::ConstPointer AOTImageConstPointer;
        typedef AOTImageType::PixelType AOTImagePixelType;
        typedef otb::ObjectList<AOTImageType> AOTImageListType;
        typedef AOTImageListType::ConstPointer AOTImageListConstPointer;

        /** Altitude input image typedefs. */
        typedef VNSDTMImageType DTMImageType;
        typedef DTMImageType::ConstPointer DTMImageConstPointer;
        typedef DTMImageType::PixelType DTMImagePixelType;
        typedef DTMImageType::InternalPixelType DTMImageInternalPixelType;
        typedef otb::ObjectList<DTMImageType> DTMImageListType;
        typedef DTMImageListType::ConstPointer DTMImageListConstPointer;

        /** Edge input image typedefs. */
        typedef VNSMaskImageType InputMaskType;
        typedef InputMaskType::ConstPointer InputMaskConstPointer;
        typedef InputMaskType::PixelType InputMaskPixelType;
        typedef InputMaskType::InternalPixelType InputMaskInternalPixelType;
        typedef otb::ObjectList<InputMaskType> InputMaskListType;
        typedef InputMaskListType::ConstPointer InputMaskListConstPointer;

        /** Zone/Detector input mask typedefs. */
        typedef VNSMaskVectorImageType VectorMaskType;
        typedef VectorMaskType::ConstPointer VectorMaskConstPointer;
        typedef VectorMaskType::PixelType VectorMaskPixelType;
        typedef VectorMaskType::InternalPixelType VectorMaskInternalPixelType;
        typedef otb::ObjectList<VectorMaskType> VectorMaskListType;
        typedef VectorMaskListType::ConstPointer VectorMaskListConstPointer;

        /** LUT image typedef */
        typedef VNSLUT3DType LUTType;
        typedef LUTType::Pointer LUTPointer;
        typedef LUTType::ConstPointer LUTConstPointer;
        typedef LUTType::PixelType LUTPixelType;
        typedef LUTType::PointType LUTPointType;
        typedef LUTPointType::ValueType LUTPointValueType;
        typedef std::map<unsigned int, LUTPointer> LUTMapType;

        /** Output image typedefs. */
        typedef VNSRealVectorImageType OutputImageType;
        typedef OutputImageType::Pointer OutputImagePointer;
        typedef OutputImageType::ConstPointer OutputImageConstPointer;
        typedef OutputImageType::PixelType OutputImagePixelType;
        typedef OutputImageType::InternalPixelType OutputImageInternalPixelType;
        typedef OutputImageType::RegionType OutputImageRegionType;
        typedef otb::ObjectList<OutputImageType> OutputImageListType;
        typedef OutputImageListType::Pointer OutputImageListPointer;

        /** ScatteringCorrectionImageFilter typedef */
        typedef ComputeScatteringCorrectionImageFilter<TOACImageType, AOTImageType, DTMImageType, InputMaskType, VectorMaskType,
                OutputImageType, LUTType> ComputeScatteringCorrectionImageFilterType;
        typedef ComputeScatteringCorrectionImageFilterType::Pointer ComputeScatteringCorrectionImageFilterPointer;

        typedef otb::ObjectList<ComputeScatteringCorrectionImageFilterType> ComputeScatteringCorrectionImageFilterListType;

        /** Set the TOAC reflectance image */
        vnsSetGetInputMacro(TOACSub , TOACImageType, 0)
        /** Set the AOT reflectance image */
        vnsSetGetInputMacro( AOTSub, AOTImageType, 1)
        /** Set the EDG image */
        vnsSetGetInputMacro( EDGSub, InputMaskType, 2)
        /** Set the DTM image */
        vnsSetGetInputMacro(DTMSub , DTMImageType, 3)
        /** Set the TOAC reflectance image list at L2 resolutions */
        vnsSetGetInputMacro( TOACList, TOACImageListType, 4)
        /** Set the AOT reflectance image list at L2 resolutions */
        vnsSetGetInputMacro( AOTList, AOTImageListType, 5)
        /** Set the EDG image list at L2 resolutions */
        vnsSetGetInputMacro( EDGList, InputMaskListType, 6)
        /** Set the DTM image list at L2 resolutions */
        vnsSetGetInputMacro( DTMList, DTMImageListType, 7)
        /** Set the mask list of the angle zone per band and for each L2 resolution */
        vnsSetGetInputMacro( AngleZoneMaskList, VectorMaskListType, 8)

        /** Set the LUT map that contains one LUT by zone */
        void
        SetLUTMap(const LUTMapType& pLUTMap)
        {
            m_LUTMap = pLUTMap;
        }

        // AOT output at L2_coarse_resolution
        itkGetObjectMacro(TOCSubOutput,OutputImageType)
        // AOT output at L2_resolutions
        itkGetObjectMacro(TOCListOutput, OutputImageListType)
        itkGetConstObjectMacro(TOCListOutput, OutputImageListType)

        /** Write L2 product to L2 resolution. */
        itkSetMacro(WriteL2Resolution, bool)
        itkGetConstReferenceMacro(WriteL2Resolution, bool)

        /** Set Image Information */
        vnsMemberAndDirectSetAndGetConstReferenceMacro(BandsDefinitions, BandsDefinitions)
        vnsMemberAndSetMacro( RealL2NoData, RealNoDataType)

        /** Generate data method */
        virtual void
        UpdateWithCaching();

    protected:
        /** Constructor */
        ScatteringCorrectionImageFilter();
        /** Destructor */
        virtual
        ~ScatteringCorrectionImageFilter();
        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
        void
        operator=(const Self&); //purposely not implemented

        /** Validate the presence of all inputs and their sizes. If at least one input
         * is missing, throws an exception. */
        void
        CheckInputs();

        /** Surface reflectance LUT */
        LUTMapType m_LUTMap;

        /** Write L2 product to L2 resolutions */
        bool m_WriteL2Resolution;

        /** Scattering correction image filter pointer */
        ComputeScatteringCorrectionImageFilterPointer m_ComputeScatteringCorrection;

        ComputeScatteringCorrectionImageFilterListType::Pointer m_ComputeScatteringCorrectionList;

        /** Ouput TOC image  at L2 Coarse resolution */
        OutputImagePointer m_TOCSubOutput;

        /** Ouput TOC image list at L2 resolution*/
        OutputImageListPointer m_TOCListOutput;

    };

} // End namespace vns

#endif /* __vnsScatteringCorrectionImageFilter_h */
