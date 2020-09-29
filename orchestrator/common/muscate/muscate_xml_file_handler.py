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
"""
###################################################################################################

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     |||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods


###################################################################################################
"""

import orchestrator.common.xml_tools as xml_tools
import os
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import MajaDataException

LOGGER = configure_logger(__name__)

MUSCATE_HANDLER_XPATH = {
    "Identifier": "//Dataset_Identification/IDENTIFIER",
    "Authority": "//Dataset_Identification/AUTHORITY",
    "Producer": "//Dataset_Identification/PRODUCER",
    "Project": "//Dataset_Identification/PROJECT",
    "AcquisitionDate": "//Product_Characteristics/ACQUISITION_DATE",
    "ProductionDate": "//Product_Characteristics/PRODUCTION_DATE",
    "Instrument": "//Product_Characteristics/INSTRUMENT",
    "SpectralContent": "//Product_Characteristics/SPECTRAL_CONTENT",
    "ProductVersion": "//Product_Characteristics/PRODUCT_VERSION",
    "ZoneGeo": "//GEOGRAPHICAL_ZONE",
    "ProductId": "//Product_Characteristics/PRODUCT_ID",
    "Platform": "//Product_Characteristics/PLATFORM",
    "OrbitNumber": "//Product_Characteristics/ORBIT_NUMBER",
    "UTCAcquisitionRangeMean": "//UTC_Acquisition_Range/MEAN",
    "UTCAcquisitionRangeDatePrecision": "//UTC_Acquisition_Range/DATE_PRECISION",
    "WaterMaskFilename": "//Mask_List/Mask[Mask_Properties/NATURE='Water']/Mask_File_List/MASK_FILE",
    "HiddenSurfaceMaskFilename": "//Mask_List/Mask[Mask_Properties/NATURE='Hidden_Surface']/Mask_File_List/MASK_FILE",
    "TopographyShadowMaskFilename": "//Mask_List/Mask[Mask_Properties/NATURE='Topography_Shadow']/" +
                                    "Mask_File_List/MASK_FILE",
    "SunTooLowMaskFilename": "//Mask_List/Mask[Mask_Properties/NATURE='Sun_Too_Low']/Mask_File_List/MASK_FILE",
    "TangentSunMaskFilename": "//Mask_List/Mask[Mask_Properties/NATURE='Tangent_Sun']/Mask_File_List/MASK_FILE",
    "SnowMaskFilename": "//Mask_List/Mask[Mask_Properties/NATURE='Snow']/Mask_File_List/MASK_FILE",
    "EdgeMaskFilename": "//Mask_List/Mask[Mask_Properties/NATURE='Edge']/Mask_File_List/MASK_FILE",
    "AOTInterpolationMaskFilename": "//Mask_List/Mask[Mask_Properties/NATURE='AOT_Interpolation']/" +
                                    "Mask_File_List/MASK_FILE",
    "PrivateDirectoryFilename": "//Data_List/Data[Data_Properties/NATURE='Private_Directory']/Data_File_List/DATA_FILE",
    "JobProcessingInformationsFileFilename": "//Data_List/Data[Data_Properties/" +
                                             "NATURE='Job_Processing_Informations_File']/Data_File_List/DATA_FILE",
    "DEMInformationMinimum": "/JobProcessingInformations/MNT_Statistics/Min",
    "DEMInformationMaximum": "/JobProcessingInformations/MNT_Statistics/Max",
    "DEMInformationAverage": "/JobProcessingInformations/MNT_Statistics/Average",
    "DEMInformationStandardDeviation": "/JobProcessingInformations/MNT_Statistics/Standard_Deviation",
    "ProcessingFlagsAndModesValidityFlag": "//Processing_Flags_And_Modes_List/" +
                                           "Processing_Flags_And_Modes[Key='Validity_Flag']/Value",
    "ProcessingFlagsAndModesQualityCheck": "//Processing_Flags_And_Modes_List/" +
                                           "Processing_Flags_And_Modes[Key='Quality_check']/Value",
    "ProcessingFlagsAndModesProcessingMode": "//Processing_Flags_And_Modes_List/" +
                                             "Processing_Flags_And_Modes[Key='Processing_Mode']/Value",
    "ProcessingFlagsAndModesProcessingAdjacencyEffectsAndSlopeCorrection": "//Processing_Flags_And_Modes_List/" +
                                    "Processing_Flags_And_Modes[Key='Adjacency_Effects_And_Slope_Correction']/Value",
    "ProcessingFlagsAndModesFileType": "//Processing_Flags_And_Modes_List/" +
                                       "Processing_Flags_And_Modes[Key='File_Type']/Value",
    "ProcessingFlagsAndModesCirrusCorrection": "//Processing_Flags_And_Modes_List/" +
                                               "Processing_Flags_And_Modes[Key='Cirrus_Correction']/Value",
    "ProcessingFlagsAndModesCAMSProcessing": "//Processing_Flags_And_Modes_List/" +
                                             "Processing_Flags_And_Modes[Key='CAMS_Processing']/Value",
    "L1NoData": "//SPECIAL_VALUE[@name='nodata']",
    "Nodata_Value": "//SPECIAL_VALUE[@name='nodata']",
    "VAP_Nodata_Value": "//SPECIAL_VALUE[@name='water_vapor_content_nodata']",
    "AOT_Nodata_Value": "//SPECIAL_VALUE[@name='aerosol_optical_thickness_nodata']",
    "QuantificationValue": "//REFLECTANCE_QUANTIFICATION_VALUE",
    "AerosolOpticalThicknessQuantificationValue": "//AEROSOL_OPTICAL_THICKNESS_QUANTIFICATION_VALUE",
    "AerosolOpticalThicknessQuantificationValueAsString": "//AEROSOL_OPTICAL_THICKNESS_QUANTIFICATION_VALUE",
    "WaterVaporContentQuantificationValue": "//WATER_VAPOR_CONTENT_QUANTIFICATION_VALUE",
    "WaterVaporContentQuantificationValueAsString": "//WATER_VAPOR_CONTENT_QUANTIFICATION_VALUE",
    "Projection": "//Horizontal_Coordinate_System/HORIZONTAL_CS_NAME",
    "ProjectionType": "//Horizontal_Coordinate_System/HORIZONTAL_CS_TYPE",
    "ProjectionCode": "//Horizontal_Coordinate_System/HORIZONTAL_CS_CODE",
    "SunAngleColStep": "//Sun_Angles_Grids/Zenith/COL_STEP",
    "SunAngleRowStep": "//Sun_Angles_Grids/Zenith/ROW_STEP"}


class MuscateXMLFileHandler(object):

    M_SCHEMA_DECIMAL_9_FORMAT = "%.9f"

    def __init__(self, main_xml_file, validate=False, schema_path=None):
        """
        Constructor
        """
        LOGGER.info("Loading " + main_xml_file)
        self.main_xml_file = main_xml_file
        self.root = xml_tools.get_root_xml(self.main_xml_file, deannotate=True)
        if validate and schema_path is not None:
            xml_tools.check_xml(main_xml_file, schema_path)

    # Writing parts
    def save_to_file(self, output_filename):
        # xml_tools.save_to_xml_file(self.root, output_filename)
        xml_tools.save_to_xml_file(self.root, output_filename)
        os.system(
            'XMLLINT_INDENT="    "  xmllint --format ' + str(output_filename) + " --output " + str(output_filename))




    def replace_node(self, node_src, xpath_src, xpath_dst = None, check_if_present=False):
        if xpath_dst is None:
            xml_tools.replace_node(node_src, xpath_src, self.root, xpath_src, check_if_present)
        else:
            xml_tools.replace_node(node_src, xpath_src, self.root, xpath_dst, check_if_present)

    def set_value_of(self, key, value):
        xml_tools.set_value(self.root, MUSCATE_HANDLER_XPATH[key], value)

    def set_string_value(self, xpath, value, check_if_present=True):
        xml_tools.set_value(self.root, xpath, value, check=check_if_present)

    def remove_node(self, xpath):
        xml_tools.remove_node(self.root, xpath)

    # Reading parts

    def get_string_value_of(self, key):
        value = xml_tools.get_only_value(self.root, MUSCATE_HANDLER_XPATH[key])
        if value is not None:
            return value.text
        else:
            raise MajaDataException("No " + MUSCATE_HANDLER_XPATH[key] + " available in the file " + self.main_xml_file)

    def set_validity_flag(self, flag):
        if flag:
            xml_tools.set_value(self.root, MUSCATE_HANDLER_XPATH["ProcessingFlagsAndModesValidityFlag"], "L2VALD")
        else:
            xml_tools.set_value(self.root, MUSCATE_HANDLER_XPATH["ProcessingFlagsAndModesValidityFlag"], "L2NOTV")

    def get_validity_flag(self):
        if "L2VALD" in self.get_string_value_of("ProcessingFlagsAndModesValidityFlag"):
            return True
        else:
            return False

    # Get the upper left corner in latitude / longitude coordinate system
    def get_upper_left_corner(self):
        # upperLeftCorner[0] = Long upperLeftCorner[1] = Lat
        return (
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='upperLeft']/LON"),
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='upperLeft']/LAT"))

    # Get the upper right corner in latitude / longitude coordinate system
    def get_upper_right_corner(self):
        # upperRightCorner[0] = Long upperRightCorner[1] = Lat
        return (
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='upperRight']/LON"),
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='upperRight']/LAT"))

    # Get the lower left corner in latitude / longitude coordinate system
    def get_lower_left_corner(self):
        # lowerLeftCorner[0] = Long lowerLeftCorner[1] = Lat
        return (
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='lowerLeft']/LON"),
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='lowerLeft']/LAT"))

    # Get the lower right corner in latitude / longitude coordinate system
    def get_lower_right_corner(self):
        # lowerRightCorner[0] = Long lowerRightCorner[1] = Lat
        return (
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='lowerRight']/LON"),
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='lowerRight']/LAT"))

    # Get the center in latitude / longitude coordinate system
    def get_center(self):
        # lowerRightCorner[0] = Long lowerRightCorner[1] = Lat
        return (
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='center']/LON"),
            xml_tools.get_xml_float_value(
                self.root,
                "//Geopositioning/Global_Geopositioning/Point[@name='center']/LAT"))

    # Get the mean solar angles
    def get_mean_solar_angles(self):
        # solAngles[0] = zenith solAngles[1] = azimuth
        return (xml_tools.get_xml_float_value(self.root, "//Sun_Angles/ZENITH_ANGLE[@unit='deg']"),
                xml_tools.get_xml_float_value(self.root, "//Sun_Angles/AZIMUTH_ANGLE[@unit='deg']"))

    # Get the mean viewing angles
    def get_mean_viewing_angles(self):
        # vieAngles[0] = zenith vieAngles[1] = azimuth
        if xml_tools.get_only_value(self.root, "//Incidence_Angles/ZENITH_ANGLE[@unit='deg']", check=True) is not None:
            return (xml_tools.get_xml_float_value(self.root, "//Incidence_Angles/ZENITH_ANGLE[@unit='deg']"),
                    xml_tools.get_xml_float_value(self.root, "//Incidence_Angles/AZIMUTH_ANGLE[@unit='deg']"))
        else:
            return 0, 0

    # Get the acquisition date with the YYYYMMDD format
    def get_acquisition_date_formated_yyyymmdd(self):
        ident = xml_tools.get_xml_string_value(self.root, "//Product_Characteristics/ACQUISITION_DATE")
        element = ident.split('T')
        date = element[0]
        return date.replace("-", "")

    # Get the acquisition date in UTC format

    def get_date_pdv_formated_utc(self):
        # Date formated:
        # Case1: <PRODUCTION_DATE>2014-12-17T09:36:57.0</PRODUCTION_DATE>
        # or
        # Case2: <PRODUCTION_DATE>2014-12-17T09:36:57</PRODUCTION_DATE>
        datepdv = xml_tools.get_xml_string_value(self.root, "//Product_Characteristics/ACQUISITION_DATE")
        element = datepdv.split('.')
        msec = ''
        if len(element) == 2:
            msec = element[1]
            if msec[-1] == 'Z':
                msec = msec[:-1]
            msec += '000'
            msec = '.' + msec[0:3] + 'Z'
        date = element[0] + msec
        # Return date formated 'YYYY-MM-DD HH:MM:SS'
        return "UTC=" + date

    def get_mean_viewing_zenithal_angles(self):
        return xml_tools.get_float_list_from_nodes(
            self.root, "//Mean_Viewing_Incidence_Angle_List/Mean_Viewing_Incidence_Angle/ZENITH_ANGLE")

    def get_mean_viewing_azimuthal_angles(self):
        return xml_tools.get_float_list_from_nodes(
            self.root, "//Mean_Viewing_Incidence_Angle_List/Mean_Viewing_Incidence_Angle/AZIMUTH_ANGLE")

    # get the sun zenithal angles
    def get_sun_zenithal_angles(self):
        return xml_tools.get_list_of_string_child_values(self.root, "//Sun_Angles_Grids/Zenith/Values_List")

    # get the sun azimuthal angles
    def get_sun_azimuthal_angles(self):
        return xml_tools.get_list_of_string_child_values(self.root, "//Sun_Angles_Grids/Azimuth/Values_List")

    # get the viewing zenithal angles
    def get_viewing_zenithal_angles(self, bandid):
        if self.has_per_band_angles():
            # S2 case, with per band and per detector angles
            return self._get_list_of_angles_all_detectors_per_band(
                "//Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List", bandid, "Zenith")
        else:
            # Venus case, with per detector angles, 'bandid' is interpreted as detector_id
            zenith_path = "//Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List/Viewing_Incidence_Angles_Grids[@detector_id='{}']/Zenith/Values_List"
            return [xml_tools.get_list_of_string_child_values(self.root,zenith_path.format(bandid))]

    # get the viewing azimuthal angles
    def get_viewing_azimuthal_angles(self, bandid):
        if self.has_per_band_angles():
            # S2 case, with per band and per detector angles
            return self._get_list_of_angles_all_detectors_per_band(
                "//Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List", bandid, "Azimuth")
        else:
            # Venus case, with per detector angles, 'bandid' is interpreted as detector_id
            azimuth_path = "//Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List/Viewing_Incidence_Angles_Grids[@detector_id='{}']/Azimuth/Values_List"
            return [xml_tools.get_list_of_string_child_values(self.root,azimuth_path.format(bandid))]

    def get_viewing_grid_col_step(self, bandid, det):
        if bandid is not None:
            band_selector = "[@band_id='{}']".format(bandid)
        else:
            band_selector = ""
        col_path = "//Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List{}/Viewing_Incidence_Angles_Grids[@detector_id='{}']/Zenith/COL_STEP"
        return xml_tools.get_xml_string_value(self.root,col_path.format(band_selector,det))

    def get_viewing_grid_row_step(self, bandid, det):
        if bandid is not None:
            band_selector = "[@band_id='{}']".format(bandid)
        else:
            band_selector = ""
        row_path = "//Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List{}/Viewing_Incidence_Angles_Grids[@detector_id='{}']/Zenith/ROW_STEP"
        return xml_tools.get_xml_string_value(self.root,row_path.format(band_selector,det))

    def get_viewing_grid_step_unit(self, bandid, det):
        if bandid is not None:
            band_selector = "[@band_id='{}']".format(bandid)
        else:
            band_selector = ""
        zenith_path = "//Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List{}/Viewing_Incidence_Angles_Grids[@detector_id='{}']/Zenith"
        return xml_tools.get_attribute(self.root,zenith_path.format(band_selector,det), "step_unit")

    # Get the list of bands
    def get_list_of_bands(self):
        # Get the number of bands
        l_count = xml_tools.get_attribute(self.root, "//Band_Global_List", "count")

        localpath = "//Band_Global_List[@count='" + str(l_count) + "']/BAND_ID"
        return xml_tools.get_string_list_from_nodes(self.root, localpath)

    # Get the list of bands
    def get_list_of_band_code(self):
        # ---------------------------------------------------------------------------------------------
        localpath = "//Spectral_Band_Informations"
        return xml_tools.get_attribute_values(self.root, localpath, "band_id")

    # Get the index of band
    def get_index_of_band_code(self, band_code):
        # ---------------------------------------------------------------------------------------------
        return self.get_list_of_bands().index(band_code)

    # Get the list of bands for a particuliar resolution
    def get_list_of_band_code_for_resol(self, resol):
        # ---------------------------------------------------------------------------------------------
        l_count = xml_tools.get_attribute(self.root, "//Band_Group_List/Group [@group_id='" + resol + "']/Band_List",
                                          "count")
        localpath = "//Band_Group_List/Group [@group_id='" + resol + "']/Band_List[@count='" + l_count + "']/BAND_ID"
        return xml_tools.get_string_list_from_nodes(self.root, localpath)

    # Get the index of band

    def get_index_of_band_code_for_resol(self, band_code, resol):
        # ---------------------------------------------------------------------------------------------
        return self.get_list_of_band_code_for_resol(resol).index(band_code)

    # Get the saturated mask filename
    def get_list_of_l1_sat_image_filenames(self):
        listofsat = []
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        # Band Loop
        for bd in listofbands:
            localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Saturation']/Mask_File_List/MASK_FILE[@band_id='"\
                        + bd + "']"
            lsatimagefilename = os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))
            listofsat.append(lsatimagefilename)
        return listofsat

    # Get the saturated mask index within its file
    def get_l1_sat_image_index(self, bandidx):
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Saturation']/Mask_File_List/MASK_FILE[@band_id='" + \
                    listofbands[bandidx] + "']"
        LOGGER.debug("localPath:" + localpath)
        return int(xml_tools.get_attribute(self.root, localpath, "bit_number"))

    def get_list_of_pix_mask_filenames(self):
        listofpix = []
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        # Band Loop
        for bd in listofbands:
            localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Aberrant_Pixels']/" + \
                        "Mask_File_List/MASK_FILE[@band_id='" + bd + "']"
            lpiximagefilename = os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))
            listofpix.append(lpiximagefilename)
        return listofpix

    def get_list_of_pix_mask_indices(self):
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Aberrant_Pixels']/" + \
                "Mask_File_List/MASK_FILE[@band_id='{}']"
        return [int(xml_tools.get_attribute_values(self.root, localpath.format(bd), "bit_number")[0]) for bd in self.get_list_of_bands()]

    def get_l1_pix_mask_index(self, bandidx):
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        localpath = "//Mask[Mask_Properties/NATURE='Aberrant_Pixels']/Mask_File_List/MASK_FILE[@band_id='" + \
            listofbands[bandidx] + "']"
        LOGGER.debug("localPath:" + localpath)
        return [int(f) for f in xml_tools.get_attribute_values(self.root, localpath, "bit_number")]

    # Get the no data mask filename
    def get_list_of_l1_ndt_image_filenames(self):
        listofndt = []
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        # Band Loop
        for bd in listofbands:
            localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Nodata']/Mask_File_List/MASK_FILE[@band_id='" \
                        + bd + "']"
            lndtimagefilename = os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))
            listofndt.append(lndtimagefilename)
        return listofndt

    # Get the saturated mask index within its file
    def get_l1_ndt_image_index(self, bandidx):
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Nodata']/Mask_File_List/MASK_FILE[@band_id='" + \
            listofbands[bandidx] + "']"
        LOGGER.debug("localPath:" + localpath)
        return int(xml_tools.get_attribute(self.root, localpath, "bit_number"))

    # Get the zone mask filename for the band
    def get_list_of_zones(self, bandid):
        # Convert the band index integer value in string
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Detector_Footprint']/Mask_File_List/MASK_FILE[@band_id='"\
                    + bandid + "']"
        # Zones are equivalent to detectors
        # Get the last attribute values (detector index) from nodes
        out = xml_tools.get_attribute_values(self.root, localpath, "detector_id")
        LOGGER.debug("zones (1st try) : "+str(out))
        # if no zone found, try Mean_Viewing_Incidence_Angle (Venus case)
        if (len(out) == 0) and (not self.has_per_band_angles()):
            anglespath = "//Mean_Viewing_Incidence_Angle_List/Mean_Viewing_Incidence_Angle"
            res = self.root.xpath(anglespath)
            LOGGER.debug("nb nodes: "+str(len(res)))
            out = xml_tools.get_attribute_values(self.root, anglespath, "detector_id")
            LOGGER.debug("zones (2nd try) : "+str(out))
        return out

    # Get the zone image index
    def get_l1_dtf_image_index(self, band, detid):
        l_listofbands = self.get_list_of_band_code()
        localpath = "//Mask[Mask_Properties/NATURE='Detector_Footprint']/Mask_File_List/MASK_FILE[@band_id='" + \
            l_listofbands[band] + "'][@detector_id='" + detid + "']"
        LOGGER.debug("localPath:" + localpath)
        return [int(f) for f in xml_tools.get_attribute_values(self.root, localpath, "bit_number")]

    # Get the zone image max index for a detector index
    def get_l1_dtf_max_image_index(self, detid):
        localpath = "//Mask[Mask_Properties/NATURE='Detector_Footprint']/Mask_File_List/MASK_FILE[@detector_id='"\
                    + detid + "']"
        LOGGER.debug("localPath:" + localpath)
        list_of_number = [int(f) for f in xml_tools.get_attribute_values(self.root, localpath, "bit_number")]
        return max(list_of_number)

    # Get the list of name of the L1 TOA image
    def get_list_of_toa_image_filenames(self):
        listofl1toa = []
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        # Band Loop
        for bd in listofbands:
            localpath = "//IMAGE_FILE[@band_id='" + bd + "']"
            ltoaimagefilename = os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))
            listofl1toa.append(ltoaimagefilename)
        return listofl1toa

    # Get the list of name of the L1 TOA image
    def get_list_of_defective_pixel_image_filenames(self):
        listoffname = []
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        # Band Loop
        for bd in listofbands:
            localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Defective_Pixel']/" + \
                        "Mask_File_List/MASK_FILE[@band_id='" + bd + "']"
            ldefectimagefilename = os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))
            listoffname.append(ldefectimagefilename)
        return listoffname

    def get_useful_image_infos_filename(self):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        localpath = "//Data_List/Data[Data_Properties/NATURE='Useful_Image_Informations_File']/Data_File_List/DATA_FILE"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_cla_image_filename(self):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        localpath = "//Data_List/Data[Data_Properties/NATURE='Cloud_Altitude_Grid']/Data_File_List/DATA_FILE"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_designated_fill_mask_filename(self):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Designated_Fill']/Mask_File_List/MASK_FILE"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_sol_image_filename(self, p_alt, p_axis):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        lalt = str(p_alt) + "m"
        localpath = "//Data_List/Data[Data_Properties/NATURE='Solar_Angles_Grid']/Data_File_List/DATA_FILE[@altitude='"\
                    + \
            lalt + "'][@axis='" + p_axis + "']"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_sol_image_index(self, p_alt, p_axis):
        # Get the path in the xml product filename
        lalt = str(p_alt) + "m"
        localpath = "//Data_List/Data[Data_Properties/NATURE='Solar_Angles_Grid']/Data_File_List/DATA_FILE[@altitude='"\
                    + \
            lalt + "'][@axis='" + p_axis + "']"
        return xml_tools.get_attribute_values(self.root, localpath, "band_number")

    def get_vie_image_filename(self, p_det, p_axis):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        localpath = "//Data_List/Data[Data_Properties/NATURE='Viewing_Angles_Grid']/" + \
                    "Data_File_List/DATA_FILE[@detector_id='" + \
            p_det + "'][@axis='" + p_axis + "']"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_vie_image_index(self, p_det, p_axis):
        # Get the path in the xml product filename
        localpath = "//Data_List/Data[Data_Properties/NATURE='Viewing_Angles_Grid']/" +\
                    "Data_File_List/DATA_FILE[@detector_id='" + \
            p_det + "'][@axis='" + p_axis + "']"
        return xml_tools.get_attribute_values(self.root, localpath, "band_number")

    # Get the list of name of the L1 TOA image
    def get_map_list_of_detector_footprint_image_filenames(self):
        mapofdftname = []
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        # Get the list of bands
        listofbands = self.get_list_of_bands()
        # Band Loop
        for bd in listofbands:
            listofzone = self.get_list_of_zones(bd)
            l_footprintmap = {}
            # detector Loop
            for idz in listofzone:
                localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Detector_Footprint']/" +\
                            "Mask_File_List/MASK_FILE[@band_id='" + bd + "' and @detector_id='" + idz + "']"
                l_imagefilename = os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))
                l_footprintmap[idz] = l_imagefilename
            mapofdftname.append(l_footprintmap)
        return mapofdftname

    def _get_list_of_angles_all_detectors_per_band(self, relativepath, bandid, angletype):
        # AngleType contain "Zenith" or "Azimuth"
        output = []
        # Convert the band index integer value in string
        path = relativepath + "[@band_id='" + bandid + "']/Viewing_Incidence_Angles_Grids"
        zonelist = self.get_list_of_zones(bandid)
        for det in zonelist:
            subpath = path + "[@detector_id='" + det + "']"
            valueslist = xml_tools.get_list_of_string_child_values(
                self.root, os.path.join(subpath, angletype + "/Values_List"))
            output.append(valueslist)
        return output

    """                    L2                      """
    # Get the list of L2 surface reflectance filenames  (Full path)

    def get_list_of_l2_surface_reflectance_filenames(self, resol):
        return self._get_list_of_reflectance_filenames("Surface_Reflectance", resol)

    # Get the list of L2 flat reflectance filenames  (Full path)
    def get_list_of_l2_flat_reflectance_filenames(self, resol):
        return self._get_list_of_reflectance_filenames("Flat_Reflectance", resol)

    def _get_list_of_reflectance_filenames(self, nature, resol):
        list_of_files = []
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        # Get the list of bands
        listofbands = self.get_list_of_band_code_for_resol(resol)
        # Band Loop
        for bd in listofbands:
            localpath = "//Image[Image_Properties/NATURE='" + nature + \
                "']/Image_File_List/IMAGE_FILE[@band_id='" + bd + "']"
            l_imagefilename = os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))
            list_of_files.append(l_imagefilename)

        return list_of_files

    def get_l2_atb_filename(self, resol):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        localpath = "//Image[Image_Properties/NATURE='Water_Vapor_Content']/Image_File_List/IMAGE_FILE[@group_id='" \
                    + resol + "']"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_l2_cld_filename(self, resol):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Detailed_Cloud']/Mask_File_List/MASK_FILE[@group_id='" + \
                    resol + "']"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_l2_vap_index(self):
        # Get the path in the xml product filename
        localpath = "//Image[Image_Properties/NATURE='Water_Vapor_Content']/Image_File_List/IMAGE_FILE"
        return xml_tools.get_attribute_values(self.root, localpath, "band_number")

    def get_l2_wat_index(self):
        # Get the path in the xml product filename
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Water']/Mask_File_List/MASK_FILE"
        return xml_tools.get_attribute_values(self.root, localpath, "bit_number")

    def get_l2_snow_index(self):
        # Get the path in the xml product filename
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Snow']/Mask_File_List/MASK_FILE"
        return xml_tools.get_attribute_values(self.root, localpath, "bit_number")

    def get_l2_aot_index(self):
        # Get the path in the xml product filename
        localpath = "//Image[Image_Properties/NATURE='Aerosol_Optical_Thickness']/Image_File_List/IMAGE_FILE"
        return xml_tools.get_attribute_values(self.root, localpath, "band_number")

    def get_list_of_l2_sat_image_filenames(self, resol):
        listofsat = []
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        # Get the list of bands
        listofbands = self.get_list_of_band_code_for_resol(resol)
        # Band Loop
        for bd in listofbands:
            localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Saturation']/Mask_File_List/MASK_FILE[@band_id='" + \
                        bd + "']"
            lsatimagefilename = os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))
            listofsat.append(lsatimagefilename)
        return listofsat

    def get_l2_edg_filename(self, resol):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Edge']/Mask_File_List/MASK_FILE[@group_id='" +\
                    resol + "']"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_l2_mg2_filename(self, resol):
        # Get the path in the xml product filename
        lpath = os.path.dirname(self.main_xml_file)
        localpath = "//Mask_List/Mask[Mask_Properties/NATURE='Water']/Mask_File_List/MASK_FILE[@group_id='" + \
                    resol + "']"
        return os.path.join(lpath, xml_tools.get_xml_string_value(self.root, localpath))

    def get_the_xpath_root_of_the_private_directory_filename(self):
        return "//Data_List/Data[Data_Properties/NATURE='Private_Directory']"

    def has_per_band_angles(self):
        nodes = self.root.xpath("//Mean_Value_List/Mean_Viewing_Incidence_Angle_List/Mean_Viewing_Incidence_Angle")
        if len(nodes):
            if 'band_id' in nodes[0].keys():
                return True
        return False
