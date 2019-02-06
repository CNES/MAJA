# -*- coding: utf-8 -*-

def str2bool(v):
    """
    :brief: Returns a boolean following a string input
    :param: v String to be tested if it"s containing a True/False value
    :return: bool True, if string contains ("yes", "true", "t", "y", "1"), false if not
    """
    import argparse
    if v.lower() in ("yes", "true", "t", "y", "1"):
        return True
    elif v.lower() in ("no", "false", "f", "n", "0"):
        return False
    else:
        raise argparse.ArgumentTypeError("Boolean value expected.")
    return