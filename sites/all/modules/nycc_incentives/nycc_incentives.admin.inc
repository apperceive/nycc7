<?php

// Configuration Page (start/end windows, pick up sheets, bill of materials)

function _nycc_incentives_admin_settings_form() {
  // TODO: check that user has incentive admin perms - do it in menu def?

  
  $form['nycc_incentives_instructions'] = array(
    '#type' => 'fieldset',
    //'#title' => t('Instructions'),
  );

  $helpitems = array(
   t("These settings apply to the @year incentives.", array('@year' => date("Y")-1)),
   t("Set the Start/End dates to control incentives block visibility and when incentive requests can be submitted."),
   t("Points are used for calculating a weighted sum of rides lead by each user."),
   t("Use the tabs at the top of this page to view reports."),
   t("Permission settings and links to related pages are displayed at the bottom of this form."),
  );
  $form['nycc_incentives_instructions']['items'] = array(
    '#value' => theme_item_list(array(
																			'items' => $helpitems,
																			'title' => t("Instructions"),
																			'type' => 'ul',
																			'attributes' => array(),
																		 )
																),
    '#prefix' => "<p>",
    '#suffix' => "</p>",
  );
  
  // TODO: how add calendar pop-up for selecting dates?
  
  $form['nycc_incentives_dates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Request Submission Dates'),
  );
  
  $form['nycc_incentives_dates']['nycc_incentives_start_date'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#title' => t('Start date'),
    '#default_value' => variable_get('nycc_incentives_start_date', ""),
    '#description' => t('Starting date for submitting incentive requests based on the previous year (@d). Format is mm/dd/yyyy.', array('@d' => date('Y')-1)),
  );

  $form['nycc_incentives_dates']['nycc_incentives_end_date'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#title' => t('End date'),
    '#default_value' => variable_get('nycc_incentives_end_date', ""),
    '#description' => t('Closing date for submitting incentive requests based on the previous year (@d). Format is mm/dd/yyyy.', array('@d' => date('Y')-1)),
  );
 
  $form['nycc_incentives_points'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ride Lead Points'),
  );
  
  $form['nycc_incentives_points']['nycc_incentives_ride_lead_cuesheet'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#title' => t("Points for cue-sheet rides"),
    '#default_value' => variable_get("nycc_incentives_ride_lead_cuesheet", "1"),
    '#size' => 10,
  );

  $form['nycc_incentives_points']['nycc_incentives_ride_lead_other'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#title' => t("Points for other rides (Training)"),
    '#default_value' => variable_get("nycc_incentives_ride_lead_other" ,".5"),
    '#size' => 10,
    '#description' => "Note that changes to point values will not effect existing Incentives Requests unless they are re-saved or have their status changed.",
  );

  $form['nycc_incentives_links'] = array(
    '#type' => 'fieldset',
    //'#title' => t('Related Links'),
  );
  
  $items = array(
    l('Incentives Page', 'incentives'),
    l('Create Incentive Request', 'node/add/incentive-request'),
    l('Review Incentives View', 'incentives/review'),
    l('Incentives Pick-up List View', 'incentives/pickup'),
    l('Incentives BOM List View', 'incentives/BOM'),
    l('Edit Incentive Views', 'admin/build/views/edit/incentives_pickup'),
    l('Edit Incentive Request Content Type', 'admin/content/node-type/incentive-request/fields'),  
  );
  
	
  $form['nycc_incentives_links']['items'] = array(
    '#markup' => theme_item_list(array(
																			'items' => $items,
																			'title' => t("Related Links"),
																			'type' => 'ul',
																			'attributes' => array(),
																		 )
																),
  );

  $form['nycc_incentives_perms'] = array(
    '#type' => 'fieldset',
    //'#collapsible' => TRUE, 
    //'#collapsed' => TRUE,     
    //'#title' => t('Incentive Permissions and Roles'),
  );
  
  $perms = nycc_incentives_permission();
	
  $roles = array();
  foreach (array_keys($perms) as $perm) {
    $roles[$perm] = $perm . ": " . implode(', ', user_roles(FALSE, $perm));
  } // $perm
	
  if (!count($roles)) {
    drupal_set_message("WARNING: no roles have access to the features of the NYCC Incentives module. Please grant them <a href='/admin/user/permissions'>here</a>", 'warning');
    $roles[] = "WARNING: no roles have access to the features of the NYCC Incentives module. Please grant them <a href='/admin/user/permissions'>here</a>";
  }
  
  $form['nycc_incentives_perms']['items'] = array(
    '#markup' => theme_item_list(array(
																			'items' => $roles,
																			'title' => t("NYCC Incentive Module Permissions - Roles"),
																			'type' => 'ul',
																			'attributes' => array(),
																		 )
																),
  );

 $form['nycc_incentives_perms2'] = array(
    '#type' => 'fieldset',
    //'#collapsible' => TRUE, 
    //'#collapsed' => TRUE,     
    //'#title' => t('Incentive Permissions and Roles'),
  );  
  
  $form['nycc_incentives_perms2']['items'] = array(
    '#markup' => theme_item_list(array(
																			'items' => _nycc_incentives_check_role_perms(),
																			'title' => t("Roles - All Permissions containing 'incentive'"),
																			'type' => 'ul',
																			'attributes' => array(),
																		 )
																),
  );

  //dpm(get_defined_vars());
    
  return system_settings_form($form);
} // _nycc_incentives_admin_settings_form

// form validate and submit handlers for admin settings form
function _nycc_incentives_admin_settings_form_validate($form, &$form_state) {
  // check for valid dates 
  $s = strtotime($form_state['values']['nycc_incentives_start_date']);
  $e = strtotime($form_state['values']['nycc_incentives_end_date']);
  if (!$s)
    form_set_error('nycc_incentives_start_date', t('Invalid start date.'));
  if (!$e)
    form_set_error('nycc_incentives_end_date', t('Invalid end date.'));
  if ($s && $e) {
    if ($s >= $e) {
      form_set_error('nycc_incentives_start_date', t('Start date must be before ending date.'));
    } // if out of order
  } // if valid
  
  // todo: consider: check that dates are for this year?
  
} // _nycc_incentives_admin_settings_form_validate