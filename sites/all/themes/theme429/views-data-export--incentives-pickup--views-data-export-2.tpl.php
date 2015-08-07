<?php


/*
  Available variables:

  $template_file
  $variables
  $template_files
  $view
  $options
  $rows
  $title
  $zebra
  $id
  $directory
  $is_admin
  $is_front
  $logged_in
  $db_is_active
  $user
  $header
  $body
  $footer


*/


  $counts = array();
 //$vars = get_defined_vars();
 //$keys = array_keys($vars);

 //print var_export($keys, 1);


  foreach ($rows as $id => $row) {
    $counts[$row] =0;
  } // foreach
  
  foreach ($rows as $id => $row) {
    $counts[$row]++;
  } // foreach

  foreach ($counts as $key => $count) {
    if (drupal_strlen(trim($key)))
      print $count . " - " . $key . "\n";
  }
  
  
  //print var_export($rows, 1);

