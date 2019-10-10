#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES, CS-SI, CESBIO - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>,
Project:        Start-MAJA, CNES
"""


def touch(path):
    """
    Create a new dummy-file of given path
    :param path: The full path to the file
    :return:
    """
    import os
    with open(path, 'a'):
        os.utime(path, None)
