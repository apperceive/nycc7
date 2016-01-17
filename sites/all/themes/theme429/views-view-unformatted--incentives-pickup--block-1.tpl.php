<?php
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
  $counts = array();
  
  print "<h3>$title</h3>";

  foreach ($rows as $id => $row) {
    $counts[$row] = 0;
    //print "<div>$row</div>";
  } // foreach
  
  foreach ($rows as $id => $row) {
    $counts[$row]++;
    //print "<div>$row</div>";
  } // foreach

  foreach ($counts as $key => $count) {
    print "<div>" . $count . " - " . $key . "</div>";
  }
  
  if (!count($counts))
    print "There are no requests at this time.";
  
  //dpm(get_defined_vars());
?>



