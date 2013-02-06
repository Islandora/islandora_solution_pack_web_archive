# Islandora Web ARChive Solution Pack

## Description

Web Archive Solution Pack for Islandora

Load all required Fedora Objects, and creates empty collection object to accept web archive files.

## Requirements

Requirements: https://github.com/internetarchive/wayback (not required, yet!)

## Installation

1. ` cd $ISLANDORA_SITE/sites/all/modules && git clone https://github.com/ruebot/islandora_solution_pack_web_archive.git `
2. Enable module: admin/build/modules

## Solr indexing

If you would like to be able to full text search your warc:

1. add the following to your `demoFoxmlToSolr.xslt`.

``xml

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

* ~~Provide warc download link~~
* Index warcs in Solr
* Incorporate wayback machine integration
* Drupal 7 version
* Edit form to include link to local Wayback Machine
