<?php

/**
 * @file
 * Export all system specific blocks.
 */

/**
 * Form constructor for export all system specific blocks array structure.
 *
 * @see _block_export_import_get_importable_object()
 * @see _block_export_import_blocks_export_form_validate()
 * @see block_export_import_blocks_export_form_submit()
 */
function block_export_import_blocks_export_form(&$form_state) {
  $block_export_import_path = drupal_get_path('module', 'block_export_import');
  drupal_add_css($block_export_import_path . '/css/block_export_import.css');

  // A condition to verify form element export_code is empty.
  if (empty($form_state['storage']['values'])) {
    // This query is used to fetch all system specific block.
    $result = db_query("SELECT boxes.bid, boxes.info FROM {blocks} INNER JOIN {boxes} ON blocks.delta = boxes.bid WHERE blocks.module = 'block'");
    if (count($result)) {
      $custom_block_prefix = '';
      $custom_block_prefix .= '<div class="block_name">Block Name</div>';
      $custom_block_prefix .= '<div class="block_action">Action</div>';
      $form['custom_block'] = array(
        '#tree' => TRUE,
        '#prefix' => '<div class="header">' . $custom_block_prefix,
        '#suffix' => '</div><div style="clear:both;"></div>',
      );

      // $counter is used to set the alternate color on extracted rows.
      $counter = 0;

      // Iterate over each element in our $form['custom_block'] array.
      while ($data = db_fetch_object($result)) {
        $class = 'odd';
        if ($counter++ % 2 != 0) {
          $class = 'even';
        }

        // Block machine name.
        $block_title = $data->info;
        // The block’s block.bid.
        $block_id = $data->bid;

        $form['custom_block'][$block_id]['export'] = array(
          '#type' => 'checkbox',
          '#title' => check_plain($block_title),
          '#prefix' => '<div class="' . $class . ' ">',
        );

        $op_link = 'admin/build/block/configure/block/' . $block_id;
        $op_link_attr = array('attributes' => array('target' => '_blank', 'style' => 'float:right;padding-right:7px;'));
        $form['custom_block'][$block_id]['operation'] = array(
          '#type' => 'item',
          '#value' => l(t('Edit block'), $op_link, $op_link_attr),
          '#suffix' => '<div style="clear:left;"></div></div>',
        );
      }

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Export Blocks'),
        '#validate' => array('_block_export_import_blocks_export_form_validate'),
        '#prefix' => '<div class="export-block-submit">',
        '#suffix' => '</div>',
      );
    }
    else {
      $form['custom_block']['no_block'] = array(
        '#type' => 'item',
        '#value' => t('No system specific block exists.'),
      );
    }
  }
  else {
    // Get Importable array structure as string.
    $export_data = _block_export_import_get_importable_object($form_state);

    $export_desc = t("For importing copy the content of the text area and");
    $export_desc .= t(" paste it into the import page textarea.");
    $form['export_code'] = array(
        '#type' => 'textarea',
        '#title' => t('Export'),
        '#default_value' => $export_data,
        '#rows' => 15,
        '#description' => $export_desc,
    );

    $link = l(t('Back to Export Blocks'), 'admin/build/export-import-block/export');
    $form['back'] = array(
        '#type' => 'item',
        '#value' => $link,
    );
  }
  return $form;
}

/**
 * Use to validate user must be selected at least one block to export.
 */
function _block_export_import_blocks_export_form_validate($form, &$form_state) {
  $custom_block = $form_state['values']['custom_block'];
  $flag = FALSE;
  foreach ($custom_block as $data) {
    if ($data['export']) {
      $flag = TRUE;
      break;
    }
  }

  if ($flag == FALSE) {
    form_set_error('custom_block', t('Please select at least one block.'));
  }
}

/**
 * Export an array object of all selected system specific blocks.
 *
 * @return array
 *   Formatted structured array for import.
 */
function _block_export_import_get_importable_object($form_state) {
  // Verify that input text area have some code.
  if (!empty($form_state['storage']['values'])) {
    // Use to store an structured array as an string.
    $export_data = '';
    // Append the blocks array.
    $export_data = "\$block = array();\n";

    // Iterate for each selected block.
    foreach ($form_state['storage']['values'] as $block_id => $storage_data) {
      if ($storage_data['export']) {

        // Variable to fetch the fields value.
        $fileds = "blocks.delta, blocks.custom, blocks.visibility,";
        $fileds .= " blocks.pages, blocks.title, blocks.status, blocks.cache,";
        $fileds .= " boxes.bid, boxes.info, boxes.body, boxes.format";

        // Variable to append the table joins.
        $joins = "";
        $joins .= " LEFT JOIN {blocks_roles} ON blocks.delta = blocks_roles.delta";
        $fileds .= ", blocks_roles.rid";

        // Join the block_class table if related module is exists.
        if (module_exists('block_class') && db_table_exists('blocks_roles')) {
          $joins .= " LEFT JOIN {block_class} ON boxes.bid = block_class.delta";
          $fileds .= ", block_class.css_class";
        }

        // Join the blockcache_alter table if related module is exists.
        if (module_exists('blockcache_alter') && db_table_exists('blockcache_alter')) {
          $joins .= " LEFT JOIN {blockcache_alter} ON boxes.bid = blockcache_alter.delta";
          $fileds .= ", blockcache_alter.cache AS block_cache";
        }

        // This query is used to fetch custom blocks data.
        $query = "SELECT " . $fileds . " FROM {blocks} INNER JOIN {boxes} ON ";
        $query .= "blocks.delta = boxes.bid " . $joins . " WHERE ";
        $query .= "blocks.module = 'block' AND blocks.delta = %d";
        $result = db_query($query, $block_id);

        // Get the count value form result set.
        $row_count = count($result);
        if (count($result)) {
          $block_obj = array();
          // Iterate through each database result.
          while ($data = db_fetch_object($result)) {
            // It may be basic or full only.
            // The Block description.
            $block_obj['data']['info'] = $data->info;
            // The Block body contents.
            $block_obj['data']['body'] = addslashes($data->body);
            // The filter_format.format of the block body.
            $block_obj['data']['format'] = $data->format;

            // Object to indicate how to control user visibility settings.
            $block_obj['data']['vsettings']['custom'] = $data->custom;
            $block_obj['data']['vsettings']['status'] = $data->status;
            $block_obj['data']['vsettings']['visibility'] = $data->visibility;
            $block_obj['data']['vsettings']['pages'] = $data->pages;
            $block_obj['data']['vsettings']['throttle'] = $data->throttle;
            $block_obj['data']['vsettings']['cache'] = $data->cache;
            $block_obj['data']['vsettings']['title'] = $data->title;

            // Object to indicate how to control role specific visibility.
            if ((int) $data->rid) {
              $rid = $data->rid;
              // The result may have some duplicate records but here we are
              // overriding it by same key of block_role array.
              // The user’s role ID.
              $block_obj['data']['rsv_settings']['role'][$rid] = $rid;
            }

            // Block class settings.
            if (array_key_exists('css_class', $data)) {
              $block_obj['data']['bclass_settings']['css_class'] = $data->css_class;
            }

            // Block cache settings.
            if (array_key_exists('block_cache', $data)) {
              $block_obj['data']['bcache_settings']['block_cache'] = $data->block_cache;
            }
          }

          // Iterate through each block structured array.
          foreach ($block_obj as $data) {
            $export_data .= "\$block[] = array(\n";
            // Append Basic Information.
            $export_data .= " 'block_custom' => array(\n";
            $export_data .= " 'info' => '" . $data['info'] . "', \n";
            $export_data .= " 'format' => '" . $data['format'] . "', \n";
            $export_data .= " 'body' => '" . $data['body'] . "', \n";
            $export_data .= " ),\n";

            if (array_key_exists('vsettings', $data)) {
              $usv_settings_data = $data['vsettings'];
              $export_data .= '/* Visibility settings */' . "\n";

              // Append specific visibility settings.
              $export_data .= " 'vsettings' =>  array(";
              foreach ($usv_settings_data as $key => $vdata) {
                switch ($key) {
                  case 'custom':
                    $export_data .= " 'custom' => " . (int) $vdata . ", \n";
                    break;
                  case 'status':
                    $export_data .= " 'status' => " . (int) $vdata . ", \n";
                    break;

                  case 'visibility':
                    $export_data .= " 'visibility' => " . (int) $vdata . ", \n";
                    break;

                  case 'pages':
                    $export_data .= " 'pages' => '" . $vdata . "', \n";
                    break;

                  case 'throttle':
                    $export_data .= " 'throttle' => " . (int) $vdata . ", \n";
                    break;

                  case 'title':
                    $export_data .= " 'title' => '" . $vdata . "', \n";
                    break;

                  case 'cache':
                    $export_data .= " 'cache' => " . $vdata . ", \n";
                    break;
                }
              }
              $export_data .= "), \n";
            }

            if (array_key_exists('rsv_settings', $data)) {
              $rsv_settings_data = $data['rsv_settings'];
              $export_data .= '/* Role specific visibility settings */' . "\n";

              // Append role specific visibility settings.
              $export_data .= " 'rsv_settings' =>  array(";
              foreach ($rsv_settings_data as $key => $rsvdata) {
                switch ($key) {
                  case 'role':
                    $export_data .= " 'block_role' => array(\n";
                    foreach ($rsvdata as $role_id) {
                      $export_data .= "  " . (int) $role_id . ", \n";
                    }
                    $export_data .= " ),\n";
                    break;
                }
              }
              $export_data .= "), \n";
            }

            if (array_key_exists('bclass_settings', $data)) {
              $bclass_data = $data['bclass_settings'];
              $export_data .= '/* Block Class Settings */' . "\n";

              // Append other block settings.
              $export_data .= " 'bclass_settings' => array(\n";
              foreach ($bclass_data as $key => $odata) {
                switch ($key) {
                  case 'css_class':
                    $export_data .= " 'css_class' => '" . $odata . "', \n";
                    break;
                }
              }
              $export_data .= " ), \n";
            }

            if (array_key_exists('bcache_settings', $data)) {
              $bcache_data = $data['bcache_settings'];
              $export_data .= '/* Block Cache Settings */' . "\n";

              // Append other block settings.
              $export_data .= " 'bcache_settings' => array(\n";
              foreach ($bcache_data as $key => $odata) {
                switch ($key) {
                  case 'block_cache':
                    $export_data .= " 'block_cache' => " . $odata . ", \n";
                    break;
                }
              }
              $export_data .= " ), \n";
            }
            $export_data .= ");\n\n";
          }
        }
      }
    }
    return $export_data;
  }
}

/**
 * Submit handler for blockexport_export_blocks_form_submit() to save a blocks.
 */
function block_export_import_blocks_export_form_submit($form, &$form_state) {

  if (empty($form_state['storage']['values'])) {
    // If there is no previous values redraw for next step.
    $form_state['storage']['values'] = $form_state['values']['custom_block'];
    $form_state['rebuild'] = TRUE;
  }
  else {
    // Form is on the next step, process the data here.
  }
}
