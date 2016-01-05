<?php

// only works on D7

$type = drush_get_option(array('type'), 'node');
//$args = drush_get_arguments();

drush_print(drupal_json_encode(array($type => array_keys(field_info_instances("node", $type)))));
  