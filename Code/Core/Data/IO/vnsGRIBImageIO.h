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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 12 octobre 2016 : Audit code - Correction qualite :       *
 *                                                               Regle : Missing parameter name in function *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 12 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsGRIBImageIO_h
#define __vnsGRIBImageIO_h

#include "otbImageIOBase.h"
#include "vnsMacro.h"
#include <fstream>
#include <string>
#include <vector>

namespace vns
{

    /** \class GRIBImageIO
     *
     * \brief ImageIO object for reading (not writing) GRIB format images
     *
     * The streaming read is implemented.
     * Two ways to open the image: precising the image name AND the bandID using a ":" as separator (myGribFileName:bandId)
     * or simply give the image name, in that case, the first band will be read.
     * The indicator of parameter value is stored in the class.
     * A GRIB file a series of grids. Each grid is constructed as follow:
     * \begin{itemize}
     * \item an indicatrice section (1-8th bytes). First 4 bytes describs "GRIB".
     * \item a parameter definition section (9-36th bytes)
     * \item a grid description section (32 bytes)
     * \item a binary data section (43572 or 389892 bytes)
     * \item a end section (4 last bytes = 7777)
     * \end{itemize}
     * GRIB data are stored as binary data, values have to be reconstructed. For that we first compute
     * the scale factor, the reference value and the decimale scale factor.
     * The scale factor (\textit{E}) is coded in 2 bytes. The first bit is the sign, other are the absolute value.
     * The reference value (\textit{R}) is coded in 4 bytes. The sign (\textit{S}) is the first bit,
     * the caracteristic (\textit{A}) the seven following and the mantissa is the 24 last bits.
     * A and B are absolute values.
     * The decimal scale factor (\textit{D}) is read in the definition parameter section.
     * R is computed as follow:
     * \f[ R = (-1)^S.2^{-24}.B.16^{(A-64)} \f]
     *
     * Thus, the real value of an extracted grid value (Y) is computed as:
     * \f[ Y = (R+X.2^E).10^{-D} \f]
     *
     * One big issue in this kind of file is that the grid value are coded in
     * 12 bits. Thus, we choose to read series of 3 bytes (2*12bits) and compute 2 pixels
     * at each loop.
     *
     * For more explanation, please report to the CNES documentation
     * \textit{Specifications d'interface externe entre la METEO et le segment sol POLDER/PARSOL}
     *
     * Pay attention, we only consider the BigEndian case.
     *
     * \ingroup IOFilters
     *
     */
    class GRIBImageIO : public otb::ImageIOBase
    {
    public:

        /** Standard class typedefs. */
        typedef GRIBImageIO Self;
        typedef otb::ImageIOBase Superclass;
        typedef itk::SmartPointer<Self> Pointer;

        /** Byte order typedef */
        typedef Superclass::ByteOrder ByteOrder;

        /** Method for creation through the object factory. */
        itkNewMacro (Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(GRIBImageIO, otb::ImageIOBase);

        /** ParameterInd Accessors */
        itkGetMacro(ParameterInd, short int);

        /** Set/Get the dataset index to extract (starting at 0)*/
        itkSetMacro(DatasetNumber, unsigned int);itkGetMacro(DatasetNumber, unsigned int);

        /*-------- This part of the interface deals with reading data. ------ */

        /** Determine the file type. Returns true if this ImageIO can read the
         * file specified. */
        virtual bool
        CanReadFile(const char* filename);

        /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
        virtual bool
        CanStreamRead()
        {
            return (true);
        }

        /** Set the spacing and dimention information for the set filename. */
        virtual void
        ReadImageInformation();

        /** Reads the data from disk into the memory buffer provided. */
        virtual void
        Read(void* buffer);

        /** Reads 3D data from multiple files assuming one slice per file. */
        virtual void
        ReadVolume(void* buffer);

        /*-------- This part of the interfaces deals with writing data. ----- */

        /** Determine the file type. Returns true if this ImageIO can read the
         * file specified. */
        virtual bool
        CanWriteFile(const char*)
        {
            return (false);
        }

        /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
        virtual bool
        CanStreamWrite()
        {
            return (false);
        }

        /** Writes the spacing and dimentions of the image.
         * Assumes SetFileName has been called with a valid file name. */
        virtual void
        WriteImageInformation()
        {
            vnsExceptionDataMacro("Writing GRIB method is NOT implemented !")
        }

        /** Writes the data to disk from the memory buffer provided. Make sure
         * that the IORegion has been set properly. */
        virtual void
        Write(const void*)
        {
            vnsExceptionDataMacro("Writing GRIB method is NOT implemented !")
        }
      /* since OTB6 */
      /** Get the number of overviews available into the file specified
       *  This imageIO didn't support overviews */
      unsigned int GetOverviewsCount() ITK_OVERRIDE
      {
	// MANTIS-1154: Source image is always considered as the best
	// resolution overview.
	return 1;
      }
      
      /** Get information about overviews available into the file specified
       * This imageIO didn't support overviews */ 
      std::vector<std::string> GetOverviewsInfo() ITK_OVERRIDE
      {
	std::vector<std::string> desc;
	return desc;
      }
      
      /** Provide hist about the output container to deal with complex pixel
       *  type (Not used here) */ 
      void SetOutputImagePixelType( bool itkNotUsed(isComplexInternalPixelType), 
				    bool itkNotUsed(isVectorImage)) ITK_OVERRIDE
      {
	
      }

    protected:
        /** Construtor.*/
        GRIBImageIO();
        /** Destructor.*/
        virtual
        ~GRIBImageIO();

        bool
        OpenDataFileForReading(const char* filename);
        //bool OpenHeaderFileForReading(const char* filename);
        void
        InternalReadImageInformation();

        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
        GRIBImageIO(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented

        /** Internal method to read header informations */
        bool
        InternalReadHeaderInformation(std::fstream & file, const bool reportError);

        std::fstream m_File;

        /** Dataset index to extract (starting at 0)*/
        unsigned int m_DatasetNumber;
        /** Reference value */
        double m_RefValue;
        /** Scale factor value */
        double m_ScaleFact;
        /** Decimale scale factor value */
        unsigned short int m_DecScaleFact;
        /** Paramater indicator, compute in the process */
        short int m_ParameterInd;
        /** Position of the grid inthe file */
        int m_GridPosition;

        static const unsigned long int m_Const256;
        static const unsigned long int m_Const256_2;
        static const unsigned long int m_Const256_3;
    };

} // end namespace vns

#endif // __vnsGRIBImageIO_h
