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
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 11 février 2014 : Améliorations algorithmiques (Spec v2.1)	*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsMultiTemporalAOTMethodThresholdsGenerator.h 2406 2011-01-19 15:38:23Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMultiTemporalAOTMethodThresholdsGenerator_h
#define __vnsMultiTemporalAOTMethodThresholdsGenerator_h

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "otbMath.h"

#include "vnsMacro.h"
#include "vnsVectorLookUpTable.h"

namespace vns
{
/** \class  MultiTemporalAOTMethodThresholdsGenerator
 * \brief This class calculates two TOA reflectance thresholds used in the multi temporal
 * selection of the pixels for the local inversion.
 *
 * Because the impact of the AOT is not quantifiable in the reflectance value for a specific range
 * of reflectances, those reflectances are removed while the multi temporal selection of the pixels
 * for the local inversion is performed.
 * 
 * \author CS Systemes d'Information
 *
 * \sa Object
 *
 * \ingroup L2
 *
 */
template < class TInputLUT = vns::VectorLookUpTable<double, 3> >
class MultiTemporalAOTMethodThresholdsGenerator : public itk::Object
{
public:
    /** Standard class typedefs. */
    typedef MultiTemporalAOTMethodThresholdsGenerator                                Self;
    typedef itk::Object                                            Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    typedef TInputLUT                                       LUTType;
	typedef typename LUTType::Pointer                       LUTPointerType;
	typedef typename LUTType::PixelType                     LUTPixelType;
	typedef typename LUTType::PointType           			LUTPointType;
	typedef typename LUTPointType::ValueType     			LUTPointValueType;

	typedef std::vector<unsigned int>                       UIVectType;
	typedef std::vector<double>                      		DoubleVectType;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(MultiTemporalAOTMethodThresholdsGenerator, Object );

    /**  Set AOT band codes. */
	vnsSetMacro(MTAOTBands, UIVectType)
	/**  Get AOT band codes. */
	vnsGetConstMacro(MTAOTBands, UIVectType)

    /**  Set the minimum AOT value. */
    vnsSetMacro(FirstAOT, double)
    /**  Get the minimum AOT value. */
    vnsGetConstMacro(FirstAOT, double)

    /**  Set the maximum AOT value. */
	vnsSetMacro(SecondAOT, double)
	/**  Get the maximum AOT value. */
	vnsGetConstMacro(SecondAOT, double)

	/**  Set the minimum TOA reflectance value. */
	vnsSetMacro(TOAReflMin, double)
	/**  Get the minimum TOA reflectance value. */
	vnsGetConstMacro(TOAReflMin, double)

	/**  Set the maximum TOA reflectance value. */
	vnsSetMacro(TOAReflMax, double)
	/**  Get the maximum TOA reflectance value. */
	vnsGetConstMacro(TOAReflMax, double)

	/**  Set the TOA reflectance step value. */
	vnsSetMacro(TOAReflStep, double)
	/**  Get the TOA reflectance step value. */
	vnsGetConstMacro(TOAReflStep, double)

	/** Set the mean altitude of the site. */
	vnsSetMacro(AltitudeMean, double)
	/**  Get the mean altitude of the site. */
	vnsGetConstMacro(AltitudeMean, double)

	/** Set the minimum difference threshold between the surface
	 * reflectances computed for extreme AOT values. */
	vnsSetMacro(MinDiffThreholds, double)
	/**  Get the mean altitude of the site. */
	vnsGetConstMacro(MinDiffThreholds, double)

	/** Set the LUT image */
	itkSetObjectMacro(LUT, LUTType)
	/** Get the DTM image */
	itkGetConstObjectMacro(LUT, LUTType)

	/** Low TOA reflectance threshold */
	vnsGetConstMacro(LowReflectanceThreshold, DoubleVectType)
	/** High TOA reflectance threshold */
	vnsGetConstMacro(HighReflectanceThreshold, DoubleVectType)

    void ComputeReflectanceThresholds();


protected:
    /** Constructor */
    MultiTemporalAOTMethodThresholdsGenerator();
    /** Destructor */
    virtual ~MultiTemporalAOTMethodThresholdsGenerator();

    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
    MultiTemporalAOTMethodThresholdsGenerator(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /** AOT band codes. */
    UIVectType m_MTAOTBands;
    /**  Minimum AOT value used to calculate the first range of reflectances. */
    double m_FirstAOT;
    /**  Maximum AOT value used to calculate the second range of reflectances. */
    double m_SecondAOT;
    /**  Minimum TOA reflectance value used to calculate the first range of reflectances. */
	double m_TOAReflMin;
	/**  Maximum TOA reflectance value used to calculate the second range of reflectances. */
	double m_TOAReflMax;
	/**  TOA reflectance step value used to compute the range of reflectances. */
	double m_TOAReflStep;
	/** Mean altitude of the site */
	double m_AltitudeMean;
	/** Minimum difference threshold between the surface reflectances computed for extreme AOT values. */
	double m_MinDiffThreholds;

	/** Surface reflectance LUT */
	LUTPointerType m_LUT;

	/** Low TOA reflectance threshold */
	DoubleVectType m_LowReflectanceThreshold;
	/** High TOA reflectance threshold */
	DoubleVectType m_HighReflectanceThreshold;

};

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMultiTemporalAOTMethodThresholdsGenerator.txx"
#endif

#endif /* __vnsMultiTemporalAOTMethodThresholdsGenerator_h */
