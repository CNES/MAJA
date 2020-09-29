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
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 18 mai 2016 : Correction refactoring,menage, etc...       *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de détection *
 *                                                                  des ombres de nuages                    *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-751-CNES : 17 février 2014 : Sélection des pixels pour l'inversion    *
 * 																	(Spec v2.2)								*
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 21 novembre 2013 : Correction de l'information géographique*
 * 									dans les produits. Methode GenerateOutputInformation privée.			*
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 18 oct. 2011 : Méthode multi temporelle                         *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsMultiTemporalComputeAOTImageFilter_h
#define vnsMultiTemporalComputeAOTImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "vnsMultiTemporalAOTCostFunction.h"
#include "vnsRunLevenbergMarquardtOptimization.h"
#include "vnsMultiTemporalAOTMethodFunctions.h"
#include "vnsMacro.h"
#include <map>

namespace vns
{
    /** \class  MultiTemporalComputeAOTImageFilter
     * \brief * \brief This class implements the multi-temporel method that is recurrent algorithm
     * based on the stability of surface reflectances during successive acquisitions of the same site.
     *
     * The AOT esimate uses a neighborhood of pixels at L2 coarse resolution. Pixels are declared
     * as suitable for the AOT estimation according two criteria:
     *      - pixels are within the image, not saturated, not cloudy, not cloud shadowed, not snow,
     *      not water,
     *      - Short wave infra-red surface reflectance varies slowly between the composite product and
     *      the present one (D). The variability in the reflectances of the neighborhood should be
     *      sufficient.
     *
     * Then, a non linear least squares inversion algorithm is used to determine the aerosol
     * optical thickness. A specific cost function is defined for this method based on the
     * difference between surface reflectances of the present product (date D) and the previous one
     * (date D-1) and between the present product and the composite one.
     *
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * TInputImage  : vector image of double expected
     * TInputMask   : Monochannel image of short expected
     * TInputDate   : Monochannel image of short expected
     * TOutputImage : Monochannel image of double expected
     * TOutputMask  : Monochannel image of short expected
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TInputVectorMask, class TInputDate, class TInputDTM, class TInputLUT,
            class TOutputImage, class TOutputMask>
        class MultiTemporalComputeAOTImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef MultiTemporalComputeAOTImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(MultiTemporalComputeAOTImageFilter, ImageToImageFilter )

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

            /** Input AOT image typedef */
            typedef TOutputImage AOTImageType;
            typedef typename AOTImageType::ConstPointer AOTImageConstPointerType;
            typedef typename AOTImageType::Pointer AOTImagePointerType;
            typedef typename AOTImageType::PixelType AOTPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointerType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointerType;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TInputVectorMask InputVectorMaskType;
            typedef typename InputVectorMaskType::Pointer InputVectorMaskPointerType;
            typedef typename InputVectorMaskType::ConstPointer InputVectorMaskConstPointerType;
            typedef typename InputVectorMaskType::PixelType InputVectorMaskPixelType;

            typedef TInputDate InputDateType;
            typedef typename InputDateType::Pointer InputDatePointerType;
            typedef typename InputDateType::ConstPointer InputDateConstPointerType;
            typedef typename InputDateType::PixelType InputDatePixelType;
            typedef typename InputDateType::InternalPixelType InputDateInternalPixelType;
            typedef InputDateInternalPixelType DateType;

            typedef TInputDTM InputDTMType;
            typedef typename InputDTMType::Pointer InputDTMPointerType;
            typedef typename InputDTMType::ConstPointer InputDTMConstPointerType;
            typedef typename InputDTMType::PixelType InputDTMPixelType;
            typedef typename InputDTMType::InternalPixelType InputDTMInternalPixelType;

            typedef TInputLUT LUTType;
            typedef typename LUTType::Pointer LUTPointerType;
            typedef typename LUTType::PixelType LUTPixelType;
            typedef typename LUTType::ParameterValuesType ParameterValuesType;
            typedef std::map<unsigned int, LUTPointerType> LUTMapType;

            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointerType;
            typedef typename OutputImageType::ConstPointer OutputImageConstPointerType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::InternalPixelType OutputImagePixelType;
            typedef typename OutputImageType::IndexType OutputImageIndexType;
            typedef typename OutputImageType::SizeType OutputImageSizeType;
            typedef OutputImagePixelType NoDataType;

            typedef TOutputMask OutputMaskType;
            typedef typename OutputMaskType::Pointer OutputMaskPointerType;
            typedef typename OutputMaskType::ConstPointer OutputMaskConstPointer;
            typedef typename OutputMaskType::PixelType OutputMaskPixelType;
            typedef typename OutputMaskType::RegionType OutputMaskRegionType;

            typedef itk::ImageBase<2> ImageBaseType;

            typedef std::vector<double> DoubleVectType;
            typedef std::vector<unsigned int> UIVectType;
            typedef std::vector<InputImagePixelType> PixelVectType;
            typedef std::vector<IndexType> IndexVectType;
            typedef std::vector<OffsetType> OffsetVectType;
            typedef std::list<DateType> DateListType;

            typedef typename itk::ImageRegionIteratorWithIndex<OutputImageType> OutputImageRegionIterator;
            typedef typename itk::ImageRegionIteratorWithIndex<OutputMaskType> OutputMaskRegionIterator;

            /** Neighborhood Iterators */
            typedef itk::ConstNeighborhoodIterator<InputImageType> ImageNeighborhoodIteratorType;
            typedef itk::ConstNeighborhoodIterator<InputMaskType> MaskNeighborhoodIteratorType;
            typedef itk::ConstNeighborhoodIterator<InputVectorMaskType> VectorMaskNeighborhoodIteratorType;
            typedef itk::ConstNeighborhoodIterator<InputDateType> DateNeighborhoodIteratorType;
            typedef itk::ConstNeighborhoodIterator<InputDTMType> DTMNeighborhoodIteratorType;

            typedef typename ImageNeighborhoodIteratorType::RadiusType RadiusType;

            /** FaceLists */
            typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> ImageBoundaryFacesType;
            typedef typename ImageBoundaryFacesType::FaceListType ImageFaceListType;
            typedef typename ImageFaceListType::iterator ImageFaceListIteratorType;
            typedef typename itk::ZeroFluxNeumannBoundaryCondition<InputImageType> ImageZeroFluxNeumannBCType;

            typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputMaskType> MaskBoundaryFacesType;
            typedef typename MaskBoundaryFacesType::FaceListType MaskFaceListType;
            typedef typename MaskFaceListType::iterator MaskFaceListIteratorType;
            typedef typename itk::ZeroFluxNeumannBoundaryCondition<InputMaskType> MaskZeroFluxNeumannBCType;

            typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputVectorMaskType> VectorMaskBoundaryFacesType;
            typedef typename VectorMaskBoundaryFacesType::FaceListType VectorMaskFaceListType;
            typedef typename VectorMaskFaceListType::iterator VectorMaskFaceListIteratorType;
            typedef typename itk::ZeroFluxNeumannBoundaryCondition<InputVectorMaskType> VectorMaskZeroFluxNeumannBCType;

            typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputDateType> DateBoundaryFacesType;
            typedef typename DateBoundaryFacesType::FaceListType DateFaceListType;
            typedef typename DateFaceListType::iterator DateFaceListIteratorType;
            typedef typename itk::ZeroFluxNeumannBoundaryCondition<InputDateType> DateZeroFluxNeumannBCType;

            typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputDTMType> DTMBoundaryFacesType;
            typedef typename DTMBoundaryFacesType::FaceListType DTMFaceListType;
            typedef typename DTMFaceListType::iterator DTMFaceListIteratorType;
            typedef typename itk::ZeroFluxNeumannBoundaryCondition<InputDTMType> DTMZeroFluxNeumannBCType;

            typedef MultiTemporalAOTCostFunction<InputImageType, InputDTMType, LUTType> AOTCostFunctionType;

            typedef RunLevenbergMarquardtOptimization<InputImageType, InputDTMType, LUTType, OutputImageType, AOTCostFunctionType> RunLMOptimizationType;
            typedef typename RunLMOptimizationType::Pointer RunLMOptimizationPointerType;

            typedef MultiTemporalAOTMethodFunctions<InputImageType, InputMaskType, InputVectorMaskType, InputDateType> MultiTemporalAOTMethodFunctionsType;

            /** Setters & Getters : Overload SetInput method. */
            vnsSetGetInputRawMacro(L2CLDSub, InputMaskType, 1)
            vnsSetGetInputRawMacro(IPSHDSub, InputMaskType, 2)
            vnsSetGetInputRawMacro(IPSAT, InputVectorMaskType, 3)
            vnsSetGetInputRawMacro(L2WASSub, InputMaskType, 4)
            vnsSetGetInputRawMacro(IPEDG, InputMaskType, 5)
            vnsSetGetInputRawMacro(IPSNWSub, InputMaskType, 6)
            vnsSetGetInputRawMacro(L2inNDT, InputMaskType, 7)
            vnsSetGetInputRawMacro(IPTOACSub, InputImageType, 0)
            vnsSetGetInputRawMacro(L2inRTA, InputImageType, 8)
            vnsSetGetInputRawMacro(L2inRTC, InputImageType, 9)
            vnsSetGetInputRawMacro(L2inPXD, InputDateType, 10)
            vnsSetGetInputRawMacro(DTMSub, InputDTMType, 11)
			vnsSetGetInputRawMacro(AOTCamsSub, AOTImageType, 12)

            OutputImageType *
            GetIPAOT();
            OutputImageType *
            GetIPAOTCams();
            OutputMaskType *
            GetOutputMask();

            /** Product julian date accessors */
            itkSetMacro(JDay, double)
            itkGetConstReferenceMacro(JDay, double)

            /** No_data pixel value accessors for output image AOT */
            itkSetMacro(NoData, NoDataType)
            itkGetConstReferenceMacro(NoData, NoDataType)

            /** Minimum radius for the CurrentRegion */
            itkSetMacro(MinSamplingInterval,unsigned int)

            /** Maximum radius for the CurrentRegion */
            itkSetMacro(MaxSamplingInterval,unsigned int)

            /** Increment N distance : step for the sampling interval increase */
            itkSetMacro(IncrementNDistance,unsigned int)

            /** Radius such as the neighborhood size is initialized with :
             * (2*m_Radius+1)*(2*m_Radius+1) */
            itkSetMacro(Radius,unsigned int)

            /** stepAOT * coarse_res = AOT_resolution */
            itkSetMacro(StepAOT,unsigned int)
            itkGetConstMacro(StepAOT,unsigned int)

            /** Maximum NIR variance parameter */
            itkSetMacro(RelativeVarMax,double)

            /** Composite maximum age threshold */
            itkSetMacro(MaxAge,double)

            /** VAR channel ID (11 by default)  for Date D of the product*/
            itkSetMacro(VARBandD,unsigned int)

            /** VAR channel ID (11 by default)  for Date D-1 of the product*/
            itkSetMacro(VARBandDm1,unsigned int)

            /** STDEV channel ID (11 by default) */
            itkSetMacro(STDEVBand,unsigned int)

            /** Std Noise parameter */
            itkSetMacro(StdNoise,double)

            /** Minimum number of pixels parameter */
            itkSetMacro(MinNumberOfPixels,unsigned int)

            /** Standard Deviation threshold parameter */
            itkSetMacro(MinStdDevThreshold,double)

            /** Vector of all used indexes considering the step StepAOT */
            vnsSetMacro(AllIndexes,IndexVectType)

            /** Low TOA reflectance threshold */
            vnsSetMacro(LowReflectanceThresholds, DoubleVectType)

            /** High TOA reflectance threshold */
            vnsSetMacro(HighReflectanceThresholds, DoubleVectType)

            /** Levenberg Marquart Optimization parameters */
            itkSetMacro(F_Tolerance,double)
            // Function value tolerance
            itkSetMacro(G_Tolerance,double)
            // Gradient magnitude tolerance
            itkSetMacro(X_Tolerance,double)
            // Search space tolerance
            itkSetMacro(Epsilon_Function,double)
            // Step
            itkSetMacro(Max_Iterations,int)
            // Maximum number of iterations
            itkSetMacro(AOTinitialD,double)
            itkSetMacro(AOTinitialDm1,double)

			//Boolean to activate CAMS
			itkSetMacro(UseCams,bool);
            //error ponderation for CAMS
            itkSetMacro(KPondCAMS,double);
            //Mean Altitude for CAMS
            itkSetMacro(MeanAltitude,double);
            //Height Scale for CAMS computation
            itkSetMacro(HeightScale,double);

            /** Set darkest pixel AOT object */
            void
            SetDarkestPixelAOT(double pDarkestPixelAOT)
            {
                m_RunLMOptimization->GetCostFunction()->SetDarkestPixelAOT(pDarkestPixelAOT);
            }

            /** Set LUT to the AOTCostFunction object */
            void
            SetLUT(LUTType * pLUT)
            {
                m_RunLMOptimization->GetCostFunction()->SetLUTD(pLUT);
            }

            /** map of LUT associated to the date LPD */
            void
            SetLUTMap(const LUTMapType& pLUTMap)
            {
                m_LUTMap = pLUTMap;
            }

            void
            SetAOTbandsD(UIVectType const& pAOTbands)
            {
                m_RunLMOptimization->GetCostFunction()->SetAOTbandsD(pAOTbands);
                m_AOTbandsD = pAOTbands;
            }

            void
            SetAOTbandsReflectanceCompositeDm1(UIVectType const& pAOTbands)
            {
                m_RunLMOptimization->GetCostFunction()->SetAOTbandsReflectanceCompositeDm1(pAOTbands);
                m_AOTbandsReflectanceCompositeDm1 = pAOTbands;
            }
            void
            SetAOTbandsLUTCompositeDm1(UIVectType const& pAOTbands)
            {
                m_RunLMOptimization->GetCostFunction()->SetAOTbandsLUTCompositeDm1(pAOTbands);
                m_AOTbandsLUTCompositeDm1 = pAOTbands;
            }

            void
            SetWeq1(double pWeq1)
            {
                m_RunLMOptimization->GetCostFunction()->SetWeq1(pWeq1);
            }

            void
            SetWeq2(double pWeq2)
            {
                m_RunLMOptimization->GetCostFunction()->SetWeq2(pWeq2);
            }

            void
            SetWdark(double pWdark)
            {
                m_RunLMOptimization->GetCostFunction()->SetWdark(pWdark);
            }

            void
            SetWbounds(double pWbounds)
            {
                m_RunLMOptimization->GetCostFunction()->SetWbounds(pWbounds);
            }

            void
            SetAOTmin(double pAOTmin)
            {
                m_RunLMOptimization->GetCostFunction()->SetAOTmin(pAOTmin);
            }

            void
            SetPixelList(PixelVectType const& pPixelVect)
            {
                m_RunLMOptimization->GetCostFunction()->SetPixelList(pPixelVect);
            }

        protected:
            /** Constructor */
            MultiTemporalComputeAOTImageFilter();
            /** Destructor */
            virtual
            ~MultiTemporalComputeAOTImageFilter();
            /** Use a marging in the requested regions to be able to compute in a
             * changing currentRegion cf. ThreadedGenerateData(). */
            void
            GenerateInputRequestedRegion();
            /** Validate the presence of all ten inputs. If one or more inputs
             * are missing, throw an exception. */
            void
            BeforeThreadedGenerateData();
            /** Multi-thread version GenerateData. */
            void
            ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** Modify output information */
            virtual void
            GenerateOutputInformation();

        private:
            MultiTemporalComputeAOTImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Add margings to the requested regions */
            void
            PadByRadiusRequestedRegion(ImageBaseType * pImagePtr);

            /** Call the Levenberg Marquardt least squares inversion */
            OutputImagePixelType
            LeastSquaresInversion(const double pWeq1, const unsigned int pLPDconst, const OffsetVectType & pExtractedOffsets,
                    ImageNeighborhoodIteratorType & pIPTOACSubIt, ImageNeighborhoodIteratorType & pL2inRTAIt,
                    ImageNeighborhoodIteratorType & pL2inRTCIt, DTMNeighborhoodIteratorType & pDTMSubIt,
					 const AOTPixelType &pAotCams, RunLMOptimizationPointerType & pLMOptimizer);

            /** Copy attributes from m_RunLMOptimization to the other optimization objects
             * contained into m_VectorOptim. Used to work with treaded generate data.
             */
            void
            CopyLMOptimizationAttributes();

            void
            CallCopyInputRegionToOutputRegion(OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion);

            void
            CallCopyOutputRegionToInputRegion(InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion);

            /** Julian day of the current product */
            double m_JDay;

            /** No_data value set in the output */
            NoDataType m_NoData;

            /** Minimum sampling interval */
            unsigned int m_MinSamplingInterval;

            /** Maximum sampling interval */
            unsigned int m_MaxSamplingInterval;

            /** Step for the sampling interval increase */
            unsigned int m_IncrementNDistance;

            /** Fixed Radius (spec N_AOT) */
            unsigned int m_Radius;

            /** StepAOT */
            unsigned int m_StepAOT;

            /** Maximum NIR variance parameter */
            double m_RelativeVarMax;

            /** Composite maximum age threshold */
            double m_MaxAge;

            /** VAR channel ID (11 by default) for Date D of the product */
            unsigned int m_VARBandD;

            /** VAR channel ID (11 by default) for Date D-1 of the product  */
            unsigned int m_VARBandDm1;

            /** STDEV channel ID */
            unsigned int m_STDEVBand;

            /** Std Noise parameter */
            double m_StdNoise;

            /** Minimum number of pixels parameter */
            unsigned int m_MinNumberOfPixels;

            /** Standard Deviation threshold parameter */
            double m_MinStdDevThreshold;

            /** list of the used band for AOT Estimation for the date D */
            std::vector<unsigned int> m_AOTbandsD;

            /** list of the used band for AOT Estimation for the date D-1 'reflectance' composite */
            std::vector<unsigned int> m_AOTbandsReflectanceCompositeDm1;

            /** list of the used band for AOT Estimation for the date D-1 'LUT' composite */
            std::vector<unsigned int> m_AOTbandsLUTCompositeDm1;

            /** Levenberg Marquardt Launcher */
            RunLMOptimizationPointerType m_RunLMOptimization;

            std::vector<RunLMOptimizationPointerType> m_VectorOptim;

            /** Vector of all used indexes considering the step StepAOT*/
            IndexVectType m_AllIndexes;

            /** Low TOA reflectance thresholds */
            DoubleVectType m_LowReflectanceThresholds;

            /** High TOA reflectance thresholds */
            DoubleVectType m_HighReflectanceThresholds;

            /** Map of LUT for LPD */
            LUTMapType m_LUTMap;

            double m_F_Tolerance; // Function value tolerance
            double m_G_Tolerance; // Gradient magnitude tolerance
            double m_X_Tolerance; // Search space tolerance
            double m_Epsilon_Function; // Step
            int m_Max_Iterations; // Maximum number of iterations
            double m_AOTinitialD;
            double m_AOTinitialDm1;

            //Boolean to activate CAMS
            bool m_UseCams;
            //error ponderation for CAMS
            double m_KPondCAMS;
            //Mean Altitude for CAMS
            double m_MeanAltitude;
            //Height Scale for CAMS computation
            double m_HeightScale;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMultiTemporalComputeAOTImageFilter.txx"
#endif

#endif /* vnsMultiTemporalComputeAOTImageFilter_h */
