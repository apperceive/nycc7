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
 
// make different buttons for different tabs href#
function _nycc_rides_form_nav_button($value, $href, $weight = 50) {
  $btn =  array(
    '#type'   => 'button',
    '#value'  => $value,      // add icon ?
    '#weight' => $weight,
    '#attributes' => array(
      'class' => array('btn btn-default btn-lg'), 
      'onclick' => "javascript: jQuery('a[href=\"#$href\"]').click(); return false;"
    ),
  );
  return $btn;
}
 
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
  //dpm($form['group_rides_htabs']['group_ride_attachements']['group_rides_attachements']);
  
  $form['group_rides_htabs']['group_ride_attachements']['group_rides_attachements']['group_rides_images']['field_ride_image']['und']['#file_upload_title'] = t("Add another image");
  
  $form['group_rides_htabs']['group_ride_attachements']['group_rides_attachements']['group_rides_attachments']['field_ride_attachments']['und']['#file_upload_title'] = t("Add another attachment");

  // add prev/next buttons
  $form['group_rides_htabs']['group_rides_info']['next'] = _nycc_rides_form_nav_button('Next', 'group-ride-info', 49);
  
  // undefined is not unique, seems to be first of each tab group
  $form['group_rides_htabs']['group_ride_info']['prev'] = _nycc_rides_form_nav_button('Prev', 'undefined', 48);
  $form['group_rides_htabs']['group_ride_info']['next'] = _nycc_rides_form_nav_button('Next', 'group-rides-images', 49);
  
  $form['group_rides_htabs']['group_ride_attachements']['prev'] = _nycc_rides_form_nav_button('Prev', 'group-ride-info', 48);
  $form['group_rides_htabs']['group_ride_attachements']['next'] = _nycc_rides_form_nav_button('Next', 'group-ride-participants', 49);
  
  $form['group_rides_htabs']['group_ride_participants']['prev'] = _nycc_rides_form_nav_button('Prev', 'group-rides-images', 48);  
  
  if ($can_approve) {
    $form['group_rides_htabs']['group_ride_participants']['next'] = _nycc_rides_form_nav_button('Next', 'group-ride-rc-info', 49);
    $form['group_rides_htabs']['group_ride_rc_info']['prev'] = _nycc_rides_form_nav_button('Prev', 'group-ride-participants', 49);
  }
  
  // alter submit button
  $form['actions']['submit']['#attributes'] = array('class' => array('btn btn-lg'));
  $form['actions']['submit']['#weight'] = 50;
  
  // copy submit button to last two tabs 
  $form['group_rides_htabs']['group_ride_participants']['submit'] = $form['actions']['submit'];
  $form['group_rides_htabs']['group_ride_rc_info']['submit'] = $form['actions']['submit'];
  
  // add prev/next for rc tab
  if ($can_approve) {
    $form['group_rides_htabs']['group_ride_participants']['next'] = _nycc_rides_form_nav_button('Next', 'group-ride-rc-info', 49);
    $form['group_rides_htabs']['group_ride_rc_info']['prev'] = _nycc_rides_form_nav_button('Prev', 'group-ride-participants', 49);
  }
  
  if ($op == 'add') {
  }
  
  if ($op == 'edit') {
    // show submit on all tabs
    $form['group_rides_htabs']['group_rides_info']['submit'] = $form['actions']['submit'];
    $form['group_rides_htabs']['group_ride_info']['submit'] = $form['actions']['submit'];
    $form['group_rides_htabs']['group_ride_attachements']['submit'] = $form['actions']['submit'];
  }
  
  // hide buttons
  hide($form['actions']);
  
  $output .= drupal_render_children($form);
  
  //$output .= drupal_render($form['actions']['submit']);

?>

<?php print $output;  ?>

