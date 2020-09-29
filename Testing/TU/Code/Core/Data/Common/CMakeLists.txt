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

IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "dataCommon")


SET(TESTS1 ${CXX_TEST_PATH}/vnsDataCommonTests1)
SET(TESTS2 ${CXX_TEST_PATH}/vnsDataCommonTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsDataPDSTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    Logger class tests
FOREACH(std "cout" "cerr" )
FOREACH(level DEBUG INFO PROGRESS WARNING ERROR)
  ADD_TEST(${PREFIX}LoggerTest${level}-${std} ${TESTS1}
    vnsLoggerTest
    ${level}
    ProcessorName
    ${std}
    ${TEMP}/${PREFIX}LoggerTest_${level}_${std}.txt
    1 # Nb Threads
    )
ENDFOREACH(level)
ENDFOREACH(std)

# -------    Loggers class tests
FOREACH(level DEBUG INFO PROGRESS WARNING ERROR)
  ADD_TEST(${PREFIX}LoggersTest${level} ${TESTS1}
    vnsLoggersTest
    ${level}
    ProcessorName
    ${TEMP}/${PREFIX}LoggersTest_${level}.txt)
ENDFOREACH(level)


# -------    Date test
ADD_TEST(${PREFIX}DateTest ${TESTS1}
    vnsDateTest
    ${TEMP}/${PREFIX}DateTest.txt
)

# -------    System test
ADD_TEST(${PREFIX}SystemTest ${TESTS1}
    vnsSystemTest
)

ADD_TEST(${PREFIX}SystemFilenameTest1 ${TESTS1}
    vnsSystemFilenameTest
    ${INPUT_MAJADATATU_IMAGES}/file_name_test.one.txt
    )

# -------    System test
ADD_TEST(${PREFIX}SystemCallCommandBasicTest ${TESTS1}
    vnsSystemCallCommandBasicTest
    ${GDAL_APPS_DIR}/gdalinfo
    "--version"
)

# -------    System test
ADD_TEST(${PREFIX}SystemCallCommandBasicGdalRasterizeDET_FOOTPRINT ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}SystemCallCommandBasicGdalRasterizeDET_FOOTPRINT.tif
    ${TEMP}/${PREFIX}SystemCallCommandBasicGdalRasterizeDET_FOOTPRINT.tif
    vnsSystemCallCommandBasicTest
    ${GDAL_APPS_DIR}/gdal_rasterize
    -a fid 
    -sql 
    \"select fid, * from MaskFeature\" 
    -at
    -ts
    500 # size_x
    500 # size_y
    ${INPUT_MAJADATATU_IMAGES}/S2A_TEST_MSK_GPPL1C_065_03_15SVD_DET_FOOTPRINT.gml
    ${TEMP}/${PREFIX}SystemCallCommandBasicGdalRasterizeDET_FOOTPRINT.tif
)

ADD_TEST(${PREFIX}CheckTest_RasterizeDET_FOOTPRINT ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}SystemCallCommandBasicGdalRasterizeDET_FOOTPRINT.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}SystemCallCommandBasicGdalRasterizeDET_FOOTPRINT.tif
    vnsCheckTestMain 
    )

# -------    System test
ADD_TEST(${PREFIX}SystemCallCommandBasicGdalRasterizePIX_NODATA ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}SystemCallCommandBasicGdalRasterizePIX_NODATA.tif
    ${TEMP}/${PREFIX}SystemCallCommandBasicGdalRasterizePIX_NODATA.tif
    vnsSystemCallCommandBasicTest
    ${GDAL_APPS_DIR}/gdal_rasterize
    -a fid 
    -sql 
    \"select fid, * from MaskFeature\" 
    -at
    -ts
    500 # size_x
    500 # size_y
    ${INPUT_MAJADATATU_IMAGES}/S2A_TEST_MSK_GPPL1C_065_02_15SVD_PIX_NODATA.gml
    ${TEMP}/${PREFIX}SystemCallCommandBasicGdalRasterizePIX_NODATA.tif
)

# -------    System test
ADD_TEST(${PREFIX}SystemCallCommandBadTest ${TESTS1}
    vnsSystemCallCommandBasicTest
    ${GDAL_APPS_DIR}/gdalinfo 
    "--toto"
)
SET_TESTS_PROPERTIES(${PREFIX}SystemCallCommandBadTest PROPERTIES WILL_FAIL true)


# -------    System test
ADD_TEST(${PREFIX}SystemCallCommandGdalRasterizeTest ${TESTS1}
    vnsSystemCallCommandBasicTest
    ${GDAL_APPS_DIR}/gdal_rasterize
    "--version"
)


# -------    LUT test
ADD_TEST(${PREFIX}LookUpTableNew ${TESTS1}
    vnsLookUpTableNew
)

ADD_TEST(${PREFIX}LookUpTableLUT3 ${TESTS1}
    vnsLookUpTableLUT3
)

# -------    LUT interpolation test
ADD_TEST(${PREFIX}LookUpTableLUTInterpolation ${TESTS1}
    vnsLookUpTableLUTInterpolation
)

# -------    LUT extraction test

# -------    LUT 3 -> LUT 2
ADD_TEST(${PREFIX}LookUpTableLUTExtraction3_2ParametersNamesAndValues ${TESTS1}
    vnsLookUpTableLUTExtraction3_2ParametersNamesAndValues 0.220 ${EPSILON_7}
)

ADD_TEST(${PREFIX}LookUpTableLUTExtraction3_2BorderStartCheck ${TESTS1}
    vnsLookUpTableLUTExtraction3_2 0.090 0.2500 0.0 ${EPSILON_7}
)

ADD_TEST(${PREFIX}LookUpTableLUTExtraction3_2MiddleCheck ${TESTS1}
    vnsLookUpTableLUTExtraction3_2 0.120 0.3126 31.0 ${EPSILON_7}
)

ADD_TEST(${PREFIX}LookUpTableLUTExtraction3_2BorderEndCheck ${TESTS1}
    vnsLookUpTableLUTExtraction3_2 0.230 0.4375 90.0 ${EPSILON_7}
)

ADD_TEST(${PREFIX}LookUpTableLUTExtraction3_2BorderStartExtract ${TESTS1}
    vnsLookUpTableLUTExtraction3_2 0.000 0.3126 31.0 ${EPSILON_7}
)

ADD_TEST(${PREFIX}LookUpTableLUTExtraction3_2BorderEndExtract ${TESTS1}
    vnsLookUpTableLUTExtraction3_2 0.240 0.3126 31.0 ${EPSILON_7}
)

# -------    LUT 6 -> LUT 3
ADD_TEST(${PREFIX}LookUpTableLUTExtraction6_3Middle ${TESTS1}
    vnsLookUpTableLUTExtraction6_3 0.120 0.3126 31.0 1500.0 27.0 0.48 ${EPSILON_7}
)

# -------    VectorLUT test
ADD_TEST(${PREFIX}VectorLookUpTableNew ${TESTS2}
    vnsVectorLookUpTableNew
)

ADD_TEST(${PREFIX}VectorLookUpTableLUT3 ${TESTS2}
    vnsVectorLookUpTableLUT3
)

# -------    VectorLUT interpolation test
ADD_TEST(${PREFIX}VectorLookUpTableLUTInterpolation ${TESTS2}
    vnsVectorLookUpTableLUTInterpolation
)


# -------    vnsExceptionsTests
ADD_TEST(${PREFIX}ExceptionsClassTests ${TESTS1}
    vnsExceptionsClassTests
)


ADD_TEST(${PREFIX}UtilitiesUTCDateToJulianDay ${TESTS2}
    vnsUtilitiesUTCDate
    "UTC=2012-01-03T10:51:44"
    2455930.452592 # Julian day attendu
    )

# -------    vnsImageListPixelIterator
ADD_TEST(${PREFIX}ImageListPixelIterator ${TESTS2}
    vnsImageListPixelIterator
    )

ADD_TEST(${PREFIX}UtilitiesTesting ${TESTS2}
    vnsUtilitiesTesting
    )


# Src1 sources files
SET(SRCS1
vnsDataCommonTests1.cxx
vnsDateTest.cxx
vnsCheckTestMain.cxx
vnsLogger.cxx
vnsLoggers.cxx
vnsSystem.cxx
vnsSystemFilename.cxx
vnsSystemCallCommandBasic.cxx
vnsSystemReadBinaryFile.cxx
vnsLookUpTableNew.cxx
vnsLookUpTableLUT3.cxx
vnsLookUpTableLUTInterpolation.cxx
vnsLookUpTableLUTExtraction3_2.cxx
vnsLookUpTableLUTExtraction3_2ParametersNamesAndValues.cxx
vnsLookUpTableLUTExtraction6_3.cxx
vnsExceptionsTests.cxx
)

SET(SRCS2
vnsDataCommonTests2.cxx
vnsVectorLookUpTableNew.cxx
vnsVectorLookUpTableLUT3.cxx
vnsVectorLookUpTableLUTInterpolation.cxx
#vnsVectorLookUpTableLUTExtraction6_3.cxx
vnsUtilitiesTesting.cxx
vnsUtilitiesUTCDate.cxx
vnsImageListPixelIterator.cxx
)


ADD_EXECUTABLE(vnsDataCommonTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsDataCommonTests1
  ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )

ADD_EXECUTABLE(vnsDataCommonTests2 ${SRCS2})
TARGET_LINK_LIBRARIES(vnsDataCommonTests2
  ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )

ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
