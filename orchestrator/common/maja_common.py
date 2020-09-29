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
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.cots.gdal.gdal_rasterize -- shortdesc

orchestrator.cots.gdal.gdal_rasterize is a description

It defines classes_and_methods


###################################################################################################
"""


from orchestrator.common.logger.maja_logging import configure_logger

LOGGER = configure_logger(__name__)


class Area:

    def __init__(self):
        self.origin = ()
        self.spacing = ()
        self.size = ()

    def __repr__(self):
        message = "Area :  Origin   : [ {} ; {} ] \n" \
                  "Spacing  : [ {} ; {} ] \n" \
                  "Size     : [ {} ; {} ] \n".format(self.origin[0], self.origin[1],
                                                     self.spacing[0], self.spacing[1],
                                                     self.size[0], self.size[1])
        return message


class BoundingBox(object):

    def __init__(self):
        self.xmin = 0
        self.ymin = 0
        self.xmax = 0
        self.ymax = 0


class PointXY:
    def __init__(self, vx=0.0, vy=0.0):
        self.x = vx
        self.y = vy
    def __add__(self, b):
        return PointXY(self.x + b.x, self.y + b.y)
    def __sub__(self, b):
        return PointXY(self.x - b.x, self.y - b.y)
    def __mul__(self, c):
        if isinstance(c,PointXY):
            return PointXY(self.x * c.x, self.y * c.y)
        else:
            return PointXY(self.x * c, self.y * c)
    def __rmul__(self,c):
        if isinstance(c,PointXY):
            return PointXY(c.x * self.x, c.y * self.y)
        else:
            return PointXY(c * self.x, c * self.y)

class Corner:

    def __init__(self):
        self.longitude = 0
        self.latitude = 0
        self.line = 0
        self.column = 0


# Plugin information : GIPPFileType
class GIPPFile:
    def __init__(self):
        self.Nature = ""
        self.FileLocation = ""
        self.LogicalName = ""


# EarthExplorer information : RegularFile
class RegularFile:
    def __init__(self):
        self.Nature = ""
        self.FileLocation = ""
        self.LogicalName = ""

# EarthExplorer information : AnnexFile


class AnnexFile:
    def __init__(self):
        self.sk = ""
        self.Nature = ""
        self.FileLocation = ""
        self.LogicalName = ""
