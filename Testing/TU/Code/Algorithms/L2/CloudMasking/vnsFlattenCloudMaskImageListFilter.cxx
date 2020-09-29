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

#include "vnsFlattenCloudMaskImageListFilter.h"
#include <map>
#include <vector>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsDataManagerTestingLibrary.h"
#include "vnsUtilities.h"

int
vnsFlattenCloudMaskImageListFilter(int argc, char * argv[])
{
    if (argc < 3)
    {
        std::cerr << argv[0]
            << " <Outputfilename>"
            << " <Maskfilename_1>"
            << " <Maskfilename_2>"
            << " <Maskfilename_3>"
            << " <...>"
            << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int cpt = 1;

    const unsigned int Dimension = 2;
    const char * Outputfilename = argv[cpt++];

    std::vector<char const*> MaskFilenameList(&argv[cpt], &argv[argc]);

    /** Pixel typedefs */
    typedef unsigned char InputMaskPixelType;;
    typedef unsigned char OutputMaskPixelType;

    /** Image typedefs */
    typedef otb::Image<InputMaskPixelType, Dimension>  InputMaskType;
    typedef otb::Image<OutputMaskPixelType, Dimension> OutputMaskType;

    typedef otb::ImageFileReader<InputMaskType>  InputMaskReaderType;
    typedef otb::ImageFileWriter<OutputMaskType> WriterType;

    typedef vns::FlattenCloudMaskImageListFilter<InputMaskType, OutputMaskType> FilterType;
    typedef otb::ImageList<InputMaskType> LabelImageListType;

    // -------
    // Readers
    // -------
    std::vector<InputMaskReaderType::Pointer> l_InputReaderList;
    LabelImageListType::Pointer l_InputImageList = LabelImageListType::New();
    for (std::size_t i=0, N=MaskFilenameList.size(); i!=N ; ++i)
    {
        std::cout << "Input #"<<i<<": " << MaskFilenameList[i] << "\n";
        InputMaskReaderType::Pointer l_InputReader = InputMaskReaderType::New();
        l_InputReader->SetFileName(MaskFilenameList[i]);
        l_InputImageList->PushBack(l_InputReader->GetOutput());
        l_InputReaderList.push_back(l_InputReader);
    }
    
    // -------
    // Filter
    // -------
    FilterType::Pointer l_Filter = FilterType::New();
    l_Filter->SetInput(l_InputImageList);
    l_Filter->Update();

    // --------
    // Writers
    // --------
    WriterType::Pointer l_OutputWriter = WriterType::New();
    l_OutputWriter->SetFileName(Outputfilename);
    l_OutputWriter->SetInput(l_Filter->GetOutput());
    l_OutputWriter->Update();

    return EXIT_SUCCESS;
}
