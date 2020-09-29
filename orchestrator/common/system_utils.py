#!/usr/bin/python
# -*- coding: iso-8859-15 -*-
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
# *
# * $Id:$
# *
# * HISTORIQUE
# *
# * MOD : VERSION : 1.0 : Creation
# *
# * FIN-HISTORIQUE
# *
# *


from threading import Thread
from contextlib import contextmanager
import shlex
from orchestrator.common.logger.maja_logging import configure_logger
import subprocess
import sys
import tracemalloc
import linecache
import os

global PROCESS_ID_FIELD
PROCESS_ID_FIELD = 1

global FATHER_PROCESS_ID_FIELD
FATHER_PROCESS_ID_FIELD = 2

LOGGER = configure_logger(__name__)

try:
    from queue import Queue, Empty
except ImportError:
    from queue import Queue, Empty  # python 2.x

"""
Execute an external command
"""


@contextmanager
def log_running_task(task_name):
    LOGGER.info("Starting task: %s" % task_name)
    yield
    LOGGER.info("Finished task %s" % task_name)


def launch_command(command):
    with log_running_task(command):
        # Convert cmd from string to list
        if isinstance(command, str):
            if sys.platform == "win32":
                lex = shlex.shlex(command, posix=False)
            else:
                lex = shlex.shlex(command)
            lex.quotes = '"'
            lex.whitespace_split = True
            cmd_name = list(lex)
        elif isinstance(command, list):
            cmd_name = command
        cmd_name = " ".join([str(x) for x in cmd_name])
        # Indeed launch command

        def enqueue_output(out, queue):
            for line in iter(out.readline, b''):
                queue.put(line)
            out.close()

        err_queue = Queue()
        out_queue = Queue()
        try:
            p = subprocess.Popen(cmd_name, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
            err_t = Thread(target=enqueue_output, args=(p.stderr, err_queue))
            out_t = Thread(target=enqueue_output, args=(p.stdout, out_queue))
            out_t.daemon = True
            err_t.daemon = True
            out_t.start()
            err_t.start()
        except OSError as err:
            LOGGER.warn('An error occured with command : %s with message %s', cmd_name, err)
            return err.errno

        while True:
            try:
                err_line = err_queue.get_nowait()
            except Empty:
                err_line = None
            try:
                out_line = out_queue.get_nowait()
            except Empty:
                out_line = None
            # Test if process is active
            status = p.poll()
            if status is not None:
                if (out_line is None) and (err_line is None):
                    LOGGER.debug("End of command")
                    break
            if out_line is not None:
                LOGGER.info(out_line)
            if err_line is not None:
                LOGGER.error(err_line)

        return p.returncode

# exception definition


class DiskSpaceUsedException(IOError):

    def __init__(self, strmsg):
        self._strMsg = strmsg

    def __str__(self):
        return 'DiskSpaceUsedException : %s' % (str(self._strMsg))


class MemoryUsedByProcessException(IOError):

    def __init__(self, strmsg):
        self._strMsg = strmsg

    def __str__(self):
        return 'MemoryUsedByProcessException: %s' % (str(self._strMsg))

# Fonction permettant de connaitre la taille d'un repertoire, le resultat est exprime en M octets


def disk_space_used(workingdirectory):
    try:
        p = subprocess.Popen("/usr/bin/du -sk %s" % workingdirectory, shell=True, stdout=subprocess.PIPE)
        p.wait()
        out = p.stdout.read()
        resultinkoctets = out.split()
        resultinmoctets = float(resultinkoctets[0]) / 1024.0
    except Exception as e:
        strerror = str(e).split("\n")
        raise DiskSpaceUsedException(strerror[0])

    return resultinmoctets

# Fonction permettant de connaitre la memoire utilisee par un processus et
# ses sous-processus associes, le resultat est exprime en M octets


def memory_used_by_process_and_children_processes(idprocess):
    try:
        # On va recuperer la liste des sous process du process que l'on veut etudier
        processlist = obtain_subprocesses_from_main_process(idprocess)
        tot_mem_mo_sum = 0

        for procnum in processlist:
            tot_mem_tmp = memory_used_by_process2(procnum)
            tot_mem_mo_sum += tot_mem_tmp

    except Exception as e:
        strerror = str(e).decode().split("\n")
        raise MemoryUsedByProcessException(strerror[0])

    return tot_mem_mo_sum

# Fonction permettant de connaitre la memoire utilisee par un processus et
# ses sous-processus associes, le resultat est exprime en M octets


def memory_used_by_children_processes(idprocess):
    try:
        # On va recuperer la liste des sous process du process que l'on veut etudier
        processlist = obtain_subprocesses_from_main_process(idprocess)
        tot_mem_mo_sum = 0
        for procnum in processlist:
            if procnum != idprocess:
                tot_mem_tmp = memory_used_by_process2(procnum)
                tot_mem_mo_sum += tot_mem_tmp
    except Exception as e:
        strerror = str(e).decode().split("\n")
        raise MemoryUsedByProcessException(strerror[0])
    return tot_mem_mo_sum


def memory_used_by_process(idprocess):
    # Fonction permettant de connaitre la memoire utilisee par un processus, le resultat est exprime en M octets
    file_name = '/proc/' + str(idprocess) + '/maps'

    try:
        fd = open(file_name)
        tot_mem = 0

        for l in fd:
            flds = l.decode().split()

            # All malloc()ed memory goes into anonymous memory blocks.
            # Hence I am considering only anonymous memory chunks, which will have only 5 columns in the output.
            if len(flds) > 5:
                continue
            mem_start, mem_end = flds[0].decode().split('-')
            mem_start = int('0x' + mem_start, 16)
            mem_end = int('0x' + mem_end, 16)
            tot_mem = tot_mem + mem_end - mem_start
        fd.close()

        tot_mem_ko = tot_mem / 1024.0
        tot_mem_mo = tot_mem_ko / 1024.0

    except Exception:
        # Si on a rencontre un probleme pour lire les donnees de memoire
        # Alors on renvoie un resultat nul
        tot_mem_mo = 0

    return tot_mem_mo


def memory_used_by_process2(idprocess):
    _scale = {'kB': 1024.0, 'mB': 1024.0 * 1024.0,
              'KB': 1024.0, 'MB': 1024.0 * 1024.0}

    try:
        statfile = open("/proc/" + str(idprocess) + "/status")
        statinfo = statfile.read()
        statfile.close()
        i = statinfo.index("VmHWM:")
        lsthwm = statinfo[i:].split()
        if len(lsthwm) < 3:
            tot_mem_mo = 0  # error bad formatting
        else:
            tot_mem_mo = float(float(lsthwm[1]) * _scale[lsthwm[2]])
    except Exception:
        tot_mem_mo = 0

    return tot_mem_mo / (1024.0 * 1024.0)


def memory_used_by_process_current(idprocess):
    _scale = {'kB': 1024.0, 'mB': 1024.0 * 1024.0,
              'KB': 1024.0, 'MB': 1024.0 * 1024.0}

    try:
        statfile = open("/proc/" + str(idprocess) + "/status")
        statinfo = statfile.read()
        statfile.close()
        i = statinfo.index("VmRSS:")
        lsthwm = statinfo[i:].split()
        if len(lsthwm) < 3:
            tot_mem_mo = 0  # error bad formatting
        else:
            tot_mem_mo = float(float(lsthwm[1]) * _scale[lsthwm[2]])
    except Exception:
        tot_mem_mo = 0

    return tot_mem_mo / (1024.0 * 1024.0)



def elapsed_time(idprocess, pov):
    p = subprocess.Popen("/bin/ps jhS %s" % idprocess, shell=True, stdout=subprocess.PIPE)
    out = p.stdout.read()
    elements = out.decode().split()
    p.wait()
    return elements[pov]


def user_elapsed_time(idprocess):
    try:
        res = elapsed_time(idprocess, 8)
    except Exception as e:
        strerror = str(e).decode().split("\n")
        raise IOError(strerror[0])
    return res


def return_child_ids(tabprocess, idprocess):
    childslist = []
    for proc in tabprocess:
        currentprocessid = int(proc[PROCESS_ID_FIELD])
        currentfatherprocessid = int(proc[FATHER_PROCESS_ID_FIELD])
        if currentfatherprocessid == idprocess:
            childslist.append(currentprocessid)
    return childslist


def modifylist(argsfinal, currentprocesslist, idprocess):
    childslist = return_child_ids(argsfinal, idprocess)
    currentprocesslist = childslist + currentprocesslist
    return currentprocesslist


def obtain_subprocesses_from_main_process(idprocess):

    argsfinal = []
    finalprocesslist = list()
    p = subprocess.Popen("ps -ef | grep " + str(idprocess), shell=True, stdout=subprocess.PIPE)
    out = p.communicate()[0]
    args = out.decode().split("\n")
    # On recupere les processus dans un tableau
    for i in range(len(args) - 1):
        argsfinal.append(args[i].split())

    # On enleve la premiere ligne
    argsfinal.pop(0)

    # Tant que l'on trouve des fils, on continue
    currentprocesslist = list()
    currentprocesslist.append(idprocess)

    while len(currentprocesslist) != 0:
        currentid = currentprocesslist.pop(0)
        finalprocesslist.append(currentid)
        currentprocesslist = modifylist(argsfinal, currentprocesslist, currentid)

    return finalprocesslist


def display_top_ram_consumer(key_type='lineno', limit=20):
    snapshot = tracemalloc.take_snapshot()
    snapshot = snapshot.filter_traces((
        tracemalloc.Filter(False, "<frozen importlib._bootstrap>"),
    ))
    top_stats = snapshot.statistics(key_type)

    LOGGER.debug("Top %s lines" % limit)
    for index, stat in enumerate(top_stats[:limit], 1):
        frame = stat.traceback[0]
        # replace "/path/to/module/file.py" with "module/file.py"
        filename = os.sep.join(frame.filename.split(os.sep)[-2:])
        LOGGER.debug("#%s: %s:%s: %.1f KiB"
              % (index, filename, frame.lineno, stat.size / 1024))
        line = linecache.getline(frame.filename, frame.lineno).strip()
        if line:
            print('    %s' % line)

    other = top_stats[limit:]
    if other:
        size = sum(stat.size for stat in other)
        LOGGER.debug("%s other: %.1f KiB" % (len(other), size / 1024))
    total = sum(stat.size for stat in top_stats)
    LOGGER.debug("Total allocated size: %.1f KiB" % (total / 1024))


def usage():
    LOGGER.debug("Usage: %s pid" % sys.argv[0])
    LOGGER.debug("Get the memory usage of the given pid and child")
    sys.exit(1)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()

    pid = int(sys.argv[1])
    LOGGER.debug(str(memory_used_by_process_and_children_processes(pid)))
    sys.exit(0)
