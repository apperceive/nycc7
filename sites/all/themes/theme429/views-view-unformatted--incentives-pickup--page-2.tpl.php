<?php
/**
 * @file 
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
  $counts = array();
  
  print "<h3>$title</h3>";

  print "<h3>Count - Item</h3>";


  foreach ($rows as $id => $row) {
    $counts[$row] = 0;
  } // foreach


  foreach ($rows as $id => $row) {
    $counts[$row]++;
  } // foreach

  foreach ($counts as $key => $count) {
    print "<div>$count - $key</div>";
  }
  
  //dpm(get_defined_vars());
?>


