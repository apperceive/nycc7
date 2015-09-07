<?php

function nycc_rides_copy_ride_form($form, $form_state, $node) {
  
  // instructions
  
  // text field - copy from additional_dates
  $form['additional_dates'] = array(
    '#type' => 'textfield',
    '#title' => 'Additional dates',
    '#description' => 'Optional. Create additional submissions based on this one, but for the list of dates entered here. Separate multiple dates by commas.',
    '#weight' => 10,
    '#value' => '',           // this is required!!!
    '#prefix' => '',
    '#suffix' => '',
    '#id' => 'edit-nycc-additional-dates',
    '#input' => true,
    '#name' => 'additional_dates',
    '#attributes' => array('class' => array('datepicker')),
    //'#cols' => 60,
    //'#rows' => 1,
    //'#default_value' => 'testing 1 2 3',  // not used?
    //'#resizable' => false,  
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit copies for ride approval.',
  );
  
  return $form;
  
}

function nycc_rides_copy_ride_form_validate($form, $form_state) {
  // copy from additional_dates
}


function nycc_rides_copy_ride_form_submit($form, $form_state) {
  // copy from additional_dates, but use values
}


