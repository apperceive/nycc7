<?php
/**
 *
 * Drush Module for NYCC Data Migration and other tasks
 *
 */

/**
 * Implementation of hook_drush_command()
 */
function nycc_migrate_drush_command() {
  $items = array();

  $items['nyccmig-command'] = array(
    'description' => 'Example NYCC Migrate command',
    'alias' => array('nyccmig-cmd'),
    'arguments' => array(
      'username' => 'Some text.',
    ),
  );

  $items['nyccmig-status'] = array(
    'description' => 'Display information about Drupal site data',
    'alias' => array('nyccmig'),
    'arguments' => array(
      'name' => 'Name of status query to display.',
    ),
    'options' => array(
      'all' => 'Output everything.',
      'format' => 'Output format: json, text, html',
      'source' => 'Source database. Leave blank for no source.',
      'destination' => 'Destination database. Leave blank for default of current @site',
      'format' => 'Output format: json, text, html',
    )
  );

  
  $items['nyccmig-rules-list'] = array(
    'description' => "List all the active rules for your site.",
    'drupal dependencies' => array('rules'),
    'aliases' => array('nyccmig-rules'),
    'options' => array(
      'status' => 'List only enabled or disabled rule names.',
    )
  );  
  
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function drush_nycc_migrate_drush_help($section) {
  switch ($section) {
    case 'drush:nyccmig-command':
      return dt("Example NYCC Migrate command.");
    case 'drush:nyccmig-status':
      return dt("Display database status (work in progress).");
    case 'drush:nyccmig-rules-list':
      return dt("List all the rules on your site.");

  }
}



function drush_nycc_migrate_command($username = '') {

  drush_print("Inside drush_nycc_migrate_command");

}


function drush_nycc_migrate_status() {
  $output = "";
  
  // args:
  // --destination
  // --source
  // blank destination defaults to current @site
  // check that @site is d7, others can be anything
  // blank source means just destination
  // default query is record count for table with that name if it exists
  
  // output to an (table) array and call theme_table?
  // json output
  // --format
  
  $format = drush_get_option('format', 'json');  // allow html, text and json use --pipe?
  
  $source = drush_get_option('source', 'json');  // allow html, text and json use --pipe?
  
  $destination = drush_get_option('destination', 'json');  // allow html, text and json use --pipe?
  
  
  $sqls = array(
    'users' => "SELECT 'total' AS name, COUNT(*) AS cnt FROM users",
    'node' => "SELECT 'total' AS name, COUNT(*) AS cnt FROM node",
    'role' => "SELECT 'total' AS name, COUNT(*) AS cnt FROM role",
    'nodetypes' => "SELECT type AS name, COUNT(*) AS cnt FROM node GROUP BY type ORDER BY type",
    'usersroles' => "SELECT r.name AS name, COUNT(*) AS cnt FROM users u, role r, users_roles ur WHERE u.uid=ur.uid AND ur.rid=r.rid GROUP BY r.name ORDER BY r.name",
  );

  if (drush_get_option('all')) {
    foreach ($sqls as $key => $sql) {
      $q = db_query($sql);
      if ($q) {
        foreach ($q as $r) {
          $output .= $key . " : " . $r->name . " : " . $r->cnt . "\n";
        }
      }
    }
  }
  else {
    while ($arg = drush_shift()) {
      if ($arg == 'nyccmig-status')
        continue;
      if (array_key_exists($arg, $sqls)) {
        $q = db_query($sqls[$arg]);
        if ($q) {
          //$output .= $title . "\n";
          foreach ($q as $r) {
            $output .= $arg . " : " . $r->name . " : " . $r->cnt . "\n";
          }
        }
      }
      else {
        $output .= "$arg : not found\n";
      }
    }
  }


  if ($format == 'html') {
    $data = array(array(1,2,3), array(4,5,6));
    $headers = array('name','src','dest');
    $vars = array(
      'header' => $headers,
      'rows' => $data,
      'attributes' => array(),
      'colgroups' => array(),
      'caption' => 'status',
      'sticky' => FALSE,
      'empty' => 'no data',
    );
    drush_print(theme_table($vars));
  }
  else {    
    drush_print($output);
  }

  
}

function drush_nycc_migrate_rules_list() {
  
  $status = drush_get_option('status', 'all');  
  
  $rules = rules_config_load_multiple(FALSE);
  $rows = array();
  foreach ($rules as $rule) {
    if (!empty($rule->name) && !empty($rule->label)) {
      $events = array();
      $event_info = rules_fetch_data('event_info');
      if ($rule instanceof RulesTriggerableInterface) {
        foreach ($rule->events() as $event_name) {
          $event_info += array($event_name => array('label' => dt('Unknown event "!event_name"', array('!event_name' => $event_name))));
          $events[] = check_plain($event_info[$event_name]['label']);
        }
      }
      if ($status == 'all' || (($status == 'disabled') && !$rule->active) || (($status == 'enabled') && $rule->active)) 
        $rows[] = $rule->name;
        /*$rows[] = array(
          $rule->name,
          $rule->label,
          implode(', ', $events),
          $rule->active ? dt('Enabled') : dt('Disabled'),
          $rule->status ? theme('entity_status', array('status' => $rule->status, 'html' => FALSE)) : '',
        );*/
    }
  }
  drush_print(implode(' ', $rows));
}
