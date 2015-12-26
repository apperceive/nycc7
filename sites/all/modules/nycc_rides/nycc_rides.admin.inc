<?php
function nycc_rides_admin() {
  $form = array();
 
	$form['nycc_rides_debug_to_log'] = array(
		'#type' => 'checkbox', 
		'#title' => t('Write debug messages to Database Log'),
		'#default_value' => variable_get('nycc_rides_debug_to_log', '')
	);

	$form['nycc_rides_days_before_ride'] = array(
		'#type' => 'textfield', 
		'#title' => t('Enable Rides XX Days Before Ride Start'),
		'#description' => t('Enter the number of days prior to a Ride when signup for rides should be enabled'),
		'#default_value' => variable_get('nycc_rides_days_before_ride', '10')
	);

	$form['nycc_rides_approval_email_domain'] = array(
		'#type' => 'textfield', 
		'#title' => t('Domain to be used for email approvals'),
		'#description' => t('Do not include the @ symbol. Note that you must configure inbound email routing to support this in Mandrill'),
		'#default_value' => variable_get('nycc_rides_approval_email_domain', 'rides.nycc.org')
	);

	$form['nycc_rides_fields_trigger_approval'] = array(
		'#type' => 'textarea',
		'#title' => t('Fields that trigger re-approval'),
		'#description' => t('Comma separated list of fields that, if changed, will trigger re approval'),
		'#default_value' => variable_get('nycc_rides_fields_trigger_approval', 'title, field_ride_description, field_ride_spots, field_ride_select_level, field_ride_distance_in_miles, field_ride_speed, field_ride_timestamp')
	);
	$role_names = user_roles($membersonly = FALSE);
	
	
  $form['nycc_rides_rc_role_id'] = array(
       '#type' => 'select',
       '#title' => t('Ride Coordinator Role Name'),
       '#options' => $role_names,
       '#default_value' => variable_get('nycc_rides_rc_role_id', '5'),
       '#description' => t('Name of the role assigned to Ride Coordinators'),
			 '#required' => TRUE
  );
	
	if(variable_get('nycc_rides_rc_role_id', '') > 0) {

		$uids = array();
		$query = 'SELECT DISTINCT(ur.uid) FROM {users_roles} AS ur WHERE ur.rid IN (:rids)';
		$result = db_query($query, array(':rids' => variable_get('nycc_rides_rc_role_id', '5')));

		foreach ($result as $row) {
			$uids[] = $row->uid;
		}
		$users = user_load_multiple($uids);
		$users_names = array();
		foreach ($users as $user) {
			$users_names[$user->uid] = nycc_get_user_full_name($user);
		}
	
	
		$form['nycc_rides_a-ride-coordinators'] = array(
			'#type' => 'checkboxes',
			'#options' => $users_names,
			'#title' => t('A-Ride Coordinator'),
       '#description' => t('Select the name of the A-Rides coordinator. Only names with the ride coordinator role are displayed. <br/>If you are not not seeing the expected names then check the roles assigned to the expected users'),
			 '#required' => TRUE,
			 '#default_value' => variable_get('nycc_rides_a-ride-coordinators', ''),
			);
		
		$form['nycc_rides_b-ride-coordinators'] = array(
			'#type' => 'checkboxes',
			'#options' => $users_names,
			'#title' => t('B-Ride Coordinator'),
       '#description' => t('Select the name of the B-Rides coordinator. Only names with the ride coordinator role are displayed. <br/>If you are not not seeing the expected names then check the roles assigned to the expected users'),
			 '#required' => TRUE,
			 '#default_value' => variable_get('nycc_rides_b-ride-coordinators', ''),
			);

		$form['nycc_rides_c-ride-coordinators'] = array(
			'#type' => 'checkboxes',
			'#options' => $users_names,
			'#title' => t('C-Ride Coordinator'),
       '#description' => t('Select the name of the C-Rides coordinator. Only names with the ride coordinator role are displayed. <br/>If you are not not seeing the expected names then check the roles assigned to the expected users'),
			 '#required' => TRUE,
			 '#default_value' => variable_get('nycc_rides_c-ride-coordinators', ''),
			);				
	}
	
	$form['#submit'][] = 'nycc_rides_admin_submit';

  return system_settings_form($form);
}

function nycc_rides_admin_validate($form, &$form_state) {
	if (empty($form_state['values']['nycc_rides_days_before_ride'])) {
    	form_set_error('nycc_rides_days_before_ride', "Days before Ride can not be blank");
	} else {
      if (!is_numeric($form_state['values']['nycc_rides_days_before_ride'])) {
    	form_set_error('nycc_rides_days_before_ride', "Days before Ride must be numeric");      
		}	
	}
	if ( (!empty($form_state['values']['nycc_rides_approval_email_domain']))
				&& (!checkdnsrr($form_state['values']['nycc_rides_approval_email_domain'], "MX"))) {

		form_set_error('nycc_rides_approval_email_domain',
			t('Error: Can not verify MX Records for specified domain. Please visit the !link and confirm the MX records are setup correctly.',
					array('!link' => l('Mandrill Website', 'https://mandrillapp.com/inbound'))));
  }
	if (!empty($form_state['values']['nycc_rides_fields_trigger_approval'])) {	
		$ride_fields = field_info_instances("node", "rides");
		foreach(explode(",", $form_state['values']['nycc_rides_fields_trigger_approval']) as $field)
		{
			$field_trimmed = trim($field);
			if(drupal_strlen( $field_trimmed) &&  ($field_trimmed != 'title') && (! array_key_exists($field_trimmed, $ride_fields))){
				$valid_fields = implode(", ", array_keys($ride_fields));
				form_set_error('nycc_rides_fields_trigger_approval', t("Error: Field: $field_trimmed is not valid for content type Rides.<br/> Valid fields are: $valid_fields"));
			}
		} //foreach
  }//!emoty		
} //_validate

function nycc_rides_admin_submit($form, &$form_state) {

}

