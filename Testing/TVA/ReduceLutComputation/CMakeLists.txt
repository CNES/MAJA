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
#                        o     o                                                                           #
#                        oo   oo   oo        o   oo        ,-.                                             #
#                        o o o o  o  o       o  o  o       \_/                                             #
#                        o  o  o o    o      o o    o     {|||D                                            #
#                        o     o oooooo      o oooooo      / \                                             #
#                        o     o o    o o    o o    o      `-^                                             #
#                        o     o o    o  oooo  o    o                                                      #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 04 janvier 2019 : Creation                                        #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################

maja_test_application(NAME apTvCamsCompute
                     APP  CAMSComputation
                     OPTIONS -camsdescription ${MAJADATA_TVA_INPUT}/CAMSComputationTest/CAMS_Description_File.xml
                             -models seasalt Dust organicmatter blackcarbon sulphate
                             -rhsampling 30 50 70 80 85 90 95
                             -limaod 0.95
                             -after.modellevels 0.2 0.3843 0.6365 0.9564 1.3448 1.8058 2.3478 2.985 3.7397 4.6462 5.7565 
                             7.1322 8.8366 10.9483 13.5647 16.8064 20.8227 25.7989 31.9642 39.6029 49.0671 60.1802 
                             73.0663 87.7274 104.229 122.614 142.902 165.089 189.147 215.025 242.652 272.059 303.217 
                             336.044 370.407 406.133 443.009 480.791 519.209 557.973 596.777 635.306 673.24 710.263 
                             746.063 780.346 812.83 843.263 871.42 897.112 920.189 940.551 958.148 972.987 985.14 
                             994.747 1002.02 1007.26 1010.85 1013.25 
                             -before.modellevels 0.2 0.3843 0.6365 0.9564 1.3448 1.8058 2.3478 2.985 3.7397 4.6462 5.7565 
                             7.1322 8.8366 10.9483 13.5647 16.8064 20.8227 25.7989 31.9642 39.6029 49.0671 60.1802 
                             73.0663 87.7274 104.229 122.614 142.902 165.089 189.147 215.025 242.652 272.059 303.217 
                             336.044 370.407 406.133 443.009 480.791 519.209 557.973 596.777 635.306 673.24 710.263 
                             746.063 780.346 812.83 843.263 871.42 897.112 920.189 940.551 958.148 972.987 985.14 
                             994.747 1002.02 1007.26 1010.85 1013.25                              
                             -lat 43.742933
                             -lon 4.92374204531563
                             -dateutc UTC=2016-06-14T10:32:31
                             -before.dateutc UTC=2016-06-14T03:00:00
                             -before.aotfile ${MAJADATA_TVA_INPUT}/CAMSComputationTest/CAMS_AOT_20160614UTC030000.nc
                             -before.rhfile ${MAJADATA_TVA_INPUT}/CAMSComputationTest/CAMS_RH_20160614UTC030000.nc
                             -before.mrfile ${MAJADATA_TVA_INPUT}/CAMSComputationTest/CAMS_MR_20160614UTC030000.nc
                             -after.dateutc UTC=2016-06-15T03:00:00
                             -after.aotfile ${MAJADATA_TVA_INPUT}/CAMSComputationTest/CAMS_AOT_20160615UTC030000.nc
                             -after.rhfile ${MAJADATA_TVA_INPUT}/CAMSComputationTest/CAMS_RH_20160615UTC030000.nc
                             -after.mrfile ${MAJADATA_TVA_INPUT}/CAMSComputationTest/CAMS_MR_20160615UTC030000.nc
                             -sumofaot ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCAMSSumOfAOT.tiff
                     
                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCAMS_Props.txt

                     VALID   --compare-image ${EPSILON_4}
                             ${MAJADATA_TVA_BASELINE}/apTvCAMSSumOfAOT.tiff
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCAMSSumOfAOT.tiff
                    )
                    
maja_test_application(NAME apTvNewCamsCompute
                     APP  CAMSComputation
                     OPTIONS -camsdescription ${MAJADATA_TVA_INPUT}/CAMSNewComputationTest/CAMS_Description_File.xml
                             -models seasalt Dust organicmatter blackcarbon sulphate nitrate ammonium
                             -rhsampling 30 50 70 80 85 90 95
                             -limaod 0.95
                             -before.modellevels  0.02 0.031 0.0457 0.0683 0.0975 0.1361 0.1861 0.2499 0.3299 0.4288 
                             0.5496 0.6952 0.869 1.0742 1.3143 1.5928 1.9134 2.2797 2.6954 3.1642 3.6898 4.2759 
                             4.9262 5.6441 6.4334 7.2974 8.2397 9.2634 10.372 11.5685 12.8561 14.2377 15.7162 
                             17.2945 18.9752 20.751 22.6543 24.6577 26.7735 29.0039 31.3512 33.8174 36.4047 
                             39.1149 41.9493 44.9082 47.9915 51.199 54.5299 57.9834 61.5607 65.2695 69.1187 
                             73.1187 77.281 81.6182 86.145 90.8774 95.828 101.0047 106.4153 112.0681 117.9714
                             124.1337 130.5637 137.2703 144.2624 151.5493 159.1403 167.045 175.2731 183.8344
                             192.7389 201.9969 211.6186 221.6146 231.9954 242.7719 253.9549 265.5556 277.5852 
                             290.0548 302.9762 316.3607 330.2202 344.5663 359.4111 374.7666 390.645 407.0583 
                             424.019 441.5395 459.6321 478.3096 497.5845 517.4198 537.7195 558.343 579.1926 
                             600.1668 621.1624 642.0764 662.8084 683.262 703.3467 722.9795 742.0855 760.5996
                             778.4661 795.6396 812.0847 827.7756 842.6959 856.8376 870.2004 882.791 894.6222
                             905.7116 916.0815 925.7571 934.7666 943.1399 950.9082 958.1037 964.7584 970.9046
                             976.5737 981.7968 986.6036 991.023 995.0824 998.8081 1002.225 1005.3562 1008.2239
                             1010.8487 1013.25
                             -before.nbnointerpol 20
                             -after.modellevels  0.02 0.031 0.0457 0.0683 0.0975 0.1361 0.1861 0.2499 0.3299 0.4288 
                             0.5496 0.6952 0.869 1.0742 1.3143 1.5928 1.9134 2.2797 2.6954 3.1642 3.6898 4.2759 
                             4.9262 5.6441 6.4334 7.2974 8.2397 9.2634 10.372 11.5685 12.8561 14.2377 15.7162 
                             17.2945 18.9752 20.751 22.6543 24.6577 26.7735 29.0039 31.3512 33.8174 36.4047 
                             39.1149 41.9493 44.9082 47.9915 51.199 54.5299 57.9834 61.5607 65.2695 69.1187 
                             73.1187 77.281 81.6182 86.145 90.8774 95.828 101.0047 106.4153 112.0681 117.9714
                             124.1337 130.5637 137.2703 144.2624 151.5493 159.1403 167.045 175.2731 183.8344
                             192.7389 201.9969 211.6186 221.6146 231.9954 242.7719 253.9549 265.5556 277.5852 
                             290.0548 302.9762 316.3607 330.2202 344.5663 359.4111 374.7666 390.645 407.0583 
                             424.019 441.5395 459.6321 478.3096 497.5845 517.4198 537.7195 558.343 579.1926 
                             600.1668 621.1624 642.0764 662.8084 683.262 703.3467 722.9795 742.0855 760.5996
                             778.4661 795.6396 812.0847 827.7756 842.6959 856.8376 870.2004 882.791 894.6222
                             905.7116 916.0815 925.7571 934.7666 943.1399 950.9082 958.1037 964.7584 970.9046
                             976.5737 981.7968 986.6036 991.023 995.0824 998.8081 1002.225 1005.3562 1008.2239
                             1010.8487 1013.25      
                             -after.nbnointerpol 20                       
                             -lat 43.742933
                             -lon 4.92374204531563
                             -dateutc UTC=2019-07-30T10:32:31
                             -before.dateutc UTC=2019-07-30T00:00:00
                             -before.aotfile ${MAJADATA_TVA_INPUT}/CAMSNewComputationTest/CAMS_AOT_20190730UTC000000.nc
                             -before.rhfile ${MAJADATA_TVA_INPUT}/CAMSNewComputationTest/CAMS_RH_20190730UTC000000.nc
                             -before.mrfile ${MAJADATA_TVA_INPUT}/CAMSNewComputationTest/CAMS_MR_20190730UTC000000.nc
                             -after.dateutc UTC=2019-07-30T12:00:00
                             -after.aotfile ${MAJADATA_TVA_INPUT}/CAMSNewComputationTest/CAMS_AOT_20190730UTC120000.nc
                             -after.rhfile ${MAJADATA_TVA_INPUT}/CAMSNewComputationTest/CAMS_RH_20190730UTC120000.nc
                             -after.mrfile ${MAJADATA_TVA_INPUT}/CAMSNewComputationTest/CAMS_MR_20190730UTC120000.nc
                             -sumofaot ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvNewCAMSSumOfAOT.tiff
                     
                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvNewCAMS_Props.txt

                     VALID   --compare-image ${EPSILON_4}
                             ${MAJADATA_TVA_BASELINE}/apTvNewCAMSSumOfAOT.tiff
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvNewCAMSSumOfAOT.tiff
                    )
                    
maja_test_application(NAME apTvNewCamsMixedCompute
                     APP  CAMSComputation
                     OPTIONS -camsdescription ${MAJADATA_TVA_INPUT}/CAMSNewMixedComputationTest/CAMS_Description_File.xml
                             -models seasalt Dust organicmatter blackcarbon sulphate
                             -rhsampling 30 50 70 80 85 90 95
                             -limaod 0.95
                             -before.modellevels 0.2 0.3843 0.6365 0.9564 1.3448 1.8058 2.3478 2.985 3.7397 4.6462 5.7565 
                             7.1322 8.8366 10.9483 13.5647 16.8064 20.8227 25.7989 31.9642 39.6029 49.0671 60.1802 
                             73.0663 87.7274 104.229 122.614 142.902 165.089 189.147 215.025 242.652 272.059 303.217 
                             336.044 370.407 406.133 443.009 480.791 519.209 557.973 596.777 635.306 673.24 710.263 
                             746.063 780.346 812.83 843.263 871.42 897.112 920.189 940.551 958.148 972.987 985.14 
                             994.747 1002.02 1007.26 1010.85 1013.25
                             -after.modellevels  0.02 0.031 0.0457 0.0683 0.0975 0.1361 0.1861 0.2499 0.3299 0.4288 
                             0.5496 0.6952 0.869 1.0742 1.3143 1.5928 1.9134 2.2797 2.6954 3.1642 3.6898 4.2759 
                             4.9262 5.6441 6.4334 7.2974 8.2397 9.2634 10.372 11.5685 12.8561 14.2377 15.7162 
                             17.2945 18.9752 20.751 22.6543 24.6577 26.7735 29.0039 31.3512 33.8174 36.4047 
                             39.1149 41.9493 44.9082 47.9915 51.199 54.5299 57.9834 61.5607 65.2695 69.1187 
                             73.1187 77.281 81.6182 86.145 90.8774 95.828 101.0047 106.4153 112.0681 117.9714
                             124.1337 130.5637 137.2703 144.2624 151.5493 159.1403 167.045 175.2731 183.8344
                             192.7389 201.9969 211.6186 221.6146 231.9954 242.7719 253.9549 265.5556 277.5852 
                             290.0548 302.9762 316.3607 330.2202 344.5663 359.4111 374.7666 390.645 407.0583 
                             424.019 441.5395 459.6321 478.3096 497.5845 517.4198 537.7195 558.343 579.1926 
                             600.1668 621.1624 642.0764 662.8084 683.262 703.3467 722.9795 742.0855 760.5996
                             778.4661 795.6396 812.0847 827.7756 842.6959 856.8376 870.2004 882.791 894.6222
                             905.7116 916.0815 925.7571 934.7666 943.1399 950.9082 958.1037 964.7584 970.9046
                             976.5737 981.7968 986.6036 991.023 995.0824 998.8081 1002.225 1005.3562 1008.2239
                             1010.8487 1013.25      
                             -after.nbnointerpol 20                       
                             -lat 43.742933
                             -lon 4.92374204531563
                             -dateutc UTC=2019-07-30T10:32:31
                             -before.dateutc UTC=2019-07-30T00:00:00
                             -before.aotfile ${MAJADATA_TVA_INPUT}/CAMSNewMixedComputationTest/CAMS_AOT_20160614UTC030000.nc
                             -before.rhfile ${MAJADATA_TVA_INPUT}/CAMSNewMixedComputationTest/CAMS_RH_20160614UTC030000.nc
                             -before.mrfile ${MAJADATA_TVA_INPUT}/CAMSNewMixedComputationTest/CAMS_MR_20160614UTC030000.nc
                             -after.dateutc UTC=2019-07-30T12:00:00
                             -after.aotfile ${MAJADATA_TVA_INPUT}/CAMSNewMixedComputationTest/CAMS_AOT_20190730UTC120000.nc
                             -after.rhfile ${MAJADATA_TVA_INPUT}/CAMSNewMixedComputationTest/CAMS_RH_20190730UTC120000.nc
                             -after.mrfile ${MAJADATA_TVA_INPUT}/CAMSNewMixedComputationTest/CAMS_MR_20190730UTC120000.nc
                             -sumofaot ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvNewCAMSMixedSumOfAOT.tiff
                     
                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvNewCAMSMixedSumOfAOT.txt

                     VALID   --compare-image 0.0001
                             ${MAJADATA_TVA_BASELINE}/apTvNewCAMSMixedSumOfAOT.tiff
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvNewCAMSMixedSumOfAOT.tiff
                    )



maja_test_application(NAME apTvDirectionalCorrection
                     APP  DirectionnalCorrection
                     OPTIONS    -solar.azimuth 143.195
                                -solar.zenith 24.0717 
                                -viewing.zenith 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321
                                -viewing.azimuth 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105
                                -refzenith 0
                                -refazimuth 0
                                -coeffsr 0 0.480620155 0.4441041348 0.3396449704 0.3396449704 0.4179634487 0.4962819269 0.4962819269 0.4962819269 0 0 0.3364431487 0.240406320
                                -coeffsv 0 0.1020671835 0.1362940276 0.1343195266 0.1343195266 0.1205060291 0.1066925315 0.1066925315 0.1066925315 0 0 0.1320699708 0.1455981941
                     
                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDirectionalCorrection.txt

                     VALID   --compare-ascii ${EPSILON_4}
                             ${MAJADATA_TVA_BASELINE}/apTvDirectionalCorrection.txt
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDirectionalCorrection.txt
                    )
                    
maja_test_application(NAME apTvReduceLut
                     APP  ReduceLut
                     OPTIONS    -solar.azimuth 143.195
                                -solar.zenith 24.0717 
                                -viewing.zenith 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321
                                -viewing.azimuth 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105
                                -reducelut ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReduceLut.mhd
                                -lut ${MAJADATA_TVA_INPUT}/ReduceLutComputationTest/giplut_cams.xml
                     VALID   --compare-binary 
                             ${MAJADATA_TVA_BASELINE}/apTvReduceLut.raw
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReduceLut.raw
                    )
                   
maja_test_application(NAME apTvReduceLutDirCorr
                     APP  ReduceLut
                     OPTIONS    -solar.azimuth 143.195
                                -solar.zenith 24.0717 
                                -viewing.zenith 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321
                                -viewing.azimuth 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105
                                -reducelut ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReduceLutDirCorr.mhd
                                -lut ${MAJADATA_TVA_INPUT}/ReduceLutComputationTest/giplut_cams.xml
                                -refzenith 0
                                -refazimuth 0
                                -dircorr true
                                -coeffsr 0 0.480620155 0.4441041348 0.3396449704 0.3396449704 0.4179634487 0.4962819269 0.4962819269 0.4962819269 0 0 0.3364431487 0.240406320
                                -coeffsv 0 0.1020671835 0.1362940276 0.1343195266 0.1343195266 0.1205060291 0.1066925315 0.1066925315 0.1066925315 0 0 0.1320699708 0.1455981941
                                
                     VALID   --compare-binary 
                             ${MAJADATA_TVA_BASELINE}/apTvReduceLutDirCorr.raw
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReduceLutDirCorr.raw
                    )

maja_test_application(NAME apTvExtractLutChannel
                     APP  LutExtractChannels
                     OPTIONS    -lutimage ${MAJADATA_TVA_INPUT}/ReduceLutComputationTest/ReduceLut.mhd
                                -lut ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvExtractLutChannel.mhd
                                -cl 1 2s
                                
                     VALID   --compare-binary 
                             ${MAJADATA_TVA_BASELINE}/apTvExtractLutChannel.raw
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvExtractLutChannel.raw
                    )

             
       
