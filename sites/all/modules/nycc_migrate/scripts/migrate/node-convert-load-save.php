<?php

/*
 * loop over all nodes and load/save
 *
 */
 
 // TODO: add more stats: read errors, save errors, total
 // TODO: option for show total first and counts in increments (1000 default)
 // TODO: option for read only 
 // TODO: add comments processing? last comment -> comment_node_statistics.cid 
 // TODO: option for save errors until end or seaparate output?

$debug = drush_get_option(array('sql'), FALSE);
$no = drush_get_option(array('no'), FALSE);
$where = drush_get_option(array('where'), '');
$where = $where ? "WHERE $where " : "0=1";          // default is no nodes

drush_print("Load/save nodes...");
$q = db_query("SELECT nid FROM {node} $where");
$cnt = 0;
foreach ($q as $r) {
  $nid = $r->nid;
  $cnt++;
  // TODO: reset timeout? not needed in cli/drush?
  if (($cnt % 1000) == 0) drush_print("$cnt nodes processed");
  try{
    $node = node_load($nid);
    
    /* perform special operataions (eg, forum_index) */
    if ($node && $node->type == 'forum' && $node->status)  {
      _forum_update_forum_index($node->nid);
      
      /*
      // TODO: omit in d7? looks this is causing dups
      $qq = db_query("SELECT cid FROM comment WHERE nid = :nid", array(':nid' => $nid));
      foreach($qq as $rr)
        _forum_update_forum_index($rr->cid);
      */
    }
    
    try{
      
      // TODO: test for missing fields/values?
      
      node_save($node);
    } catch (Exception $e) {
      $m = $e->getMessage();
      drush_print("Error saving node $nid: $m");
    }
  } catch (Exception $e) {
    $m = $e->getMessage();
    drush_print("Error loading node $nid : $m");
  }
}
drush_print("$cnt nodes processed.");
