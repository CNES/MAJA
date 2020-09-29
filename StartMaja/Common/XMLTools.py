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


from __future__ import print_function
import os
import shutil
import logging

log = logging.getLogger(__name__)


def indent(elem, level=0):
    """
    Source: https://stackoverflow.com/questions/749796/pretty-printing-xml-in-python
    :param elem: The elem to indent
    :param level: The level of indentation
    :return: The indented element
    """
    # TODO Unittest?
    i = "\n" + level * "  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level + 1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i
    return elem


def write_xml(root, filepath):
    """
    Write a pretty-printed xml to the given filepath.
    :param root: The root xml-Element
    :param filepath: The filepath to write it to.
    :return: Writes the given xml file with proper indentation.
    """
    from xml.etree import ElementTree
    assert os.path.isdir(os.path.dirname(filepath))
    indent(root)
    tree = ElementTree.ElementTree(root)
    tree.write(filepath, encoding="UTF-8", xml_declaration=True)


def get_xpath(xml, xpath):
    """
    Return all xpath results of from xml-file. Strips of ALL namespaces before parsing.
    :param xml: The full path to the xml file
    :param xpath: The xpath to be searched for.
    :return:
    """
    from xml.etree import ElementTree
    it = ElementTree.iterparse(xml)
    for _, el in it:
        if '}' in el.tag:
            el.tag = el.tag.split('}', 1)[1]  # strip all namespaces
    root = it.root
    return root.findall(xpath)


def get_single_xpath(xml, xpath):
    """
    Get a single xpath element
    :param xml: The full path to the xml file
    :param xpath: The xpath to be searched for.
    :return:
    """
    return get_xpath(xml, xpath)[0].text
