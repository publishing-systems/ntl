<?xml version="1.0" encoding="UTF-8"?>
<!--
Copyright (C) 2012-2017  Stephan Kreutzer

This file is an extension for the
digital_publishing_workflow_tools package.

This file is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License version 3 or any later version,
as published by the Free Software Foundation.

This file is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Affero General Public License 3 for more details.

You should have received a copy of the GNU Affero General Public License 3
along with part of this file. If not, see <http://www.gnu.org/licenses/>.
-->
<xsl:stylesheet version="1.0" xmlns:fo="http://www.w3.org/1999/XSL/Format" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" version="1.0" encoding="UTF-8"/>

  <xsl:param name="page-width">210mm</xsl:param>
  <xsl:param name="page-height">297mm</xsl:param>
  <xsl:param name="outer-margin-odd-pages">20mm 20mm 20mm 20mm</xsl:param>
  <xsl:param name="outer-margin-even-pages">20mm 20mm 20mm 20mm</xsl:param>
  <xsl:param name="inner-margin-odd-pages">0mm 95mm 0mm 0mm</xsl:param>
  <xsl:param name="inner-margin-even-pages">0mm 0mm 0mm 95mm</xsl:param>
  <!-- See http://xmlgraphics.apache.org/fop/trunk/fonts.html -->
  <xsl:param name="font-face">FreeSans</xsl:param>
  <xsl:param name="font-size">8pt</xsl:param>
  <xsl:param name="line-height">9.6pt</xsl:param>

  <xsl:template match="/ntml">
    <fo:root>
      <fo:layout-master-set>
        <fo:simple-page-master page-height="{$page-height}" page-width="{$page-width}" margin="{$outer-margin-odd-pages}" master-name="odd-page">
          <fo:region-body margin="{$inner-margin-odd-pages}"/>
          <fo:region-before region-name="head-odd-pages" extent="5mm" display-align="before"/>
        </fo:simple-page-master>
        <fo:simple-page-master page-height="{$page-height}" page-width="{$page-width}" margin="{$outer-margin-even-pages}" master-name="even-page">
          <fo:region-body margin="{$inner-margin-even-pages}"/>
          <fo:region-before region-name="head-even-pages" extent="5mm" display-align="before"/>
        </fo:simple-page-master>
        <fo:page-sequence-master master-name="page-content">
          <fo:repeatable-page-master-alternatives>
            <fo:conditional-page-master-reference master-reference="odd-page" odd-or-even="odd"/>
            <fo:conditional-page-master-reference master-reference="even-page" odd-or-even="even"/>
          </fo:repeatable-page-master-alternatives>
        </fo:page-sequence-master>
      </fo:layout-master-set>
      <fo:page-sequence master-reference="page-content">
        <fo:static-content flow-name="head-odd-pages">
          <fo:block font-family="{$font-face}" font-size="{$font-size}" line-height="{$line-height}" text-align="right">
            <fo:page-number/>
          </fo:block>
        </fo:static-content>
        <fo:static-content flow-name="head-even-pages">
          <fo:block font-family="{$font-face}" font-size="{$font-size}" line-height="{$line-height}">
            <fo:page-number/>
          </fo:block>
        </fo:static-content>
        <fo:static-content flow-name="xsl-footnote-separator">
          <fo:block text-align-last="justify" space-before="1mm" space-after="1mm">
            <fo:leader leader-length="20mm" rule-thickness="0.5pt" leader-pattern="rule"/>
          </fo:block>
        </fo:static-content>
        <fo:flow flow-name="xsl-region-body">
          <!-- fo:block font-family="{$font-face}" font-size="{$font-size}" line-height="{$line-height}" hyphenate="true" lang="en" -->
          <fo:block font-family="{$font-face}" font-size="{$font-size}" line-height="{$line-height}">
            <xsl:apply-templates select="/ntml/title"/>
            <xsl:apply-templates select="/ntml/p | /ntml/list"/>
          </fo:block>
        </fo:flow>
      </fo:page-sequence>
    </fo:root>
  </xsl:template>

  <xsl:template match="/ntml/title[1]">
    <fo:block font-size="{$font-size} * 1.4" line-height="{$font-size} * 1.3" font-weight="bold" text-align="center" space-after="6mm" space-after.precedence="1" keep-with-next.within-page="always">
      <xsl:value-of select="./text()"/>
    </fo:block>
  </xsl:template>

  <xsl:template match="/ntml/p">
    <fo:block space-after="{$line-height}" text-align="justify">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

  <xsl:template match="/ntml/p/text()">
    <xsl:value-of select="."/>
  </xsl:template>

  <xsl:template match="/ntml/list">
    <fo:list-block>
      <xsl:apply-templates/>
    </fo:list-block>
  </xsl:template>

  <xsl:template match="/ntml/list/item">
    <fo:list-item>
      <fo:list-item-label end-indent="label-end()">
        <fo:block>
          <fo:inline font-family="Symbol">•</fo:inline>
        </fo:block>
      </fo:list-item-label>
      <fo:list-item-body start-indent="body-start()">
        <fo:block>
          <xsl:apply-templates/>
        </fo:block>
      </fo:list-item-body>
    </fo:list-item>
  </xsl:template>

  <xsl:template match="/ntml/list/item/text()">
    <xsl:value-of select="."/>
  </xsl:template>

  <xsl:template match="/ntml/p/highlighted | /ntml/p/list/item/highlighted">
    <fo:inline font-weight="bold">
      <xsl:apply-templates/>
    </fo:inline>
  </xsl:template>

  <xsl:template match="/ntml/p/highlighted/text() | /ntml/p/list/item/highlighted/text()">
    <xsl:value-of select="."/>
  </xsl:template>

  <xsl:template match="node()|@*|text()"/>

</xsl:stylesheet>
