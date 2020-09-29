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
from datetime import timedelta, datetime
from StartMaja.Common import FileSystem
from StartMaja.Chain import AuxFile, GippFile, Product
from StartMaja.Chain.Workplan import Workplan, Nominal, Backward, Init


class StartMaja(object):
    """
    Run the MAJA processor
    """
    version = "4.2.0"
    current_dir = p.dirname(p.realpath(__file__))
    max_product_difference = timedelta(hours=6)

    def __init__(self, folder, tile, site, start, end, **kwargs):
        """
        Init the instance using the old start_maja parameters

        :keyword nbackward: Number of products used to run in backward mode. Default is 8.
        :keyword logger: Global ``logging.logger`` instance
        :keyword overwrite: Overwrite existing L2 products. Default is false.
        :keyword cams: Use CAMS during processing. The CAMS files have to be available in the repCAMS dir.
        :keyword skip_confirm: Skip workplan confirmation. Default is False.
        :keyword platform: Manually override which platform to use.
                           By default this is deducted by the available input product(s)
        :keyword type_dem: DEM type. If none is given, any will be used
        """

        self.logger = kwargs.get("logger", logging.getLogger("root"))
        self.logger.info("=============This is Start_Maja v%s==============" % self.version)
        self.userconf = p.realpath(p.join(self.current_dir, "userconf"))
        if not p.isdir(self.userconf):
            raise OSError("Cannot find userconf folder: %s" % self.userconf)
        self.folder = p.realpath(folder)
        self.logger.debug("Checking config file: %s" % folder)
        if not p.isfile(self.folder):
            raise OSError("Cannot find folder definition file: %s" % self.folder)
        self.rep_work, self.gipp_root, self.rep_l1, self.rep_l2, self.maja, self.rep_cams, \
            self.rep_mnt, self.rep_raw, self.rep_gsw = self.parse_config(self.folder)
        self.logger.debug("Config file parsed without errors.")

        self.logger.debug("Setting site and tile")
        if tile[0] == "T" and re.search(Product.MajaProduct.reg_tile, tile):
            self.tile = tile[1:]  # Remove the T from e.g. T32ABC
        else:
            self.tile = tile

        self.site = site
        self.path_input_l1, self.path_input_l2, self.__site_info = self.__set_input_paths()
        self.logger.debug("Found %s" % self.__site_info)
        self.logger.info("Detecting input products...")
        self.avail_input_l1, self.avail_input_l2 = self.get_all_available_products()

        if not kwargs.get("platform"):
            platform = list(set([prod.platform for prod in self.avail_input_l1 + self.avail_input_l2]))
            if len(platform) != 1:
                raise IOError("Cannot mix multiple platforms: %s."
                              "Please set parameter --platform to specify." % platform)
            self.platform = platform[0]
        else:
            self.platform = kwargs.get("platform")

        ptype = list(set([prod.type for prod in self.avail_input_l1 + self.avail_input_l2]))
        if len(ptype) != 1:
            if sorted(ptype) == ["muscate", "natif"]:
                # This is allowed. Maja4 doesn't need this, it is kept only for Maja3.
                self.ptype = "tm"
                pass
            else:
                raise IOError("Cannot mix multiple plugin types: %s" % ptype)
        else:
            self.ptype = ptype[0]

        # Parse product dates
        if start:
            self.start = datetime.strptime(start, "%Y-%m-%d")
        else:
            dates = sorted([prod.date for prod in self.avail_input_l1])
            self.start = dates[0]

        if end:
            self.end = datetime.strptime(end, "%Y-%m-%d")
        else:
            dates = sorted([prod.date for prod in self.avail_input_l1])
            self.end = dates[-1]

        if self.start > self.end:
            raise ValueError("Start date has to be before the end date: %s -> %s" % (self.start, self.end))

        self.nbackward = kwargs.get("nbackward", int(8))
        self.use_cams = kwargs.get("cams", False)

        self.cams_files = []
        if self.rep_cams and self.use_cams:
            self.logger.info("Searching for CAMS")
            self.cams_files = self.get_cams_files()
            self.logger.info("...found %s CAMS files" % len(self.cams_files))
        else:
            self.logger.info("Skipping CAMS file detection.")
        self.logger.info("Checking GIPP files")
        if not p.isdir(self.gipp_root):
            raise OSError("Cannot find GIPP folder: %s" % self.gipp_root)
        self.logger.info("Setting up GIPP folder: %s" % self.gipp_root)
        self.gipp = GippFile.GippSet(self.gipp_root, self.platform, self.ptype, cams=self.use_cams)
        if not self.gipp.check_completeness():
            self.logger.info("Cannot find GIPP for %s. Will attempt to download it." % self.gipp.gipp_folder_name)

        # Other parameters:
        self.overwrite = kwargs.get("overwrite", False)
        self.skip_error = kwargs.get("skip_errors", False)
        self.maja_log_level = "DEBUG" if self.logger.level == logging.DEBUG else "PROGRESS"
        self.skip_confirm = kwargs.get("skip_confirm", False)

        self.logger.info("Searching for DTM")
        self.type_dem = kwargs.get("type_dem", "any")
        self.dtm = self.get_dtm(type_dem=self.type_dem)
        if not self.dtm:
            self.logger.info("Cannot find DTM. Will attempt to download it for type '%s'" % self.type_dem)
        else:
            self.logger.info("Found DTM: %s" % self.dtm.hdr)

        return

    @staticmethod
    def init_loggers(msg_level=logging.DEBUG):
        """
        Init a stdout logger
        :param msg_level: Standard msgLevel for both loggers. Default is DEBUG
        """

        logging.getLogger().addHandler(logging.NullHandler())
        # Create default path or get the pathname without the extension, if there is one
        startmaja_logger = logging.getLogger("root")
        startmaja_logger.handlers = []  # Remove the standard handler again - Bug in logging module

        startmaja_logger.setLevel(msg_level)
        formatter = logging.Formatter("%(asctime)s [%(levelname)-5.5s] %(message)s")

        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setFormatter(formatter)
        startmaja_logger.addHandler(console_handler)
        return startmaja_logger

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
        import configparser as cfg
        # Parsing configuration file
        config = cfg.ConfigParser()
        config.read(cfg_file)
    
        # Maja_Inputs
        rep_work = os.path.realpath(os.path.expanduser(config.get("Maja_Inputs", "repWork")))
        if not p.isdir(rep_work):
            FileSystem.create_directory(rep_work)
        rep_gipp = os.path.realpath(os.path.expanduser(config.get("Maja_Inputs", "repGipp")))
        if not p.isdir(rep_gipp):
            FileSystem.create_directory(rep_gipp)
        rep_l1 = StartMaja.__read_config_param(config, "Maja_Inputs", "repL1")
        rep_l2 = os.path.realpath(os.path.expanduser(config.get("Maja_Inputs", "repL2")))
        if not p.isdir(rep_l2):
            FileSystem.create_directory(rep_l2)
        exe_maja = StartMaja.__read_config_param(config, "Maja_Inputs", "exeMaja")
        rep_mnt = os.path.realpath(os.path.expanduser(config.get("Maja_Inputs", "repMNT")))
        if not p.isdir(rep_mnt):
            FileSystem.create_directory(rep_mnt)
        # CAMS is optional:
        try:
            rep_cams = os.path.realpath(os.path.expanduser(config.get("Maja_Inputs", "repCAMS")))
        except cfg.NoOptionError:
            self.logger.warning("repCAMS is missing. Processing without CAMS")
            rep_cams = None

        # DTM_Creation
        rep_raw = os.path.realpath(os.path.expanduser(config.get("DTM_Creation", "repRAW")))
        if not p.isdir(rep_raw):
            FileSystem.create_directory(rep_raw)
        rep_gsw = os.path.realpath(os.path.expanduser(config.get("DTM_Creation", "repGSW")))
        if not p.isdir(rep_gsw):
            FileSystem.create_directory(rep_gsw)
        return rep_work, rep_gipp, rep_l1, rep_l2, exe_maja, rep_cams, rep_mnt, rep_raw, rep_gsw

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

    def get_all_available_products(self):
        """
        Set the input folders for L1- and L2- products
        :return: The available L1 and L2 products in the given folders.
        """

        if not p.isdir(self.path_input_l1):
            raise OSError("L1 folder for %s not existing: %s" % (self.__site_info, self.path_input_l1))

        if not p.isdir(self.path_input_l2):
            self.logger.warning("L2 folder for %s not existing: %s" % (self.__site_info, self.path_input_l2))

        avail_input_l1 = sorted(Workplan.get_available_products(self.path_input_l1, level="L1C", tile=self.tile))

        if not avail_input_l1:
            raise IOError("No L1C products detected for %s in %s" % (self.__site_info, self.path_input_l1))
        else:
            self.logger.info("%s L1C product(s) detected for %s in %s" % (len(avail_input_l1),
                                                                          self.__site_info,
                                                                          self.path_input_l1))
        avail_input_l2 = sorted(Workplan.get_available_products(self.path_input_l2, level="L2A", tile=self.tile))
        if not avail_input_l2:
            self.logger.warning("No L2A products detected for %s in %s" % (self.__site_info, self.path_input_l2))
        else:
            self.logger.info("%s L2A product(s) detected for %s in %s" % (len(avail_input_l2),
                                                                          self.__site_info,
                                                                          self.path_input_l2))
        return avail_input_l1, avail_input_l2

    def get_dtm(self, type_dem):
        """
        Find DTM folder for tile and search for associated HDR and DBL files
        A DTM folder has the following naming structure:
            *_AUX_REFDE2_TILEID_*DBL.DIR with TILEID e.g. T31TCH, KHUMBU ...
        A single .HDR file and an associated .DBL.DIR file
        has to be found. OSError is thrown otherwise.
        :param type_dem: The DEM-type as str, e.g. 'srtm'
        :return: The full path to the hdr and dbl.dir. None if they're not found.
        """

        regexes = ["%s%s_%s.DBL(.DIR)?$" % (AuxFile.DTMFile.get_specifiable_regex(), self.tile, nbr)
                   for nbr in AuxFile.DTMFile.mnt_version[type_dem]]
        mnt_folders = []
        for regex in regexes:
            try:
                mnt_folders += FileSystem.find(regex, self.rep_mnt)
            except ValueError:
                pass
        if not mnt_folders:
            return None
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
        try:
            cams_folders = FileSystem.find(AuxFile.CAMSFile.regex, self.rep_cams)
        except ValueError:
            return []
        cams = [AuxFile.CAMSFile(c) for c in cams_folders]
        cams = [c for c in cams if c is not None]
        return cams

    def create_workplans(self, max_product_difference):
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
        :return: List of workplans to be executed
        """
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
            min_time = used_prod_l1[0].date - used_prod_l1[0].max_l2_diff
            max_time = used_prod_l1[0].date
            # Filter closest l2 prod so the product itself is not included (in case of overwriting):
            if self.overwrite:
                has_closest_l2_prod = [prod for prod in self.avail_input_l2 if min_time <= prod.date <= max_time
                                       and prod.date.strftime("%Y%m%d") != used_prod_l1[0].date.strftime("%Y%m%d")]
            else:
                has_closest_l2_prod = [prod for prod in self.avail_input_l2 if min_time <= prod.date <= max_time]
            if has_closest_l2_prod:
                # Proceed with NOMINAL
                workplans.append(Nominal(wdir=self.rep_work,
                                         outdir=self.path_input_l2,
                                         l1=used_prod_l1[0],
                                         l2_date=used_prod_l1[0].date,
                                         log_level=self.maja_log_level,
                                         skip_errors=self.skip_error,
                                         cams=Workplan.filter_cams_by_products(self.cams_files,
                                                                               [used_prod_l1[0].date])
                                         ))
                pass
            else:
                if len(self.avail_input_l1) >= self.nbackward:
                    # Proceed with BACKWARD
                    index_current_prod = self.avail_input_l1.index(used_prod_l1[0])
                    l1_list = self.avail_input_l1[index_current_prod:index_current_prod + self.nbackward]
                    l1 = used_prod_l1[0]
                    workplans.append(Backward(wdir=self.rep_work,
                                              outdir=self.path_input_l2,
                                              l1=l1,
                                              l1_list=l1_list,
                                              log_level=self.maja_log_level,
                                              skip_errors=self.skip_error,
                                              cams=Workplan.filter_cams_by_products(self.cams_files,
                                                                                    [prod.date for prod in
                                                                                     [l1] + l1_list])
                                              ))
                    pass
                else:
                    # Proceed with INIT
                    logger.info("Not enough L1 products available for a BACKWARD mode. Beginning with INIT...")
                    workplans.append(Init(wdir=self.rep_work,
                                          outdir=self.path_input_l2,
                                          l1=used_prod_l1[0],
                                          log_level=self.maja_log_level,
                                          skip_errors=self.skip_error,
                                          cams=Workplan.filter_cams_by_products(self.cams_files,
                                                                                [used_prod_l1[0].date])
                                          ))
                    pass
                pass
        else:
            logger.debug("Skipping L1 product %s because it was already processed!" % used_prod_l1[0].base)

        # For the rest: Setup NOMINAL.
        # Except: The time series is 'stopped' - The gap between two products is too large.
        # In this case, proceed with a re-init.
        for i, prod in enumerate(used_prod_l1[1:]):
            if prod in has_l2 and not self.overwrite:
                logger.debug("Skipping L1 product %s because it was already processed!" % prod.base)
                continue
            # Note: i, in this case is the previous product -> Not the current one, which is i+1
            date_gap = prod.date - used_prod_l1[i].date
            if date_gap >= prod.max_l2_diff:
                index_current_prod = self.avail_input_l1.index(prod)
                if len(self.avail_input_l1[index_current_prod:]) >= self.nbackward:
                    # Proceed with BACKWARD
                    l1_list = self.avail_input_l1[index_current_prod:index_current_prod + self.nbackward]
                    workplans.append(Backward(wdir=self.rep_work,
                                              outdir=self.path_input_l2,
                                              l1=prod,
                                              l1_list=l1_list,
                                              log_level=self.maja_log_level,
                                              skip_errors=self.skip_error,
                                              cams=Workplan.filter_cams_by_products(self.cams_files,
                                                                                    [prod.date for prod in
                                                                                     [prod] + l1_list])
                                              ))
                    pass
                else:
                    # Proceed with INIT
                    logger.info("Not enough L1 products available for a BACKWARD mode. Continuing with INIT...")
                    workplans.append(Init(wdir=self.rep_work,
                                          outdir=self.path_input_l2,
                                          l1=prod,
                                          log_level=self.maja_log_level,
                                          skip_errors=self.skip_error,
                                          cams=Workplan.filter_cams_by_products(self.cams_files,
                                                                                [prod.date])
                                          ))
                    pass
                pass
            else:
                workplans.append(Nominal(wdir=self.rep_work,
                                         outdir=self.path_input_l2,
                                         l1=prod,
                                         l2_date=prod.date,
                                         log_level=self.maja_log_level,
                                         skip_errors=self.skip_error,
                                         cams=Workplan.filter_cams_by_products(self.cams_files, [prod.date]),
                                         # Fallback parameters:
                                         remaining_l1=used_prod_l1[(i + 1):],
                                         nbackward=self.nbackward,
                                         remaining_cams=self.cams_files
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
            -   Create the input directory and link/copy all the needed inputs
            -   Create the output directory
            -   Run MAJA
        """
        if not self.dtm:
            logger.info("Attempting to download DTM...")
            self.avail_input_l1[0].get_mnt(dem_dir=self.rep_mnt, type_dem=self.type_dem,
                                           raw_dem=self.rep_raw, raw_gsw=self.rep_gsw)
            self.dtm = self.get_dtm(type_dem=self.type_dem)
            logger.info("DTM Creation succeeded.")
        if not self.gipp.check_completeness():
            logger.info("Attempting to download Gipp for %s" % self.gipp.gipp_folder_name)
            self.gipp.download()
        logger.info("GIPP Creation succeeded for %s" % self.gipp.gipp_folder_name)

        workplans = self.create_workplans(self.max_product_difference)
        logger.info("%s workplan(s) successfully created:" % len(workplans))
        # Print without the logging-formatting:
        print(str("%19s | %10s | %8s | %70s | %15s" % ("DATE", "TILE", "MODE", "L1-PRODUCT", "ADDITIONAL INFO")))
        for wp in workplans:
            print(wp)
        if not self.skip_confirm:
            input("Press Enter to continue...\n")
        logger.info("Beginning workplan execution.")
        for i, wp in enumerate(workplans):
            logger.info("Executing workplan #%s/%s" % (i+1, len(workplans)))
            wp.execute(self.maja, self.dtm, self.gipp, self.userconf)
        logger.info("=============Start_Maja v%s finished=============" % self.version)
        pass


if __name__ == "__main__":
    assert sys.version_info >= (3, 5), "Start_maja needs python >= 3.5.\n Run 'python --version' for more info."
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--tile", help="Tile number",
                        type=str, required=True)
    parser.add_argument("-s", "--site", help="Site name. If not specified,"
                                             "the tile number is used directly for finding the L1/L2 product directory",
                        type=str, required=False)
    parser.add_argument("-f", "--folder", help="Config/Folder-definition file used for all permanent paths.",
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
    parser.add_argument("--type_dem",
                        help="DEM type. If none is given, any will be used", required=False, type=str, default="any",
                        choices=["srtm", "eudem", "any"])
    parser.add_argument("-y", help="Skip workplan confirmation. Default is False",
                        action="store_true", required=False, default=False)
    parser.add_argument("--skip_errors", help="Skip erroneous products without stopping.",
                        action="store_true", required=False, default=False)
    parser.add_argument("--version", action='version', version='%(prog)s ' + str(StartMaja.version))
    parser.add_argument("--platform", help="Manually override which platform to use."
                                           "By default this is deducted by the available input product(s)",
                        choices=["sentinel2", "landsat8", "venus"], type=str, required=False, default=None)
    args = parser.parse_args()

    # TODO Add error skipping
    logging_level = logging.DEBUG if args.verbose else logging.INFO
    logger = StartMaja.init_loggers(msg_level=logging_level)

    s = StartMaja(args.folder, args.tile, args.site,
                  args.start, args.end, nbackward=args.nbackward, logger=logger,
                  overwrite=args.overwrite, cams=args.cams,
                  skip_confirm=args.y, platform=args.platform,
                  type_dem=args.type_dem, skip_errors=args.skip_errors)
    s.run()
