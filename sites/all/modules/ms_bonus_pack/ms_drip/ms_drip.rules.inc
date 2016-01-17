<?php

// ======================================
// CONDITIONS
// ======================================

/*
* Conditions:
* + Check if user has an active set
*/

/**
* Implementation of hook_rules_condition_info().
* @ingroup rules
*/
function ms_drip_rules_condition_info() {
  return array(
    'ms_drip_condition_user_has_set' => array(
      'label' => t('Check if a user has an active Drip Set'),
      'arguments' => array(
        'set_id' => array('type' => 'drip_set', 'label' => t('Drip Set')),
        'account' => array('type' => 'user', 'label' => t('User'))
      ),
      'module' => 'MS Drip',
    ),
  );
}

/**
* Condition: Check if a user has the set
*/
function ms_drip_condition_user_has_set($set_id, $account) {
  if (ms_drip_user_has_set($set_id, $account->uid)) {
    return TRUE;
  }
  return FALSE;
}

// ======================================
// ACTIONS
// ======================================

/*
* Actions:
* + Start Set for User
* + End Set for User
*/

/**
* Implementation of hook_rules_action_info().
* @ingroup rules
*/
function ms_drip_rules_action_info() {
  return array(
    'ms_drip_action_start_drip_set' => array(
      'label' => t('Start a Drip Set for a User'),
      'arguments' => array(
        'set_id' => array('type' => 'drip_set', 'label' => t('Drip Set')),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'module' => 'MS Drip',
    ),
    'ms_drip_action_schedule_drip_set' => array(
      'label' => t('Schedule a Drip Set for a User'),
      'arguments' => array(
        'set_id' => array('type' => 'drip_set', 'label' => t('Drip Set')),
        'account' => array('type' => 'user', 'label' => t('User')),
        'start' => array('type' => 'date', 'label' => t('Start Date')),
      ),
      'module' => 'MS Drip',
    ),
    'ms_drip_action_end_drip_set' => array(
      'label' => t('End a Drip Set for a User'),
      'arguments' => array(
        'set_id' => array('type' => 'drip_set', 'label' => t('Drip Set')),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'module' => 'MS Drip',
    ),
  );
}

/**
* Action: Schedule a set for a User
*/
function ms_drip_action_schedule_drip_set($set_id, $account, $start) {
  // Schedule the set for this user
  ms_drip_start_set($set_id, $account->uid, strtotime($start));
}

/**
* Action: Start a set for a User
*/
function ms_drip_action_start_drip_set($set_id, $account) {
  // Start the set for this user
  ms_drip_start_set($set_id, $account->uid);
}

/**
* Action: Start a set for a User
*/
function ms_drip_action_end_drip_set($set_id, $account) {
  // End the set for this user
  ms_drip_end_set($set_id, $account->uid);
}

// ======================================
// TRIGGERS
// ======================================

/**
 * Implementation of hook_rules_event_info()
 */
function ms_drip_rules_event_info() {
  $events = array();

  $sets = ms_drip_get_sets();
  foreach ($sets as $set) {
    $items = ms_drip_get_set_items($set->id);

    foreach ($items as $item) {
      $events['ms_drip_event_'. $set->id .'_'. $item->id] = array(
        'label' => ms_drip_get_item_rule_name($set, $item),
        'module' => 'MS Drip Content',
        'help' => t("Triggers on day @day of a subscription.", array('@day' => $item->day)),
        'arguments' => array(
          'account' => array('type' => 'user', 'label' => t('User Account')),
        ),
      );
    }

  }

  return $events;
}

// ======================================
// DATA TYPES
// ======================================

/**
* Implementation of hook_rules_data_type_info().
*/
function ms_drip_rules_data_type_info() {
  return array(
    'drip_set' => array(
      'label' => t('Drip Set'),
      'class' => 'rules_data_type_drip_set',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => TRUE,
      'module' => 'MS Drip',
    ),
  );
}

/**
* Defines the rules drip_set data type.
*/
class rules_data_type_drip_set extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    return array(
      '#type' => 'select',
      '#title' => $info['label'],
      '#description' => t('Status'),
      '#options' => ms_drip_get_sets_list(),
      '#required' => TRUE,
      '#default_value' => $value,
    );
  }
}
