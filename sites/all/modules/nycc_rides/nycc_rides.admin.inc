<?php
function nycc_rides_admin() {
  $form = array();
 
	
	$form['nycc_rides_debug_to_log'] = array(
		'#type' =>'checkbox', 
		'#title' => t('Write debug messages to Database Log'),
		'#default_value' => variable_get('nycc_rides_debug_to_log', '')
	);

    
	$form['nycc_rides_days_before_ride'] = array(
		'#type' =>'textfield', 
		'#title' => t('Enable Rides XX Days Before Ride Start'),
		'#description' => t('Enter the number of days prior to a Ride when signup for rides should be enabled'),
		'#default_value' => variable_get('nycc_rides_days_before_ride', '10')
	); 		     
  return system_settings_form($form);
}

function nycc_rides_admin_validate($form, $form_state) {
	if (empty($form_state['values']['nycc_rides_days_before_ride'])) {
    	form_set_error('nycc_rides_days_before_ride', "Days before Ride can not be blank");
	} else {
      if (!is_numeric($form_state['values']['nycc_rides_days_before_ride'])) {
    	form_set_error('nycc_rides_days_before_ride', "Days before Ride must be numeric");      
		}	
	}
}


?>