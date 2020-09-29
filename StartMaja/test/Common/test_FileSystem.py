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
from StartMaja.Common import TestFunctions
from StartMaja.Common import FileSystem
import os
from os import path as p


class TestFileSystem(unittest.TestCase):

    root = os.path.abspath(os.path.join(os.getcwd(), "test_filesystem_dir"))

    subdir_prefix = "subdir"
    file_a1 = "a"
    file_a2 = "a.jpg"
    file_b1 = "b.jpg"
    file_c1 = "c.xml"

    def setUp(self):
        """
        Sets up a random tree-like structure with a few sub-files and -folders
        :return:
        """
        os.makedirs(self.root)
        TestFunctions.touch(p.join(self.root, self.file_a1))
        TestFunctions.touch(p.join(self.root, self.file_a2))
        TestFunctions.touch(p.join(self.root, self.file_b1))
        TestFunctions.touch(p.join(self.root, self.file_c1))
        for i in range(2):
            subdir = p.join(self.root, self.subdir_prefix + str(i))
            os.makedirs(subdir)
            TestFunctions.touch(p.join(subdir, self.file_a1))
            TestFunctions.touch(p.join(subdir, self.file_a2))
            TestFunctions.touch(p.join(subdir, self.file_b1))
            TestFunctions.touch(p.join(subdir, self.file_c1))
            for j in range(2):
                ssubdir = p.join(subdir, self.subdir_prefix + str(j))
                os.makedirs(ssubdir)
                TestFunctions.touch(p.join(ssubdir, self.file_a1))
                TestFunctions.touch(p.join(ssubdir, self.file_a2))
                TestFunctions.touch(p.join(ssubdir, self.file_b1))
                TestFunctions.touch(p.join(ssubdir, self.file_c1))

    def tearDown(self):
        import shutil
        shutil.rmtree(self.root)

    def test_create_remove_dir(self):
        path = os.path.join(os.getcwd(), "test_create_remove_dir")
        # This throws a log message
        FileSystem.create_directory(path)
        self.assertTrue(os.path.isdir(path))
        FileSystem.remove_directory(path)
        self.assertFalse(os.path.isdir(path))
        self.assertFalse(os.path.exists(path))

    def test_create_remove_file(self):
        path = os.path.join(os.getcwd(), "test_create_remove_file")
        self.assertFalse(os.path.exists(path))
        TestFunctions.touch(path)
        self.assertTrue(os.path.exists(path))
        self.assertTrue(os.path.isfile(path))
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))
    
    def test_get_file_depth1(self):
        expected = os.path.join(self.root, "a")
        dirnames_e = p.normpath(expected).split(os.sep)
        calculated = FileSystem.find(path=self.root, pattern="^a$", depth=1)
        self.assertEqual(len(calculated), 1)
        dirnames_c = p.normpath(calculated[0]).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(expected, calculated[0])
    
    def test_get_file_depth2(self):
        expected = os.path.join(self.root, "subdir1", "a")
        dirnames_e = p.normpath(expected).split(os.sep)
        calculated = FileSystem.find(path=self.root, pattern="^a$", depth=2)
        self.assertEqual(len(calculated), 3)
        dirnames_c = p.normpath(calculated[0]).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(expected, sorted(calculated)[-1])

    def test_get_file_depth3(self):
        expected = os.path.join(self.root, "subdir1", "subdir1", "a")
        dirnames_e = p.normpath(expected).split(os.sep)
        calculated = FileSystem.find(path=self.root, pattern="^a$", depth=3)
        self.assertEqual(len(calculated), 7)
        dirnames_c = p.normpath(calculated[0]).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(expected, sorted(calculated)[-1])

    def test_get_file_ending(self):
        expected = os.path.join(self.root, "c.xml")
        dirnames_e = p.normpath(expected).split(os.sep)
        calculated = FileSystem.find(path=self.root, pattern="*xml", depth=1)
        self.assertEqual(len(calculated), 1)
        dirnames_c = p.normpath(calculated[0]).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(expected, calculated[0])

    def test_get_file_full(self):
        expected = os.path.join(self.root, "b.jpg")
        dirnames_e = p.normpath(expected).split(os.sep)
        calculated = FileSystem.find(path=self.root, pattern="b.jpg", depth=1)
        calculated2 = FileSystem.find(path=self.root, pattern="b.jpg", depth=1, ftype="file")
        self.assertEqual(calculated, calculated2)
        self.assertEqual(len(calculated), 1)
        dirnames_c = p.normpath(calculated[0]).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(expected, calculated[0])
    
    def test_get_folder(self):
        expected = os.path.join(self.root, "subdir0")
        dirnames_e = p.normpath(expected).split(os.sep)
        filename = p.basename(expected)
        calculated = FileSystem.find(path=self.root, pattern="subdir0", depth=1)
        calculated2 = FileSystem.find(path=self.root, pattern="subdir0", depth=1, ftype="folder")
        self.assertEqual(calculated, calculated2)
        self.assertEqual(len(calculated), 1)
        dirnames_c = p.normpath(calculated[0]).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(filename, p.basename(calculated[0]))

    def test_find_get_sub_folder(self):
        expected = "subdir0"
        calculated = FileSystem.find(path=self.root, pattern=expected)
        self.assertEqual(expected, p.basename(calculated[0]))

    def test_find_get_file(self):
        expected = "a.jpg"
        calculated = FileSystem.find(path=self.root, pattern=expected)
        self.assertEqual(expected, p.basename(calculated[0]))
    
    def test_find_get_subsubfolder(self):
        expected = "subdir1"
        calculated = FileSystem.find(path=self.root, pattern=expected)
        self.assertEqual(expected, p.basename(calculated[0]))

    def test_make_symlink_file(self):
        origin = os.path.join(self.root, self.file_a1)
        destination = os.path.join(self.root, "symlink1")
        self.assertTrue(os.path.exists(origin))
        self.assertFalse(os.path.exists(destination))
        FileSystem.symlink(origin, destination)
        self.assertTrue(os.path.islink(destination))
        os.remove(destination)
        self.assertFalse(os.path.exists(destination))
        self.assertTrue(os.path.exists(origin))

    def test_make_symlink_folder(self):
        origin = os.path.join(self.root, self.subdir_prefix + "0")
        destination = os.path.join(self.root, "symlink2")
        self.assertTrue(os.path.exists(origin))
        self.assertFalse(os.path.exists(destination))
        FileSystem.symlink(origin, destination)
        self.assertTrue(os.path.islink(destination))
        os.remove(destination)
        self.assertFalse(os.path.exists(destination))
        self.assertTrue(os.path.exists(origin))

    def test_run_app(self):
        import logging
        cmd = "echo"
        args = ["Hello"]
        self.assertEqual(FileSystem.run_external_app(cmd, args, log_level=logging.INFO), 0)

    def test_run_nonexisting_app(self):
        import subprocess
        cmd = "non_existing_app"
        args = [""]
        err = subprocess.CalledProcessError
        with self.assertRaises(err):
            FileSystem.run_external_app(cmd, args)


if __name__ == '__main__':
    unittest.main()
