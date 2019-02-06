#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Tool to create the DTM files expected by MAJA

Author:         Peter KETTIG <peter.kettig@cnes.fr>, Pierre LASSALLE <pierre.lassalle@cnes.fr>
Project:        StartMaja, CNES
Created on:     Tue Sep 11 15:31:00 2018

==================== Copyright
Software (DTMCreation.py)

Copyright© 2018 Centre National d’Etudes Spatiales

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not, see
https://www.gnu.org/licenses/gpl-3.0.fr.html

"""

import sys, os
assert sys.version_info >= (2,7)
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..')) #Import relative modules
from Common import FileSystem
from prepare_dtm import conversion_format_maja as conversion

class DTMCreator():
    """
    Class to create the DTM based on the SRTMs, Water-masks and Metadata of a
    L1 product file downloaded. The output DTM contains an HDR File and a Folder
    both of filename *AUX_REF*, which are both created in the given output path
    """
    filenameL1NativeS2 = "S2*_MSIL1C_*"
    filenameL1NativeVns = "VENUS*L1C_*"
    filenameL1MuscateS2 = "SENTINEL2*_L1C_*"
    filenameL1MuscateVns = "VENUS*_L1C_*"
    filenameSRTM = r"[srtm]\_\d+\_\d+\.zip"
    filenameWater = r"[we]\d+[ns]\d+[a-z]\.zip"
    venusMission = "VENUS"
    s2Mission = "SENTINEL2"
    typeMuscate = "MUSCATE"
    typeNatif = "NATIF"
    modeUndefined = -1
    modeMTD = 0
    modeGranule = 1
    mode = -1 # Processing mode: 0 = MTD; 1 = GRANULE; -1= Undefined
    
    def __init__(self, dirProduct, kml, granuleID, dirSRTM, dirWater, waterOnly=False, mntType="SRTM", coarseRes=240):
        """
        Init the DTMCreator by finding the Metadata file
        
        :param dirProduct: The full path to the product folder
        :type product: string
        :param kml: Path to the S2 tiling grid kml
        :param granuleID: Granule- or Tile-ID
        :param dirSRTM: SRTM directory
        :param dirWater: SWBD directory
        :param waterOnly: True, if only water masks shall be generated, false if not
        :type waterOnly: bool
        :param mntType: Type of mnt (SRTM)
        :type mntType: string
        :param coarseRes: Coarse resolution of the MNT in Meter
        :type coarseRes: int
        """
        
        import re
        import os     
        files = [f for f in os.listdir(dirSRTM) if re.search(self.filenameSRTM, f)]
        if(files):
            print("Found SRTM zip-archives...")
        else:
            files = [f for f in os.listdir(dirWater) if re.search(self.filenameSRTM.replace("zip","tif"), f)]
            if(files):
                raise ValueError("Need to provide unextracted zip-archives of SRTM: {0}".format(" ".join(f for f in files)))
            else:
                raise OSError("Cannot find SRTM-archives in directory: {0}".format(dirWater))
                
        files = [f for f in os.listdir(dirWater) if re.search(self.filenameWater, f)]
        if(files):
            print("Found Water zip-archives...")
            self.WaterZipped = True
        else:
            raise OSError("Cannot find Water-Mask zip-archives in directory: {0}".format(dirWater))

        self.dirInSRTM = dirSRTM
        self.dirInWater = dirWater
        assert type(waterOnly) == bool
        self.waterOnly = waterOnly
        self.mntType = mntType
        self.coarseRes = coarseRes
        
        #Decide where to retrieve the site data from
        if(dirProduct):
            self.mission, self.mtd, self.dtype = self.findMetadataFile(dirProduct)
            if(not self.mtd):
                raise ValueError("Cannot find Metadata for product: {0}".format(dirProduct))
            self.mode = self.modeMTD
        elif(kml and granuleID):
            print("Processing with GranuleID")
            self.mode = self.modeGranule
            self.kml = kml
            self.granule = granuleID
        else:
            raise ValueError("Need to specify either L1 product directory or Tiling Grid KML")
        pass
    
    def isNativeProduct(self, fullpath):
        """
        Try to find a matching file-pattern for native products using fnmatch
        
        :param fullpath: The full path to the product
        :type fullpath: string
        :return: Returns the mission name if path is existing or none if not
        :rtype: string
        """
        import fnmatch
        import os
        #Remove trailing slash if existing
        product = [os.path.basename(fullpath if fullpath[-1:] != "/" else fullpath[:-1])]
        if(fnmatch.filter(product, self.filenameL1NativeS2) != []):
            return self.s2Mission
        if(fnmatch.filter(product, self.filenameL1NativeVns) != []):
            return self.venusMission
        return ""
    
    def isMuscateProduct(self, fullpath):
        """
        Try to find a matching file-pattern for muscate products using fnmatch
        
        :param fullpath: The full path to the product
        :type fullpath: string
        :return: Returns the mission name if path is existing or none if not
        :rtype: string
        """
        import fnmatch
        import os
        #Remove trailing slash if existing
        product = [os.path.basename(fullpath if fullpath[-1:] != "/" else fullpath[:-1])]
        if(fnmatch.filter(product, self.filenameL1MuscateS2) != []):
            return self.s2Mission
        if(fnmatch.filter(product, self.filenameL1MuscateVns) != []):
            return self.venusMission
        return ""
    
    def findMetadataFile(self, product):
        """
        Find Mission and metadata-File
        
        :param product: fullpath to the product
        :type fullpath: string
        :return: The mission name as well as the path to the metadata
        :rtype: string
        """
        import glob
        import os
        
        if(os.path.isfile(product)):
            raise ValueError("Need to provide product parent directory, not a file: {0}".format(product))
        mission = self.isNativeProduct(product)
        if(mission):
            return mission, glob.glob(os.path.join(product,"GRANULE","L1C_*","MTD_TL.xml"))[0], self.typeNatif
        mission = self.isMuscateProduct(product)
        if(mission):
            print(os.path.join(product,"*_L1C_*_MTD_ALL.xml"))
            return mission, glob.glob(os.path.join(product,"*_L1C_*_MTD_ALL.xml"))[0], self.typeMuscate
        raise ValueError("Unkown mission found for product {0}".format(product))

    def getSiteInfo(self, mtd, dtype):
        """
        Get all the necessary parameters for the site info and create the site-object
        For each Metadata type extract the needed info and process it individually if needed (Tile-name etc.)
        Common parameters are processed afterwards
        :param mtd: Path to the metadata-file
        :param dtype: Type of metadata - Natif or Muscate for now
        """
        from lxml import etree
        from prepare_dtm.tuilage_mnt_eau_S2 import TuilageParamsConverter
        if(dtype == self.typeNatif):
            xpathTileName = "/n1:Level-1C_Tile_ID/n1:General_Info/TILE_ID"
            xpathCSName = "/n1:Level-1C_Tile_ID/n1:Geometric_Info/Tile_Geocoding/HORIZONTAL_CS_NAME"
            xpathCSCode = "/n1:Level-1C_Tile_ID/n1:Geometric_Info/Tile_Geocoding/HORIZONTAL_CS_CODE"
            xpathWidth = "/n1:Level-1C_Tile_ID/n1:Geometric_Info/Tile_Geocoding/Size[@resolution='10']/NROWS"
            xpathHeight = "/n1:Level-1C_Tile_ID/n1:Geometric_Info/Tile_Geocoding/Size[@resolution='10']/NCOLS"
            xpathULX = "/n1:Level-1C_Tile_ID/n1:Geometric_Info/Tile_Geocoding/Geoposition[@resolution='10']/ULX"
            xpathULY = "/n1:Level-1C_Tile_ID/n1:Geometric_Info/Tile_Geocoding/Geoposition[@resolution='10']/ULY"
            tree = etree.parse(str(mtd))
            #Set namespace n1
            import re
            m = re.match('\{.*\}', tree.getroot().tag)
            namespace = {"n1" : m.group(0)[1:-1]}
            #Get the raw Site-values from the xml
            tileFilename = str(tree.xpath(xpathTileName, namespaces=namespace)[0].text)
            CSName = str(tree.xpath(xpathCSName, namespaces=namespace)[0].text)
            CSCode = str(tree.xpath(xpathCSCode, namespaces=namespace)[0].text)
            width = str(tree.xpath(xpathWidth, namespaces=namespace)[0].text)
            height = str(tree.xpath(xpathHeight, namespaces=namespace)[0].text)
            ULX = str(tree.xpath(xpathULX, namespaces=namespace)[0].text)
            ULY = str(tree.xpath(xpathULY, namespaces=namespace)[0].text)
            #Get TileID
            tileFilenameItems = tileFilename.split("_")
            assert len(tileFilenameItems) == 11
            assert tileFilenameItems[9][0] == "T"
            tileName = str(tileFilenameItems[9][1:])
            #EPSG_out e.g. 32632
            assert CSCode[0:4] == "EPSG"
            EPSGOut = int(CSCode[5:])
            #Get CSInfo - Zone (e.g. UTMxxN/S)
            CSItems = CSName.split(" ")
            assert CSItems[0] == "WGS84"
            assert len(CSItems) == 5
            zone = str(CSItems[2]+CSItems[4])

        if(dtype == self.typeMuscate):
            xpathTileName = "/Muscate_Metadata_Document/Dataset_Identification/GEOGRAPHICAL_ZONE"
            xpathCSName = "/Muscate_Metadata_Document/Geoposition_Informations/Coordinate_Reference_System/Horizontal_Coordinate_System/HORIZONTAL_CS_NAME"
            xpathCSCode = "/Muscate_Metadata_Document/Geoposition_Informations/Coordinate_Reference_System/Horizontal_Coordinate_System/HORIZONTAL_CS_CODE"
            xpathWidth = "/Muscate_Metadata_Document/Geoposition_Informations/Geopositioning/Group_Geopositioning_List/Group_Geopositioning[@group_id='R1']/NROWS"
            xpathHeight = "/Muscate_Metadata_Document/Geoposition_Informations/Geopositioning/Group_Geopositioning_List/Group_Geopositioning[@group_id='R1']/NCOLS"
            xpathULX = "/Muscate_Metadata_Document/Geoposition_Informations/Geopositioning/Group_Geopositioning_List/Group_Geopositioning[@group_id='R1']/ULX"
            xpathULY = "/Muscate_Metadata_Document/Geoposition_Informations/Geopositioning/Group_Geopositioning_List/Group_Geopositioning[@group_id='R1']/ULY"
            tree = etree.parse(str(mtd))
            #Get the raw Site-values from the xml
            tileNameFull = str(tree.xpath(xpathTileName, namespaces=None)[0].text)
            CSName = str(tree.xpath(xpathCSName, namespaces=None)[0].text)
            CSCode = str(tree.xpath(xpathCSCode, namespaces=None)[0].text)
            width = str(tree.xpath(xpathWidth, namespaces=None)[0].text)
            height = str(tree.xpath(xpathHeight, namespaces=None)[0].text)
            ULX = str(tree.xpath(xpathULX, namespaces=None)[0].text)
            ULY = str(tree.xpath(xpathULY, namespaces=None)[0].text)
            assert tileNameFull[0] == "T"
            tileName = tileNameFull[1:]
            EPSGOut = int(CSCode)
            #Get CSInfo - Zone (e.g. UTMxxN/S)
            CSItems = CSName.split(" ")
            assert CSItems[0] == "WGS"
            assert CSItems[1] == "84"
            assert len(CSItems) == 6
            zone = str(CSItems[3]+CSItems[5])

        #Process the common parameters:
        #Step X and Y
        pasX = int(width)*10
        pasY = int(height)*10
        #Origin
        originX = int(ULX)
        originY = int(ULY)
        converter = TuilageParamsConverter()
        print(tileName)
        print(zone)
        print(EPSGOut)
        print(pasX)
        print(pasY)
        print(originX)
        print(originY)
        return converter.getSiteFromParams(tileName, zone, EPSGOut, pasX, pasY, originX, originY)
    def run(self, outdir, tempout):
        """
        Run the DTM Creation using the two modules tuilage_mnt_eau*py and lib_mnt.py
        :param outdir: Output directory
        :param tempout: Directory to write temporary files to
        """
        FileSystem.createDirectory(outdir) #Try to create output dir 
        import os
        if(self.mode == self.modeMTD):
            self.site = self.getSiteInfo(self.mtd, self.dtype)
        elif(self.mode == self.modeGranule):
            from prepare_dtm import lib_mnt
            self.site = lib_mnt.lire_fichier_site_kml(self.kml, self.granule)
        if(tempout == None):
            import tempfile
            tempout = tempfile.mkdtemp(prefix=self.site.nom)
        print("Working directory: {0}".format(tempout))
        FileSystem.createDirectory(tempout) #Try to create tempout dir
        from prepare_dtm import tuilage_mnt_eau_S2 as tiling
        mntcreator = tiling.TuilageSentinel()
        mntcreator.run(self.dirInSRTM, self.dirInWater,tempout, tempout, self.coarseRes, site=self.site, mnt=self.mntType, waterOnly=self.waterOnly, wdir=tempout,
                       water_zipped = self.WaterZipped)
        
        if(self.waterOnly == False):
            converter = conversion.MAJAConverter()
            converter.run(self.site.nom, tempout, self.coarseRes, outdir)
        
        print("Finished DTM creation for {0}".format(self.site.nom))
    
if __name__ == "__main__":
    from osgeo import gdal, gdal_array
    # Script runs fine with gdal >= 2.1.x
    if int(gdal.VersionInfo()) <= 2010000:
        raise ImportError("Please update your GDAL version to >2.1")
    # Bug with v2.3.0 results in errors within the script:
    if int(gdal.VersionInfo()) == 2030000:
        raise ImportError("prepare_dtm can not run on GDAL 2.3.0. Please select another version")
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input",help="The path to the native or muscate L1C product folder.", required=False, type=str)
    parser.add_argument("-k", "--kml",help="The path to the Sentinel2 Tiling Grid file.", required=False, type=str)
    parser.add_argument("-g", "--granule",help="If the KML file is given, the granule ID has to be specified here.", required=False, type=str)
    parser.add_argument("-s", "--srtm",help="The path to the folder containing the extracted SRTM files. Required.", required=True, type=str)
    parser.add_argument("-w", "--water",help="The path to the folder containing the extracted Water-Body shapefiles. Required.", required=True, type=str)
    parser.add_argument("--wateronly",help="Process water only. Default is false", required=False, type=str)
    parser.add_argument("-m", "--mnt",help="MNT type. Default is SRTM", required=False, type=str)
    parser.add_argument("-c", "--coarseres",help="Coarse resolution. Default is 240", required=False, type=int)
    parser.add_argument("-o", "--out", help="Output directory. Required.", required=True, type=str)
    parser.add_argument("-t", "--tempout", help="Temporary output directory. If none is given, it is set to /tmp", required=False, type=str)
    args = parser.parse_args()

    if(args.wateronly == None):
        args.wateronly = False
    else:
        from Common import ParameterConverter
        args.wateronly = ParameterConverter.str2bool(args.wateronly)
    if(args.mnt == None):
        args.mnt = "SRTM"
    if(args.coarseres == None):
        args.coarseres = int(240)
    creator = DTMCreator(args.input, args.kml, args.granule, args.srtm, args.water, args.wateronly, args.mnt, args.coarseres)
    creator.run(args.out, args.tempout)
