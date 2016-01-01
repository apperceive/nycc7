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
$sourcedb = $_ENV["sourcedb"];

// options:
$type = drush_get_option(array('type'), '');
$kind = drush_get_option(array('kind'), 'value');
$debug = drush_get_option(array('sql'), FALSE);
$no = drush_get_option(array('no'), FALSE);
$targetfield = drush_get_option(array('targetfield'), '');
$targetkind = drush_get_option(array('targetkind'), $kind);

$sqltemp =<<<EOS
REPLACE INTO field_data_field_XXXXX
(entity_type,
bundle,
deleted,
entity_id,
revision_id,
language,
delta,
field_YYYYY_KKKKK)
SELECT 
  'node',
  node.type,
  0,
  node.nid,
  node.vid,
  'und',
  0,
  $sourcedb.field_XXXXX_JJJJJ
FROM $sourcedb.content_AAAAA INNER JOIN node ON ($sourcedb.content_AAAAA.nid=node.nid AND $sourcedb.content_AAAAA.vid=node.vid);
EOS;

$args = drush_get_arguments();
// skip scr and this script as first two args to drush scr ./YYYYY.php fielda fieldb
foreach ($args as $ndx => $arg) {
  if ($ndx > 1) {
    $sql = str_replace(array('AAAAA', 'KKKKK', 'JJJJJ', 'ZZZZZ', 'WWWWW', 'XXXXX', 'YYYYY'), array(($type ? 'ZZZZZ_WWWWW' : 'ZZZZZ_XXXXX'), $kind, $targetkind, ($type ? 'type' : 'field'), $type, $arg, ($targetfield ? $targetfield : $arg)), $sqltemp);
    drush_print("field_copy: executing $sourcedb $type $kind $arg -> $targetfield $targetkind");
    if ($debug) drush_print("type: $type, kind: $kind, sql: $sql");
    if (!$no) db_query($sql)->execute();
  }
}



