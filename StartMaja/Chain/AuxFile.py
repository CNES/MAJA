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
import re


class EarthExplorer(object):
    """
    Class to store an auxiliary EarthExplorer file
    """

    regex = r"^[\w-]+.\w+"

    # TODO Add platform detection

    def __new__(cls, dbl, regex=None):
        """
        Instantiate a new EarthExplorer file
        :param dbl: A folder name
        :return:
        """

        if not os.path.isdir(dbl):
            return None
        reg = cls.regex if regex is None else regex
        if not re.search(reg, os.path.basename(dbl)):
            return None
        return object.__new__(cls)

    def __init__(self, dbl, regex=None):
        from StartMaja.Common import FileSystem
        self.regex = self.regex if regex is None else regex
        self.dbl = dbl
        self.base = os.path.basename(dbl).split(".")[0]
        # Find associated HDR
        self.hdr = FileSystem.find_single(path=os.path.join(dbl, "../"), pattern=self.base + ".HDR")
        assert os.path.isfile(self.hdr)

    def __str__(self):
        return "\n".join(["DBL: " + self.dbl,
                          "HDR: " + self.hdr])

    def __repr__(self):
        return self.__str__()

    @classmethod
    def get_specifiable_regex(cls):
        """
        Return regex without r"\\w+" at the end in order to make it costumizable.
        :return: The class regex without \\w+
        """
        return cls.regex[:-3]

    def link(self, dest):
        """
        Symlink a file to the working dir without copying it
        :param dest: The destination folder
        :return:
        """
        from StartMaja.Common import FileSystem
        hdr_realpath = os.path.realpath(os.path.expanduser(self.hdr))
        dbl_realpath = os.path.realpath(os.path.expanduser(self.dbl))
        FileSystem.symlink(hdr_realpath, os.path.join(dest, os.path.basename(hdr_realpath)))
        FileSystem.symlink(dbl_realpath, os.path.join(dest, os.path.basename(dbl_realpath)))


class CAMSFile(EarthExplorer):
    """
    Stores a single CAMS file
    """
    regex = r"(VE|S2_|L8)_(TEST|PROD|OPER)_EXO_CAMS_\d{8}T\d{6}_\d{8}T\d{6}.DBL(.DIR)?"

    @classmethod
    def check_regex(cls, dbl):
        return re.search(cls.regex, dbl)

    def get_date(self):
        from datetime import datetime
        base = os.path.splitext(os.path.basename(self.hdr))[0]
        return datetime.strptime(base.split("_")[-2], "%Y%m%dT%H%M%S")

    def get_end_date(self):
        from datetime import datetime
        base = os.path.splitext(os.path.basename(self.hdr))[0]
        return datetime.strptime(base.split("_")[-1], "%Y%m%dT%H%M%S")


class DTMFile(EarthExplorer):
    """
    Stores a single DTM
    """
    regex = r"\w*_(TEST|PROD)_AUX_REFDE2_\w+"

    mnt_version = {"srtm": ["1001", "0001"], "merit": ["2001"], "eudem": ["3001"],
                   "any": [r"\d001"]}
