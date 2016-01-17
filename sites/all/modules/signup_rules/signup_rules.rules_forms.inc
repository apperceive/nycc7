<?php

/**
 * @file
 * Form callbacks for Signup Rules module
 */

/**
 * Form for signup total condition.
 *
 * @param $settings
 *   An array of settings for edited forms.
 * @param &$form
 *   The existing form array. This argument is passed by reference so
 *   we just alter it and that's all.
 */
function signup_rules_condition_signup_total_form($settings, &$form) {
  $form['settings']['signup_rules_condition'] = array(
    '#type' => 'select',
    '#title' => t('Signup total is'),
    '#options' => array(
      'greater' => t('greater than'),
      'equal' => t('equal to'),
      'less' => t('less than'),
    ),
    '#default_value' => $settings['signup_rules_condition'],
    '#required' => TRUE,
  );
  $form['settings']['signup_rules_total'] = array(
    '#type' => 'textfield',
    '#title' => t('value'),
    '#default_value' => $settings['signup_rules_total'],
    '#required' => TRUE,
  );
}

/**
 * Validation callback for signup total condition form.
 */
function signup_rules_condition_signup_total_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['settings']['signup_rules_total'])) {
    form_set_error('signup_rules_total', t('You must enter a numeric value.'));
  }
}

/**
 * Action "Sign up user to content" configuration form.
 */
function signup_rules_action_signup_user_form($settings = array(), &$form) {
  $form['settings']['ignore'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore content signup status'),
    '#default_value' => $settings['ignore'],
    '#description' => t('Sign up the user regardless of whether signups have been closed for the content.'),
  );
}

/**
 * Action "Send mail to all users signed up for content" configuration form.
 *
 * @see signup_broadcast_form()
 */
function signup_rules_action_mail_to_signup_users_form($settings = array(), &$form) {
  $settings += array('subject' => '', 'message' => '');
  $form['settings']['limit'] = array(
    '#type' => 'select',
    '#title' => t('Send mail to'),
    '#options' => array(
      'all' => t('All signed up users'),
      'attended' => t('Users who attended'),
      'not attended' => t('Users who did not attend'),
    ),
    '#default_value' => $settings['limit'],
    '#required' => TRUE,
    '#description' => t('Limit the mail to specific users signed up for this content.'),
  );
  $form['settings']['from'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#default_value' => $settings['from'],
    '#description' => 'The mail\'s from address. Leave it empty to use the site-wide configured address.',
  );
  $form['settings']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings['subject'],
    '#required' => TRUE,
    '#description' => t("The mail's subject."),
  );
  
  $tokens = array('%node_title', '%node_url', '%user_mail', '%user_name', t('%cancel_signup_url (access to this link is denied to users without the "cancel own signups" permission)'));

  // We don't have the node object so can't check _signup_get_node_scheduler().
  // Load these anyway to cover 99% of cases that use a signup scheduler.
  $tokens = array_merge(array('%node_start_time'), $tokens);

  if (module_exists('token')) {
    $token_text = t('Supported string substitutions: %tokens, and any tokens in the %replacement_tokens list.', array('%tokens' => implode(', ', $tokens), '%replacement_tokens' => t('Replacement tokens')));
  }
  else {
    $token_text = t('Supported string substitutions: %tokens.', array('%tokens' => implode(', ', $tokens)));
  }
  
  $form['settings']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $settings['message'],
    '#required' => TRUE,
    '#description' => t('Body of the email message you wish to send to all users who have signed up for this content. !token_description', array('!token_description' => $token_text)),
  );
}

/**
 * validation callback for send mail action.
 */
function signup_rules_action_mail_to_signup_users_validate($form, $form_state) {
  $from = $form_state['values']['settings']['from'];
  if (!empty($from) && !valid_email_address($from)) {
    form_set_error('from', t('You must enter a valid e-mail address.'));
  }
}
