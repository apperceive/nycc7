<?php
function nycc_email_trap_admin() {
  $form = array();
 
  $form['nycc_email_trap_enabled'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Trap all Emails (excluding those sent to users with the specified role'),
    '#default_value' => variable_get('nycc_email_trap_enabled', '')
  );
  $form['nycc_email_trap_debug'] = array(
    '#type' => 'checkbox', 
    '#title' => t('write debug messages to log'),
    '#default_value' => variable_get('nycc_email_trap_debug', '')
  );  
  $form['nycc_email_trap_exclude_roles'] = array(
    '#type' => 'textfield', 
    '#title' => t('Role(s) to be excluded'),
    '#description' => t('Enter a list of roles (separated by commas) for which mail will not be trapped and delivered as usual'),
    '#default_value' => variable_get('nycc_email_trap_exclude_roles', '')
  );
  $form['nycc_email_trap_email'] = array(
    '#type' => 'textfield', 
    '#title' => t('email address where trapped messages should be sent'),
    '#description' => t('Email address where trapped messages should be sent'),
    '#default_value' => variable_get('nycc_email_trap_email', '')
  );   

  return system_settings_form($form);
}
function nycc_email_trap_admin_validate($form, &$form_state) {
  if ( (!empty($form_state['values']['nycc_email_trap_email']))
        && (!valid_email_address($form_state['values']['nycc_email_trap_email']))) {

    form_set_error('nycc_email_trap_email',
      t('Error: Invalid Email Address'));
    }
}


