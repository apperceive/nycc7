<?php
function nycc_mandrill_admin() {
  $form = array();
 
  $form['nycc_mandrill_debug_to_log'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Write debug messages to Database Log'),
    '#default_value' => variable_get('nycc_mandrill_debug_to_log', '')
  );
	$key = variable_get('nycc_mandrill_api_key', '');
  	$form['nycc_mandrill_api_key'] = array(
		'#title' => t('Mandrill API Key'),
		'#type' => 'textfield',
		'#description' => t('Create or grab your API key from the !link.',
	  		array('!link' => l('Mandrill settings', 'https://mandrillapp.com/settings/index'))),
		'#default_value' => $key
  	);
  $key = variable_get('nycc_mandrill_api_key', '');
  
  if (empty($key)) {
    drupal_set_message(filter_xss("You must configure the Mandrill module with an API key"), "error");
  }
  else {
    $form['webhooks_header'] = array(
      '#title' => t('Web Hooks Header'),
      '#type' => 'markup',  
      '#prefix' => '<br/><div class="webhook_header"><h2>',
      '#suffix' => '</h2></div>', 
      '#value' => t('Webhook Status'),
    );  
    $rows = nycc_mandrill_get_webhooks();;
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
  }
    //$form['#submit'][] = 'nycc_mandrill_admin_submit';

  return system_settings_form($form);
}

function nycc_mandrill_admin_settings_validate($form, &$form_state) {
	if (empty($form_state['values']['nycc_mandrill_api_key'])) {
    	form_set_error('from', "nycc_mandrill_api_key: Mandrill API key can not be blank.");
	} else {
		try {
			$mandrill = nycc_mandrill_get_api_object($form_state['values']['nycc_email_mandrill_api_key']);
		} catch(Mandrill_Exception $e) {
			form_set_error('nycc_mandrill_api_key', "Error connecting to Mandrill: " . print_r ($e->getMessage()));
		}	
	} 
}

