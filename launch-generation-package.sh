#!/bin/bash
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
#
############################################################################################################
#                                                                                                          #
#                                        __  __    __     ____   __                                        #
#                                       (  \/  )  /__\   (_  _) /__\                                       #
#                                        )    (  /(__)\ .-_)(  /(__)\                                      #
#                                       (_/\/\_)(__)(__)\____)(__)(__)                                     #
#                                                                                                          #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 2.1.0 : FA : LAIG-FA-MAJA-2296-CNES : 26 juin 2017 : Ajout prerequis 'gzip'                    #
# VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 1 mars 2017 : Modif script pour lancement nightly TU     #
# VERSION : 2.0.0 : FA : LAIG-FA-MAJA-2144-CNES : 20 fevrier 2017 : Correction de l'option --tar(non --tgz)#
#                                            + Controle de la presence de cmake en testant le lancement du #
#                                                       programme (plus de test sur le paquet rpm 'cmake') #
# VERSION : 1.0.0 : DM : LAIG-DM-MAC-151090-CS : 28 novembre 2016 : Renommage de MACCS a MAJA              #
# VERSION : 1.0.0 : FA : LAIG-FA-MAC-148483-CS : 13 octobre 2016 : Ajout suppression du repertoire install #
#     avant génération. Remove install directory, to be sur remove alls deprecated files (ex: GIPPs, etc.) #
# VERSION : 5-1-2 : FA : LAIG-FA-MAC-1890-CNES : 9 septembre 2016 : revert changements dans les            #
#                                                              arborescences d'install TGZ                 #
# VERSION : 5-1-1 : FA : LAIG-FA-MAC-1890-CNES : 5 septembre 2016 : revert changements dans les            #
#                                                              arborescences d'install TGZ                 #
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 22 juin 2016 : Renommage avant maja                       #
#                                                               Simplification procedure generation RPMR   #
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 6 juin 2016 : Modification passage des COTS en parametre  #
# VERSION : 5-1-0 : DM : LAIG-DM-MAC-145080-CS : 24 mai 2016 : Modifications packaging binaires            #
# VERSION : 5-0-0 : DM : LAIG-DM-MAC-1636-CNES : 11 janvier 2016 : Portage pour RHEL 7.                    #
# VERSION : 4-4-0 : DM : LAIG-DM-MAC-1283-CNES : 06 juin 2015 : Plugins SENTINEL2                          #
# VERSION : 4-4-0 : DM : LAIG-DM-MAC-1284-CNES : 06 juin 2015 : Plugins LANDSAT8                           #
# VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     #
# VERSION : 3-3-0 : DM : LAIG-DM-MAC-744-CNES : 17 fevrier 2014 : Remplacement de S2N_ par S2__            #
#                                                                               (plugin SENTINEL2)         #
# VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         #
# VERSION : 3-1-0 : FA : LAIG-FA-MAC-642-CNES : 29 octobre 2013 : Creation automatique du fichier de       #
#                                               controle sha256 du paquet binaire genere                   #
# VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 29 octobre 2013 : Mise a jour de l'utilisation de la       #
#                                          version. Deploiement sur 2 digits avec mise a jour des scripts  #
# VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Modification pour la prise en compte de la DM #
#                                                            du MACCS generique (PM LAIG-NT-MAC-0239-CNES) #
# VERSION : 2-3-0 : DM : LAIG-DM-MAC-399-CNES : 28 aout 2012 : Modification suite a l'install relocatable  #
# VERSION : 2-3-0 : DM : LAIG-DM-MAC-367-CNES : 22 aout 2012 : Modification pour DM 367                    #
# VERSION : 2-3-0 : FA : LAIG-FA-MAC-378-CNES : 24 aout 2012 : Le .sh et le rpm sont stocke dans un .tar   #
# VERSION : 2-3-0 : DM : LAIG-DM-MAC-367-CNES : 7 aout 2012 : Modification pour DM 367                     #
# VERSION : 2-3-0 : FA : LAIG-FA-MAC-397-CNES : 7 aout 2012 : Ajout commentaire abandon generation         #
# VERSION : 2-2-0 : FA : LAIG-FA-MAC-317-CNES : 26 juin 2012 : Suppression OpenJpeg pr eviter conflit OTB  #
# VERSION : 2-1-0 : FA : LAIG-FA-MAC-335-CNES : 30 mai 2012 : Verification de l'install de la lib GD.      #
# VERSION : 1-0-0-2 : FA : 284 : 2 juin 2011 : Correction pour la gestion des versions des Chaines           #
#                                                                                                          #
# VERSION : 1-0-0-2 : FA : 212 : 2 juin 2011 : Possibilite de preciser le nombre de thread utilise pour      #
#                                              pour generer les chaines MACCS                              #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 octobre 2010 : Creation                                          #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################
CZERO="\033[0m"
CRED="\033[31m"
CBLUE="\033[34m"
CGREEN="\033[32m"
CCYAN="\033[36m"
CBLACK="\033[30m"
CPURPLE="\033[35m"
CYELLOW="\033[33m"
TBOLD=$(tput bold)
TNORMAL=$(tput sgr0)

COLOR=${CBLUE}
NOCOLOR=${CZERO}
ERRCO=${CRED}
WARCO=${CYELLOW}
PASSEDCO=${CGREEN}

# ---------------------------------------------------------------------------------------------------------------------------
# Pre requested
# MAJA COTS
#



# ---------------------------------------------------------------------------------------------------------------------------
function check_basic_command
{
    ${1} --version >& /dev/null
    exit_code=$?
    if [ "-${exit_code}" != "-0" ]; then
        echo  -e ${ERRCO}" * Check program '${1}' installation failed!"${NOCOLOR}
        echo  -e ${ERRCO}"   -> Please install the basic command ${1}."${NOCOLOR}
        exit ${exit_code}
    else
        echo  -e ${COLOR}" * Check program '${1}' installation ${TBOLD}${PASSEDCO}passed${COLOR}${TNORMAL}."${NOCOLOR}
    fi
    return ${exit_code}
}


check_basic_command basename
check_basic_command readlink
check_basic_command dirname
check_basic_command getopt
check_basic_command lsb_release
check_basic_command gzip

SHELL_BASE_DIR=$(dirname $(readlink -f ${BASH_SOURCE}))

# ---------------------------------------------------------------------------------------------------------------------------
# VERSION of MAJA chains and MAJA COTS
# Note: Version values set by a specific script. Set MAJA_VERSION and MAJA_COTS_VERSION
. ${SHELL_BASE_DIR}/Versions.sh

# ---------------------------------------------------------------------------------------------------------------------------
MAJA_VERSION_MAJOR=`echo ${MAJA_VERSION} | awk -F. '{print $1}' `
MAJA_VERSION_MINOR=`echo ${MAJA_VERSION} | awk -F. '{print $2}' `
MAJA_VERSION_PATCH=`echo ${MAJA_VERSION} | awk -F. '{print $3}' `
# Version for install directory (2 digits)
MAJA_VERSION_FOR_SW_COMPONENT=${MAJA_VERSION_MAJOR}.${MAJA_VERSION_MINOR}

# ---------------------------------------------------------------------------------------------------------------------------
MAJA_COTS_VERSION_MAJOR=`echo ${MAJA_COTS_VERSION} | awk -F. '{print $1}' `
MAJA_COTS_VERSION_MINOR=`echo ${MAJA_COTS_VERSION} | awk -F. '{print $2}' `
# Version for install directory (2 digits)
MAJA_COTS_VERSION_FOR_SW_COMPONENT=${MAJA_COTS_VERSION_MAJOR}.${MAJA_COTS_VERSION_MINOR}


# ---------------------------------------------------------------------------------------------------------------------------
NomProc=`basename $0`
# Default value
NBTHREADS="4"
ROOT_DIRECTORY=/opt
TARGET_PACKAGE_NAME="maja"
TARGET_DEVEL_NAME=""
# DEFAULT values
CPACK_BINARY_RPM=OFF
CPACK_BINARY_TGZ=OFF
ENABLE_SPECTRAL_CAMERA_VENUS=OFF
ENABLE_SPECTRAL_CAMERA_SENTINEL2=OFF
ENABLE_SPECTRAL_CAMERA_SENTINEL2_GPP=OFF
ENABLE_SPECTRAL_CAMERA_SENTINEL2_MUSCATE=OFF

ENABLE_SPECTRAL_CAMERA_FORMOSAT_MUSCATE_PROTO=OFF
ENABLE_SPECTRAL_CAMERA_SPOT4_MUSCATE_PROTO=OFF
ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE_PROTO=OFF
ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE=OFF

ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE_PROTO=OFF
ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE=OFF
ENABLE_SPECTRAL_CAMERA_LANDSAT8=OFF

usage ()
{
    echo -e ${COLOR}"${TBOLD}  Generation of MAJA chains binaries V${MAJA_VERSION}  using COTS V${MAJA_COTS_VERSION}${TNORMAL}"${NOCOLOR}
    echo -e ${COLOR}"            "${NOCOLOR}
    echo -e ${COLOR}"  Usage : $NomProc "${NOCOLOR}
    echo -e ${COLOR}"${TBOLD}    - package options:${TNORMAL}"${NOCOLOR}
    echo -e ${COLOR}"               -r, --rpm                   Generates a RPM package."${NOCOLOR}
    echo -e ${COLOR}"               -t, --tar                   Generates a TAR (tarball) package."${NOCOLOR}
    echo -e ${COLOR}"${TBOLD}    - camera options:${TNORMAL}"${NOCOLOR}
    echo -e ${COLOR}"               -a, --all                   Enable all spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --venus                     Enable VENUS spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --sentinel2                 Enable SENTINEL2 spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --sentinel2-gpp             Enable SENTINEL2_GPP spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --sentinel2-muscate         Enable SENTINEL2_MUSCATE spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --formosat-muscate-proto    Enable FORMOSAT_MUSCATE_PROTO spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --spot4-muscate-proto       Enable SPOT4_MUSCATE_PROTO spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --landsat-muscate           Enable LANDSAT_MUSCATE spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --landsat-muscate-proto     Enable LANDSAT_MUSCATE_PROTO spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --landsat8                  Enable LANDSAT8 spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --landsat8-muscate          Enable LANDSAT8_MUSCATE spectral camera."${NOCOLOR}
    echo -e ${COLOR}"               --landsat8-muscate-proto    Enable LANDSAT8_MUSCATE_PROTO spectral camera."${NOCOLOR}
    echo -e ${COLOR}"${TBOLD}    - building options:${TNORMAL}"${NOCOLOR}
    echo -e ${COLOR}"               -p, --prefix                Set a specific MAJA root prefix directory to find the MAJA Cots directory, necessary to build MAJA."${NOCOLOR}
    echo -e ${COLOR}"                                           Default value: ${ROOT_DIRECTORY}"${NOCOLOR}
    echo -e ${COLOR}"                                           For example, for the '--prefix' set to '${ROOT_DIRECTORY}', the MAJA COTS shall be install in the ${ROOT_DIRECTORY}/maja/cots/${MAJA_COTS_VERSION_FOR_SW_COMPONENT} directory"${NOCOLOR}
    echo -e ${COLOR}"               -n, --nbthreads             NbThreads used for compilation"${NOCOLOR}  
    echo -e ${COLOR}"                                           Default value: ${NBTHREADS}"${NOCOLOR}
    echo -e ${COLOR}"               -d, --devel                 Build the maja 'devel' package. Default, build the maja package."${NOCOLOR}
    echo -e ${COLOR}"                                           Use only these option for build the 'MAJA-Validation' package."${NOCOLOR}
    echo -e ${COLOR}"               --release                   Enable compilation in 'release' mode (high performances). Default value."${NOCOLOR}
    echo -e ${COLOR}"           or  --debug                     Enable compilation in 'debug' mode."${NOCOLOR}
    echo -e ${COLOR}"               --gcc                       Enable builder with 'gcc'. Default value."${NOCOLOR}
    echo -e ${COLOR}"           or  --icc                       Enable builder with 'icc' (Intel)."${NOCOLOR}
    echo -e ${COLOR}"           or  --clang                     Enable builder with 'clang'."${NOCOLOR}
    echo -e ${COLOR}"               --compiler-overrides        Set dedicated env.bash and overrides.cmake files."${NOCOLOR}
    echo -e ${COLOR}"${TBOLD}    -h, --help                             Display this help and exit${TNORMAL} "${NOCOLOR}
    exit 1
}

TEMP=`getopt -o hdrtp:n:avslfSL --long gcc,icc,clang,release,debug,devel,prefix:,rpm,tar,nbthreads:,help,all,venus,sentinel2,sentinel2-gpp,sentinel2-muscate,landsat-muscate,landsat-muscate-proto,landsat8,landsat8-muscate,landsat8-muscate-proto,formosat-muscate-proto,spot4-muscate-proto,disable-interactive,output-working-directory:,maja-development-context,cmake-buildname-site:,compiler-overrides:,enable-launches:,data:,data-tu: -- "$@"`
             
# Note the quotes around `$TEMP': they are essential!
eval set -- "$TEMP"

DISABLE_INTERACTIVE_MODE="NO"
OUTPUT_WORKING_DIR=""
ENABLE_LAUNCHES="NO" # = tu ou coverage ou cppcheck
MAJA_DEVELOPMENT_CONTEXT="NO"
SITE=""
COMMAND_CMAKE_SITE=""
BUILD_TYPE="release"
COMPILER="gcc"
SET_COMPILER_OVERRIDES="NO"
SET_COMPILER_OVERRIDES_FILENAMES="NOT_INITIALIZED"
MAJADATA_SOURCE_DIR=""

while true; do
  case "$1" in
    -d | --devel )  TARGET_DEVEL_NAME="-devel"; 
                    shift 1 ;;
    --maja-development-context ) MAJA_DEVELOPMENT_CONTEXT="YES"; shift 1 ;;
    --gcc ) COMPILER="gcc"; shift 1 ;;
    --icc ) COMPILER="icc"; shift 1 ;;
    --clang ) COMPILER="clang"; shift 1 ;;
    --release ) BUILD_TYPE="release"; shift 1 ;;
    --debug ) BUILD_TYPE="debug"; shift 1 ;;
    -p | --prefix ) ROOT_DIRECTORY="$2"; shift 2 ;;
    -r | --rpm ) CPACK_BINARY_RPM=ON; shift 1 ;;
    -t | --tar ) CPACK_BINARY_TGZ=ON; shift 1 ;;
    -a | --all ) ENABLE_SPECTRAL_CAMERA_VENUS=ON; 
                 ENABLE_SPECTRAL_CAMERA_SENTINEL2=ON;
                 ENABLE_SPECTRAL_CAMERA_SENTINEL2_GPP=ON;
                 ENABLE_SPECTRAL_CAMERA_SENTINEL2_MUSCATE=ON;
                 ENABLE_SPECTRAL_CAMERA_FORMOSAT_MUSCATE_PROTO=ON;
                 ENABLE_SPECTRAL_CAMERA_SPOT4_MUSCATE_PROTO=ON;
                 ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE_PROTO=ON;
                 ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE=ON;
                 ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE_PROTO=ON;
                 ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE=ON;
                 ENABLE_SPECTRAL_CAMERA_LANDSAT8=ON;
                 shift 1 ;;
    --venus ) ENABLE_SPECTRAL_CAMERA_VENUS=ON; shift 1 ;;
    --sentinel2 ) ENABLE_SPECTRAL_CAMERA_SENTINEL2=ON; shift 1 ;;
    --sentinel2-gpp ) ENABLE_SPECTRAL_CAMERA_SENTINEL2_GPP=ON; shift 1 ;;
    --sentinel2-muscate ) ENABLE_SPECTRAL_CAMERA_SENTINEL2_MUSCATE=ON; shift 1 ;;
    --formosat-muscate-proto ) ENABLE_SPECTRAL_CAMERA_FORMOSAT_MUSCATE_PROTO=ON; shift 1 ;;
    --landsat-muscate-proto ) ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE_PROTO=ON; shift 1 ;;
    --landsat-muscate ) ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE=ON; shift 1 ;;
    --landsat8 ) ENABLE_SPECTRAL_CAMERA_LANDSAT8=ON; shift 1 ;;
    --landsat8-muscate ) ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE=ON; shift 1 ;;
    --landsat8-muscate-proto ) ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE_PROTO=ON; shift 1 ;;
    --spot4-muscate-proto ) ENABLE_SPECTRAL_CAMERA_SPOT4_MUSCATE_PROTO=ON; shift 1 ;;
    -n | --nbthreads ) NBTHREADS="$2"; shift 2 ;;
    -h | --help ) usage; shift 1;;
    --disable-interactive ) DISABLE_INTERACTIVE_MODE="YES"; shift 1 ;;
    --enable-launches ) ENABLE_LAUNCHES="$2"; shift 2 ;;
    --data ) MAJADATA_SOURCE_DIR="$2"; shift 2 ;;
   # --data-tu ) MAJADATATU_SOURCE_DIR="$2"; shift 2 ;;
    --output-working-directory ) OUTPUT_WORKING_DIR="$2"; shift 2 ;;
    --cmake-buildname-site ) SITE="$2"; shift 2 ;;
    --compiler-overrides ) SET_COMPILER_OVERRIDES="YES"; 
                SET_COMPILER_OVERRIDES_FILENAMES="$2";
                shift 2 ;;
    -- ) shift; break ;;
    *) break ;;
   esac
done

CURRENT_PWD=$(pwd)

if [ "-${OUTPUT_WORKING_DIR}" != "-" ] ; then
    CURRENT_PWD=${OUTPUT_WORKING_DIR}
fi 
WORKING_DIR=${CURRENT_PWD}/binaries-${TARGET_PACKAGE_NAME}${TARGET_DEVEL_NAME}-${MAJA_VERSION}
MAJA_INSTALL_DIR_BASE=${CURRENT_PWD}/install-${TARGET_PACKAGE_NAME}${TARGET_DEVEL_NAME}-${MAJA_VERSION}

#        echo ""
#        echo -e ${ERRCO}"ERROR : '$1' bad parameter !"${NOCOLOR}
#        usage
#        ;;

if [ "-${ENABLE_SPECTRAL_CAMERA_VENUS}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_FORMOSAT_MUSCATE_PROTO}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE_PROTO}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE_PROTO}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_LANDSAT8}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_SPOT4_MUSCATE_PROTO}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_SENTINEL2}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_SENTINEL2_MUSCATE}" == "-OFF" ] && \
   [ "-${ENABLE_SPECTRAL_CAMERA_SENTINEL2_GPP}" == "-OFF" ] ;then
    echo -e ${ERRCO}" We must to enable a Spectral camera. See the help for see spectral camera options !"${NOCOLOR}
    usage
fi

if [ "-${CPACK_BINARY_RPM}" == "-OFF" ] && [ "-${CPACK_BINARY_TGZ}" == "-OFF" ] ;then
    echo -e ${ERRCO}" We must identify the package (rpm and/or tgz) !"${NOCOLOR}
    usage
fi

# expand to absolute PATH
ROOT_DIRECTORY=$(readlink -f ${ROOT_DIRECTORY})

#------------------------------------------------------------------------------
# Initialize var env of MAJA Cots
MAJA_COTS_INSTALL_DIR=${ROOT_DIRECTORY}/maja/cots/${MAJA_COTS_VERSION_FOR_SW_COMPONENT}
INIT_VAR_MAJA_COTS_SHELL=${MAJA_COTS_INSTALL_DIR}/init-var-cots.sh
MAJA_COTS_INCLUDE_FUNCTIONS_CHECK_SHELL=${MAJA_COTS_INSTALL_DIR}/cots-include-functions-check.sh

MAJA_SOURCE_DIR=${SHELL_BASE_DIR}

# Set the MAJA_VERSION_CHAIN formated 00.00
printf -v MAJA_VERSION_CHAIN "%02d.%02d" $MAJA_VERSION_MAJOR $MAJA_VERSION_MINOR

# TODO: BUG in CPACK RPM. The --prefix do not configure the CMAKE_INSTALL_PREFIX variable in the code
#MAJA_INSTALL_DIR=${CURRENT_PWD}/install-${TARGET_PACKAGE_NAME}-${MAJA_VERSION}  #/opt/maja/core/${MAJA_VERSION}
BASE_INSTALL_DIR_CORE=${MAJA_INSTALL_DIR_BASE}/maja/core
MAJA_INSTALL_DIR=${BASE_INSTALL_DIR_CORE}/${MAJA_VERSION_FOR_SW_COMPONENT}
BASE_DIR_FOR_UNISTALL_GENERATION_LIST_FILE=${MAJA_VERSION_FOR_SW_COMPONENT}

GENERATION_UNINSTALLFILE=${MAJA_SOURCE_DIR}/Code/Core/Scripts/generation-of-uninstall.TAR.sh
if [ ! -f ${GENERATION_UNINSTALLFILE} ]; then
    echo -e ${ERRCO}" The script file ${GENERATION_UNINSTALLFILE} doesn't exist."${NOCOLOR}
    exit 128
fi

. ${MAJA_COTS_INCLUDE_FUNCTIONS_CHECK_SHELL}

exit_code=$?
if [ "-${exit_code}" != "-0" ]; then
    echo -e ${ERRCO}" ->   Error while sourcing the script file '${MAJA_COTS_INCLUDE_FUNCTIONS_CHECK_SHELL}' ! (return ${exit_code})."${NOCOLOR}
    exit 128
fi
. ${INIT_VAR_MAJA_COTS_SHELL}

exit_code=$?
if [ "-${exit_code}" != "-0" ]; then
    echo -e ${ERRCO}" ->   Error while sourcing the script file '${INIT_VAR_MAJA_COTS_SHELL}' ! (return ${exit_code})."${NOCOLOR}
    exit 128
fi

BUILDER_ENV_BASH=""
BUILDER_OVERRIDES_CMAKE=""
BUILDNAME="MAJA_BUILD_${COMPILER}_${BUILD_TYPE}"
echo -e ${COLOR}" ->   Builder configurations ${SHELL_BASE_DIR}..."${NOCOLOR}
if [ "-${SET_COMPILER_OVERRIDES}" == "-YES" ]; then
    BUILDER_ENV_BASH=`echo "${SET_COMPILER_OVERRIDES_FILENAMES}" | awk '{split($0,a," "); print a[1]}' `
    BUILDER_OVERRIDES_CMAKE=`echo "${SET_COMPILER_OVERRIDES_FILENAMES}" | awk '{split($0,a," "); print a[2]}' `
    echo -e ${CBLUE}"      Attention: The builder 'env.bash' and 'overrides cmake' filenames set in command line parameter are '${BUILDER_ENV_BASH}' and '${BUILDER_OVERRIDES_CMAKE}'..."${NOCOLOR}
else
    BUILDER_ENV_BASH=$(find ${MAJA_SOURCE_DIR}/make -type f -name ${COMPILER}-env.bash)
    BUILDER_OVERRIDES_CMAKE=$(find ${MAJA_SOURCE_DIR}/make -type f -name ${COMPILER}-overrides.cmake)
fi
        
check_file_with_exit ${BUILDER_ENV_BASH}
check_file_with_exit ${BUILDER_OVERRIDES_CMAKE}

# Source var builders    
# Var COMPILER_NAME set in the "xxx-env.bash file"
. ${BUILDER_ENV_BASH}

# Context CS pour la validation continue
if [ "-${MAJA_DEVELOPMENT_CONTEXT}" == "-YES"  ] ; then
    echo -e ${ERRCO}" ======================================================================"${NOCOLOR}
    echo -e ${ERRCO}" ======================================================================"${NOCOLOR}
    echo -e ${ERRCO}" MODE DELELOPMENT : simple install de MAJA !"${NOCOLOR}
    echo -e ${ERRCO}" ======================================================================"${NOCOLOR}
    echo -e ${ERRCO}" ENABLE_LAUNCHES = '${ENABLE_LAUNCHES}'"${NOCOLOR}
    echo -e ${ERRCO}" ======================================================================"${NOCOLOR}
    COMMAND_CMAKE_SITE="-D SITE:STRING=${SITE}"
    echo -e ${ERRCO}" COMMAND_CMAKE_SITE : ${COMMAND_CMAKE_SITE}"${NOCOLOR}
    # TU context
    if [ "-${ENABLE_LAUNCHES}" != "-NO"  ] ; then
	BUILDNAME="TU-${ENABLE_LAUNCHES}-${COMPILER}-${BUILD_TYPE}"
	WORKING_DIR=${CURRENT_PWD}/build
        # Install maja dans le repertoire TU/install
	MAJA_INSTALL_DIR=${CURRENT_PWD}/install
        if [ "-${ENABLE_LAUNCHES}" == "-coverage"  ] ; then
	    # Install maja dans le repertoire TU/install
	    MAJA_INSTALL_DIR_BASE=${ROOT_DIRECTORY}
            BASE_INSTALL_DIR_CORE=${MAJA_INSTALL_DIR_BASE}/maja/core
	    MAJA_INSTALL_DIR=${BASE_INSTALL_DIR_CORE}/${MAJA_VERSION_FOR_SW_COMPONENT}
        fi
    else
        WORKING_DIR=${CURRENT_PWD}/maja-binaries-${COMPILER_NAME}-${BUILD_TYPE}
        # Install maja dans le repertoire ou il y a les COTS pour avoir un repertoire [...]/maja complet avec cots et core (pour le context MAJA-Validation)
        MAJA_INSTALL_DIR_BASE=${ROOT_DIRECTORY}
        BASE_INSTALL_DIR_CORE=${MAJA_INSTALL_DIR_BASE}/maja/core
	   MAJA_INSTALL_DIR=${BASE_INSTALL_DIR_CORE}/${MAJA_VERSION_FOR_SW_COMPONENT}
    fi    
fi


MAJA_BINARY_HOST_INFO=$(get_host_platform_information)
MAJA_BINARY_HOST_INFO="${MAJA_BINARY_HOST_INFO}-${BUILD_TYPE}-${COMPILER_NAME}"

# RPM variables 
OFFICIAL_RPM_NAME="${TARGET_PACKAGE_NAME}${TARGET_DEVEL_NAME}-${MAJA_VERSION}-${MAJA_BINARY_HOST_INFO}.rpm"
OFFICIAL_RPM_NAME_SHA="${OFFICIAL_RPM_NAME}.sha256"

# TAR variables
MAJA_INSTALL_PACKAGE_TAR="${TARGET_PACKAGE_NAME}${TARGET_DEVEL_NAME}-${MAJA_VERSION}-${MAJA_BINARY_HOST_INFO}"
MAJA_INSTALL_TAR_PACKAGE_TAR="${MAJA_INSTALL_PACKAGE_TAR}.tar"
MAJA_INSTALL_TAR_PACKAGE_TAR_SHA="${MAJA_INSTALL_TAR_PACKAGE_TAR}.sha256"
MAJA_INSTALL_SCRIPT_PACKAGE_TAR_BINARY_FILENAME="install${TARGET_DEVEL_NAME}.sh"
MAJA_UNINSTALL_SCRIPT_PACKAGE_TAR_BINARY_FILENAME="uninstall${TARGET_DEVEL_NAME}-${MAJA_VERSION_FOR_SW_COMPONENT}.sh"
MAJA_INSTALL_SCRIPT_PACKAGE_TAR="install.sh"
OFFICIAL_TGZ_NAME="binaries.tgz"

# Set the CPack name
MAJA_PACKAGE_NAME="${TARGET_PACKAGE_NAME}${TARGET_DEVEL_NAME}"

MAKE_PARAMETERS="-j${NBTHREADS}"

echo -e ${COLOR}"------------------------------------------------------------------------"${NOCOLOR}
echo -e ${COLOR}" * Options:"${NOCOLOR}
echo -e ${COLOR}"   - Target package:                       - ${ERRCO}${TARGET_PACKAGE_NAME}${TARGET_DEVEL_NAME}${COLOR}"${NOCOLOR}
echo -e ${COLOR}"   - Root directory:                       - ${ERRCO}${ROOT_DIRECTORY}${COLOR}"${NOCOLOR}
echo -e ${COLOR}"   - NbThreads                             - ${ERRCO}${NBTHREADS}${COLOR}"${NOCOLOR}
echo -e ${COLOR}"   - Builder:                              - ${ERRCO}${COMPILER_NAME}${COLOR}"${NOCOLOR}
echo -e ${COLOR}"   - Compilation mode:                     - ${ERRCO}${BUILD_TYPE}${COLOR}"${NOCOLOR}
echo -e ${ERRCO}"   launch with '--help' to show the options"${NOCOLOR}
echo -e ${COLOR}""${NOCOLOR}
echo -e ${COLOR}" * Spectral camera activated:"${NOCOLOR}
echo -e ${COLOR}"   - VENUS                                 - ${ENABLE_SPECTRAL_CAMERA_VENUS}"${NOCOLOR}
echo -e ${COLOR}"   - SENTINEL2                             - ${ENABLE_SPECTRAL_CAMERA_SENTINEL2}"${NOCOLOR}
echo -e ${COLOR}"   - SENTINEL2  GPP                        - ${ENABLE_SPECTRAL_CAMERA_SENTINEL2_GPP}"${NOCOLOR}
echo -e ${COLOR}"   - SENTINEL2  MUSCATE                    - ${ENABLE_SPECTRAL_CAMERA_SENTINEL2_MUSCATE}"${NOCOLOR}
echo -e ${COLOR}"   - FORMOSAT   MUSCATE PROTO              - ${ENABLE_SPECTRAL_CAMERA_FORMOSAT_MUSCATE_PROTO}"${NOCOLOR}
echo -e ${COLOR}"   - LANDSAT    MUSCATE PROTO              - ${ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE_PROTO}"${NOCOLOR}
echo -e ${COLOR}"   - LANDSAT    MUSCATE                    - ${ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE}"${NOCOLOR}
echo -e ${COLOR}"   - LANDSAT8                              - ${ENABLE_SPECTRAL_CAMERA_LANDSAT8}"${NOCOLOR}
echo -e ${COLOR}"   - LANDSAT8   MUSCATE                    - ${ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE}"${NOCOLOR}
echo -e ${COLOR}"   - LANDSAT8   MUSCATE PROTO              - ${ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE_PROTO}"${NOCOLOR}
echo -e ${COLOR}"   - SPOT4      MUSCATE PROTO              - ${ENABLE_SPECTRAL_CAMERA_SPOT4_MUSCATE_PROTO}"${NOCOLOR}
echo -e ${COLOR}" * Check the COTS dependencies for MAJA GENERATION launching..."${NOCOLOR}
echo -e ${COLOR}"   - gcc/icc, make, cmake"${NOCOLOR}
echo -e ${COLOR}"   - make"${NOCOLOR}
echo -e ${COLOR}"   - cmake"${NOCOLOR}
echo -e ${COLOR}"   - libxslt package"${NOCOLOR}
echo -e ${COLOR}"   - xsltproc program"${NOCOLOR}
echo -e ${COLOR}"   - gd and gd-devel packages"${NOCOLOR}
echo -e ${COLOR}" * Generation of MAJA package ..."${NOCOLOR}
echo -e ${COLOR}"   - MAJA_BINARY_HOST_INFO                 - ${MAJA_BINARY_HOST_INFO}"${NOCOLOR}
echo -e ${COLOR}"   - MAJA_VERSION                          - ${MAJA_VERSION}"${NOCOLOR}
echo -e ${COLOR}"   - MAJA_VERSION_CHAIN                    - ${MAJA_VERSION_CHAIN}"${NOCOLOR}
echo -e ${COLOR}"   - CMAKE_INSTALL_PREFIX                  - ${MAJA_INSTALL_DIR}"${NOCOLOR}
echo -e ${COLOR}"   - INIT_VAR_MAJA_COTS_SHELL              - ${INIT_VAR_MAJA_COTS_SHELL}"${NOCOLOR}
echo -e ${COLOR}"   - MAJA_COTS_VERSION                     - ${MAJA_COTS_VERSION}"${NOCOLOR}
echo -e ${COLOR}"   - MAJA_COTS_INSTALL_DIR                 - ${MAJA_COTS_INSTALL_DIR}"${NOCOLOR}
echo -e ${COLOR}"   - MAJA_SOURCE_DIR                       - ${MAJA_SOURCE_DIR}"${NOCOLOR}
echo -e ${COLOR}"   - WORKING_DIR                           - ${WORKING_DIR}"${NOCOLOR}
echo -e ${COLOR}" "${NOCOLOR}
echo -e ${CBLUE}"   - Working directory:                    - ${CURRENT_PWD}"${NOCOLOR}
echo -e ${COLOR}"   - Outputs generated:"${NOCOLOR}
echo -e ${CBLUE}"   - Build RPM:                            - ${CPACK_BINARY_RPM}"${NOCOLOR}
if [ "-${CPACK_BINARY_RPM}" == "-ON" ]; then
echo -e ${CBLUE}"          . Package RPM file               - ${OFFICIAL_RPM_NAME} "${NOCOLOR}
echo -e ${COLOR}" "${NOCOLOR}
fi
echo -e ${CBLUE}"   - Build TAR:                            - ${CPACK_BINARY_TGZ}"${NOCOLOR}
if [ "-${CPACK_BINARY_TGZ}" == "-ON" ]; then
echo -e ${CBLUE}"          . Package tarball file           - ${MAJA_INSTALL_TAR_PACKAGE_TAR} "${NOCOLOR}
echo -e ${CBLUE}"            containing: "${NOCOLOR}
echo -e ${CBLUE}"                      * Install script     - ${MAJA_INSTALL_PACKAGE_TAR}/${MAJA_INSTALL_SCRIPT_PACKAGE_TAR}"${NOCOLOR}
echo -e ${CBLUE}"                      * tgz for binaries   - ${MAJA_INSTALL_PACKAGE_TAR}/${OFFICIAL_TGZ_NAME}"${NOCOLOR}
echo -e ${COLOR}" "${NOCOLOR}
fi

exit_code=""
if [ "-${DISABLE_INTERACTIVE_MODE}" == "-YES" ]; then
    echo -e ${COLOR}" Disable interactive mode !!!"${NOCOLOR}
    exit_code="Y"
else
    echo -e ${COLOR}" Ready  (Y/n) ?"${NOCOLOR}
    read exit_code
fi

if [ "-${exit_code}" == "-Y" ]; then
    remove_directory ${WORKING_DIR}
    create_directory  ${WORKING_DIR}
    move_to ${WORKING_DIR}

    # Remove install directory, to be sur remove alls deprecated files (ex: GIPPs, etc.)     
    remove_directory ${MAJA_INSTALL_DIR}

    #------------------------------------------------------------------------------
    MAJA_BUILD_TYPE=Release
    if [ "-${BUILD_TYPE}" == "-debug" ]; then
        MAJA_BUILD_TYPE=RelWithDebInfo
    fi
                        
    echo -e ${COLOR}" ->   Check CC and CXX env variables ..."${NOCOLOR}
    check_var_with_exit "CC" ${CC}
    check_var_with_exit "CXX" ${CXX}

    check_program_with_exit ${CC}
    check_program_with_exit ${CXX}
                        
    check_package_with_exit_make
    check_program_with_exit_cmake
    check_package_with_exit_libxslt
    check_program_with_exit_xsltproc
    check_package_with_exit_gd
    check_package_with_exit_gd_devel
    check_package_with_exit_coreutils  # for sha256 for example
    
    # RPM Binaries (PAS le DEVEL)
    MAJA_INSTALL_NO_DEVELOPMENT=ON
    MAJA_INSTALL_NO_RUNTIME=OFF
    MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES=OFF
    MAJA_ENABLE_VALIDATION=OFF
    
    if [ "x${TARGET_DEVEL_NAME}" == "x-devel" ] ; then
        # RPM DEVEL (Que .h, txx, *.cmake, etc...)
        MAJA_INSTALL_NO_DEVELOPMENT=OFF
        MAJA_INSTALL_NO_RUNTIME=ON
        MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES=ON
        MAJA_ENABLE_VALIDATION=ON
    fi
    
    # Option use only for CS developer 
    BUILD_TESTING=OFF
    ENABLE_MAJA_BUILD_TESTING=OFF
    MAJA_ENABLE_COVERAGE=OFF
    MAJA_CPPCHECK_TEST=OFF
    if [ "-${MAJA_DEVELOPMENT_CONTEXT}" == "-YES"  ] ; then
        # Enable to report build in dasboard
        BUILD_TESTING=ON
        MAJA_INSTALL_NO_DEVELOPMENT=OFF
        MAJA_INSTALL_NO_RUNTIME=OFF
        MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES=ON
        MAJA_ENABLE_VALIDATION=ON
        if [ "-${ENABLE_LAUNCHES}" != "-NO"  ] ; then
            if [ "-${ENABLE_LAUNCHES}" == "-tu"  ] ; then
                ENABLE_MAJA_BUILD_TESTING=ON
            fi
            if [ "-${ENABLE_LAUNCHES}" == "-coverage"  ] ; then
                ENABLE_MAJA_BUILD_TESTING=ON
        	    MAJA_ENABLE_COVERAGE=ON
            fi
            if [ "-${ENABLE_LAUNCHES}" == "-cppcheck"  ] ; then
                ENABLE_MAJA_BUILD_TESTING=ON
        	    MAJA_CPPCHECK_TEST=ON
            fi
        fi
    fi

    #------------------------------------------------------------------------------
    echo -e ${COLOR}" ->   Configure ..."${NOCOLOR}
    launch_command "Configure" cmake -Wno-dev \
     -D "BUILDNAME:STRING='${BUILDNAME}'" \
     -DCMAKE_USER_MAKE_RULES_OVERRIDE=${BUILDER_OVERRIDES_CMAKE} \
     -D "MAJA_VERSION:STRING='${MAJA_VERSION}'" \
     -D "MAJA_VERSION_MINOR:STRING='${MAJA_VERSION_MINOR}'" \
     -D "MAJA_VERSION_PATCH:STRING='${MAJA_VERSION_PATCH}'" \
     -D "MAJA_VERSION_CHAIN:STRING='${MAJA_VERSION_CHAIN}'" \
     ${COMMAND_CMAKE_SITE} \
     -D "CMAKE_INSTALL_PREFIX:PATH='${MAJA_INSTALL_DIR}'" \
     -D "MAJA_COTS_VERSION:STRING='${MAJA_COTS_VERSION}'" \
     -D "MAJA_INSTALL_NO_DEVELOPMENT:BOOL=${MAJA_INSTALL_NO_DEVELOPMENT}" \
     -D "MAJA_INSTALL_NO_RUNTIME:BOOL=${MAJA_INSTALL_NO_RUNTIME}" \
     -D "ENABLE_SPECTRAL_CAMERA_VENUS:BOOL=${ENABLE_SPECTRAL_CAMERA_VENUS}" \
     -D "ENABLE_SPECTRAL_CAMERA_SENTINEL2:BOOL=${ENABLE_SPECTRAL_CAMERA_SENTINEL2}" \
     -D "ENABLE_SPECTRAL_CAMERA_SENTINEL2_GPP:BOOL=${ENABLE_SPECTRAL_CAMERA_SENTINEL2_GPP}" \
     -D "ENABLE_SPECTRAL_CAMERA_SENTINEL2_MUSCATE:BOOL=${ENABLE_SPECTRAL_CAMERA_SENTINEL2_MUSCATE}" \
     -D "ENABLE_SPECTRAL_CAMERA_FORMOSAT_MUSCATE_PROTO:BOOL=${ENABLE_SPECTRAL_CAMERA_FORMOSAT_MUSCATE_PROTO}" \
     -D "ENABLE_SPECTRAL_CAMERA_SPOT4_MUSCATE_PROTO:BOOL=${ENABLE_SPECTRAL_CAMERA_SPOT4_MUSCATE_PROTO}" \
     -D "ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE_PROTO:BOOL=${ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE_PROTO}" \
     -D "ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE:BOOL=${ENABLE_SPECTRAL_CAMERA_LANDSAT_MUSCATE}" \
     -D "ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE_PROTO:BOOL=${ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE_PROTO}" \
     -D "ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE:BOOL=${ENABLE_SPECTRAL_CAMERA_LANDSAT8_MUSCATE}" \
     -D "ENABLE_SPECTRAL_CAMERA_LANDSAT8:BOOL=${ENABLE_SPECTRAL_CAMERA_LANDSAT8}" \
     -D "MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES:BOOL=${MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES}" \
     -D "BUILD_TESTING:BOOL=${BUILD_TESTING}" \
     -D "ENABLE_MAJA_BUILD_TESTING:BOOL=${ENABLE_MAJA_BUILD_TESTING}" \
     -D "MAJA_ENABLE_COVERAGE:BOOL=${MAJA_ENABLE_COVERAGE}" \
     -D "MAJA_CPPCHECK_TEST:BOOL=${MAJA_CPPCHECK_TEST}" \
     -D "CMAKE_BUILD_TYPE:STRING='${MAJA_BUILD_TYPE}'" \
     -D "BUILD_DOXYGEN:BOOL=OFF" \
     -D "BUILD_SHARED_LIBS:BOOL=ON" \
     -D "MAJA_COTS_INSTALL_DIR:PATH='${MAJA_COTS_INSTALL_DIR}'" \
     -D "MAJA_ENABLE_VALIDATION:BOOL=${MAJA_ENABLE_VALIDATION}" \
     -D "MAJA_USE_CPACK:BOOL=ON" \
     -D "MAJA_PACKAGE_NAME:STRING='${MAJA_PACKAGE_NAME}'" \
     -D "CPACK_PACKAGE_NAME:STRING='${MAJA_PACKAGE_NAME}'" \
     -D "CPACK_BINARY_STGZ:BOOL=OFF" \
     -D "CPACK_BINARY_TBZ2:BOOL=OFF" \
     -D "CPACK_BINARY_TZ:BOOL=OFF" \
     -D "CPACK_BINARY_ZIP:BOOL=OFF" \
     -D "CPACK_BINARY_DEB:BOOL=OFF" \
     -D "CPACK_BINARY_RPM:BOOL=${CPACK_BINARY_RPM}" \
     -D "CPACK_BINARY_TGZ:BOOL=${CPACK_BINARY_TGZ}" \
    ${MAJA_SOURCE_DIR}
    if [ "-${exit_code}" != "-0" ]; then
        echo -e ${COLOR}" ->   Configure ${ERRCO}failed!${COLOR} (return ${exit_code})."${NOCOLOR}
        exit 128
    fi

    # Ne fait que compiler et faire un make install
    if [ "-${MAJA_DEVELOPMENT_CONTEXT}" == "-YES" ]; then
        echo -e ${COLOR}" ->   Move to the working directory ${WORKING_DIR} ..."${NOCOLOR}
        move_to ${WORKING_DIR}
        # Force the MAKECOMMAND value with nbthreads (impossible to do this in configure parameters 
        utilities_replace_Line_With_KeyMatching_by_NewLine ${WORKING_DIR}/CMakeCache.txt "MAKECOMMAND:STRING="  "MAKECOMMAND:STRING=/usr/bin/make ${MAKE_PARAMETERS}"
        # Add Eclispe configuration
        cmake -G"Eclipse CDT4 - Unix Makefiles" \
        -D "MAJADATA_SOURCE_DIR:PATH='${MAJADATA_SOURCE_DIR}'" \
       # -D "MAJADATATU_SOURCE_DIR:PATH='${MAJADATATU_SOURCE_DIR}'" \
        .
        if [ "-${exit_code}" != "-0" ]; then
            echo -e ${COLOR}" ->   Configure with Eclipse Generator ${ERRCO}failed!${COLOR} (return ${exit_code})."${NOCOLOR}
            exit 128
        fi
        echo -e ${COLOR}" * Configure with Eclipse Generator ${PASSEDCO}passed${COLOR}"${NOCOLOR}
        launch_command "Development context : export varenv MAJA_INSTALL_DIR" "export MAJA_INSTALL_DIR"
        launch_command "Development context : launches build MAJA in Dasboard with ctest -D Experimental" "ctest -D Experimental -VV"
        launch_command "Development context : Creation of the MAJA install " "/usr/bin/make install"
    else    
        # *****************************************************************************
        #                           S T A R T     R P M
        # *****************************************************************************
        if [ "-${CPACK_BINARY_RPM}" == "-ON" ]; then
    
            move_to ${WORKING_DIR}

            # Build maja
            launch_command "Creation of the RPM MAJA" "/usr/bin/make ${MAKE_PARAMETERS} package"

            # Find the rpm file
            listrpm=`ls ${MAJA_PACKAGE_NAME}*.rpm`
            move_file ${listrpm}  ${CURRENT_PWD}/${OFFICIAL_RPM_NAME}

            # Create the .sha file
            move_to ${CURRENT_PWD}
            compute_sha256key_filename ${OFFICIAL_RPM_NAME} ${OFFICIAL_RPM_NAME_SHA}
            
        fi # if [ "-${CPACK_BINARY_RPM}" == "-ON" ]; then
    
        # *****************************************************************************
        #                           S T A R T     T G Z
        # *****************************************************************************
        if [ "-${CPACK_BINARY_TGZ}" == "-ON" ]; then
            move_to ${WORKING_DIR}
    
            create_directory ${CURRENT_PWD}/${MAJA_INSTALL_PACKAGE_TAR}
            launch_command "Creation of the TARBALL MAJA" "/usr/bin/make ${MAKE_PARAMETERS} install"
            copy_shell  ${WORKING_DIR}/${MAJA_INSTALL_SCRIPT_PACKAGE_TAR_BINARY_FILENAME} ${CURRENT_PWD}/${MAJA_INSTALL_PACKAGE_TAR}/${MAJA_INSTALL_SCRIPT_PACKAGE_TAR}
            launch_command "Creation of the uninstall script" "${GENERATION_UNINSTALLFILE} -t ${TARGET_PACKAGE_NAME} -b ${BASE_INSTALL_DIR_CORE} -p ${BASE_DIR_FOR_UNISTALL_GENERATION_LIST_FILE} -o ${CURRENT_PWD}/${MAJA_INSTALL_PACKAGE_TAR}/${MAJA_UNINSTALL_SCRIPT_PACKAGE_TAR_BINARY_FILENAME}"
            compress_gzip_directory ${CURRENT_PWD}/${MAJA_INSTALL_PACKAGE_TAR}/${OFFICIAL_TGZ_NAME} ${MAJA_INSTALL_DIR_BASE} maja
            tar_directory ${CURRENT_PWD}/${MAJA_INSTALL_TAR_PACKAGE_TAR} ${CURRENT_PWD} ${MAJA_INSTALL_PACKAGE_TAR}  
            remove_directory ${CURRENT_PWD}/${MAJA_INSTALL_PACKAGE_TAR}
            
            # Create the .sha file
            move_to ${CURRENT_PWD}
            compute_sha256key_filename ${MAJA_INSTALL_TAR_PACKAGE_TAR} ${MAJA_INSTALL_TAR_PACKAGE_TAR_SHA}
            
        fi # if [ "-${CPACK_BINARY_TGZ}" == "-ON" ]; then
    
        remove_directory ${WORKING_DIR}
        remove_directory ${MAJA_INSTALL_DIR_BASE}
    fi    
    echo -e ${COLOR}" * Generation of MAJA package done."${NOCOLOR}
else # if [ "-${exit_code}" == "-Y" ]; then
    echo -e ${COLOR}" * Generation of MAJA package cancelled!"${NOCOLOR}
fi # if [ "-${exit_code}" == "-Y" ]; then

exit 0
