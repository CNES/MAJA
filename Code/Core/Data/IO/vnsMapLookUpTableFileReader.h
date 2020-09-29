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
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 sept. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsMapLookUpTableFileReader_h
#define vnsMapLookUpTableFileReader_h

#include "vnsMacro.h"
#include "vnsLookUpTable.h"
#include "vnsVectorLookUpTable.h"
#include "itkProcessObject.h"
#include "itkImageFileReader.h"
#include "vnsUtilities.h"
#include "vnsAtomicRegistry.h"

namespace vns
{
    /** \class MapLookUpTableFileReader
     * \brief
     *
     * MapLookUpTableFileReader is used to load a several LUT files in a map of LUT.
     * The output is an std::map of LUT with associated keys equal to the corresponding
     * julian day.
     *
     * \ingroup L2
     *
     * \sa  LookUpTable
     * \sa  VectorLookUpTable
     */
    template<class TLookUpTableType>
        class MapLookUpTableFileReader : public itk::ProcessObject
        {
        public:

            /** Standard class typedefs. */
            typedef MapLookUpTableFileReader<TLookUpTableType> Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Run-time type information (and related methods). */
            itkTypeMacro(MapLookUpTableFileReader,itk::ProcessObject)

            /** Typedefs */
            typedef TLookUpTableType LookUpTableType;
            typedef typename LookUpTableType::PixelType LookUpTablePixelType;
            typedef typename LookUpTableType::InternalPixelType LookUpTableInternalPixelType;
            typedef typename LookUpTableType::Pointer LookUpTablePointerType;

            typedef typename LookUpTableType::ParameterValuesType ParameterValuesType;
            typedef typename LookUpTableType::ParametersValuesType ParametersValuesType;
            typedef typename LookUpTableType::IndexType IndexType;

            /** Julian day */
            typedef unsigned int JulianDayType;

            /** LUTMapType */
            typedef std::map<JulianDayType, LookUpTablePointerType> LUTMapType;

            /** Useful lists typedefs */
            typedef std::vector<unsigned int> ListOfJulianDays;
            typedef typename Utilities::ListOfFilenames ListOfFilenames;

            /** Public methods **/

            /** Add the same parameters values to each LUT */
            void
            AddParameterValues(const ParameterValuesType &NewParamValues);

            /** Set the list of miniLUT filenames :
             * create a list of reader and performs
             * SetFilename() for each subLUTReader, using the ReferenceDate */
            void
            SetListOfFilenames(const ListOfFilenames & pList, const std::string & pReferenceDate);

            /** Load all the LUTs and fill the map.
             * Warning : before generating of the mapLUT,
             * the parameters and the filenames had to be set
             * */
            void
            GenerateLUTMap();

            /** Get the output */
            const LUTMapType &
            GetLUTMap() const
            {
                return m_LUTMap;
            }

        protected:
            /** Constructor */
            MapLookUpTableFileReader();
            /** Destructor */
            virtual
            ~MapLookUpTableFileReader();

        private:
            MapLookUpTableFileReader(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Attributes */
            ListOfFilenames m_ListOfFilenames;
            ListOfJulianDays m_ListOfJulianDays;
            ParametersValuesType m_ParametersValues;
            LUTMapType m_LUTMap;

            AtomicRegistry<itk::MetaImageIOFactory> m_MetaRegistry;
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsMapLookUpTableFileReader.txx"
#endif

#endif /* vnsMapLookUpTableFileReader_h */
