<?php
/**
 * @file
 * This file contains all the functions for warc manipulation used in the
 * web archive solution pack.
 */

/**
 * creates the derivatives for this content model type
 * @param object $object
 *   tuque FedoraObject
 * @return string
 */
function islandora_web_archive_create_all_derivatives($object) {
  $mime_detect = new MimeDetect();
  if (!isset($object['OBJ'])) {
    drupal_set_message(t('Could not create warc derivatives for %s.  No warc file was uploaded.', array('%s' => $object->id)), 'error');
    return "";
  }
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);

  // Create a file object we can save.
  $file_uri = file_create_filename("{$file_name}OBJ.{$ext}", 'temporary://');
  $file = new stdClass();
  $file->uri = $file_uri;
  $file->filename = $file_name;
  $file->filemime = $object['OBJ']->mimeType;
  // Temporary.
  $file->status = 0;
  $object['OBJ']->getContent($file_uri);
  $original_file = file_save($file);

  file_delete($original_file);
}


/**
 * adds a datastream and deletes the tmp file from local file system
 * @param object $object
 * @param string $dsid
 * @param object $file
 *
 * @TODO: could this be a generic function? eg: islandora_add_datastream($object, $dsid, $file). ?
 */
function islandora_web_archive_add_datastream($object, $dsid, $file) {
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $dsid;
    $ds->mimeType = $object['OBJ']->mimeType;
    $ds->setContentFromFile(drupal_realpath($file->uri));
    $object->ingestDatastream($ds);
    file_delete($file);
  } catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
  }
}
