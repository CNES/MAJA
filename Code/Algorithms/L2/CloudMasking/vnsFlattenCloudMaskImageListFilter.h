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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection **
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsFlattenCloudMaskImageListFilter_h__
#define vnsFlattenCloudMaskImageListFilter_h__

#include <set>
#include <map>
#include <cassert>
#include "otbImageList.h"
#include "otbImageListToImageFilter.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  FlattenCloudMaskImageListFilter
     * \brief This class flatten an image list into a mask.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<typename TInputMask, typename TOutputMask>
        class FlattenCloudMaskImageListFilter : public otb::ImageListToImageFilter<TInputMask, TOutputMask>
        {
        public:

            /**@name Standard class typedefs */
            //@{
            typedef FlattenCloudMaskImageListFilter Self;
            typedef otb::ImageListToImageFilter<TInputMask, TOutputMask> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;
            //@}

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(FlattenCloudMaskImageListFilter, ImageListToImageFilter)

            /**@name Typedefs */
            //@{
            typedef TInputMask InputMaskType;
            typedef typename Superclass::InputImageListType InputMaskListType;
            typedef TOutputMask OutputMaskType;
            typedef typename InputMaskListType::Pointer InputMaskListPointerType;
            typedef typename OutputMaskType::Pointer OutputMaskPointerType;

            typedef typename Superclass::InputImagePixelType MaskImagePixelType;
            //@}

        protected:
            /** Constructor */
            FlattenCloudMaskImageListFilter();
            /** Destructor */
            virtual
            ~FlattenCloudMaskImageListFilter()
            {
            }

            virtual void
            GenerateData() /*ITK_OVERRIDE*/;

            /** GenerateOutputInformation
             * Set the number of bands of the output.
             * Copy informations from the first image of the list if existing.
             **/
            virtual void
            GenerateOutputInformation(void) /*ITK_OVERRIDE*/;

            /**
             * GenerateInputRequestedRegion
             * Set the requested region of each image in the list.
             */
            virtual void
            GenerateInputRequestedRegion(void) /*ITK_OVERRIDE*/;

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
            /** The function that does compute the output. */
            void
            DoGenerateData();
        };

} // vns namespace

#include "vnsFlattenCloudMaskImageListFilter.txx"

#endif // vnsFlattenCloudMaskImageListFilter_h__
