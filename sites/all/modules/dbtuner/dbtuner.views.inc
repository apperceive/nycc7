<?php

/**
 * Implementation of hook_views_preview_info_alter.
 */
function dbtuner_views_preview_info_alter(&$rows, $view) {
  $replacements = module_invoke_all('views_query_substitutions', $view); 
  $query = str_replace(array_keys($replacements), $replacements, $view->build_info['query']);
  $results = db_query("EXPLAIN " . $query, $view->build_info['query_args']);

  while ($explain = db_fetch_array($results)) {
    // Only populate $headers on the first iteration.
    if (empty($explain_headers)) {
      $explain_headers = array_map('ucfirst', array_keys($explain));
    }
    $explain_rows[] = array_values($explain);
  }
  $rows[] = array(array('data' => '<strong>Explain</strong>' . theme('table', $explain_headers, $explain_rows), 'colspan' => 2));
}


