#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jul  5 17:42:44 2018

@author: akynos
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
