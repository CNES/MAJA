# -*- coding: utf-8 -*-
#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""
###################################################################################################

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################
orchestrator.tests.common.xml_tools -- shortdesc

orchestrator.tests.common.xml_tools is a description

It defines classes_and_methods


###################################################################################################
"""

from lxml import etree as ET
from orchestrator.common.maja_exceptions import MajaProcessingError
from orchestrator.common.system_utils import launch_command
from lxml import objectify
from .maja_exceptions import MajaIOError
from orchestrator.common.logger.maja_logging import configure_logger
import orchestrator.common.maja_utils as maja_utils
import orchestrator.common.file_utils as file_utils
import os,copy

LOGGER = configure_logger(__name__)


def open_xml_file(xml_filepath):
    """
    Open the given filepath and return the dom if not None
    :param xml_filepath:
    :return:
    """
    with open(xml_filepath, 'r') as f:
        dom = ET.parse(f)

    if dom is not None:
        return dom
    else:
        raise MajaIOError("Can't get the node of given xml file {}".format(xml_filepath))


def save_to_xml_file(root_node, xml_filepath):
    LOGGER.debug("Writing %s", xml_filepath)
    tree = ET.ElementTree(root_node)

    f = open(xml_filepath, "w")
    f.write(ET.tostring(tree, pretty_print=True, xml_declaration=True, method="xml",
                        encoding="UTF-8").decode())
    f.close()
    LOGGER.debug("%s Created", xml_filepath)


def get_root_xml(xml_filepath, deannotate=False):
    """
    Get the root of the given xml_filepath
    :param xml_filepath:
    :return:
    """
    dom = open_xml_file(xml_filepath)
    if deannotate:
        return deannotate_xml(dom.getroot())
    else:
        return dom.getroot()


def get_only_value(dom, xpath_request, namespaces={}, check=False):
    """
    Get the value requested by the given xpath
    If only one value is found, return it,
    Else raise an error

    :param dom:
    :type dom:
    :param xpath_request:
    :type xpath_request:
    :param namespaces:
    :type namespaces:
    """
    if namespaces != {}:
        xpath_list_result = dom.xpath(xpath_request, namespaces=namespaces)
    else:
        xpath_list_result = dom.xpath(xpath_request)

    if len(xpath_list_result) == 1 and xpath_list_result[0] is not None:
        return xpath_list_result[0]
    elif len(xpath_list_result) > 1:
        LOGGER.error("Fail to get the only value for %s in given dom. There several results",
                     xpath_request)
        raise MajaIOError("Fail to get the only value for %s in given dom. There several results" % xpath_request)
    else:
        # allows to check if the requested xpath is in the node
        if check:
            return None
        else:
            LOGGER.error("No result found for %s in the given dom.", xpath_request)
            raise MajaIOError("No result found for %s in the given dom." % xpath_request)


def get_attribute(dom, xpath_request, attribute, namespaces={}, check=False):
    node = get_only_value(dom, xpath_request, namespaces, check)
    return node.get(attribute)


def get_attribute_values(dom, xpath_request, attribute, namespaces={}):
    l_values = []
    nodes = get_all_values(dom, xpath_request, namespaces)
    for node in nodes:
        l_values.append(node.get(attribute))
    return l_values


def get_list_of_string_child_values(dom, relativePath, namespaces={}):
    l_valuelist = []
    xnode = get_only_value(dom, relativePath, namespaces=namespaces)
    # Loops on all child node of the main node 'xnode'
    for child in list(xnode):
        value = child.text
        # Add space to separate last value with the first value of the following value in the next line
        l_valuelist.append(value + " ")
    return l_valuelist


def get_all_values(dom, xpath_request, namespaces={}):
    """
    Get the values requested by the given xpath

    :param dom:
    :type dom:
    :param xpath_request:
    :type xpath_request:
    :param namespaces:
    :type namespaces:
    """
    if namespaces != {}:
        xpath_list_result = dom.xpath(xpath_request, namespaces=namespaces)
    else:
        xpath_list_result = dom.xpath(xpath_request)

    return xpath_list_result


def translate_xsl(source, stylesheet):
    command_line = "xsltproc --output " + source + " " + stylesheet + " " + source

    LOGGER.info("Running %s", command_line)

    # TODO: TBC use append env or manage entire env for each COTS ?
    status = launch_command(command_line)

    # TODO: TBC MOve status to post ?
    # TODO: see status management by system command executor
    if status != 0:
        raise MajaIOError("Error running {}. Exit code {}".format(command_line, status))


def can_load_file(filepath):
    ret = True
    try:
        with open(filepath, 'r') as f:
            dom = ET.parse(f)
    except Exception as e:
        ret = False
    return ret


def deannotate_xml(root):
    """
    Remove the namespaces of the current root.
    See :
    https:#stackoverflow.com/questions/18159221/remove-namespace-and-prefix-from-xml-in-python-using-lxml
    http:#www.goodmami.org/2015/11/04/python-xpath-and-default-namespaces.html
    :param root:
    :return:
    """
    ####
    for elem in root.iter():
        if not hasattr(elem.tag, 'find'):
            continue  # (1)
        i = elem.tag.find('}')
        if i >= 0:
            elem.tag = elem.tag[i + 1:]
    objectify.deannotate(root, xsi=True, cleanup_namespaces=True)

    return root
    ####


def as_bool(val):
    return (val in ('true', '1', "True"))


def as_float_list(val):
    return [float(f) for f in val.split()]


def as_int_list(val):
    return [int(f) for f in val.split()]


def as_string_list(val):
    return val.split()


def get_xml_string_value(dom, xpath_request, namespaces={}, check=False):
    """
    Return the requested value in string format
    :param dom:
    :param xpath_request:
    :param namespaces:
    :param check:
    :return:
    """
    node = get_only_value(dom, xpath_request, namespaces, check=check)
    out = ""
    if node is not None:
        out = str(node.text)
    return out


def get_xml_float_value(dom, xpath_request, namespaces={}, check=False):
    """
    Return the requested value in float format
    :param dom:
    :param xpath_request:
    :param namespaces:
    :param check:
    :return:
    """
    # TODO: add try except for type conversion
    return float(get_xml_string_value(dom, xpath_request, namespaces, check=check))


def get_xml_int_value(dom, xpath_request, namespaces={}):
    """
    Return the requested value in float format
    :param dom:
    :param xpath_request:
    :param namespaces:
    :param check:
    :return:
    """
    # TODO: add try except for type conversion
    return int(get_xml_string_value(dom, xpath_request, namespaces))


def get_xml_bool_value(dom, xpath_request, namespaces={}):
    """
    Return the requested value in float format
    :param dom:
    :param xpath_request:
    :param namespaces:
    :param check:
    :return:
    """
    # TODO: add try except for type conversion
    value = get_xml_string_value(dom, xpath_request, namespaces)

    return as_bool(value)


def get_list_value_sorted_by_attribut(dom, root_xpath, xpath_before, xpath_after, root_attribut, sub_attribute, offset):
    l_NumberOfNodes = len(get_all_values(dom, root_xpath + xpath_before))
    # If the RootNodeAttributName attribut doesn't exist in the XML file, so set the RootNodeAttributName = ""
    l_Count = l_NumberOfNodes
    l_ListOfStrings = []
    if root_attribut is not None and len(root_attribut) != 0:
        l_Count = int(get_attribute(dom, root_xpath, root_attribut))
        if l_Count != l_NumberOfNodes:
            raise MajaProcessingError(
                "In the XML file in the <" +
                root_xpath +
                "> node, the 'count' value (" +
                str(l_Count) +
                ") is different than the number of sub-nodes (" +
                str(l_NumberOfNodes) +
                ") !!")
        # Start to "1"
        for i in range(offset, l_Count + offset):
            xpath = root_xpath + xpath_before + "[@" + sub_attribute + "='" + str(i) + "']" + xpath_after
            l_ListOfStrings.append(get_only_value(dom, xpath).text)
    return l_ListOfStrings


def get_string_list_from_nodes(dom, root_xpath, namespaces={}):
    return [str(n.text) for n in get_all_values(dom, root_xpath, namespaces=namespaces)]


def get_float_list_from_nodes(dom, root_xpath, namespaces={}):
    return [float(n.text) for n in get_all_values(dom, root_xpath, namespaces=namespaces)]


"""
 Setters part
"""


def set_value(dom, xpath_request, value, namespaces={}, check=False):
    if namespaces != {}:
        xpath_list_result = dom.xpath(xpath_request, namespaces=namespaces)
    else:
        xpath_list_result = dom.xpath(xpath_request)

    if len(xpath_list_result) == 1:
        if isinstance(value, bool):
            xpath_list_result[0].text = str(value).lower()
        else:
            xpath_list_result[0].text = str(value)
    elif len(xpath_list_result) > 1:
        LOGGER.error("Fail to get the only value for %s in given dom. There several results",
                     xpath_request)
        raise MajaIOError("Fail to get the only value for %s in given dom. There several results" % xpath_request)
    else:
        # allows to check if the requested xpath is in the node
        if check:
            return
        else:
            LOGGER.error("No result found for %s in the given dom.", xpath_request)
            raise MajaIOError("No result found for in the given dom")


def set_int_value(dom, xpath_request, value, namespaces={}, check=False):
    set_value(dom, xpath_request, value, namespaces, check)


def set_double_value(dom, xpath_request, value, namespaces={}, check=False):
    set_value(dom, xpath_request, value, namespaces, check)


def set_string_list_value_sorted_by_attribute(dom, ListOfValues, RootXPath, SubNodeName, RootNodeAttributName,
                                              RootSubNodeAttributName,
                                              OffsetAttributValue, p_attributs={}, namespaces={}, check=False):
    # Ex:
    #  RootXPath   = #Image_Information/List_of_Bands
    #  SubNodeName = Band
    # RootXPath must be exist

    # Selects the node with the root path
    xnode = get_only_value(dom, RootXPath)

    l_count = len(ListOfValues)
    main_node = xnode
    attrib = main_node.get(RootNodeAttributName, "")
    # ---------------------------------------------------------------------------------------------
    # If the attribute variable not empty, set the dedicated value
    main_node.set(RootNodeAttributName, str(l_count))

    for i in range(0, l_count):
        # ---------------------------------------------------------------------------------------------
        # Set the value to the dedicated node
        sub_node = ET.Element(SubNodeName)
        sub_node.set(RootSubNodeAttributName, str(i + OffsetAttributValue))
        # ---------------------------------------------------------------------------------------------
        # Loops on creates additionnals attributs
        if len(p_attributs) != 0:
            for itkey, itvalue in p_attributs.items():
                sub_node.set(itkey, itvalue)
        sub_node.text = str(ListOfValues[i])
        main_node.append(sub_node)


def replace_node(dom_src, xpath_src, dom_dest, xpath_det, check_if_present=False):
    # Selects the node with the root path
    xnodesource = get_only_value(dom_src, xpath_src, check=check_if_present)
    xnodedest = get_only_value(dom_dest, xpath_det, check=check_if_present)

    if xnodedest is not None:
        if xnodesource is not None:
            # Insert new node after
            xnodedest.getparent().insert(xnodedest.getparent().index(xnodedest), copy.copy(xnodesource))
        elif check_if_present:
            LOGGER.debug("Node : " + xpath_src + " is not available in input dom")
        # Delete previous template node
        try:
            xnodedest.getparent().remove(xnodedest)
            pass
        except BaseException:
            raise MajaIOError("Error while delete the xml first node '" + xpath_det +
                              "' after copy sibling node in the xml file! Details: ")
    elif check_if_present:
        LOGGER.debug("Node : " + xpath_det + " is not available in output dom")
        if xnodesource is None:
            LOGGER.debug("Node : " + xpath_src + " is not available in input dom")
            return


def remove_node(dom_src, xpath_src):
    # Selects the node with the root path
    xnodesource = get_only_value(dom_src, xpath_src, check=True)
    # Delete previous template node
    try:
        if xnodesource is not None:
            xnodesource.getparent().remove(xnodesource)
    except BaseException:
        raise MajaIOError("Error while delete the xml first node '" + xpath_src +
                          "' after copy sibling node in the xml file! Details: ")


def rename_node(dom_src, xpath_src, new_name):
    # Selects the node with the root path
    xnodesource = get_only_value(dom_src, xpath_src)
    # Rename node
    xnodesource.tag = new_name


def remove_child(dom_src, xpath_src):
    # Selects the node with the root path
    xnodesource = get_only_value(dom_src, xpath_src)
    # Delete previous template node
    try:
        for child in xnodesource:
            xnodesource.remove(child)
    except BaseException:
          MajaIOError("Error while delete the xml first node '" + xpath_src +
                          "' after copy sibling node in the xml file! Details: ")


def copies_to_child(DocSource, XPathSource, DocDestination, XPathDestination):
    # ---------------------------------------------------------------------------------------------
    xnodessource = get_all_values(DocSource, XPathSource)
    # ---------------------------------------------------------------------------------------------
    xnodedest = get_only_value(DocDestination, XPathDestination)
    # ---------------------------------------------------------------------------------------------
    # Loops and copy nodes
    for node in xnodessource:
        xnodedest.append(copy.copy(node))


def copy_to_sibling(DocSource, XPathSource, DocDestination, XPathDestination):
    # ---------------------------------------------------------------------------------------------
    xnodesource = get_only_value(DocSource, XPathSource)
    # ---------------------------------------------------------------------------------------------
    xnodedest = get_only_value(DocDestination, XPathDestination)
    contentdiv = xnodedest.getparent()
    contentdiv.insert(contentdiv.index(xnodedest) + 1, xnodesource)


def copy_to_sibling_inplace(DocSource, XPathSource, XPathDestination):
    # ---------------------------------------------------------------------------------------------
    xnodesource = get_only_value(DocSource, XPathSource)
    # ---------------------------------------------------------------------------------------------
    xnodedest = get_only_value(DocSource, XPathDestination)
    contentdiv = xnodedest.getparent()
    contentdiv.insert(contentdiv.index(xnodedest) + 1, xnodesource)

def extract_nodes(DocSource, XPathSource):
    outnode = ET.Element("root")
    copies_to_child(DocSource, XPathSource, outnode, ".")
    if len(outnode):
        return outnode
    return None

def set_attribute(dom_src, xpath_src, attribute, value):
    # Selects the node with the root path
    xnodesource = get_only_value(dom_src, xpath_src)
    # Delete previous template node
    xnodesource.set(attribute, value)

def add_child(dom, xpath_src, name, value):
    """
    Adds a child element "<name>value</name>" to the node xpath_src
    """
    node = ET.Element(name)
    node.text = value
    xnodesource = get_only_value(dom, xpath_src)
    xnodesource.append(node)

"""
    Validation parts
"""


def get_schema_location_attrib(dom):
    lXsdFilename = None
    for attr in list(dom.items()):
        if "schemaLocation" in attr[0]:
            lXsdFilename = attr[1]
            break
    if lXsdFilename is None:
        for attr in list(dom.items()):
            if "noNamespaceSchemaLocation" in attr[0]:
                lXsdFilename = attr[1]
                break
    return lXsdFilename


def get_schema_location(dom):
    lXsdFilename = None
    for attr in list(dom.items()):
        if "schemaLocation" in attr[0]:
            lXsdFilename = attr[1]
            break
    if lXsdFilename is None:
        for attr in list(dom.items()):
            if "noNamespaceSchemaLocation" in attr[0]:
                lXsdFilename = attr[1]
                break
    if lXsdFilename is None or len(lXsdFilename) == 0:
        return None

    # --------------------------------------
    # Ex: lXsdFilename = xsi:schemaLocation="http:#eop-cfi.esa.int/CFI
    # ../../Schemas/Venus/VE_GIP_CKQLTL_QuicklookTool.xsd"
    # Extract the name of the Xsd file
    lXsdFilename = str(lXsdFilename)
    listString = maja_utils.split_filename(lXsdFilename, ' ', False)
    # Get the Latest value
    lTheXsdFilename = ""
    nbStrings = len(listString)
    # Loop under strings
    for i in range(0, nbStrings):
        lTheXsdFilename = listString[i]
        lTheXsdFilename = file_utils.convert_to_unix_slash(
            lTheXsdFilename)  # to be sur (because header from CNES data are generated from Window platform)
        if "xsd" in os.path.splitext(lTheXsdFilename)[1]:
            lTheXsdFilename = os.path.basename(lTheXsdFilename)

    return lTheXsdFilename


def check_xml(xml_file, schema_location=None, xsd_file=None):
    """
    #TODO: to be implemented
    :param xml_file:
    :param xsd_file:
    :return:
    """

    # -----------------------
    # Load the xml file
    dom_node = get_root_xml(xml_file, deannotate=True)

    if xsd_file is None:
        if schema_location is None:
            raise MajaIOError("Either schema path or schema location needed to validate xml")
        # --------------------------------------
        # Get the schema location
        lXsdFilename = get_schema_location(dom_node)
        if lXsdFilename is None:
            raise MajaIOError(
                "Error while reading the attribute 'xsi:schemaLocation' or 'xsi:noNamespaceSchemaLocation' " +
                "in the xml file " + xml_file + ". Impossible to check the file with its schema!")

        LOGGER.debug("The Xsd Filename <" + lXsdFilename + "> had been detected in the xml file <" + xml_file + ">.")

        # --------------------------------------
        # Get the full path of the schemalocation
        lFullXsdFilename = os.path.join(schema_location, lXsdFilename)
    else:
        lFullXsdFilename = xsd_file

    # --------------------------------------
    # Check the Xsd to the xml file
    result = launch_command("xmllint --noout " + xml_file + " --schema " + lFullXsdFilename)
    if result != 0:
        raise MajaIOError(
            "The xml file <" + xml_file + "> is not conform with its schema <" + lFullXsdFilename + "> ! ")
    # --------------------------------------
    LOGGER.debug("The XML file <" + xml_file + "> is conform with its schema.")

    return True

def sort_children(node, xpath_sort_key ):
    """
    take all children from input node and sort them using 'xpath_sort_key'
    """
    a = sorted(node, key=lambda item: item.xpath(xpath_sort_key))
    for item in a:
      node.remove(item)
    node.extend(a)
