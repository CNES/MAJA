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


def random_tile(platform="sentinel2"):
    """
    Create a random tile string for a given platform.
    For 'sentinel2': T12ABC, for 'landsat8': 192820, for 'venus': ABCDE
    :param platform: Return a tile corresponding to the given platform
    :return: String compliant with the given platform
    """
    import string
    import random

    letters = string.ascii_uppercase
    tile = "T" + str(random.randint(0, 99)).zfill(2) + ''.join(random.choice(letters) for _ in range(3))
    if platform == "sentinel2":
        return tile
    elif platform == "landsat8":
        number = str(random.randint(0, 999999)).zfill(6)
        return random.choice([number, tile])
    elif platform == "venus":
        return ''.join(random.choice(letters) for _ in range(5))

    return tile


def random_platform(product_level=None):
    import random
    if product_level == "L1C":
        return random.choice(["sentinel2"])
    return random.choice(["sentinel2", "venus", "landsat8"])


def random_date():
    import random
    from datetime import datetime, timedelta
    date = datetime(2015, 1, 1) + timedelta(days=random.randint(0, 10000),
                                            hours=random.randint(10, 12),
                                            minutes=random.randint(0, 60),
                                            seconds=random.randint(0, 60))
    return date


class DummyGenerator(object):
    """
    Base class for all dummy generators
    """
    def __init__(self, root, date=None, tile=None, platform=random_platform()):
        self.root = root
        if not date:
            self.date = random_date()
        else:
            self.date = date
        if platform.lower() == "sentinel2":
            self.platform = "sentinel2"
        elif platform.lower() == "landsat8":
            self.platform = "landsat8"
        elif platform.lower() == "venus":
            self.platform = "venus"
        else:
            raise ValueError("Unknown platform found: %s" % platform)
        if not tile:
            self.tile = random_tile(platform=platform)
        else:
            self.tile = tile

    def generate(self, **kwargs):
        raise NotImplementedError


class DummyEarthExplorer(DummyGenerator):
    """
    Base class for all EarthExplorer-like files
    """

    mission_choices = {"tm": {"sentinel2": "SENTINEL-2"},
                       "muscate": {"sentinel2": "SENTINEL2", "landsat8": "LANDSAT8", "venus": "VENUS"},
                       "natif": {"sentinel2": "SENTINEL-2", "landsat8": "LANDSAT_8", "venus": "VENuS"}
                       }

    mission_short = {"sentinel2": "S2_",
                     "venus": "VE",
                     "landsat8": "L8"}

    def __init__(self, root, date=None, tile=None, platform=random_platform()):
        super(DummyEarthExplorer, self).__init__(root, date, tile, platform)
        self.hdr = None
        self.dbl = None

    def get_mission(self):
        """
        Return a random Mission name
        :return:
        """
        import random
        mtype = random.choice(["muscate", "natif"])
        return self.mission_choices[mtype][self.platform]

    def create_dummy_hdr(self, file_path, mission=None):
        """
        Create a dummy HDR file with only the "Mission" field in it.
        :param file_path: The full path to the file that should be written
        :param mission: The content of the 'Mission' field to be set.
        """
        import random
        from xml.etree import ElementTree
        platform_hdr = {"sentinel2": random.choice(["SENTINEL2_", "SENTINEL-2_"]),
                        "venus": random.choice(["VENUS", "VENuS"]),
                        "landsat8": random.choice(["LANDSAT8", "LANDSAT_8"])}
        mission = mission if mission else platform_hdr[self.platform]
        root = ElementTree.Element("Earth_Explorer_Header")
        sub = ElementTree.SubElement(root, "Fixed_Header")
        ElementTree.SubElement(sub, "Mission").text = mission
        ElementTree.SubElement(root, "Variable_Header")
        tree = ElementTree.ElementTree(root)
        tree.write(file_path)

    def generate(self, **kwargs):
        # Pass this on
        raise NotImplementedError


class MNTGenerator(DummyEarthExplorer):
    """
    Class to create a single dummy MNT.
    """
    def generate(self, **kwargs):
        import random
        from StartMaja.Common import FileSystem
        from StartMaja.Chain.AuxFile import DTMFile
        mission_param = kwargs.get("mission", self.get_mission())
        mission_specifier = "_" if self.platform == "sentinel2" else ""
        basename = "_".join([self.mission_short[self.platform] + mission_specifier,
                             "TEST", "AUX", "REFDE2", self.tile,
                             str(random.randint(0, 1000)).zfill(4)])
        self.dbl = os.path.join(self.root, basename + ".DBL.DIR")
        self.hdr = os.path.join(self.root, basename + ".HDR")
        FileSystem.create_directory(self.dbl)
        self.create_dummy_hdr(self.hdr, mission=mission_param + mission_specifier)
        return DTMFile(self.dbl)


class CAMSGenerator(DummyEarthExplorer):
    """
    Class to create a single dummy CAMS file.
    """
    def generate(self, **kwargs):
        from datetime import datetime
        from StartMaja.Common import FileSystem
        from StartMaja.Chain.AuxFile import CAMSFile
        end_date = datetime(2099, 1, 1, 23, 59, 59)
        mission_param = kwargs.get("mission", self.get_mission())
        mission_specifier = "_" if self.platform == "sentinel2" else ""
        basename = "_".join([self.mission_short[self.platform],
                             "TEST", "EXO", "CAMS",
                             self.date.strftime("%Y%m%dT%H%M%S"),
                             end_date.strftime("%Y%m%dT%H%M%S")])
        self.dbl = os.path.join(self.root, basename + ".DBL.DIR")
        self.hdr = os.path.join(self.root, basename + ".HDR")
        FileSystem.create_directory(self.dbl)
        self.create_dummy_hdr(self.hdr, mission=mission_param + mission_specifier)
        return CAMSFile(self.dbl)


class GippGenerator(DummyEarthExplorer):
    """
    Class to create a single dummy GIPP set.
    """

    def __init__(self, root, date=None, tile=None, platform=random_platform()):
        super(GippGenerator, self).__init__(root, date, tile, platform)

    def _create_hdr(self, out_path, sat, name, start_date, version, model, mission, file_type):
        """
        Create a single HDR or EEF file
        """
        prefix = "L2"
        mission_specifier = sat[-1] if sat in ["S2A", "S2B"] else ""
        if name == "SITE" and sat == "S2B":
            return ""
        if name == "SITE" and sat == "S2A":
            sat = "S2_"
            mission_specifier = "_"
        if name in ["EXTL", "QLTL"]:
            prefix = "CK"
        basename = "_".join([sat, "TEST", "GIP", prefix + name, "L",
                             "{s:_^8}".format(s=model),
                             version, start_date.strftime("%Y%m%d"),
                             self.date.strftime("%Y%m%d")])
        hdr_name = os.path.join(out_path, basename + file_type)
        self.create_dummy_hdr(hdr_name, mission=mission + mission_specifier)
        return basename

    def generate(self, **kwargs):
        from datetime import datetime
        import random
        from StartMaja.Common import FileSystem
        from StartMaja.Chain.GippFile import GippSet
        mission_param = kwargs.get("mission", random.choice(["muscate", "natif"]))
        if mission_param == "tm":
            self.platform = "sentinel2"
        mission = self.mission_choices[mission_param][self.platform]
        satellites = [self.mission_short[self.platform]] if self.platform != "sentinel2" else ["S2A", "S2B"]
        with_cams = kwargs.get("cams", True)
        cams_suffix = "_CAMS" if with_cams else ""
        out_path = os.path.join(self.root, "_".join([self.platform.upper(), mission_param.upper()])
                                           + cams_suffix)
        FileSystem.create_directory(out_path)
        if with_cams:
            models = ["CONTINEN"] + ["ORGANICM", "BLACKCAR", "DUST", "SEASALT", "SULPHATE"]
        else:
            models = ["CONTINEN"]
        allsites = "ALLSITES"
        hdr_types = ["ALBD", "DIFT", "DIRT", "TOCR", "WATV"]
        eef_types = ["COMM", "SITE", "SMAC", "EXTL", "QLTL"]
        tm_types = ["COMM", "EXTL", "QLTL"]
        version = random.randint(0, 9999)
        version_str = str(version).zfill(5)
        start_date = datetime(2014, 12, 30)
        for sat in satellites:
            for name in eef_types:
                self._create_hdr(out_path, sat, name, start_date, version_str, allsites, mission, ".EEF")
            for name in hdr_types:
                for model in models:
                    basename = self._create_hdr(out_path, sat, name, start_date, version_str, model, mission, ".HDR")
                    dbl_name = os.path.join(out_path, basename + ".DBL.DIR")
                    FileSystem.create_directory(dbl_name)
            # For TM: Add an additional set of COMM, EXTL and QLTL files with muscate mission:
            for name in tm_types:
                if mission_param != "tm":
                    continue
                tm_mission = "SENTINEL2"
                tm_version_str = str(version + 10000).zfill(5)
                self._create_hdr(out_path, sat, name, start_date, tm_version_str, allsites, tm_mission, ".EEF")
        return GippSet(self.root, self.platform, mission_param, cams=with_cams)


class ProductGenerator(DummyGenerator):
    platform_options = {"L1C": {"sentinel2": ["S2A", "S2B"]},
                        "L2A": {"sentinel2": ["SENTINEL2B", "SENTINEL2A"],
                                "landsat8": ["LANDSAT8-OLITIRS"],
                                "venus": ["VENUS-XS"]}}

    def __init__(self, root, date=None, tile=None, platform=random_platform()):
        super(ProductGenerator, self).__init__(root, date, tile, platform)
        self.prod = None
        self.mtd = None

    def generate(self, **kwargs):
        raise NotImplementedError


class L1Generator(ProductGenerator):
    """
    Class to create a dummy L1C product
    """
    def __init__(self, root, date=None, tile=None, platform=random_platform(product_level="L1C")):
        super(L1Generator, self).__init__(root, date, tile, platform)

    def generate(self, **kwargs):
        import random
        from StartMaja.Chain import Product
        from StartMaja.Common import TestFunctions, FileSystem
        platform_specifier = random.choice(self.platform_options["L1C"][self.platform])
        orbit = kwargs.get("orbit", random.randint(0, 999))
        version_orbit = kwargs.get("version", random.randint(0, 9))
        if "sentinel2" in self.platform:
            date_str = self.date.strftime("%Y%m%dT%H%M%S")
            product_name = "_".join([platform_specifier,
                                     "MSIL1C",
                                     date_str,
                                     "N" + str(orbit).zfill(4),
                                     "R" + str(version_orbit).zfill(3),
                                     self.tile,
                                     date_str + ".SAFE"])
            product_path = os.path.join(self.root, product_name)
            metadata_path = os.path.join(product_path, "MTD_MSIL1C.xml")
        else:
            date_str = self.date.strftime("%Y%m%d-%H%M%S")
            product_name = "_".join([platform_specifier,
                                     date_str + "-000",
                                     "L2A",
                                     self.tile,
                                     random.choice("DC"),
                                     "V" + str(version_orbit) + "-" + str(version_orbit)])
            product_path = os.path.join(self.root, product_name)
            metadata_path = os.path.join(product_path, product_name + "_MTD_ALL.xml")

        self.prod = product_path
        self.mtd = metadata_path
        FileSystem.create_directory(product_path)
        TestFunctions.touch(metadata_path)
        return Product.MajaProduct.factory(self.prod)


class L2Generator(ProductGenerator):
    """
    Class to create a dummy L2A product
    """
    def __init__(self, root, date=random_date(), tile=None, platform=random_platform(product_level="L2A")):
        super(L2Generator, self).__init__(root, date, tile, platform)

    def generate(self, **kwargs):
        import random
        from StartMaja.Chain import Product
        from StartMaja.Common import TestFunctions, FileSystem
        platform_specifier = random.choice(self.platform_options["L2A"][self.platform])
        ms = kwargs.get("ms", random.randint(0, 999))
        version = kwargs.get("version", random.randint(0, 9))
        date_str = self.date.strftime("%Y%m%d-%H%M%S-") + str(ms).zfill(3)
        product_name = "_".join([platform_specifier,
                                 date_str,
                                 "L2A",
                                 self.tile,
                                 random.choice("DC"),
                                 "V" + str(version) + "-" + str(version)])
        product_path = os.path.join(self.root, product_name)
        self.prod = product_path
        metadata_path = os.path.join(product_path, product_name + "_MTD_ALL.xml")
        self.mtd = metadata_path
        FileSystem.create_directory(product_path)
        TestFunctions.touch(metadata_path)
        return Product.MajaProduct.factory(self.prod)
