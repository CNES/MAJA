#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 10 13:42:03 2018

@author: akynos
"""

from selenium import webdriver
from selenium.webdriver.chrome.options import Options

#Chrome Settings
windowSize = "1024,768"
headless = "--headless" #Hides window

class SRTMDownloader():
    #Page infos
    url="http://srtm.csi.cgiar.org/SELECTION/inputCoord.asp"
    titleName="SRTM Data Search"
    
    #Radio Button names:
    server="USA" #CGIAR-CSI (USA)
    selectionMethod="Radio1" #Input Coordinates
    selectionType="Decimal" #Decimal Degress
    fileFormat="GeoTiff" #File Format
    
    #Text box names (For Decimal selection)
    txtlonmin = "txtLongMin"
    txtlatmin = "txtLatMin"
    txtlonmax = "txtLongMax"
    txtlatmax = "txtLatMax"
    
    def __init__(self, latmin, lonmin, latmax, lonmax):
        """
        :brief: Init the SRTM Downloader with the min/max Lat/Lon Coordinates
        """
        self.latmin = latmin
        self.lonmin = lonmin
        self.latmax = latmax
        self.lonmax = lonmax
    

    def getSRTM(self):
        """
        :brief: Download the SRTM from CGIAR-CSI using the previously given
                Coordinates.
        :note: Uses selenium and chrome-webdriver to open a headless browser
        """
        chrome_options = Options()  
        chrome_options.add_argument(headless)
        chrome_options.add_argument("--window-size=%s" % windowSize)
        
        driver = webdriver.Chrome(options = chrome_options)
        driver.get(self.url)
        assert self.titleName in driver.title #Check if the site is available
        #Set all radio buttons:
        driver.find_element_by_css_selector("input[type='radio'][value='"+self.server+"']").click()
        driver.find_element_by_css_selector("input[type='radio'][value='"+self.selectionMethod+"']").click()
        driver.find_element_by_css_selector("input[type='radio'][value='"+self.selectionType+"']").click()
        driver.find_element_by_css_selector("input[type='radio'][value='"+self.fileFormat+"']").click()
        
        #TODO Enter coordinates
        #TODO Wait for next page to open
        #TODO Download SRTM
        
        #For Debug purposes either take a screenshot or deactivate headless mode
        driver.get_screenshot_as_file("capture.png")
        driver.close()
    
if __name__ == "__main__":
    c = SRTMDownloader("43.2382625528", "0.495928592915", "44.2478306835", "1.8886830142")
    c.getSRTM()
