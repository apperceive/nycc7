<?php

/**
 * @file
 * Run explain on queries
 */

/**
 * Page generation fucntion for admin/settings/explain-views
 */
function dbtuner_explain_views_page() {
  $output = '';
  if (module_exists('views')) {
    $output .= dbtuner_explain_get_views();
  }
  else {
    $output = 'Views Not Installed';
  }
  return $output;
}

/**
 * Page generation fucntion for admin/settings/explain-slow-log
 */
function dbtuner_explain_slow_query_log_page() {
  $output = 'Not Implemented';
  return $output;
}

function dbtuner_explain_get_views() {
  // Disable Query Cache for this session temp
  $query_cache = db_fetch_array(db_query("SHOW VARIABLES WHERE Variable_name = '%s'", 'query_cache_type'));
  $query_cache = $query_cache['Value'];
  db_query("SET SESSION query_cache_type = OFF");

  // Load up all views
  $views = views_get_all_views();
  $data = array();
  $output = '';
  foreach ($views as $key => $view) {
    // disabled views get nothing.
    if (!empty($view->disabled)) {
      unset($views[$key]);
      continue;
    }

    $view->init_display();
    // Check each view display
    foreach ($view->display as $display_id => $display) {
      $good = $view->set_display($display_id);
      if (!$good) {
        continue;
      }
      $filters = $view->get_items('filter', $display_id);
      $hash = $key . ' - ' . $display_id;
      //$relationships = $view->get_items('relationship', $display_id);

      // Run View
      $view->pre_execute();
      $view->execute();

      // Skip if view query ran in less then 9 ms
      if ($view->execute_time < 0.009) {
        //$output .= $view->execute_time . '<br />';
        continue;
      }

      // Prep view for explain
      $query = $view->build_info['query'];
      $query_args = $view->build_info['query_args'];
      $replacements = module_invoke_all('views_query_substitutions', $view);
      $query = str_replace(array_keys($replacements), $replacements, $query);
      $query_args = str_replace(array_keys($replacements), $replacements, $query_args);
      $view_arguments = $view->display_handler->options['arguments'];

      // can not handle view arguments very well ATM; skip.
      if ((empty($query) && count($view_arguments) < 1) || (count($view_arguments) > 0)) {
        array_unshift($data, array($hash => array(
            'time-build' => $view->build_time,
            'time-run' => $view->execute_time,
          )
        ));
        continue;
      }
  //     if (empty($query) && count($view_arguments) < 1) {
  //       //array_unshift($data, array($hash => array('query' => 'NOT FOUND. Check View! Still working on view arguments... known issue with false positives.')));
  //       continue;
  //     }
  //     if (count($view_arguments) > 0) {
  //       continue;
  //     }

      $data[$hash]['time-build'] = $view->build_time;
      $data[$hash]['time-run'] = $view->execute_time;
      $data[$hash]['rows returned'] = count($view->result);
      $data[$hash]['query'] = "\n" . dbtuner_db_query($query, $query_args);

      // Add hash to query incase something goes wrong; you can figure out what view caused the error.
      $query .= ' /* ' . $hash . '*/';
      $explain = db_query('EXPLAIN ' . $query, $query_args);
      while($value = db_fetch_array($explain)) {
        $data[$hash][] = $value;
      }
    }
  }

  // Set query cache back to orginal state
  db_query("SET SESSION query_cache_type = %s", $query_cache);

  // Generate output
  $output .= t('Number of slow views: %num <br />', array('%num' => count($data)));
  $output .= str_replace('    ', '&nbsp;&nbsp;&nbsp;&nbsp;', nl2br(htmlentities(print_r($data, TRUE))));
  return $output;
}

function dbtuner_db_query($query) {
  $args = func_get_args();
  array_shift($args);
  $query = db_prefix_tables($query);
  if (isset($args[0]) and is_array($args[0])) { // 'All arguments in one array' syntax
    $args = $args[0];
  }
  _db_query_callback($args, TRUE);
  $query = preg_replace_callback(DB_QUERY_REGEXP, '_db_query_callback', $query);
  return $query;
}

