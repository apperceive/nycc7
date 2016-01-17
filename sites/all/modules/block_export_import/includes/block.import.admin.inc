<?php

/**
 * @file
 * Import all system specific blocks.
 */

/**
 * Form constructor for import system specific blocks.
 *
 * @see block_export_import_blocks_import_form_submit()
 * @see import_block_form_submit()
 */
function block_export_import_blocks_import_form(&$form_state) {
  $block_export_import_path = drupal_get_path('module', 'block_export_import');
  drupal_add_css($block_export_import_path . '/css/block_export_import.css');

  if (empty($form_state['storage']['values'])) {
    $form['import_code'] = array(
        '#type' => 'textarea',
        '#title' => t('Import'),
        '#required' => TRUE,
        '#rows' => 15,
        '#description' => t('Paste an exported blocks array structure here.'),
    );

    $override_desc = t("If selected '<b>Yes</b>', any existing System Block");
    $override_desc .= t(" with the same identifier will be replaced by the");
    $override_desc .= t(" import.");
    $form['override_existing'] = array(
        '#type' => 'radios',
        '#title' => t('Override'),
        '#options' => array(1 => t('Yes'), -1 => t('No')),
        '#required' => TRUE,
        '#description' => $override_desc,
    );
  }

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Import Blocks'),
      '#prefix' => '<div class="import-block-submit">',
      '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit handler for import_custom_blocks_form() to import/save a block.
 *
 * @see block_export_import_block_import_process()
 * @see _block_export_import_validate_import_data()
 */
function block_export_import_blocks_import_form_submit($form, &$form_state) {
  if (empty($form_state['storage']['values'])) {
    $override_existing = $form_state['values']['override_existing'];
    $block = array();
    $string = $form_state['values']['import_code'];
    @eval($string);
    if (is_array($block)) {
      $php_filter_module_status = module_exists('php');
      foreach ($block as $block_data) {
        if (empty($block_data['block_custom']['info'])) {
          $error = t('Please ensure that each block description is unique.');
          form_set_error('info', $error);
        }
        else {
          block_export_import_block_import_process($block_data, $override_existing, $php_filter_module_status);
        }
      }
      cache_clear_all();
      $form_state['rebuild'] = TRUE;
    }
  }
  else {
    // Do nothing.
  }
  drupal_goto('admin/build/export-import-block/export');
}

/**
 * The Process steps to create a system specific block.
 *
 * @see _block_export_import_get_block_fields()
 * @see _block_export_import_import_visibility_settings()
 */
function block_export_import_block_import_process($block_data, $override_existing, $php_filter_module_status) {
  // It is use to verify related module are enabled or not if not enabled then
  // it throw an error,for a particular block.
  $bypass_flag = TRUE;

  // The variable to display all types of messages which are produce in
  // the blocks creation process.
  $message = "";

  // An array of block basic information.
  $block_custom = $block_data['block_custom'];
  // Block description.
  $block_info = $block_custom['info'];
  // The filter_format format of the block body.
  $format = $block_custom['format'];
  // The Block contents.
  $body = stripslashes($block_custom['body']);

  // Query to verify that this block is exists or not.
  $query = "SELECT bid AS block_id FROM {boxes} WHERE info = '%s'";
  $block_id = db_result(db_query($query, $block_info));
  if ($block_id > 0) {
    // Override the exisitng block.
    if ($override_existing == 1) {
      $visibility_data = $block_data['vsettings'];
      if ($block_data['vsettings']['visibility'] == 2) {
        if (!$php_filter_module_status) {
          $message = t("The PHP filter module should be enabled to&nbsp;");
          $message .= t("override the block &nbsp;");
          $message .= t("'!block_info'", array('!block_info' => $block_info));
          drupal_set_message(check_plain($message), 'error');
          $bypass_flag = FALSE;
        }
      }

      if ($bypass_flag) {
        // Update custom block's basic information.
        db_query("UPDATE {boxes} SET body = '%s', format = %d WHERE bid = %d", $body, $format, $block_id);

        // Update block's Visibility settings, title etc.
        $query = "UPDATE {blocks} SET custom = %d, status = %d, visibility = %d, throttle = %d, cache = %d, title = '%s', pages = '%s' WHERE module = 'block' AND delta = %d";

        db_query($query, $visibility_data['custom'], $visibility_data['status'], $visibility_data['visibility'], $visibility_data['throttle'], $visibility_data['cache'], $visibility_data['title'], $visibility_data['pages'], $block_id);

        // Detete the existing roles form same block to this block to
        // override by new settings.
        db_query("DELETE FROM {blocks_roles} WHERE module = 'block' AND delta = %d", $block_id);
        // Reset the role specific visibility settings.
        if (array_key_exists('rsv_settings', $block_data)) {
          $rsv_settings_data = $block_data['rsv_settings'];
          if (count($rsv_settings_data['block_role'])) {
            foreach ($rsv_settings_data['block_role'] as $rid) {
              db_query("INSERT INTO {blocks_roles} (module, delta, rid) VALUES ('block', %d, %d)", $block_id, $rid);
            }
          }
        }

        if (array_key_exists('bclass_settings', $block_data)) {
          if (module_exists('block_class') && db_table_exists('block_class')) {
            $bclass_settings_data = $block_data['bclass_settings'];
            db_query("DELETE FROM {block_class} WHERE module = 'block' AND delta = %d", $block_id);
            db_query("INSERT INTO {block_class} (module, delta, css_class) VALUES ('block', %d, '%s')", $block_id, $bclass_settings_data['css_class']);
          }
        }

        if (array_key_exists('bcache_settings', $block_data)) {
          if (module_exists('blockcache_alter') && db_table_exists('blockcache_alter')) {
            $bcache_settings_data = $block_data['bcache_settings'];
            db_query("DELETE FROM {blockcache_alter} WHERE module = 'block' AND delta = %d", $block_id);
            db_query("INSERT INTO {blockcache_alter} (module, delta, cache) VALUES ('block', %d, %d)", $block_id, $bcache_settings_data['block_cache']);
          }
        }

        $message = t("Block '!block_info' has been override successfully.", array('!block_info' => check_plain($block_info)));
        drupal_set_message(check_plain($message));
      }
    }
  }
  else {
    if ($block_data['vsettings']['visibility'] == 2) {
      if (!$php_filter_module_status) {
        $message = t("The PHP filter module should be enabled to&nbsp;");
        $message .= t("import the block &nbsp;");
        $message .= t("'!block_info'", array('!block_info' => $block_info));
        drupal_set_message(check_plain($message), 'error');
        $bypass_flag = FALSE;
      }
    }

    if ($bypass_flag) {
      // Create a new system specific blocks.
      db_query("INSERT INTO {boxes} (body, info, format) VALUES ('%s', '%s', %d)", $body, $block_info, $format);
      // Get the current block id.
      $block_id = db_last_insert_id($table = 'boxes', $field = 'bid');

      $visibility_data = $block_data['vsettings'];
      // Returns a list of all currently available themes.
      $available_themes = list_themes();
      foreach ($available_themes as $theme) {
        if ($theme->status) {
          // Initialize a blank array to set the default value of block fields.
          db_query("INSERT INTO {blocks} (module, delta, theme, status, custom, visibility, throttle, cache, title, pages) VALUES ('%s', %d, '%s', %d, %d, %d, %d, %d, '%s', '%s')", 'block', $block_id, $theme->name, $visibility_data['status'], $visibility_data['custom'], $visibility_data['visibility'], $visibility_data['throttle'], $visibility_data['cache'], $visibility_data['title'], $visibility_data['pages']);
        }
      }

      // Insert the role specific visibility settings.
      if (array_key_exists('rsv_settings', $block_data)) {
        $rsv_settings_data = $block_data['rsv_settings'];
        if (count($rsv_settings_data['block_role'])) {
          foreach ($rsv_settings_data['block_role'] as $rid) {
            db_query("INSERT INTO {blocks_roles} (module, delta, rid) VALUES ('block', %d, %d)", $block_id, $rid);
          }
        }
      }

      // Insert the class specific visibility settings.
      if (array_key_exists('bclass_settings', $block_data)) {
        if (module_exists('block_class') && db_table_exists('block_class')) {
          $bclass_settings_data = $block_data['bclass_settings'];
          db_query("INSERT INTO {block_class} (module, delta, css_class) VALUES ('block', %d, '%s')", $block_id, $bclass_settings_data['css_class']);
        }
      }

      // Insert the cache specific visibility settings.
      if (array_key_exists('bcache_settings', $block_data)) {
        if (module_exists('blockcache_alter') && db_table_exists('blockcache_alter')) {
          $bcache_settings_data = $block_data['bcache_settings'];
          db_query("INSERT INTO {blockcache_alter} (module, delta, cache) VALUES ('block', %d, %d)", $block_id, $bcache_settings_data['block_cache']);
        }
      }

      $message = t("New block '!block_info' has been created successfully.", array('!block_info' => $block_info)
      );
      drupal_set_message(check_plain($message));
      return;
    }
  }
}
