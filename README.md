# Islandora Web ARChive Solution Pack

## Description

Web Archive Solution Pack for Islandora

Adds all required Fedora objects to allow users to ingest and retrieve web archives through the Islandora interface

## Requirements

1. [wget 1.14+](http://ftp.gnu.org/gnu/wget/) (**not required, yet!**)
2. [wkhtmltopdf](http://code.google.com/p/wkhtmltopdf/) `sudo apt-get instll wkhtmltopdf` (**not required, yet!**)
3. [wkhtmltoimage](http://code.google.com/p/wkhtmltopdf/downloads/detail?name=wkhtmltoimage-0.11.0_rc1-static-amd64.tar.bz2&can=2&q=) (put in path) (**not required, yet!**)
4. [pngcrush](http://pmt.sourceforge.net/pngcrush/) `sudo apt-get install pngcrush` (**not required, yet!**)
5. [Wayback Machine](https://github.com/internetarchive/wayback) (**not required, yet!**)

## Installation

1. ` cd $ISLANDORA_SITE/sites/all/modules && git clone https://github.com/ruebot/islandora_solution_pack_web_archive.git `
2. Enable module: admin/build/modules

## Solr indexing

If you would like to be able to full text search your warc:

1. add the following to your `demoFoxmlToSolr.xslt`.

```xml

<xsl:variable name="CModel">
  <xsl:value-of select="foxml:datastream[@ID='RELS-EXT']/foxml:datastreamVersion[last()]/foxml:xmlContent//fedora-model:hasModel/@rdf:resource"/>
</xsl:variable>

<xsl:if test="$CModel = 'islandora:sp_web_archive'">
  <xsl:for-each select="foxml:datastream[@ID='OBJ']/foxml:datastreamVersion[last()]">
    <field>
      <xsl:attribute name="name">
        <xsl:value-of select="concat('OBJ.', 'OBJ')"/>
      </xsl:attribute>
      <xsl:value-of select="islandora-exts:getDatastreamTextRaw($PID, $REPOSITORYNAME, 'OBJ', $FEDORASOAP, $FEDORAUSER, $FEDORAPASS, $TRUSTSTOREPATH, $TRUSTSTOREPASS)"/>
    </field>
  </xsl:for-each>
</xsl:if>

```

2. Restart the Islandora/Fedora stack
3. Reindex Solr http://your.site.ca:8080/fedoragsearch/rest?operation=updateIndex

## License

GPLv3 - [Standard Islandora license](http://islandora.ca/about)

## Todo

- [x] Display TN
- [x] Display PNG
- [x] Download link for warc
- [x] Download link for PDF
- [ ] Solr integration (indexing warcs)
- [ ] Wayback Machine dissemination
- [ ] Automatic harvesting
