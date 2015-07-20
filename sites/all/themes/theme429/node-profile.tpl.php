<?php
// $Id: node.tpl.php,v 1.4 2007/08/07 08:39:36 goba Exp $
// redirect profile node to user profile tab
if ((arg(0) == "node") && (arg(1) > 0)  && !arg(2)) {
  $uid = $node->uid;
  $url = $base_url . drupal_get_path_alias("/user/$uid");
  header("Location:$url");
}
?>

