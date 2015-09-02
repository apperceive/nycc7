<?php
/**
 * @file
 * Default theme implementation to edit a node.
 *
 * Available variables:
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

  $output = '';
  // move to nycc_rides_processor_node_ride_edit()
  $op = (arg(1) == 'add') ? 'add' : ((arg(2) == 'edit') ? 'edit' : 'noop');
  $can_approve = nycc_rides_can_approve();
  
  if (!$can_approve) {
    hide($form['group_rides_htabs']['group_ride_rc_info']);
    hide($form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_riders']);
    hide($form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_waitlist']);
    hide($form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_attendees']);
  }
  
  // where is best place to do this? does not work in hook_form_alter as 'group_rides_htabs' key does not yet exist yet
      
  //dpm($form['group_rides_htabs']['group_ride_participants']['group_rides_participants']);
  
  $form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_leaders']['field_ride_current_leaders']['und']['add_more']['#value'] = t("Add another ride leader");
  
  $form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_riders']['field_ride_current_riders']['und']['add_more']['#value'] = t("Add another rider");
  
  $form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_waitlist']['field_ride_waitlist']['und']['add_more']['#value'] = t("Add to waitlist");
  
  $form['group_rides_htabs']['group_ride_participants']['group_rides_participants']['group_rides_attendees']['field_ride_attendees']['und']['add_more']['#value'] = t("Add another attendee");
  
  //dpm($form['group_rides_htabs']['group_ride_attachements']);
  
  $form['group_rides_htabs']['group_ride_attachements']['field_ride_image']['und']['#file_upload_title'] = t("Add another image");
  
  // note various spellings of attachments/attachements. ugh!
  $form['group_rides_htabs']['group_ride_attachements']['field_ride_attachments']['und']['#file_upload_title'] = t("Add another attachment");

  /*
  // add attitional dates field next to timestamp
  // todo: make collapsable? collpased?
  $form['group_rides_htabs']['group_rides_info']['field_ride_timestamp']['und'][0]['value']['additional_dates'] = array(
    '#type' => 'textarea',
    '#title' => 'Additional dates',
    '#description' => 'Optional. Create additional submissions based on this one, but for the list of dates entered here. Separate multiple dates by commas.',
    '#cols' => 60,
    '#rows' => 1,
    '#weight' => 10,
    //'#default_value' => 'testing 1 2 3',
    '#value' => 'initial value',  // required!!!
    '#prefix' => '',
    '#suffix' => '',
    '#resizable' => false,  // just kills window-shade sizer, not bootstrap or browsers
    //'#tree' => false, 
    //'#parents' => array('additional_dates'),
  );
  */
 $form['group_rides_htabs']['group_rides_info']['field_ride_timestamp']['und'][0]['value']['additional_dates'] = array(
      '#type' => 'textfield',
      '#title' => 'Additional dates',
      '#description' => 'Optional. Create additional submissions based on this one, but for the list of dates entered here. Separate multiple dates by commas.',
      //'#cols' => 60,
      //'#rows' => 1,
      '#weight' => 10,
      '#value' => '',           // this is required!!!
      '#prefix' => '',
      '#suffix' => '',
      '#id' => 'edit-additonal_dates',
      //'#default_value' => 'testing 1 2 3',  // not used?
      '#input' => true,
      '#name' => 'additional_dates',
      //'#resizable' => false,  // just kills window-shade sizer, not bootstrap or browsers
      //'#tree' => true, 
      //'#parents' => array('group_rides_htabs', 'group_rides_info', 'field_ride_timestamp', 'und', 0, 'value', 'additional_dates'),
      //'#parents' => array('field_ride_timestamp', 'und', 0, 'value', 'additional_dates'),
      //'#parents' => array('additional_dates'),
      //'#processed' => true,
    ); 
  
  //dpm($form['group_rides_htabs']['group_rides_info']['field_ride_timestamp']['und'][0]['value']);
  
  
  if ($op == 'add') {
    hide($form['group_rides_htabs']['group_ride_rc_info']);
    // todo: hide leaders fieldset, but keep leader userref field?
  }
  
  if ($op == 'edit') {
    /*
    if ($status == 'Approved')
      $output .= drupal_render($form['revision_information']);
    */
  }
  
  hide($form['actions']);
  
  $output .= drupal_render_children($form);
  $output .= drupal_render($form['actions']['submit']);
  
?>

<?php print $output;  ?>

