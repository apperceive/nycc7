<?php

function nycc_og_admin() {
  $form = array();
	$role_names = og_roles('node', 'Group');

  $form['nycc_og_caotain_role_id'] = array(
       '#type' => 'select',
       '#title' => t('Captain Role Name'),
       '#options' => $role_names,
       '#default_value' => variable_get('nycc_og_caotain_role_id', '6'),
       '#description' => t('Name of the role assigned to Captains'),
			 '#required' => TRUE
  );
  
  $form['nycc_og_leader_role_id'] = array(
       '#type' => 'select',
       '#title' => t('Leader Role Name'),
       '#options' => $role_names,
       '#default_value' => variable_get('nycc_og_leader_role_id', '4'),
       '#description' => t('Name of the role assigned to leaders'),
			 '#required' => TRUE
  );
    
  $form['nycc_og_rider_role_id'] = array(
       '#type' => 'select',
       '#title' => t('Confirmed Rider Role Name'),
       '#options' => $role_names,
       '#default_value' => variable_get('nycc_og_rider_role_id', '5'),
       '#description' => t('Name of the role assigned to confirmed riders'),
			 '#required' => TRUE
  );

  $form['nycc_og_waiter_role_id'] = array(
       '#type' => 'select',
       '#title' => t('Watlisted Rider Role Name'),
       '#options' => $role_names,
       '#default_value' => variable_get('nycc_og_waiter_role_id', '7'),
       '#description' => t('Name of the role assigned to waitlisted riders'),
			 '#required' => TRUE
  );  
  
  $form['nycc_og_group_domain_suffix'] = array(
    '#type' => 'textfield', 
    '#title' => t('Domain Suffix for Group Emails'),
    '#description' => t('Enter the domain suffix to be used for group emails (do not include the @ sign)'),
    '#default_value' => variable_get('nycc_og_group_domain_suffix', '')
  );    

  $form['nycc_og_signing_domain'] = array(
    '#type' => 'textfield', 
    '#title' => t('Signing Domain for Group Emails'),
    '#description' => t('Enter the domain to be used to sign group emails (do not include the @ sign)'),
    '#default_value' => variable_get('nycc_og_signing_domain', '')
  );    

  $form['nycc_og_inbound_url'] = array(
    '#type' => 'textfield', 
    '#title' => t('Url suffix configured in Mandrill Webhook'),
    '#description' => t("The webhook must be configured in <a href='https://mandrillapp.com/inbound' target='_blank'>Mandrill</a> before you configure this setting. (do not include the hostname or a leading slash)"),
    '#default_value' => variable_get('nycc_og_inbound_url', '')
  ); 

  $form['nycc_og_bcc_errors'] = array(
    '#type' => 'textfield', 
    '#title' => t('BCC error messages to this address'),
    '#description' => t('Leave blank if you only want the logged'),
    '#default_value' => variable_get('nycc_og_bcc_errors', '')
  );   
  $form['nycc_og_debug_to_log'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Write debug messages to Database Log'),
    '#default_value' => variable_get('nycc_og_debug_to_log', '')
  );    
  
  $key = variable_get('mandrill_api_key', '');
  
  if (empty($key)) {
    drupal_set_message(filter_xss("You must configure the <a href='admin/config/services/mandrill' target='_blank'>Mandrill module</a> with an API key  "), "error");
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
  $form['#submit'][] = 'nycc_og_admin_settings_submit';
  return system_settings_form($form);
}

function nycc_og_admin_settings_submit($form, &$form_state) {
  cache_clear_all(NULL, 'cache_menu'); 
}

function nycc_og_admin_validate($form, &$form_state) {
  //Confirm that signing domain is valid (if specified)
  if ( (!empty($form_state['values']['nycc_og_group_domain_suffix']))
        && (!checkdnsrr($form_state['values']['nycc_og_group_domain_suffix'], "MX"))) {

    form_set_error('nycc_og_group_domain_suffix',
      t('Error: Can not verify MX Records for specified domain. Please visit the !link and confirm the MX records are setup correctly.',
          array(
                '!link' => l(
                             'Mandrill Website',
                             'https://mandrillapp.com/inbound',
                             array('attributes' => array('target'=>'_blank')
                                   )
                             )
                )
          )
      );
    }
    
  //check to confirm inbound domain is configured in Mandrill
  if ( (!empty($form_state['values']['nycc_og_inbound_url']))
        && (! nycc_mandrill_url_is_valid($form_state['values']['nycc_og_inbound_url']))) {

    form_set_error('nycc_og_inbound_url',
      t('Error: Cannot verify Inbound URL specified. Please visit the !link and confirm the inbound domain settings.',
          array(
                '!link' => l(
                             'Mandrill Website', 'https://mandrillapp.com/inbound',
                             array('attributes' => array('target'=>'_blank')
                                   )
                             )
                )
          )
      );
    }

  //check to confirm inbound domain is specified 
  if ( empty($form_state['values']['nycc_og_inbound_url'])){

    form_set_error('nycc_og_inbound_url',
      t('Error: You must specifiy an inbound domain. Please visit the !link to configure the inbound domain settings.',
          array(
                '!link' => l(
                             'Mandrill Website',
                             'https://mandrillapp.com/inbound',
                             array('attributes' => array('target'=>'_blank')
                                   )
                             )
                )
          )
      );
    }    
    
    
  //confirm that signing domain is valid  
  if ( (!empty($form_state['values']['nycc_og_signing_domain']))
        && (!checkdnsrr($form_state['values']['nycc_og_signing_domain'], "MX"))) {

    form_set_error('nycc_og_signing_domain',
      t('Error: Can not verify MX Records for specified domain. Please visit the !link and confirm the MX records are setup correctly.',
          array(
                '!link' => l(
                             'Mandrill Website',
                             'https://mandrillapp.com/settings/sending-domains',
                             array('attributes' => array('target'=>'_blank')
                                   )
                             )
                )
          )
      );
    }

  //confirm that BCC email address is valid (if specified)
  if ( ( ! empty($form_state['values']['nycc_og_bcc_errors']))
        && ( ! valid_email_address($form_state['values']['nycc_og_bcc_errors']))) {

    form_set_error('nycc_og_bcc_errors',
      t('Error: Invalid email address specified'));
    }
}


