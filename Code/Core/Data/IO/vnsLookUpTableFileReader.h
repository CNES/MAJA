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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 2 mai 2016 : Correction warning, qualite, etc             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-372-CNES : 18 avril 2014 : Decoupage des methodes trop complexes.      *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 12 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsLookUpTableFileReader_h
#define __vnsLookUpTableFileReader_h

#include "vnsLookUpTable.h"
#include "vnsVectorLookUpTable.h"
#include "itkProcessObject.h"
#include "vnsUtilities.h"
#include "vnsSystem.h"

namespace vns
{

    /** \class LookUpTableFileReader
     * \brief
     *
     * LookUpTableFileReader is used to load a LookUpTable file.
     * The output is an otbImage or a n otbVectorImage with precision and dimension
     * is given as class template.
     *
     * \ingroup L2
     *
     * \sa  LookUpTable
     * \sa  VectorLookUpTable
     */
    template<class TLookUpTableType = LookUpTable<double> >
        class LookUpTableFileReader : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef LookUpTableFileReader<TLookUpTableType> Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            typedef TLookUpTableType LookUpTableType;
            typedef typename LookUpTableType::PixelType LookUpTablePixelType;
            typedef typename LookUpTableType::InternalPixelType LookUpTableInternalPixelType;

            typedef typename LookUpTableType::ParameterValuesType ParameterValuesType;
            typedef typename LookUpTableType::ParametersValuesType ParametersValuesType;
            typedef typename LookUpTableType::IndexType IndexType;
            typedef typename LookUpTableType::IndexValueType IndexValueType;

            typedef Utilities::ListOfListOfFloats ListOfListOfFloats;

            typedef itk::VariableLengthVector<LookUpTableInternalPixelType> VectorPixelType;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Run-time type information (and related methods). */
            itkTypeMacro(LookUpTableFileReader,itk::ProcessObject)

            /**
             * Get the output LookUpTable
             * \return The extracted LookUpTablea.
            */
            virtual LookUpTableType *
            GetLUT(void)
            {
                return static_cast<LookUpTableType *>(m_LUT);
            }

            /** Check FileName : is the nth filename is available */
            bool
            IsLookUpTableFile(int num);

            /** Add parameters values to the LUT */
            void
            AddParameterValues(const ParameterValuesType &NewParamValues)
            {
                // Add parameters to the LUT (readind hdr)
                m_LUT->AddParameterValues(NewParamValues);
            }

            /** LUT bands filename Add/Get */
            void
            AddBandFilename(std::string path)
            {
                m_BandFileName.push_back(path);
                // Increase the number of bands
                m_LUT->SetNumberOfComponentsPerPixel(m_BandFileName.size());
            }
            std::string
            GetBandFileName()
            {
                return m_BandFileName.at(0);
            }
            std::string
            GetBandFileName(unsigned int num)
            {
                return m_BandFileName.at(num);
            }

            /* Read and fill the LUT */
            void
            GenerateLUT(void);

        protected:
            LookUpTableFileReader();
            virtual
            ~LookUpTableFileReader()
            {
            }

            void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Generate Requested Data */
            virtual void
            GenerateData(void)
            {
            }

            void
            SetPixelSize(VectorPixelType & pPix, unsigned int pSize)
            {
                pPix.SetSize(pSize);
            }
            void
            SetPixelSize(LookUpTableInternalPixelType &, unsigned int)
            {
            }

            void
            SetPixelValue(VectorPixelType & pPix, float var, unsigned int lNumBand)
            {
                pPix[lNumBand] = static_cast<LookUpTableInternalPixelType>( var);
            }
            void
            SetPixelValue(LookUpTableInternalPixelType & pPix, float var, unsigned int)
            {
                pPix = static_cast<LookUpTableInternalPixelType>(var);
            }

        private:
            LookUpTableFileReader(const LookUpTableFileReader&); //purposely not implemented
            void
            operator=(const LookUpTableFileReader&); //purposely not implemented

            // ----------------------------------------------------------
            // Generates LUT with 2D
            template<class TValue>
                void
                GenerateXDLUT(VectorLookUpTable<TValue, 2> * pLUT, const ListOfListOfFloats & pBuffersBands, const unsigned int pNbBands)
                {
                    typedef VectorLookUpTable<TValue, 2> LocalLookUpTableType;

                    vnsLogDebugMacro("Generate LUT "<<LocalLookUpTableType::ImageDimension <<"D ...")
                    // ----------------------------------------------------------
                    // Used only for VectorLUTs
                    // The pixel size is equal to the number of spectral band
                    typename LocalLookUpTableType::PixelType lPix = static_cast<typename LocalLookUpTableType::PixelType>(0);
                    this->SetPixelSize(lPix, pNbBands);
                    // ----------------------------------------------------------
                    // Index of the pixel
                    typename LocalLookUpTableType::IndexType index;
                    // ----------------------------------------------------------
                    // Fill to 0
                    index.Fill((typename LocalLookUpTableType::IndexValueType) 0);

                    // ----------------------------------------------------------
                    // Initialization
                    std::streamoff cpt(0);

                    const ParametersValuesType & LUT_Indexes(pLUT->GetParametersValues());
                    const unsigned int LUT_Indexes0ParameterValuesSize(LUT_Indexes.at(0).ParameterValues.size());
                    const unsigned int LUT_Indexes1ParameterValuesSize(LUT_Indexes.at(1).ParameterValues.size());

                    const std::size_t l_NbValues = pBuffersBands.front().size();
                    const std::size_t l_IndexSize = LUT_Indexes0ParameterValuesSize * LUT_Indexes1ParameterValuesSize;

                    if (l_NbValues != l_IndexSize)
                    {
                        vnsExceptionDataMacro(
                                "The size of the buffered value is "<<l_NbValues << " and is incoherent with the size of the indexes parameters values "<< l_IndexSize)
                    }
                    // loop over the first parameter (for example TOA reflectance)
                    for (unsigned int a = 0; a < LUT_Indexes0ParameterValuesSize; a++)
                    {
                        index[0] = a;
                        // loop over the second parameter (for example altitude reflectance)
                        for (unsigned int b = 0; b < LUT_Indexes1ParameterValuesSize; b++)
                        {
                            index[1] = b;
                            // Number of components per pixel
                            for (unsigned int lNumBand = 0; lNumBand < pNbBands; lNumBand++)
                            {
                                this->SetPixelValue(lPix, pBuffersBands[lNumBand][cpt], lNumBand);
                            }
                            cpt++;
                            pLUT->SetValue(index, lPix);
                        }
                    }
                }
            // ----------------------------------------------------------
            // Generates LUT with 2D
            template<class TValue>
                void
                GenerateXDLUT(LookUpTable<TValue, 2> * pLUT, const ListOfListOfFloats & pBuffersBands, const unsigned int pNbBands)
                {
                    typedef LookUpTable<TValue, 2> LocalLookUpTableType;

                    vnsLogDebugMacro("Generate LUT "<<LocalLookUpTableType::ImageDimension <<"D ...")
                    // ----------------------------------------------------------
                    // Used only for VectorLUTs
                    // The pixel size is equal to the number of spectral band
                    typename LocalLookUpTableType::PixelType lPix = static_cast<typename LocalLookUpTableType::PixelType>(0);
                    this->SetPixelSize(lPix, pNbBands);
                    // ----------------------------------------------------------
                    // Index of the pixel
                    typename LocalLookUpTableType::IndexType index;
                    // ----------------------------------------------------------
                    // Fill to 0
                    index.Fill((typename LocalLookUpTableType::IndexValueType) 0);

                    // ----------------------------------------------------------
                    // Initialization
                    std::streamoff cpt(0);

                    const ParametersValuesType & LUT_Indexes(pLUT->GetParametersValues());
                    const unsigned int LUT_Indexes0ParameterValuesSize(LUT_Indexes.at(0).ParameterValues.size());
                    const unsigned int LUT_Indexes1ParameterValuesSize(LUT_Indexes.at(1).ParameterValues.size());

                    const std::size_t l_NbValues = pBuffersBands.front().size();
                    const std::size_t l_IndexSize = LUT_Indexes0ParameterValuesSize * LUT_Indexes1ParameterValuesSize;

                    if (l_NbValues != l_IndexSize)
                    {
                        vnsExceptionDataMacro(
                                "The size of the buffered value is "<<l_NbValues << " and is incoherent with the size of the indexes parameters values "<< l_IndexSize)
                    }

                    // loop over the first parameter (for example TOA reflectance)
                    for (unsigned int a = 0; a < LUT_Indexes0ParameterValuesSize; a++)
                    {
                        index[0] = a;
                        // loop over the second parameter (for example altitude reflectance)
                        for (unsigned int b = 0; b < LUT_Indexes1ParameterValuesSize; b++)
                        {
                            index[1] = b;
                            // Number of components per pixel
                            for (unsigned int lNumBand = 0; lNumBand < pNbBands; lNumBand++)
                            {
                                this->SetPixelValue(lPix, pBuffersBands[lNumBand][cpt], lNumBand);
                            }
                            cpt++;
                            pLUT->SetValue(index, lPix);
                        }
                    }
                }
            // ----------------------------------------------------------
            // Generates LUT with 3D
            template<class TValue>
                void
                GenerateXDLUT(VectorLookUpTable<TValue, 3> * pLUT, const ListOfListOfFloats & pBuffersBands, const unsigned int pNbBands)
                {
                    typedef VectorLookUpTable<TValue, 3> LocalLookUpTableType;

                    vnsLogDebugMacro("Generate LUT "<<LocalLookUpTableType::ImageDimension <<"D ...")

                    // ----------------------------------------------------------
                    // Used only for VectorLUTs
                    // The pixel size is equal to the number of spectral band
                    typename LocalLookUpTableType::PixelType lPix = static_cast<typename LocalLookUpTableType::PixelType>(0);
                    this->SetPixelSize(lPix, pNbBands);
                    // ----------------------------------------------------------
                    // Index of the pixel
                    typename LocalLookUpTableType::IndexType index;
                    // ----------------------------------------------------------
                    // Fill to 0
                    index.Fill((typename LocalLookUpTableType::IndexValueType) 0);

                    // ----------------------------------------------------------
                    // Initialization
                    std::streamoff cpt(0);

                    const ParametersValuesType & LUT_Indexes(pLUT->GetParametersValues());
                    const unsigned int LUT_Indexes0ParameterValuesSize(LUT_Indexes.at(0).ParameterValues.size());
                    const unsigned int LUT_Indexes1ParameterValuesSize(LUT_Indexes.at(1).ParameterValues.size());
                    const unsigned int LUT_Indexes2ParameterValuesSize(LUT_Indexes.at(2).ParameterValues.size());
                    const std::size_t l_NbValues = pBuffersBands.front().size();
                    const std::size_t l_IndexSize = LUT_Indexes0ParameterValuesSize * LUT_Indexes1ParameterValuesSize
                            * LUT_Indexes2ParameterValuesSize;
                    if (l_NbValues != l_IndexSize)
                    {
                        vnsExceptionDataMacro(
                                "The size of the buffered value is "<<l_NbValues << " and is incoherent with the size of the indexes parameters values "<< l_IndexSize)
                    }
                    // loop over the first parameter (for example TOA reflectance)
                    for (unsigned int a = 0; a < LUT_Indexes0ParameterValuesSize; a++)
                    {
                        index[0] = a;
                        // loop over the second parameter (for example AOT)
                        for (unsigned int b = 0; b < LUT_Indexes1ParameterValuesSize; b++)
                        {
                            index[1] = b;
                            // loop over the third parameter (for example altitude)
                            for (unsigned int c = 0; c < LUT_Indexes2ParameterValuesSize; c++)
                            {
                                index[2] = c;
                                // Number of components per pixel
                                for (unsigned int lNumBand = 0; lNumBand < pNbBands; lNumBand++)
                                {
                                    this->SetPixelValue(lPix, pBuffersBands[lNumBand][cpt], lNumBand);

                                }
                                pLUT->SetValue(index, lPix);
                                cpt++;
                            }
                        }
                    }
                }
            // ----------------------------------------------------------
            // Generates LUT with 3D
            template<class TValue>
                void
                GenerateXDLUT(LookUpTable<TValue, 3> * pLUT, const ListOfListOfFloats & pBuffersBands, const unsigned int pNbBands)
                {
                    typedef LookUpTable<TValue, 3> LocalLookUpTableType;

                    vnsLogDebugMacro("Generate LUT "<<LocalLookUpTableType::ImageDimension <<"D ...")

                    // ----------------------------------------------------------
                    // Used only for VectorLUTs
                    // The pixel size is equal to the number of spectral band
                    typename LocalLookUpTableType::PixelType lPix = static_cast<typename LocalLookUpTableType::PixelType>(0);
                    this->SetPixelSize(lPix, pNbBands);
                    // ----------------------------------------------------------
                    // Index of the pixel
                    typename LocalLookUpTableType::IndexType index;
                    // ----------------------------------------------------------
                    // Fill to 0
                    index.Fill((typename LocalLookUpTableType::IndexValueType) 0);

                    // ----------------------------------------------------------
                    // Initialization
                    std::streamoff cpt(0);

                    const ParametersValuesType & LUT_Indexes(pLUT->GetParametersValues());
                    const unsigned int LUT_Indexes0ParameterValuesSize(LUT_Indexes.at(0).ParameterValues.size());
                    const unsigned int LUT_Indexes1ParameterValuesSize(LUT_Indexes.at(1).ParameterValues.size());
                    const unsigned int LUT_Indexes2ParameterValuesSize(LUT_Indexes.at(2).ParameterValues.size());
                    const std::size_t l_NbValues = pBuffersBands.front().size();
                    const std::size_t l_IndexSize = LUT_Indexes0ParameterValuesSize * LUT_Indexes1ParameterValuesSize
                            * LUT_Indexes2ParameterValuesSize;
                    if (l_NbValues != l_IndexSize)
                    {
                        vnsExceptionDataMacro(
                                "The size of the buffered value is "<<l_NbValues << " and is incoherent with the size of the indexes parameters values "<< l_IndexSize)
                    }

                    // loop over the first parameter (for example TOA reflectance)
                    for (unsigned int a = 0; a < LUT_Indexes0ParameterValuesSize; a++)
                    {
                        index[0] = a;
                        // loop over the second parameter (for example AOT)
                        for (unsigned int b = 0; b < LUT_Indexes1ParameterValuesSize; b++)
                        {
                            index[1] = b;
                            // loop over the third parameter (for example altitude)
                            for (unsigned int c = 0; c < LUT_Indexes2ParameterValuesSize; c++)
                            {
                                index[2] = c;
                                // Number of components per pixel
                                for (unsigned int lNumBand = 0; lNumBand < pNbBands; lNumBand++)
                                {
                                    this->SetPixelValue(lPix, pBuffersBands[lNumBand][cpt], lNumBand);

                                }
                                pLUT->SetValue(index, lPix);
                                cpt++;
                            }
                        }
                    }
                }
            // ----------------------------------------------------------
            // Generates LUT with 4D
            template<class TValue>
                void
                GenerateXDLUT(VectorLookUpTable<TValue, 4> * pLUT, const ListOfListOfFloats & pBuffersBands, const unsigned int pNbBands)
                {
                    typedef VectorLookUpTable<TValue, 4> LocalLookUpTableType;

                    vnsLogDebugMacro("Generate LUT "<<LocalLookUpTableType::ImageDimension <<"D ...")

                    // ----------------------------------------------------------
                    // Used only for VectorLUTs
                    // The pixel size is equal to the number of spectral band
                    typename LocalLookUpTableType::PixelType lPix = static_cast<typename LocalLookUpTableType::PixelType>(0);
                    this->SetPixelSize(lPix, pNbBands);
                    // ----------------------------------------------------------
                    // Index of the pixel
                    typename LocalLookUpTableType::IndexType index;
                    // ----------------------------------------------------------
                    // Fill to 0
                    index.Fill((typename LocalLookUpTableType::IndexValueType) 0);

                    // ----------------------------------------------------------
                    // Initialization
                    std::streamoff cpt(0);

                    const ParametersValuesType & LUT_Indexes(pLUT->GetParametersValues());
                    const unsigned int LUT_Indexes0ParameterValuesSize(LUT_Indexes.at(0).ParameterValues.size());
                    const unsigned int LUT_Indexes1ParameterValuesSize(LUT_Indexes.at(1).ParameterValues.size());
                    const unsigned int LUT_Indexes2ParameterValuesSize(LUT_Indexes.at(2).ParameterValues.size());
                    const unsigned int LUT_Indexes3ParameterValuesSize(LUT_Indexes.at(3).ParameterValues.size());
                    const std::size_t l_NbValues = pBuffersBands.front().size();
                    const std::size_t l_IndexSize = LUT_Indexes0ParameterValuesSize * LUT_Indexes1ParameterValuesSize
                            * LUT_Indexes2ParameterValuesSize * LUT_Indexes3ParameterValuesSize;
                    if (l_NbValues != l_IndexSize)
                    {
                        vnsExceptionDataMacro(
                                "The size of the buffered value is "<<l_NbValues << " and is incoherent with the size of the indexes parameters values "<< l_IndexSize)
                    }

                    for (unsigned int a = 0; a < LUT_Indexes0ParameterValuesSize; a++)
                    {
                        index[0] = a;
                        for (unsigned int b = 0; b < LUT_Indexes1ParameterValuesSize; b++)
                        {
                            index[1] = b;
                            for (unsigned int c = 0; c < LUT_Indexes2ParameterValuesSize; c++)
                            {
                                index[2] = c;
                                for (unsigned int d = 0; d < LUT_Indexes3ParameterValuesSize; d++)
                                {
                                    index[3] = d;
                                    // Number of components per pixel
                                    for (unsigned int lNumBand = 0; lNumBand < pNbBands; lNumBand++)
                                    {
                                        this->SetPixelValue(lPix, pBuffersBands[lNumBand][cpt], lNumBand);

                                    }
                                    pLUT->SetValue(index, lPix);
                                    cpt++;
                                }
                            }
                        }
                    }
                }
            // ----------------------------------------------------------
            // Generates LUT with 4D
            template<class TValue>
                void
                GenerateXDLUT(LookUpTable<TValue, 4> * pLUT, const ListOfListOfFloats & pBuffersBands, const unsigned int pNbBands)
                {
                    typedef LookUpTable<TValue, 4> LocalLookUpTableType;

                    vnsLogDebugMacro("Generate LUT "<<LocalLookUpTableType::ImageDimension <<"D ...")

                    // ----------------------------------------------------------
                    // Used only for VectorLUTs
                    // The pixel size is equal to the number of spectral band
                    typename LocalLookUpTableType::PixelType lPix = static_cast<typename LocalLookUpTableType::PixelType>(0);
                    this->SetPixelSize(lPix, pNbBands);
                    // ----------------------------------------------------------
                    // Index of the pixel
                    typename LocalLookUpTableType::IndexType index;
                    // ----------------------------------------------------------
                    // Fill to 0
                    index.Fill((typename LocalLookUpTableType::IndexValueType) 0);

                    // ----------------------------------------------------------
                    // Initialization
                    std::streamoff cpt(0);

                    const ParametersValuesType & LUT_Indexes(pLUT->GetParametersValues());
                    const unsigned int LUT_Indexes0ParameterValuesSize(LUT_Indexes.at(0).ParameterValues.size());
                    const unsigned int LUT_Indexes1ParameterValuesSize(LUT_Indexes.at(1).ParameterValues.size());
                    const unsigned int LUT_Indexes2ParameterValuesSize(LUT_Indexes.at(2).ParameterValues.size());
                    const unsigned int LUT_Indexes3ParameterValuesSize(LUT_Indexes.at(3).ParameterValues.size());

                    const std::size_t l_NbValues = pBuffersBands.front().size();
                    const std::size_t l_IndexSize = LUT_Indexes0ParameterValuesSize * LUT_Indexes1ParameterValuesSize
                            * LUT_Indexes2ParameterValuesSize * LUT_Indexes3ParameterValuesSize;
                    if (l_NbValues != l_IndexSize)
                    {
                        vnsExceptionDataMacro(
                                "The size of the buffered value is "<<l_NbValues << " and is incoherent with the size of the indexes parameters values "<< l_IndexSize)
                    }

                    for (unsigned int a = 0; a < LUT_Indexes0ParameterValuesSize; a++)
                    {
                        index[0] = a;
                        for (unsigned int b = 0; b < LUT_Indexes1ParameterValuesSize; b++)
                        {
                            index[1] = b;
                            for (unsigned int c = 0; c < LUT_Indexes2ParameterValuesSize; c++)
                            {
                                index[2] = c;
                                for (unsigned int d = 0; d < LUT_Indexes3ParameterValuesSize; d++)
                                {
                                    index[3] = d;
                                    // Number of components per pixel
                                    for (unsigned int lNumBand = 0; lNumBand < pNbBands; lNumBand++)
                                    {
                                        this->SetPixelValue(lPix, pBuffersBands[lNumBand][cpt], lNumBand);
                                    }
                                    pLUT->SetValue(index, lPix);
                                    cpt++;
                                }
                            }
                        }
                    }
                }

            // ----------------------------------------------------------
            // Generates LUT with 6D
            template<class TValue>
                void
                GenerateXDLUT(VectorLookUpTable<TValue, 6> * pLUT, const ListOfListOfFloats & pBuffersBands, const unsigned int pNbBands)
                {
                    typedef VectorLookUpTable<TValue, 6> LocalLookUpTableType;

                    vnsLogDebugMacro("Generate LUT "<<LocalLookUpTableType::ImageDimension <<"D ...")

                    // ----------------------------------------------------------
                    // Used only for VectorLUTs
                    // The pixel size is equal to the number of spectral band
                    typename LocalLookUpTableType::PixelType lPix = static_cast<typename LocalLookUpTableType::PixelType>(0);
                    this->SetPixelSize(lPix, pNbBands);

                    // ----------------------------------------------------------
                    // Index of the pixel
                    typename LocalLookUpTableType::IndexType index;
                    // ----------------------------------------------------------
                    // Fill to 0
                    index.Fill((typename LocalLookUpTableType::IndexValueType) 0);

                    // ----------------------------------------------------------
                    // Initialization
                    std::streamoff cpt(0);

                    // ----------------------------------------------------------
                    //Init parameters
                    const ParametersValuesType & LUT_Indexes(pLUT->GetParametersValues());
                    const unsigned int LUT_Indexes0ParameterValuesSize(LUT_Indexes.at(0).ParameterValues.size());
                    const unsigned int LUT_Indexes1ParameterValuesSize(LUT_Indexes.at(1).ParameterValues.size());
                    const unsigned int LUT_Indexes2ParameterValuesSize(LUT_Indexes.at(2).ParameterValues.size());
                    const unsigned int LUT_Indexes3ParameterValuesSize(LUT_Indexes.at(3).ParameterValues.size());
                    const unsigned int LUT_Indexes4ParameterValuesSize(LUT_Indexes.at(4).ParameterValues.size());
                    const unsigned int LUT_Indexes5ParameterValuesSize(LUT_Indexes.at(5).ParameterValues.size());

                    const std::size_t l_NbValues = pBuffersBands.front().size();
                    const std::size_t l_IndexSize = LUT_Indexes0ParameterValuesSize * LUT_Indexes1ParameterValuesSize
                            * LUT_Indexes2ParameterValuesSize * LUT_Indexes3ParameterValuesSize * LUT_Indexes4ParameterValuesSize
                            * LUT_Indexes5ParameterValuesSize;
                    if (l_NbValues != l_IndexSize)
                    {
                        vnsExceptionDataMacro(
                                "The size of the buffered value is "<<l_NbValues << " and is incoherent with the size of the indexes parameters values "<< l_IndexSize)
                    }

                    for (unsigned int a = 0; a < LUT_Indexes0ParameterValuesSize; a++)
                    {
                        index[0] = a;
                        for (unsigned int b = 0; b < LUT_Indexes1ParameterValuesSize; b++)
                        {
                            index[1] = b;
                            for (unsigned int c = 0; c < LUT_Indexes2ParameterValuesSize; c++)
                            {
                                index[2] = c;
                                for (unsigned int d = 0; d < LUT_Indexes3ParameterValuesSize; d++)
                                {
                                    index[3] = d;
                                    for (unsigned int e = 0; e < LUT_Indexes4ParameterValuesSize; e++)
                                    {
                                        index[4] = e;
                                        for (unsigned int f = 0; f < LUT_Indexes5ParameterValuesSize; f++)
                                        {
                                            index[5] = f;
                                            // Number of components per pixel
                                            for (unsigned int lNumBand = 0; lNumBand < pNbBands; lNumBand++)
                                            {
                                                this->SetPixelValue(lPix, pBuffersBands[lNumBand][cpt], lNumBand);
                                            }
                                            pLUT->SetValue(index, lPix);
                                            cpt++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            // ----------------------------------------------------------
            // Generates LUT with 6D
            template<class TValue>
                void
                GenerateXDLUT(LookUpTable<TValue, 6> * pLUT, const ListOfListOfFloats & pBuffersBands, const unsigned int pNbBands)
                {
                    typedef LookUpTable<TValue, 6> LocalLookUpTableType;

                    vnsLogDebugMacro("Generate LUT "<<LocalLookUpTableType::ImageDimension <<"D ...")

                    // ----------------------------------------------------------
                    // Used only for VectorLUTs
                    // The pixel size is equal to the number of spectral band
                    typename LocalLookUpTableType::PixelType lPix = static_cast<typename LocalLookUpTableType::PixelType>(0);
                    this->SetPixelSize(lPix, pNbBands);

                    // ----------------------------------------------------------
                    // Index of the pixel
                    typename LocalLookUpTableType::IndexType index;
                    // ----------------------------------------------------------
                    // Fill to 0
                    index.Fill((typename LocalLookUpTableType::IndexValueType) 0);

                    // ----------------------------------------------------------
                    // Initialization
                    std::streamoff cpt(0);

                    // ----------------------------------------------------------
                    //Init parameters
                    const ParametersValuesType & LUT_Indexes(pLUT->GetParametersValues());
                    const unsigned int LUT_Indexes0ParameterValuesSize(LUT_Indexes.at(0).ParameterValues.size());
                    const unsigned int LUT_Indexes1ParameterValuesSize(LUT_Indexes.at(1).ParameterValues.size());
                    const unsigned int LUT_Indexes2ParameterValuesSize(LUT_Indexes.at(2).ParameterValues.size());
                    const unsigned int LUT_Indexes3ParameterValuesSize(LUT_Indexes.at(3).ParameterValues.size());
                    const unsigned int LUT_Indexes4ParameterValuesSize(LUT_Indexes.at(4).ParameterValues.size());
                    const unsigned int LUT_Indexes5ParameterValuesSize(LUT_Indexes.at(5).ParameterValues.size());

                    const std::size_t l_NbValues = pBuffersBands.front().size();
                    const std::size_t l_IndexSize = LUT_Indexes0ParameterValuesSize * LUT_Indexes1ParameterValuesSize
                            * LUT_Indexes2ParameterValuesSize * LUT_Indexes3ParameterValuesSize * LUT_Indexes4ParameterValuesSize
                            * LUT_Indexes5ParameterValuesSize;
                    if (l_NbValues != l_IndexSize)
                    {
                        vnsExceptionDataMacro(
                                "The size of the buffered value is "<<l_NbValues << " and is incoherent with the size of the indexes parameters values "<< l_IndexSize)
                    }

                    for (unsigned int a = 0; a < LUT_Indexes0ParameterValuesSize; a++)
                    {
                        index[0] = a;
                        for (unsigned int b = 0; b < LUT_Indexes1ParameterValuesSize; b++)
                        {
                            index[1] = b;
                            for (unsigned int c = 0; c < LUT_Indexes2ParameterValuesSize; c++)
                            {
                                index[2] = c;
                                for (unsigned int d = 0; d < LUT_Indexes3ParameterValuesSize; d++)
                                {
                                    index[3] = d;
                                    for (unsigned int e = 0; e < LUT_Indexes4ParameterValuesSize; e++)
                                    {
                                        index[4] = e;
                                        for (unsigned int f = 0; f < LUT_Indexes5ParameterValuesSize; f++)
                                        {
                                            index[5] = f;
                                            // Number of components per pixel
                                            for (unsigned int lNumBand = 0; lNumBand < pNbBands; lNumBand++)
                                            {
                                                this->SetPixelValue(lPix, pBuffersBands[lNumBand][cpt], lNumBand);
                                            }
                                            pLUT->SetValue(index, lPix);
                                            cpt++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            /** LUT Bands filename */
            Utilities::ListOfStrings m_BandFileName;

            /** LUT Object */
            typename LookUpTableType::Pointer m_LUT;
        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsLookUpTableFileReader.txx"
#endif

#endif
