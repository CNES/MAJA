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
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mai 2014 : Améliorations algorithmiques (Spec v2.1)		*
 * 							Correction boucle sur les threads dans méthode Synthetize						*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudAlreadyDetectedImageFilter_h
#define __vnsCloudAlreadyDetectedImageFilter_h

#include "itkImageToImageFilter.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  PersistentCloudAlreadyDetectedImageFilter
     * \brief This class detects pixels that have already been detected
     * by radiometric tests.
     *
     * The ThreadedGeberateData method computes the values for each thread.
     * The Synthetize method gathers every thread values.
     * The output is a boolean that indicates if a cloud has been already detected.
     *
     * Input image have to be otbImage.
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

    template<class TInputMask, class TInputLabel>
        class PersistentCloudAlreadyDetectedImageFilter : public otb::PersistentImageFilter<TInputMask, TInputMask>
        {
        public:
            /** Standard class typedefs. */
            typedef PersistentCloudAlreadyDetectedImageFilter Self;
            typedef otb::PersistentImageFilter<TInputMask, TInputMask> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(PersistentCloudAlreadyDetectedImageFilter, PersistentImageFilter )

            /** Some convenient typedefs. */
            typedef TInputLabel InputLabelType;
            typedef typename InputLabelType::Pointer InputLabelPointerType;
            typedef typename InputLabelType::ConstPointer InputLabelConstPointer;
            typedef typename InputLabelType::PixelType InputLabelPixelType;
            typedef typename InputLabelType::RegionType InputImageRegionType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointerType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;
            typedef typename InputMaskType::InternalPixelType InputMaskInternalPixelType;

            /** Type to use for computations. */
            typedef typename itk::NumericTraits<InputMaskInternalPixelType>::RealType RealType;

            /** Result map typedef */
            typedef typename std::map<InputLabelPixelType, bool> BoolMapType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;
            typedef itk::SimpleDataObjectDecorator<BoolMapType> MapBoolObjectType;

            /** vector of map typedefs */
            typedef std::vector<BoolMapType> BoolVectorMapType;

            // LAIG-FA-MAC-1555-CNES : inversion odre des index des input 0 et 1 pour que le  this->GetInput() dans  GenerateOutputInformation() soit compatible avec premier template
            vnsSetGetInputMacro( IPCLDExtLabel, InputLabelType, 1)
            vnsSetGetInputMacro( IPCLDRefl, InputMaskType, 0)
            vnsSetGetInputMacro( IPCLDReflVar, InputMaskType, 2)

            const MapBoolObjectType*
            GetCloudFoundMap() const;

            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer
            MakeOutput(unsigned int idx);

            /** Inside value accessor */
            itkSetMacro(InsideValue, InputMaskPixelType)
            itkGetConstReferenceMacro(InsideValue, InputMaskPixelType)


            /** Excluded shadow mask value */
            itkSetMacro(ExcludedLabelValue, InputLabelPixelType)
            itkGetConstReferenceMacro(ExcludedLabelValue, InputLabelPixelType)

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
            PersistentCloudAlreadyDetectedImageFilter();
            /** Destructor */
            virtual
            ~PersistentCloudAlreadyDetectedImageFilter()
            {
            }

            /** Multi-thread version GenerateData. */
            virtual void
            ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            PersistentCloudAlreadyDetectedImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            InputMaskPixelType m_InsideValue;
            InputLabelPixelType m_ExcludedLabelValue;

            BoolVectorMapType m_CloudFoundVectorMap;

        };
    // end of class PersistentCloudAlreadyDetectedImageFilter

    /**===========================================================================*/

    /** \class  CloudAlreadyDetectedImageFilter
     * \brief This class implements detects pixels that have already been detected
     * by radiometric tests.
     *
     * Input image have to be otbImage.
     *
     * \author CS Systemes d'Information
     *
     * \sa vnsPersistentCloudAlreadyDetectedImageFilter
     * \sa otbPersistentImageFilter
     * \sa otbPersistentFilterStreamingDecorator
     *
     * \ingroup L2
     *
     */

    template<class TInputMask, class TInputLabel>
        class CloudAlreadyDetectedImageFilter : public otb::PersistentFilterStreamingDecorator<
                PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel> >
        {
        public:
            /** Standard Self typedef */
            typedef CloudAlreadyDetectedImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentCloudAlreadyDetectedImageFilter<TInputMask, TInputLabel> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(CloudAlreadyDetectedImageFilter,PersistentFilterStreamingDecorator)

            typedef typename Superclass::FilterType StatFilterType;

            /** Some convenient typedefs. */
            typedef TInputLabel InputLabelType;
            typedef typename InputLabelType::ConstPointer InputLabelConstPointerType;
            typedef typename InputLabelType::Pointer InputLabelPointerType;
            typedef typename InputLabelType::PixelType InputLabelPixelType;
            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskImagePointerType;
            typedef typename InputMaskType::ConstPointer InputMaskImageConstPointer;
            typedef typename InputMaskType::PixelType InputMaskImagePixelType;

            /** Containers typedefs from PersistentCloudAlreadyDetectedImageFilter */
            typedef typename StatFilterType::BoolMapType BoolMapType;

            /** Getters/Setters **/
            void
            SetIPCLDExtLabel(const InputLabelType * image)
            {
                this->GetFilter()->SetIPCLDExtLabelInput(image);
            }

            InputLabelType*
            GetIPCLDExtLabel()
            {
                return this->GetFilter()->GetIPCLDExtLabelInput();
            }

            void
            SetIPCLDRefl(const InputMaskType * image)
            {
                this->GetFilter()->SetIPCLDReflInput(image);
            }
            InputMaskType*
            GetIPCLDRefl()
            {
                return this->GetFilter()->GetIPCLDReflInput();
            }

            void
            SetIPCLDReflVar(const InputMaskType * image)
            {
                this->GetFilter()->SetIPCLDReflVarInput(image);
            }
            InputMaskType*
            GetIPCLDReflVar()
            {
                return this->GetFilter()->GetIPCLDReflVarInput();
            }

            /** Set the inside mask pixel value.*/
            void
            SetInsideValue(const InputMaskImagePixelType & pixVal)
            {
                this->GetFilter()->SetInsideValue(pixVal);
            }
            InputMaskImagePixelType
            GetInsideValue()
            {
                return this->GetFilter()->GetInsideValue();
            }

            /** Set the shadow mask excluded value.*/
            void
            SetExcludedLabelValue(const InputLabelPixelType & pixVal)
            {
                this->GetFilter()->SetExcludedLabelValue(pixVal);
            }
            InputLabelPixelType
            GetExcludedLabelValue()
            {
                return this->GetFilter()->GetExcludedLabelValue();
            }

            // Outputs
            const BoolMapType
            GetCloudFound() const
            {
                return this->GetFilter()->GetCloudFoundMap()->Get();
            }

        protected:
            /** Constructor */
            CloudAlreadyDetectedImageFilter()
            {
            }
            /** Destructor */
            virtual
            ~CloudAlreadyDetectedImageFilter()
            {
            }

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            CloudAlreadyDetectedImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsCloudAlreadyDetectedImageFilter.txx"
#endif

#endif /* __vnsCloudAlreadyDetectedImageFilter_h */
