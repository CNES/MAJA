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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsCloudGeomMasksToKeptLabelMasksFilter_h
#define vnsCloudGeomMasksToKeptLabelMasksFilter_h

#include <set>
#include <map>
#include "otbImageList.h"
#include "otbImageListToImageFilter.h"
#include "vnsMacro.h"

namespace vns
{
    template<typename TInputGeomLabelImage, typename TOutputKeptLabelsMasks>
        class CloudGeomMasksToKeptLabelMasksFilter : public otb::ImageListToImageFilter<TInputGeomLabelImage,
                TOutputKeptLabelsMasks>
        {
        public:

            /**@name Standard class typedefs */
            //@{
            typedef CloudGeomMasksToKeptLabelMasksFilter                                      Self;
            typedef otb::ImageListToImageFilter<TInputGeomLabelImage, TOutputKeptLabelsMasks> Superclass;
            typedef itk::SmartPointer<Self>                                                   Pointer;
            typedef itk::SmartPointer<const Self>                                             ConstPointer;
            //@}

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(CloudGeomMasksToKeptLabelMasksFilter, ImageListToImageFilter)

            /**@name Typedefs */
            //@{
            typedef TInputGeomLabelImage                                 InputGeomLabelImageType;
            typedef typename Superclass::InputImageListType              InputGeomLabelImageListType;
            typedef typename InputGeomLabelImageListType::Pointer        InputGeomLabelImageListPointerType;
            typedef TOutputKeptLabelsMasks                               OutputKeptLabelsMasksType;
            typedef typename OutputKeptLabelsMasksType::Pointer          OutputKeptLabelsMasksPointerType;

            typedef typename Superclass::InputImagePixelType             LabelImagePixelType;
            typedef unsigned long                                        NbPixelsType;
            typedef typename std::map<LabelImagePixelType, NbPixelsType> NbPixelsForEachLabelMapType;
//    typedef typename std::map<unsigned short, NbPixelsType>            NbCLDPixelsForEachLabelMapType;

            typedef std::set<LabelImagePixelType>                        LabelImagePixelSetType;
            //@}

            /**@name Inputs */
            //@{
            vnsSetRawMacro(CloudPixelsNumberMap, NbPixelsForEachLabelMapType)
            vnsSetRawMacro(ShadowPixelsNumberMap, NbPixelsForEachLabelMapType)
            itkSetMacro (ShadowCloudRatioPixNumberThreshold, NbPixelsType)
            itkSetMacro (ShadowCloudRatioMax, double)
            itkSetMacro (ShadowCloudRatioMin, double)
            //@}

            /**@name Outputs */
            //@{
            itkGetConstMacro(RejectedLabelList, LabelImagePixelSetType)
            itkGetConstMacro(KeptLabelList, LabelImagePixelSetType)
            //@}

        protected:
            /** Constructor */
            CloudGeomMasksToKeptLabelMasksFilter();
            /** Destructor */
            virtual
            ~CloudGeomMasksToKeptLabelMasksFilter()
            {
            }

            virtual void
            GenerateData() /*ITK_OVERRIDE*/;

            /** GenerateOutputInformation
             * Set the number of bands of the output.
             * Copy informations from the first image of the list if existing.
             **/
            virtual void GenerateOutputInformation(void) /*ITK_OVERRIDE*/;

            /**
             * GenerateInputRequestedRegion
             * Set the requested region of each image in the list.
             */
            virtual void GenerateInputRequestedRegion(void) /*ITK_OVERRIDE*/;

            /**PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const /*ITK_OVERRIDE*/;

        private:
            /** Prepares everything before computing the outputs.
             * 1- Check class invariants.
             * 2- Allocates everything
             */
            void
            BeforeGenerateData();
            /** The function that does compute the outputs. */
            void
            DoGenerateData();
            /** Build the list of labels kepts, and label rejected.
             * For each label from the \c m_ShadowPixelsNumberMap:
             * - shadowCloudRatio = (nb cloud pix < m_ShadowCloudRatioPixNumberThreshold= ? m_ShadowCloudRatioMax : m_ShadowCloudRatioMin
             * - label if kept IFF nb shadow pix < shadowCloudRatio
             */
            void
            DoBuildLabelLists();

            void
            DoBuildMaskImage();

            /**@name Other inputs */
            //@{
            NbPixelsForEachLabelMapType m_CloudPixelsNumberMap;
            NbPixelsForEachLabelMapType m_ShadowPixelsNumberMap;
            NbPixelsType m_ShadowCloudRatioPixNumberThreshold;
            double m_ShadowCloudRatioMin;
            double m_ShadowCloudRatioMax;
            //@}

            /**@name Other outputs */
            //@{
            /** Set of rejected labels.
             * By construction, it will be sorted.
             */
            LabelImagePixelSetType m_RejectedLabelList;
            /** Set of kept labels.
             * By construction, it will be sorted.
             */
            LabelImagePixelSetType m_KeptLabelList;
            //@}
        };

} // vns namespace

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsCloudGeomMasksToKeptLabelMasksFilter.txx"
#endif

#endif // __vnsCloudGeomMasksToKeptLabelMasksFilter_h
