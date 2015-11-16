<?php
function nycc_mandrill_admin() {
  $form = array();
 
  $form['nycc_mandrill_debug_to_log'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Write debug messages to Database Log'),
    '#default_value' => variable_get('nycc_mandrill_debug_to_log', '')
  );

  $key = variable_get('mandrill_api_key', '');  
  if (empty($key)) {
    drupal_set_message(filter_xss("You must configure the Mandrill module with an API key: <a href='/admin/config/services/mandrill' _target='_blank'>/admin/config/services/mandrill</a>"), "error");
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



