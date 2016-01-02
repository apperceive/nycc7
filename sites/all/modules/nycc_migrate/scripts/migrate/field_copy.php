<?php

// run from drush:  drush @site scr this.php --type=rides fielda fieldb

// options apply to all fields in command line

// kind: value, nid, uid, tid, etc
// type: type or field, default is type but also sets another template patter to blank 
// todo: separate type, remove double replace on WWWWW
// --no sill not execute (like -n) - todo: use -n
// --sql dumps debug data - how use --debug without effecting all of drush?
// target fields and kind for changes like ride_leaders to ride_current_leaders

// environment exports: 
//$sourcedb = $_ENV["sourcedb"];

// options:
$sourcedb = drush_get_option(array('sourcedb'), 'd6test');
$sourceexp = drush_get_option(array('sourceexp'), '');
$type = drush_get_option(array('type'), '');
$kind = drush_get_option(array('kind'), 'value');
$targetfield = drush_get_option(array('targetfield'), '');
$targetkind = drush_get_option(array('targetkind'), $kind);
$where = drush_get_option(array('where'), '');
$where = $where ? "WHERE $where" : '';
$debug = drush_get_option(array('sql'), FALSE);
$no = drush_get_option(array('no'), FALSE);

// source column names (note that at least one is required, even for expression)
$args = drush_get_arguments();

$srctype = $type ? 'type' : 'field';

foreach ($args as $ndx => $arg) {
  // skip scr and this script as first two args to drush scr scriptname fielda fieldb
  if ($ndx > 1) {

    $targetcol = $targetfield ? $targetfield : $arg;
    $srctable = $srctype . "_" . ($type ? $type : $arg);
    $expr = $sourceexp ? $sourceexp : "content_$srctable.field_{$arg}_$kind";

    $sql =<<<EOS
REPLACE INTO field_data_field_$arg (entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_{$targetcol}_$targetkind) SELECT 'node', IF(LENGTH(TRIM(node.type)) > 0, node.type, 'page'), 0, node.nid, node.vid, 'und', 0, $expr FROM $sourcedb.content_$srctable INNER JOIN node ON ($sourcedb.content_$srctable.nid=node.nid AND $sourcedb.content_$srctable.vid=node.vid) $where;
EOS;

    drush_print("field_copy: executing $sourcedb $type $kind $arg $expr -> $targetfield $targetkind");
    if ($debug) drush_print('Debug: defined variables ' . var_export(get_defined_vars(),1));
    if (!$no) {
      try {
        db_query($sql)->execute();
      }
      catch (Exception $e) {
        $error = $e->getMessage();
        unset($e);
        drush_print("Error: $error");
        if ($debug) drush_print("Defined variables: " . var_export(get_defined_vars(),1));
        else drush_print("SQL: $sql");
      }
    }
  }
}



