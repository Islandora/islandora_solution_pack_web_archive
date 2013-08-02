<?php
/**
 * @file
 * Holds administration functionality for the web archive solution pack.
 */

/**
 * Islandora video administration settings.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The state of the form.
 *
 * @return array
 *   The form definition.
 */
function islandora_web_archive_admin($form, &$form_state) {
  $form = array();
  module_load_include('inc', 'islandora', 'includes/solution_packs');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('islandora_web_archive_admin_submit'),
  );

  $form['islandora_web_archive_wkhtmltopdf_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to wkhtmltopdf executable.'),
    '#default_value' => variable_get('islandora_web_archive_wkhtmltopdf_path', 'wkhtmltopdf'),
    '#description' => t('Path to wkhtmltopdf on your server.'),
    '#required' => TRUE,
  );

  $form['islandora_web_archive_make_wkhtmltopdf_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create wkhtmltopdf locally ?'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => variable_get('islandora_web_archive_make_wkhtmltopdf_locally', TRUE),
  );

  $form['islandora_web_archive_wkhtmltoimage_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to wkhtmltoimage executable.'),
    '#default_value' => variable_get('islandora_web_archive_wkhtmltoimage_path', 'wkhtmltoimage'),
    '#description' => t('Path to wkhtmltoimage on your server.'),
    '#required' => TRUE,
  );

  $form['islandora_web_archive_make_wkhtmltoimage_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create wkhtmltoimage locally ?'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => variable_get('islandora_web_archive_make_wkhtmltoimage_locally', TRUE),
  );

  $form['islandora_web_archive_pngcrush_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pngcrush executable.'),
    '#default_value' => variable_get('islandora_web_archive_pngcrush_path', 'pngcrush'),
    '#description' => t('Path to pngcrush on your server.'),
    '#required' => TRUE,
  );

  $form['islandora_web_archive_make_pngcrush_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('pngcrush locally ?'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => variable_get('islandora_web_archive_make_pngcrush_locally', TRUE),
  );

  $form['islandora_web_archive_wget_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to wget executable.'),
    '#default_value' => variable_get('islandora_web_archive_wget_path', 'wget'),
    '#description' => t('Path to wget on your server.'),
    '#required' => TRUE,
  );

  $form['islandora_web_archive_make_warc_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create warc locally ?'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => variable_get('islandora_web_archive_make_warc_locally', TRUE),
  );
  return system_settings_form($form);
}

/**
 * Form submit for allowing the deletion of the viewer variable.
 */
function islandora_web_archive_admin_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_web_archive_wkhtmltopdf_path');
      variable_del('slandora_web_archive_make_wkhtmltopdf_locally');
      variable_del('islandora_web_archive_wkhtmltoimage_path');
      variable_del('slandora_web_archive_make_wkhtmltoimage_locally');
      variable_del('islandora_web_archive_pngcrush_path');
      variable_del('islandora_web_archive_make_pngcrush_locally');
      variable_del('islandora_web_archive_wget_path');
      variable_del('islandora_web_archive_make_warc_locally');
      break;
  }
}
