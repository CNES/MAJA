# -*- coding: cp1252 -*-
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
#
###########################################################################################################
#                                                                                                          #
#                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         #
#                             o                                                                            #
#                          o                                                                               #
#                        o                                                                                 #
#                      o                                                                                   #
#                     o       ooooooo       ooooooo    o         o      oo                                 #
#    o              o       o        o     o       o   o         o     o   o                               #
#      o           o       o          o   o         o  o         o    o      o                             #
#        o        o       o           o   o         o  o         o    o        o                           #
#         o      o        o      oooo     o         o  o         o   o           o                         #
#          o    o          o              o         o  o         o   o           o                         #
#           o  o            o             o         o  o o      o   o          o                           #
#            oo              oooooooo    o         o   o  oooooo   o      oooo                             #
#                                                     o                                                    #
#                                                     o                                                    #
#                                                    o                            o                        #
#                                                    o            o      oooo     o   o      oooo          #
#                                                   o             o         o    o    o         o          #
#                                                   o            o       ooo     o   o       ooo           #
#                                                               o       o       o   o          o           #
#                                                               ooooo   oooo    o   ooooo  oooo            #
#                                                                              o                           #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# MOD : VERSION : 1.0  : 01 avril 2010 : Creation                                                          #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################

# ---------#
# Imports #
# ---------#
import getopt
import sys
import os
import shutil
import numpy as np
import os
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)


def open_and_multiply(input_file, output_file, coeff):
    x = np.fromfile(input_file, np.float32)
    o = x * np.float32(coeff)
    o.tofile(output_file)


def multiply_and_add(input_file, output_file, coeff):
    if os.path.exists(output_file):
        x = np.fromfile(input_file, np.float32)
        o = x * np.float32(coeff)
        y = np.fromfile(output_file, np.float32)
        z = np.add(y, o)
        z.tofile(output_file)
    else:
        open_and_multiply(input_file, output_file, coeff)


def main(argv):
    try:
        opt, args = getopt.getopt(argv, "h:i:o:c:", ["help", "input=", "output", "coeff"])
    except getopt.GetoptError as err:
        LOGGER.debug(str(err))
        usage()
        sys.exit(2)
    if len(args) == 0:
        usage()
        sys.exit(2)
    input_file = ""
    output_file = "out"
    coeff = 0
    for o, a in opt:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-i", "--input"):
            if os.path.isfile(a) != True:
                LOGGER.debug("\nERROR --input: " + a + " is not a file.")
                sys.exit(2)
            else:
                # Affectation repertoire des fichiers de description
                input_file = a
        elif o in ("-o", "--output"):
            # Affectation repertoire des fichiers de description
            output_file = a
        elif o in ("-c", "--coeff"):
            coeff = float(a)
        else:
            assert False, "unhandled option"

    open_and_multiply(input_file, output_file, coeff)


def usage():
    usage = """
    ####################################################################################
    #                         HELP--LutTranspose                                       #
    ####################################################################################
    #                                                                                  #
    #                                                                                  #
    ####################################################################################
    # -h --help         Affiche l'aide                                                 #
    # -i --input        Chemin vers le fichier d'entré                                #
    # -o --output       Chemin vers le fichier de sortie                               #
    # -c --coeff        Coeff to multiply                                              #
    ####################################################################################
    """
    LOGGER.debug(usage.decode("latin-1"))


# ####################
# Execution du main #
# ####################
if __name__ == "__main__":
    main(sys.argv[1:])
