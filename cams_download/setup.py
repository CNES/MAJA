#!/usr/bin/env python
#
# (C) Copyright 2012-2013 ECMWF.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
# In applying this licence, ECMWF does not waive the privileges and immunities 
# granted to it by virtue of its status as an intergovernmental organisation nor
# does it submit to any jurisdiction.
#

from setuptools import setup, find_packages

import ecmwfapi


setup(
    name="ecmwf-api-client",
    version=ecmwfapi.__version__,
    description=ecmwfapi.__doc__,
    author="ECMWF",
    author_email="software.support@ecmwf.int",
    url="https://software.ecmwf.int/stash/projects/PRDEL/repos/ecmwf-api-client/browse",

    # entry_points={
    #     "console_scripts": [
    #         "mars = XXX:main",
    #     ],
    # },

    packages=find_packages(),
    zip_safe=False,
)
