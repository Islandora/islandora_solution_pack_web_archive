# Islandora Web ARChive Solution Pack

[![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_web_archive.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_web_archive)

## Description

Web Archive Solution Pack for Islandora

Adds all required Fedora objects to allow users to ingest and retrieve web archives through the Islandora interface

## Requirements

Install [warctools](https://github.com/internetarchive/warctools) and set the paths for warcindex and warcfilter at admin/islandora/web_archive

## Installation

1. `cd $ISLANDORA_SITE/sites/all/modules && git clone https://github.com/ruebot/islandora_solution_pack_web_archive.git `
2. Enable module: admin/build/modules

## Solr indexing

If you are using Solr 4+, warcs will automatically be indexed via Apache Tika. You will need to add `ds.WARC_FILTERED^1` to the Query fields form in 'admin/islandora/search/islandora_solr/settings'

## License

GPLv3

## Todo

- [x] Display TN
- [x] Display PNG
- [x] Download link for warc
- [x] Download link for PDF
- [x] Solr integration (indexing warcs)
- [x] warctools integration
- [ ] Wayback Machine dissemination
- [ ] Automatic harvesting
