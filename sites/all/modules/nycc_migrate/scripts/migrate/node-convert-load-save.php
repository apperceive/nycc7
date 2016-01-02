<?php

/*
 * loop over all nodes and load/save
 *
 */
drush_print("load/save nodes...");
$q = db_query("SELECT nid FROM {node}");
$cnt = 0;
foreach ($q as $r) {
  $nid = $r->nid;
  $cnt++;
  // TODO: reset timeout?
  if (($cnt % 1000) == 0) drush_print("$cnt nodes processed");
  try{
    $node = node_load($nid);
    try{
      node_save($node);
    } catch (Exception $e) {
      $m = $e->getMessage();
      drupal_print("Error saving node $nid: $m");
    }
  } catch (Exception $e) {
    $m = $e->getMessage();
    drush_print("Error loading node $nid : $m");
  }
}
drush_print("$cnt nodes processed");
