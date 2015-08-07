<?php

  global $user;

  // todo: check that status is not already rejected or accepted
  $field = $node->field_incentive_status;
  $status = isset($field) && is_array($field) ? $field[0]['value'] : "";
  $nycc_admin_incentives = user_access('coordinate nycc incentives', $user) || user_access('administer nycc incentives', $user);

  $acct = $user;
  if ($node->uid)
    $acct = user_load($node->uid);
  $uid = $acct->uid;
  $prevyear = date("Y") - 1;
  $cp = content_profile_load('profile', $acct->uid);
  if ($cp) {
    $name = "<a href='/user/$uid' target='_blank'>". $cp->field_first_name[0]['value'] ." ". $cp->field_last_name[0]['value'] ."</a>";
    $title .= " for $name for $prevyear";
  }
  drupal_set_title($title);

  /*
  $incentive = "";
  foreach ($taxonomy as $term) {
    if (drupal_strlen($incentive))
      $incentive .= " - ";
    $incentive .= $term['title'];
  } // foreach term
  */
  //dpm(get_defined_vars());

?>
  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
    <div class="indent">

      <div class="title">
        <?php print $picture ?>
        
        <h1><?php print $title ?></h1>
        
        <div class="date"><?php print $submitted ?></div> 
        
      </div>
      
      <?php if ($nycc_admin_incentives && ('Submitted' == $status)) : ?>
      <div class='nycc-buttons'>
          <span class='nycc-button-wrapper'><a href='/incentives/update/status?nid=<?php print $nid?>&status=Validated'>Accept</a></span>
        
          <span class='nycc-button-wrapper'><a href='/incentives/update/status?nid=<?php print $nid?>&status=Rejected'>Reject</a></span>
        </div>
      <?php endif; ?>
    
      <div class="text-box"><?php print $content ?></div>
      
      <?php if ($links): ?>
        <div class="post-links"><?php print $links ?></div>
      <?php endif; ?>

    </div>
  </div>
