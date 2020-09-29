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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * MOD : VERSION : 1-0-0  : 27 oct. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMultiSpectralAOTMethodFunctions_h
#define __vnsMultiSpectralAOTMethodFunctions_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "vnsMacro.h"
#include "otbVegetationIndicesFunctor.h"

namespace vns
{
    /** \class  MultiSpectralAOTMethodFunctions
     * \brief This class implements .
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa Object
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
    template<class TInputImage, class TInputMask, class TInputVectorMask>
        class MultiSpectralAOTMethodFunctions : public itk::Object
        {
        public:
            /** Standard class typedefs. */
            typedef MultiSpectralAOTMethodFunctions Self;
            typedef itk::Object Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            ;
            /** Creation through object factory macro */
            itkTypeMacro(MultiSpectralAOTMethodFunctions, Object )
            ;

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointerType;
            typedef typename InputImageType::Pointer InputImagePointerType;
            typedef typename InputImageType::RegionType InputImageRegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::InternalPixelType InternalPixelType;
            typedef typename InputImageType::IndexType IndexType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::OffsetType OffsetType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointerType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointerType;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TInputVectorMask InputVectorMaskType;
            typedef typename InputVectorMaskType::Pointer InputVectorMaskPointerType;
            typedef typename InputVectorMaskType::ConstPointer InputVectorMaskConstPointerType;
            typedef typename InputVectorMaskType::PixelType InputVectorMaskPixelType;
            typedef typename InputVectorMaskType::InternalPixelType InputVectorMaskInternalPixelType;


            typedef otb::Functor::NDVI<InternalPixelType,
                                       InternalPixelType>   NDVIFunctorType;

            typedef std::vector<double> DoubleVectType;
            typedef std::vector<unsigned int> UIVectType;
            typedef std::vector<InputImagePixelType> PixelVectType;
            typedef std::vector<IndexType> IndexVectType;
            typedef std::vector<OffsetType> OffsetVectType;

            /** Multi temporal selection of the pixels for the local inversion */
            static void
            AOTPixelSelection(const OffsetType & pOffset, const InputImagePixelType & pIPTOACSubPix, const InputMaskPixelType & pIPEDGPix,
                    const InputMaskPixelType & pIPSNWSubPix, const InputMaskPixelType & pL2CLDSubPix,
                    const InputVectorMaskPixelType & pIPSATPix, const InputMaskPixelType & pIPSHDSubPix,
                    const InputMaskPixelType & pL2WASSubPix, const std::vector<unsigned int> & pAOTbands, const unsigned int & pRedChannel,
                    const unsigned int & pNDVIAOTBand1, const unsigned int & pNDVIAOTBand2, const double & pRedTOAThreshold,
                    const double & pNDVIThreshold, OffsetVectType & pValidOffsets, PixelVectType & pValidIPTOAC,
                    bool & pNeighborhoodNotEmpty);

        protected:
            /** Constructor */
            MultiSpectralAOTMethodFunctions();
            /** Destructor */
            virtual ~MultiSpectralAOTMethodFunctions();
            /** PrintSelf method */
            virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            MultiSpectralAOTMethodFunctions(const Self&); //purposely not implemented
            void operator=(const Self&); //purposely not implemented

        };

    } // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMultiSpectralAOTMethodFunctions.txx"
#endif

#endif /* __vnsMultiSpectralAOTMethodFunctions_h */
