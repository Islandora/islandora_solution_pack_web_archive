<?php

class WarcCreation {

  function WarcCreation() {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
  }

  function get_thumbnail($parameterArray, $dsid, $file, $file_ext) {

    $stored_thumbnail = drupal_get_path('module', 'islandora_web_archive_sp') . '/images/warc_TN.png';
    file_copy($stored_thumbnail, file_directory_path());
    $thumbnail = file_directory_path() . '/' . 'warc_TN.png';
    $_SESSION['fedora_ingest_files']["TN"] = $thumbnail;
    return TRUE;
  }
}
