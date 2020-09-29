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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 13 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 1-0-0-3 : DM : 357 : 20 juil. 2011 : Define vectors and use their norm and scalar product        *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 27 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDTMProcessingImageFilter_h
#define __vnsDTMProcessingImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

#include "vnsDEMDataManager.h"

#include "vnsMacro.h"

namespace vns
{
    /** \class  DTMProcessingImageFilter
     * \brief This class computes two masks that are used for water and aerosol detection:
     * \begin{itemize}
     *      \item Cast Shadows,
     *      \item Hidden Faces.
     * \end{itemize}
     *
     * For a given pixel, knowing the sun or viewing direction, this filter finds the
     * first intersection of viewing direction with earth surface (DTM). If this point
     * is different from the given pixel, the pixel is hidden.
     *
     * The input images are the viewing and solar grids which are otb::VectorImage.
     * For each viewing spectral band, the image contains one band for X offsets and the next
     * one for Y offsets.
     * The DTM an otb::Image.
     * The two output masks are otb::Image.
     *
     * This class is based on the intersector module of Euclide (utilitiesI3D).
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TGRIDImage, class TDTMImage, class TInputMask, class TOutputImage>
        class DTMProcessingImageFilter : public itk::ImageToImageFilter<TGRIDImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef DTMProcessingImageFilter Self;
            typedef itk::ImageToImageFilter<TGRIDImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(DTMProcessingImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType GRIDImageType;
            typedef typename GRIDImageType::ConstPointer GRIDImageConstPointer;
            typedef typename GRIDImageType::PixelType GRIDImagePixelType;
            typedef typename GRIDImageType::RegionType RegionType;
            typedef typename GRIDImageType::SizeType SizeType;
            typedef typename GRIDImageType::IndexType IndexType;
            typedef typename GRIDImageType::PointType PointType;
            typedef typename GRIDImageType::SpacingType SpacingType;

            typedef TDTMImage DTMImageType;
            typedef typename DTMImageType::Pointer DTMImagePointer;
            typedef typename DTMImageType::ConstPointer DTMImageConstPointer;
            typedef typename DTMImageType::PixelType DTMImagePixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;
            typedef typename InputMaskType::InternalPixelType InputMaskInternalPixelType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;

            typedef itk::ImageRegionConstIterator<DTMImageType> DTMConstItType;
            typedef itk::ImageRegionConstIterator<GRIDImageType> GRIDConstItType;

            typedef DEMDataManager<DTMImageType> Intersector3DType;
            typedef itk::Vector<typename PointType::ValueType, 2> VectorType;

            // L2CoarseResolution accessors
            itkSetMacro(L2CoarseResolution, unsigned int)
            itkGetConstReferenceMacro(L2CoarseResolution, unsigned int)

            // B5 reference altitude for viewing direction accessors
            itkSetMacro(VIEHRef, double)
            itkGetConstReferenceMacro(VIEHRef, double)

            // Reference altitude for solar direction accessors
            itkSetMacro(SOLHRef, double)
            itkGetConstReferenceMacro(SOLHRef, double)

            // Distance beyond which the pixel is declared hidden
            itkSetMacro(DistanceThreshold, double)
            itkGetConstReferenceMacro(DistanceThreshold, double)

            // Intersector 3D object
            virtual void
            SetI3D(Intersector3DType* i3d)
            {
                m_I3D = i3d;
                if (m_I3D == NULL)
                {
                    vnsExceptionBusinessMacro("I3D pointer is NULL!");
                }
            }

            /** Set the solar direction grid */
            vnsSetGetInputMacro( IPSOL, GRIDImageType, 0)
            /** Set the viewing direction grid */
            vnsSetGetInputMacro( IPVIE, GRIDImageType, 1)
            /** Set the DTM image */
            vnsSetGetInputMacro( DTM, DTMImageType, 2)
            /** Set the edege mask */
            vnsSetGetInputMacro( IPEDG, InputMaskType, 3)

            /** Get the cast shadow mask */
            OutputImageType *
            GetIPSHDOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
            }


            /** Get the hidden faces mask */
            OutputImageType *
            GetIPHIDOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(1));
            }


        protected:
            /** Constructor */
            DTMProcessingImageFilter();
            /** Destructor */
            virtual
            ~DTMProcessingImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Multi-thread version GenerateData. */
            virtual void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

        private:
            DTMProcessingImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            VectorType
            ComputeDTMIntersection(const PointType & groundPoint, const PointType & gridPoint, const DTMConstItType & dtmIt,
                    const double p_GRIDHRef, const std::string & comment);

            /** L2 coarse resolution declaration */
            unsigned int m_L2CoarseResolution;

            /** Reference altitude for viewing direction declaration */
            double m_VIEHRef;

            /** Reference altitude for solar direction declaration */
            double m_SOLHRef;

            /** Distance beyond which the pixel is declared hidden */
            double m_DistanceThreshold;

            // Intersector 3D
            Intersector3DType* m_I3D;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsDTMProcessingImageFilter.txx"
#endif

#endif /* __vnsDTMProcessingImageFilter_h */
