<?php
function nycc_rides_admin() {
  $form = array();
 
	
	$form['nycc_rides_debug_to_log'] = array(
		'#type' =>'checkbox', 
		'#title' => t('Write debug messages to Database Log'),
		'#default_value' => variable_get('nycc_rides_debug_to_log', '')
	); 	  

  return system_settings_form($form);
}
?>