<?php

function nycc_rides_copy_ride_form($form, &$form_state, $node) {
  
  // TODO: sanity check on $node and $node->type == 'rides'
  
  drupal_set_title("Copy Ride <em/>" . $node->title . "</em>", PASS_THROUGH);

  // TODO: CONSIDER: show preview of node or other useful data
  // TODO: show current date
  
  // TODO: better instructions
  $form['instructions'] = array(
    '#markup' => "<p>Use this form to 'clone' a ride you like to additional dates.  All ride information, except the date and riders, will be copied to the new rides. This will create new rides with the same title, leaders, descrition, cue sheets, times and other settings.</p>",
  );
  
  // text field - copy from additional_dates
  $form['additional_dates'] = array(
    '#type' => 'textfield',
    '#title' => "Replicate <em/>" . $node->title . "</em> to the following dates",
    '#description' => 'Create additional submissions based on this one, but for the list of dates entered here. Separate multiple dates by commas.',
    '#prefix' => '',
    '#suffix' => '',
    '#id' => 'edit-nycc-additional-dates',
    '#name' => 'additional_dates',
    '#required' => true,
    '#attributes' => array(
      'class' => array('datepicker'), 
      'placeholder' => 'Example: ' . date("M d Y", strtotime('+1 month')) . ', ' . date("M d Y", strtotime('+2 month')),
    ),
    //'#weight' => 10,
    //'#cols' => 60,
    //'#rows' => 1,
    //'#resizable' => false,  
    //'#default_value' => 'testing 1 2 3',  // not used?
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit Copies',
  );
  
  $form['#after_build'] = array('_nycc_rides_custom_form_uidatepicker');
  
  if (!array_key_exists('#node', $form))
    $form['#node'] = $node;

  return $form;
  
} // nycc_rides_copy_ride_form

// how best pass $node to callbacks? use $form['#node']???

function nycc_rides_copy_ride_form_validate($form, &$form_state) {

  // TODO: refactor this to a utilty function to validate a list of dates and return errors

  $additional_dates = $form_state['values']['additional_dates'];
  $additional_dates = check_plain($additional_dates);
  $additional_dates = trim($additional_dates);
  //$additional_dates = str_replace(' ', '', $additional_dates);
  
  if (drupal_strlen($additional_dates)) {
    // parse dates, validate each one
    // TODO: remove dups and dup of current node date
    $errors = array();
    $adsarr = explode(",", $additional_dates);
    if (is_array($adsarr)) {
      foreach ($adsarr as $ds) {
        $d = strtotime($ds);
        if (!$d)
          $errors[] = "$ds ($d)";
      } // for
      if (count($errors)) {
        $errstr = theme('item_list', array('items' => $errors));
        form_set_error('additional_dates', "Invalid additional dates: $errstr");
      } // count
      if (array_key_exists('additional_dates', $form))
        form_set_value($form['additional_dates'], $additional_dates, $form_state);  
    } // if array
  } // if additional dates
  else {
    // drupal_set_message('$_POST["additional_dates"] has no length');    
  }

} // nycc_rides_copy_ride_form_validate


function nycc_rides_copy_ride_form_submit($form, &$form_state) {

  $additional_dates = $form_state['values']['additional_dates'];
  $additional_dates = check_plain($additional_dates);
  $additional_dates = trim($additional_dates);

  $onode = $form['#node'];

  if (drupal_strlen($additional_dates)) {
    $adsarr = explode(",", $additional_dates);
    if (is_array($adsarr)) {
      foreach ($adsarr as $ds) {
        $d = strtotime($ds);
        
        $new_node = _nycc_rides_clone_ride($onode, $d);
        if (isset($new_node) && is_object($new_node)) {
          watchdog('nycc_rides', "nycc_rides_copy_ride_form_submit - successfully cloned ride :onid to ride :nnid for date $ds",
                   array(':onid' => $onode->nid, ':nnid' => $new_node->nid), WATCHDOG_INFO);
  
          $nid = $new_node->nid;
          $title = $new_node->title;
          $datestr = date("Y-m-d", $datetime);

          drupal_set_message(check_markup("Recurring ride for <a href='/node/$nid' title='View recurring ride in another tab' target='_blank'>$title</a> created with date $datestr", 'filtered_html', '', FALSE));
            
        } else {
          drupal_set_message("Unable to clone the ride. Please contact the webmaster.");
          watchdog('nycc_rides', 'nycc_rides_copy_ride_form_submit - Failed to clone ride :onid', array(':onid' => $onode->nid), WATCHDOG_ERROR);
        }
        
      } // for
    } // if array
  } // if additional dates
} // nycc_rides_copy_ride_form_submit


