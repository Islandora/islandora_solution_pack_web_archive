<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Clones datastream within same object.
 *
 * @param abstractObject $object
 *   Object containing datastream to be cloned.
 * @param string $existing_datastream
 *   DSID of an existing datastream to be cloned.
 * @param string $new_dsid
 *   DSID of new datastream.
 */
function islandora_web_archive_clone_datastream($object, $existing_datastream, $new_dsid) {
  $temp_file = drupal_tempnam('temporary://', 'datastream');
  $existing_datastream->getContent($temp_file);
  $new_datastream = $object->constructDatastream($new_dsid);
  $new_datastream->label = $new_dsid;
  $new_datastream->mimetype = $existing_datastream->mimetype;
  $new_datastream->setContentFromFile($temp_file);
  $new_datastream->logMessage = "Copied {$existing_datastream->id} to $new_dsid";
  $object->ingestDatastream($new_datastream);
  drupal_unlink($temp_file);
}
