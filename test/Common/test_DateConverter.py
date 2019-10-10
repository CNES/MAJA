# -*- coding: utf-8 -*-

import unittest
from Common import DateConverter


class TestDateConverter(unittest.TestCase):

    def testDateConversionShort(self):
        orig = "20180522"
        origAsDT = DateConverter.stringToDatetime(orig)
        self.assertEqual(DateConverter.datetimeToStringShort(origAsDT), orig)
        self.assertEqual(DateConverter.datetimeToString(origAsDT),
                         "2018-05-22T00:00:00.000Z")
       
    def testDateConversionLong(self):
        orig = "2018-05-22T00:00:00.000Z"
        origAsDT = DateConverter.stringToDatetime(orig)
        self.assertEqual(DateConverter.datetimeToStringShort(origAsDT), "20180522")
        self.assertEqual(DateConverter.datetimeToString(origAsDT),
                         orig)


if __name__ == '__main__':
    unittest.main()

