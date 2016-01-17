<?php

/**
 * @file
 * Auto Tune Database
 */


/**
 * Page generation fucntion for admin/settings/dbtuner
 */
function dbtuner_indexes_page() {
  // Display Warning
  $output = '';
  $output = '<div class="messages warning"><ul><li>' . t('Be aware that while this transformation is taking place the tables getting an index added will be LOCKED.') . '</li></ul></div>';
  return $output . drupal_get_form('dbtuner_index_configuration');
}

/**
 * Page generation fucntion for admin/settings/dbtuner/engine
 */
function dbtuner_engine_page() {
  // Display Warning
  $output = '';
  $output = '<div class="messages error"><ul><li>' . t('WARNING: Backup your database! Be aware that while this SLOW transformation is taking place your database will be LOCKED. Recommend putting your site into maintance mode (for real).') . '</li></ul></div>';
  return $output . drupal_get_form('dbtuner_engine_configuration');
}

/**
 * Page generation fucntion for admin/settings/dbtuner/collation
 */
function dbtuner_collation_page() {
  // Display Warning
  $output = '';
  $output = '<div class="messages error"><ul><li>' . t('EXPERIMENTAL!!! WARNING: Backup your database! Be aware that while this SLOW transformation is taking place your database will be LOCKED. Recommend putting your site into maintance mode (for real).') . '</li></ul></div>';
  return $output . drupal_get_form('dbtuner_collation_configuration');
}

/**
 * Form builder; collation configuration page.
 */
function dbtuner_collation_configuration() {
  // Options
  // db_query("SHOW COLLATION");
  $settings = array(
    'utf8_general_ci' => 'utf8_general_ci - Fastest',
    'utf8_unicode_ci' => 'utf8_unicode_ci',
    'utf8_swedish_ci' => 'utf8_swedish_ci - Slowest (language specific)',
  );

  // Get Current collation for database
  $char = db_fetch_array(db_query("SHOW VARIABLES LIKE 'character_set_database'"));
  $collations[$char['Value'] . ' Database'] = $char['Value'];
  $char = db_fetch_array(db_query("SHOW VARIABLES LIKE 'collation_database'"));
  $collations[$char['Value'] . ' Database'] = $char['Value'];
  // Get Current collation for tables
  $result = db_query('SHOW TABLE STATUS');
  $tables = array();
  while ($row = db_fetch_array($result)) {
    //$tables[$row['Engine']][$row['Name']] = $row;
    $tables[] = $row['Name'];
    $collations[$row['Collation'] . ' Table'] = $row['Collation'];
  }
  // Get Current collation for rows
  foreach ($tables as $table) {
    $result = db_query("SHOW FULL FIELDS FROM %s", $table);
    while ($row = db_fetch_array($result)) {
      if (!empty($row['Collation']) && $row['Collation'] != 'binary' && $row['Collation'] != 'utf8_bin') {
        $collations[$row['Collation'] . ' Row'] = $row['Collation'];
      }
    }
  }

  // Build Form
  $form = array();
  $form['db_collation'] = array(
    '#type' => 'radios',
    '#title' => t('Select collation to use. Recommend utf8_general_ci'),
    '#options' => $settings,
    '#description' => 'Collations in use: ' . implode(', ', array_keys($collations)),
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Change Database Collation'));
  $form['#submit'][] = 'dbtuner_collation_configuration_submit';
  return $form;
}

/**
 * Form builder; collation configuration page.
 */
function dbtuner_collation_configuration_submit($form, &$form_state) {
  // Example Database Query
  //  ALTER DATABASE drupal DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci
  // Example Table Query
  //  ALTER TABLE access DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci
  // Example Field Query
  //  ALTER TABLE access CHANGE `mask` `mask` VARCHAR( 255 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL

  // Get selected collation
  $collation = $form_state['values']['db_collation'];
  $encode = 'utf8';

  // Change database if needed.
  $update = FALSE;
  $char = db_fetch_array(db_query("SHOW VARIABLES LIKE 'character_set_database'"));
  if ($char['Value'] != $encode) {
    $update = TRUE;
  }
  $coll = db_fetch_array(db_query("SHOW VARIABLES LIKE 'collation_database'"));
  if ($coll['Value'] != $collation) {
    $update = TRUE;
  }
  if ($update) {
    $database_name = db_result(db_query("select database()"));
    db_query("ALTER DATABASE `%s` DEFAULT CHARACTER SET %s COLLATE %s", $database_name, $encode, $collation);
    drupal_set_message(t('Database Changed from %char to %encode and %coll to %collation', array(
      '%char' => $char['Value'],
      '%encode' => $encode,
      '%coll' => $coll['Value'],
      '%collation' => $collation,
    )));
  }

  $counter = 0;
  // Change tables collation
  $result = db_query('SHOW TABLE STATUS');
  $tables = array();
  while ($row = db_fetch_array($result)) {
    $tables[] = $row['Name'];
    if ($row['Collation'] != $collation) {
      //db_query("ALTER TABLE %s DEFAULT CHARACTER SET %s COLLATE %s", $row['Name'], $encode, $collation);
      db_query("ALTER TABLE %s CONVERT TO CHARACTER SET %s COLLATE %s", $row['Name'], $encode, $collation);
      $counter++;
    }
  }
  drupal_set_message(t('%count Tables Changed to to %collation', array(
    '%count' => $counter,
    '%collation' => $collation,
  )));

//   $counter = 0;
//   foreach ($tables as $table) {
//     $result = db_query("SHOW FULL FIELDS FROM %s", $table);
//     while ($row = db_fetch_array($result)) {
//       // Change rows
//       if (!empty($row['Collation']) && $row['Collation'] != 'binary' && $row['Collation'] != $collation && $row['Collation'] != 'utf8_bin') {
//         $null_text = ($row['Null'] == 'No') ? 'NOT NULL' : 'NULL';
//         db_query("ALTER TABLE %s CHANGE `%s` `%s` %s CHARACTER SET %s COLLATE %s %s",
//           $table,
//           $row['Field'],
//           $row['Field'],
//           strtoupper($row['Type']),
//           $encode,
//           $collation,
//           $null_text
//         );
//         $counter++;
//         drupal_set_message(str_replace('    ', '&nbsp;&nbsp;&nbsp;&nbsp;', nl2br(htmlentities(print_r($row, TRUE)))));
//       }
//     }
//   }
//   if ($counter > 0) {
//     drupal_set_message(t('%count Rows Changed to to %collation', array(
//       '%count' => $counter,
//       '%collation' => $collation,
//     )));
//   }
}

/**
 * Form builder; engine configuration page.
 */
function dbtuner_engine_configuration() {
  // Get database info
  $result = db_query('SHOW TABLE STATUS');
  $tables = array();
  while ($row = db_fetch_array($result)) {
    //$tables[$row['Engine']][$row['Name']] = $row;
    $tables[$row['Engine']] = $row['Engine'];
  }
  $result = db_query('SHOW ENGINES');
  $engines = array();
  while ($row = db_fetch_array($result)) {
    // Only add supported DB engines
    if ($row['Support'] == 'YES' || $row['Support'] == 'DEFAULT') {
      $engines[$row['Engine']] = $row['Engine'];
    }
  }

  // Build Form
  $form = array();
  $form['db_engine'] = array(
    '#type' => 'radios',
    '#title' => t('Select Database Engine To Use. Recommend InnoDB or MyISAM.'),
    '#options' => $engines,
    '#description' => 'Engines in use: ' . implode(', ', array_keys($tables)),
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Change Database Engine'));
  $form['#submit'][] = 'dbtuner_engine_configuration_submit';
  return $form;
}

/**
 * Form builder; indexes configuration page.
 */
function dbtuner_engine_configuration_submit($form, &$form_state) {
  $engine = $form_state['values']['db_engine'];
  $tables = db_query('SHOW TABLE STATUS');
  $counter = 0;
  while ($table = db_fetch_array($tables)) {
    if ($table['Engine'] != $engine) {
      db_query('ALTER TABLE %s ENGINE = %s', $table['Name'], $engine);
      $counter++;
    }
  }
  drupal_set_message(t('@count tables changed to @name', array('@count' => $counter, '@name' => $engine)));
}

/**
 * Form builder; Displays DB Tuners configuration page.
 */
function dbtuner_index_configuration() {
  $form = array();
  if (module_exists('views') && module_exists('content')) {
    $indexes = dbtuner_get_checkboxes(dbtuner_views_filters_relationshps());
    if ($indexes) {
      $defaults = $indexes['defaults'];
      $options = $indexes['options'];

      $form['views'] = array(
        '#type'           => 'fieldset',
        '#title'          => t('Indexes for Views'),
        '#description'    => t('If a CCK field is used in a view filter it can be quite slow due to the database not using an index. This will add an index to these fields. Checked means its indexed.'),
      );
      $form['views']['view-indexes'] = array(
        '#type'           => 'checkboxes',
        '#title'          => t('Select the fields you wish to be indexed by the database.'),
        '#description'    => t('DB fields that could use an index detected by loading all views and looking at what filters and relationships are used.'),
        '#default_value'  => $defaults,
        '#options'        => $options,
      );
    }
  }

  $indexes = dbtuner_get_checkboxes(dbtuner_core_indexes());
  if ($indexes) {
    $defaults = $indexes['defaults'];
    $options = $indexes['options'];
    $form['core'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Indexes for Core modules'),
      '#description'    => t('Core could really use indexes on these fields.'),
    );
    $form['core']['core-indexes'] = array(
      '#type'           => 'checkboxes',
      '#title'          => t('Select the fields you wish to be indexed by the database.'),
      '#description'    => t('DB fields that could use an index.'),
      '#default_value'  => $defaults,
      '#options'        => $options,
    );
  }

  $indexes = dbtuner_get_checkboxes(dbtuner_expermental_core_indexes());
  if ($indexes) {
    $defaults = $indexes['defaults'];
    $options = $indexes['options'];
    $form['experimental'] = array(
      '#type'           => 'fieldset',
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
      '#title'          => t('Experimental Indexes for Core modules'),
      '#description'    => t('Unproven indexes; may hurt or help performance.'),
    );
    $form['experimental']['experimental-indexes'] = array(
      '#type'           => 'checkboxes',
      '#title'          => t('Select the fields you wish to be indexed by the database.'),
      '#description'    => t('DB fields that could use an index.'),
      '#default_value'  => $defaults,
      '#options'        => $options,
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Set Index Configuration'));
  $form['#validate'][] = 'dbtuner_index_configuration_validate';
  return $form;
}

/**
 * Form builder; Displays DB Tuners configuration page.
 */
function dbtuner_index_configuration_validate($form, &$form_state) {
  $add = array();
  $del = array();

  // Views
  $old = $form['views']['view-indexes']['#default_value'];
  $new = $form_state['values']['view-indexes'];
  dbtuner_get_add_del($old, $new, $add, $del);

  // Core
  $old = $form['core']['core-indexes']['#default_value'];
  $new = $form_state['values']['core-indexes'];
  dbtuner_get_add_del($old, $new, $add, $del);

  // Experimental
  $old = $form['experimental']['experimental-indexes']['#default_value'];
  $new = $form_state['values']['experimental-indexes'];
  dbtuner_get_add_del($old, $new, $add, $del);

  // Change Indexes
  $ret = array();
  if (!empty($add)) {
    foreach ($add as $value) {
      db_add_index($ret, $value[0], $value[1], array($value[1]));
    }
  }
  if (!empty($del)) {
    foreach ($del as $value) {
      db_drop_index($ret, $value[0], $value[1]);
    }
  }
  if (!empty($ret)) {
    drupal_set_message(str_replace('    ', '&nbsp;&nbsp;&nbsp;&nbsp;', nl2br(htmlentities(print_r($ret, TRUE)))));
  }
}

/**
 * Form API Checkboxes handler. Only get what has changed.
 *
 * @param $old
 *   Default values
 * @param $new
 *   New values from user
 * @param $add
 *   added by selection
 * @param $del
 *   removed by selection
 */
function dbtuner_get_add_del($old, $new, &$add, &$del) {
  if (!is_array($old) || !is_array($new)) {
    return;
  }
  foreach ($new as $key => $value) {
    if (!in_array($value, $old) && stristr($key, $value)) {
      $add[] = dbtuner_extract_table_index($key);
    }
    if (is_numeric($value) && in_array($key, $old)) {
      $del[] = dbtuner_extract_table_index($key);
    }
  }
}

/**
 * Process the indexes array and return options and defaults.
 *
 * @param $indexes
 *   Report from database
 * @return
 *   array('options' => $options, 'defaults' => $defaults)
 */
function dbtuner_get_checkboxes($indexes) {
  $options = array();
  $defaults = array();
  if (empty($indexes)) {
    return FALSE;
  }
  foreach ($indexes as $key => $value) {
    extract($value);
    $index = trim($index);
    $table = trim($table);
    $key = trim($key);
    $info = dbtuner_empty_index($table, $index);
    if ($info === TRUE) {
      $options[$key] = 'Table: ' . $table . ' Column: ' . $index;
    }
    elseif (is_string($info)) {
      $defaults[] = $key;
      $options[$key] = 'Table: ' . $table . ' Column: ' . $index . ' ' . $info;
    }
    elseif ($info === FALSE) {
    }
  }
  return array('options' => $options, 'defaults' => $defaults);
}

/**
 * Get table & index info out of array key
 *
 * @param $value
 *   key value from array
 * @return
 *   array($table, $index)
 */
function dbtuner_extract_table_index($value) {
  return explode(' ', $value);
}

/**
 * Find if an index exists
 *
 * @param $table
 *   The table to be altered.
 * @param $index
 *   The name of the index.
 * @return
 *  Key_name if index exists
 *  TRUE if index does not exist
 *  FALSE if database column doesn't exist
 */
function dbtuner_empty_index($table, $index) {
  global $db_type;
  $column_exists = FALSE;

  if (!db_table_exists($table)) {
    return FALSE;
  }
  // PostgreSQL
  if (stristr($db_type, 'pgsql')) {
    return FALSE;
    // Selecting a db schema table, don't put pg_indexes inside {}
    //$result = db_query("SELECT * FROM pg_indexes WHERE tablename = '{%s}'", $table);
    //while ($name = db_fetch_array($result)) {
      //if (stristr($name['indexname'], $index)) {
        //return TRUE;
      //}
    //}
  }
  // MySQL
  elseif (stristr($db_type, 'mysql') || stristr($db_type, 'mysqli')) {
    $result = db_query("SHOW INDEX FROM {" . $table . "}");

    // Get all indexes
    $table_info = array();
    while ($column = db_fetch_array($result)) {
      $table_info[$column['Key_name']][] = $column['Column_name'];
    }
    // Find the indexes that only contain 1 key_name in it
    foreach ($table_info as $columns) {
      $counter = 0;
      $match = FALSE;
      foreach ($columns as $column_name) {
        $counter++;
        if ($column_name == $index) {
          $match = TRUE;
        }
      }
      // Index exists
      if ($match && $counter == 1) {
        return trim($info['Key_name']);
      }
      // Index exists but is appart of a multi index;
      // these are not supported at this time
      elseif ($match) {
        return FALSE;
      }
    }

    // Index does not exist
    $result = db_query("SHOW COLUMNS FROM {" . $table . "}");
    while ($column = db_fetch_array($result)) {
      if ($column['Field'] == $index) {
        // Indexes on text is not supported at this time
        if (stristr($column['Type'], 'text') || stristr($column['Type'], 'blob')) {
          $name = explode('_', $index);
          array_pop($name);
          array_shift($name);
          $name = implode('_', $name);
          $bad_views = dbtuner_views_find_bad_filter($table, $column['Type']);
          drupal_set_message(t('CCK Type %name is used in a view filter in the %table table, but database row is of type %type. This is generally a bad idea. Please change your view(s) to not filter on text or set the max length value in the CCK field to less then 255. !views', array(
            '%name' => $name,
            '%type' => $column['Type'],
            '%table' => $table,
            '!views' => implode(', ', $bad_views),
          )), 'warning');
          return FALSE;
        }
        $column_exists = TRUE;
        break;
      }
    }
  }
  // DB is not supported
  else {
    return FALSE;
  }

  // Index doesn't exist
  if ($column_exists) {
    return TRUE;
  }
  // Column doesn't exist
  else {
    return FALSE;
  }
}

/**
 * Get all views that filter on this table
 *
 * @param $table
 *   db table name
 * @param $type
 *   db field type
 * @return array
 *   array of view names with a bad filter
 */
function dbtuner_views_find_bad_filter($table, $type) {
  $views = views_get_all_views();
  $poor_views = array();
  foreach ($views as $key => $view) {
    // disabled views get nothing.
    if (!empty($view->disabled)) {
      unset($views[$key]);
      continue;
    }

    $view->init_display();
    foreach ($view->display as $display_id => $display) {
      $filters = $view->get_items('filter', $display_id);
      //$relationships = $view->get_items('relationship', $display_id);

      if (!empty($filters)) {
        foreach ($filters as $name => $filter) {
          if (!empty($filter['table']) && $filter['table'] == $table) {
            if (!empty($filter['field']) && db_table_exists($table)) {
              $result = db_query('SHOW COLUMNS FROM {%s}', $table);
              while ($column = db_fetch_array($result)) {
                if ($column['Field'] == $filter['field'] && $column['Type'] == $type) {
                  $poor_views[$key] = $key;
                }
              }
            }
          }
        }
      }
    }
  }
  return $poor_views;
}

/**
 * Get all filters and relationships from views
 *
 * @return array
 *   array('table' => $table, 'index' => $field)
 */
function dbtuner_views_filters_relationshps() {
  $views = views_get_all_views();
  $indexes = array();
  foreach ($views as $key => $view) {
    // disabled views get nothing.
    if (!empty($view->disabled)) {
      unset($views[$key]);
      continue;
    }

    $view->init_display();
    foreach ($view->display as $display_id => $display) {
      $filters = $view->get_items('filter', $display_id);
      $relationships = $view->get_items('relationship', $display_id);

      // Build data structure
      if (!empty($filters)) {
        foreach ($filters as $name => $filter) {
          if (!empty($filters['type']['value'])) {
            foreach ($filters['type']['value'] as $key => $value) {
              $indexes[$filter['table']][$filter['id']]['node-type'][$key] = $value;
            }
          }
          else {
            $indexes[$filter['table']][$filter['id']]['node-type']['-1'] = 'all';
          }
          $indexes[$filter['table']][$filter['id']]['relationship'] = $filter['relationship'];
        }
      }
      if (!empty($relationships)) {
        foreach ($relationships as $name => $relationship) {
          $indexes[$relationship['table']][$relationship['id']][$name] = $relationship['relationship'];
        }
      }
    }
  }

  // Get all content_type tables
  $cck_content_types = array();
  $result = db_query("SHOW TABLES LIKE '{content_type_%}'");
  while ($table = db_result($result)) {
    $cck_content_types[] = $table;
  }

  // Relate indexes to tables.
  $return = array();
  if (empty($indexes)) {
    return $return;
  }
  foreach ($indexes as $table => $values) {
    foreach ($values as $field => $extra) {
      $cck_table = str_replace('node_data', 'content', $table);

      // Not a cck field
      if (db_table_exists($table)) {
        $return[$table . ' ' . $field] = array('table' => $table, 'index' => $field);
        if ($extra['relationship'] != 'none') {
          $return[$table . ' ' . $extra['relationship']] = array('table' => $table, 'index' => $extra['relationship']);
        }
      }

      // CCK field with own table
      elseif (db_table_exists($cck_table)) {
        $return[$cck_table . ' ' . $field] = array('table' => $cck_table, 'index' => $field);
        if ($extra['relationship'] != 'none') {
          $return[$cck_table . ' ' . $extra['relationship']] = array('table' => $cck_table, 'index' => $extra['relationship']);
        }
      }

      // CCK field in content type table
      elseif (!empty($cck_content_types) && is_array($cck_content_types)) {
        foreach ($cck_content_types as $cck_content_table) {
          // Place index on all content_type tables
          if (!empty($extra['node-type']['-1']) && $extra['node-type']['-1'] == 'all') {
            $return[$cck_content_table . ' ' . $field] = array('table' => $cck_content_table, 'index' => $field);
            if ($extra['relationship'] != 'none') {
              $return[$cck_content_table . ' ' . $extra['relationship']] = array('table' => $cck_content_table, 'index' => $extra['relationship']);
            }
          }

          // Place index only where needed
          elseif (!empty($extra['node-type']) && is_array($extra['node-type'])) {
            foreach ($extra['node-type'] as $nodetype) {
              $return['content_type_' . $nodetype . ' ' . $field] = array('table' => 'content_type_' . $nodetype, 'index' => $field);
              if ($extra['relationship'] != 'none') {
                $return['content_type_' . $nodetype . ' ' . $extra['relationship']] = array('table' => 'content_type_' . $nodetype, 'index' => $extra['relationship']);
              }
            }
          }
        }
      }
    }
  }
  ksort($return);
  return $return;
}

/**
 * Build core index
 *
 * @return array
 *   array('table' => $table, 'index' => $field)
 */
function dbtuner_core_indexes() {
  $return = array();
  $return['node_counter totalcount'] = array('table' => 'node_counter', 'index' => 'totalcount');
  $return['node_counter daycount'] = array('table' => 'node_counter', 'index' => 'daycount');
  $return['node_counter timestamp'] = array('table' => 'node_counter', 'index' => 'timestamp');
  //$return[' '] = array('table' => '', 'index' => '');
  ksort($return);
  return $return;
}

/**
 * Get all filters and relationships from views
 *
 * @return array
 *   array('table' => $table, 'index' => $field)
 */
function dbtuner_expermental_core_indexes() {
  $return = array();
  $return['access type'] = array('table' => 'access', 'index' => 'type');
  $return['access mask'] = array('table' => 'access', 'index' => 'mask');
  $return['access status'] = array('table' => 'access', 'index' => 'status');
  $return['comments timestamp'] = array('table' => 'comments', 'index' => 'timestamp');
  $return['node_comment_statistics comment_count'] = array('table' => 'node_comment_statistics', 'index' => 'comment_count');
  $return['menu_links external'] = array('table' => 'menu_links', 'index' => 'external');
  $return['menu_links updated'] = array('table' => 'menu_links', 'index' => 'updated');
  $return['menu_links customized'] = array('table' => 'menu_links', 'index' => 'customized');
  $return['menu_links depth'] = array('table' => 'menu_links', 'index' => 'depth');
  $return['menu_custom title'] = array('table' => 'menu_custom', 'index' => 'title');
  $return['users pass'] = array('table' => 'users', 'index' => 'pass');
  $return['users status'] = array('table' => 'users', 'index' => 'status');
  $return['filter_formats roles'] = array('table' => 'filter_formats', 'index' => 'roles');
  $return['term_data name'] = array('table' => 'term_data', 'index' => 'name');
  $return['blocks module'] = array('table' => 'blocks', 'index' => 'module');
  $return['blocks delta'] = array('table' => 'blocks', 'index' => 'delta');
  $return['system name'] = array('table' => 'system', 'index' => 'name');
  $return['system status'] = array('table' => 'system', 'index' => 'status');
  $return['system type'] = array('table' => 'system', 'index' => 'type');
  $return['date_format_types title'] = array('table' => 'date_format_types', 'index' => 'title');
  $return['files filepath'] = array('table' => 'files', 'index' => 'filepath');
  //$return[' '] = array('table' => '', 'index' => '');
  ksort($return);
  return $return;
}
