# -*- coding: utf-8 -*-

from datetime import datetime as dt

def datetimeToString(timestamp):
    """
    @brief Convert datetime to string %Y-%m-%dT%H:%M:%S.%f
    """
    return timestamp.strftime("%Y-%m-%dT%H:%M:%S.%f")[:-3] + "Z"

def datetimeToStringShort(timestamp):
    """
    @brief Convert datetime to string YYYYMMDD
    """
    return timestamp.strftime("%Y%m%d")

def datetimeToStringName(timestamp):
    """
    @brief Convert datetime to string YYYYMMDD
    """
    return timestamp.strftime("%Y%m%d-%H%M%S-%f")[:-3]

def stringToDatetime(s):
    """
    @brief Convert string of format YYYYMMDD or %Y-%m-%dT%H:%M:%S.%f to datetime
    """
    if(len(s) == 24):
        return dt.strptime(s[:-1], "%Y-%m-%dT%H:%M:%S.%f")
    if(len(s) == 8):
        return dt.strptime(s, "%Y%m%d")
    raise ValueError("Unknown datetime string {0}".format(s))