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
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 															Création de l'algorithme SnowMaskValidation		*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsSnowMaskValidationByLabelImageFilter.h 3279 2011-10-04 14:44:32Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskValidationByLabelImageFilter_h
#define __vnsSnowMaskValidationByLabelImageFilter_h

#include "vnsMacro.h"
#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

namespace vns
{
    /** \class  PersistentSnowMaskValidationByLabelImageFilter
     * \brief This class tests if all the pixels of an input mask ( snow border mask by label )
     * are classified as cloudy (or edge), then the snow zone is surrounded by clouds.
     * The class returns true.
     * Therefore the pixels of this zone are considered as cloudy and not snowy. The input cloud
     * and snow masks will be updated for all the pixels in the current label.
     *
     * The ThreadedGeberateData method computes the values for each thread.
     * The Synthetize method gathers every thread values.
     * The outputs are :
     *
     * \begin{itemize}
     * \item the number of snowy pixels
     * \item the number of valid pixels
     * \item a boolean that indicates if all the pixels in the label are surrounded by clouds
     * \end{itemize}
     *
     * Input masks have to be otbImage.
     *
     * \author CS Systemes d'Information
     *
     * \sa Object
     * \sa otbPersistentImageFilter
     * \sa otbPersistentFilterStreamingDecorator
     *
     * \ingroup L2
     *
     */

    template<class TMaskType>
        class PersistentSnowMaskValidationByLabelImageFilter : public otb::PersistentImageFilter<TMaskType, TMaskType>
        {
        public:
            /** Standard class typedefs. */
            typedef PersistentSnowMaskValidationByLabelImageFilter Self;
            typedef otb::PersistentImageFilter<TMaskType, TMaskType> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(PersistentSnowMaskValidationByLabelImageFilter, PersistentImageFilter )

            /** Some convenient typedefs. */
            typedef TMaskType MaskType;
            typedef typename MaskType::Pointer MaskPointerType;
            typedef typename MaskType::ConstPointer MaskConstPointer;
            typedef typename MaskType::PixelType MaskPixelType;
            typedef typename MaskType::RegionType MaskRegionType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;

            /** Type of DataObjects used for outputs */
            typedef unsigned long ULType;
            typedef itk::SimpleDataObjectDecorator<ULType> ULObjectType;

            /** vector of map typedefs */
            typedef std::vector<ULType> ULVectorType;

            /** Getters/Setters **/
            vnsSetGetInputRawMacro( SNWLabelBorderMask , MaskType , 0 )
            vnsSetGetInputRawMacro( IPEDGMask , MaskType , 1)
            vnsSetGetInputRawMacro( IPCLDReflMask , MaskType , 2 )
            vnsSetGetInputRawMacro( IPCLDReflVarMask , MaskType , 3 )

            const ULObjectType *
            GetNumberOfPixels() const
            {
                return static_cast<const ULObjectType*>(this->itk::ProcessObject::GetOutput(1));
            }
            const ULObjectType *
            GetSnowPixels() const
            {
                return static_cast<const ULObjectType*>(this->itk::ProcessObject::GetOutput(2));
            }

            vnsGetMacro(IsValid, bool)
            vnsGetConstMacro(IsValid, bool)

            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer
            MakeOutput(unsigned int idx);

            /** Pass the input through unmodified. Do this by Grafting in the
             *  AllocateOutputs method.
             */
            virtual void
            AllocateOutputs(void);
            virtual void
            GenerateOutputInformation(void);
            virtual void
            Synthetize(void);
            virtual void
            Reset(void);

        protected:
            /** Constructor */
            PersistentSnowMaskValidationByLabelImageFilter();
            /** Destructor */
            virtual
            ~PersistentSnowMaskValidationByLabelImageFilter()
            {
            }

            /** Multi-thread version GenerateData. */
            virtual void
            ThreadedGenerateData(const MaskRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            PersistentSnowMaskValidationByLabelImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Validate the presence of all inputs and their sizes. If at least one input
             * is missing, throws an exception. */
            void
            CheckInputs();

            ULVectorType m_NumberOfPixels;
            ULVectorType m_SnowPixels;
            bool m_IsValid;

        };
    // end of class PersistentConditionalStatisticsVectorImageFilter

    /**===========================================================================*/

    /** \class  SnowMaskValidationByLabelImageFilter
     * \brief This class tests if the snow zone (label)
     * is entirely surrounded by clouds (or edges). If it is the class returns true.
     *
     * The outputs are :
     *
     * \begin{itemize}
     * \item the number of snowy pixels
     * \item the number of valid pixels
     * \item a boolean that indicates if all the pixels in the label are surrounded by clouds
     * \end{itemize}
     *
     * Input masks have to be otbImage.
     *
     * \author CS Systemes d'Information
     *
     * \sa vnsPersistentSnowMaskValidationByLabelImageFilter
     * \sa otbPersistentImageFilter
     * \sa otbPersistentFilterStreamingDecorator
     *
     * \ingroup L2
     *
     */

    template<class TMaskType>
        class SnowMaskValidationByLabelImageFilter : public otb::PersistentFilterStreamingDecorator<
                PersistentSnowMaskValidationByLabelImageFilter<TMaskType> >
        {
        public:
            /** Standard Self typedef */
            typedef SnowMaskValidationByLabelImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentSnowMaskValidationByLabelImageFilter<TMaskType> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(SnowMaskValidationByLabelImageFilter,PersistentFilterStreamingDecorator)

            typedef typename Superclass::FilterType StatFilterType;
            typedef typename StatFilterType::ULType ULType;

            /** Some convenient typedefs. */
            typedef TMaskType MaskType;
            typedef typename MaskType::Pointer MaskPointerType;
            typedef typename MaskType::ConstPointer MaskConstPointer;
            typedef typename MaskType::PixelType MaskPixelType;
            typedef typename MaskType::RegionType MaskRegionType;

            /** Getters/Setters **/
            void
            SetSNWLabelBorderMask(const MaskType * pMask)
            {
                this->GetFilter()->SetSNWLabelBorderMask(pMask);
            }
            MaskType*
            GetSNWLabelBorderMask()
            {
                return this->GetFilter()->GetSNWLabelBorderMask();
            }

            void
            SetIPEDGMask(const MaskType * pMask)
            {
                this->GetFilter()->SetIPEDGMask(pMask);
            }
            MaskType*
            GetIPEDGMask()
            {
                return this->GetFilter()->GetIPEDGMask();
            }

            void
            SetIPCLDReflMask(const MaskType * pMask)
            {
                this->GetFilter()->SetIPCLDReflMask(pMask);
            }
            MaskType*
            GetIPCLDReflMask()
            {
                return this->GetFilter()->GetIPCLDReflMask();
            }

            void
            SetIPCLDReflVarMask(const MaskType * pMask)
            {
                this->GetFilter()->SetIPCLDReflVarMask(pMask);
            }
            MaskType*
            GetIPCLDReflVarMask()
            {
                return this->GetFilter()->GetIPCLDReflVarMask();
            }

            // Outputs
            const ULType
            GetNumberOfValidPixels() const
            {
                return this->GetFilter()->GetNumberOfPixels()->Get();
            }
            const ULType
            GetSnowPixels() const
            {
                return this->GetFilter()->GetSnowPixels()->Get();
            }

            bool
            GetIsValid() const
            {
                return this->GetFilter()->GetIsValid();
            }

        protected:
            /** Constructor */
            SnowMaskValidationByLabelImageFilter()
            {
            }

            /** Destructor */
            virtual
            ~SnowMaskValidationByLabelImageFilter()
            {
            }

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            SnowMaskValidationByLabelImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsSnowMaskValidationByLabelImageFilter.txx"
#endif

#endif /* __vnsSnowMaskValidationByLabelImageFilter_h */
