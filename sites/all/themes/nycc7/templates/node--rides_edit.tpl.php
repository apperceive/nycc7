<?php
/**
 * @file
 * Default theme implementation to edit a node.
 *
 * Available variables:
 *
 *
 * Notes:
 *  1. this is called for rides node add, node edit and node content type edit 
 *
 */
 
  // note: customizations in nycc_rides module's 
  // nycc_rides_output_ride_node_form
  // and
  // nycc_rides_form_alter
 
  //dpm($form);
  //dpm(array_keys(get_defined_vars()));
  //dpm(get_defined_vars());
  //print "<pre>" . var_export(array_keys($form), 1) . "</pre>";
  // print render($form['group_rides_htabs']);

  
  // todo: align fields on details tab with bootstrap row and cols
  // todo: next/prev for add (but not edit) or use pages and pagegroup?
  
  
  $output = '';
  // move to nycc_rides_processor_node_ride_edit()
  $op = (arg(1) == 'add') ? 'add' : ((arg(2) == 'edit') ? 'edit' : 'noop');
  
  $can_approve = function_exists('nycc_rides_can_approve') ? nycc_rides_can_approve() : false;
  
  if (!$can_approve) {
    hide($form['group_rides_htabs']['group_ride_rc_info']);
    hide($form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_riders']);
    hide($form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_waitlist']);
      
    //hide($form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_attendees']);
  } 
  else {
    /*
    $form['group_rides_htabs']['group_ride_rc_info']['revs'] = array(
      '#markup' => drupal_render($form['revision_information']),
    );    
    */
  }
  // hide extra fields from all unless '/admin' is appended to url
  if (arg(3) != 'admin') {
    hide($form['group_rides_htabs']['group_rides_advanced']);
    hide($form['additional_settings']);
  }
  
  
  // where is best place to do this? does not work in hook_form_alter as 'group_rides_htabs' key does not yet exist yet
      
  //dpm($form['group_rides_htabs']['group_ride_participants']['group_rides_participants']);
  
  $form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_leaders']['field_ride_current_leaders']['und']['add_more']['#value'] = t("Add ride leader");
  
  $form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_riders']['field_ride_current_riders']['und']['add_more']['#value'] = t("Add rider");
  
  $form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_waitlist']['field_ride_waitlist']['und']['add_more']['#value'] = t("Add to waitlist");
  
  $form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_attendees']['field_ride_attendees']['und']['add_more']['#value'] = t("Add attendee");
  
  
  // note various spellings of attachments/attachements. ugh!
  //dpm($form['group_rides_htabs']['group_ride_attachements']);
  
  $form['group_rides_htabs']['group_ride_attachements']['field_ride_image']['und']['#file_upload_title'] = t("Add another image");
  
  $form['group_rides_htabs']['group_ride_attachements']['field_ride_attachments']['und']['#file_upload_title'] = t("Add another attachment");
  
  $next =  array(
    '#type'   => 'submit',
    '#value'  => 'Next',      // add icon
    '#weight' => 49,
    '#attributes' => array('class' => array('btn btn-default btn-lg')),
    '#submit' => array('jquery_form_submit'),
  );
  $form['group_rides_htabs']['group_rides_info']['next'] = $next;
  $form['group_rides_htabs']['group_ride_info']['next'] = $next;
  $form['group_rides_htabs']['group_ride_attachements']['next'] = $next;

  $form['actions']['submit']['#attributes'] = array('class' => array('btn btn-lg'));
  $form['actions']['submit']['#weight'] = 50;
  
  $form['group_rides_htabs']['group_ride_attachements']['submit'] = $form['actions']['submit'];
  $form['group_rides_htabs']['group_ride_participants']['submit'] = $form['actions']['submit'];
  $form['group_rides_htabs']['group_ride_rc_info']['submit'] = $form['actions']['submit'];
  
  if ($op == 'add') {
  }
  
  if ($op == 'edit') {
  }
  
 hide($form['actions']);
  
  $output .= drupal_render_children($form);
  
  //$output .= drupal_render($form['actions']['submit']);

?>

<?php print $output;  ?>

