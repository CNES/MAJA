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

logger = logging.getLogger("root")


class Workplan(object):
    """
    Stores all information about a single execution of Maja
    """
    mode = "INIT"

    def __init__(self, wdir, outdir, l1, log_level="INFO", **kwargs):
        supported_params = {
            param
            for param in ("cams", "meteo", )
            if kwargs.get(param, None) is not None
        }
        # Check if the directories exist:
        assert os.path.isdir(wdir)
        assert os.path.isdir(outdir)
        self.l1 = l1
        self.outdir = outdir

        self.root = wdir
        self.input_dir = os.path.join(self.root, "Start_maja_" + self.get_dirname(self.l1.base))
        self.wdir = os.path.join(self.input_dir, "maja_working_directory")

        self.tile = self.l1.tile
        self.date = self.l1.date
        self.log_level = log_level if log_level.upper() in ['INFO', 'PROGRESS', 'WARNING', 'DEBUG', 'ERROR'] else "INFO"
        self.aux_files = []
        for key in supported_params:
            self.aux_files += kwargs[key]

    def __str__(self):
        raise NotImplementedError

    def execute(self, maja, dtm, gipp, conf):
        """
        Run the workplan with its given parameters
        :param maja: The path to the maja executable
        :param dtm: The DTM object
        :param gipp: The GIPP object
        :param conf: The full path to the userconf folder
        :return:
        """
        raise NotImplementedError

    @staticmethod
    def get_dirname(name):
        """
        Create a hash of the product name in order to have a unique folder name
        :param name: The product basename
        :return: The product name as hex-hash
        """
        import hashlib
        return hashlib.md5(name.encode("utf-8")).hexdigest()

    def create_working_dir(self, dtm, gipps):
        """
        Create a temporary working directory for a single start maja execution
        Then, link all files of the input directory, which are:
            Product(s) (1C/2A)
            GIPPs
            DTM
            (CAMS) if existing
        :param dtm: The DTM object
        :param gipps: The GIPP object
        :return: The full path to the created input directory
        """
        from Common.FileSystem import create_directory, remove_directory
        # Try to remove the directory before proceeding:
        remove_directory(self.input_dir)
        create_directory(self.input_dir)
        create_directory(self.wdir)
        if not os.path.isdir(self.input_dir) or not os.path.isdir(self.wdir):
            raise OSError("Cannot create temp directory %s, %s" % (self.input_dir, self.wdir))
        self.l1.link(self.input_dir)
        for f in self.aux_files:
            f.link(self.input_dir)
        dtm.link(self.input_dir)
        gipps.link(self.input_dir)

        return self.input_dir

    def launch_maja(self, maja, wdir, inputdir, outdir, conf):
        """
        Run the MAJA processor for the given input_dir, mode and tile
        :param maja: The full path to the maja executable
        :param wdir: The working dir containing all inputs
        :param inputdir: The input directory containing all necessary files
        :param outdir: The output L2-directory
        :param conf: The full path to the userconf folder
        :return: The return code of Maja
        """
        from Common import FileSystem
        args = ["-w",
                wdir,
                "--input",
                inputdir,
                "--output",
                outdir,
                "--mode",
                "L2" + self.mode,
                "-ucs",
                conf,
                "--TileId",
                self.tile,
                "--loglevel",
                self.log_level]
        return FileSystem.run_external_app(maja, args)


class Init(Workplan):
    mode = "INIT"

    def execute(self, maja, dtm, gipp, conf):
        """
        Run the workplan with its given parameters
        :param maja: The path to the maja executable
        :param dtm: The DTM object
        :param gipp: The GIPP object
        :param conf: The full path to the userconf folder
        :return: The return code of the Maja app
        """
        from Common.FileSystem import remove_directory
        self.create_working_dir(dtm, gipp)
        return_code = self.launch_maja(maja, wdir=self.wdir, inputdir=self.input_dir, outdir=self.outdir, conf=conf)
        remove_directory(self.input_dir)
        return return_code
        
    def __str__(self):
        return str("%19s | %5s | %8s | %70s | %15s" % (self.date, self.tile,
                                                       self.mode, self.l1.base,
                                                       "Init mode - No previous L2"))


class Backward(Workplan):
    mode = "BACKWARD"

    def __init__(self, wdir, outdir, l1, l1_list, log_level="INFO", **kwargs):
        self.l1_list = l1_list
        super(Backward, self).__init__(wdir, outdir, l1, log_level, **kwargs)

    def execute(self, maja, dtm, gipp, conf):
        """
        Run the workplan with its given parameters
        :param maja: The path to the maja executable
        :param dtm: The DTM object
        :param gipp: The GIPP object
        :param conf: The full path to the userconf folder
        :return: The return code of the Maja app
        """
        from Common.FileSystem import remove_directory
        self.create_working_dir(dtm, gipp)
        # Link additional L1 products:
        for prod in self.l1_list:
            prod.link(self.input_dir)
        return_code = self.launch_maja(maja, wdir=self.wdir, inputdir=self.input_dir, outdir=self.outdir, conf=conf)
        remove_directory(self.input_dir)
        return return_code

    def __str__(self):
        return str("%19s | %5s | %8s | %70s | %15s" % (self.date, self.tile,
                                                       self.mode, self.l1.base,
                                                       "Backward of %s products" % str(len(self.l1_list) + 1)))


class Nominal(Workplan):
    mode = "NOMINAL"

    def __init__(self, wdir, outdir, l1, l2_date, log_level="INFO", **kwargs):
        import datetime
        assert isinstance(l2_date, datetime.datetime)
        self.l2_date = l2_date
        self.l2 = kwargs.get("l2", None)
        self.remaining_l1 = kwargs.get("remaining_l1", [])
        self.nbackward = kwargs.get("nbackward", int(8))
        super(Nominal, self).__init__(wdir, outdir, l1, log_level, **kwargs)

    def _get_available_l2_products(self):
        """
        Get the list of available l2 products
        :return: The list of l2 products currently available that are after the given l1 date
        """
        from Start_maja import StartMaja
        # Find the previous L2 product
        avail_input_l2 = StartMaja.get_available_products(self.outdir, "l2a", self.tile)
        # Get only products which are close to the desired l2 date and before the l1 date:
        l2_prods = [prod for prod in avail_input_l2
                    if abs(prod.date - self.l2_date) < StartMaja.max_l2_diff and
                    prod.date < self.date and prod.validity]
        return l2_prods

    @staticmethod
    def _get_l2_product(l2_prods):
        """
        Get the L2 product used for the execution
        :return:
        """
        # Take the first product:
        # TODO Verify edge cases
        return l2_prods[0]

    def execute(self, maja, dtm, gipp, conf):
        """
        Run the workplan with its given parameters
        :param maja: The path to the maja executable
        :param dtm: The DTM object
        :param gipp: The GIPP object
        :param conf: The full path to the userconf folder
        :return: The return code of the Maja app
        """
        from Common.FileSystem import remove_directory
        self.create_working_dir(dtm, gipp)
        l2_prods = self._get_available_l2_products()
        if not l2_prods:
            logger.error("Cannot find previous L2 product for date %s in %s" % (self.date, self.outdir))
            if len(self.remaining_l1) >= self.nbackward:
                logging.info("Setting up a BACKWARD execution instead.")
                backup_wp = Backward(self.wdir, self.outdir, self.l1, l1_list=self.remaining_l1,
                                     log_level=self.log_level, cams=self.aux_files)
            else:
                logging.info("Setting up an INIT execution instead.")
                backup_wp = Init(self.wdir, self.outdir, self.l1, self.log_level, cams=self.aux_files)
            return backup_wp.execute(maja, dtm, gipp, conf)
        if len(l2_prods) > 1:
            logger.info("%s products found for date %s" % (len(l2_prods), self.date))
        self.l2 = self._get_l2_product(l2_prods)
        # Link additional L2 products:
        self.l2.link(self.input_dir)
        return_code = self.launch_maja(maja, wdir=self.wdir, inputdir=self.input_dir, outdir=self.outdir, conf=conf)
        remove_directory(self.input_dir)
        return return_code

    def __str__(self):
        return str("%19s | %5s | %8s | %70s | %15s" % (self.date, self.tile,
                                                       self.mode, self.l1.base,
                                                       "L2 from previous"))


if __name__ == "__main__":
    pass
