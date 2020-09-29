#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (C) 2016-2020 Centre National d'Etudes Spatiales (CNES), CSSI, CESBIO  All Rights Reserved

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import sys
import os
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))  # Import relative modules

__version__ = "4.0.1"


class DTMCreator:
    """
    Class to create the DTM based on the SRTMs, Water-masks and Metadata of a
    L1 product file downloaded. The output DTM contains an HDR File and a Folder
    both of filename *AUX_REFDE2*, which are created in the given output path
    """

    def __init__(self, product_path, dem_dir, water_dir, type_dem, coarse_res, full_res_only):
        """
        Init the DTMCreator by finding the Metadata file assiociated with the product
        :param product_path: The full path to the L1C/L2A product folder
        :type product_path: string
        :param dem_dir: DEM directory
        :param water_dir: Water-mask directory
        :param type_dem: Type of mnt
        :param coarse_res: Coarse resolution of the MNT in Meter
        :type coarse_res: int
        :param full_res_only: Write full resolution masks only
        """
        from StartMaja.Chain import Product
        self.product = Product.MajaProduct.factory(os.path.normpath(product_path))
        print(self.product)
        if not self.product:
            raise ValueError("Unknown product found for path %s" % product_path)
        self.dem_dir = dem_dir
        self.water_dir = water_dir
        self.type_dem = type_dem
        if self.type_dem != "srtm" and not self.dem_dir:
            raise ValueError("Need to provide param '--dem_dir' for chosen DEM type: %s" % self.type_dem)
        self.coarse_res = int(coarse_res)
        self.full_res_only = full_res_only

    def run(self, outdir, tempdir):
        """
        Run the DTM Creation using the two modules tuilage_mnt_eau*py and lib_mnt.py
        :param outdir: Output directory
        :param tempdir: Temporary-/Working-directory
        """
        self.product.get_mnt(dem_dir=outdir,
                             raw_dem=self.dem_dir,
                             raw_gsw=self.water_dir,
                             wdir=tempdir,
                             type_dem=self.type_dem,
                             full_res_only=self.full_res_only)
        print("Finished DTM creation for site/tile %s" % self.product.tile)


if __name__ == "__main__":
    import sys
    from osgeo import gdal
    assert sys.version_info > (3, 0), "Please update Python to >3.0"
    # Script runs fine with gdal >= 2.1.x
    if int(gdal.VersionInfo()) <= 2010000:
        raise ImportError("Please update your GDAL version to >2.1")
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--product",
                        help="The path to a Landsat-8, Venus or Sentinel-2 L1C/L2A product folder.",
                        required=True, type=str)
    parser.add_argument("-o", "--out_dir", help="Output directory. Default is the current directory.",
                        default=os.getcwd(), required=False, type=str)
    parser.add_argument("-d", "--dem_dir",
                        help="The path to the folder containing the downloaded DEM files/archives."
                             "If not existing, an attempt will be made to download them.", required=False, type=str)
    parser.add_argument("-w", "--water_dir",
                        help="The path to the folder containing the GSW occurence .tif-files."
                             "If not existing, an attempt will be made to download them.", required=False, type=str)
    parser.add_argument("-t", "--temp_dir",
                        help="The path to temp the folder."
                             "If not existing, it is set to a /tmp/ location", required=False, type=str)
    parser.add_argument("--type_dem",
                        help="DEM type. Default is srtm.", required=False, type=str, default="srtm",
                        choices=["srtm", "eudem"])
    parser.add_argument("-c", "--coarse_res",
                        help="Coarse resolution in meters. Default is 240", default=240, required=False, type=int)
    parser.add_argument("--full_res_only", help="Output full resolution imgs only. Discards the coarse_res parameter.",
                        action="store_true", required=False, default=False)
    parser.add_argument('-v', '--version', action='version', version='%(prog)s ' + str(__version__))
    args = parser.parse_args()
    creator = DTMCreator(args.product, args.dem_dir, args.water_dir, args.type_dem, args.coarse_res, args.full_res_only)
    creator.run(args.out_dir, args.temp_dir)
