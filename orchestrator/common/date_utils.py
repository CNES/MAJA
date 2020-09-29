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

orchestrator.common.directory_manager -- shortdesc

orchestrator.common.directory_manager is a description

It defines classes_and_methods


###################################################################################################
"""
import datetime
import math


def get_datetime_from_utc(utcstr):
    if "Z" in utcstr:
        return datetime.datetime.strptime(utcstr, "UTC=%Y-%m-%dT%H:%M:%S.%fZ")
    else:
        return datetime.datetime.strptime(utcstr, "UTC=%Y-%m-%dT%H:%M:%S")


def get_datetime_from_yyyymmddthhmmss(datestr):
    return datetime.datetime.strptime(datestr, "%Y%m%dT%H%M%S")


def get_datetime_from_yyyymmdd(datestr):
    return datetime.datetime.strptime(datestr, "%Y%m%d")


def get_utc_from_datetime(datet):
    return datetime.datetime.strftime(datet, "UTC=%Y-%m-%dT%H:%M:%S")


def get_julianday_as_double(p_tm):
    year = p_tm.year
    month = p_tm.month
    day = p_tm.day
    hour = p_tm.hour
    minu = p_tm.minute
    sec = p_tm.second
    millisec = p_tm.microsecond // 1000
    # Conversion to julian day according to
    #  http: // en.wikipedia.org / wiki / Julian_day
    # division are integer divisions:
    a = (14 - month) // 12
    y = year + 4800 - a
    m = month + 12 * a - 3
    julianday = day + (153 * m + 2) // 5 + 365 * y + y // 4 - y // 100 + y // 400 - 32045
    # now, the division are NOT integer
    julianday += (hour - 12) / 24. + minu / 1440. + sec / 86400.
    # now, the millisecond
    julianday += millisec / 86.4e6
    return julianday


def get_julianday_as_int(p_tm):
    return math.floor(get_julianday_as_double(p_tm))


def get_average_time(ptimestartutc, ptimestoputc):
    # Get the validity start date in julian day
    l_timestart_jd = get_julianday_as_double(get_datetime_from_utc(ptimestartutc))
    # Get the validity stop date in julian day
    l_timestop_jd = get_julianday_as_double(get_datetime_from_utc(ptimestoputc))
    # Compute the average date
    return (l_timestart_jd + l_timestop_jd) / 2.

def get_date_yyyymmdd_from_tm(p_tm):
    # Write the date in the formated "UTC=%04d-%02d-%02dT%02d:%02d:%02d"
    return str("{:04d}{:02d}{:02d}").format(p_tm.year, p_tm.month, p_tm.day)

def get_date_hhmmss_from_tm(p_tm):
    # Write the date in the formated "UTC=%04d-%02d-%02dT%02d:%02d:%02d"
    return str("{:02d}{:02d}{:02d}").format(p_tm.hour, p_tm.minute, p_tm.second)

def get_date_millisecs_from_tm(p_tm):
    return str("{:03d}").format( int(p_tm.microsecond/1000.0))
