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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de détection *
 *                                                                  des ombres de nuages                    *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-751-CNES : 17 février 2014 : Sélection des pixels pour l'inversion    *
 * 																	(Spec v2.2)								*
 * MOD : VERSION : 1-0-0  : 27 oct. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsMultiTemporalAOTMethodFunctions_h
#define vnsMultiTemporalAOTMethodFunctions_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  MultiTemporalAOTMethodFunctions
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
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate>
        class MultiTemporalAOTMethodFunctions : public itk::Object
        {
        public:
            /** Standard class typedefs. */
            typedef MultiTemporalAOTMethodFunctions Self;
            typedef itk::Object Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self);
            /** Creation through object factory macro */
            itkTypeMacro(MultiTemporalAOTMethodFunctions, Object );

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

            typedef TInputDate InputDateType;
            typedef typename InputDateType::Pointer InputDatePointerType;
            typedef typename InputDateType::ConstPointer InputDateConstPointerType;
            typedef typename InputDateType::PixelType InputDatePixelType;
            typedef typename InputDateType::InternalPixelType InputDateInternalPixelType;
            typedef InputDateInternalPixelType DateType;

            typedef std::vector<double> DoubleVectType;
            typedef std::vector<unsigned int> UIVectType;
            typedef std::vector<InputImagePixelType> PixelVectType;
            typedef std::vector<IndexType> IndexVectType;
            typedef std::vector<OffsetType> OffsetVectType;
            typedef std::list<DateType> DateListType;

            /** Multi temporal selection of the pixels for the local inversion */
            static void
            AOTPixelSelection(const OffsetType & pOffset, const InputImagePixelType & pIPTOACSubPix,
                    const InputImagePixelType & pL2inRTAPix, const InputMaskPixelType & pIPEDGPix, const InputMaskPixelType & pIPSNWSubPix,
                    const InputMaskPixelType & pL2inNDTPix, const InputMaskPixelType & pL2CLDPix,
                    const InputVectorMaskPixelType & pIPSATPix, const InputMaskPixelType & pIPSHDPix,
                    const InputMaskPixelType & pL2WASSubPix, const InputDatePixelType & pL2inPXDPix,
                    const std::vector<unsigned int> & pAOTbands, const unsigned int pVARBandD, const unsigned int pVARBandDm1, const double pRelativeVarMax,
                    const DoubleVectType & pLowReflectanceThresholds, const DoubleVectType & pHighReflectanceThresholds,
                    const double pAgeMax, const double pJDay,
                    OffsetVectType & pValidOffsets, PixelVectType & pValidIPTOAC, PixelVectType & pValidL2inRTA, DateListType & pDates,
                    bool & pNeighborhoodNotEmpty);

            /** Computation of the most frequent date in the list */
            static unsigned int ComputeGreaterFrequencyDate(const DateListType & pDateList);

            /** Extract the values of the vector associated to the LPD date */
           template <typename TAnyVectType>
           static TAnyVectType ExtractMoreFreqValues( const DateListType & pDateList, const TAnyVectType & pVect,
                    const unsigned int pGreaterFreqDate);

           /**
             * Compute the weight used for Levenberg Marquard algorithm.
             * If the reutrn bool is false, the list of pixel in not valid, lWeq1 will be set to 0.
             */
            static bool ComputeWeight(const int lGreaterFreq,
                    const DateListType & pDates, const PixelVectType & pValidIPTOAC, const PixelVectType & pValidL2inRTA,
                    const std::vector<unsigned int> & pAOTbandsD, const std::vector<unsigned int> & pAOTbandsDm1, const unsigned int pSTDEVBand, const double pMinStdDevThreshold,
                    const double pStdNoise, double & pWeq1);

        protected:
            /** Constructor */
            MultiTemporalAOTMethodFunctions();
            /** Destructor */
            virtual ~MultiTemporalAOTMethodFunctions();

        private:
            MultiTemporalAOTMethodFunctions(const Self&); //purposely not implemented
            void operator=(const Self&); //purposely not implemented
        };

    } // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMultiTemporalAOTMethodFunctions.txx"
#endif

#endif /* vnsMultiTemporalAOTMethodFunctions_h */
