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

class DisplayWarcStreams {

  private $pid = NULL;

  /**
   * Constructor
   * @param type $pid
   */
  function DisplayWarcStreams($pid) {
    //drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    $this->pid = $pid;
  }

  function showWarcLink() {
    global $base_url;
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $item = new fedora_item($this->pid);
    $title = $item->objectProfile->objLabel;
    $collection_fieldset = array(
      '#collapsible' => FALSE,
      '#value' => '<div><a href="' . $base_url . '/fedora/repository/' . $this->pid . '/OBJ/' . $title . '.warc">Download ' . $title . '.</a></div>',
    );
    $tabset = array();
    $tabset['warc'] = array(
      '#type' => 'tabpage',
      '#title' => $title,
      '#content' => theme('fieldset', $collection_fieldset),
    );

    return $tabset;
  }
}
