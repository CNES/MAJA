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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsCirrusReflectanceFunctor.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "itkCastImageFilter.h"


#include "itkBinaryFunctorImageFilter.h"

int
vnsCirrusReflectanceFunctorTest(int argc, char * argv[])
{
    if (argc != 8)
    {
        std::cerr << argv[0] << " <Cirrus input filename> <DTM input filename> "
                <<"<Cirrus Mask filename> <Refl output filename>   <CirrusMaskThresholdGain>"
                <<" <m_CirrusMaskThresholdOffset> <CirrusThresholdFactor>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_CirrusFileName = argv[1];
    const char * l_DTMFileName = argv[2];
    const char * l_CirrusMaskFileName = argv[3];
    const char * l_ReflOutFileName = argv[4];
    const double l_CirrusMaskThresholdGain = atof(argv[5]);
    const double l_CirrusMaskThresholdOffset = atof(argv[6]);
    const double l_CirrusMaskThresholdFactor = atof(argv[7]);

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned short MaskPixelType;
    typedef double OutputPixelType;

    /** Image typedefs */
    typedef otb::Image<InputPixelType, Dimension> InputImageType;
    typedef otb::Image<MaskPixelType, Dimension> MaskImageType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileReader<MaskImageType> MaskReaderType;

    typedef vns::Functor::CirrusReflectanceFunctor<InputPixelType, InputPixelType, OutputPixelType> FunctorType;
    typedef itk::BinaryFunctorImageFilter<InputImageType, InputImageType, OutputImageType,FunctorType> FilterType;

    //Output typedef
    typedef otb::Image<short>  OutImageType;
    typedef otb::MultiplyByScalarImageFilter<InputImageType, InputImageType> MultiplyFilterType;
    typedef itk::CastImageFilter<InputImageType, OutImageType> CastImageFilterType;
    typedef otb::ImageFileWriter<OutImageType> WriterType;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_CirrusReader = ReaderType::New();
    ReaderType::Pointer l_DTMReader = ReaderType::New();
    MaskReaderType::Pointer l_MaskReader = MaskReaderType::New();

    WriterType::Pointer l_Writer = WriterType::New();

    l_CirrusReader->SetFileName(l_CirrusFileName);
    l_DTMReader->SetFileName(l_DTMFileName);
    l_MaskReader->SetFileName(l_CirrusMaskFileName);

    l_Writer->SetFileName(l_ReflOutFileName);

    l_Filter->SetInput1(l_DTMReader->GetOutput());
    l_Filter->SetInput2(l_CirrusReader->GetOutput());
    l_Filter->GetFunctor().SetCirrusThresholdFactor(l_CirrusMaskThresholdFactor);
    l_Filter->GetFunctor().SetMaskThresholdGain(l_CirrusMaskThresholdGain);
    l_Filter->GetFunctor().SetMaskThresholdOffset(l_CirrusMaskThresholdOffset);

    MultiplyFilterType::Pointer l_Multiply = MultiplyFilterType::New();
    l_Multiply->SetInput(l_Filter->GetOutput());
    l_Multiply->SetCoef(1000);

    CastImageFilterType::Pointer l_Caster = CastImageFilterType::New();
    l_Caster->SetInput(l_Multiply->GetOutput());

    l_Writer->SetInput(l_Caster->GetOutput());

    l_Writer->Update();

    return EXIT_SUCCESS;
}
