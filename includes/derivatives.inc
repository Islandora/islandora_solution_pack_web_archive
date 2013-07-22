<?php

/**
 * @file
 *
 * This file contains all the functions for warc manipulation used in the
 * web archive solution pack.
 */

/**
 * Creates the derivatives for this content model type
 *
 * @param FedoraObject $object
 *  The object to add derivatives to.
 *
 *  @return boolean
 *    TRUE if all derivatives were created successfully, FALSE otherwise.
 */
function islandora_web_archive_create_all_derivatives(AbstractObject $object) {
  if (!isset($object["PNG"])) {
    drupal_set_message(t("Could not create derivatives for @s.  No warc file was uploaded.", array("@s" => $object->id)), "error");
    return FALSE;
  }
  $base_name = str_replace(':', '-', $object->id);
  $uploaded_file = islandora_web_archive_get_uploaded_file($object, $base_name);
  if ($uploaded_file) {
    $jpg = islandora_web_archive_create_JPG_derivative($object, $uploaded_file, $base_name);
    $tn = islandora_web_archive_create_TN_derivative($object, $uploaded_file, $base_name);
    return $jpg && $tn && file_unmanaged_delete($uploaded_file);
  }
  return FALSE;
}

/**
 * Fetches the upload image file from Fedora, and saves it to a temp location.
 *
 * @param FedoraObject $object
 *  The object to fetch the uploaded image file from.
 * @param string $base_name
 *  The base name to use in creating temp files.
 *
 * @return string
 *  The file path to the temp file if successful, FALSE otherwise.
 */
function islandora_web_archive_get_uploaded_file(AbstractObject $object, $base_name) {
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($object['PNG']->mimeType);
  $filename = file_create_filename("{$base_name}_PNG.{$ext}", 'temporary://');
  $object['PNG']->getContent($filename);
  return $filename;
}

/**
  * Creates the JPG derived datatstream from the uploaded image file.
  *
  * @param FedoraObject $object
  *   The object to add the derived datastream to.
  * @param string $uploaded_file
  *   The file path to the uploaded image file to derive from.
  * @param string $base_name
  *   The base name to use in creating temp files.
  *
  * @return boolean
  *   TRUE if successful, FALSE otherwise.
  */
function islandora_web_archive_create_JPG_derivative(AbstractObject $object, $uploaded_file, $base_name) {
  $args = array();
  $args[] = '-resize ' . escapeshellarg("600 x 800");
  $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
  $derivative_file = islandora_web_archive_imagemagick_convert($uploaded_file, "temporary://{$base_name}_MED.jpg", $args);
  if ($derivative_file === FALSE) {
    drupal_set_message(t('Failed to create JPG derivative'), 'error');
    return FALSE;
  }
  islandora_web_archive_add_datastream($object, 'JPG', $derivative_file, 'image/jpeg', t('Medium sized JPEG'));
  return file_unmanaged_delete($derivative_file);
}

/**
  * Creates the TN derived datatstream from the uploaded image file.
  *
  * @param FedoraObject $object
  *   The object to add the derived datastream to.
  * @param string $uploaded_file
  *   The file path to the uploaded image file to derive from.
  * @param string $base_name
  *   The base name to use in creating temp files.
  *
  * @return boolean
  *   TRUE if successful, FALSE otherwise.
  */
function islandora_web_archive_create_TN_derivative(AbstractObject $object, $uploaded_file, $base_name) {
  $args = array();
  $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
  $args[] = '-resize ' . escapeshellarg("200 x 200");
  $derivative_file = islandora_web_archive_imagemagick_convert($uploaded_file, "temporary://{$base_name}_TN.jpg", $args);
  if ($derivative_file === FALSE) {
    drupal_set_message(t('Failed to create TN derivative'), 'error');
    return FALSE;
  }
  islandora_web_archive_add_datastream($object, 'TN', $derivative_file, 'image/jpeg', t('Thumbnail'));
  return file_unmanaged_delete($derivative_file);
}

/**
  * Calls imagemagick's convert command with the given arguments.
  *
  * @param string $src
  *   The source file uri to convert from.
  * @param string $dest
  *   The destination file uri to convert into.
  * @param int $width
  *   The source file to convert from.
  * @param int $height
  *   The destination file to convert into.
  *
  * @return string
  *   The destination file path if successful otherwise FALSE.
  */
function islandora_web_archive_imagemagick_convert($src, $dest, $args) {
  $src = drupal_realpath($src) . '[0]';
  $dest = drupal_realpath($dest);
  $context = array(
    'source' => $src,
    'destination' => $dest,
  );
  drupal_alter('imagemagick_arguments', $args, $context);
  /**
   * To make use of ImageMagick 6's parenthetical command grouping we need to make the $source image the first parameter and $dest the last.
   * See http://www.imagemagick.org/Usage/basics/#cmdline
   */
   $command = escapeshellarg($src) . ' ' . implode(' ', $args) . ' ' . escapeshellarg($dest);
   $output = '';
   $ret = -1;
   if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {
     $message = 'ImageMagick failed to create derivatives<br/>Error: @ret<br/>Command: @command<br/>Output: @output';
     $variables = array('@ret' => $ret, '@command' => $command, '@output' => $output);
     watchdog('islandora_web_archive', $message, $variables, WATCHDOG_ERROR);
     return FALSE;
   }
   return $dest;
}

/**
  * Adds a datastream to an object.
  *
  * @param object $object
  *   The object to add a datastream to.
  * @param string $dsid
  *   The datastream ID to be added.
  * @param string $file
  *   The URI of the file to be added as the datastream content.
  * @param string $mimetype
  *   The mimetype of the file.
  * @param string $label
  *   The label for the datastream.
  *
  * @TODO: could this be a generic function? eg: islandora_add_datastream($object, $dsid, $file). ?
  */
function islandora_web_archive_add_datastream(AbstractObject $object, $dsid, $file, $mimetype, $label) {
  $ds = $object->constructDatastream($dsid, 'M');
  $ds->label = $label;
  $ds->mimeType = $mimetype;
  $ds->setContentFromFile($file);
  try {
    $object->ingestDatastream($ds);
  } catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
    return FALSE;
  }
  return TRUE;
}

/**
  * Uses Imagemagick's identify to examine the file in question.
  *
  * @param string $file
  *   A file-system path to the file in question.
  *
  * @return boolean
  *   A boolean indicating if the file contains a TIFF which is not compressed,
  *   (if it could be reencoded by the "free" version of Kakadu.
  */
function islandora_large_image_is_uncompressed_png($file) {
  $identify = islandora_large_image_get_identify();
  $file = drupal_realpath($file);
  $compression = exec(escapeshellcmd("$identify -format \"%C\" $file"));
  $compressed = (strtolower($compression) != 'none');
  return !$compressed && islandora_web_archive_is_png($file);
}

/**
  * Uses Imagemagick's identify to determine if the file is a PNG.
  *
  * @param string $file
  *   A file-system path to the file in question.
  *
  * @return boolean
  *   A boolean indicating if the file contains a PNG.
  */
function islandora_web_archive_is_png($file) {
  $identify = islandora_web_archive_get_identify();
  $file = drupal_realpath($file);
  $codec = exec(escapeshellcmd("$identify -format \"%m\" $file"));
  $is_png = (strtolower($codec) == 'png');
  return $is_png;
}

/**
  * Attempt to get the "identify" executable...
  *
  * Assume convert is sanely named, and that identify can be obtained from the
  * same place.
  *
  * @return string
  *   The command to run for "identify".
  */
function islandora_web_archive_get_identify() {
  //Get path for convert
  $convert = variable_get('imagemagick_convert', 'convert');

  // Replace "convert" with "identify"
  $identify = str_replace('convert', 'identify', $convert);
  return $identify;
}
