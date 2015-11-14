<?php

function nycc_og_email_admin(&$form_state) {
  $form = array();

  $form['nycc_og_email_group_domain_suffix'] = array(
    '#type' => 'textfield', 
    '#title' => t('Domain Suffix for Group Emails'),
    '#description' => t('Enter the domain suffix to be used for group emails (do not include the @ sign)'),
    '#default_value' => variable_get('nycc_og_email_group_domain_suffix', '')
  );    

  $form['nycc_og_email_signing_domain'] = array(
    '#type' => 'textfield', 
    '#title' => t('Signing Domain for Group Emails'),
    '#description' => t('Enter the domain to be used to sign group emails (do not include the @ sign)'),
    '#default_value' => variable_get('nycc_og_email_signing_domain', '')
  );    
  
  $form['nycc_og_email_debug_to_log'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Write debug messages to Database Log'),
    '#default_value' => variable_get('nycc_og_email_debug_to_log', '')
  );    
  
  //$form['#submit'][] = 'nycc_og_email_admin_settings_submit';
  return system_settings_form($form);
}

function nycc_og_email_admin_validate($form, &$form_state) {
  if ( (!empty($form_state['values']['nycc_og_email_group_domain_suffix']))
        && (!checkdnsrr($form_state['values']['nycc_og_email_group_domain_suffix'], "MX"))) {

    form_set_error('nycc_og_email_group_domain_suffix',
      t('Error: Can not verify MX Records for specified domain. Please visit the !link and confirm the MX records are setup correctly.',
          array('!link' => l('Mandrill Website', 'https://mandrillapp.com/inbound'))));
    }
    
  if ( (!empty($form_state['values']['nycc_og_email_signing_domain']))
        && (!checkdnsrr($form_state['values']['nycc_og_email_signing_domain'], "MX"))) {

    form_set_error('nycc_og_email_signing_domain',
      t('Error: Can not verify MX Records for specified domain. Please visit the !link and confirm the MX records are setup correctly.',
          array('!link' => l('Mandrill Website', 'https://mandrillapp.com/settings/sending-domains'))));
    }    
}


