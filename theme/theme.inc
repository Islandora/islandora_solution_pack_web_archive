<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Implements hook_preprocess().
 */
function islandora_web_archive_preprocess_islandora_web_archive(&$variables) {
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $islandora_object = $variables['islandora_object'];
  $repository = $islandora_object->repository;
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  // Get DC metadata
  try {
    $dc = $islandora_object['DC']->content;
    $dc_object = DublinCore::importFromXMLString($dc);
  } catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $islandora_object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : NULL;
  $variables['dc_array'] = isset($dc_object) ? $dc_object->asArray() : array();
  $variables['islandora_object_label'] = $islandora_object->label;
  $variables['theme_hook_suggestions'][] = 'islandora_web_archive__' . str_replace(':', '_', $islandora_object->id);
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($islandora_object);
  global $base_url;
  // original
  if (isset($islandora_object['OBJ'])) {
    $warc_url = url("islandora/object/{$islandora_object->id}/datastream/OBJ/view", array('absolute' => TRUE));
    $png_url = url("islandora/object/{$islandora_object->id}/datastream/PNG/view", array('absolute' => TRUE));
    $pdf_url = url("islandora/object/{$islandora_object->id}/datastream/PDF/view", array('absolute' => TRUE));
    $variables['islandora_warc'] = $warc_url;
    $variables['islandora_pdf'] = $pdf_url;
    $variables['islandora_png'] = $png_url;
    $params = array(
      'title' => $islandora_object->label,
      'path' => $warc_url
    );
    $variables['islandora_warc'] = theme('warc', $params);
  }
  // thumbnail
  if (isset($islandora_object['TN'])) {
    $thumbnail_size_url = url("islandora/object/{$islandora_object->id}/datastream/TN/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $thumbnail_size_url
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
  }
  // Medium size.
  if (isset($islandora_object['JPG'])) {
    $medium_size_url = url("islandora/object/{$islandora_object->id}/datastream/JPG/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $medium_size_url,
    );
    $variables['islandora_medium_img'] = theme('image', $params);
    if (isset($png_url)) {
      $variables['islandora_content'] = l($variables['islandora_medium_img'], $png_url, array('html' => TRUE));
    }
    else {
      $variables['islandora_content'] = $variables['islandora_medium_img'];
    }
  }
  // Download warc
  if (isset($islandora_object['OBJ'])) {
    $warc_url = url("islandora/object/{$islandora_object->id}/datastream/OBJ/{$islandora_object->label}.warc", array('absolute' => TRUE));
    $params = array(
      'title' => $islandora_object->label,
      'path' => $warc_url
    );
    $variables['islandora_warc'] = theme('warc', $params);
    if (isset($warc_url)) {
      $variables['islandora_warc'] = l($variables['islandora_object_label'] . '.warc', $warc_url, array('html' => TRUE));
    }
  }

  // Download pdf
  if (isset($islandora_object['PDF'])) {
    $warc_url = url("islandora/object/{$islandora_object->id}/datastream/PDF/{$islandora_object->label}.pdf", array('absolute' => TRUE));
    $params = array(
      'title' => $islandora_object->label,
      'path' => $pdf_url
    );
    $variables['islandora_pdf'] = theme('warc', $params);
    if (isset($pdf_url)) {
      $variables['islandora_pdf'] = l($variables['islandora_object_label'] . '.pdf', $pdf_url, array('html' => TRUE));
    }
  }

  // Download screenshot
  if (isset($islandora_object['PNG'])) {
    $warc_url = url("islandora/object/{$islandora_object->id}/datastream/PNG/{$islandora_object->label}.png", array('absolute' => TRUE));
    $params = array(
      'title' => $islandora_object->label,
      'path' => $png_url
    );
    $variables['islandora_png'] = theme('warc', $params);
    if (isset($png_url)) {
      $variables['islandora_png'] = l($variables['islandora_object_label'] . '.png', $png_url, array('html' => TRUE));
    }
  }
}
