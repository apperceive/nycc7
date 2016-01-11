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

// use an expression instead of source col value specified by argument
$sourceexp = drush_get_option(array('sourceexp'), '');
// add an extra column and value for format fields, etc NOTE: only one allowed 
$addcol = drush_get_option(array('addcol'), "");   
// append where not null on source expression
$notnull = drush_get_option(array('notnull'), FALSE);
// do not truncate target table
$truncate = drush_get_option(array('notruncate'), FALSE);

// set for content_type_ source tables, leave blank for content_field_ sources
$type = drush_get_option(array('type'), '');
// default field suffix, set to uid, nid, tid, etc as for non text fields
// TODO: change this to value, formatted, userref, noderef, term, etc 
// so as to allow for more actions (e.g. add's fields)
$kind = drush_get_option(array('kind'), 'value');

// default is source field is prefix_arg_suffix_kind
$targetkind = drush_get_option(array('targetkind'), $kind);
$targetfield = drush_get_option(array('targetfield'), '');

// do not auto add prefix and suffix (target and source)
// suffixes default to kind/targetkind
// prefixes default to field_data_field_ and field_revision_field_ for targets
// 
$noprefix = drush_get_option(array('noprefix'), FALSE);
$nosuffix = drush_get_option(array('nosuffix'), FALSE);

// additional conditions
$where = drush_get_option(array('where'), '');

// development options
$trace = drush_get_option(array('trace'), FALSE);
$debug = drush_get_option(array('sql'), FALSE);
$no = drush_get_option(array('no'), FALSE);


$extrccol = "";
$extraval = "";
if ($addcol) {
  list($extracol, $extraval) = explode(",", $addcol);
  $extracol = ", " . $extracol;
  $extraval = ", " . $extraval;
}

// source column names (note that at least one is required, even for expression)
// NOTE: select is based on this argument (eg, foreach row in arg table)
$args = drush_get_arguments();

// specifying type sets source to content_type_, omitting sets source table to content_field source_
$srctype = $type ? 'type' : 'field';

foreach ($args as $ndx => $arg) {
  // skip scr and this script as first two args to drush scr scriptname fielda fieldb
  if ($ndx > 1) {

    $prefixstr = $noprefix ? "" : "field_";
    $suffixstr = $nosuffix ? "" : "_$targetkind";
    
    $srctable = ($noprefix ? "" : "content_{$srctype}_") . ($type ? $type : $arg);
    
    $targetcol = $targetfield ? $targetfield : $arg;
    
    //$expr = $sourceexp ? $sourceexp : "content_$srctable.field_{$arg}_$kind";
    $expr = $sourceexp ? $sourceexp : "{$srctable}.{$prefixstr}{$arg}_{$kind}";
    
    // TODO: these are the same!
    //$srccol = $prefixstr . $targetcol . $suffixstr;
    //$destcol = "{$prefixstr}{$targetcol}{$suffixstr}";

    $tables = array('data', 'revision');
    foreach ($tables as $table) {
   
      
      $stable = "{$srctable}";
      $sexpr = "{$expr}";
      $dtable = "{$prefixstr}{$table}_{$prefixstr}{$targetcol}";
      $dcol = "{$prefixstr}{$targetcol}{$suffixstr}";
      
      $where = $notnull ? (($where ? "WHERE $where AND " : "WHERE ") . "NOT $sexpr IS NULL") : $where;
      
      //if ($table == 'data') drush_print("$sexpr -> $dtable.$dcol");

      
      if (!$notruncate) {
        if ($trace) drush_print("field_copy ($table): truncating $arg ($dtable)");
        $sqlt = "TRUNCATE `$dtable`";
        if (!$no) {
          try {
            db_query($sqlt)->execute();
          }
          catch (Exception $e) {
            $error = $e->getMessage();
            unset($e);
            drush_print("Error: $error");
            if ($debug) drush_print("Defined variables: " . var_export(get_defined_vars(),1));
            else drush_print("SQL: $sqlt");
          }
        }
      }

      $sql =<<<EOS
REPLACE INTO `$dtable` (entity_type, bundle, deleted, entity_id, revision_id, language, delta, $dcol  $extracol) SELECT 'node', IF(LENGTH(TRIM(node.type)) > 0, node.type, 'page'), 0, node.nid, node.vid, 'und', 0, $expr $extraval FROM $sourcedb.`$stable` INNER JOIN $sourcedb.`node` ON ($sourcedb.`$stable`.nid=node.nid AND $sourcedb.`$stable`.vid=node.vid) $where;
EOS;

      //if ($trace) drush_print("field_copy ($table): executing $sourcedb $srctable $type $kind $arg $expr -> $targetcol $targetkind");
      if ($trace) drush_print("field_copy ($table): executing $sourcedb $type $kind $stable $sexpr -> $dtable $tcol $targetkind");
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
}



