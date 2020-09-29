<?xml version="1.0" encoding="utf-8"?>
<!-- 
 
  HISTORIQUE
 
  VERSION : 1.3 : DM : VE-FT-847 : 26/08/2015 : Changement des icônes pour avoir des icônes libres de droits
 
  FIN-HISTORIQUE
 -->
<xsl:stylesheet version="1.0"
  xmlns:ee="http://eop-cfi.esa.int/CFI" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
  <xsl:output method="html" encoding="utf-8"/>

  <xsl:variable name="displayVersion">1.3</xsl:variable>

  <xsl:variable name="colWidth">5px</xsl:variable>
  <!--  ============================================================================== -->
  <xsl:template match="/">
  <!--  ============================================================================== -->
    <html>
      <xsl:call-template name="html-head"/>
      <body>
        <table width="50%">
          <tr>
            <td align="center">
              <xsl:apply-templates select="ee:Earth_Explorer_Header|*/ee:Earth_Explorer_Header" mode="header">
              </xsl:apply-templates>
            </td>
          </tr>
          <tr>
            <td>
              <xsl:apply-templates select="/" mode="detail">
                <xsl:with-param name="parentPath" select="''"/>
              </xsl:apply-templates>
            </td>
          </tr>
          <tr>
            <td class="displayVersion">
              display : version <xsl:value-of select="$displayVersion"/>
            </td>
          </tr>
        </table>
      </body>
    </html>
  </xsl:template>
  <!--  ============================================================================== -->
  <xsl:template name="html-head">
  <!--  ============================================================================== -->
    <head>
      <style type="text/css">
body { 
  color: #000000;
  font-family: Verdana, Arial, Sans Serif;
}
.displayVersion {
  color:#888888;
  font-size:9px;
  text-align:center;
}
.plusMinus {
  position:relative;
  margin-left:5px;
}
.header-table {
  position:relative;
  margin-bottom:10px;
}
.header-interface, .header-id {
  font-weight:bolder;
  text-align:center;
  padding:5px 3px 5px 3px;
}
.header-interface {
  font-size:14px;
  background-color:#003399;
  color:#FFFFFF;
}
.header-id {
  border:1px solid #003399;
  font-size:12px;
}
.header-key-instance {
  background-color:#FFFF99;
  font-size:10px;
  font-weight:bolder;
  width:50%;
  text-align:right;
}
.header-value-instance {
  border:1px solid #BBBBBB;
  font-size:10px;
  font-weight:bolder;
  width:50%;
}
.detail-table {
  border:1px solid #000000;
  width:100%;
  background-color:#FFFFFF;
}
.key {
  font-size:10px;
  font-weight:bolder;
}
.errorKey {
}
.value {
  font-size:10px;
  width:500px;
}
.filler {
  background-color: #FFFFFF;
  color: #FFFFFF;  	
  width:15px;
}
.flat {
  height:1px;
}
.even {
  background-color:#DDDDDD;
}
.odd {
  background-color:#FFFFFF;
}
.snSk {
  border:1px solid #FFAA00;
  background-color:#FFFFBB;
  margin-left:5px;
  padding:0px 3px 0px 3px;
}
.idOrStatus {
  color:#AAAAAA;
  margin-left:5px;
  padding:0px 3px 0px 3px;
}
.unit {
  color:#555555;
  margin-left:20px;
  padding:0px 3px 0px 3px;
  font-size:9px;
  font-weight:bolder;
}
.hideShowImg {
}
.showElt {
}
.hideElt {
display:none;
}
.error {
  background-color:#FFBBBB;
  font-size:10px;
  font-style:italics;
}
      </style>
      <script language="javascript">
<![CDATA[     
      // *******************************
      function hideShow(idImg,idTable) {
      // *******************************
      	var eltImg = document.getElementById(idImg);
        var eltTable = document.getElementById(idTable);

        if (eltTable) {
          if (eltTable.className == "showElt") {
            eltImg.src = "DISPLAY/go-next.png";
            eltTable.className = "hideElt";
          } else {
            eltImg.src = "DISPLAY/go-down.png";
            eltTable.className = "showElt";
          }
        }
      }
      
      // **************************
      function showList(idParent) {
      // **************************
        hideShowList(idParent,"showElt");
      }
      
      // **************************
      function hideList(idParent) {
      // **************************
        hideShowList(idParent,"hideElt");
      }
      
      // *******************************************
      function hideShowList(idParent,imgClassName) {
      // *******************************************
        var alertD = false;
      	var trList = document.getElementsByTagName('tr');
      	var cptDiff = 0;
      	var cptEqu = 0;
      	for (var i = 0;i < trList.length;i++) {
          var listPathAttr = trList.item(i).getAttribute('listPath');
          if (listPathAttr) {
          
            if (listPathAttr == idParent) {
              cptEqu++;
              var listImgId = trList.item(i).getAttribute('listImgId');
              var listTableId = trList.item(i).getAttribute('listTableId');
              
              if (listTableId) {
                var eltTable = document.getElementById(listTableId);
                if (eltTable.className != imgClassName) {
                  hideShow(listImgId,listTableId);
                }
              }
            } else {
              cptDiff++;
            }
          }
      	}
        //alert("idParent=" + idParent + "\ntotal=" + trList.length + " ,cptEqu=" + cptEqu + " ,cptDiff=" + cptDiff);
      }
]]>
      </script>
    </head>
  </xsl:template>

  <!--  ============================================================================== -->
  <xsl:template match="ee:Earth_Explorer_Header" mode="header">
  <!--  ============================================================================== -->
    <table class="header-table">
      <tr>
        <td class="header-interface" colspan="2"><xsl:value-of select="ee:Fixed_Header/ee:File_Description"/></td>
      </tr>
      <tr>
        <td class="header-id" colspan="2"><xsl:value-of select="ee:Fixed_Header/ee:File_Name"/></td>
      </tr>
<!--      
      <tr>
        <td class="header-key">File name</td>
        <td class="header-value"><xsl:value-of select="ee:Fixed_Header/ee:File_Name"/></td>
      </tr>
      <tr>
        <td class="header-key">Interface</td>
        <td class="header-value"><xsl:value-of select="ee:Fixed_Header/ee:File_Description"/></td>
      </tr>
-->
      <tr>
        <td colspan="2"/>
      </tr>
      <xsl:apply-templates select="ee:Variable_Header/ee:Specific_Product_Header/ee:Instance_Id/*" mode="instance"/>
    </table>
  </xsl:template>
  
  <!--  ============================================================================== -->
  <xsl:template match="*[*]" mode="instance">
  <!--  ============================================================================== -->
    <xsl:param name="parentPath" select="''"/>

    <xsl:apply-templates select="*" mode="instance">
      <xsl:with-param name="parentPath" select="concat($parentPath,local-name(),'/')"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <!--  ============================================================================== -->
  <xsl:template match="*[not(*)]" mode="instance">
  <!--  ============================================================================== -->
    <xsl:param name="parentPath" select="''"/>
    <xsl:variable name="path" select="concat($parentPath,local-name())"/>

    <tr>
      <td class="header-key-instance"><xsl:value-of select="$path"/></td>
      <td class="header-value-instance">
        <xsl:value-of select="."/>
        <xsl:for-each select="@unit">
          <span class="unit">(<xsl:value-of select="concat(local-name(),':',.)"/>)</span>
        </xsl:for-each>
      </td>
    </tr>
  </xsl:template>
  
  <!--  ============================================================================== -->
  <xsl:template match="/|*[*]" mode="detail">
  <!--  ============================================================================== -->
    <xsl:param name="parentPath" select="''"/>
    <xsl:param name="position" select="'0'"/>
    <xsl:variable name="path" select="concat($parentPath,'/',local-name(),'[',$position,']')"/>
    
    <table class="detail-table">
      <tr height="1px">
        <td/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat filler" width="{$colWidth}"/>
        <td class="flat value"/>
      </tr>
      <xsl:for-each select="*">
        <xsl:variable name="err1">
          <xsl:if test="@sn and (position() != number(@sn))">[WARNING] sn attribute value seems to be wrong</xsl:if>
        </xsl:variable>
        <xsl:variable name="hasErr1" select="string-length(normalize-space($err1)) &gt; 0"/>
        <xsl:variable name="err2">
          <xsl:if test="@count and (count(*) != @count)">[WARNING] count attribute value seems to be wrong</xsl:if>
        </xsl:variable>
        <xsl:variable name="hasErr2" select="string-length(normalize-space($err2)) &gt; 0"/>
        <xsl:variable name="err3">
          <xsl:if test="starts-with(local-name(),'List_of_') and not(@count)">[WARNING]count attribute is mandatory</xsl:if>
        </xsl:variable>
        <xsl:variable name="hasErr3" select="string-length(normalize-space($err3)) &gt; 0"/>
        <xsl:variable name="err4">
          <xsl:if test="starts-with(local-name(),'List_of_') and (*[not(@sn) and not(@sk)])">[WARNING]one or more sub-elements has no sn nor sk attribute</xsl:if>
        </xsl:variable>
        <xsl:variable name="hasErr4" select="string-length(normalize-space($err4)) &gt; 0"/>
        <xsl:variable name="hasErr" select="$hasErr1 or $hasErr2 or $hasErr3 or $hasErr4"/>

        <xsl:variable name="keyClass">
          <xsl:choose>
            <xsl:when test="$hasErr">key errorKey</xsl:when>
            <xsl:otherwise>key</xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
      
        <xsl:variable name="childPath" select="concat($path,'/',local-name(),'[',position(),']')"/>
        <xsl:variable name="pos" select="position()"/>
        <xsl:variable name="oddEven">
          <xsl:choose>
            <xsl:when test="(position() mod 2) = 1">odd</xsl:when>
            <xsl:otherwise>even</xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="refId" select="$childPath"/>
        <xsl:variable name="icon">DISPLAY/go-down.png</xsl:variable>
        <xsl:choose>
          <xsl:when test="*">
            <tr>
              <xsl:if test="starts-with(local-name(..),'List_of_')">
                <xsl:attribute name="listPath"><xsl:value-of select="$path"/></xsl:attribute>
                <xsl:attribute name="listImgId"><xsl:value-of select="concat($refId,'_img')"/></xsl:attribute>
                <xsl:attribute name="listTableId"><xsl:value-of select="concat($refId,'_table')"/></xsl:attribute>
              </xsl:if>
              <td title="{$path}"> <!-- ZWH -->
                <img src="{$icon}" class="hideShowImg" onClick="javascript:hideShow('{$refId}_img','{$refId}_table');" id="{$refId}_img"/>
              </td>
              <td class="{$keyClass}" colspan="13" width="100%">
                <xsl:value-of select="local-name()"/>
                <xsl:for-each select="@schema_version|@sn|@sk|@count">
                  <span class="snSk">
                    <xsl:value-of select="concat(local-name(),':',.)"/>
                  </span>
                </xsl:for-each>
                <xsl:if test="starts-with(local-name(),'List_of_') and (count(*[*]) &gt; 1)">
                  <img src="DISPLAY/list-add.png" class="plusMinus" onClick="javascript:showList('{$childPath}');" title="expand all children"/>
                  <img src="DISPLAY/list-remove.png" class="plusMinus" onClick="javascript:hideList('{$childPath}');" title="collapse all children"/>
                </xsl:if>
                <xsl:if test="starts-with(local-name(..),'List_of_')">
                  <xsl:if test="(substring-before(local-name(*[1]),'_Id')) and not(substring-after(local-name(*[1]),'_Id'))">
                    <span class="idOrStatus">(id=<xsl:value-of select="*[1]"/>)</span>
                  </xsl:if>
                  <xsl:for-each select="*[local-name() = 'Status']">
                    <span class="idOrStatus">(status=<xsl:value-of select="."/>)</span>
                  </xsl:for-each>
                </xsl:if>
              </td>
              <td>
                <xsl:if test="$hasErr">
                  <div class="error">
                    <xsl:value-of select="concat($err1,$err2,$err3,$err4)"/>
                  </div>
                </xsl:if>
              </td>
            </tr>
            <tr id="{$refId}_table" class="showElt">
              <td/>
              <td class="{$oddEven}" colspan="14">
                <xsl:apply-templates select="." mode="detail">
                  <xsl:with-param name="parentPath" select="$path"/>
                  <xsl:with-param name="position" select="$pos"/>
                </xsl:apply-templates>
                <xsl:for-each select="@unit">
                  <span class="unit">(<xsl:value-of select="concat(local-name(),':',.)"/>)</span>
                </xsl:for-each>
              </td>
            </tr>
          </xsl:when>
          <xsl:otherwise>
            <tr>
              <td/>
              <td class="key" colspan="3">
                <xsl:value-of select="local-name()"/>
              </td>
              <td class="value {$oddEven}" colspan="11">
                <xsl:apply-templates select="." mode="detail">
                  <xsl:with-param name="parentPath" select="$path"/>
                  <xsl:with-param name="position" select="$pos"/>
                </xsl:apply-templates>
                <xsl:for-each select="@unit">
                  <span class="unit">(<xsl:value-of select="concat(local-name(),':',.)"/>)</span>
                </xsl:for-each>
                <xsl:for-each select="@sn|@sk|@count">
                  <span class="snSk">
                    <xsl:value-of select="concat(local-name(),':',.)"/>
                  </span>
                </xsl:for-each>
                <xsl:if test="$hasErr">
                  <div class="error">
                    <xsl:value-of select="concat($err1,$err2,$err3,$err4)"/>
                  </div>
                </xsl:if>
              </td>
            </tr>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </table>
  </xsl:template>
  <!--  ============================================================================== -->
  <xsl:template match="*[not(*)]" mode="detail">
  <!--  ============================================================================== -->
    <xsl:value-of select="."/>
  </xsl:template>
</xsl:stylesheet>
