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
 *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction données des CAMS             *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-981-CNES : 2 mars 2017 : ReducedLutComputation extracted as new algo *
 * VERSION : 2.0.0 : <TypeFT> : <NumFT> : 2 mars. 2017 : Creation                                           *                                                                                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsReducedLutComputationFilter_h
#define __vnsReducedLutComputationFilter_h

#include "itkProcessObject.h"
#include "vnsVectorLookUpTable.h"
#include "vnsVectorLookUpTableExtractor.h"
#include "vnsMultiplyByVectorFunctor.h"
#include "itkImageScanlineIterator.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "vnsDirectionalCorrectionCompute.h"
#include "itkUnaryFunctorImageFilter.h"
#include "vnsCAMSFileHandler.h"
#include "vnsNaryApplyProportionsFunctor.h"
#include "vnsNaryFunctorImageFilter.h"

namespace vns
{
    /** \class  ReducedLutComputationFilter
     * \brief This class is used to genrate a reduced 3D lut from a 6D lut.
     *
     * The low resolution Lut is always generated and use mean viewing angles.
     * If the sensor has a wide viewing field than a high resolution lut
     * is also generated per viewing zone gien in the input viewing angles map.
     * A Directional correction can also be applied on the low resolution map
     * to correct the direction to simulate view from the Ref Angles.
     *
     * \author CS Systemes d'Information
     *
     * \sa ProcessObject
     *
     * \ingroup L2
     *
     */
    //template<class TInputImage, class TOutputImage>
    class ReducedLutComputationFilter : public itk::ProcessObject
    {
    public:

        /** CR index constant for other filter exploiting the map */
        const static unsigned int CR_LUT_INDEX;

        /** Standard class typedefs. */
        typedef ReducedLutComputationFilter Self;
        typedef itk::ProcessObject Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)

        /** Creation through object factory macro */
        itkTypeMacro(ReducedLutComputationFilter, ProcessObject )

        /** Some convenient typedefs. */
        //typedef TInputLut InputLutType;
        //Reduced output lut typedef
        typedef VNSLUT3DType ReducedLutType;
        typedef ReducedLutType::ConstPointer ReducedLutConstPointer;
        typedef ReducedLutType::Pointer ReducedLutPointer;
        typedef ReducedLutType::RegionType ReducedRegionType;
        typedef ReducedLutType::PixelType ReducedLutPixelType;
        typedef ReducedLutType::SizeType ReducedLutSizeType;
        typedef ReducedLutType::InternalPixelType ReducedLutInternalPixelType;
        typedef std::map<unsigned int, ReducedLutPointer> ReducedLutMapType;
        //Model LUT
        typedef std::map<std::string, VNSLUT6DType::Pointer> Model6DLutMapType;
        typedef std::map<std::string, VNSLUT3DType::Pointer> Model3DLutMapType;
        typedef std::map<std::string, VNSLUT2DType::Pointer> Model2DLutMapType;
        //Full dimension LUT typedef
        typedef VNSLUT6DType LutType;
        typedef LutType::ConstPointer LutConstPointer;
        typedef LutType::Pointer LutPointer;
        typedef LutType::RegionType RegionType;
        typedef LutType::PixelType LutPixelType;
        typedef LutType::SizeType LutSizeType;
        typedef LutType::InternalPixelType LutInternalPixelType;
        //Proportions typedef
        typedef CAMSFileHandler::ProportionMapType ProportionMapType;
        //Proportion apply functor
        typedef vns::Functor::NaryApplyProportionsFunctor<VNSLUT3DType::PixelType, VNSLUT3DType::PixelType> Lut3DProportionsFunctorType;
        typedef vns::NaryFunctorImageFilter<VNSLUT3DType, VNSLUT3DType, Lut3DProportionsFunctorType> Lut3DProportionsFilterType;
        typedef Lut3DProportionsFilterType::Pointer Lut3DProportionsFilterPointerType;
        typedef vns::Functor::NaryApplyProportionsFunctor<VNSLUT2DType::PixelType, VNSLUT2DType::PixelType> Lut2DProportionsFunctorType;
        typedef vns::NaryFunctorImageFilter<VNSLUT2DType, VNSLUT2DType, Lut3DProportionsFunctorType> Lut2DProportionsFilterType;
        typedef Lut2DProportionsFilterType::Pointer Lut2DProportionsFilterPointerType;

        //Input Angles typedef
        typedef std::map<unsigned int, std::vector<double> > ViewingAngleMapType;
        typedef Utilities::ListOfDoubles ListOfDoubles;
        typedef Utilities::ListOfStrings ListOfStrings;


        //Extract LUT filter
        typedef VectorLookUpTableExtractor<LutType,ReducedLutType> VectorLookUpTableExtractorType;

        //Multiply functor
        typedef vns::Functor::MultiplyByVectorFunctor<ReducedLutType::PixelType, ReducedLutType::PixelType> FunctorType;
        typedef itk::UnaryFunctorImageFilter<ReducedLutType, ReducedLutType, FunctorType> MultiplyByVectorFilterType;
        typedef MultiplyByVectorFilterType::Pointer MultiplyByVectorFilterPointerType;


        //Directional correction compute
        typedef DirectionalCorrectionCompute DirectionalCorrectionComputeType;

        /** Parameters definition */
        typedef FunctorType::CoefsVectorType CoefsVectorType;

        /** Set the input full dimension lut **/
        vnsSetGetInputMacro( LUT , LutType, 0 )
        /** Set the model input TOCR **/
        vnsSetMacro( TOCR , Model6DLutMapType)
        /** Set the model input TDIR **/
        vnsSetMacro( TDIR , Model3DLutMapType)
        /** Set the model input TDIF **/
        vnsSetMacro( TDIF , Model3DLutMapType)
        /** Set the model input ALB **/
        vnsSetMacro( ALB , Model2DLutMapType)
        /** Set the input viewingZenith angles map for high resolution **/
        vnsSetMacro( ViewingZenithAnglesMap , ViewingAngleMapType)
        /** Set the input viewingAzimuth angles map for high resolution **/
        vnsSetMacro( ViewingAzimuthAnglesMap , ViewingAngleMapType)
        /** Set the input mean viewingZenith angle for low resolution lut **/
        vnsSetMacro( MeanViewingZenithAngles , ListOfDoubles)
        /** Set the input mean viewingAzimuth angle for low resolution lut **/
        vnsSetMacro( MeanViewingAzimuthAngles , ListOfDoubles)

        /** Set the input direction correction R coefficients **/
        vnsSetMacro( CoefsR, CoefsVectorType)
        /** Set the input direction correction V coefficients **/
        vnsSetMacro( CoefsV, CoefsVectorType)

        // Solar zenith angle
        vnsSetMacro(SolarZenith, double)
        // Solar azimuth angle
        vnsSetMacro(SolarAzimuth, double)

        // Ref zenith angle
        vnsSetMacro(RefZenith, double)
        // Ref azimuth angle
        vnsSetMacro(RefAzimuth, double)

        //Models proportion
        vnsSetMacro(Proportions,ProportionMapType)
        //Use cams data
        vnsSetMacro(UseCamsData,bool)
        vnsSetMacro(ModelList,ListOfStrings)

        /** Get the Map of LUT per zone */
        itkGetConstReferenceMacro( LUTMapOutput, ReducedLutMapType)

        /** Get the TDIR Lut **/
        itkGetConstReferenceMacro( TDIROutput, VNSLUT3DType::Pointer)

        /** Get the TDIF Lut **/
        itkGetConstReferenceMacro( TDIFOutput, VNSLUT3DType::Pointer)

        /** Get the TDIR Lut **/
        itkGetConstReferenceMacro( ALBOutput, VNSLUT2DType::Pointer)

        /** Is the sensor wide with multiple viewing Angles (SENTINEL-2*/
        vnsBooleanMemberAndSetAndGetConstReferenceMacro(WideFieldSensor)

        /** Enable directional correction */
        vnsBooleanMemberAndSetAndGetConstReferenceMacro(DirectionalCorrection)

        /** Acessor to the CR Lut*/
        const ReducedLutPointer& GetCRLut();

        /** Acessor to the CR Lut not corrected with directional*/
        const ReducedLutPointer& GetCRLutNotCorrected();

        /** Main computation method. */
        virtual void
        UpdateWithCaching(void);

    protected:
        /** Constructor */
        ReducedLutComputationFilter();
        /** Destructor */
        virtual
        ~ReducedLutComputationFilter()
        {
        }

        template <class T>
        std::string GetKeyForModel(const T& p_map, const std::string& p_model) const;


        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
        ReducedLutComputationFilter(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented
        ReducedLutMapType m_LUTMapOutput;
        ReducedLutPointer m_CRLutNotCorrected;
        ReducedLutPointer m_CRLutCorrected;
        VNSLUT3DType::Pointer m_TDIROutput;
        VNSLUT3DType::Pointer m_TDIFOutput;
        VNSLUT2DType::Pointer m_ALBOutput;
        MultiplyByVectorFilterPointerType m_MultiplyFilter;
        //Solar Angles
        double m_SolarZenith;
        double m_SolarAzimuth;
        //Ref Angles
        double m_RefZenith;
        double m_RefAzimuth;
        //VViewing Angles per band and detector for sensor that have them
        ViewingAngleMapType m_ViewingZenithAnglesMap;
        ViewingAngleMapType m_ViewingAzimuthAnglesMap;
        //VViewing Angles
        ListOfDoubles m_MeanViewingZenithAngles;
        ListOfDoubles m_MeanViewingAzimuthAngles;
        //Correction coeff
        CoefsVectorType m_CoefsR;
        CoefsVectorType m_CoefsV;
        //Model Proportions params
        Model6DLutMapType m_TOCR;
        Model3DLutMapType m_TDIR;
        Model3DLutMapType m_TDIF;
        Model2DLutMapType m_ALB;
        ProportionMapType m_Proportions;
        bool m_UseCamsData;
        ListOfStrings m_ModelList;
        //Model proportions filter
        Lut3DProportionsFilterPointerType m_TDIRProportionApplyFilter;
        Lut3DProportionsFilterPointerType m_TDIFProportionApplyFilter;
        Lut2DProportionsFilterPointerType m_ALBProportionApplyFilter;
        std::map<unsigned int, Lut3DProportionsFilterPointerType> m_TOCRWideProportionApplyFilters;
        //Internal data
        Lut3DProportionsFilterPointerType m_TOCRProportionApplyFilter;
        std::vector<ReducedLutPointer> m_InternalModelReducedLuts;
        std::vector<ReducedLutMapType> m_InternalModelLUTMaps;

    };

} // End namespace vns

#endif /* __vnsCloudMaskingImageFilter_h */
