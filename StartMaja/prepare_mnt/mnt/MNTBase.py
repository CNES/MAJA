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

import os
import tempfile
from osgeo import gdal
import logging
from datetime import datetime
from xml.etree import ElementTree
import numpy as np
from scipy import ndimage
from scipy.ndimage import zoom
import math
from collections import namedtuple
from StartMaja.Common import FileSystem, ImageTools, ImageIO, XMLTools
from StartMaja.prepare_mnt.mnt.DEMInfo import DEMInfo

# GlobalSurfaceWater (GSW) base url for direct-download of the files:
surface_water_url = "https://storage.googleapis.com/global-surface-water/downloads2/occurrence/occurrence_%s_v1_1.tif"


class MNT(object):
    """
    Base class to set the DEM/MNT interfaces for a given site.
    DEM := Digital Elevation model)
    MNT := Modèle numérique de terrain; french for DEM
    """
    def __init__(self, site, **kwargs):
        if not int(gdal.VersionInfo()) >= 2000000:
            raise ImportError("MNT creation needs Gdal >2.0!")
        self.site = site
        self.dem_dir = kwargs.get("dem_dir", os.path.join(tempfile.gettempdir(), "maja_dem_files"))
        if not os.path.isdir(self.dem_dir):
            FileSystem.create_directory(self.dem_dir)
        self.wdir = kwargs.get("wdir", None)
        if not self.wdir:
            self.wdir = tempfile.mkdtemp(prefix="raw_dem_")
        else:
            FileSystem.create_directory(self.wdir)
        # This needs to be provided for EuDEM:
        self.raw_dem = kwargs.get("raw_dem", None)
        if not self.raw_dem:
            self.raw_dem = tempfile.mkdtemp(prefix="raw_dem_")
        else:
            FileSystem.create_directory(self.raw_dem)
        self.raw_gsw = kwargs.get("raw_gsw", None)
        if not self.raw_gsw:
            self.raw_gsw = tempfile.mkdtemp(prefix="raw_gsw_")
        else:
            FileSystem.create_directory(self.raw_gsw)
        self.gsw_codes = self.get_gsw_codes(self.site)
        self.dem_version = kwargs.get("dem_version", None)
        self.gsw_threshold = kwargs.get("gsw_threshold", 30.)
        self.gsw_dst = kwargs.get("gsw_dst", os.path.join(self.wdir, "surface_water_mask.tif"))
        self.quiet = not kwargs.get("verbose", False)

    def get_raw_data(self):
        """
        Get the DEM raw-data from a given directory. If not existing, an attempt will be made to download
        it automatically.
        :return: Write a single raster containing the water-data
        """
        raise NotImplementedError

    def prepare_mnt(self):
        """
        Prepare the ALT file from a given MNT-type (e.g. SRTM).
        :return: Write a single raster containing the altitude-data
        """
        raise NotImplementedError

    @staticmethod
    def calc_gradient(mnt_arr, res_x, res_y):
        """
        Calculate the gradient in x and y direction.
        :param mnt_arr: The ALT numpy array
        :param res_x: The resolution in x-direction. *Both positive and negative will work.*
        :param res_y: The resolution in y-direction. *Both positive and negative will work.*
        :return: The gradient/derivative in x- and y-direction.
        """
        # TODO Find a pure numpy 2D convolution.
        kernel_horizontal = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
        kernel_vertical = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])
        dz_dc = ndimage.convolve(mnt_arr, kernel_horizontal) / 8. / np.abs(res_x)
        dz_dl = ndimage.convolve(mnt_arr, kernel_vertical) / 8. / np.abs(res_y)
        return dz_dc, dz_dl

    @staticmethod
    def calc_slope_aspect(dz_dc, dz_dl):
        """
        Calculate the slope and aspect.
        :param dz_dc: The derivative in x-direction
        :param dz_dl: The derivative in y-direction
        :return: The slope and aspect as numpy arrays.
        """
        norme = np.sqrt(dz_dc * dz_dc + dz_dl * dz_dl)
        slope = np.arctan(norme)
        aspect = np.where(dz_dc > 0, np.arccos(dz_dl / norme), 2 * np.pi - np.arccos(dz_dl / norme))
        aspect = np.where(slope < 0.0001, 0, aspect)
        slope = np.array(slope * 100., dtype=np.int16)
        aspect = np.array(aspect * 100., dtype=np.int16)
        return slope, aspect

    @staticmethod
    def resample_to_full_resolution(image, mnt_resolution, full_resolution, order=3):
        """
        Resample an image from the mnt resolution (usually a low one) to the
        max resolution of a given product (10m for S2).
        :param image: A numpy image
        :param mnt_resolution: The input resolution as tuple (res_x, res_y)
        :param full_resolution: The output resolution as tuple (res_x, res_y)
        :param order: The spline interpolation order. Default is 3.
        :return:
        """
        zoom_factor = (np.abs(mnt_resolution[0] / full_resolution[0]),
                       np.abs(mnt_resolution[1] / full_resolution[1]))

        # Order 3 = Cubic interpolation
        return zoom(image, zoom_factor, order=order)

    @staticmethod
    def get_gsw_codes(site, grid_step=10):
        """
        Get the list of GSW files for a given site.
        :param site: The site class
        :param grid_step: The step size of the grid
        :return: The list of filenames of format 'XX(E/W)_YY(N/S)' needed in order to cover to whole site.
        """
        if site.ul_lonlat[0] > 170 and site.lr_lonlat[0] < 160:
            raise ValueError("Cannot wrap around longitude change")

        point = namedtuple("point", ("x", "y"))
        pts = []
        for pt in [site.ul_lonlat, site.lr_lonlat]:
            lat_dec = (math.fabs(pt[1]) / grid_step)
            lon_dec = (math.fabs(pt[0]) / grid_step)
            if pt[1] > 0:
                lat_id = int(math.ceil(lat_dec) * grid_step)
            else:
                lat_id = -1 * int(math.floor(lat_dec) * grid_step)

            if pt[0] < 0:
                lon_id = int(math.ceil(lon_dec) * grid_step)
            else:
                lon_id = -1 * int(math.floor(lon_dec) * grid_step)
            pts.append(point(lon_id, lat_id))
        gsw_granules = []
        for x in range(pts[1].x, pts[0].x + grid_step, grid_step):
            for y in range(pts[1].y, pts[0].y + grid_step, grid_step):
                code_lat = "S" if y < 0 else "N"
                code_lon = "W" if x > 0 else "E"
                gsw_granules.append("%s%s_%s%s" % (int(math.fabs(x)), code_lon, int(math.fabs(y)), code_lat))
        return gsw_granules

    def get_raw_water_data(self):
        """
        Find the given gsw files or download them if not existing.

        :return: The list of filenames downloaded.
        """
        filenames = []
        for code in self.gsw_codes:
            current_url = surface_water_url % code
            filename = os.path.basename(current_url)
            output_path = os.path.join(self.raw_gsw, filename)
            if not os.path.isfile(output_path):
                # Download file:
                FileSystem.download_file(current_url, output_path, log_level=logging.INFO)
            filenames.append(output_path)
        return filenames

    def prepare_water_data(self):
        """
        Prepare the water mask constituing of a set of gsw files.

        :return: Writes the tiles water_mask to the self.gsw_dst path.
        """
        occ_files = self.get_raw_water_data()
        vrt_path = os.path.join(self.wdir, "vrt_%s.vrt" % self.site.nom)
        ImageTools.gdal_buildvrt(*occ_files, dst=vrt_path)
        # Overlay occurrence image with same extent as the given site.
        # Should the occurrence files not be complete, this sets all areas not covered by the occurrence to 0.
        ds_warped = ImageTools.gdal_warp(vrt_path,
                                         r="near",
                                         te=self.site.te_str,
                                         t_srs=self.site.epsg_str,
                                         tr=self.site.tr_str,
                                         dstnodata=0,
                                         multi=True)
        # Threshold the final image and write to destination:
        image_bin = ds_warped.array > self.gsw_threshold
        FileSystem.remove_file(vrt_path)
        ImageIO.write_geotiff_existing(image_bin, self.gsw_dst, ds_warped.get_ds())

    def to_maja_format(self, platform_id, mission_field, mnt_resolutions, coarse_res, full_res_only=False):
        """
        Writes an MNT in Maja (=EarthExplorer) format: A folder .DBL.DIR containing the rasters and an
        accompanying .HDR xml-file.
        The two files follow the maja syntax::
            *AUX_REFDE2*.(HDR|DBL.DIR)
        :param platform_id: The platform ID of two digits (e.g. S2_ for Sentinel2A/B; VS for Venus)
        :param mission_field: Similar to the platform ID, this is used in the <Mission>-field for the HDR file.
                              e.g. SENTINEL-2 for S2
        :param mnt_resolutions: A dict containing the resolutions for the given sensor. E.g.::
            {"XS": (10, -10)}
        :param coarse_res: A tuple of int describing the coarse resolution. E.g.::
            (240, -240).
        :param full_res_only:  If True, no coarse_res rasters will be created.
        :return: Writes the .DBL.DIR and .HDR into the specified self.dem_dir
        """
        assert len(mnt_resolutions) >= 1
        basename = str("%s_TEST_AUX_REFDE2_%s_%s" % (platform_id, self.site.nom, str(self.dem_version).zfill(4)))

        # Get mnt data
        mnt_max_res = self.prepare_mnt()

        # Water mask not needed with optional coarse_res writing:
        if coarse_res and not full_res_only:
            # Get water data
            self.prepare_water_data()
        mnt_res = (self.site.res_x, self.site.res_y)
        dbl_base = basename + ".DBL.DIR"
        dbl_dir = os.path.join(self.dem_dir, dbl_base)
        FileSystem.create_directory(dbl_dir)
        hdr = os.path.join(self.dem_dir, basename + ".HDR")

        # Calulate gradient mask at MNT resolution:
        mnt_in, drv = ImageIO.tiff_to_array(mnt_max_res, array_only=False)
        grad_y_mnt, grad_x_mnt = self.calc_gradient(mnt_in, self.site.res_x, self.site.res_y)

        full_res = (int(mnt_resolutions[0]["val"].split(" ")[0]),
                    int(mnt_resolutions[0]["val"].split(" ")[1]))

        grad_x = self.resample_to_full_resolution(grad_x_mnt, mnt_resolution=mnt_res, full_resolution=full_res, order=3)
        grad_y = self.resample_to_full_resolution(grad_y_mnt, mnt_resolution=mnt_res, full_resolution=full_res, order=3)

        slope, aspect = self.calc_slope_aspect(grad_y, grad_x)

        # Write full res slope and aspect
        geotransform = list(drv.GetGeoTransform())
        geotransform[1] = float(full_res[0])
        geotransform[-1] = float(full_res[1])
        projection = drv.GetProjection()
        tmp_asp = tempfile.mktemp(dir=self.wdir, suffix="_asp.tif")
        ImageIO.write_geotiff(aspect, tmp_asp, projection, tuple(geotransform))
        tmp_slp = tempfile.mktemp(dir=self.wdir, suffix="_slp.tif")
        ImageIO.write_geotiff(slope, tmp_slp, projection, tuple(geotransform))

        # Full resolution:
        write_resolution_name = True if len(mnt_resolutions) > 1 else False
        # Names for R1, R2 etc.
        rasters_written = []
        path_alt, path_asp, path_slp = "", "", ""
        all_paths_alt = []
        for res in mnt_resolutions:
            # ALT:
            bname_alt = basename + "_ALT"
            bname_alt += "_" + str(res["name"]) if write_resolution_name else ""
            bname_alt += ".TIF"
            rel_alt = os.path.join(dbl_base, bname_alt)
            path_alt = os.path.join(self.dem_dir, rel_alt)
            all_paths_alt.append(path_alt)
            ImageTools.gdal_warp(mnt_max_res, dst=path_alt, tr=res["val"], r="cubic", multi=True)
            rasters_written.append(rel_alt)
            # ASP:
            bname_asp = basename + "_ASP"
            bname_asp += "_" + res["name"] if write_resolution_name else ""
            bname_asp += ".TIF"
            rel_asp = os.path.join(dbl_base, bname_asp)
            path_asp = os.path.join(self.dem_dir, rel_asp)
            ImageTools.gdal_warp(tmp_asp, dst=path_asp, tr=res["val"], r="cubic", multi=True)
            rasters_written.append(rel_asp)
            # SLP:
            bname_slp = basename + "_SLP"
            bname_slp += "_" + res["name"] if write_resolution_name else ""
            bname_slp += ".TIF"
            rel_slp = os.path.join(dbl_base, bname_slp)
            path_slp = os.path.join(self.dem_dir, rel_slp)
            ImageTools.gdal_warp(tmp_slp, dst=path_slp, tr=res["val"], r="cubic", multi=True)
            rasters_written.append(rel_slp)

        # Optional coarse_res writing:
        if coarse_res and not full_res_only:
            # Resize all rasters for coarse res.
            coarse_res_str = str(coarse_res[0]) + " " + str(coarse_res[1])
            # ALC:
            bname_alc = basename + "_ALC.TIF"
            rel_alc = os.path.join(dbl_base, bname_alc)
            path_alc = os.path.join(self.dem_dir, rel_alc)
            ImageTools.gdal_warp(path_alt, dst=path_alc, tr=coarse_res_str, multi=True)
            rasters_written.append(rel_alc)
            # ALC:
            bname_asc = basename + "_ASC.TIF"
            rel_asc = os.path.join(dbl_base, bname_asc)
            path_asc = os.path.join(self.dem_dir, rel_asc)
            ImageTools.gdal_warp(path_asp, dst=path_asc, tr=coarse_res_str, multi=True)
            rasters_written.append(rel_asc)
            # ALC:
            bname_slc = basename + "_SLC.TIF"
            rel_slc = os.path.join(dbl_base, bname_slc)
            path_slc = os.path.join(self.dem_dir, rel_slc)
            ImageTools.gdal_warp(path_slp, dst=path_slc, tr=coarse_res_str, multi=True)
            rasters_written.append(rel_slc)
            # Water mask:
            bname_msk = basename + "_MSK.TIF"
            rel_msk = os.path.join(dbl_base, bname_msk)
            path_msk = os.path.join(self.dem_dir, rel_msk)
            ImageTools.gdal_warp(self.gsw_dst, dst=path_msk, tr=coarse_res_str, multi=True)
            rasters_written.append(rel_msk)

        # Write HDR Metadata:

        date_start = datetime(1970, 1, 1)
        date_end = datetime(2100, 1, 1)
        dem_info = DEMInfo(self.site, all_paths_alt[0])
        root = self._get_root()
        self._create_hdr(root, mission_field, basename, rasters_written,
                         dem_info, date_start, date_end, self.dem_version)
        XMLTools.write_xml(root, hdr)

        # Remove temp files:
        FileSystem.remove_file(tmp_asp)
        FileSystem.remove_file(tmp_slp)
        FileSystem.remove_file(mnt_max_res)
        return hdr, dbl_dir

    @staticmethod
    def _get_root():
        """
        Create the xml-root of a single dem file
        :return: The `ElementTree.Element` root node with all namespaces and schemas filled out.
        """
        xmlns = "http://eop-cfi.esa.int/CFI"
        xsi = "http://www.w3.org/2001/XMLSchema-instance"
        schema_location = "%s ./%s" % (xmlns, "AUX_REFDE2_ReferenceDemDataLevel2.xsd")
        type_xsi = "REFDE2_Header_Type"
        root = ElementTree.Element("Earth_Explorer_Header",
                                   attrib={"xmlns": xmlns,
                                           "schema_version": "1.00",
                                           "{" + xsi + "}schemaLocation": schema_location,
                                           "{" + xsi + "}type": type_xsi})
        return root

    @staticmethod
    def _create_hdr(root, mission, basename_out, rel_files, dem_info, date_start, date_end, version):
        """
        Create a DEM HDR file with the needed information.
        :param root: The HDR xml-root.
        :param mission: The full mission name, e.g. 'SENTINEL2_'
        :param basename_out: The basename of the HDR
        :param dem_info: A DEMInfo object
        :param date_start: The validity start date, usually 1970-01-01
        :param date_end: The validity end date, usually 2100-01-01
        :param rel_files: The list of files inside the .DBL.DIR. The filepaths are all relative to the HDR.
        :return: Writes the HDR in the same directory as the .DBL.DIR.
        """
        creation_date = datetime.now()

        a1 = ElementTree.SubElement(root, "Fixed_Header")
        toto = ElementTree.SubElement(a1, "File_Name")
        toto.text = basename_out
        toto = ElementTree.SubElement(a1, "File_Description")
        toto.text = "ReferenceDemDataLevel2"
        toto = ElementTree.SubElement(a1, "Notes")
        toto.text = "Created using Start_Maja"
        toto = ElementTree.SubElement(a1, "Mission")
        toto.text = mission
        toto = ElementTree.SubElement(a1, "File_Class")
        toto.text = "TEST"
        toto = ElementTree.SubElement(a1, "File_Type")
        toto.text = "AUX_REFDE2"
        b1 = ElementTree.SubElement(a1, "Validity_Period")
        toto = ElementTree.SubElement(b1, "Validity_Start")
        toto.text = "UTC=%s" % date_start.strftime("%Y-%m-%dT%H:%M:%S")
        toto = ElementTree.SubElement(b1, "Validity_Stop")
        toto.text = "UTC=%s" % date_end.strftime("%Y-%m-%dT%H:%M:%S")
        toto = ElementTree.SubElement(a1, "File_Version")
        toto.text = str(version).zfill(4)
        b2 = ElementTree.SubElement(a1, "Source")
        toto = ElementTree.SubElement(b2, "System")
        toto.text = "MAJA"
        toto = ElementTree.SubElement(b2, "Creator")
        toto.text = "Start_Maja"
        toto = ElementTree.SubElement(b2, "Creator_Version")
        toto.text = "1.0"
        toto = ElementTree.SubElement(b2, "Creation_Date")
        toto.text = "UTC=" + creation_date.strftime("%Y-%m-%dT%H:%M:%S")

        a = ElementTree.SubElement(root, "Variable_Header")
        b2 = ElementTree.SubElement(a, "Main_Product_Header")
        ElementTree.SubElement(b2, "List_of_Consumers", count="0")
        ElementTree.SubElement(b2, "List_of_Extensions", count="0")
        b3 = ElementTree.SubElement(a, "Specific_Product_Header")
        b4 = ElementTree.SubElement(b3, "Instance_Id")
        ElementTree.SubElement(b4, "Applicable_Site_Nick_Name").text = dem_info.name
        ElementTree.SubElement(b4, "File_Version").text = str(version).zfill(4)
        b5 = ElementTree.SubElement(b3, "List_of_Applicable_SiteDefinition_Ids", count="1")
        site_defintion_base = "_".join(basename_out.split("_")[:2])  # Get e.g. 'VE_TEST' or 'S2__TEST'
        site_definiton_name = site_defintion_base + "_MPL_SITDEF_S_" + dem_info.name
        ElementTree.SubElement(b5, "Applicable_SiteDefinition_Id", sn="1").text = site_definiton_name
        b6 = ElementTree.SubElement(b3, "DEM_Information")
        c1 = ElementTree.SubElement(b6, "Cartographic")
        d1 = ElementTree.SubElement(c1, "Coordinate_Reference_System")
        ElementTree.SubElement(d1, "Code").text = str(dem_info.epsg)
        ElementTree.SubElement(d1, "Short_Description").text = str(dem_info.short_description)
        d2 = ElementTree.SubElement(c1, "Upper_Left_Corner")
        ElementTree.SubElement(d2, "X", unit="m").text = str(dem_info.ulx)
        ElementTree.SubElement(d2, "Y", unit="m").text = str(dem_info.uly)
        d3 = ElementTree.SubElement(c1, "Sampling_Interval")
        ElementTree.SubElement(d3, "By_Line", unit="m").text = str(dem_info.resx)
        ElementTree.SubElement(d3, "By_Column", unit="m").text = str(dem_info.resy)
        d4 = ElementTree.SubElement(c1, "Size")
        ElementTree.SubElement(d4, "Lines").text = str(dem_info.ly)
        ElementTree.SubElement(d4, "Columns").text = str(dem_info.lx)
        ElementTree.SubElement(b6, "Mean_Altitude_Over_L2_Coverage", unit="m").text = str(dem_info.mean_alt)
        ElementTree.SubElement(b6, "Altitude_Standard_Deviation_Over_L2_Coverage",
                               unit="m").text = str(dem_info.std_dev_alt)
        ElementTree.SubElement(b6, "Nodata_Value").text = str(int(dem_info.nodata))
        ElementTree.SubElement(b6, "L2_To_DEM_Subsampling_Ratio").text = str(dem_info.dem_subsampling_ratio)
        ElementTree.SubElement(b6, "Comment").text = "No comment"

        b4 = ElementTree.SubElement(b3, "DBL_Organization")
        b5 = ElementTree.SubElement(b4, "List_of_Packaged_DBL_Files", count=str(len(rel_files)))
        for index, dem_file in enumerate(rel_files):
            b6 = ElementTree.SubElement(b5, "Packaged_DBL_File", sn=str(index+1))
            b7 = ElementTree.SubElement(b6, "Relative_File_Path")
            b7.text = dem_file


if __name__ == "__main__":
    pass
