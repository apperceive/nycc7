<?php

function nycc_rides_copy_ride_form($form, $form_state, $node) {
  
  // TODO: sanity check on $node and $node->type == 'rides'
  
  drupal_set_title("Copy Ride <em/>" . $node->title . "</em>", PASS_THROUGH);
  
  // TODO: show current date
  
  // TODO: add datepicker
  
  // TODO: CONSIDER: show preview of node or other useful data
  
  // TODO: instructions
  $form['instructions'] = array(
    '#markup' => "<h3>This page is under construction.</h3>",
  );
  
  // text field - copy from additional_dates
  $form['additional_dates'] = array(
    '#type' => 'textarea',
    '#title' => 'Additional dates',
    '#description' => 'Create additional submissions based on this one, but for the list of dates entered here. Separate multiple dates by commas.',
    '#value' => '',           // this is required!!!
    '#prefix' => '',
    '#suffix' => '',
    '#id' => 'edit-nycc-additional-dates',
    '#input' => true,
    '#name' => 'additional_dates',
    '#required' => true,
    '#attributes' => array('class' => array('datepicker')),
    //'#weight' => 10,
    //'#cols' => 60,
    //'#rows' => 1,
    //'#default_value' => 'testing 1 2 3',  // not used?
    //'#resizable' => false,  
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit Copies',
  );
  
  return $form;
  
}

// how best pass $node to callbacks? use $form['#node']???

function nycc_rides_copy_ride_form_validate($form, $form_state) {
  // copy from additional_dates, do not allow current date
}


function nycc_rides_copy_ride_form_submit($form, $form_state) {
  // copy from additional_dates, but use values
}


