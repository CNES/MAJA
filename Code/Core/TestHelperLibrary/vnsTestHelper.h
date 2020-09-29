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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 mars 2017 : Refactoring pour ameliorer la qualite     *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153176-CS : 3 fevrier 2017 : Modifications pour désactiver           *
 *                          l'incrustation de texte dans les images jpg des produits QCK. Suppression de la *
 *                          gestion de masque dasn les baseline pour que les comparaisons des résultats sur *
 *                          les redhat 5, 6 et 7 soient plus simples à géréer                               *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 20 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 14 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144776-CS : 10 mai 2016 : Correction recherche multibaseline          *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-354-CNES : 8 juin 2012 : Correction qualite (regle ou metrique)       *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 17 decembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsTestHelper_h
#define __vnsTestHelper_h

#include <vnsStreamingDifferenceImageFilter.h>
#include <map>
#include <string>
#include <vector>
#include "otbMetaDataKey.h"

#include "vnsLoggers.h"
#include "itkMacro.h"
#include "itkObject.h"

// OGR Headers files
#include "ogrsf_frmts.h"
#include "vnsUtilities.h"
#include <string.h>
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"
#include "otbStreamingShrinkImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkImageFileWriter.h"

namespace vns
{
    /**
     * \class TestHelper
     * \brief Helper class to perform the baseline comparisons during the tests
     */
    class /*ITK_EXPORT*/TestHelper : public itk::Object
    {
    public:
        /** Standard class typedefs. */
        typedef TestHelper Self;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;
        typedef itk::Object Superclass;

        itkTypeMacro(TestHelper, itk::Object)
        itkNewMacro(Self)

        TestHelper() :
                m_EpsilonBoundaryChecking(1.0e-30), m_ReportErrors(false), m_Verbose(false), m_IgnoreLineOrder(false), m_ToleranceDiffValue(
                        0.), m_Epsilon(0.)
        {
        }

        ~TestHelper()
        {
        }

        typedef std::vector<std::string> StringList;
        typedef StringList::const_iterator StringListIt;
        typedef std::map<std::string, std::string> StringMap;
        typedef StringMap::const_iterator StringMapIt;

        int
        RegressionTestAllImages(const StringList&, const StringList&, const StringMap&, const std::string &);
        int
        RegressionTestAllMetaData(const StringList&, const StringList&);
        int
        RegressionTestAllAsciiFiles(const StringList&, const StringList&, const StringMap&, const StringList&, const std::string &);
        int
        RegressionTestAllBinaryFiles(const StringList&, const StringList&, const StringMap&);

        int
        RegressionTestCheckPerformances(const unsigned int p_CheckPerfosRamMax, const unsigned int p_CheckPerfosTimeMax,
                const std::string & p_CheckPerfosXmlOutptuResult);

        static bool
        IsAnImageFilename(const std::string & p_filename);

        static bool
        IsAnAsciiFilename(const std::string & p_filename);

        static bool
        IsAMAJAConfigFilename(const std::string & p_filename);

        static bool
        IsAnBinaryFilename(const std::string & p_filename);

        static bool
        IsAMultiBaselineFilename(const std::string & p_filename);

        static bool
        IsHaveMultiBaselineFilename(const std::string & p_filename);

        static bool
        IsAMaskBaselineFilename(const std::string & p_filename);

        static bool
        IsFilenameMatchWithKey(const std::string & p_filename, const StringList & selectedFilenamesWithKeyMatching);

        static bool
        IsFilenameNoMatchWithKey(const std::string & p_filename, const StringList & excludedFilenamesWithKeyMatching);

        static void
        RegressionGetListOfAllImagesFilesFromDirectory(StringList& p_baselineFilenames, StringList& p_testFilenames,
                const std::string & p_ReferenceInputDirectory, const std::string & p_ReferenceOutDirectory,
                const std::string & p_ReferenceOutDirectory2, const StringList & selectedFilenamesWithKeyMatchingImage,
                const bool p_recursively);

        static Utilities::ListOfStrings
        ExcludeGlobalFilenamesWithKeyMatching(const std::string & p_ReferenceInputDirectory,
                const Utilities::ListOfStrings & p_InputListOfFilenames, const StringList & p_ExcludeSudDirectories);

        static StringMap
        RegressionGetListOfAllAbsenceImageFilesFromDirectory(const std::string & p_ReferenceInputDirectory,
                const std::string & p_ReferenceOutDirectory, const StringList & selectedFilenamesWithKeyMatchingImage,
                const StringList & excludedGlobalFilenamesWithKeyMatching, const bool p_recursively);

        static void
        RegressionGetListOfAllAsciiFilesFromDirectory(StringList& p_baselineFilenames, StringList& p_testFilenames,
                const std::string & p_ReferenceInputDirectory, const std::string & p_ReferenceOutDirectory,
                const std::string & p_ReferenceOutDirectory2, const StringList & selectedFilenamesWithKeyMatchingAscii,
                const StringList & excludedFilenamesWithKeyMatchingAscii, const bool p_recursively);

        static StringMap
        RegressionGetListOfAllAbsenceAsciiFilesFromDirectory(const std::string & p_ReferenceInputDirectory,
                const std::string & p_ReferenceOutDirectory, const StringList & selectedFilenamesWithKeyMatchingAscii,
                const StringList & excludedFilenamesWithKeyMatchingAscii, const StringList & excludedGlobalFilenamesWithKeyMatching,
                const bool p_recursively);

        static void
        RegressionGetListOfAllBinaryFilesFromDirectory(StringList& p_baselineFilenames, StringList& p_testFilenames,
                const std::string & p_ReferenceInputDirectory, const std::string & p_ReferenceOutDirectory,
                const std::string & p_ReferenceOutDirectory2, const StringList & selectedFilenamesWithKeyMatchingImage,
                const bool p_recursively);

        static StringMap
        RegressionGetListOfAllAbsenceBinaryFilesFromDirectory(const std::string & p_ReferenceInputDirectory,
                const std::string & p_ReferenceOutDirectory, const StringList & selectedFilenamesWithKeyMatchingBinary,
                const StringList & excludedGlobalFilenamesWithKeyMatching, const bool p_recursively);

        //        namespace Functor
//        {
        template<class TInputPixel, class TMaskPixel, class TOutputPixel>
            class TestApplyMaskImageFunctor
            {
            public:
                TestApplyMaskImageFunctor(void)
                {
                }

                virtual
                ~TestApplyMaskImageFunctor(void)
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TMaskPixel MaskPixelType;
                typedef TOutputPixel OutputPixelType;
                typedef typename OutputPixelType::ValueType OutputInternalPixelType;

                inline OutputPixelType
                operator()(const InputPixelType & inPix, const MaskPixelType &maskPix) const
                {
                    OutputPixelType outPix;
                    const unsigned int l_size = inPix.Size();
                    // Set output pixel dimension
                    outPix.SetSize(l_size);

                    // Test if the pixel value of the mask is greater than the minimum threshold value
                    if (maskPix == 0)
                    {
                        // Set pixel to OutputValue for all bands
                        outPix.Fill(static_cast<OutputInternalPixelType>(0));

                    }
                    else
                    {
                        // Band Loop
                        for (unsigned int i = 0; i < l_size; i++)
                        {
                            // Keep the input pixel value
                            outPix[i] = static_cast<OutputInternalPixelType>(inPix[i]);
                        }
                    }
                    return outPix;
                }
            };

        // } // end namespace functor

        std::map<std::string, int>
        RegressionTestbaselines(char *baselineFilename) const;
      
        template<class TReader, class TWriter>
            int
            RegressionTestImage(const char *testImageFilename, const char *baselineImageFilename, const double toleranceDiffPixelImage,
                    const std::string & baseOutputdir, int & p_binchecksum, bool & p_check_origin, bool & p_check_spacing,
                    bool & p_check_pixel_componenttype, std::string & fullUpdateCommandLine) const
            {
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // Use the factory mechanism to read the test and baseline files and convert them to double
                typedef otb::VectorImage<double, 2> ImageType;

                typedef TReader ReaderType;
                typedef typename ReaderType::OutputImageType InputImageType;
                typedef typename InputImageType::PixelType InputPixelType;

                typedef TWriter WriterType;

                // ---------------------------------------------------------------------------------------------
                std::string testImageFilenameBase(baseOutputdir);
                testImageFilenameBase += "/" + itksys::SystemTools::GetFilenameName(testImageFilename);
                // ---------------------------------------------------------------------------------------------
                if ((m_ReportErrors) || (m_Verbose == true))
                // ---------------------------------------------------------------------------------------------
                {
                    std::cout
                            << "********************************************************************************************************************************************************************************"
                            << std::endl;
                    std::cout << "      START compare the '" << itksys::SystemTools::GetFilenameName(testImageFilename) << "' image file"
                            << std::endl;
                    std::cout << "              Baseline image: " << baselineImageFilename << std::endl;
                    std::cout << "              Test image:     " << testImageFilename << std::endl;
                    std::cout << "      => update baseline :" << std::endl;
                    std::cout << "         cp " << testImageFilename << " " << baselineImageFilename << ";" << std::endl;
                    fullUpdateCommandLine += std::string("cp ") + testImageFilename + " " + baselineImageFilename + ";";
                    std::cout << "      => view commands :" << std::endl;
                    std::cout << "         mont.bash " << testImageFilename << " " << baselineImageFilename << " &" << std::endl;
                    std::cout << "         qgis.bash " << testImageFilename << " " << baselineImageFilename << " &" << std::endl;
                    std::cout << "      => diff commands :" << std::endl;
                    std::cout << "         simplediffer.bash " << testImageFilename << " " << baselineImageFilename << " "
                            << testImageFilename << ".diff.tif" << std::endl;
                    std::cout << "      => diff + stat diff command :" << std::endl;
                    std::cout << "         simplediffer.bash " << testImageFilename << " " << baselineImageFilename << " "
                            << testImageFilename << ".diff.tif ; stater.bash " << testImageFilename << ".diff.tif" << std::endl;
                    std::cout << "      => diff + stat diff + mont command :" << std::endl;
                    std::cout << "         simplediffer.bash " << testImageFilename << " " << baselineImageFilename << " "
                            << testImageFilename << ".diff.tif ; stater.bash " << testImageFilename << ".diff.tif ; mont.bash "
                            << testImageFilename << " " << baselineImageFilename << " " << testImageFilename << ".diff.tif" << std::endl;
                    std::cout << "      => diff + stat diff + qgis command :" << std::endl;
                    std::cout << "         simplediffer.bash " << testImageFilename << " " << baselineImageFilename << " "
                            << testImageFilename << ".diff.tif ; stater.bash " << testImageFilename << ".diff.tif ; qgis.bash "
                            << testImageFilename << " " << baselineImageFilename << " " << testImageFilename << ".diff.tif &" << std::endl;

                    std::cout << "         differenceimagefilter.bash " << testImageFilename << " " << baselineImageFilename << " "
                            << testImageFilename << ".diff.tif" << std::endl;
                }
                // ---------------------------------------------------------------------------------------------
                unsigned int cptfic = 0;
                bool continuer(true);
                // ---------------------------------------------------------------------------------------------
                while (continuer)
                {
                    // ---------------------------------------------------------------------------------------------
                    cptfic++;
                    std::ostringstream diffName2;
                    std::ostringstream baseName2;
                    std::ostringstream testName2;
                    diffName2 << testImageFilenameBase << cptfic << ".diff.png";
                    testName2 << testImageFilenameBase << cptfic << ".test.png";
                    baseName2 << testImageFilenameBase << cptfic << ".base.png";
                    if (itksys::SystemTools::FileExists(diffName2.str().c_str()) || itksys::SystemTools::FileExists(testName2.str().c_str())
                            || itksys::SystemTools::FileExists(baseName2.str().c_str()))
                        continuer = true;
                    else
                        continuer = false;
                    // ---------------------------------------------------------------------------------------------
                    if (cptfic > 1000)
                    {
                        itkGenericExceptionMacro(<< "Nb image test file to big");
                        return 1500;
                    }
                }

                // ---------------------------------------------------------------------------------------------
                std::ostringstream diffName;
                std::ostringstream baseName;
                std::ostringstream testName;
                diffName << testImageFilenameBase << cptfic << ".diff.png";
                testName << testImageFilenameBase << cptfic << ".test.png";
                baseName << testImageFilenameBase << cptfic << ".base.png";

                // ====================================================================================================================================
                // ====================================================================================================================================
                // ====================================================================================================================================
                // VENUS Specificities : for full images (SRE, FRE), do a md5sum (=check the binary contain) before compare pixels images)
                std::string stringtestImageFilename = itksys::SystemTools::GetFilenameName(testImageFilename);

                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                p_binchecksum = 0;
                if (m_ReportErrors == 0)
                {
                    p_binchecksum = RegressionTestBinaryFile(testImageFilename, baselineImageFilename);
                    if (m_Verbose == true)
                    {
                        std::cout << "     -> 'BINARY' compare image for '" << stringtestImageFilename << " result: " << p_binchecksum
                                << std::endl;
                    }
                    if (p_binchecksum == 0)
                    {
                        if (m_Verbose == true)
                        {
                            std::cout << "     -> 'BINARY' compare image for '" << stringtestImageFilename << "'   ok." << std::endl;
                        }
                        return 0;
                    }
                }
                // ====================================================================================================================================
                // ====================================================================================================================================
                // ====================================================================================================================================

                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // Read the baseline file
                typename ReaderType::Pointer baselineReader = ReaderType::New();
                baselineReader->SetFileName(baselineImageFilename);
                std::string l_baselineImageFilename(baselineImageFilename);
                if (itksys::SystemTools::FileExists(l_baselineImageFilename.c_str()) == false)
                {
                    std::cout << " ---------------    Create baseline : " << std::endl;
                    std::cout << " cp " << testImageFilename << " " << baselineImageFilename << ";" << std::endl;
                    itkGenericExceptionMacro(<< "ERROR : The baseline image filename '"<<baselineImageFilename<<"' doesn't exist !!")
                    return 1000;
                }
                try
                {
                    // Since 4-1 : seg fault avec UpdateLargestPossibleRegion sur grosse image: il passe 2 heures à faire charger l'image puis segfault dan sgdal rasterio.
                    //            baselineReader->UpdateLargestPossibleRegion();
                    baselineReader->UpdateOutputInformation();
                }
                catch (itk::ExceptionObject& e)
                {
                    itkGenericExceptionMacro(<< "Exception detected while reading " << baselineImageFilename << " : " << e.GetDescription());
                    return 1000;
                }

                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // Read the file generated by the test
                typename ReaderType::Pointer testReader = ReaderType::New();
                testReader->SetFileName(testImageFilename);
                std::string l_testImageFilename(testImageFilename);
                if (itksys::SystemTools::FileExists(l_testImageFilename.c_str()) == false)
                {
                    itkGenericExceptionMacro(<< "ERROR : The test image filename '"<<testImageFilename<<"' doesn't exist !!")
                    return 1000;
                }
                try
                {
                    // Since 4-1 : seg fault avec UpdateLargestPossibleRegion sur grosse image: il passe 2 heures à faire charger l'image puis segfault dan sgdal rasterio.
                    //            testReader->UpdateLargestPossibleRegion();
                    testReader->UpdateOutputInformation();
                }
                catch (itk::ExceptionObject& e)
                {
                    itkGenericExceptionMacro(<< "Exception detected while reading " << testImageFilename << " : " << e.GetDescription());
                    return 1000;
                }

                p_check_origin = true;
                p_check_spacing = true;
                /*        std::cout << "GetComponentTypeAsString : "
                 << baselineReader->GetImageIO()->GetComponentTypeAsString(baselineReader->GetImageIO()->GetComponentType()) << std::endl;
                 std::cout << "GetComponentType : " << baselineReader->GetImageIO()->GetComponentType() << std::endl;
                 std::cout << "GetComponentSize : " << baselineReader->GetImageIO()->GetComponentSize() << std::endl;
                 std::cout << "GetImageSizeInComponents : " << baselineReader->GetImageIO()->GetImageSizeInComponents() << std::endl;
                 std::cout << "GetByteOrder : " << baselineReader->GetImageIO()->GetByteOrder() << std::endl;
                 std::cout << "GetByteOrderAsString : "
                 << baselineReader->GetImageIO()->GetByteOrderAsString(baselineReader->GetImageIO()->GetByteOrder()) << std::endl;
                 std::cout << "GetFileTypeAsString : "
                 << baselineReader->GetImageIO()->GetFileTypeAsString(baselineReader->GetImageIO()->GetFileType()) << std::endl;
                 //        std::cout << "GetFileTypeAsString : "<< baselineReader->GetImageIO()->GetPixelSize(()<< std::endl;
                 std::cout << "GetPixelTypeAsString : "
                 << baselineReader->GetImageIO()->GetPixelTypeAsString(baselineReader->GetImageIO()->GetPixelType()) << std::endl;*/

                if ((m_ReportErrors) || (m_Verbose == true))
                {
                    std::cout << "Baseline image : Origin                       : " << baselineReader->GetOutput()->GetOrigin()
                            << std::endl;
                    std::cout << "                 Spacing                      : " << baselineReader->GetOutput()->GetSpacing()
                            << std::endl;
                    std::cout << "                 Size                         : "
                            << baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
                    std::cout << "                 NumberOfComponentsPerPixel   : "
                            << baselineReader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
                    std::cout << "                 ComponentType                : "

                    << baselineReader->GetImageIO()->GetComponentTypeAsString(baselineReader->GetImageIO()->GetComponentType())
                            << std::endl;
                    std::cout << "                 ComponentSize                : " << baselineReader->GetImageIO()->GetComponentSize()
                            << std::endl;
                    std::cout << "Test image     : Origin                       : " << testReader->GetOutput()->GetOrigin() << std::endl;
                    std::cout << "                 Spacing                      : " << testReader->GetOutput()->GetSpacing() << std::endl;
                    std::cout << "                 Size                         : "
                            << testReader->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
                    std::cout << "                 NumberOfComponentsPerPixel   : "
                            << testReader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
                    std::cout << "                 ComponentType                : "
                            << testReader->GetImageIO()->GetComponentTypeAsString(testReader->GetImageIO()->GetComponentType())
                            << std::endl;
                    std::cout << "                 ComponentSize                : " << testReader->GetImageIO()->GetComponentSize()
                            << std::endl;
                }
                if (baselineReader->GetOutput()->GetOrigin() != testReader->GetOutput()->GetOrigin())
                {
                    p_check_origin = false;
                }
                if (baselineReader->GetOutput()->GetSpacing() != testReader->GetOutput()->GetSpacing())
                {
                    p_check_spacing = false;
                }
                if (baselineReader->GetImageIO()->GetComponentType() != testReader->GetImageIO()->GetComponentType())
                {
                    p_check_pixel_componenttype = false;
                }
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // The sizes of the baseline and test image must match
                ImageType::SizeType baselineSize;
                baselineSize = baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize();
                ImageType::SizeType testSize;
                testSize = testReader->GetOutput()->GetLargestPossibleRegion().GetSize();

                if (baselineSize != testSize)
                {
                    if ((m_ReportErrors != 0) || (m_Verbose == true))
                    {
		      if (m_Verbose)
			{
			  std::cerr << "The size of the baseline image and Test image do not match!" << std::endl;
			  std::cerr << "baseline image: " << baselineImageFilename << " has size " << baselineSize << std::endl;
			  std::cerr << "Test image:     " << testImageFilename << " has size " << testSize << std::endl;
			  std::string fname_baseline(basename(baselineImageFilename));
			  std::string fname_test(basename(testImageFilename));
			  std::cout << "<DartMeasurementFile name=\"BaselineImage (";
			  std::cout << fname_baseline << ")\" type=\"image/png\">";
			  std::cout << baselineImageFilename << "</DartMeasurementFile>" << std::endl;
			  
			  std::cout << "<DartMeasurementFile name=\"TestImage (";
			  std::cout << fname_test << ")\" type=\"image/png\">";
			  std::cout << testImageFilename << "</DartMeasurementFile>" << std::endl;
			}
                    }

                    return 1;
                }
                const unsigned int nb_bands_baseline = baselineReader->GetOutput()->GetNumberOfComponentsPerPixel();
                const unsigned int nb_bands_test = testReader->GetOutput()->GetNumberOfComponentsPerPixel();
                if (nb_bands_baseline != nb_bands_test)
                {
                    if ((m_ReportErrors != 0) || (m_Verbose == true))
                    {
                        std::cerr << "The number of bands of the baseline image and Test image do not match!" << std::endl;
                        std::cerr << "nb bands baseline : " << nb_bands_baseline << " for image file " << baselineImageFilename
                                << std::endl;
                        std::cerr << "         test     : " << nb_bands_test << "                " << testImageFilename << std::endl;
                    }
                    return 1;
                }

                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // Check if there is a mask in the baseline : baselineImageFilename + ".mask.tif or .mask....

                // Find a mask image file
                const std::string baselineImageFilenameMaskTif = baselineImageFilename + std::string(".mask.tif");
                const std::string baselineImageFilenameMaskJpg = baselineImageFilename + std::string(".mask.jpg");
                const std::string baselineImageFilenameMaskPng = baselineImageFilename + std::string(".mask.png");
                std::string baselineImageFilenameMask("");
                bool l_ApplyMask(false);
                if (itksys::SystemTools::FileExists(baselineImageFilenameMaskTif.c_str()) == true)
                {
                    l_ApplyMask = true;
                    baselineImageFilenameMask = baselineImageFilenameMaskTif;
                }
                else if (itksys::SystemTools::FileExists(baselineImageFilenameMaskJpg.c_str()) == true)
                {
                    l_ApplyMask = true;
                    baselineImageFilenameMask = baselineImageFilenameMaskJpg;
                }
                else if (itksys::SystemTools::FileExists(baselineImageFilenameMaskPng.c_str()) == true)
                {
                    l_ApplyMask = true;
                    baselineImageFilenameMask = baselineImageFilenameMaskPng;
                }

                // Disable mask processing
                l_ApplyMask = false;
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // Now compare the two images
                typedef vns::StreamingDifferenceImageFilter<ImageType,ImageType> DiffType;

                // ---------------------------------------------------------------------------------------------
                DiffType::Pointer diff = DiffType::New();

                typedef unsigned char MaskPixelType;
                typedef otb::Image<MaskPixelType, 2> MaskImageType;
                typedef otb::ImageFileReader<MaskImageType> MaskReaderType;

                typedef TestApplyMaskImageFunctor<InputPixelType, MaskPixelType, InputPixelType> ApplyMaskFunctorType;
                typedef itk::BinaryFunctorImageFilter<InputImageType, MaskImageType, InputImageType, ApplyMaskFunctorType> ApplyMaskFilterType;

                typename ApplyMaskFilterType::Pointer l_BaselineApplyMask = ApplyMaskFilterType::New();
                typename ApplyMaskFilterType::Pointer l_TestApplyMask = ApplyMaskFilterType::New();
                typename MaskReaderType::Pointer l_ReaderMask = MaskReaderType::New();
                typename ApplyMaskFilterType::Pointer l_OutputApplyMask = ApplyMaskFilterType::New();

                if (l_ApplyMask == true)
                {
                    if (m_Verbose == false)
                    {
                        std::cout << " Apply mask on baseline image and test image (" << baselineImageFilenameMask << ")..." << std::endl;
                    }
                    l_ReaderMask->SetFileName(baselineImageFilenameMask.c_str());

                    l_BaselineApplyMask->SetInput1(baselineReader->GetOutput());
                    l_BaselineApplyMask->SetInput2(l_ReaderMask->GetOutput());
                    l_TestApplyMask->SetInput1(testReader->GetOutput());
                    l_TestApplyMask->SetInput2(l_ReaderMask->GetOutput());

                    diff->SetTestInput(l_BaselineApplyMask->GetOutput());
                    diff->SetValidInput(l_TestApplyMask->GetOutput());

                }
                else
                {
                    diff->SetTestInput(baselineReader->GetOutput());
                    diff->SetValidInput(testReader->GetOutput());

                }

                diff->SetDifferenceThreshold(toleranceDiffPixelImage);
                if (m_Verbose == true)
                {
                    std::cout << " Before diff update ! " << std::endl;
                }
                try
                {
                    // Since 4-1 : seg fault avec UpdateLargestPossibleRegion sur grosse image: il passe 2 heures à faire charger l'image puis segfault dan sgdal rasterio.

                    diff->UpdateOutputInformation();
                    //            diff->UpdateLargestPossibleRegion();
                    diff->Update();
                    // ---------------------------------------------------------------------------------------------
                }
                catch (itk::ExceptionObject& e)
                {
                    if ((m_ReportErrors) || (m_Verbose == true))
                    {
                        std::cout << "itkExceptionObject detected while updating the  otb::DifferenceImageFilter filter:" << std::endl;
                        std::cout << " message :  " << e.GetDescription() << std::endl;
                    }
                    return 1;
                }
                catch (...)
                {
                    if ((m_ReportErrors) || (m_Verbose == true))
                    {
                        std::cout << " Erreur sur le filtre otb::DifferenceImageFilter sur image " << testImageFilename << std::endl;
                    }
                    return 1;
                }
                ImageType::PixelType statusDif = diff->GetTotalDifference();
                const unsigned long numberOfPixelsWithDifferences = diff->GetNumberOfPixelsWithDifferences();

                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                if ((statusDif.GetSquaredNorm() > 0 && m_ReportErrors) || (m_Verbose == true))
                {
                    std::cout
                            << "      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "
                            << std::endl;
                    std::cout << "   Results compare image  *  Difference threshold                 : " << toleranceDiffPixelImage
                            << std::endl;
                    std::cout << "                          *  Diff filter: " << std::endl;
                    std::cout << "                               .  diff->GetTotalDifference():                     "
                            << diff->GetTotalDifference() << std::endl;
                    std::cout << "                               .  diff->GetMeanDifference():                      "
                            << diff->GetMeanDifference() << std::endl;
                    std::cout << "                               .  diff->GetNumberOfPixelsWithDifferences():       "
                            << diff->GetNumberOfPixelsWithDifferences() << std::endl;
                    std::cout << "   (value for decision)       -> statusDiff.GetSquaredNorm():                     "
                            << statusDif.GetSquaredNorm() << std::endl;

                    // ---------------------------------------------------------------------------------------------
                    typedef otb::StreamingShrinkImageFilter<ImageType, ImageType> ShrinkerType;
                    typedef otb::PrintableImageFilter<ImageType> RescaleType;
                    ShrinkerType::Pointer shrink = ShrinkerType::New();
                    RescaleType::Pointer rescale = RescaleType::New();
                    typename WriterType::Pointer writer = WriterType::New();

                    // ---------------------------------------------------------------------------------------------
                    unsigned int ratio = 1;
                    unsigned int sizeMax = 512;

                    diff->UpdateOutputInformation();

                    // ---------------------------------------------------------------------------------------------
                    // ---------------------------------------------------------------------------------------------
                    if ((diff->GetOutput()->GetLargestPossibleRegion().GetSize()[0] > sizeMax)
                            || (diff->GetOutput()->GetLargestPossibleRegion().GetSize()[1] > sizeMax))
                    {
                        unsigned int ratioX = static_cast<unsigned int>(vcl_ceil(
                        static_cast<double>(diff->GetOutput()->GetLargestPossibleRegion().GetSize()[0]) / static_cast<double>(sizeMax) + 0.5));
                        unsigned int ratioY = static_cast<unsigned int>(vcl_ceil(
                        static_cast<double>(diff->GetOutput()->GetLargestPossibleRegion().GetSize()[1]) / static_cast<double>(sizeMax) + 0.5));

                        ratio = std::max(ratioX, ratioY);
                        shrink->SetShrinkFactor(ratio);
                    }

                    // ---------------------------------------------------------------------------------------------
                    std::string shortFilename = itksys::SystemTools::GetFilenameName(testImageFilename);
                    std::cout
                            << "   = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = ="
                            << std::endl;
                    std::cout << "   * Create the quicklooks images..." << std::endl;
                    std::cout
                            << "      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "
                            << std::endl;
                    std::cout << "      Shrink ratio computed     : " << ratio << std::endl;
                    std::cout << "      for max image size submit : " << sizeMax << std::endl;
                    std::cout
                            << "      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "
                            << std::endl;
                    std::cout << "      Images diff produced: " << std::endl;
                    std::cout << "              DiffImage:      " << diffName.str().c_str() << std::endl;
                    std::cout << "              BaseImage:      " << baseName.str().c_str() << std::endl;
                    std::cout << "              Testimage:      " << testName.str().c_str() << std::endl;

                    writer->SetInput(rescale->GetOutput());

                    std::cout
                            << "      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "
                            << std::endl;
                    std::cout << "      Print 'DartMeasurement' informations to submit on the CDash... " << std::endl;
                    //    std::cout << "<DartMeasurement name=\"ImageError\" type=\"numeric/double\">";
                    //        std::cout << "<DartMeasurement name=\"ImageError " << cpt << "\" type=\"numeric/double\">";
                    std::cout << "<DartMeasurement name=\"ImageError " << shortFilename << "\" type=\"numeric/double\">";
                    std::cout << statusDif;
                    std::cout << "</DartMeasurement>" << std::endl;

                    //        std::cout << "<DartMeasurement name=\"NumberOfPixelsWithDifferences " << cpt << "\" type=\"numeric/integer\">";
                    std::cout << "<DartMeasurement name=\"NumberOfPixelsWithDifferences " << shortFilename
                            << "\" type=\"numeric/integer\">";
                    std::cout << numberOfPixelsWithDifferences;
                    std::cout << "</DartMeasurement>" << std::endl;

                    std::cout << "<DartMeasurement name=\"ToleranceDiffPixelImage\" type=\"numeric/double\">";
                    std::cout << toleranceDiffPixelImage;
                    std::cout << "</DartMeasurement>" << std::endl;

                    try
                    {
                        if (ratio != 1)
                        {
                            shrink->SetInput(diff->GetOutput());
                            shrink->Update(); // StreamingShrinkImageFilter is a Persistent filter !!
                            rescale->SetInput(shrink->GetOutput());
                        }
                        else
                        {
                            rescale->SetInput(diff->GetOutput());
                        }

                        for (unsigned int i = 1; i <= std::min(diff->GetOutput()->GetNumberOfComponentsPerPixel(), 3U); ++i)
                        {
                            rescale->SetChannel(i);
                        }

                        writer->SetInput(rescale->GetOutput());
                        //      rescale->Update();
                    }
                    catch (itk::ExceptionObject& e)
                    {
                        itkGenericExceptionMacro(
                                "itkExceptionObject during rescale of " << diffName.str() << " \n message :  " << e.GetDescription());
                    }
                    catch (...)
                    {
                        itkGenericExceptionMacro(<< "Error during rescale of " << diffName.str());
                    }
                    writer->SetFileName(diffName.str().c_str());
                    try
                    {
                        writer->Update();
                    }
                    catch (...)
                    {
                        itkGenericExceptionMacro(<< "Error during write of " << diffName.str());
                    }

                    //    std::cout << "<DartMeasurementFile name=\"DifferenceImage\" type=\"image/png\">";
                    std::cout << "<DartMeasurementFile name=\"DifferenceImage " << shortFilename << "\" type=\"image/png\">";
                    std::cout << diffName.str();
                    std::cout << "</DartMeasurementFile>" << std::endl;

                    //            std::ostringstream baseName;
                    //            baseName << testImageFilename << ".base.png";
                    try
                    {
                        if (ratio != 1)
                        {
                            shrink->SetInput(baselineReader->GetOutput());
                            shrink->Update(); // StreamingShrinkImageFilter is a Persistent filter !!
                            rescale->SetInput(shrink->GetOutput());
                        }
                        else
                        {
                            rescale->SetInput(baselineReader->GetOutput());
                        }
                        //      rescale->Update();
                    }
                    catch (...)
                    {
                        itkGenericExceptionMacro(<< "Error during rescale of " << baseName.str());
                    }
                    try
                    {
                        writer->SetFileName(baseName.str().c_str());
                        writer->Update();
                    }
                    catch (...)
                    {
                        itkGenericExceptionMacro(<< "Error during write of " << baseName.str());
                    }

                    std::cout << "<DartMeasurementFile name=\"BaselineImage " << shortFilename << "\" type=\"image/png\">";
                    std::cout << baseName.str();
                    std::cout << "</DartMeasurementFile>" << std::endl;

                    //            std::ostringstream testName;
                    //            testName << testImageFilename << ".test.png";
                    try
                    {
                        if (ratio != 1)
                        {
                            shrink->SetInput(testReader->GetOutput());
                            shrink->Update(); // StreamingShrinkImageFilter is a Persistent filter !!
                            rescale->SetInput(shrink->GetOutput());
                        }
                        else
                        {
                            rescale->SetInput(testReader->GetOutput());
                        }
                        //      rescale->Update();
                    }
                    catch (...)
                    {
                        itkGenericExceptionMacro(<< "Error during rescale of " << testName.str());
                    }
                    try
                    {
                        writer->SetFileName(testName.str().c_str());
                        writer->Update();
                    }
                    catch (...)
                    {
                        itkGenericExceptionMacro(<< "Error during write of " << testName.str());
                    }

                    //    std::cout << "<DartMeasurementFile name=\"TestImage\" type=\"image/png\">";
                    std::cout << "<DartMeasurementFile name=\"TestImage " << shortFilename << "\" type=\"image/png\">";
                    std::cout << testName.str();
                    std::cout << "</DartMeasurementFile>" << std::endl;

                }
                if (statusDif.GetSquaredNorm() > 0)
                {
                    if ((m_ReportErrors) || (m_Verbose == true))
                    {
                        std::cout << "     -> 'COMPARE_IMAGE' image for '" << stringtestImageFilename << "'   FAILED." << std::endl;
                    }
                    return 1;
                }
                else
                {
                    if ((m_ReportErrors) || (m_Verbose == true))
                    {
                        std::cout << "     -> 'COMPARE_IMAGE' image for '" << stringtestImageFilename << "'   ok." << std::endl;
                    }
                    return 0;
                }
            }

        int
        RegressionTestBinaryFile(const char * testBinaryFileName, const char * baselineBinaryFileName) const;

        int
        RegressionTestAsciiFile(const char * testAsciiFileName, const char * baselineAsciiFileName, const double epsilon,
                std::vector<std::string> ignoredLines, const std::string & baseOutputdir, std::string & fullUpdateCommandLine) const;

        int
        RegressionTestMetaData(const char *testImageFilename, const char *baselineImageFilename,
                const double toleranceDiffPixelImage) const;

        void
        ReportErrorsOn()
        {
            m_ReportErrors = true;
        }
        void
        ReportErrorsOff()
        {
            m_ReportErrors = false;
        }

        void
        VerboseOn()
        {
            m_Verbose = true;
        }
        void
        VerboseOff()
        {
            m_Verbose = false;
        }

        void
        IgnoreLineOrderOn()
        {
            m_IgnoreLineOrder = true;
        }
        void
        IgnoreLineOrderOff()
        {
            m_IgnoreLineOrder = false;
        }

        void
        SetEpsilonBoundaryChecking(double epsilonBoundary)
        {
            m_EpsilonBoundaryChecking = epsilonBoundary;
        }

        itkSetMacro(ToleranceDiffValue, double)
        ;itkSetMacro(Epsilon, double)
        ;

    private:
        bool
        isNumber(int i) const;
        bool
        isHexaNumber(int i) const;
        bool
        isPoint(int i) const;
        bool
        isMinusSign(int i) const;
        bool
        isNumeric(std::string str) const;
        bool
        isScientificNumeric(std::string str) const;
        bool
        isHexaPointerAddress(std::string str) const;
        std::string
        VectorToString(otb::MetaDataKey::VectorType vector) const;
        bool
        CompareLines(std::string strfileref, std::string strfiletest, int& nbdiff, std::ofstream& fluxfilediff, int& numLine,
                std::vector<std::string>& listStrDiffLineFileRef, std::vector<std::string>& listStrDiffLineFileTest, double epsilon) const;

        double m_EpsilonBoundaryChecking;
        bool m_ReportErrors;
        bool m_Verbose;
        bool m_IgnoreLineOrder;
        double m_ToleranceDiffValue;
        double m_Epsilon;

    };
}

/****************************************************************************
 Macros used to check input test parameters
 ****************************************************************************/

#define vnsCheckInputArgumentsMacro(arg,value) \
    arg = value; \
    vnsLogDebugMacro(#arg<<" = "<<arg);

#endif
