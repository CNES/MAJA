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
 * VERSION : 5.0 : <TypeFT> : <NumFT> :
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudGeomMasksToKeptLabelMasksFilterTest.cxx 11787 2016-03-14 16:43:17Z abricier $
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsCloudGeomMasksToKeptLabelMasksFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include <map>


#include "vnsDataManagerTestingLibrary.h"

#include "vnsMacro.h"
#include "vnsUtilities.h"

int
vnsCloudGeomMasksToKeptLabelMasksFilterTest(int argc, char * argv[])
{
    if (argc != 5)
    {
        std::cerr << argv[0]
                          << " <LabeledShadowZonefilename_1>"
                          << " <LabeledShadowZonefilename_2>"
                          << " <LabeledShadowZonefilename_3>"
                          << " <Outputfilename>"
                          << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int cpt = 1;

    const unsigned int Dimension = 2;
    const char * LabeledShadowZonefilename_1 = argv[cpt++];
    const char * LabeledShadowZonefilename_2 = argv[cpt++];
    const char * LabeledShadowZonefilename_3 = argv[cpt++];
    const char * Outputfilename = argv[cpt++];

    /** Parameters definition */
    std::map<unsigned int, unsigned long> l_ShadowPixelsNumberMap;
    l_ShadowPixelsNumberMap[1] = 10;
    l_ShadowPixelsNumberMap[2] = 10;
    l_ShadowPixelsNumberMap[3] = 10;

    std::map<unsigned int, unsigned long> l_CloudPixelsNumberMap;
    l_CloudPixelsNumberMap[1] = 10;
    l_CloudPixelsNumberMap[2] = 20;
    l_CloudPixelsNumberMap[3] = 30;

    double l_ShadowCloudRatioMin = 0.5;
    double l_ShadowCloudRatioMax = 1.;
    unsigned int l_ShadowCloudRatioPixNumberThreshold = 10;

    /** Pixel typedefs */
    typedef unsigned int LabelPixelType;
    typedef unsigned char OutputMaskPixelType;

    /** Image typedefs */
    typedef otb::Image<LabelPixelType, Dimension>      LabelImageType;
    typedef otb::Image<OutputMaskPixelType, Dimension> OutputMaskType;

    typedef otb::ImageFileReader<LabelImageType> InputLabelReaderType;
    typedef otb::ImageFileWriter<OutputMaskType> WriterType;

    typedef vns::CloudGeomMasksToKeptLabelMasksFilter<LabelImageType, OutputMaskType> FilterType;
    typedef otb::ImageList<LabelImageType> LabelImageListType;


    FilterType::Pointer l_Filter = FilterType::New();
    InputLabelReaderType::Pointer l_InputReader_1 = InputLabelReaderType::New();
    InputLabelReaderType::Pointer l_InputReader_2 = InputLabelReaderType::New();
    InputLabelReaderType::Pointer l_InputReader_3 = InputLabelReaderType::New();
    WriterType::Pointer            l_OutputWriter = WriterType::New();

    // -------
    // Readers
    // -------
    l_InputReader_1->SetFileName(LabeledShadowZonefilename_1);
    l_InputReader_2->SetFileName(LabeledShadowZonefilename_2);
    l_InputReader_3->SetFileName(LabeledShadowZonefilename_3);

    LabelImageListType::Pointer l_InputImageList = LabelImageListType::New();
    l_InputImageList->PushBack(l_InputReader_1->GetOutput());
    l_InputImageList->PushBack(l_InputReader_2->GetOutput());
    l_InputImageList->PushBack(l_InputReader_3->GetOutput());


    l_Filter->SetInput(l_InputImageList);
    l_Filter->SetShadowCloudRatioMin(l_ShadowCloudRatioMin);
    l_Filter->SetShadowCloudRatioMax(l_ShadowCloudRatioMax);
    l_Filter->SetShadowCloudRatioPixNumberThreshold(l_ShadowCloudRatioPixNumberThreshold);
    l_Filter->SetShadowPixelsNumberMap(l_ShadowPixelsNumberMap);
    l_Filter->SetCloudPixelsNumberMap(l_CloudPixelsNumberMap);
    l_Filter->Update();


    // --------
    // Writers
    // --------

    l_OutputWriter->SetFileName(Outputfilename);
    l_OutputWriter->SetInput(l_Filter->GetOutput());
    l_OutputWriter->Update();

    return EXIT_SUCCESS;
}
