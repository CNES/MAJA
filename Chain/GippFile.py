#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES, CS-SI, CESBIO - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>
Project:        Start-MAJA, CNES
"""

import os
import logging
from Chain.AuxFile import EarthExplorer


class GIPPFile(EarthExplorer):
    regex = r"\w+_" \
            r"(TEST|PROD)_" \
            r"GIP_" \
            r"(L2ALBD|L2DIFT|L2DIRT|L2TOCR|L2WATV|L2COMM|L2SITE|L2SMAC|CKEXTL|CKQLTL)_" \
            r"\w_(\w+)_\d{5}_\d{8}_\d{8}\.(HDR|EEF)"

    regex_dbl = r"\w+_" \
                r"(TEST|PROD)_" \
                r"GIP_" \
                r"(L2ALBD|L2DIFT|L2DIRT|L2TOCR|L2WATV|L2COMM|L2SITE|L2SMAC|CKEXTL|CKQLTL)_" \
                r"\w_(\w+)_\d{5}_\d{8}_\d{8}\.DBL(.DIR)?"

    @staticmethod
    def get_mission(hdr):
        """
        Return the "Mission" field for a single GIPP file.
        :param hdr: The full path to the HDR file
        :return:
        """
        from Common import FileSystem
        return FileSystem.get_single_xpath(hdr, "./Fixed_Header/Mission")


class GippALBD(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_L2ALBD_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippDIFT(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_L2DIFT_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippDIRT(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_L2DIRT_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippTOCR(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_L2TOCR_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippWATV(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_L2WATV_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippCOMM(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_L2COMM_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippSITE(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_L2SITE_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippSMAC(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_L2SMAC_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippEXTL(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_CKEXTL_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippQLTL(EarthExplorer):
    regex = r"\w+_(TEST|PROD)_GIP_CKQLTL_\w_(\w+)_\d{5}_\d{8}_\d{8}\.\w+"


class GippSet(object):
    """
    Stores a set of Gipp Files
    """
    url = "http://tully.ups-tlse.fr/olivier/gipp_maja/repository/archive.zip?ref=master"
    zenodo_reg = r"https?:\/\/zenodo.org\/record\/\d+\/files\/\w+.zip\?download=1"

    platforms = ["sentinel2", "landsat8", "venus"]
    plugins = {"sentinel2": ["natif", "muscate", "tm"],
               "landsat8": ["natif", "muscate", "tm"],
               "venus": ["natif", "muscate", "tm"]}

    def __init__(self, root, platform, gtype, cams=False, log_level=logging.INFO):
        """
        Set the path to the root gipp folder
        :param root: The full path to the root gipp folder
        :param platform: The platform name. Has to be in ["sentinel2", "landsat8", "venus"]
        :param gtype: The gipp type. Has to be in ["muscate", "natif", "tm"]
        :param cams: Build GIPP with CAMS models
        :param log_level: The log level for the messages displayed.
        """
        from Common import FileSystem
        self.fpath = os.path.realpath(root)
        if platform not in self.platforms:
            raise ValueError("Unknown platform found: %s" % platform)
        if gtype not in self.plugins[platform]:
            raise ValueError("No Plugin of type %s existing for platform %s" % (gtype, platform))
        if gtype == "tm" and platform in ["landsat8", "venus"]:
            self.gtype = "natif"
        else:
            self.gtype = gtype
        self.platform = platform
        self.cams_suffix = "_CAMS" if cams else ""
        self.log_level = log_level
        self.n_sat = 2 if platform == "sentinel2" else 1
        # Create root if not existing:
        FileSystem.create_directory(self.fpath)

        # Create folder names:
        self.gipp_archive = os.path.join(self.fpath, "archive.zip")
        self.lut_archive = os.path.join(self.fpath, "lut_archive.zip")
        self.temp_folder = os.path.join(self.fpath, "tempdir")
        self.gipp_folder_name = "%s_%s" % (self.platform.upper(), self.gtype.upper()) + self.cams_suffix
        self.out_path = os.path.join(self.fpath, self.gipp_folder_name)

    def __clean_up(self):
        """
        Clean up the download directory.
        :return:
        """
        from Common import FileSystem
        FileSystem.remove_directory(self.temp_folder)
        FileSystem.remove_file(self.lut_archive)
        FileSystem.remove_file(self.gipp_archive)

    def download(self):
        """
        Download a specific set of Gipps to the given folder.
        First, attempt to download the most recent git archive containing the .EEF files as well as the
        url to download the LUTs. Then, the latter will be downloaded separately.
        :return:
        """
        import shutil
        from Common import FileSystem
        self.out_path = os.path.join(self.fpath, self.gipp_folder_name)
        FileSystem.download_file(self.url, self.gipp_archive, self.log_level)
        FileSystem.unzip(self.gipp_archive, self.temp_folder)
        gipp_maja_git = os.path.join(self.temp_folder, "gipp_maja.git")
        platform_folder = FileSystem.get_file(root=gipp_maja_git, filename= "^" + self.gipp_folder_name + "$")
        if not platform_folder:
            self.__clean_up()
            raise OSError("Cannot find any gipp folder for platform %s" % self.gipp_folder_name)
        readme = FileSystem.get_file(filename="readme*", root=platform_folder)
        if not readme:
            self.__clean_up()
            raise OSError("Cannot find download-file for LUT-Download in %s" % platform_folder)
        lut_url = FileSystem.find_in_file(readme, self.zenodo_reg)
        if not lut_url:
            self.__clean_up()
            raise OSError("Cannot find url to download LUTs")
        FileSystem.download_file(lut_url, self.lut_archive, self.log_level)
        FileSystem.unzip(self.lut_archive, platform_folder)
        lut_folder = FileSystem.get_file(root=platform_folder, filename="LUTs")
        if not lut_folder:
            self.__clean_up()
            raise OSError("Cannot find 'LUTs' folder in %s" % self.temp_folder)
        for f in os.listdir(lut_folder):
            shutil.move(os.path.join(lut_folder, f), platform_folder)
        if os.path.isdir(self.out_path):
            FileSystem.remove_directory(self.out_path)
        shutil.move(platform_folder, self.out_path)
        FileSystem.remove_directory(lut_folder)
        self.__clean_up()

    def link(self, dest):
        """
        Symlink a set of Gipps to a given destination
        :param dest: The destination directory
        :return:
        """
        from Common import FileSystem
        eefs = FileSystem.find(GIPPFile.regex, self.out_path)
        dbls = FileSystem.find(GIPPFile.regex_dbl, self.out_path)
        for f in eefs + dbls:
            base = os.path.basename(f)
            FileSystem.symlink(f, os.path.join(dest, base))

    def get_models(self):
        """
        Get the list of models present in the gipp-set.
        :return: List of models in alphabetical order.
        """
        from Common import FileSystem
        import re
        hdr_reg = os.path.splitext(GIPPFile.regex)[0] + ".HDR"
        hdrs = FileSystem.find(hdr_reg, self.out_path)
        raw_models = [re.search(hdr_reg, h).group(3).replace("_", "").lower() for h in hdrs]
        models = list(set(raw_models))
        return sorted(models)

    def check_completeness(self):
        """
        Check if the Gipp-folder exists already
        :return: True if existing. False if not.
        """
        from Common import FileSystem
        n_files_per_model = 5
        expected_n_models = [6, 8] if self.cams_suffix else [1]
        try:
            found_n_models = len(self.get_models())
        except ValueError:
            return False
        if not os.path.isdir(self.out_path):
            return False
        if found_n_models not in expected_n_models:
            return False
        try:
            hdrs = FileSystem.find("*.HDR", self.out_path)
            dbls = FileSystem.find("*.DBL.DIR", self.out_path)
            eefs = FileSystem.find("*.EEF", self.out_path)
        except ValueError:
            return False
        if len(eefs) < 4:
            return False
        # Models = 4 (TOCR, DIRT, DIFT, ALBD) + 1 constant for WATV per satellite
        if len(hdrs) != len(dbls) != n_files_per_model * self.n_sat * (found_n_models - 1) + 1 * self.n_sat:
            return False

        return True
