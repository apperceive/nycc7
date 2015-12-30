<?php
/**
 * @file
 * carousel.inc
 *
 * Various Bootstrap Carousel functions.
 */

/**
 * Helper function for returning the default values of a carousel object.
 */
function bootstrap_field_carousel_defaults() {
  static $defaults;
  if (!isset($defaults)) {
    $defaults = (object) array(
      'fid' => 0,
      'entity_id' => 0,
      'entity_type' => '',
      'entity_bundle' => '',
      'settings' => array(
        'title' => '',
        'description' => '',
        'url' => '',
        'url_new_window' => 1,
      ),
    );
  }
  // Clone the object, otherwise it will keep the changes made to it.
  return clone $defaults;
}

/**
 * Helper function for retrieving Bootstrap Carousel slide items.
 *
 * @param array|int $fids
 *   An indexed array of {file_managed}.fid or an single fid integer.
 * @param array $conditions
 *   An associative array of conditions which to filter by. Can be one of:
 *     - entity_id: An entity identifier.
 *     - entity_type: An entity type.
 *     - entity_bundle: An entity bundle type.
 *
 * @return array
 *   An associative array where the key is the fid associated with this slide
 *   and the value is the DB entry object containing:
 *     - fid: The {file_managed}.fid association.
 *     - entity_id: An entity identifier.
 *     - entity_type: An entity type.
 *     - entity_bundle: An entity bundle type.
 *     - title: The slide title.
 *     - description: The slide description.
 *     - url: The slide URL.
 */
function bootstrap_field_load_carousel($fids = array(), $conditions = array()) {
  if (!is_array($fids)) {
    $fids = array((int) $fids);
  }

  // Default values.
  $defaults = bootstrap_field_carousel_defaults();
  if (isset($conditions['entity_id'])) {
    $defaults->entity_id = (int) $conditions['entity_id'];
  }
  elseif (isset($conditions['entity']) && isset($conditions['entity_type'])) {
    $info = entity_get_info($conditions['entity_type']);
    $key = isset($info['entity keys']['name']) ? $info['entity keys']['name'] : $info['entity keys']['id'];
    $defaults->entity_id = isset($conditions['entity']->$key) ? (int) $conditions['entity']->$key : 0;
  }
  if (isset($conditions['entity_type'])) {
    $defaults->entity_type = $conditions['entity_type'];
  }
  if (isset($conditions['entity_bundle'])) {
    $defaults->entity_bundle = $conditions['entity_bundle'];
  }

  // Retrieve database entry. If a field instance is provided, check to see
  // if it has the bootstrap_carousel fields enabled (i.e. prevent unnecessary
  // database calls).
  $results = array();
  if (!isset($conditions['instance']['widget']['settings']) || $conditions['instance']['widget']['settings']['bootstrap_carousel']) {
    $query = db_select('bootstrap_carousel', 'bc')->fields('bc');
    if ($fids) {
      $query->condition('fid', $fids);
    }
    if (isset($conditions['entity_id'])) {
      // We must use an OR condition here. New entities that are saved do not
      // yet have an entity ID, so a temporary -1 value is used instead.
      $query->condition(db_or()->condition('entity_id', $conditions['entity_id'])->condition('entity_id', -1));
    }
    if (isset($conditions['entity_type'])) {
      $query->condition('entity_type', $conditions['entity_type']);
    }
    if (isset($conditions['entity_bundle'])) {
      $query->condition('entity_bundle', $conditions['entity_bundle']);
    }
    $results = $query->execute()->fetchAllAssoc('fid');
  }

  // Iterate over each result.
  $integers = array('cid', 'fid', 'entity_id');
  foreach ($results as $fid => $result) {
    $results[$fid] = clone $defaults;
    foreach ((array) $result as $name => $value) {
      if (empty($value)) {
        continue;
      }
      elseif (in_array($name, $integers)) {
        $value = (int) $value;
      }
      elseif ($name === 'settings') {
        $value = unserialize($value);
      }
      $results[$fid]->$name = $value;
    }
  }

  // Always provide default valued results for each of the $fids.
  if (!$results) {
    foreach ($fids as $fid) {
      $results[$fid] = $defaults;
      $results[$fid]->fid = $fid;
    }
  }

  return $results;
}

/**
 * A callback for #process.
 *
 * Expands the image_image type to include the Bootstrap Carousel fields.
 */
function bootstrap_field_image_widget_carousel_process($element, &$form_state, $form) {
  $fid = isset($element['#value']['fid']) ? (int) $element['#value']['fid'] : 0;

  // Retrieve the database settings for this Bootstrap Carousel image.
  $conditions = array(
    'entity_type' => $element['#entity_type'],
    'entity_bundle' => $element['#bundle'],
    'entity' => $element['#entity'],
  );
  $carousel = bootstrap_field_load_carousel($fid, $conditions);

  $element['bootstrap_carousel'] = array(
    '#access' => (bool) $fid,
    '#type' => 'fieldset',
    '#title' => t('Slide settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  // Add the additional Bootstrap Carousel fields.
  $element['bootstrap_carousel']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => $carousel[$fid]->settings['title'],
    '#description' => t('This text will be used as the slide title.'),
    '#maxlength' => 512,
    '#attributes' => array(
      'class' => array(
        'input-sm',
      ),
    ),
  );
  $element['bootstrap_carousel']['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#description' => t('This text will be used as the slide description.'),
    '#attributes' => array(
      'class' => array(
        'input-sm',
      ),
    ),
  );
  if (isset($carousel[$fid]->settings['description']['value'])) {
    $element['bootstrap_carousel']['description']['#default_value'] = $carousel[$fid]->settings['description']['value'];
  }
  if (isset($carousel[$fid]->settings['description']['format'])) {
    $element['bootstrap_carousel']['description']['#format'] = $carousel[$fid]->settings['description']['format'];
  }
  $element['bootstrap_carousel']['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#default_value' => $carousel[$fid]->settings['url'],
    '#description' => t('This text will be used as the URL to wrap the slide image with a link. It may be an internal path (i.e %node) or an absolute url (i.e. %url). If this is an internal path, it will be subject to permission checks. If a user does not have access, the link will not show.', array(
      '%node' => 'node/1',
      '%url' => 'http://www.example.com',
    )),
    '#maxlength' => 1024,
    '#element_validate' => array('bootstrap_field_image_widget_carousel_url_validate'),
    '#attributes' => array(
      'class' => array(
        'input-sm',
      ),
    ),
  );
  $element['bootstrap_carousel']['url_new_window'] = array(
    '#title' => t('Open URL in new window'),
    '#type' => 'checkbox',
    '#default_value' => $carousel[$fid]->settings['url_new_window'],
    '#states' => array(
      'invisible' => array(
        ':input[name="' . $element['#name'] . '[bootstrap_carousel][url]"]' => array('value' => ''),
      ),
    ),
  );
  return $element;
}

/**
 * A validation callback for a slide's URL setting.
 */
function bootstrap_field_image_widget_carousel_url_validate($element, &$form_state, $form) {
  if (!empty($element['#value']) && !drupal_valid_path($element['#value'])) {
    form_error($element, t('Invalid Bootstrap Carousel slide URL.'));
  }
}

/**
 * A callback for #file_value_callbacks.
 *
 * Saves the title, description and URL values to the database.
 */
function bootstrap_field_image_widget_carousel_value(&$element, $input, $form_state) {
  // Only process existing files and those fields setup as a Bootstrap Carousel.
  if ($fid = isset($element['#default_value']['fid']) ? (int) $element['#default_value']['fid'] : 0) {
    $primary_keys = array();

    // Retrieve the database entry, in which case this would be an update.
    $conditions = array(
      'entity_type' => $element['#entity_type'],
      'entity_bundle' => $element['#bundle'],
      'entity' => $element['#entity'],
    );
    $carousel = bootstrap_field_load_carousel($fid, $conditions);

    // Populate carousel slide settings with data from $input.
    if (isset($input['bootstrap_carousel']) && is_array($input['bootstrap_carousel'])) {
      foreach ($input['bootstrap_carousel'] as $name => $value) {
        $carousel[$fid]->settings[$name] = $value;
      }
    }

    // Determine if the primary key is present, in which case this would be
    // an update.
    if (isset($carousel[$fid]->cid)) {
      $primary_keys[] = 'cid';
    }

    // Save the entry.
    drupal_write_record('bootstrap_carousel', $carousel[$fid], $primary_keys);
  }
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function bootstrap_field_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $image_styles = image_style_options(FALSE, PASS_THROUGH);
  $element['bootstrap_carousel_image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image style'),
    '#default_value' => $settings['bootstrap_carousel_image_style'],
    '#empty_option' => t('None (original image)'),
    '#options' => $image_styles,
  );
  $element['interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Interval'),
    '#description' => t('The amount of time, in milliseconds, to delay between automatically cycling an item. If <code>0</code>, carousel will not automatically cycle. Default: <code>5000</code>.'),
    '#default_value' => $settings['interval'],
  );
  $element['controls'] = array(
    '#type' => 'checkbox',
    '#title' => t('Controls'),
    '#description' => t('Show the previous/next arrowed controls.'),
    '#default_value' => $settings['controls'],
  );
  $element['indicators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Indicators'),
    '#description' => t('Show the indicators used to determine which slide is currently active.'),
    '#default_value' => $settings['indicators'],
  );
  $element['pause'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause on hover'),
    '#description' => t('Pauses the cycling of the carousel on mouseenter and resumes the cycling of the carousel on mouseleave.'),
    '#default_value' => $settings['pause'],
  );
  $element['wrap'] = array(
    '#type' => 'checkbox',
    '#title' => t('Continuous'),
    '#description' => t('Whether the carousel should cycle continuously or have hard stops.'),
    '#default_value' => $settings['wrap'],
  );
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function bootstrap_field_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  $image_styles = image_style_options(FALSE, PASS_THROUGH);
  // Unset possible 'No defined styles' option.
  unset($image_styles['']);
  if (isset($image_styles[$settings['bootstrap_carousel_image_style']])) {
    $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['bootstrap_carousel_image_style']]));
  }
  // Styles could be lost because of enabled/disabled modules that defines
  // their styles in code.
  elseif (!empty($settings['bootstrap_carousel_image_style'])) {
    $summary[] = t('Image style: Original image (broken: @style)', array('@style' => $settings['bootstrap_carousel_image_style']));
  }
  else {
    $summary[] = t('Original image');
  }

  $summary[] = t('Interval: @value', array(
    '@value' => check_plain($settings['interval']),
  ));
  $summary[] = t('Controls: @value', array(
    '@value' => $settings['controls'] ? t('yes') : t('no'),
  ));
  $summary[] = t('Indicators: @value', array(
    '@value' => $settings['indicators'] ? t('yes') : t('no'),
  ));
  $summary[] = t('Pause on hover: @value', array(
    '@value' => $settings['pause'] ? t('yes') : t('no'),
  ));
  $summary[] = t('Continuous: @value', array(
    '@value' => $settings['wrap'] ? t('yes') : t('no'),
  ));

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function bootstrap_field_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  // Extract the file IDs.
  $fids = array();
  foreach ($items as $item) {
    $fids[] = (int) $item['fid'];
  }

  // Load the field slide settings.
  $conditions = array(
    'entity_type' => $entity_type,
    'entity_bundle' => $instance['bundle'],
    'entity' => $entity,
    'instance' => $instance,
  );
  $carousel = bootstrap_field_load_carousel($fids, $conditions);

  // Construct the bootstrap_carousel items.
  $images = array();
  foreach ($items as $delta => $item) {
    // Render the description in the appropriate format.
    if (isset($carousel[$item['fid']]->settings['description']['format']) && isset($carousel[$item['fid']]->settings['description']['value'])) {
      $carousel[$item['fid']]->settings['description'] = check_markup($carousel[$item['fid']]->settings['description']['value'], $carousel[$item['fid']]->settings['description']['format'], $langcode);
    }

    // Determine if the image should be linked.
    $path = '';
    if ($carousel[$item['fid']]->settings['url']) {
      $path = array(
        'path' => $carousel[$item['fid']]->settings['url'],
        'options' => array(),
      );
      if ($carousel[$item['fid']]->settings['url_new_window']) {
        $path['options']['attributes']['target'] = '_blank';
      }
    }

    // Ensure the image doesn't overflow its container.
    $item['attributes']['class'][] = 'img-responsive';

    // Create the carousel item.
    $images[$delta] = array(
      'image' => array(
        '#theme' => 'image_formatter',
        '#item' => $item,
        '#image_style' => $display['settings']['bootstrap_carousel_image_style'],
        '#path' => $path,
      ),
      'title' => $carousel[$item['fid']]->settings['title'],
      'description' => $carousel[$item['fid']]->settings['description'],
    );
  }
  return array(
    '#theme' => 'bootstrap_carousel',
    '#items' => $images,
    '#interval' => (int) $display['settings']['interval'],
    '#controls' => (bool) $display['settings']['controls'],
    '#indicators' => (bool) $display['settings']['indicators'],
    '#pause' => (bool) $display['settings']['pause'],
    '#wrap' => (bool) $display['settings']['wrap'],
    // Emulate field attributes as this is bypassing a normal field template.
    '#attributes' => array(
      'id' => drupal_html_id($field['field_name']),
      'class' => array(
        'field',
        'field-name-' . drupal_html_class($field['field_name']),
        'field-type-' . drupal_html_class($field['type']),
        'field-label-' . drupal_html_class($display['label']),
      ),
    ),
  );
}
