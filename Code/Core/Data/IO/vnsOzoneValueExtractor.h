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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1608-CNES : 7 avril 2016 : correction la valeur supp a 360 - spacing  *
 * VERSION : 4-2-0 : FA : LAIG-FA-MAC-1222-CNES : 25 avril 2015 : gestion des valeurs aux bords             *
 *                                                                            (ex : longigute = 359 degrés) *
 * VERSION : 3-1-1 : FA : LAIG-FA-MAC-708-CNES : 7 novembre 2013 : déplacement de la classe depuis Algorithms*
 * 																   vers DATA/IO								*
 * VERSION : 1-0-0-3 : FA : 314 : 13 septembre 2011 : Correction anomalie sur une longitude negative.         *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsOzoneValueExtractor_h
#define __vnsOzoneValueExtractor_h

#include "otbImageFileReader.h"
#include "vnsGRIBImageIO.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbImage.h"
#include "vnsMacro.h"

namespace vns
{

    /** \class OzoneValueExtractor
     * \brief This class extract an Ozone value from a GRIB image.
     *
     * The compute this, we use the GRIB format characteristics that allows only two image types.
     * \begin{itemize}
     * \item size : 361*720, spacing is 0.5°.
     * \item size : 121*240, spacing is 1.5°.
     * \end{itemize}
     * In both cases, upper left corner has the coordinates (90° north in latitude, 0° in longitude) and lower right corner
     * has the coordinates (-90° north in latitude, 359.5° in longitude)
     *
     *
     * Inputs are:
     * \begin{itemize}
     * \item the GRIB file path,
     * \item the cordinates of the point where the value is wanted (latitude, longitude).
     * \end{itemize}
     * \item Output : The OzoneValue.
     * \end{itemize}
     *
     * \ingroup Radiometry
     *
     */

    class OzoneValueExtractor
    {
    public:

        /** Image typedefs. */
        typedef float PixelType;
        typedef double OzoneValueType;
        typedef otb::Image<PixelType, 2> ImageType;
        typedef ImageType::PointType PointType;
        typedef ImageType::SizeType SizeType;
        typedef ImageType::RegionType RegionType;

        /** Reader typedef. */
        typedef otb::ImageFileReader<ImageType> ReaderType;

        /** GRIB image io typedef. */
        typedef GRIBImageIO::Pointer GRIBImageIOPointerType;

        /** Interpolator typedef. */
        typedef itk::LinearInterpolateImageFunction<ImageType> InterpolatroType;
        typedef InterpolatroType::ContinuousIndexType ContinuousIndexType;

        /** Static method to compute ozone. Filename is the GRIB image path, point is the coordinates to get the ozone value.
         * point is in latitude, longitude. Latitude has to be in [-90°, 90°] and longitude to [0°, 360°]
         */
        static OzoneValueType
        GetOzoneValue(const std::string & filename, const PointType & point)
        {
            //Copy point coordinate
            PointType l_point(point);
            // Check latitude point value
            if ((l_point[1] < -90.) || (l_point[1] > 90))
            {
                vnsStaticExceptionBusinessMacro("Error in OzoneValueExtractor:\nInvalid latitude point "<<l_point[1]<<". Has to be in [-90, 90]");
            }
            SizeType l_LittleSize;
            SizeType l_BigSize;
            PointType l_Origin;
            l_BigSize[0] = 720; // Long. size
            l_BigSize[1] = 361; // Lat. size
            l_LittleSize[0] = 240; // Long. size
            l_LittleSize[1] = 121;// Lat. size
            l_Origin[0] = 0.; // Ref Long. origin
            l_Origin[1] = -90.;// Ref Lat. origin

            ReaderType::Pointer reader = ReaderType::New();
            reader->SetFileName(filename);
            reader->Update();

            // Store image region
            RegionType region = reader->GetOutput()->GetLargestPossibleRegion();
            // Store image size
            SizeType size = region.GetSize();

            PointType orig;
            double curSpacing = 0.;
            if (size == l_BigSize)
            {
                curSpacing = 0.5;
            }
            else if (size == l_LittleSize)
            {
                curSpacing = 1.5;
            }

            else
            {
                vnsStaticExceptionBusinessMacro("Error in OzoneValueExtractor:\nInvalid GRIB image size "<<size<<".");
            }
            // Check longitude point value
            // if  longitude point value is in [-180 ; 0], add 360 degrees.
            if (l_point[0] < 0.)
            {
                l_point[0] = l_point[0] + 360.;
                vnsLogDebugMacro("OzoneValueExtractor: Add 360 degrees for longitude value (for adapting value in [0, "<<360.-curSpacing<<"].")
            }

            // LAIG-FA-MAC-1222-CNES
            if ((l_point[0] < 0.) || (l_point[0] >= 360. /*. - curSpacing*/))
            {
                vnsStaticExceptionBusinessMacro("Error in OzoneValueExtractor:\nInvalid longitude point "<<l_point[0]<<". Has to be in [0, "<<360/*.-curSpacing*/<<"[");
            }

            // LAIG-FA-MAC-1608-CNES
            // Cas ou l_point[0] = 359.6...
            if (l_point[0] >= 360. - curSpacing)
            {
                l_point[0] = 360. - curSpacing;
            }

            // Compute corresponding image index
            const double ULXDistance = (-l_Origin[0] + l_point[0]) / curSpacing;
            const double ULYDistance = (-l_Origin[1] + l_point[1]) / curSpacing;

            ContinuousIndexType contId;
            contId[0] = ULXDistance;
            contId[1] = ULYDistance;

            if (region.IsInside(contId) == false)
            {
                vnsLogDebugMacro("Input point l_point = "<<l_point<<", l_Origin = "<<l_Origin<<", curSpacing = "<<curSpacing<<" => Computed continuous point "<<contId<<".");
                vnsStaticExceptionBusinessMacro("Error in OzoneValueExtractor:\nComputed continuous point "<<contId<<" from the input point "<<l_point<<" is outside the largest possible region "<<region<<".");
            }
            InterpolatroType::Pointer interp = InterpolatroType::New();
            interp->SetInputImage(reader->GetOutput());
            OzoneValueType ozone = static_cast<OzoneValueType> (interp->EvaluateAtContinuousIndex(contId));

            return ozone;
        }

    protected:
        OzoneValueExtractor()
        {
        }
        virtual
        ~OzoneValueExtractor()
        {
        }

    };

} // end namespace itk

#endif

