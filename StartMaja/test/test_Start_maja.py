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

import unittest
from StartMaja.Start_maja import StartMaja
import sys
import os
from datetime import datetime
sys.path.append(StartMaja.current_dir)  # Replaces __init__.py


def modify_folders_file(root, new_file, **kwargs):
    """
    Modify the template test_folders file with the given arguments
    :param root: The path to the existing template file
    :param new_file: The new file path
    :param kwargs: The arguments to modify inside the file
    :return: The full path to the new file
    """
    import configparser as cfg
    print(root)
    assert os.path.isfile(root)
    cfg_parser = cfg.RawConfigParser()
    cfg_parser.read(root)
    for arg in kwargs:
        cfg_parser.set("Maja_Inputs", arg, kwargs[arg])

    with open(new_file, 'w') as f:
        cfg_parser.write(f)
    return new_file


class TestStartMaja01XYZ(unittest.TestCase):

    root = os.path.join(os.getcwd(), "TestStartMaja01XYZ")
    n_not_used = 5
    n_dummies = 5
    start_product = datetime(2014, 12, 31, 10, 50)
    end_product = datetime(2099, 12, 31)

    tile = "T01XYZ"
    site = None
    start = None
    end = None
    nbackward = 8
    overwrite = True
    verbose = False
    template_folders_file = os.path.join(os.path.dirname(__file__), "test_folders.txt")

    @classmethod
    def setUpClass(cls):
        from StartMaja.Chain import DummyFiles
        from StartMaja.Common import FileSystem
        cls.product_root = os.path.join(cls.root, cls.tile)
        FileSystem.remove_directory(cls.root)
        FileSystem.create_directory(cls.root)
        FileSystem.create_directory(cls.product_root)
        DummyFiles.L1Generator(cls.product_root,
                               tile=cls.tile,
                               date=cls.start_product,
                               platform="sentinel2").generate()
        DummyFiles.L1Generator(cls.product_root,
                               tile=cls.tile,
                               date=cls.end_product,
                               platform="sentinel2").generate()
        for d in set([DummyFiles.random_date() for _ in range(cls.n_dummies)]):
            DummyFiles.L1Generator(cls.product_root, tile=cls.tile, date=d, platform="sentinel2").generate()
            DummyFiles.L2Generator(cls.product_root, tile=cls.tile, date=d, platform="sentinel2").generate()

        for d in set([DummyFiles.random_date() for _ in range(cls.n_not_used)]):
            DummyFiles.L1Generator(cls.product_root, date=d, platform="sentinel2").generate()
            DummyFiles.L2Generator(cls.product_root, date=d, platform="sentinel2").generate()

        cls.folders_file = os.path.join(cls.root, "test_working_folders_file.txt")
        modify_folders_file(cls.template_folders_file, new_file=cls.folders_file,
                            exeMaja=os.getcwd(),
                            repWork=os.getcwd(),
                            repGIPP=os.getcwd(),
                            repL1=os.getcwd(),
                            repL2=os.getcwd(),
                            repMNT=os.getcwd())
        cls.mnt = DummyFiles.MNTGenerator(root=cls.root, tile=cls.tile, platform="sentinel2")
        cls.mnt.generate()
        cls.cams = os.path.join(cls.root, "CAMS")
        os.makedirs(cls.cams)
        DummyFiles.CAMSGenerator(cls.cams, cls.start_product).generate()
        DummyFiles.CAMSGenerator(cls.cams, cls.end_product).generate()
        for _ in range(cls.n_not_used):
            DummyFiles.CAMSGenerator(cls.cams).generate()

        assert os.path.isfile(cls.folders_file)

    @classmethod
    def tearDownClass(cls):
        from StartMaja.Common import FileSystem
        # In case there's duplicates, remove them:
        FileSystem.remove_directory(cls.product_root)
        FileSystem.remove_file(cls.folders_file)
        FileSystem.remove_directory(cls.cams)
        FileSystem.remove_directory(cls.mnt.dbl)
        FileSystem.remove_file(cls.mnt.hdr)

    def test_dates_and_products(self):
        start_maja = StartMaja(self.folders_file,
                               self.tile,
                               self.site,
                               self.start,
                               self.end,
                               nbackward=self.nbackward,
                               overwrite=self.overwrite)
        self.assertGreaterEqual(self.n_dummies + 2, len(start_maja.avail_input_l1))
        self.assertGreaterEqual(self.n_dummies, len(start_maja.avail_input_l2))
        self.assertEqual(start_maja.start, self.start_product)
        self.assertEqual(start_maja.end, self.end_product)

    def test_parasite_l2a_product(self):
        from StartMaja.Chain import DummyFiles
        prod = DummyFiles.L2Generator(self.product_root,
                                      platform="venus",
                                      tile=self.tile)
        prod.generate()
        with self.assertRaises(IOError):
            StartMaja(self.folders_file,
                      self.tile,
                      self.site,
                      self.start,
                      self.end,
                      nbackward=self.nbackward,
                      overwrite=self.overwrite)
        import shutil
        shutil.rmtree(prod.prod)
        self.assertFalse(os.path.exists(prod.prod))

    def test_non_existing_l1c_folder(self):
        folders_path = os.path.join(os.getcwd(), "test_error_folder_file.txt")
        modify_folders_file(self.folders_file, new_file=folders_path,
                            repL1="/a/b/c")
        with self.assertRaises(OSError):
            StartMaja(folders_path,
                      self.tile,
                      self.site,
                      self.start,
                      self.end,
                      nbackward=self.nbackward,
                      overwrite=self.overwrite)

        os.remove(folders_path)
        self.assertFalse(os.path.exists(folders_path))

    def test_custom_start_end_dates(self):
        start = datetime(2017, 1, 1)
        end = datetime(2019, 1, 1)
        s = StartMaja(self.folders_file,
                      self.tile,
                      self.site,
                      start.strftime("%Y-%m-%d"),
                      end.strftime("%Y-%m-%d"),
                      nbackward=self.nbackward,
                      overwrite=self.overwrite)

        self.assertEqual(s.start, start)
        self.assertEqual(s.end, end)

    def test_custom_start_end_dates(self):
        start = datetime(2017, 1, 1)
        end = datetime(2019, 1, 1)
        s = StartMaja(self.folders_file,
                      self.tile,
                      self.site,
                      start.strftime("%Y-%m-%d"),
                      end.strftime("%Y-%m-%d"),
                      nbackward=self.nbackward,
                      overwrite=self.overwrite,
                      verbose=self.verbose)

        self.assertEqual(s.start, start)
        self.assertEqual(s.end, end)


class TestStartMaja31TCH(unittest.TestCase):

    root = os.path.join(os.getcwd(), "TestStartMaja31TCH")
    n_not_used = 10
    n_dummies = 10
    l1_dates = [datetime(2018, 4, 1, 10, 50),
                datetime(2018, 4, 3, 10, 55)]
    end_product = datetime(2099, 12, 31)

    tile = "T31TCH"
    site = None
    start = None
    end = None
    nbackward = 8
    overwrite = True
    verbose = False
    template_folders_file = os.path.join(os.getcwd(), "test", "test_folders.txt")

    @classmethod
    def setUpClass(cls):
        from StartMaja.Chain import DummyFiles
        from StartMaja.Common import FileSystem
        FileSystem.remove_directory(cls.root)
        FileSystem.create_directory(cls.root)
        cls.product_root = os.path.join(cls.root, cls.tile)
        FileSystem.create_directory(cls.product_root)
        DummyFiles.L1Generator(cls.product_root,
                               tile=cls.tile,
                               date=cls.start_product,
                               platform="sentinel2").generate()
        DummyFiles.L1Generator(cls.product_root,
                               tile=cls.tile,
                               date=cls.end_product,
                               platform="sentinel2").generate()
        for d in set([DummyFiles.random_date() for _ in range(cls.n_dummies)]):
            DummyFiles.L1Generator(cls.product_root, tile=cls.tile, date=d, platform="sentinel2").generate()
            DummyFiles.L2Generator(cls.product_root, tile=cls.tile, date=d, platform="sentinel2").generate()

        for d in set([DummyFiles.random_date() for _ in range(cls.n_not_used)]):
            DummyFiles.L1Generator(cls.product_root, date=d, platform="sentinel2").generate()
            DummyFiles.L2Generator(cls.product_root, date=d, platform="sentinel2").generate()

        cls.folders_file = os.path.join(cls.root, "test_working_folders_file.txt")
        modify_folders_file(cls.template_folders_file, new_file=cls.folders_file,
                            exeMaja=os.getcwd(),
                            repWork=os.getcwd(),
                            repGIPP=os.getcwd(),
                            repL1=os.getcwd(),
                            repL2=os.getcwd(),
                            repMNT=os.getcwd())
        cls.mnt = DummyFiles.MNTGenerator(root=cls.root, tile=cls.tile, platform="sentinel2")
        cls.mnt.generate()
        cls.cams = os.path.join(cls.root, "CAMS")
        os.makedirs(cls.cams)
        DummyFiles.CAMSGenerator(cls.cams, cls.start_product).generate()
        DummyFiles.CAMSGenerator(cls.cams, cls.end_product).generate()
        for _ in range(cls.n_not_used):
            DummyFiles.CAMSGenerator(cls.cams).generate()

        assert os.path.isfile(cls.folders_file)

    @classmethod
    def tearDownClass(cls):
        from StartMaja.Common import FileSystem
        # In case there's duplicates, remove them:
        FileSystem.remove_directory(cls.product_root)
        FileSystem.remove_file(cls.folders_file)
        FileSystem.remove_directory(cls.cams)
        FileSystem.remove_directory(cls.mnt.dbl)
        FileSystem.remove_file(cls.mnt.hdr)


if __name__ == '__main__':
    assert sys.version_info >= (3, 5)
    import logging
    logger = logging.getLogger("root")
    unittest.main()
