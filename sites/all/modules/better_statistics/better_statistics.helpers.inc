<?php

/**
 * @file
 * Helper functions for the Better Statistics module.
 */


/**
 * Returns field definitions for fields added by this module.
 *
 * @param $description
 *   Boolean value indicating whether or not to include the field description.
 *
 * @return
 *   An array of field definition arrays keyed by field name.
 */
function _better_statistics_define_fields($description = FALSE) {
  $definition['cache'] =  array(
    'type' => 'varchar',
    'length' => 128,
    'not null' => FALSE,
    'description' => 'Cache hit, miss, or not applicable.',
  );
  $definition['user_agent'] = array(
    'type' => 'varchar',
    'length' => 255,
    'not null' => FALSE,
    'description' => 'User-agent string used on the request.',
  );

  // Remove the description field if desired.
  if (!$description) {
    foreach ($definition as &$field) {
      unset($field['description']);
    }
  }

  return $definition;
}


/**
 * Returns Views field data for each of our defined fields.
 *
 * @param $field
 *   The name of the field for which to return data.
 *
 * @return
 *   An array representing the views field.
 *
 * @see better_statistics_views_data_alter()
 */
function _better_statistics_views_field($field) {
  $views_info = FALSE;

  // Define standard string handlers to use on string fields.
  $string_handlers = array(
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  switch ($field) {
    case 'cache':
      $views_info = array(
        'title' => t('Cache status'),
        'help' => t('The cache status of the page (HIT, MISS, or NULL).'),
      );

      // Append standard string handlers.
      $views_info += $string_handlers;
      break;

    case 'user_agent':
      $views_info = array(
        'title' => t('User-agent'),
        'help' => t('User-agent string of the user who visited your page.'),
      );

      // Append standard string handlers.
      $views_info += $string_handlers;
      break;
  }

  return $views_info;
}


/**
 * Returns the cache status for the current page load.
 */
function _better_statistics_page_is_cached() {
  $cache_status = NULL;
  global $user;

  // Don't bother if caching isn't even enabled.
  if (!variable_get('cache', 0)) {
    return $cache_status;
  }

  // Calling page_get_cache(TRUE) can return either TRUE or FALSE. If TRUE, the
  // page is cacheable, but there was a cache miss. If FALSE, then it's either
  // a cache hit, or the page is not cacheable.
  if (page_get_cache(TRUE)) {
    $cache_status = 'MISS';
  }
  else {
    // This is taken directly from page_get_cache(). If this evaluates to TRUE,
    // then we must have a cache hit. Otherwise, the page was not cacheable to
    // begin with.
    if (!$user->uid && $_SERVER['REQUEST_METHOD'] == 'GET' && count(drupal_set_message()) == 0 && $_SERVER['SERVER_SOFTWARE'] !== 'PHP CLI') {
      $cache_status = 'HIT';
    }
  }

  return $cache_status;
}
