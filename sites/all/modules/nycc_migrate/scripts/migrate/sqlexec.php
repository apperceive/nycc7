<?php

// used for testing drush commands

$sourcedb = drush_get_option(array('sourcedb'), '');
$targetdb = drush_get_option(array('targetdb'), '');
$debug = drush_get_option(array('sql'), FALSE);
$no = drush_get_option(array('no'), FALSE);

$sourcedb = $sourcedb ? $sourcedb . '.' : '';
$targetdb = $targetdb ? $targetdb . '.' : ''; 

$args = drush_get_arguments();

foreach ($args as $ndx => $arg) {
  // skip scr and this script as first two args to drush scr scriptname fielda fieldb
  if ($ndx > 1) {
    $text = file_get_contents($arg);
    $sql = str_replace(array('$sourcedb', '$targetdb'), array($sourcedb, $targetdb), $text);
    drush_print("sqlexec: executing $arg $sourcedb $targetdb");
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