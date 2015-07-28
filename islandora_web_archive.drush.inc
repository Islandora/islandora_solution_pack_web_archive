<?php

/**
 * @file
 * Implementation of Drush hooks.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_web_archive_drush_command() {
  return array(
    'islandora_web_archive_migrate_png_to_screenshot' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_MAX,
      'drupal_dependencies' => array(
        'islandora',
      ),
      'description' => 'Iterates each web archive object and copies the PNG datastream into a SCREENSHOT datastream.',
      'examples' => array(
        'drush -u 1 islandora_web_archive_migrate_png_to_screenshot',
      ),
    ),
  );
}

/**
 * Implements hook_drush_command().
 */
function drush_islandora_web_archive_migrate_png_to_screenshot() {
  // Get all islandora web archive objects.
  $query = <<<EOQ
    PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
    SELECT DISTINCT ?pid
    FROM <#ri>
    WHERE {
      ?pid <fedora-model:hasModel> <info:fedora/islandora:sp_web_archive>
    }
EOQ;
  $connection = islandora_get_tuque_connection();
  $archive_pids = $connection->repository->ri->sparqlQuery($query);
  module_load_include('inc', 'islandora_web_archive', 'includes/utilities');
  foreach ($archive_pids as $pid) {
    $obj = islandora_object_load($pid['pid']['value']);
    // Create a new SCREENSHOT ds from PNG ds if one is not already created.
    if (!isset($obj['SCREENSHOT']) && isset($obj['PNG'])) {
      islandora_web_archive_clone_datastream($obj, $obj['PNG'], 'SCREENSHOT');
      drush_print(dt("Successfully migrated !SRC to !DEST datastream for object: !PID.\n", array(
        '!PID' => $pid['pid']['value'],
        '!SRC' => 'PNG',
        '!DEST' => 'SCREENSHOT',
      )));
    }
  }
}
