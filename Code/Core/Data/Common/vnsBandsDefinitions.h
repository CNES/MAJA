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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring pour ameliorer la qualite    *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 4-4-0 : DM : LAIG-DM-MAC-1283-CNES : 5 juin 2015 : Creation plug-in Sentinel2 natif            *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 3-1-0 : FA : LAIG-FA-MAC-114873-CS : 25 octobre 2013 : Methode GetListOfBandCode renommée en   *
 *                                                                  GetListOfL2BandCode                     *
 * VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 10 octobre 2013 : Creation du plugin LANDSAT_MUSCATE_PROTO 5 et 7        *
 * 									ajout d'une méthode GetListOfL1Band utilisée par les CheckTools			*
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-354-CNES : 8 juin 2012 : Correction qualite (regle ou metrique)       *
 *                                                                                                          *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 18 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0  : 17 oct. 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsBandsDefinitions.h 9781 2015-04-01 14:23:51Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsBandsDefinitions_h
#define __vnsBandsDefinitions_h

#include "itkSize.h"
#include "vnsMacro.h"
#include "vnsUtilities.h"
#include "vnsLoggers.h"
#include "itkNumericTraits.h"
#include <iostream>
#include <iomanip>      // std::setprecision

namespace vns
{
    /** \class  BandsDefinitions
     * \brief This class implements .
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa Object
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
    class BandsDefinitions
    {
    public:
        /** Standard class typedefs. */
        typedef BandsDefinitions Self;
        typedef itk::Size<2> SizeType;
        typedef std::vector<SizeType> ListOfSizeType;
        typedef SizeType::SizeValueType SizeValueType;
        typedef itk::Vector<double, 2> SpacingType;
        typedef std::vector<SpacingType> ListOfSpacingType;
        typedef SpacingType::ValueType SpacingValueType;
        typedef Utilities::ListOfUIntegers ListOfUIntegers;
        typedef Utilities::ListOfStrings ListOfStrings;
        typedef std::map<std::string, unsigned int> MapOfUInteger;

        /** Constructor */
        BandsDefinitions()
        {
            m_ListOfL1Resolution.clear();
            m_ListOfL2Resolution.clear();
            m_ReferenceResolution = "R1";
        }

        //single resolution informations struct
        struct SingleResolutionImageInfo
        {
            unsigned int L1Resolution;
            ListOfStrings ListOfBandCode;
        };
        typedef struct SingleResolutionImageInfo SRImageInfoType;

        typedef std::map<std::string, SRImageInfoType> ImagesInformationMapType;

        typedef std::map<std::string, ListOfStrings> L1ListOfBandsMapType;

        /** Destructor */
        virtual
        ~BandsDefinitions()
        {
        }

        virtual unsigned int
        GetL1BandId(const std::string & code) const
        {
            if (m_L1BandMap.find(code) != m_L1BandMap.end())
            {
                return m_L1BandMap.find(code)->second;
            }
            else
            {
                vnsStaticExceptionBusinessMacro("The code '"<<code<<"' is not present in L1 !")
            }
        }
        virtual unsigned int
        GetBandIdInL2Coarse(const std::string & code) const
        {
            if (m_L2CoarseBandMap.find(code) != m_L2CoarseBandMap.end())
            {
                return m_L2CoarseBandMap.find(code)->second;
            }
            else
            {
                vnsStaticExceptionBusinessMacro("The code '"<<code<<"' is not present in L2Coarse !")
            }
        }

        /**
         * Get the resolution in meters from the resolution name "R1" for example
         */
        unsigned int
        GetL1Resolution(const std::string &res) const
        {
            // ---------------------------------------------------------------------------------------------
            if (m_ImagesInformationMap.find(res) != m_ImagesInformationMap.end())
            {
                return m_ImagesInformationMap.find(res)->second.L1Resolution;
            }
            // ---------------------------------------------------------------------------------------------
            else
            {
                vnsStaticExceptionBusinessMacro("The resolution '"<<res<<"' is not defined !")
            }
        }


        std::string
        GetL1ResolutionForBandCode(const std::string &BandCode) const
        {
            // ---------------------------------------------------------------------------------------------
            // Loops on the map
            for (ImagesInformationMapType::const_iterator it = m_ImagesInformationMap.begin(); it != m_ImagesInformationMap.end(); ++it)
            {
                const ListOfStrings l_ListOfBandCode = this->GetListOfL1BandCode(it->first);
                const unsigned int l_NbListOfBandCode = l_ListOfBandCode.size();
                // ---------------------------------------------------------------------------------------------
                // Loops on the band code
                for (unsigned int l_band = 0; l_band < l_NbListOfBandCode; l_band++)
                {
                    if (std::string(BandCode) == l_ListOfBandCode.at(l_band))
                    {
                        return it->first;
                    }
                }
            }
            vnsStaticExceptionBusinessMacro(
                    "The Band Code " <<BandCode<<" is incorrect (it was not found in the list of band codes for each resolution)!")
            return std::string("NotFound");
        }

        /*
         * This function take a bandCide in input and return :
         * BandId : band index in the vector image for the band resolution
         * strResolution : the resolution in string
         * IdResolution : the index of the image in the imageList in L1 Resolution
         */
        void
        GetL1InformationForBandCode(const std::string &BandCode,unsigned int &BandId,
                std::string &strResolution, unsigned int & IdResolution) const
        {
            // --------------------------------------
            // Get the number of resolution
            const unsigned int l_NbResolutions = m_ListOfL1Resolution.size();
            // --------------------------------------
            vnsLogSuperDebugMacro(" l_NbResolutions = === "<<l_NbResolutions)
            // --------------------------------------
            // Init the resolution
            unsigned int l_Resolution(0);
            // --------------------------------------
            // Declares and init boolean variables
            bool l_BandFound(false);
            // --------------------------------------
            // Loop under resolution
            for (unsigned int l_resol = 0; l_resol < l_NbResolutions; l_resol++)
            {
                vnsLogSuperDebugMacro(" m_ListOfL1Resolution.at(l_resol) = === "<<m_ListOfL1Resolution.at(l_resol))

                // --------------------------------------
                // Get the list of bands
                const ListOfStrings l_ListOfBandCode = this->GetListOfL1BandCode(m_ListOfL1Resolution.at(l_resol));
                const unsigned int l_NbListOfBandCode = l_ListOfBandCode.size();
                vnsLogSuperDebugMacro(" l_NbListOfBandCode = === "<<l_NbListOfBandCode)
                // --------------------------------------
                // Loop under list of band code
                for (unsigned int l_band = 0; l_band < l_NbListOfBandCode; l_band++)
                {
                    vnsLogSuperDebugMacro(" l_ListOfBandCode.at(l_resol) = === "<<l_ListOfBandCode.at(l_band))
                    // --------------------------------------
                    // Init the red resolution and index
                    if (BandCode == l_ListOfBandCode.at(l_band))
                    {
                        l_Resolution = l_resol;
                        BandId = l_band;
                        l_BandFound = true;
                    }
                }
            }

            // --------------------------------------
            // Check the validity of the band
            if (l_BandFound == false)
            {
                vnsStaticExceptionBusinessMacro("The band value '"<<BandCode<<"' is incorrect !")
            }
            strResolution = m_ListOfL2Resolution.at(l_Resolution);
            IdResolution = l_Resolution;

        }

        std::string
        GetL1ResolutionForQuicklookBandCode(const std::string &redBandCode, const std::string &greenBandCode,
                const std::string &blueBandCode, unsigned int &redBandId, unsigned int &greenBandId, unsigned int &blueBandId) const
        {
            // --------------------------------------
            // Get the number of resolution
            const unsigned int l_NbResolutions = m_ListOfL1Resolution.size();
            // --------------------------------------
            vnsLogSuperDebugMacro(" l_NbResolutions = === "<<l_NbResolutions)
            // --------------------------------------
            // Init the RGB resolution
            unsigned int l_RedResolution(0);
            unsigned int l_GreenResolution(0);
            unsigned int l_BlueResolution(0);
            // --------------------------------------
            // Declares and init boolean variables
            bool l_RedBandFound(false);
            bool l_GreenBandFound(false);
            bool l_BlueBandFound(false);
            // --------------------------------------
            // Loop under resolution
            for (unsigned int l_resol = 0; l_resol < l_NbResolutions; l_resol++)
            {
                vnsLogSuperDebugMacro(" m_ListOfL1Resolution.at(l_resol) = === "<<m_ListOfL1Resolution.at(l_resol))

                // --------------------------------------
                // Get the list of bands
                const ListOfStrings l_ListOfBandCode = this->GetListOfL2BandCode(m_ListOfL1Resolution.at(l_resol));
                const unsigned int l_NbListOfBandCode = l_ListOfBandCode.size();
                vnsLogSuperDebugMacro(" l_NbListOfBandCode = === "<<l_NbListOfBandCode)
                // --------------------------------------
                // Loop under list of band code
                for (unsigned int l_band = 0; l_band < l_NbListOfBandCode; l_band++)
                {
                    vnsLogSuperDebugMacro(" l_ListOfBandCode.at(l_resol) = === "<<l_ListOfBandCode.at(l_band))
                    // --------------------------------------
                    // Init the red resolution and index
                    if (redBandCode == l_ListOfBandCode.at(l_band))
                    {
                        l_RedResolution = l_resol;
                        redBandId = l_band;
                        l_RedBandFound = true;
                    }
                    // --------------------------------------
                    // Init the green resolution and index
                    if (greenBandCode == l_ListOfBandCode.at(l_band))
                    {
                        l_GreenResolution = l_resol;
                        greenBandId = l_band;
                        l_GreenBandFound = true;
                    }
                    // --------------------------------------
                    // Init the blue resolution and index
                    if (blueBandCode == l_ListOfBandCode.at(l_band))
                    {
                        l_BlueResolution = l_resol;
                        blueBandId = l_band;
                        l_BlueBandFound = true;
                    }
                }
            }

            // --------------------------------------
            // Check the validity of the red band
            if (l_RedBandFound == false)
            {
                vnsStaticExceptionBusinessMacro("The Red band value '"<<redBandCode<<"' is incorrect !")
            }
            // --------------------------------------
            // Check the validity of the green band
            if (l_GreenBandFound == false)
            {
                vnsStaticExceptionBusinessMacro("The Green band value '"<<greenBandCode<<"' is incorrect !")
            }
            // --------------------------------------
            // Check the validity of the blue band
            if (l_BlueBandFound == false)
            {
                vnsStaticExceptionBusinessMacro("The Blue band value '"<<blueBandCode<<"' is incorrect !")
            }
            // --------------------------------------
            // Check the compatibility of the resolution
            if ((l_RedResolution != l_GreenResolution) || (l_RedResolution != l_BlueResolution))
            {
                vnsStaticExceptionBusinessMacro(
                        "The resolutions for the three Quicklook bands ["<<redBandCode<<","<<greenBandCode<<","<<blueBandCode<<"] are not identical !")
            }
            return m_ListOfL1Resolution.at(l_RedResolution);
        }

        void
        GetL2InformationForQuicklookBandCode(const std::string &redBandCode, const std::string &greenBandCode,
                const std::string &blueBandCode, unsigned int &redBandId, unsigned int &greenBandId, unsigned int &blueBandId,
                std::string &strResolution, unsigned int & IdResolution) const
        {
            // --------------------------------------
            // Get the number of resolution
            const unsigned int l_NbResolutions = m_ListOfL2Resolution.size();
            // --------------------------------------
            // Declares RGB variables
            unsigned int l_RedResolution(0);
            unsigned int l_GreenResolution(0);
            unsigned int l_BlueResolution(0);
            // --------------------------------------
            // Declares and init boolean variables
            bool l_RedBandFound(false);
            bool l_GreenBandFound(false);
            bool l_BlueBandFound(false);

            // --------------------------------------
            // Loop under resolutions
            for (unsigned int l_resol = 0; l_resol < l_NbResolutions; l_resol++)
            {
                // --------------------------------------
                // Get the number of band code
                const ListOfStrings l_ListOfBandCode = this->GetListOfL2BandCode(m_ListOfL2Resolution.at(l_resol));
                const unsigned int l_NbListOfBandCode = l_ListOfBandCode.size();
                // --------------------------------------
                // Loop under bands codes
                for (unsigned int l_band = 0; l_band < l_NbListOfBandCode; l_band++)
                {
                    // --------------------------------------
                    // Init the red resolution and index
                    if (redBandCode == l_ListOfBandCode.at(l_band))
                    {
                        l_RedResolution = l_resol;
                        redBandId = l_band;
                        l_RedBandFound = true;
                    }
                    // --------------------------------------
                    // Init the green resolution and index
                    if (greenBandCode == l_ListOfBandCode.at(l_band))
                    {
                        l_GreenResolution = l_resol;
                        greenBandId = l_band;
                        l_GreenBandFound = true;
                    }
                    // --------------------------------------
                    // Init the blue resolution and index
                    if (blueBandCode == l_ListOfBandCode.at(l_band))
                    {
                        l_BlueResolution = l_resol;
                        blueBandId = l_band;
                        l_BlueBandFound = true;
                    }
                }
            }
            // --------------------------------------
            // Check the validity of the red band
            if (l_RedBandFound == false)
            {
                vnsStaticExceptionBusinessMacro("The Red band value '"<<redBandCode<<"' is incorrect !")
            }
            // --------------------------------------
            // Check the validity of the green band
            if (l_GreenBandFound == false)
            {
                vnsStaticExceptionBusinessMacro("The Green band value '"<<greenBandCode<<"' is incorrect !")
            }
            // --------------------------------------
            // Check the validity of the blue band
            if (l_BlueBandFound == false)
            {
                vnsStaticExceptionBusinessMacro("The Blue band value '"<<blueBandCode<<"' is incorrect !")
            }

            // --------------------------------------
            // Check the compatibility of the resolution
            if ((l_RedResolution != l_GreenResolution) || (l_RedResolution != l_BlueResolution))
            {
                vnsStaticExceptionBusinessMacro(
                        "The resolutions for the three Quicklook bands ["<<redBandCode<<","<<greenBandCode<<","<<blueBandCode<<"] are not identical !")
            }
            strResolution = m_ListOfL2Resolution.at(l_RedResolution);
            IdResolution = l_RedResolution;
        }

        /** Return the number of bands in the L1 product */
        virtual unsigned short
        GetNumberOfL1Bands() const
        {
            // ---------------------------------------------------------------------------------------------
            return m_L1BandMap.size();
        }
        virtual ListOfStrings
        GetListOfL1BandCode(const std::string & res) const
        {
            // ---------------------------------------------------------------------------------------------
            if (m_L1ListOfBandsMap.find(res) != m_L1ListOfBandsMap.end())
            {
                return m_L1ListOfBandsMap.find(res)->second;
            }
            // ---------------------------------------------------------------------------------------------
            else
            {
                vnsStaticExceptionBusinessMacro("The resolution '"<<res<<"' is not defined !")
            }
        }

        virtual ListOfStrings
        GetListOfL2BandCode(const std::string & res) const
        {
            // ---------------------------------------------------------------------------------------------
            if (m_ImagesInformationMap.find(res) != m_ImagesInformationMap.end())
            {
                return m_ImagesInformationMap.find(res)->second.ListOfBandCode;
            }
            // ---------------------------------------------------------------------------------------------
            else
            {
                vnsStaticExceptionBusinessMacro("The resolution '"<<res<<"' is not defined !")
            }
        }

        /** ATTENTION : List of index is not sorted */
        virtual ListOfStrings
        GetListOfBandCodeInL2Coarse(void) const
        {
            // ---------------------------------------------------------------------------------------------
            ListOfStrings listOfKey;
            for (MapOfUInteger::const_iterator it = m_L2CoarseBandMap.begin(); it != m_L2CoarseBandMap.end(); ++it)
            {
                listOfKey.push_back(it->first);
            }
            // ---------------------------------------------------------------------------------------------
            // Returns the list
            return listOfKey;
        }

        /** ATTENTION : List of index is not sorted */
        virtual ListOfStrings
        GetListOfBandCodeInL2CoarseSortedByIndex(void) const
        {
            // ---------------------------------------------------------------------------------------------
            ListOfStrings listOfKey;
            const unsigned int l_NumberOfBand(m_L2CoarseBandMap.size());
            // ---------------------------------------------------------------------------------------------
            // Loops on the bands
            for (unsigned int id = 0; id < l_NumberOfBand; id++)
            {
                bool found(false);
                // ---------------------------------------------------------------------------------------------
                // Loops on the band map at L2Coarse
                for (MapOfUInteger::const_iterator it = m_L2CoarseBandMap.begin(); it != m_L2CoarseBandMap.end(); ++it)
                {
                    if (it->second == id)
                    {
                        found = true;
                        listOfKey.push_back(it->first);
                    }
                }
                // ---------------------------------------------------------------------------------------------
                // If not found, throws an exception
                if (found == false)
                {
                    vnsStaticExceptionBusinessMacro(
                            "Internal error in the GetListOfBandCodeInL2CoarseSortedByIndex method: the index '"<<id<<"' has not be found !")
                }
            }
            // ---------------------------------------------------------------------------------------------
            // CHeck the size of the list created
            if (listOfKey.size() != l_NumberOfBand)
            {
                vnsStaticExceptionBusinessMacro("Internal error in the GetListOfBandCodeInL2CoarseSortedByIndex method !")
            }
            return listOfKey;
        }

        virtual ListOfUIntegers
        GetListOfBandIdInL2Coarse() const
        {
            // ---------------------------------------------------------------------------------------------
            ListOfUIntegers l_ListOfL2CoarseBandId;
            const unsigned int l_NumberOfBand(m_L2CoarseBandMap.size());
            // ---------------------------------------------------------------------------------------------
            // Loops on the bands
            for (unsigned int id = 0; id < l_NumberOfBand; id++)
            {
                l_ListOfL2CoarseBandId.push_back(id);
            }
            return l_ListOfL2CoarseBandId;
        }

        virtual ListOfUIntegers
        GetListOfL2CoarseBandIdAssociatedToL2BandCode(ListOfStrings pListOfBandCode) const
        {
            ListOfUIntegers l_ListOfL2CoarseBandId;
            // ---------------------------------------------------------------------------------------------
            // Loops on the bands
            for (unsigned int band = 0; band < pListOfBandCode.size(); band++)
            {
                const std::string l2BandCode = pListOfBandCode[band];
                l_ListOfL2CoarseBandId.push_back(this->GetBandIdInL2Coarse(l2BandCode));
            }
            return l_ListOfL2CoarseBandId;
        }

        void
        GetL3ResolutionForQuicklookBandCode(const std::string &redBandCode, const std::string &greenBandCode,
                const std::string &blueBandCode, std::string &strResolution, unsigned int & IdResolution) const
        {
            // --------------------------------------
            // Get the number of resolution
            const unsigned int l_NbResolutions = m_ListOfL2Resolution.size();
            // --------------------------------------
            // Declares RGB resolutions
            unsigned int l_RedResolution(0);
            unsigned int l_GreenResolution(0);
            unsigned int l_BlueResolution(0);
            // --------------------------------------
            //Loop under resolutions
            for (unsigned int l_resol = 0; l_resol < l_NbResolutions; l_resol++)
            {
                const ListOfStrings l_ListOfListOfBandCode = this->GetListOfL2BandCode(m_ListOfL2Resolution.at(l_resol));
                const unsigned int l_NbListOfListOfBandCode = l_ListOfListOfBandCode.size();
                for (unsigned int l_band = 0; l_band < l_NbListOfListOfBandCode; l_band++)
                {
                    // --------------------------------------
                    // Init the red resolution band
                    if (redBandCode == l_ListOfListOfBandCode.at(l_band))
                    {
                        l_RedResolution = l_resol;
                    }
                    // --------------------------------------
                    // Init the green resolution band
                    if (greenBandCode == l_ListOfListOfBandCode.at(l_band))
                    {
                        l_GreenResolution = l_resol;
                    }
                    // --------------------------------------
                    // Init the blue resolution band
                    if (blueBandCode == l_ListOfListOfBandCode.at(l_band))
                    {
                        l_BlueResolution = l_resol;
                    }
                }
            }

            // --------------------------------------
            // Check the compatibility of the resolution
            if ((l_RedResolution != l_GreenResolution) || (l_RedResolution != l_BlueResolution))
            {
                vnsStaticExceptionBusinessMacro(
                        "The resolutions for the three Quicklook bands ["<<redBandCode<<","<<greenBandCode<<","<<blueBandCode<<"] are not identical !")
            }
            strResolution = m_ListOfL2Resolution.at(l_RedResolution);
            IdResolution = l_RedResolution;
        }

        void
        PrintSelf(std::ostream &os) const
        {
            // --------------------------------------
            os << " Specific definitions : " << std::endl;
            os << " L2 Image definition map " << std::endl;

            // --------------------------------------
            os << std::fixed;
            os << std::setprecision(15);
            // --------------------------------------
            os << "    L1BandMap " << std::endl;

            // --------------------------------------
            for (MapOfUInteger::const_iterator it3 = m_L1BandMap.begin(); it3 != m_L1BandMap.end(); ++it3)
            {
                os << "         L1BandMap[" << it3->first << "] = " << it3->second << std::endl;
            }
            // --------------------------------------
            os << "    L2CoarseBandMap " << std::endl;

            // --------------------------------------
            for (MapOfUInteger::const_iterator it2 = m_L2CoarseBandMap.begin(); it2 != m_L2CoarseBandMap.end(); ++it2)
            {
                os << "         L2CoarseBandMap[" << it2->first << "] = " << it2->second << std::endl;
            }

            // --------------------------------------
            const ListOfUIntegers l_ListOfBandIdInL2Coarse = this->GetListOfBandIdInL2Coarse();
            // --------------------------------------
            const unsigned int l_NbListOfBandIdInL2Coarse = l_ListOfBandIdInL2Coarse.size();
            os << "    GetListOfBandIdInL2Coarse " << l_NbListOfBandIdInL2Coarse << ":" << std::endl;
            // --------------------------------------
            for (unsigned int i = 0; i < l_NbListOfBandIdInL2Coarse; i++)
            {
                os << "       [" << i << "] = " << l_ListOfBandIdInL2Coarse.at(i) << std::endl;
            }
            // --------------------------------------
            const ListOfStrings l_ListOfBandCodeInL2Coarse = this->GetListOfBandCodeInL2Coarse();
            // --------------------------------------
            const unsigned int l_NbListOfBandCodeInL2Coarse = l_ListOfBandCodeInL2Coarse.size();
            os << "    GetListOfBandCodeInL2Coarse " << l_NbListOfBandCodeInL2Coarse << ":" << std::endl;
            // --------------------------------------
            for (unsigned int i = 0; i < l_NbListOfBandCodeInL2Coarse; i++)
            {
                // you have to call the InitializeL1 method before calling the PrintSelf one !
                os << "       [" << i << "] = " << l_ListOfBandCodeInL2Coarse.at(i) << " => L1Resolution: "
                        << this->GetL1ResolutionForBandCode(l_ListOfBandCodeInL2Coarse.at(i)) << std::endl;
            }

            // --------------------------------------
            const ListOfStrings l_ListOfBandCodeInL2CoarseSortedByIndex = this->GetListOfBandCodeInL2CoarseSortedByIndex();
            // --------------------------------------
            const unsigned int l_NbListOfBandCodeInL2CoarseSortedByIndex = l_ListOfBandCodeInL2CoarseSortedByIndex.size();
            os << "    GetListOfBandCodeInL2CoarseSortedByIndex " << l_NbListOfBandCodeInL2CoarseSortedByIndex << ":" << std::endl;
            // --------------------------------------
            for (unsigned int i = 0; i < l_NbListOfBandCodeInL2CoarseSortedByIndex; i++)
            {
                os << "       [" << i << "] = " << l_ListOfBandCodeInL2CoarseSortedByIndex.at(i) << std::endl;
            }

            // --------------------------------------
            const unsigned int l_NbListOfL1Resolution = m_ListOfL1Resolution.size();
            os << "    ListOfL1Resolution :" << l_NbListOfL1Resolution << std::endl;
            // --------------------------------------
            for (unsigned int i = 0; i < l_NbListOfL1Resolution; i++)
            {
                os << "       [" << i << "] = " << m_ListOfL1Resolution.at(i) << std::endl;
            }
            // --------------------------------------
            const unsigned int l_NbListOfL2Resolution = m_ListOfL2Resolution.size();
            os << "    ListOfL2Resolution :" << l_NbListOfL2Resolution << std::endl;
            // --------------------------------------
            for (unsigned int i = 0; i < l_NbListOfL2Resolution; i++)
            {
                os << "       [" << i << "] = " << m_ListOfL2Resolution.at(i) << std::endl;
            }
            os << "    ListOfBandCode :" << l_NbListOfL2Resolution << std::endl;
            // --------------------------------------
            for (unsigned int l_resol = 0; l_resol < l_NbListOfL2Resolution; l_resol++)
            {
                os << "       Resolution [" << l_resol << "]:" << std::endl;
                // --------------------------------------
                const ListOfStrings l_ListOfBandCode = this->GetListOfL2BandCode(m_ListOfL2Resolution.at(l_resol));
                // --------------------------------------
                const unsigned int l_NbListOfBandCode = l_ListOfBandCode.size();
                // --------------------------------------
                for (unsigned int l_band = 0; l_band < l_NbListOfBandCode; l_band++)
                {
                    os << "            BandCode:    [" << l_band << "] = " << l_ListOfBandCode.at(l_band) << std::endl;
                }
                // --------------------------------------
                const ListOfUIntegers l_ListOfL2CoarseBandId = this->GetListOfL2CoarseBandIdAssociatedToL2BandCode(l_ListOfBandCode);
                // --------------------------------------
                const unsigned int l_NbListOfL2CoarseBandId = l_ListOfL2CoarseBandId.size();
                // --------------------------------------
                for (unsigned int l_band = 0; l_band < l_NbListOfL2CoarseBandId; l_band++)
                {
                    os << "            Index Of L2CoarseBandIdAssociatedToL2BandCode:   [" << l_band << "] = "
                            << l_ListOfL2CoarseBandId.at(l_band) << std::endl;
                }
            }
        }

        inline std::ostream &
        operator<<(std::ostream &os)
        {
            this->PrintSelf(os);
            return os;
        }

        vnsPublicMemberAndSetAndGetConstReferenceMacro(L1ListOfBandsMap, L1ListOfBandsMapType)
        vnsPublicMemberAndSetAndGetConstReferenceMacro(ImagesInformationMap, ImagesInformationMapType)
        vnsPublicMemberAndSetAndGetConstReferenceMacro(L1BandMap, MapOfUInteger)
        vnsPublicMemberAndSetAndGetConstReferenceMacro(L2CoarseBandMap, MapOfUInteger)
        vnsPublicMemberAndSetAndGetConstReferenceMacro(ListOfL1Resolution, ListOfStrings)
        vnsPublicMemberAndSetAndGetConstReferenceMacro(ListOfL2Resolution, ListOfStrings)
        vnsPublicMemberAndSetAndGetConstReferenceMacro(ReferenceResolution, std::string)

};

}
// End namespace vns

#endif /* __vnsBandsDefinitions_h */
