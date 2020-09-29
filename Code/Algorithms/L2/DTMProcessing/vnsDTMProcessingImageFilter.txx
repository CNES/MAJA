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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-2168-CNES : 28 fevrier 2017 : Correction I3D car sortie en erreur    *
 *                                                                                         boite englobante *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 13 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 8 juillet 2016 : Audit code - suppression de macros ID3   *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 1-0-0-3 : DM : 357 : 20 juil. 2011 : Define vectors and use their norm and scalar product        *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 27 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDTMProcessingImageFilter_txx
#define __vnsDTMProcessingImageFilter_txx

#include "vnsDTMProcessingImageFilter.h"
#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsUtilities.h"

#include "itkVector.h"
#include "vnsDEMDataManager.h"

#include <iomanip>

namespace vns
{

    // Constructor
    template<class TGRIDImage, class TDTMImage, class TInputMask, class TOutputImage>
        DTMProcessingImageFilter<TGRIDImage, TDTMImage, TInputMask, TOutputImage>::DTMProcessingImageFilter() :
                m_L2CoarseResolution(1), m_VIEHRef(0.), m_SOLHRef(0.), m_DistanceThreshold(0.)
        {

            // Input/Output initialization
            this->SetNumberOfIndexedInputs(4);
            this->SetNumberOfRequiredInputs(4);
            this->SetNumberOfRequiredOutputs(2);
            this->SetNumberOfIndexedOutputs(2);

            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, OutputImageType::New());

            m_I3D = NULL;

        }

    // Destructor
    template<class TGRIDImage, class TDTMImage, class TInputMask, class TOutputImage>
        DTMProcessingImageFilter<TGRIDImage, TDTMImage, TInputMask, TOutputImage>::~DTMProcessingImageFilter()
        {
        }

    //====================== Main computation method ======================
    template<class TGRIDImage, class TDTMImage, class TInputMask, class TOutputImage>
        void
        DTMProcessingImageFilter<TGRIDImage, TDTMImage, TInputMask, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            PointType l_GroundPoint;
            PointType l_ProjPoint;
            PointType VIEPoint;
            PointType SOLPoint;

            VectorType l_Vector;
            l_Vector.Fill(0);
            VectorType l_DV;
            l_DV.Fill(0);

            VectorType l_VectorSOL;
            l_VectorSOL.Fill(0);
            VectorType l_DS;
            l_VectorSOL.Fill(0);

            // Get pointers to input images
            DTMImageConstPointer l_DTMPtr = this->GetDTMInput();
            GRIDImageConstPointer l_IPSOLPtr = this->GetIPSOLInput();
            GRIDImageConstPointer l_IPVIEPtr = this->GetIPVIEInput();
            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();

            // Get pointers to output images
            OutputImagePointer l_IPSHDPtr = this->GetIPSHDOutput();
            OutputImagePointer l_IPHIDPtr = this->GetIPHIDOutput();

            // Define the input image iterators
            itk::ImageRegionConstIterator<DTMImageType> l_DTMIt(l_DTMPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<GRIDImageType> l_IPSOLIt(l_IPSOLPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<GRIDImageType> l_IPVIEIt(l_IPVIEPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> l_IPEDGIt(l_IPEDGPtr, outputRegionForThread);

            // Define the output image iterators
            itk::ImageRegionIterator<OutputImageType> l_IPSHDIt(l_IPSHDPtr, outputRegionForThread);
            itk::ImageRegionIterator<OutputImageType> l_IPHIDIt(l_IPHIDPtr, outputRegionForThread);

            l_DTMIt.GoToBegin();
            l_IPSOLIt.GoToBegin();
            l_IPVIEIt.GoToBegin();
            l_IPEDGIt.GoToBegin();
            l_IPSHDIt.GoToBegin();
            l_IPHIDIt.GoToBegin();

            //Get number of viewing bands
            const unsigned int l_nbBands = static_cast<unsigned int>(l_IPVIEPtr->GetNumberOfComponentsPerPixel() / 2);

            // Pixel loop
            while (l_IPVIEIt.IsAtEnd() == false)
            {

                // Initialization of the output masks
                l_IPSHDIt.Set(static_cast<OutputImagePixelType>(0));
                l_IPHIDIt.Set(static_cast<OutputImagePixelType>(0));

                // If the pixel is not an edge or a no_data
                if (l_IPEDGIt.Get() == 0)
                {

                    for (unsigned int i = 0; i < l_nbBands; i++)
                    {
                        // Determination of ground coordinates of the pixel
                        l_DTMPtr->TransformIndexToPhysicalPoint(l_DTMIt.GetIndex(), l_GroundPoint);

                        // Compute distance between the pixel and the intersection of the viewing
                        // directions with earth surface
                        // Two bands (X,Y) for each viewing spectral band
                        VIEPoint[0] = l_IPVIEIt.Get()[2 * i];
                        VIEPoint[1] = l_IPVIEIt.Get()[2 * i + 1];
                        l_Vector = ComputeDTMIntersection(l_GroundPoint, VIEPoint, l_DTMIt, m_VIEHRef,
                                "ViewingForBandIndex" + Utilities::AsString(i));

                        l_DV[0] = VIEPoint[0]; //l_IPVIEIt.Get()[2 * i];
                        l_DV[1] = VIEPoint[1]; //l_IPVIEIt.Get()[2 * i + 1];

                        if ((l_Vector.GetNorm() > m_DistanceThreshold) && (l_Vector * l_DV > 0))
                        {
                            l_IPHIDIt.Set(static_cast<OutputImagePixelType>(1));
                        }
                    }

                    // Compute distance between the pixel and the intersection of the solar
                    // direction with earth surface
                    SOLPoint[0] = l_IPSOLIt.Get()[0];
                    SOLPoint[1] = l_IPSOLIt.Get()[1];
                    l_VectorSOL = ComputeDTMIntersection(l_GroundPoint, SOLPoint, l_DTMIt, m_SOLHRef, "Solar");

                    l_DS[0] = SOLPoint[0]; //l_IPSOLIt.Get()[0];
                    l_DS[1] = SOLPoint[1]; //l_IPSOLIt.Get()[1];

                    if ((l_VectorSOL.GetNorm() > m_DistanceThreshold) && (l_VectorSOL * l_DS > 0))
                    {
                        l_IPSHDIt.Set(static_cast<OutputImagePixelType>(1));
                    }
                }

                ++l_DTMIt;
                ++l_IPSOLIt;
                ++l_IPVIEIt;
                ++l_IPEDGIt;
                ++l_IPSHDIt;
                ++l_IPHIDIt;

            } // end pixel loop

        }

    // ================== Compute the intersection of a direction with earth surface  ============
    template<class TGRIDImage, class TDTMImage, class TInputMask, class TOutputImage>
        typename DTMProcessingImageFilter<TGRIDImage, TDTMImage, TInputMask, TOutputImage>::VectorType
        DTMProcessingImageFilter<TGRIDImage, TDTMImage, TInputMask, TOutputImage>::ComputeDTMIntersection(const PointType & p_GroundPoint,
                const PointType & p_IPGRID, const DTMConstItType & p_DTMIt, const double p_GRIDHRef, const std::string & comment)
        {

            PointType l_IPGRIDGroundPoint;
            l_IPGRIDGroundPoint.Fill(0);
            PointType l_Point;
            l_Point.Fill(0);
            PointType l_ProjPoint;
            l_ProjPoint.Fill(0);

            // Init vector
            std::vector<double> l_PositionVector(3);
            std::vector<double> l_GRIDVector(3);
            std::vector<double> l_ProjVector(3);

            // Altitude of the pixel
            const double l_DTMValue = static_cast<double>(p_DTMIt.Get());

            // IPVIE is a vector => the origin is not taken into account
            l_IPGRIDGroundPoint[0] = p_IPGRID[0];
            l_IPGRIDGroundPoint[1] = p_IPGRID[1];

            // For Hidden faces, compute index of the satellite position
            if (vnsEqualsDoubleMacro(p_GRIDHRef,0) == true)
            {
                vnsExceptionBusinessMacro("The reference altitude for viewing direction in band B5 or B6 is null !");
            }

            // Computes point
            l_Point[0] = p_GroundPoint[0] + (p_GRIDHRef - l_DTMValue) * l_IPGRIDGroundPoint[0] / p_GRIDHRef;
            l_Point[1] = p_GroundPoint[1] + (p_GRIDHRef - l_DTMValue) * l_IPGRIDGroundPoint[1] / p_GRIDHRef;

            // Determination of the satellite position vector or the sun one
            l_PositionVector[0] = static_cast<double>(l_Point[0]);
            l_PositionVector[1] = static_cast<double>(l_Point[1]);
            l_PositionVector[2] = static_cast<double>(p_GRIDHRef);

            l_GRIDVector[0] = static_cast<double>(-l_IPGRIDGroundPoint[0]);
            l_GRIDVector[1] = static_cast<double>(-l_IPGRIDGroundPoint[1]);
            l_GRIDVector[2] = static_cast<double>(-p_GRIDHRef);

            // Call the utilities Intersection3D
            const bool l_Valid = m_I3D->LaunchesIntersection3D("DTMProcessingImageFilter for '" + comment + "'", l_PositionVector,
                    l_GRIDVector, l_ProjVector, false /* pWithoutError */, __FILE__, __FUNCTION__, __LINE__);

            // Valid point
            VectorType l_Vector;
            if (l_Valid == true)
            {
                l_ProjPoint[0] = l_ProjVector[0];
                l_ProjPoint[1] = l_ProjVector[1];
                l_Vector[0] = l_ProjPoint[0] - p_GroundPoint[0];
                l_Vector[1] = l_ProjPoint[1] - p_GroundPoint[1];
            }
            else
            {
                std::ostringstream message;
                message << comment << " - call DTMProcessingImageFilter \n"
                        "with input parameters:\n" << std::setprecision(30) << "   Input p_GroundPoint: [" << p_GroundPoint[0] << ", "
                        << p_GroundPoint[1] << "]\n"
                                "   Input p_IPGRID:      [" << p_IPGRID[0] << ", " << p_IPGRID[1] << "]\n"
                                "   Input l_DTMValue:    [" << l_DTMValue << "]\n"
                                "   Input l_DTMIndex:    " << p_DTMIt.GetIndex() << "\n"
                                "   Computed l_Point:    [" << l_Point[0] << ", " << l_Point[1] << "]\n"
                                "   l_PositionVector:    [" << l_PositionVector[0] << ", " << l_PositionVector[1] << ", "
                        << l_PositionVector[2] << "]\n"
                                "   l_GRIDVector:        [" << l_GRIDVector[0] << ", " << l_GRIDVector[1] << ", " << l_GRIDVector[2]
                        << "]\n"
                                "output values:\n" << "   l_ProjVector:        [" << l_ProjVector[0] << ", " << l_ProjVector[1] << ", "
                        << l_ProjVector[2] << "]\n"
                                "   return code valid    " << Utilities::BoolToString(l_Valid) << "\n";
                vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::SUPER_DEBUG,
                         __FILE__, __FUNCTION__, __LINE__);

                // Set the Vector to 0
                l_Vector.Fill(0);
            }
            return l_Vector;
        }

    // PrintSelf method
    template<class TGRIDImage, class TDTMImage, class TInputMask, class TOutputImage>
        void
        DTMProcessingImageFilter<TGRIDImage, TDTMImage, TInputMask, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsDTMProcessingImageFilter_txx */
