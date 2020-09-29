# -*- coding: utf-8 -*-
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
"""
###################################################################################################

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.directory_manager -- shortdesc

orchestrator.common.directory_manager is a description

It defines classes_and_methods

###################################################################################################
"""
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.gipp_utils import uncompress_dbl_product
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
from orchestrator.cots.otb.algorithms.otb_multiply_by_scalar import multiply_by_scalar
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.cots.otb.algorithms.otb_stats import stats
from orchestrator.cots.gdal.gdal_dataset_info import GdalDatasetInfo
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.common.maja_common import Area
import orchestrator.common.xml_tools as xml_tools
import os
LOGGER = configure_logger(__name__)


class DEMBase(object):

    def __init__(self):
        self.ALTList = []
        self.ALT_Mean = 0
        self.ALT_Min = 0
        self.ALT_Max = 0
        self.ALT_Stdv = 0
        self.ALT_LogicalName = ""
        self.__ASPListInternal = []
        self.ASPList = []
        self.__SLPListInternal = []
        self.SLPList = []
        self.ALC = ""
        self.MSK = ""
        self.ASC = None
        self.__SLCInternal = ""
        self.SLC = ""
        self.Satellite = ""
        self._coeff = 0.01
        self._resList = []
        self._expr = "im1b1 * "
        self.CoarseArea = None
        self.L2Areas = []
        self.ProjRef = ""
        self.ProjCode = ""
        self.ProjType = ""
        self.ProjName = ""
        self.Site = ""
        self._apps = OtbPipelineManager()

    def initialize(self, filename, working_dir, has_snow):
        file_hdr = os.path.splitext(filename)[0] + ".HDR"
        file_dbl = os.path.splitext(filename)[0] + ".DBL"
        file_dbldir = os.path.splitext(filename)[0] + ".DBL.DIR"
        LOGGER.info("AUX_REFDE2 filename: " + filename)
        # uncompress dbl
        uncompress_dbl_product(file_dbl)

        # DEM filenames provider
        list_of_file = os.listdir(file_dbldir)
        nbresol = 0
        for f in list_of_file:
            if "_ALT" in f and "TIF" in os.path.splitext(f)[1]:
                nbresol = nbresol + 1
        LOGGER.info("Nb resolution found " + str(nbresol))
        self.initialize_res_list(nbresol)

        LOGGER.info("DEMFilenamesProvider::Initialize. Nb resolution computed:" + str(len(self._resList)))
        for resol in self._resList:
            LOGGER.debug("DEMFilenamesProvider::Initialize. Prefix resol : " + resol)

        handler = EarthExplorerXMLFileHandler(file_hdr)
        list_of_dbl_files = handler.get_list_of_packaged_dbl_files(True, False)
        LOGGER.info("DEMFileNames found " + str(len(list_of_dbl_files)) + " files")

        for i in range(0,len(list_of_dbl_files)):
            if list_of_dbl_files[i].split('.TIF')[-1]:
                raise MajaDataException("Wrong file extension detected. Delete the file: " + str(list_of_dbl_files[i]))


        # --------------------------------------
        # Find the correct filename
        for fi in list_of_dbl_files:
            # LAIG - FA - MAC - 1610 - CNES
            # ../ CONTEXTES_ANOMALIES / TMA_VENUS_maccs_errors / 4398 / VE_TEST_AUX_REFDE2_BRASCHAT_0001.DBL.DIR / VE_TEST_AUX_REFDE2_BRASCHAT_0001_SLP.TIF
            # Extract the last value -> SLP
            # ../ CONTEXTES_ANOMALIES / TMA_VENUS_maccs_errors / 4398 / VE_TEST_AUX_REFDE2_BRASCHAT_0001.DBL.DIR / VE_TEST_AUX_REFDE2_BRASCHAT_0001_ALT_R1.TIF
            # Extract the last value -> ALT
            l_splitted = (os.path.splitext(os.path.basename(fi))[0]).split("_")
            l_lenghtlistfilenamename = len(l_splitted)
            # Extract the tow last values -> ex: 0001 _SLP or ALT_R1
            l_keytype = l_splitted[-1]
            if l_lenghtlistfilenamename > 2:
                l_keytype = l_splitted[-2] + "_" + l_keytype
            # --------------------------------------
            # Test if the filename is ALC
            if "ALC" in l_keytype:
                self.ALC = fi
            # --------------------------------------
            # Test if the filename is MSK
            elif "MSK" in l_keytype:
                self.MSK = fi
            # --------------------------------------
            # Test if the filename is ASC
            elif "ASC" in l_keytype:
                self.ASC = fi
            # --------------------------------------
            # Test if the filename is SLC
            elif "SLC" in l_keytype:
                self.__SLCInternal = fi
            else:
                # --------------------------------------
                # Lop under resolutions
                for res in self._resList:
                    # --------------------------------------
                    # Test if the filename is SLP
                    if "SLP" in l_keytype:
                        if res in l_keytype:
                            self.__SLPListInternal.append(fi)
                    # --------------------------------------
                    # Test if the filename is ALT
                    elif "ALT" in l_keytype:
                        if res in l_keytype:
                            self.ALTList.append(fi)
                    # --------------------------------------
                    # Test if the filename is ASP
                    elif "ASP" in l_keytype:
                        if res in l_keytype:
                            self.__ASPListInternal.append(fi)
                    else:
                        LOGGER.debug("Unknown Filename and associated product type.")
                # endloop resol
        # --------------------------------------
        # Check existent of ALC filename
        if not os.path.exists(self.ALC):
            raise MajaDataException("The ALC file '" + self.ALC + "' of the DTM doesn't exist !")

        # --------------------------------------
        # Check existent of MSK filename
        if not os.path.exists(self.MSK):
            raise MajaDataException("The MSK file '" + self.MSK + "' of the DTM doesn't exist !")

        # --------------------------------------
        # Check existent of SLC filename
        if not os.path.exists(self.__SLCInternal):
            raise MajaDataException("The SLC file '" + self.__SLCInternal + "' of the DTM doesn't exist !")
        else:
            LOGGER.debug("Starting multiply " + self.__SLCInternal + " * " + str(self._coeff))
            self.SLC = os.path.join(working_dir, "Mul_" + os.path.basename(self.__SLCInternal))
            self._apps.add_otb_app(multiply_by_scalar(self.__SLCInternal, self._coeff, output_image=self.SLC))
            mtdat = GdalDatasetInfo(self.__SLCInternal)
            self.CoarseArea = Area()
            self.CoarseArea.size = mtdat.size
            self.CoarseArea.origin = mtdat.origin
            self.CoarseArea.spacing = mtdat.pixel_size
            LOGGER.debug("Done")

        # --------------------------------------
        for resol in range(0, len(self._resList)):
            # --------------------------------------
            # Check existent of SLP filename
            if not os.path.exists(self.__SLPListInternal[resol]):
                raise MajaDataException(
                    "One of the SLP file '" +
                    self.__SLPListInternal[resol] +
                    "' of the DTM doesn't exist !")
            else:
                LOGGER.debug("Starting multiply " + self.__SLPListInternal[resol] + " * " + str(self._coeff))
                tmp = os.path.join(working_dir, "Mul_" + os.path.basename(self.__SLPListInternal[resol]))
                slp_mul_app = multiply_by_scalar(self.__SLPListInternal[resol], self._coeff, output_image=tmp,write_output=False)
                self._apps.add_otb_app(slp_mul_app)
                mtdat = GdalDatasetInfo(self.__SLPListInternal[resol])
                l2area = Area()
                l2area.size = mtdat.size
                l2area.origin = mtdat.origin
                l2area.spacing = mtdat.pixel_size
                self.ProjRef = mtdat.dataset.GetProjectionRef()
                self.L2Areas.append(l2area)
                LOGGER.debug("Done")
                self.SLPList.append(slp_mul_app.getoutput().get("out"))
            # --------------------------------------
            # Check existent of ALT filename
            if not os.path.exists(self.ALTList[resol]):
                raise MajaDataException("One of the ALT file '" + self.ALTList[resol] + "' of the DTM doesn't exist !")
            # --------------------------------------
            # Check existent of ASP filename
            if not os.path.exists(self.__ASPListInternal[resol]):
                raise MajaDataException(
                    "One of the ASP file '" +
                    self.__ASPListInternal[resol] +
                    "' of the DTM doesn't exist !")
            else:
                LOGGER.debug("Starting multiply " + self.__ASPListInternal[resol] + " * " + str(self._coeff))
                tmp = os.path.join(working_dir, "Mul_" + os.path.basename(self.__ASPListInternal[resol]))
                asp_mul_app = multiply_by_scalar(self.__ASPListInternal[resol], self._coeff, output_image=tmp,write_output=False)
                self._apps.add_otb_app(asp_mul_app)
                LOGGER.debug("Done")
                self.ASPList.append(asp_mul_app.getoutput().get("out"))

        # end loop resol

        LOGGER.debug(nbresol)

        l_cartoCode = xml_tools.get_only_value(
            handler.root,
            "//DEM_Information/Cartographic/Coordinate_Reference_System/Code",
            namespaces=handler.nss,
            check=True)
        l_geoCode = xml_tools.get_only_value(
            handler.root,
            "//DEM_Information/Geographic/Coordinate_Reference_System/Code",
            namespaces=handler.nss,
            check=True)
        if l_cartoCode is not None:
            self.ProjCode = l_cartoCode.text
            self.ProjType = "PROJECTED"
        elif l_geoCode is not None:
            self.ProjCode = l_geoCode.text
            self.ProjType = "GEOGRAPHIC"
        else:
            raise MajaDataException("Unknown DEM type")

        LOGGER.debug("DEM Projection Code: "+self.ProjCode)
        LOGGER.debug("DEM Projection Type: "+self.ProjType)

        self.Site = xml_tools.get_xml_string_value(
            handler.root,
            "//Specific_Product_Header/Instance_Id/Applicable_Site_Nick_Name",
            namespaces=handler.nss)

        if nbresol != 0:
            param_stats = {"im": self.ALTList[0]}
            stat_app = stats(self.ALTList[0])
            self.ALT_Mean = stat_app.getoutput().get("mean")
            self.ALT_Max = stat_app.getoutput().get("max")
            self.ALT_Min = stat_app.getoutput().get("min")
            self.ALT_Stdv = stat_app.getoutput().get("stdv")
            self.ALT_LogicalName = "LOCAL=" + os.path.splitext(os.path.basename(file_hdr))[0]
            LOGGER.info("DEM Mean : " + str(self.ALT_Mean))
            LOGGER.info("DEM Max : " + str(self.ALT_Max))
            LOGGER.info("DEM Min : " + str(self.ALT_Min))
            LOGGER.info("DEM Stdv : " + str(self.ALT_Stdv))

    def initialize_res_list(self, nb_resol):
        if nb_resol <= 1:
            self._resList.append("")
        else:
            for i in range(0, nb_resol):
                self._resList.append("_R" + str(i + 1))

    def __del__(self):
        self._apps.free_otb_app()
