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
	
	$key = variable_get('mandrill_api_key', '');	
	if(empty($key)) {
		drupal_set_message(filter_xss("You must configure the Mandrill module with an API key: <a href='/admin/config/services/mandrill' _target='_blank'>/admin/config/services/mandrill</a>"), "error");
	} else {
  	$form['current_webhook'] = array(
		'#title' => t('Web Hooks'),
		'#type' => 'markup',  
		'#prefix' => '<div class="webhooks">',
		'#suffix' => '</div>', 
		'#value' => t('You should add following webhook(s) to routes in your Mandrill configuration:<br/> %url', 
					array('%url' => url('nycc_rides/inbound', array('absolute' => TRUE)),)),
  	);		
	  $form['webhooks_header'] = array(
			'#title' => t('Web Hooks Header'),
			'#type' => 'markup',  
			'#prefix' => '<br/><div class="webhook_header"><h2>',
			'#suffix' => '</h2></div>', 
			'#value' => t('Webhook Status'),
		);	
		$rows = array();

		try {
			$rows = array();
			foreach (mandrill_get_api_object()->webhooks->getList() as $list) {
				$rows[] = array(
					$list['id'],
					$list['url'],
					$list['auth_key'],
					isset($list['last_sent_at']) ? $list['last_sent_at'] : t('Never'),
					$list['batches_sent'],
					$list['events_sent'],
					$list['description'],
				);
			}
		
			$header = array(
				t('ID'),
				t('URL'),
				t('Auth Key'),
				t('Last Successful'),
				t('Batches Sent'),
				t('Events Sent'),
				t('Description'),
			);
		
			$form['webhooks'] = array(
				'#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
			);	
		
		} catch(Mandrill_Exception $e) {
			drupal_set_message("Error connecting to Mandrill: " . $e->getMessage());
		}
	}
		$form['#submit'][] = 'nycc_rides_admin_submit';

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



