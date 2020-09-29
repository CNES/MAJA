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
#
#                        o     o
#                        oo   oo   oo        o   oo        ,-.
#                        o o o o  o  o       o  o  o       \_/
#                        o  o  o o    o      o o    o     {|||)<
#                        o     o oooooo      o oooooo      / \
#                        o     o o    o o    o o    o      `-^
#                        o     o o    o  oooo  o    o
#
###################################################################################################

orchestrator.plugins.maja_dataset -- shortdesc

orchestrator.plugins.maja_dataset is a description

It defines classes_and_methods


###################################################################################################
"""
from orchestrator.common.muscate.muscate_xml_file_handler import MuscateXMLFileHandler
from orchestrator.common.muscate.muscate_uii_xml_file_handler import MuscateUIIXMLFileHandler
from orchestrator.plugins.common.muscate.maja_muscate_l1_image_info import MajaMuscateL1ImageInformations
from orchestrator.plugins.venus_muscate.maja_venus_muscate_plugin import MajaVenusMuscatePlugin
from orchestrator.common.logger.maja_logging import configure_logger
import orchestrator.common.date_utils as date_utils
LOGGER = configure_logger(__name__)


class MajaVenusMuscateL1ImageInformations(MajaMuscateL1ImageInformations):

    def __init__(self):
        super(MajaVenusMuscateL1ImageInformations, self).__init__()
        self._plugin = MajaVenusMuscatePlugin()
        self.UniqueSatellite = self._plugin.UniqueSatellite
        self.IS_TILE_DEPENDENT = False

    def detect_l1_products(self, input_directory, product_list):
        LOGGER.info("Start VenusMuscate L1 DetectL1Products in " + input_directory)
        MajaMuscateL1ImageInformations.muscate_detect_l1_products(input_directory, product_list, self._plugin)

    def initialize(self, product_filename, validate=False, schema_path=None):
        LOGGER.info("Start VenusMuscate L1 Initialize on product " + product_filename)
        # Initialize the Image filename provider
        if not MajaMuscateL1ImageInformations.is_a_muscate_by_checking_the_filename(product_filename):
            LOGGER.debug("The filename <" + product_filename + "> is not an 'muscate' L1 header file.")
            return False
        if not MajaMuscateL1ImageInformations.is_a_muscate_by_checking_the_satellite(product_filename, self._plugin):
            LOGGER.debug(
                "The filename <" +
                product_filename +
                "> is not an 'muscate' L1 header file (by reading platform in the xml file).")
            return False
        # Generic metadata handling by MuscateL1ImageInfo
        if not self.muscate_initialize(product_filename, self._plugin):
            LOGGER.debug("Failed to load generic muscate information")
            return False

        # Customization for VENUS
        # Init XML handler
        lHandler = MuscateXMLFileHandler(product_filename, validate=validate, schema_path=schema_path)

        # Read the usefull image information xml
        l_useful_filename = lHandler.get_useful_image_infos_filename()
        l_uii_handler = MuscateUIIXMLFileHandler(l_useful_filename)

        # Get longitude and latitude coordinates of the product
        # -------------------------------------------------------------------------
        # Estimation of the coordinate of the central point
        l_center = l_uii_handler.get_geopositioning_center()
        self.CenterCorner.longitude = l_center[0]
        self.CenterCorner.latitude = l_center[1]
        self.CenterCorner.line = 0
        self.CenterCorner.column = 0

        self.ListOfViewingAnglesPerBandAtL2Resolution = []
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution = []

        # Initialize the Viewing angles for each detectors (Zenith and Azimuth)
        # Read the constants values from the Header Envi file name
        # The angles must be in degree
        l_ViewAngle1 = l_uii_handler.get_image_center_viewing_angles("01")
        l_ViewAngle2 = l_uii_handler.get_image_center_viewing_angles("02")
        l_ViewAngle3 = l_uii_handler.get_image_center_viewing_angles("03")
        l_ViewAngle4 = l_uii_handler.get_image_center_viewing_angles("04")

        LOGGER.debug("Viewing Zenith - tri-detector 1 : " + str(l_ViewAngle1[0]))
        LOGGER.debug("Viewing Zenith - tri-detector 2 : " + str(l_ViewAngle2[0]))
        LOGGER.debug("Viewing Zenith - tri-detector 3 : " + str(l_ViewAngle3[0]))
        LOGGER.debug("Viewing Zenith - tri-detector 4 : " + str(l_ViewAngle4[0]))

        # B01 -> D01
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle1[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle1[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle1[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle1[1])
                                                                    })
        # B02 -> D01
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle1[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle1[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle1[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle1[1])
                                                                    })
        # B03 -> D04
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle4[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle4[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle4[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle4[1])
                                                                    })
        # B04 -> D04
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle4[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle4[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle4[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle4[1])
                                                                    })
        # B05 -> D01
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle1[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle1[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle1[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle1[1])
                                                                    })
        # B06 -> D04
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle4[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle4[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle4[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle4[1])
                                                                    })
        # B07 -> D03
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle3[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle3[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle3[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle3[1])
                                                                    })
        # B08 -> D03
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle3[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle3[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle3[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle3[1])
                                                                    })
        # B09 -> D03
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle3[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle3[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle3[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle3[1])
                                                                    })
        # B10 -> D02
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle2[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle2[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle2[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle2[1])
                                                                    })
        # B11 -> D02
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle2[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle2[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle2[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle2[1])
                                                                    })
        # B12 -> D02
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle2[0]))
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_ViewAngle2[1]))
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append({"incidence_zenith_angle": str(l_ViewAngle2[0]),
                                                                    "incidence_azimuth_angle": str(l_ViewAngle2[1])
                                                                    })
        # Fill the L2 resolution angles
        self.ListOfViewingAnglesPerBandAtL2Resolution = self.ListOfViewingAnglesPerBandAtL2CoarseResolution

        # Viewing Angles
        self.ViewingAngle = {
                "incidence_zenith_angle": str( (l_ViewAngle1[0]+l_ViewAngle2[0]+l_ViewAngle3[0]+l_ViewAngle4[0])/4.0 ),
                "incidence_azimuth_angle": str( (l_ViewAngle1[1]+l_ViewAngle2[1]+l_ViewAngle3[1]+l_ViewAngle4[1])/4.0 )
                }

        # Solar Angles
        lSolarAngles = l_uii_handler.get_image_center_solar_angles()
        self.SolarAngle = {
            "sun_zenith_angle": lSolarAngles[0],
            "sun_azimuth_angle": lSolarAngles[1]
        }

        return True
