#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
############################################################################################################
#                                                                                                          #
#                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         #
#                             o                                                                            #
#                          o                                                                               #
#                        o                                                                                 #
#                      o                                                                                   #
#                     o       ooooooo       ooooooo    o         o      oo                                 #
#    o              o       o        o     o       o   o         o     o   o                               #
#      o           o       o          o   o         o  o         o    o      o                             #
#        o        o       o           o   o         o  o         o    o        o                           #
#         o      o        o      oooo     o         o  o         o   o           o                         #
#          o    o          o              o         o  o         o   o           o                         #
#           o  o            o             o         o  o o      o   o          o                           #
#            oo              oooooooo    o         o   o  oooooo   o      oooo                             #
#                                                     o                                                    #
#                                                     o                                                    #
#                                                    o                            o                        #
#                                                    o            o      oooo     o   o      oooo          #
#                                                   o             o         o    o    o         o          #
#                                                   o            o       ooo     o   o       ooo           #
#                                                               o       o       o   o          o           #
#                                                               ooooo   oooo    o   ooooo  oooo            #
#                                                                              o                           #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
#  VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 8 novembre 2011 : Modification pour prise en compte            #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################


IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "dataIO")


SET(TESTS1 ${CXX_TEST_PATH}/vnsDataIOTests1)
SET(TESTS2 ${CXX_TEST_PATH}/vnsDataIOTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsDataIOTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(${PREFIX}XmlFileHandlerTemplateTest ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}XmlFileHandlerTemplateTest.xml
        ${TEMP}/${PREFIX}XmlFileHandlerTemplateTest.xml
    vnsXmlFileHandlerTemplateTest
    ${INPUT_MAJADATATU_IMAGES}/VE_FileClass_VSC_PDTIMG_L2VALD_NickName_ShortDate_FRE.HDR
    ${INPUT_MAJADATATU_IMAGES}/VE_TEST_VSC_L2VALD_CAMARGUE_20120101.HDR
    ${TEMP}/${PREFIX}XmlFileHandlerTemplateTest.xml
)


add_test(${PREFIX}GenericRSTransformNew ${TESTS1}  vnsGenericRSTransformNew )

add_test(${PREFIX}GenericRSTransform ${TESTS1}
         --compare-ascii ${NOTOL}
             ${BASELINEDATATU_FILES}/${PREFIX}GenericRSTransform.txt
             ${TEMP}/${PREFIX}GenericRSTransform.txt
          vnsGenericRSTransform
             1.35617289802566
             43.4876035537
             ${TEMP}/${PREFIX}GenericRSTransform.txt
)

add_test(${PREFIX}GenericRSTransformFromImage ${TESTS1}
               vnsGenericRSTransformFromImage
               ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
               )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsDataIOTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    GRIDImageIO Test Tu
ADD_TEST(${PREFIX}GRIBImageIONew ${TESTS2}
    vnsGRIBImageIONew
    )

# -------    GRIDImageIO Test CanRead
ADD_TEST(${PREFIX}GRIBImageIOTestCanReadGRIB ${TESTS2}
    vnsGRIBImageIOTestCanRead
	${INPUT_MAJADATATU_IMAGES}/METEO_2010030612
    )

# MACCS : GRIB tests comment because the GRIBImageIO->CanRead in not in charge of parse the :BandId. It in charge of the ImaegFIel Reader.
# -------    GRIDImageIO Test CanRead
#ADD_TEST(${PREFIX}GRIBImageIOTestCanReadGRIBWithBand ${TESTS2}
#    vnsGRIBImageIOTestCanRead
#	${INPUT_MAJADATATU_IMAGES}/METEO_2010030612:1 )

# -------    GRIDImageIO Test CanRead
#ADD_TEST(${PREFIX}GRIBImageIOTestCanReadGRIBWithOutsideBand ${TESTS2}
#    vnsGRIBImageIOTestCanRead
#	${INPUT_MAJADATATU_IMAGES}/METEO_2010030612:100 )
#SET_TESTS_PROPERTIES(${PREFIX}GRIBImageIOTestCanReadGRIBWithOutsideBand PROPERTIES WILL_FAIL true)

# -------    GRIDImageIO Test CanRead
#ADD_TEST(${PREFIX}GRIBImageIOTestCanReadNoGRIB ${TESTS2}
#    vnsGRIBImageIOTestCanRead
#	${INPUT_MAJADATATU_IMAGES}/METEO_201003061 )
#SET_TESTS_PROPERTIES(${PREFIX}GRIBImageIOTestCanReadNoGRIB PROPERTIES WILL_FAIL true)


# -------    Read GRID Image
ADD_TEST(${PREFIX}ReadGRIBImage ${TESTS2}
--compare-image ${NOTOL} 
   	        ${BASELINEDATATU_IMAGES}/grib_METEO_Band7.tif 
		${TEMP}/grib_band7.tif
    vnsReadGRIBImage
    ${INPUT_MAJADATATU_IMAGES}/METEO_2010030612?&resol=7
    ${TEMP}/grib_band7.tif
    )

# -------    Read GRID Image
ADD_TEST(${PREFIX}ExtractGRIBImage ${TESTS2}
--compare-image ${NOTOL} 
   	        ${BASELINEDATATU_IMAGES}/gribExtract_20_10_101_200.tif 
		${TEMP}/grib_band_20_10_101_200.tif
    vnsExtractGRIBImage
    ${INPUT_MAJADATATU_IMAGES}/METEO_2010030612?&resol=1
    ${TEMP}/grib_band_20_10_101_200.tif
    20   #startX
    10  #startY
    101 #sizeX
    200 #sizeY
    )


# -------    vnsMapLookUpTableFileReaderTest
ADD_TEST(${PREFIX}MapLookUpTableFileReaderTest ${TESTS2}
    vnsMapLookUpTableFileReaderTest
    ${INPUT_MAJADATATU_FILES}/LUT_x_x_x_x_TDIR_20100921_Float32.TIF
    ${TEMP}/LUT_x_x_x_x_TDIR_20100921_Float32.mha
    )
    
# ATTENTION: This test check the output of the previous test
#        ${BASELINEDATATU_IMAGES}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_FORMO_EUSOuest00A00B_20060810.mha
ADD_TEST(${PREFIX}MetaMHAReadWrite3DVectorImageTestLUT_L2_FORMOSAT ${TESTS2}
    --compare-image ${NOTOL} 
        ${INPUT_MAJADATATU_IMAGES}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_FORMO_Sudouest_20060810.mha
        ${TEMP}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_FORMO_Sudouest_20060810_COPY.mha
    vnsReadWrite3DVectorImage
    ${INPUT_MAJADATATU_IMAGES}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_FORMO_Sudouest_20060810.mha
    ${TEMP}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_FORMO_Sudouest_20060810_COPY.mha
    )
    

# ATTENTION: This test check the output of the previous test
ADD_TEST(${PREFIX}MetaMHAReadWrite3DVectorImageTestLUT_L2_VENUS ${TESTS2}
    --compare-image ${NOTOL} 
        ${BASELINEDATATU_IMAGES}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_VENUS_CAMARGUE_20120101_COPY.mha
        ${TEMP}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_VENUS_CAMARGUE_20120101_COPY.mha
    vnsReadWrite3DVectorImage
    ${INPUT_MAJADATATU_IMAGES}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_VENUS_CAMARGUE_20120101.mha
    ${TEMP}/${PREFIX}MapLookUpTableFileReaderRegisterLUT_VENUS_CAMARGUE_20120101_COPY.mha
    )

    
# -------    vnsMapLookUpTableFileReaderTest
ADD_TEST(${PREFIX}MetaMHAReadWrite3DVectorImageTest ${TESTS2}
    --compare-image ${NOTOL} 
        ${INPUT_MAJADATATU_IMAGES}/VE_TEST_FSC_LUTANX_L2VALD_Sudouest_20060819_LTC.DBL.mha
        ${TEMP}/${PREFIX}MetaMHAReadWrite3DVectorImageTest.mha
    vnsReadWrite3DVectorImage
    ${INPUT_MAJADATATU_IMAGES}/VE_TEST_FSC_LUTANX_L2VALD_Sudouest_20060819_LTC.DBL.mha
    ${TEMP}/${PREFIX}MetaMHAReadWrite3DVectorImageTest.mha
    )


SET(LIST_OF_IMAGES Romania_Extract_15Bands_80x80.tif Sudouest_20060329_MS_fmsat_ortho_toa_nua_96m.nua METEO_2010030612 F2_IPCLA.tif)
FOREACH(file ${LIST_OF_IMAGES})
    GET_FILENAME_COMPONENT(FILE_NAME "${file}" NAME )
    GET_FILENAME_COMPONENT(FILE_NAME_WE "${file}" NAME_WE )

    ADD_TEST(${PREFIX}ImageFileReaderTest_${FILE_NAME_WE} ${TESTS2}
        otbImageFileReaderTest
        --InputImage  ${INPUT_MAJADATATU_IMAGES}/${FILE_NAME}
        --OutputImage ${TEMP}/${PREFIX}otbImageFileReader_${FILE_NAME_WE}.tif
        --ImageType VI_F_2D
        --ModeStreamDivisions 1
        --TilingStreamDivisions 200
        --NbThreads 16)
ENDFOREACH(file)

SET(LIST_OF_BANDS 1 2 3 4 5 6 7 )
FOREACH(band ${LIST_OF_BANDS})
    ADD_TEST(${PREFIX}ImageFileReaderTest_METEO_2010030612_Band${band} ${TESTS2}
        otbImageFileReaderTest
        --InputImage  ${INPUT_MAJADATATU_IMAGES}/METEO_2010030612?&resol=${band}
        --OutputImage ${TEMP}/${PREFIX}otbImageFileReader_METEO_2010030612_Band${band}.tif
        --ImageType VI_F_2D
        --ModeStreamDivisions 1
        --TilingStreamDivisions 200
        --NbThreads 16)
ENDFOREACH(band)


ADD_TEST(${PREFIX}ImageFileReaderTest_MAJADATATU_Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m_tif ${TESTS2}
    otbImageFileReaderTest
    --InputImage  ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
    --OutputImage ${TEMP}/${PREFIX}ImageFileReaderTest_MAJADATATU_Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
    --ImageType VI_F_2D
    --ModeStreamDivisions 1
    --TilingStreamDivisions 200
    --NbThreads 16)
    
ADD_TEST(${PREFIX}ImageFileReaderTest_MAJADATATU_Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m ${TESTS2}
    otbImageFileReaderTest
    --InputImage  ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m
    --OutputImage ${TEMP}/${PREFIX}ImageFileReaderTest_MAJADATATU_Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m.tif
    --ImageType VI_F_2D
    --ModeStreamDivisions 1
    --TilingStreamDivisions 200
    --NbThreads 16)
ADD_TEST(${PREFIX}ImageFileReaderTest_MAJADATATU_Sudouest_20060329_MS_fmsat_ortho_toa_nua_96m_IPCLDAll_tif ${TESTS2}
    otbImageFileReaderTest
    --InputImage  ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060329_MS_fmsat_ortho_toa_nua_96m_IPCLDAll.tif
    --OutputImage ${TEMP}/${PREFIX}ImageFileReaderTest_MAJADATATU_Sudouest_20060329_MS_fmsat_ortho_toa_nua_96m_IPCLDAll.tif
    --ImageType VI_F_2D
    --ModeStreamDivisions 1
    --TilingStreamDivisions 200
    --NbThreads 16)
ADD_TEST(${PREFIX}ImageFileReaderTest_MAJADATATU_Sudouest_96m_mnt ${TESTS2}
    otbImageFileReaderTest
    --InputImage  ${INPUT_MAJADATATU_IMAGES_DTM}/Sudouest_96m.mnt
    --OutputImage ${TEMP}/${PREFIX}ImageFileReaderTest_MAJADATATU_Sudouest_96m_mnt.tif
    --ImageType VI_F_2D
    --ModeStreamDivisions 1
    --TilingStreamDivisions 200
    --NbThreads 16)

SET(LISTE_TAUX_COMPRESSION 20 40 60 80 90 95 )
FOREACH(taux ${LISTE_TAUX_COMPRESSION})
    ADD_TEST(${PREFIX}ImageFileReaderTest_CheckCompressionJPEG_${taux} ${TESTS2}
        --compare-image ${NOTOL} 
            ${BASELINEDATATU_IMAGES}/${PREFIX}ImageFileReaderTest_check_compress_jpeg_quality_${taux}.jpg
            ${TEMP}/${PREFIX}ImageFileReaderTest_check_compress_jpeg_quality_${taux}.jpg
        otbImageFileReaderTest
            --InputImage  ${INPUT_MAJADATATU_IMAGES}/check_compress_jpeg.tif
            --OutputImage ${TEMP}/${PREFIX}ImageFileReaderTest_check_compress_jpeg_quality_${taux}.jpg?&gdal:co:QUALITY=${taux}
            --ImageType VI_UC_2D
            --ModeStreamDivisions 1
            --TilingStreamDivisions 100
            --NbThreads 4)
ENDFOREACH(taux)


#IF(MAJA_TU_ENABLE_PERFORMANCES_TESTING)
IF(OFF)

    # 30 minutes = 1800 secondes
    # 45 minutes = 2700 secondes
    SET(MAJA_TIMEOUT_TU_PERF 2700)
    
    # TU test de perf sur Reader/Writer sur produit JPEG 2000
    SET(LISTE_MODE 0 1 2 3 4 5 )
    SET(LISTE_DIVISIONS 200 800 1024 )
    FOREACH(division ${LISTE_DIVISIONS})
        FOREACH(mode ${LISTE_MODE})
            SET(TU_NAME ${PREFIX}ImageFileReaderTest_Performance_ReadWrite_JPEG2000_S2_division_${division}_mode_${mode}) 
            ADD_TEST(${TU_NAME} ${TESTS2}
    # TODO THOMAS : activer copare mais das TU a part pour pas influencer time perf dans Dashboard.
    # Faire un TU Stub
    #            --compare-image ${NOTOL} 
    #                ${BASELINEDATATU_IMAGES}/${PREFIX}dataIOImageFileReaderTest_Performance_ReadWrite_JPEG2000_S2.tif
    #                ${TEMP}/${PREFIX}ImageFileReaderTest_Performance_ReadWrite_JPEG2000_S2_${division}_${mode}.tif
                otbImageFileReaderTest
                    --InputImage  ${INPUT_MAJADATA_IMAGES_L1_SENTINEL2_GPP}/S2A_TEST_PRD_GPPL1C_065_20091211165851_20091211165932_0003/S2A_TEST_IMG_GPPL1C_065_20091211165851_20091211165932_01_000000_15SVD_0003.jp2
                    --OutputImage ${TEMP}/${PREFIX}ImageFileReaderTest_Performance_ReadWrite_JPEG2000_S2_${division}_${mode}.tif
                    --ImageType VI_F_2D
                    --ModeStreamDivisions ${mode}
                    --TilingStreamDivisions ${division}
                    --NbThreads 4)
            SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
        ENDFOREACH(mode)
    ENDFOREACH(division)
    
    SET(LISTE_MODE 0 1 2 3 4 5 )
    SET(LISTE_DIVISIONS 200 800 1024 )
    SET(LISTE_NB_THREADS 1 8 )
    FOREACH(division ${LISTE_DIVISIONS})
        FOREACH(mode ${LISTE_MODE})
            FOREACH(thread ${LISTE_NB_THREADS})
                SET(TU_NAME ${PREFIX}ImageFileReaderTest_Performance_ReadWrite_BIG_TIFF_division_${division}_mode_${mode}_thread_${thread}) 
                ADD_TEST(${TU_NAME} ${TESTS2}
    # TODO THOMAS : activer copare mais das TU a part pour pas influencer time perf dans Dashboard.
    # Faire un TU Stub
    #            --compare-image ${NOTOL} 
    #                ${BASELINEDATATU_IMAGES}/${PREFIX}dataIOImageFileReaderTest_Performance_ReadWrite_JPEG2000_S2.tif
    #                ${TEMP}/${PREFIX}ImageFileReaderTest_Performance_ReadWrite_JPEG2000_S2_${division}_${mode}.tif
                otbImageFileReaderTest
                    --InputImage  ${INPUT_MAJADATATU_IMAGES}/dataIOImageFileReaderTest_Performance_ReadWrite_BIG_TIFF.tif
                    --OutputImage ${TEMP}/${PREFIX}ImageFileReaderTest_Performance_ReadWrite_BIG_TIFF_${division}_${mode}_thread_${thread}.tif
                    --ImageType VI_F_2D
                    --ModeStreamDivisions ${mode}
                    --TilingStreamDivisions ${division}
                    --NbThreads ${thread})
    			SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
            ENDFOREACH(thread)
        ENDFOREACH(mode)
    ENDFOREACH(division)
    
#ENDIF(MAJA_TU_ENABLE_PERFORMANCES_TESTING)
ENDIF(OFF)

#    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m
#    ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060329_MS_fmsat_ortho_toa_nua_96m_IPCLDAll.tif
#    ${INPUT_MAJADATATU_IMAGES_DTM}/Sudouest_96m.mnt

#------------------------ vnsOzoneValueExtractor ------------------------
# OK test
ADD_TEST(${PREFIX}OzoneValueExtractorTestOK ${TESTS2}
   vnsOzoneValueExtractorTest
${INPUT_MAJADATATU_IMAGES}/METEO_2010030612?&resol=7  # GRIB
5                                # latitude
180                              # longitude
0.0050822                        # grib file value  
)

# Wrong image format
ADD_TEST(${PREFIX}OzoneValueExtractorTestTif ${TESTS2}
   vnsOzoneValueExtractorTest
${INPUT_MAJADATATU_IMAGES}/poupees.tif         # GRIB
5                                # latitude
180                              # longitude
0.0050822                        # grib file value  
)
SET_TESTS_PROPERTIES(${PREFIX}OzoneValueExtractorTestTif PROPERTIES WILL_FAIL true)

# Wrong filename
ADD_TEST(${PREFIX}OzoneValueExtractorTestWrongPath ${TESTS2}
   vnsOzoneValueExtractorTest
${INPUT_MAJADATATU_IMAGES}/METEO_201003061?&resol=7   # GRIB
5                                # latitude
180                              # longitude
0.0050822                        # grib file value  
)
SET_TESTS_PROPERTIES(${PREFIX}OzoneValueExtractorTestWrongPath PROPERTIES WILL_FAIL true)

# wrong latitude
ADD_TEST(${PREFIX}OzoneValueExtractorTestLat ${TESTS2}
   vnsOzoneValueExtractorTest
${INPUT_MAJADATATU_IMAGES}/METEO_2010030612?&resol=7  # GRIB
-91                              # latitude
180                              # longitude
0.0050822                        # grib file value  
)
SET_TESTS_PROPERTIES(${PREFIX}OzoneValueExtractorTestLat PROPERTIES WILL_FAIL true)

# negative longitude
ADD_TEST(${PREFIX}OzoneValueExtractorTestNegativeLong ${TESTS2}
   vnsOzoneValueExtractorTest
${INPUT_MAJADATATU_IMAGES}/METEO_2010030612?&resol=7  # GRIB
5                                # latitude
-3.5                             # longitude
0.00537678433789                 # grib file value  
)
# wrong longitude
ADD_TEST(${PREFIX}OzoneValueExtractorTestLong ${TESTS2}
   vnsOzoneValueExtractorTest
${INPUT_MAJADATATU_IMAGES}/METEO_2010030612?&resol=7  # GRIB
5                                # latitude
360                              # longitude
0.0050822                        # grib file value  
)
SET_TESTS_PROPERTIES(${PREFIX}OzoneValueExtractorTestLong PROPERTIES WILL_FAIL true)



# Src1 sources files
SET(SRCS1
vnsDataIOTests1.cxx
vnsXmlFileHandlerReadXPathTest.cxx
vnsXmlFileHandlerTemplateTest.cxx
vnsCheckOriginAndSpacingAndSizeOfImagesTest.cxx
vnsGenericRSTransform.cxx
vnsGenericRSTransformNew.cxx
vnsGenericRSTransformFromImage.cxx
vnsGenericRSTransformGenericTest.cxx
)

# Src2 sources files
SET(SRCS2
vnsDataIOTests2.cxx
vnsGRIBImageIONew.cxx
vnsGRIBImageIOTestCanRead.cxx
vnsReadGRIBImage.cxx
vnsExtractGRIBImage.cxx
vnsMapLookUpTableFileReaderTest.cxx
vnsReadWrite3DVectorImage.cxx
vnsImageFileReaderTest.cxx
vnsOzoneValueExtractorTest.cxx
)


ADD_EXECUTABLE(vnsDataIOTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsDataIOTests1
  ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )

ADD_EXECUTABLE(vnsDataIOTests2 ${SRCS2})
TARGET_LINK_LIBRARIES(vnsDataIOTests2
  ${OTB_LIBRARIES}  
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )

ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
