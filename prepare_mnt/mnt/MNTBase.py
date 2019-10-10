#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>, Pierre LASSALLE <pierre.lassalle@cnes.fr>
Project:        StartMaja, CNES
Created on:     Tue Sep 11 15:31:00 2018
"""

# GlobalSurfaceWater (GSW) base url for direct-download of the files:
surface_water_url = "https://storage.googleapis.com/global-surface-water/downloads2/occurrence/occurrence_%s_v1_1.tif"


class MNT(object):
    """
    Base class to set the DEM/MNT interfaces for a given site.
    DEM := Digital Elevation model)
    MNT := Modèle numérique de terrain; french for DEM
    """
    def __init__(self, site, **kwargs):
        import os
        import tempfile
        from osgeo import gdal
        from Common import FileSystem
        if not int(gdal.VersionInfo()) >= 2000000:
            raise ImportError("MNT creation needs Gdal >2.0!")
        self.site = site
        self.dem_dir = kwargs.get("dem_dir", os.path.join(tempfile.gettempdir(), "maja_dem_files"))
        if not os.path.isdir(self.dem_dir):
            FileSystem.create_directory(self.dem_dir)
        self.wdir = kwargs.get("wdir", None)
        if not self.wdir:
            self.wdir = tempfile.mkdtemp(prefix="prepare_mnt_")
        if not os.path.isdir(self.wdir):
            FileSystem.create_directory(self.wdir)
        self.raw_dem = kwargs.get("raw_dem", None)
        if not self.raw_dem:
            self.raw_dem = tempfile.mkdtemp(prefix="raw_dem_")
        if not os.path.exists(self.raw_dem):
            FileSystem.create_directory(self.raw_dem)
        self.raw_gsw = kwargs.get("raw_gsw", None)
        if not self.raw_gsw:
            self.raw_gsw = tempfile.mkdtemp(prefix="raw_gsw_")
        if not os.path.exists(self.raw_gsw):
            FileSystem.create_directory(self.raw_gsw)
        self.gsw_codes = self.get_gsw_codes(self.site)
        self.dem_version = kwargs.get("dem_version", 1)
        self.gsw_threshold = kwargs.get("gsw_threshold", 30.)
        self.gsw_dst = kwargs.get("gsw_dst", os.path.join(self.wdir, "surface_water_mask.tif"))

    def get_raw_data(self):
        """
        Get the DEM raw-data from a given directory. If not existing, an attempt will be made to download
        it automatically.
        :return:
        """
        raise NotImplementedError

    def prepare_mnt(self):
        raise NotImplementedError

    @staticmethod
    def calc_gradient(mnt_arr):
        import numpy as np
        from scipy import ndimage
        # TODO Find a pure numpy 2D convolution.
        kernel_horizontal = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
        kernel_vertical = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])
        dz_dc = ndimage.convolve(mnt_arr, kernel_horizontal)
        dz_dl = ndimage.convolve(mnt_arr, kernel_vertical)
        return dz_dc, dz_dl

    @staticmethod
    def calc_slope_aspect(dz_dc, dz_dl):
        import numpy as np
        norme = np.sqrt(dz_dc * dz_dc) + (dz_dl * dz_dl)
        slope = np.arctan(norme)
        aspect = np.where(dz_dc > 0, np.arccos(dz_dl / norme), 2 * np.pi - np.arccos(dz_dl / norme))
        aspect = np.where(slope == 0, 0, aspect)
        return slope, aspect

    @staticmethod
    def get_gsw_codes(site):
        """
        Get the list of GSW files for a given site.
        :param site: The site class
        :return: The list of filenames of format 'XX(E/W)_YY(N/S)' needed in order to cover to whole site.
        """
        import math
        from collections import namedtuple
        if site.ul_latlon[1] > 170 and site.lr_latlon[1] < 160:
            raise ValueError("Cannot wrap around longitude change")

        grid_step = 10
        point = namedtuple("point", ("y", "x"))
        pts = []
        for pt in [site.ul_latlon, site.lr_latlon]:
            lat_dec = (math.fabs(pt[0]) / grid_step)
            lon_dec = (math.fabs(pt[1]) / grid_step)
            if pt[0] > 0:
                lat_id = int(math.ceil(lat_dec) * grid_step)
            else:
                lat_id = -1 * int(math.floor(lat_dec) * grid_step)

            if pt[1] < 0:
                lon_id = int(math.ceil(lon_dec) * grid_step)
            else:
                lon_id = -1 * int(math.floor(lon_dec) * grid_step)
            pts.append(point(lat_id, lon_id))
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
        import os
        from Common import FileSystem
        import logging
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
        import os
        from Common import ImageIO
        occ_files = self.get_raw_water_data()
        # Fusion of all gsw files:
        fusion_path = os.path.join(self.wdir, "occurrence.tiff")
        water_mask = os.path.join(self.wdir, "water_mask_comb.tif")
        ImageIO.gdal_merge(fusion_path, *occ_files)
        # Overlay occurrence image with same extent as the given site.
        # Should the occurrence files not be complete, this sets all areas not covered by the occurrence to 0.
        ImageIO.gdal_warp(water_mask, fusion_path,
                          r="near",
                          te=self.site.te_str,
                          t_srs=self.site.epsg_str,
                          tr=self.site.tr_str,
                          dstnodata="0")
        # Threshold the final image and write to destination:
        image, drv = ImageIO.tiff_to_array(water_mask, array_only=False)
        image_bin = image > self.gsw_threshold
        ImageIO.write_geotiff_existing(image_bin, self.gsw_dst, drv)

    def to_maja_format(self, platform_id, mission_field, mnt_resolutions, coarse_res):
        import os
        import tempfile
        from datetime import datetime
        from Common import ImageIO, XMLTools, FileSystem
        from prepare_mnt.mnt.DEMInfo import DEMInfo
        assert len(mnt_resolutions) >= 1
        basename = str("%s_TEST_AUX_REFDE2_%s_%s" % (platform_id, self.site.nom, str(self.dem_version).zfill(4)))
        # Get water data
        self.prepare_water_data()
        # Get mnt data
        mnt_full_res = self.prepare_mnt()
        dbl_base = basename + ".DBL.DIR"
        dbl_dir = os.path.join(self.dem_dir, dbl_base)
        FileSystem.create_directory(dbl_dir)
        hdr = os.path.join(self.dem_dir, basename + ".HDR")

        # Read MNT and water mask at full resolution:
        mnt_in, drv = ImageIO.tiff_to_array(mnt_full_res, array_only=False)
        grad_y, grad_x = self.calc_gradient(mnt_in)
        slope, aspect = self.calc_slope_aspect(grad_y, grad_x)
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
            ImageIO.gdal_translate(path_alt, mnt_full_res, tr=res["val"])
            rasters_written.append(rel_alt)
            # ASP:
            bname_asp = basename + "_ASP"
            bname_asp += "_" + res["name"] if write_resolution_name else ""
            bname_asp += ".TIF"
            rel_asp = os.path.join(dbl_base, bname_asp)
            tmp_asp = tempfile.mktemp(dir=self.wdir)
            path_asp = os.path.join(self.dem_dir, rel_asp)
            ImageIO.write_geotiff_existing(aspect, tmp_asp, drv)
            ImageIO.gdal_translate(path_asp, tmp_asp, tr=res["val"])
            rasters_written.append(rel_asp)
            # SLP:
            bname_slp = basename + "_SLP"
            bname_slp += "_" + res["name"] if write_resolution_name else ""
            bname_slp += ".TIF"
            rel_slp = os.path.join(dbl_base, bname_slp)
            tmp_slp = tempfile.mktemp(dir=self.wdir)
            path_slp = os.path.join(self.dem_dir, rel_slp)
            ImageIO.write_geotiff_existing(aspect, tmp_slp, drv)
            ImageIO.gdal_translate(path_slp, tmp_slp, tr=res["val"])
            rasters_written.append(rel_slp)

        # Resize all rasters for coarse res.
        coarse_res_str = str(coarse_res[0]) + " " + str(coarse_res[1])
        # ALC:
        bname_alc = basename + "_ALC.TIF"
        rel_alc = os.path.join(dbl_base, bname_alc)
        path_alc = os.path.join(self.dem_dir, rel_alc)
        ImageIO.gdal_translate(path_alc, path_alt, tr=coarse_res_str)
        rasters_written.append(rel_alc)
        # ALC:
        bname_asc = basename + "_ASC.TIF"
        rel_asc = os.path.join(dbl_base, bname_asc)
        path_asc = os.path.join(self.dem_dir, rel_asc)
        ImageIO.gdal_translate(path_asc, path_asp, tr=coarse_res_str)
        rasters_written.append(rel_asc)
        # ALC:
        bname_slc = basename + "_SLC.TIF"
        rel_slc = os.path.join(dbl_base, bname_slc)
        path_slc = os.path.join(self.dem_dir, rel_slc)
        ImageIO.gdal_translate(path_slc, path_slp, tr=coarse_res_str)
        rasters_written.append(rel_slc)
        # Water mask:
        bname_msk = basename + "_MSK.TIF"
        rel_msk = os.path.join(dbl_base, bname_msk)
        path_msk = os.path.join(self.dem_dir, rel_msk)
        ImageIO.gdal_translate(path_msk, self.gsw_dst, tr=coarse_res_str)
        rasters_written.append(rel_msk)

        # Write HDR Metadata:

        date_start = datetime(1970, 1, 1)
        date_end = datetime(2100, 1, 1)
        dem_info = DEMInfo(self.site, all_paths_alt[0])
        root = self._get_root()
        self._create_hdr(root, mission_field, basename, rasters_written,
                         dem_info, date_start, date_end, self.dem_version)
        XMLTools.write_xml(root, hdr)

        return hdr, dbl_dir

    @staticmethod
    def _get_root():
        """
        Create the root of a single dem file
        :return:
        """
        from xml.etree import ElementTree
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
        from datetime import datetime
        from xml.etree import ElementTree
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
        # TODO Correct this:
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
