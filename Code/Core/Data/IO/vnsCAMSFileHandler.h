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
 * VERSION : 3.2.1 : FA : LAIG-FA-MAJA-2861-CNES : 27 sept 2018 : Erreur dans la selection du netcdf        *
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction donnees des CAMS             *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$                                                                                                     *
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCAMSFileHandler_h
#define __vnsCAMSFileHandler_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include "vnsUtilities.h"
#include "itkLinearInterpolateImageFunction.h"


namespace vns
{
    /** \class  CAMSFileHandler
     * \brief This class implements the basic functionnalities to read a CAMS file.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa XMLFileHandler
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
    class CAMSFileHandler : public itk::Object
    {
    public:
        /** Typedef for Corners */
         typedef class Corner
         {
         public:
             double Longitude;
             double Latitude;
             int Line;
             int Column;
         } CornerType;
         /**Internal data types */
         typedef struct CAMSFile
         {
             std::string AOT_Filename; // Filename of the AOT data
             std::string MR_Filename; // Filename of the MR data
             std::string RH_Filename; // Filename of the RH data
             Utilities::ListOfDoubles ModelLevels; //Model levels
             unsigned int NbNoInterpolable; // Nb of non interpolable levels
             double JulianDayAquisition; // JulianDay of the CAMS Data
         } CAMSFileType;

        /** Standard class typedefs. */
        typedef CAMSFileHandler Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        typedef double PixelType;
        typedef otb::VectorImage<PixelType, 2> ImageType;
        typedef otb::Image<PixelType, 2> AOTImageType;
        typedef ImageType::PointType PointType;
        typedef ImageType::SizeType SizeType;
        typedef ImageType::RegionType RegionType;
        typedef ImageType::PixelType VectorPixel;

        /** Reader typedef. */
        typedef otb::ImageFileReader<ImageType> ReaderType;

        /** Interpolator typedef. */
        typedef itk::LinearInterpolateImageFunction<ImageType> InterpolatorType;
        typedef InterpolatorType::ContinuousIndexType ContinuousIndexType;

        /** Creation through object factory macro */
        itkTypeMacro(CAMSFileHandler, itk::Object )

        /** Type macro */
        itkNewMacro(Self)

        typedef struct ExtinctionValue
        {
            std::string description;
            std::vector<double> values;
            bool isRhDep;

            ExtinctionValue(const std::string& p_descr, const std::vector<double>& p_val,
                    const bool p_rhDep): description(p_descr),isRhDep(p_rhDep)
            {
                values = p_val;
            }

            ExtinctionValue(): description(""),isRhDep(false)
            {
            }
            bool operator==( const ExtinctionValue & n ) const {
              return this->description == n.description;   // for example
            }

        } ExtinctionValueType;

        typedef Utilities::ListOfStrings ListOfStrings;
        typedef Utilities::ListOfFilenames ListOfFilenames;
        typedef Utilities::ListOfDoubles ListOfDoubles;
        typedef Utilities::ListOfUIntegers ListOfUIntegers;
        typedef std::map<std::string, ExtinctionValue > ExtinctionMapType;
        typedef std::map<std::string, VectorPixel>  MRMapType;
        typedef std::map<std::string, VectorPixel>  AOTMapType;
        typedef std::map<std::string, double >  ProportionMapType;

        /** Set the input parameters **/
        //vnsSetMacro( ModelLevels , ListOfDoubles)
        vnsSetMacro( ExtinctionMap , ExtinctionMapType)
        vnsSetMacro( LimAOD , double)
        vnsSetMacro( RHTab , ListOfUIntegers)
        vnsSetMacro( RHSampling , ListOfUIntegers)
        vnsSetMacro( ListOfModels , ListOfStrings)

        /** get output **/
        vnsGetConstReferenceMacro(OutProportions, ProportionMapType)
        vnsGetConstReferenceMacro(OutSamplingRH, double)


		void SetBeforeCAMS(const std::string& pBeforeCamsAOT, const std::string& pBeforeCamsMR,
						   const std::string& pBeforeCamsRH,const ListOfDoubles pModelLevels,
						   const unsigned int pNbNonInterpol, const double pCAMSTimeJD)
        {
        	this->m_beforeFile.AOT_Filename = pBeforeCamsAOT;
        	this->m_beforeFile.MR_Filename = pBeforeCamsMR;
        	this->m_beforeFile.RH_Filename = pBeforeCamsRH;
        	this->m_beforeFile.JulianDayAquisition = pCAMSTimeJD;
        	this->m_beforeFile.ModelLevels = pModelLevels;
        	this->m_beforeFile.NbNoInterpolable = pNbNonInterpol;
        	this->m_beforeFileFound = true;
        }

        void SetAfterCAMS(const std::string& pAfterCamsAOT, const std::string& pAfterCamsMR,
				   	   const std::string& pAfterCamsRH,const ListOfDoubles pModelLevels,
					   const unsigned int pNbNonInterpol,const double pCAMSTimeJD)
        {
        	this->m_afterFile.AOT_Filename = pAfterCamsAOT;
        	this->m_afterFile.MR_Filename = pAfterCamsMR;
        	this->m_afterFile.RH_Filename = pAfterCamsRH;
        	this->m_afterFile.JulianDayAquisition = pCAMSTimeJD;
        	this->m_afterFile.ModelLevels = pModelLevels;
        	this->m_afterFile.NbNoInterpolable = pNbNonInterpol;
        	this->m_afterFileFound = true;
        }

		AOTImageType* GetAOTImage()
        {
        	return m_AOTImage.GetPointer();
        }

        /** Read the CAMS Data and extract the needed informations
         * */
        void ExtractCAMSData(const CornerType & p_CenterCorner,const double pImageProductAquisitionTimeJD);

        /**
         * Return either the data is valid,
         * any access to data when the valid flag is false will result in an exception
         */
        bool isValid() { return m_IsValid;};

    protected:
        /** Constructor */
        CAMSFileHandler();
        /** Destructor */
        virtual
        ~CAMSFileHandler();
        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

        enum Mode {
            BEFORE,
            AFTER
        } ;

        //Internal subroutines
        void SearchValidCAMSFilenames();
        void TreatOneFile(CAMSFileType const& p_CamsFiles, const CornerType & p_CenterCorner, const Mode& p_mode );
        /* Get the value of the cams file at CenterCorner */
        std::pair<std::string,VectorPixel> GetCAMSValueAndDescription(const std::string & filename, const CornerType & p_CenterCorner);
        /* Get the levels of values in the CAMS file */
        std::vector<double> GetCAMSLevels(const std::string & filename);
        /* Compute model proportions  */
        ProportionMapType ComputeModelsProportions(const AOTMapType& p_AOT);
        /* Compute the average RH */
        double ComputeAverageRH(const VectorPixel& p_RH, const MRMapType& p_MR,
        		const ListOfDoubles& p_pressureLevels, const ListOfDoubles& p_ModelLevels ,
				const unsigned int p_NbNonInterpolable);
        /* Get the closest sampling */
        double GetRHSampling(const double p_avgRH);
        /* Compute the total AOT of the column, from the mixing ratios, and save the AOT of each level */
        void ComputeAltitudeLimit(const MRMapType& p_MR, const ListOfDoubles& p_RHModels,const ListOfDoubles& p_ModelLevels);
        /* Temporal interpolation between after and before data*/
        void TemporalInterpolation();
        /* 1D interpolator as the one un python */
        template<typename T, typename P>
        double Interpolate1D(const T& p_indexes, const P& p_Values, const unsigned int p_ValuesSize, const double l_interIndex);
        /* is this dataset valid */
        bool isValidDataset(const std::string& filename);
        std::string GetModelFromDescription(const std::string& p_desc);
        //NETCDF Metadata utility functions
        double GetMetadataValueAsDouble(const ImageType* image, std::string const& name);
        CAMSFileHandler::ListOfStrings GetMetadataValueAsListOfString(const ImageType* image, std::string const& name);
        std::string GetMetadataValueAsString(const ImageType* image, std::string const& name);
        std::vector<double> GetMetadataValueAsDoubleVector(const ImageType* image, std::string const& name);
        //Compute the sum of aot for all models
        void ComputeSumOfAOT(const std::string & filename, const Mode& p_mode);


        //Cleanup function
        void cleanup();
        //Verify that input data has been filled
        void verifyData();

        //Constant on the MAXIMUM number of dataset possible in a netcdf
        static const unsigned int NETCDF_MAX_DATASET_NUMBER;

        //CAMS Filename
        CAMSFileType m_beforeFile;
        bool m_beforeFileFound;
        CAMSFileType m_afterFile;
        bool m_afterFileFound;

        //Validity flag
        bool m_IsValid;
        double m_ImageTimeAquisitionJD;

        //Internal algo data
        ListOfDoubles m_AOTml; //AOT of the model level
        double m_altLimit; //limit altitude
        unsigned int m_indLimit; //Index of the limit altitude
        AOTMapType m_beforeAOTs; //AOT for before file
        ProportionMapType m_beforeProps; //Model proportions for before file
        double m_beforeSamplingRH; //RH sampling for the before file
        double m_beforeAvgRH; //Average RH for the before file
        AOTMapType m_afterAOTs; //AOT for before file
        ProportionMapType m_afterProps; //Model proportions for before file
        double m_afterSamplingRH; //RH sampling for the before file
        double m_afterAvgRH; //Average RH for the before file

        //Algo params
        ExtinctionMapType m_ExtinctionMap;
        double m_LimAOD;
        ListOfUIntegers m_RHTab;
        ListOfUIntegers m_RHSampling;
        ListOfStrings m_ListOfModels;

        //Output
        ProportionMapType m_OutProportions;
        double m_OutSamplingRH; //RH sampling for the before file

        //Image of sum of AOT of all model
        AOTImageType::Pointer m_AOTBeforeImage;
        AOTImageType::Pointer m_AOTAfterImage;
        AOTImageType::Pointer m_AOTImage;


    private:
        CAMSFileHandler(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented


    };

} // End namespace vns

#endif /* __vnsCAMSFileHandler_h */
