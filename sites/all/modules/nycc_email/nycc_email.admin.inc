<?php

function nycc_email_admin_settings(&$form_state) {
	$key = variable_get('nycc_email_mandrill_api_key', '');
  	$form['nycc_email_mandrill_api_key'] = array(
		'#title' => t('Mandrill API Key'),
		'#type' => 'textfield',
		'#description' => t('Create or grab your API key from the !link.',
	  		array('!link' => l('Mandrill settings', 'https://mandrillapp.com/settings/index'))),
		'#default_value' => $key
  	);
  

//   	$form['current_webhook'] = array(
// 		'#title' => t('Web Hooks'),
// 		'#type' => 'markup',  
// 		'#prefix' => '<div class="webhooks">',
// 		'#suffix' => '</div>', 
// 		'#value' => t('You should add following webhook(s) to routes in your Mandrill configuration:<br/> %url', 
// 					array('%url' => url('nycc/inbound', array('absolute' => TRUE)),)),
//  	);
  	
 

	$form['nycc_email_group_domain_suffix'] = array(
		'#type' =>'textfield', 
		'#title' => t('Domain Suffix for Group Emails'),
		'#description' => t('Enter the domain suffix to be used for group emails (do not include the @ sign)'),
		'#default_value' => variable_get('nycc_email_group_domain_suffix', '')
	); 		

	$form['nycc_email_signing_domain'] = array(
		'#type' =>'textfield', 
		'#title' => t('Signing Domain for Group Emails'),
		'#description' => t('Enter the domain to be used to sign group emails (do not include the @ sign)'),
		'#default_value' => variable_get('nycc_email_signing_domain', '')
	); 		
	
	$form['nycc_email_debug_to_log'] = array(
		'#type' =>'checkbox', 
		'#title' => t('Write debug messages to Database Log'),
		'#default_value' => variable_get('nycc_email_debug_to_log', '')
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
			foreach (  nycc_email_mandrill_get_api_object()->request('webhooks/list') as $list) {
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
	$form['#submit'][] = 'nycc_email_admin_settings_submit';
	return system_settings_form($form);
}

function nycc_email_admin_settings_validate($form, $form_state) {
	if (empty($form_state['values']['nycc_email_mandrill_api_key'])) {
    	form_set_error('from', "nycc_email_mandrill_api_key: Mandrill API key can not be blank.");
	} else {
		try {
			$mandrill = nycc_email_mandrill_get_api_object($form_state['values']['nycc_email_mandrill_api_key']);
		} catch(Mandrill_Exception $e) {
			form_set_error('nycc_email_mandrill_api_key', "Error connecting to Mandrill: " . $e->getMessage());
		}	
	}
	if ( (!empty($form_state['values']['nycc_email_group_domain_suffix']))
				&& (!checkdnsrr($form_state['values']['nycc_email_group_domain_suffix'], "MX"))) {

		form_set_error('nycc_email_group_domain_suffix',
			t('Error: Can not verify MX Records for specified domain. Please visit the !link and confirm the MX records are setup correctly.',
					array('!link' => l('Mandrill Website', 'https://mandrillapp.com/inbound'))));
    }
    
	if ( (!empty($form_state['values']['nycc_email_signing_domain']))
				&& (!checkdnsrr($form_state['values']['nycc_email_signing_domain'], "MX"))) {

		form_set_error('nycc_email_signing_domain',
			t('Error: Can not verify MX Records for specified domain. Please visit the !link and confirm the MX records are setup correctly.',
					array('!link' => l('Mandrill Website', 'https://mandrillapp.com/settings/sending-domains'))));
    }    
}

function nycc_email_admin_settings_submit($form, &$form_state) {

}

