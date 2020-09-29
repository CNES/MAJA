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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 21 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 16 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDTMProcessing_h
#define __vnsDTMProcessing_h

#include "itkProcessObject.h"

#include "itkTernaryFunctorImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"

#include "vnsSolarDirectionForCloudAltitudeFunctor.h"
#include "vnsDTMProcessingImageFilter.h"
#include "vnsConstantImageSource.h"

#include "vnsMacro.h"

#include "vnsDEMDataManager.h"

#include "otbImage.h"

namespace vns
{
    /** \class  DTMProcessing
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
    class DTMProcessing : public itk::ProcessObject
    {
    public:
        /** Standard class typedefs. */
        typedef DTMProcessing Self;
        typedef itk::ProcessObject Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)

        /** Creation through object factory macro */
        itkTypeMacro(DTMProcessing, ProcessObject )

        /** Some convenient typedefs. */
        typedef VNSRealVectorImageType GRIDImageType;
        typedef GRIDImageType::ConstPointer GRIDImageConstPointer;
        typedef GRIDImageType::Pointer GRIDImagePointer;
        typedef GRIDImageType::PixelType GRIDImagePixelType;
        typedef GRIDImageType::RegionType RegionType;
        typedef GRIDImageType::SizeType SizeType;
        typedef GRIDImageType::IndexType IndexType;
        typedef GRIDImageType::PointType PointType;
        typedef GRIDImageType::SpacingType SpacingType;

        typedef VNSDTMImageType DTMImageType;
        typedef DTMImageType::Pointer DTMImagePointer;
        typedef DTMImageType::ConstPointer DTMImageConstPointer;
        typedef DTMImageType::PixelType DTMImagePixelType;

        typedef VNSMaskImageType InputMaskType;
        typedef InputMaskType::Pointer InputMaskPointer;
        typedef InputMaskType::ConstPointer InputMaskConstPointer;
        typedef InputMaskType::PixelType InputMaskPixelType;
        typedef InputMaskType::InternalPixelType InputMaskInternalPixelType;

        typedef VNSMaskImageType OutputImageType;
        typedef OutputImageType::Pointer OutputImagePointer;
        typedef OutputImageType::PixelType OutputImagePixelType;
        typedef OutputImageType::RegionType OutputImageRegionType;

        typedef DEMDataManager<DTMImageType> Intersector3DType;

        // Altitude image type
        typedef double AltPixelType;
        typedef otb::Image<AltPixelType, GRIDImageType::ImageDimension> AltImageType;
        typedef AltImageType::PixelType AltImagePixelType;

        /** Filters typedefs */

        typedef ConstantImageSource<AltImageType> AltConstantFilterType;
        typedef AltConstantFilterType::Pointer AltConstantFilterPointerType;

        /*typedef Functor::SolarDirectionForCloudAltitudeFunctor<GRIDImagePixelType, GRIDImagePixelType, DTMImagePixelType,
         GRIDImagePixelType> FunctorType;
         typedef itk::TernaryFunctorImageFilter<GRIDImageType, GRIDImageType, DTMImageType, GRIDImageType, FunctorType> SolarFilterType;*/
        typedef Functor::SolarDirectionForCloudAltitudeFunctor<GRIDImagePixelType, GRIDImagePixelType, AltImagePixelType, GRIDImagePixelType> FunctorType;
        typedef itk::TernaryFunctorImageFilter<GRIDImageType, GRIDImageType, AltImageType, GRIDImageType, FunctorType> SolarFilterType;
        typedef SolarFilterType::Pointer SolarFilterPointer;

        typedef DTMProcessingImageFilter<GRIDImageType, DTMImageType, InputMaskType, OutputImageType> DTMProcessingFilterType;
        typedef DTMProcessingFilterType::Pointer DTMProcessingPointer;

        // L2CoarseResolution accessors
        itkSetMacro(L2CoarseResolution, unsigned int)
        itkGetConstReferenceMacro(L2CoarseResolution, unsigned int)

        // B5 reference altitude for viewing direction accessors
        itkSetMacro(VIEHRef, double)
        itkGetConstReferenceMacro(VIEHRef, double)

        // Reference altitude for the first solar grid accessors
        itkSetMacro(SOLH1, double)
        itkGetConstReferenceMacro(SOLH1, double)

        // Reference altitude for the second solar grid accessors
        itkSetMacro(SOLH2, double)
        itkGetConstReferenceMacro(SOLH2, double)

        // Reference altitude for solar direction accessors
        itkSetMacro(SOLHRef, double)
        itkGetConstReferenceMacro(SOLHRef, double)

        // Distance beyond which the pixel is declared hidden
        itkSetMacro(DistanceThreshold, double)
        itkGetConstReferenceMacro(DistanceThreshold, double)

        /** Set the solar direction grids at two altitudes SOLH1 and SOLH2 */
        vnsCxxMemberSetGetObjectMacro(IPSOL1Input, GRIDImageType)
        vnsCxxMemberSetGetObjectMacro(IPSOL2Input, GRIDImageType)

        /** Set the viewing direction grid */
        vnsCxxMemberSetGetObjectMacro(IPVIEInput, GRIDImageType)

        /** Set the DTM image */
        vnsCxxMemberSetGetObjectMacro(DTMInput, DTMImageType)

        /** Set the edge mask image */
        vnsCxxMemberSetGetObjectMacro(IPEDGInput, InputMaskType)

        /** Get the cast shadow mask */
        vnsCxxMemberGetObjectMacro(IPSHDOutput, OutputImageType)

        /** Get the hidden faces mask */
        vnsCxxMemberGetObjectMacro(IPHIDOutput, OutputImageType)

        virtual void
        UpdateWithCaching();

    protected:
        /** Constructor */
        DTMProcessing();
        /** Destructor */
        virtual
        ~DTMProcessing();

        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
        DTMProcessing(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented

        /** Parameters declaration */

        /** L2 coarse resolution declaration */
        unsigned int m_L2CoarseResolution;

        /** Reference altitude for viewing direction declaration */
        double m_VIEHRef;

        /** Altitude for the first solar grid declaration */
        double m_SOLH1;

        /** Altitude for the second solar grid declaration */
        double m_SOLH2;

        /** Reference altitude for solar direction declaration */
        double m_SOLHRef;

        /** Distance beyond which the pixel is declared hidden */
        double m_DistanceThreshold;

        // Intersector 3D
        Intersector3DType m_I3D;

        /** Filters declaration */
        AltConstantFilterPointerType m_SOLHRefConstantFilter;
        DTMProcessingPointer m_DTMProcessingFilter;
        SolarFilterPointer m_SolarFilter;

    };

} // End namespace vns

#endif /* __vnsDTMProcessing_h */
