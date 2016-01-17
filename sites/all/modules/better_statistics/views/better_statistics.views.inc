<?php

/**
 * @file
 * Views hooks for the Better Statistics module.
 */


/**
 * Implements hook_views_data_alter().
 *
 * Exposes the fields this module's added to the accesslog to views.
 */
function better_statistics_views_data_alter(&$data) {
  // Fetch the fields defined by this module.
  module_load_include('inc', 'better_statistics', 'better_statistics.helpers');
  $fields = _better_statistics_define_fields();

  // For each field, expose the field to Views.
  foreach ($fields as $name => $field) {
    $data['accesslog'][$name] = _better_statistics_views_field($name);
  }
}
