<?php

/*
 * Ride related block functions
 *
 *
*/




// this code is currently inside one of the block visibility conditions as php format
// todo: plan better place for this and how things like this are handled
// nycc_blocks.module or nycc_site.module or membership?
// called by http://markus.test.nycc.org/admin/structure/block/manage/block/21/configure
// function to replace block code
// moved back to block due to scoping issues
/*
function nycc_rides_renew_button_pages() {
  $editnids = array('2', '45511', '27725', '13902', '45415', '45416', '4', '45421', '6', '5', '45457', '45457', '45475', '45512', '45517', '45519', '45520', '45521');
  
  if (drupal_is_front_page() || 
      (arg(0) == "node" && arg(2) != "edit" && in_array(arg(1), $editnids)) ||
      (arg(0) == "node" && arg(1) =="add" && arg(2) == "rides") ||
      (arg(0) == "upcoming-events") )
    return true;

  $node = node_load(arg(1));
  if (!$node)
      return false;
  
  $node_type = $node->type;
  
  if ((($node_type == "rides") && (arg(0)  != "current-riders")) ||
      (($node_type == "region") && (arg(2) != "edit")) ||
      (($node_type == "cue_sheet") && (arg(2) != "edit")) ||
      (($node_type == "product") && (arg(2) != "edit") && (arg(1) != 27756)) ||
      (($node_type == "message_board") && (arg(2) != "edit")))
    return true;

return false;

} // nycc_rides_renew_button_pages
*/
