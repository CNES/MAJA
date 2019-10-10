#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES, CS-SI, CESBIO - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>,
                Olivier HAGOLLE (CESBIO)
Project:        Start-MAJA, CNES
"""

import os
import sys
import re
import logging
from os import path as p
from datetime import timedelta

from Chain import Product
from Common import FileSystem
from Chain import AuxFile, GippFile


class StartMaja(object):
    """
    Run the MAJA processor
    """
    version = "4.0.1rc1"
    date_regex = r"\d{4}-\d{2}-\d{2}"  # YYYY-MM-DD
    current_dir = p.dirname(p.realpath(__file__))
    max_product_difference = timedelta(hours=6)
    max_l2_diff = timedelta(days=14)

    def __init__(self, folder, tile, site, start, end, verbose=False, **kwargs):
        """
        Init the instance using the old start_maja parameters
        Optional params: nbackward, overwrite, verbose, cams, skip_confirm, fixed_platform
        """
        from Common import DateConverter
        self.verbose = verbose
        logging_level = logging.DEBUG if self.verbose else logging.INFO
        self.logger = self.__init_loggers(msg_level=logging_level)
        self.logger.info("=============This is Start_Maja v%s==============" % self.version)
        self.userconf = p.realpath(p.join(self.current_dir, "userconf"))
        if not p.isdir(self.userconf):
            raise OSError("Cannot find userconf folder: %s" % self.userconf)
        self.folder = p.realpath(folder)
        self.logger.debug("Checking config file: %s" % folder)
        if not p.isfile(self.folder):
            raise OSError("Cannot find folder definition file: %s" % self.folder)
        self.rep_work, self.gipp_root, self.rep_l1, self.rep_l2,\
            self.rep_mnt, self.maja, self.rep_cams = self.parse_config(self.folder)
        self.logger.debug("Config file parsed without errors.")

        self.logger.debug("Setting site and tile")
        if tile[0] == "T" and re.search(Product.MajaProduct.reg_tile, tile):
            self.tile = tile[1:]  # Remove the T from e.g. T32ABC
        else:
            self.tile = tile

        self.site = site
        self.path_input_l1, self.path_input_l2, self.__site_info = self.__set_input_paths()
        self.logger.debug("Found %s" % self.__site_info)

        self.avail_input_l1, self.avail_input_l2 = self.__get_all_availables_products()

        if not kwargs.get("platform"):
            platform = list(set([prod.platform for prod in self.avail_input_l1 + self.avail_input_l2]))
            if len(platform) != 1:
                raise IOError("Cannot mix multiple platforms: %s" % platform)
            self.platform = platform[0]
        else:
            assert kwargs.get("platform") in ["sentinel2", "landsat8", "venus"]
            self.platform = kwargs.get("platform")

        ptype = list(set([prod.type for prod in self.avail_input_l1 + self.avail_input_l2]))
        if len(ptype) != 1:
            if sorted(ptype) == ["muscate", "natif"]:
                # This is allowed.
                pass
            else:
                raise IOError("Cannot mix multiple plugin types: %s" % ptype)
        if ptype[0] == "natif" and self.platform == "sentinel2":
            self.ptype = "tm"
        else:
            self.ptype = ptype[0]

        # Parse product dates
        if start:
            if re.search(self.date_regex, start):
                self.start = DateConverter.stringToDatetime(start.replace("-", ""))
            else:
                raise ValueError("Unknown date encountered: %s" % start)
        else:
            dates = sorted([prod.date for prod in self.avail_input_l1])
            self.start = dates[0]

        if end:
            if re.search(self.date_regex, end):
                self.end = DateConverter.stringToDatetime(end.replace("-", ""))
            else:
                raise ValueError("Unknown date encountered: %s" % end)
        else:
            dates = sorted([prod.date for prod in self.avail_input_l1])
            self.end = dates[-1]

        if self.start > self.end:
            raise ValueError("Start date has to be before the end date!")

        self.nbackward = kwargs.get("nbackward", int(8))

        self.logger.debug("Checking GIPP files")
        self.use_cams = kwargs.get("cams", False)
        if not p.isdir(self.gipp_root):
            raise OSError("Cannot find GIPP folder: %s" % self.gipp_root)
        self.logger.debug("Found GIPP folder: %s" % self.gipp_root)

        self.gipp = GippFile.GippSet(self.gipp_root, self.platform, self.ptype, cams=self.use_cams)
        self.logger.debug("Prepared GIPP for %s %s" % (self.platform, self.ptype))

        # Other parameters:

        self.overwrite = kwargs.get("overwrite", False)
        self.maja_log_level = "PROGRESS" if not self.verbose else "DEBUG"
        self.skip_confirm = kwargs.get("skip_confirm", False)

        self.logger.debug("Searching for DTM")
        try:
            self.dtm = self.get_dtm()
        except OSError:
            self.dtm = None
            self.logger.debug("Cannot find DTM. Will attempt to download it...")
        else:
            self.logger.debug("Found DTM: %s" % self.dtm.hdr)

        self.cams_files = []
        if self.rep_cams:
            self.logger.debug("Searching for CAMS")
            self.cams_files = self.get_cams_files()
            self.logger.debug("...found %s CAMS files" % len(self.cams_files))
        return

    @staticmethod
    def __init_loggers(msg_level=logging.DEBUG):
        """
        Init a stdout logger
        :param msg_level: Standard msgLevel for both loggers. Default is DEBUG
        """

        logging.getLogger().addHandler(logging.NullHandler())
        # Create default path or get the pathname without the extension, if there is one
        logger = logging.getLogger("root")
        logger.handlers = []  # Remove the standard handler again - Bug in logging module

        logger.setLevel(msg_level)
        formatter = logging.Formatter("%(asctime)s [%(levelname)-5.5s] %(message)s")

        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setFormatter(formatter)
        logger.addHandler(console_handler)
        return logger

    @staticmethod
    def __read_config_param(config, section, option):
        """
        Read a config parameter (a path) and check whether it exists
        :param config: The parsed config file
        :param section: The section to be searched for, e.g. PATH
        :param option: The parameter name
        :return: The path read as string or OSError if not
        """

        param = os.path.realpath(os.path.expanduser(config.get(section, option)))
        if not p.exists(param):
            raise OSError("%s %s is missing: %s" % (section, option, param))
        return param

    def parse_config(self, cfg_file):
        """
        Read contents of the config/folders.txt file containing:
        Required params:
            repWork, repL1, repL2, repMNT, exeMaja
        Optional params:
            repCAMS
        :param cfg_file: The path to the file
        :return: The parsed paths for each of the directories. None for the optional ones if not given.
        """
        from Common.FileSystem import create_directory
        import configparser as cfg
        # Parsing configuration file
        config = cfg.ConfigParser()
        config.read(cfg_file)
    
        # get cfg parameters
        rep_work = os.path.realpath(os.path.expanduser(config.get("PATH", "repWork")))
        if not p.isdir(rep_work):
            create_directory(rep_work)
        rep_gipp = os.path.realpath(os.path.expanduser(config.get("PATH", "repGipp")))
        if not p.isdir(rep_gipp):
            create_directory(rep_gipp)
        rep_l1 = self.__read_config_param(config, "PATH", "repL1")
        rep_l2 = os.path.realpath(os.path.expanduser(config.get("PATH", "repL2")))
        if not p.isdir(rep_l2):
            create_directory(rep_l2)
        rep_mnt = os.path.realpath(os.path.expanduser(config.get("PATH", "repMNT")))
        if not p.isdir(rep_mnt):
            create_directory(rep_mnt)
        exe_maja = self.__read_config_param(config, "PATH", "exeMaja")

        # CAMS is optional:
        try:
            rep_cams = os.path.realpath(os.path.expanduser(config.get("PATH", "repCAMS")))
        except cfg.NoOptionError:
            self.logger.warning("repCAMS is missing. Processing without CAMS")
            rep_cams = None
        
        return rep_work, rep_gipp, rep_l1, rep_l2, rep_mnt, exe_maja, rep_cams

    def __set_input_paths(self):
        """
        Set the available L1 and L2 input paths.
        :return: The L1-, L2- folders (creating the latter if needed).
                 An info-string whether site and/or tile is used.
        """
        if self.site:
            site_l1 = FileSystem.find_single(r"^%s$" % self.site, self.rep_l1)
            self.logger.warning(self.tile)
            path_input_l1 = FileSystem.find_single(r"^T?%s$" % self.tile, site_l1)
            try:
                site_l2 = FileSystem.find_single(r"^%s$" % self.site, self.rep_l2)
            except ValueError:
                site_l2 = os.path.join(self.rep_l2, self.site)
                FileSystem.create_directory(site_l2)

            try:
                path_input_l2 = FileSystem.find_single(r"^T?%s$" % self.tile, site_l2)
            except ValueError:
                path_input_l2 = os.path.join(self.rep_l2, self.site, self.tile)
                FileSystem.create_directory(path_input_l2)
            site_info = "site %s and tile %s" % (self.site, self.tile)
        else:
            path_input_l1 = FileSystem.find_single(r"^T?%s$" % self.tile, self.rep_l1)
            try:
                path_input_l2 = FileSystem.find_single(r"^T?%s$" % self.tile, self.rep_l2)
            except ValueError:
                path_input_l2 = os.path.join(self.rep_l2, self.tile)
                FileSystem.create_directory(path_input_l2)
            site_info = "tile %s" % self.tile

        return path_input_l1, path_input_l2, site_info

    def __get_all_availables_products(self):
        """
        Set the input folders for L1- and L2- products
        :return: The available L1 and L2 products in the given folders.
        """

        if not p.isdir(self.path_input_l1):
            raise OSError("L1 folder for %s not existing: %s" % (self.__site_info, self.path_input_l1))

        if not p.isdir(self.path_input_l2):
            self.logger.warning("L2 folder for %s not existing: %s" % (self.__site_info, self.path_input_l1))

        avail_input_l1 = sorted(self.get_available_products(self.path_input_l1, level="L1C", tile=self.tile))

        if not avail_input_l1:
            raise IOError("No L1C products detected for %s in %s" % (self.__site_info, self.path_input_l1))
        else:
            self.logger.info("%s L1C product(s) detected for %s in %s" % (len(avail_input_l1),
                                                                          self.__site_info,
                                                                          self.path_input_l1))
        avail_input_l2 = sorted(self.get_available_products(self.path_input_l2, level="L2A", tile=self.tile))
        if not avail_input_l2:
            self.logger.warning("No L2A products detected for %s in %s" % (self.__site_info, self.path_input_l2))
        else:
            self.logger.info("%s L2A product(s) detected for %s in %s" % (len(avail_input_l2),
                                                                          self.__site_info,
                                                                          self.path_input_l2))
        return avail_input_l1, avail_input_l2

    @staticmethod
    def get_available_products(root, level, tile):
        """
        Parse the products from the constructed L1- or L2- directories
        :param root: The root folder to be searched from
        :param level: The product level to be search for
        :param tile: The tileID
        :return: A list of MajaProducts available in the given directory
        """
        avail_folders = [p.join(root, f) for f in os.listdir(root)]
        avail_products = [Product.MajaProduct(f).factory() for f in avail_folders if p.isdir(f)]
        # Remove the ones that didn't work:
        avail_products = [prod for prod in avail_products if prod is not None]
        return [prod for prod in avail_products if prod.level == level.lower() and prod.tile == tile]

    def get_dtm(self):
        """
        Find DTM folder for tile and search for associated HDR and DBL files
        A DTM folder has the following naming structure:
            *_AUX_REFDE2_TILEID_*DBL.DIR with TILEID e.g. T31TCH, KHUMBU ...
        A single .HDR file and an associated .DBL.DIR file
        has to be found. OSError is thrown otherwise.
        :return: The full path to the hdr and dbl.dir. Throws OSError if they're not found.
        """
        from Common import FileSystem
        regex = AuxFile.DTMFile.get_specifiable_regex() + r"T?" + self.tile + r"\w+.DBL.DIR"
        try:
            mnt_folders = FileSystem.find(regex, self.rep_mnt)
        except ValueError:
            raise OSError("Cannot find DTM.")
        mnts = [AuxFile.DTMFile(mnt) for mnt in mnt_folders]
        mnts = [mnt for mnt in mnts if mnt is not None]
        return mnts[0]

    def get_cams_files(self):
        """
        Find all associated CAMS- HDR and DBL files
        A CAMS folder has the following naming structure:
            MMM_TEST_EXO_CAMS_YYYYMMDDThhmmss_YYYYMMDDThhmmss
            with MMM = mission (see regex tests)
        For each CAMS a single .HDR file and an associated .DBL.DIR/.DBL file
        has to be found. Otherwise it gets discarded
        """
        from Common import FileSystem
        cams_folders = FileSystem.find(AuxFile.CAMSFile.regex, self.rep_cams)
        cams = [AuxFile.CAMSFile(c) for c in cams_folders]
        cams = [c for c in cams if c is not None]
        return cams

    @staticmethod
    def filter_cams_by_product(cams_files, prod_date, delta_t=timedelta(hours=12)):
        """
        Filter out all CAMS files if they are between the given start_date and end_date
        :param cams_files: The list of cams objects
        :param prod_date: The product date
        :param delta_t: The maximum time difference a CAMS file can be apart from the product date.
        :return: The cams files available in the given time interval
        """
        cams_filtered = []
        t_min, t_max = prod_date - delta_t, prod_date + delta_t
        for cams in cams_files:
            date = cams.get_date()
            if t_min <= date <= t_max:
                cams_filtered.append(cams)
        return cams_filtered

    def create_workplans(self, max_product_difference=timedelta(hours=6), max_l2_diff=timedelta(days=14)):
        """
        Create a workplan for each Level-1 product found between the given date period
        For the first product available, check on top if an L2 product from the date
        before is present to run in NOMINAL.
        If not, check if there are at minimum n_backward products to run
        a BACKWARD processing.
        If both of those conditions are not met, a simple INIT is run and the rest
        in NOMINAL
        :param max_product_difference: Maximum time difference that the same L1 and L2 products date can be apart.
        This is necessary due to the fact that the acquisition date can vary in between platforms.
        :param max_l2_diff: Maximum time difference a separate L2 product can be apart from an L1 following it.
        :return: List of workplans to be executed
        """
        from Chain import Workplan

        # Get actually usable L1 products:
        used_prod_l1 = [prod for prod in self.avail_input_l1
                        if self.start <= prod.date <= self.end]

        if not used_prod_l1:
            raise ValueError("No products available for the given start and end dates: %s -> %s"
                             % (self.start, self.end))

        # Get L1 products that already have an L2 product available using a timedelta:
        has_l2 = []
        for prod_l1 in used_prod_l1:
            for prod_l2 in self.avail_input_l2:
                if abs(prod_l1.date - prod_l2.date) < max_product_difference:
                    has_l2.append(prod_l1)
        # Setup workplans:
        workplans = []
        # Process the first product separately:
        if used_prod_l1[0] not in has_l2 or self.overwrite:
            # Check if there is a recent L2 available for a nominal workplan
            min_time = used_prod_l1[0].date - max_l2_diff
            max_time = used_prod_l1[0].date
            has_closest_l2_prod = [prod for prod in self.avail_input_l2 if min_time <= prod.date <= max_time]
            if has_closest_l2_prod:
                # Proceed with NOMINAL
                workplans.append(Workplan.Nominal(wdir=self.rep_work,
                                                  outdir=self.path_input_l2,
                                                  l1=used_prod_l1[0],
                                                  l2_date=used_prod_l1[0].date,
                                                  log_level=self.maja_log_level,
                                                  cams=self.filter_cams_by_product(self.cams_files,
                                                                                   used_prod_l1[0].date)
                                                  ))
                pass
            else:
                if len(self.avail_input_l1) >= self.nbackward:
                    # Proceed with BACKWARD
                    index_current_prod = self.avail_input_l1.index(used_prod_l1[0])
                    workplans.append(Workplan.Backward(wdir=self.rep_work,
                                                       outdir=self.path_input_l2,
                                                       l1=used_prod_l1[0],
                                                       l1_list=self.avail_input_l1[index_current_prod:self.nbackward],
                                                       log_level=self.maja_log_level,
                                                       cams=self.filter_cams_by_product(self.cams_files,
                                                                                        used_prod_l1[0].date)
                                                       ))
                    pass
                else:
                    # Proceed with INIT
                    self.logger.info("Not enough L1 products available for a BACKWARD mode. Beginning with INIT...")
                    workplans.append(Workplan.Init(wdir=self.rep_work,
                                                   outdir=self.path_input_l2,
                                                   l1=used_prod_l1[0],
                                                   log_level=self.maja_log_level,
                                                   cams=self.filter_cams_by_product(self.cams_files,
                                                                                    used_prod_l1[0].date)
                                                   ))
                    pass
                pass
        else:
            self.logger.info("Skipping L1 product %s because it was already processed!" % used_prod_l1[0].base)

        # For the rest: Setup NOMINAL.
        # Except: The time series is 'stopped' - The gap between two products is too large.
        # In this case, proceed with a re-init.
        for i, prod in enumerate(used_prod_l1[1:]):
            if prod in has_l2 or self.overwrite:
                self.logger.info("Skipping L1 product %s because it was already processed!" % prod.base)
                continue
            # Note: i, in this case is the previous product -> Not the current one, which is i+1
            date_gap = prod.date - used_prod_l1[i].date
            if date_gap >= max_l2_diff:
                self.logger.info("Will not continue time-series. Date gap too large between products %s vs. %s" %
                                 (prod.date, used_prod_l1[i].date))
                index_current_prod = self.avail_input_l1.index(prod)
                if len(self.avail_input_l1[index_current_prod:]) >= self.nbackward:
                    # Proceed with BACKWARD
                    workplans.append(Workplan.Backward(wdir=self.rep_work,
                                                       outdir=self.path_input_l2,
                                                       l1=prod,
                                                       l1_list=self.avail_input_l1[index_current_prod:self.nbackward],
                                                       log_level=self.maja_log_level,
                                                       cams=self.filter_cams_by_product(self.cams_files,
                                                                                        prod.date)
                                                       ))
                    pass
                else:
                    # Proceed with INIT
                    self.logger.info("Not enough L1 products available for a BACKWARD mode. Continuing with INIT...")
                    workplans.append(Workplan.Init(wdir=self.rep_work,
                                                   outdir=self.path_input_l2,
                                                   l1=prod,
                                                   log_level=self.maja_log_level,
                                                   cams=self.filter_cams_by_product(self.cams_files,
                                                                                    prod.date)
                                                   ))
                    pass
                pass
            else:
                workplans.append(Workplan.Nominal(wdir=self.rep_work,
                                                  outdir=self.path_input_l2,
                                                  l1=prod,
                                                  l2_date=prod.date,
                                                  log_level=self.maja_log_level,
                                                  cams=self.filter_cams_by_product(self.cams_files, prod.date),
                                                  # Fallback parameters:
                                                  remaining_l1=used_prod_l1[(i+1):],
                                                  nbackward=self.nbackward
                                                  ))

        # This should never happen:
        if not workplans:
            raise ValueError("No workplans were created!")
        
        return workplans

    def run(self):
        """
        Run the whole artillery:
            - Find all L1 and L2 products
            - Find all CAMS files
            - Filter both by start and end dates, if there are
            - Determine the Workplans and a mode for each (INIT, BACKWARD, NOMINAL)
            - For each workplan:
            -   Create the input directory and link all the needed inputs
            -   Create the output directory
            -   Run MAJA
        """
        if not self.dtm:
            self.logger.info("Attempting to download DTM...")
            self.avail_input_l1[0].get_mnt(dem_dir=self.rep_mnt)
            self.dtm = self.get_dtm()
            self.logger.info("DTM Creation succeeded.")
        if not self.gipp.check_completeness():
            self.logger.debug("Downloading Gipp for %s %s" % (self.platform, self.ptype))
            self.gipp.download()
        workplans = self.create_workplans(self.max_product_difference, self.max_l2_diff)
        self.logger.info("%s workplan(s) successfully created:" % len(workplans))
        # Print without the logging-formatting:
        print(str("%19s | %5s | %8s | %70s | %15s" % ("DATE", "TILE", "MODE", "L1-PRODUCT", "ADDITIONAL INFO")))
        for wp in workplans:
            print(wp)
        if not self.skip_confirm:
            try:
                if sys.version >= (3, 0):
                    input("Press Enter to continue...\n")
            except TypeError:
                input("Press Enter to continue...\n")
        self.logger.info("Beginning workplan execution.")
        for i, wp in enumerate(workplans):
            self.logger.info("Executing workplan #%s/%s" % (i+1, len(workplans)))
            wp.execute(self.maja, self.dtm, self.gipp, self.userconf)
        self.logger.info("=============Start_Maja v%s finished=============" % self.version)
        pass


if __name__ == "__main__":
    if not sys.version_info >= (3, 5):
        raise AssertionError("Start_maja needs python >= 3.5.\n"
                             "Run 'python --version' for more info.")

    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--tile", help="Tile number",
                        type=str, required=True)
    parser.add_argument("-s", "--site", help="Site name. If not specified,"
                                             "the tile number is used directly for finding the L1/L2 product directory",
                        type=str, required=False)
    parser.add_argument("-f", "--folder", help="Config/Folder-definition file used for all permanent settings.",
                        type=str, required=True)
    parser.add_argument("-d", "--start", help="Start date for processing in format YYYY-MM-DD. If none is provided,"
                                              "all products until the end date will be processed",
                        type=str, required=False, default="1970-01-01")
    parser.add_argument("-e", "--end", help="Start date for processing in format YYYY-MM-DD. If none is provided,"
                                            "all products from the start date onwards will be processed",
                        type=str, required=False, default="3000-01-01")
    parser.add_argument("-v", "--verbose", help="Provides detailed (DEBUG) logging for Maja. Default is false",
                        default=False, action="store_true")
    parser.add_argument("--nbackward", help="Number of products used to run in backward mode. Default is 8.",
                        type=int, default=int(8))
    parser.add_argument("--overwrite", help="Overwrite existing L2 products. Default is false.",
                        default=False, action="store_true")
    parser.add_argument("--cams", help="Use CAMS during processing."
                                       "The CAMS files have to be available in the repCAMS dir. Default is False.",
                        action="store_true", required=False, default=False)
    parser.add_argument("-y", help="Skip workplan confirmation. Default is False",
                        action="store_true", required=False, default=False)
    parser.add_argument("--version", action='version', version='%(prog)s ' + str(StartMaja.version))
    parser.add_argument("-p", "--platform", help="Manually override which platform to use. By default this is deducted"
                                                 "by the available input product(s). Options are 'sentinel2',"
                                                 "'landsat8' and 'venus'.",
                        type=str, required=False, default=None)
    # TODO Add output plugin override, gipp re-download
    # TODO Add "copy" parameter that copies files instead of symlink'n them.
    args = parser.parse_args()
    
    s = StartMaja(args.folder, args.tile, args.site,
                  args.start, args.end, nbackward=args.nbackward, verbose=args.verbose,
                  overwrite=args.overwrite, cams=args.cams,
                  skip_confirm=args.y, platform=args.platform)
    s.run()
