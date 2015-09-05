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
	
  $form['nycc_rides_mandrill_api_key'] = array(
		'#title' => t('Mandrill API Key'),
		'#type' => 'textfield',
		'#description' => t('Create or grab your API key from the !link.',
	  		array('!link' => l('Mandrill settings', 'https://mandrillapp.com/settings/index'))),
		'#default_value' => variable_get('nycc_rides_mandrill_api_key', '')
  	);
  

  	$form['current_webhook'] = array(
		'#title' => t('Web Hooks'),
		'#type' => 'markup',  
		'#prefix' => '<div class="webhooks">',
		'#suffix' => '</div>', 
		'#value' => t('You should add following webhook(s) to routes in your Mandrill configuration:<br/> %url', 
					array('%url' => url('nycc/inbound', array('absolute' => TRUE)),)),
  	);
  	
	
	if(!empty ($key)) {
	  	$form['webhooks_header'] = array(
			'#title' => t('Web Hooks Header'),
			'#type' => 'markup',  
			'#prefix' => '<br/><div class="webhook_header"><h2>',
			'#suffix' => '</h2></div>', 
			'#value' => t('Webhook Status'),
		);	
		$rows = array();

		try {
			foreach (  nycc_rides_mandrill_get_api_object()->request('webhooks/list') as $list) {
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
		      '#type' => 'markup',
		      '#value' => theme_table($header, $rows)); 		
		
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
	if (empty($form_state['values']['nycc_email_mandrill_api_key'])) {
    	form_set_error('from', "Error: Mandrill API key can not be blank.");
	} else {
		try {
			$mandrill = nycc_rides_mandrill_get_api_object($form_state['values']['nycc_email_mandrill_api_key']);
		} catch(Mandrill_Exception $e) {
			form_set_error('from', "Error connecting to Mandrill: " . $e->getMessage());
		}	
	}	
	
}



