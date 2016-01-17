<?php

// ======================================
// CONDITIONS
// ======================================

/*
* Conditions:
* + Check if user has a purchase
* - Check the status of a purchase
*/

/**
* Implementation of hook_rules_condition_info().
* @ingroup rules
*/
function ms_products_rules_condition_info() {
  return array(
    'ms_products_condition_user_has_purchase' => array(
      'label' => t('Check if user has purchase'),
      'arguments' => array(
        'pid' => array('type' => 'pid', 'label' => t('Product Plan')),
        'account' => array('type' => 'user', 'label' => t('User'))
      ),
      'module' => 'MS Products',
    ),
    'ms_products_condition_product_plan_is' => array(
      'label' => t('Product Plan is equal to'),
      'arguments' => array(
        'pid' => array('type' => 'pid', 'label' => t('Product Plan')),
        'product_plan' => array('type' => 'ms_products_plan', 'label' => t('Product Plan')),
      ),
      'module' => 'MS Products',
    ),
  );
}

/**
* Condition: Check if a user has the purchase
*/
function ms_products_condition_user_has_purchase($pid, $account) {
  if (ms_products_user_has_purchase($account->uid, $pid)) {
    return TRUE;
  }
  return FALSE;
}

/**
* Condition: Check what the product plan is
*/
function ms_products_condition_product_plan_is($pid, $product_plan) {
  if ($product_plan->pid == $pid) {
    return TRUE;
  }
  return FALSE;
}

// ======================================
// ACTIONS
// ======================================

/*
* Actions:
* + Add Purchase to User
* + Set Expiration Date for User Purchase
* + Change Purchase Status (Active, Cancelled, EOT, etc)
* - Load Purchase from User
* - Load Product Plan
*/

/**
* Implementation of hook_rules_action_info().
* @ingroup rules
*/
function ms_products_rules_action_info() {
  return array(
    'ms_products_action_add_purchase' => array(
      'label' => t('Add a Purchase to a User'),
      'arguments' => array(
        'pid' => array('type' => 'pid', 'label' => t('Product Plan')),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'new variables' => array(
        'purchase' => array(
          'type' => 'ms_purchase',
          'label' => t('New purchase'),
        ),
      ),
      'module' => 'MS Products',
    ),
    'ms_products_action_add_purchase_by_id' => array(
      'label' => t('Add a Purchase to a User by Plan ID'),
      'arguments' => array(
        'pid' => array('type' => 'number', 'label' => t('Product Plan ID')),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'new variables' => array(
        'purchase' => array(
          'type' => 'ms_purchase',
          'label' => t('New purchase'),
        ),
      ),
      'module' => 'MS Products',
    ),
    'ms_products_action_change_purchase_expiration' => array(
      'label' => t('Change the Expiration Date of a Purchase'),
      'arguments' => array(
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'expiration' => array('type' => 'date', 'label' => t('Date')),
      ),
      'module' => 'MS Products',
    ),
    'ms_products_action_change_purchase_status' => array(
      'label' => t('Change the Status of a Purchase'),
      'arguments' => array(
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'status' => array('type' => 'purchase_status', 'label' => t('Status')),
      ),
      'module' => 'MS Products',
    ),
  );
}

/**
* Action: Change the status of a purchase
*/
function ms_products_action_change_purchase_status($purchase, $status) {
  // Change the Purchase Status
  return ms_products_change_purchase_status($purchase->mid, $status);
}

/**
* Action: Add a purchase to a user
*/
function ms_products_action_add_purchase($pid, $account) {
  $product_plan = ms_products_load_product_plan($pid);

  // Add a new purchase for this user
  $purchase = ms_products_insert_new_purchase($product_plan, $account);
  return array('purchase' => $purchase);
}

/**
* Action: Add a purchase to a user
*/
function ms_products_action_add_purchase_by_id($pid, $account) {
  $product_plan = ms_products_load_product_plan($pid);
  // Add a new purchase for this user
  $purchase = ms_products_insert_new_purchase($product_plan, $account);
  // Add and remove the roles at the start of the purchase
  ms_products_change_roles($account->uid, $product_plan->start_grant_roles, $product_plan->start_remove_roles);
  return array('purchase' => $purchase);
}

/**
* Action: Change the expiration date of a purchase
*/
function ms_products_action_change_purchase_expiration($purchase, $expiration) {
  return ms_products_change_purchase_expiration($purchase->mid, strtotime($expiration));
}

// ======================================
// TRIGGERS
// ======================================

/*
* Triggers:
* + Purchase is added to a user
* + Purchase is marked as expiring soon
* + Purchase is expiring
* + Purchase is cancelled
* + Purchase has a payment
* + Purchase is renewed
* + Purchase is modified
*/

/**
 * Implementation of hook_rules_event_info()
 */
function ms_products_rules_event_info() {
  $items = array(
    'ms_products_event_purchase_cancel' => array(
      'label' => t('Purchase Cancelled'),
      'module' => 'MS Products',
      'help' => t("Triggers when a purchase is cancelled."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Purchase Account')),
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'product_plan' => array('type' => 'ms_products_plan', 'label' => t('Product Plan')),
      ),
    ),
    'ms_products_event_purchase_signup' => array(
      'label' => t('Purchase Signup'),
      'module' => 'MS Products',
      'help' => t("Triggers when a user signs up for a new purchase."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Purchase Account')),
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'product_plan' => array('type' => 'ms_products_plan', 'label' => t('Product Plan')),
      ),
    ),
    'ms_products_event_purchase_modification' => array(
      'label' => t('Purchase Modification'),
      'module' => 'MS Products',
      'help' => t("Triggers when a user modifies their purchase."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Purchase Account')),
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'new_plan' => array('type' => 'ms_products_plan', 'label' => t('New Product Plan')),
        'old_plan' => array('type' => 'ms_products_plan', 'label' => t('Old Product Plan')),
      ),
    ),
    'ms_products_event_purchase_renewal' => array(
      'label' => t('Purchase Renewal'),
      'module' => 'MS Products',
      'help' => t("Triggers when a user renews their purchase."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Purchase Account')),
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'product_plan' => array('type' => 'ms_products_plan', 'label' => t('Product Plan')),
      ),
    ),
    'ms_products_event_purchase_payment' => array(
      'label' => t('Purchase Payment'),
      'module' => 'MS Products',
      'help' => t("Triggers when a payment is received for a purchase."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Purchase Account')),
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'product_plan' => array('type' => 'ms_products_plan', 'label' => t('Product Plan')),
      ),
    ),
    'ms_products_event_purchase_expiring_soon' => array(
      'label' => t('Purchase Expiring Soon'),
      'module' => 'MS Products',
      'help' => t("Triggers when a purchase is marked as expiring soon."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Purchase Account')),
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'product_plan' => array('type' => 'ms_products_plan', 'label' => t('Product Plan')),
      ),
    ),
    'ms_products_event_purchase_expiring' => array(
      'label' => t('Purchase Expiring'),
      'module' => 'MS Products',
      'help' => t("Triggers when a purchase is expiring."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Purchase Account')),
        'purchase' => array('type' => 'ms_purchase', 'label' => t('Purchase')),
        'product_plan' => array('type' => 'ms_products_plan', 'label' => t('Product Plan')),
      ),
    ),
  );

  return $items;
}


// ======================================
// DATA TYPES
// ======================================

/**
* Implementation of hook_rules_data_type_info().
*/
function ms_products_rules_data_type_info() {
  return array(
    'pid' => array(
      'label' => t('Product Plan'),
      'class' => 'rules_data_type_ms_products_pid',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => TRUE,
      'module' => 'MS Products',
    ),
    'purchase_status' => array(
      'label' => t('Purchase Status'),
      'class' => 'rules_data_type_ms_purchase_status',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => TRUE,
      'module' => 'MS Products',
    ),
    'ms_products_plan' => array(
      'label' => t('Product Plan'),
      'class' => 'rules_data_type_ms_products_plan',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'uses_input_form' => FALSE,
      'module' => 'MS Products',
    ),
    'ms_purchase' => array(
      'label' => t('Purchase'),
      'class' => 'rules_data_type_ms_purchase',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'uses_input_form' => FALSE,
      'module' => 'MS Products',
    ),
  );
}

/**
* Defines the rules purchase data type.
*/
class rules_data_type_ms_purchase extends rules_data_type {
  function load($mid) {
    return ms_products_load_purchase($mid);
  }

  function get_identifier() {
    $purchase = &$this->get();
    return $purchase->mid;
  }
}

/**
* Defines the rules ms_products_plan data type.
*/
class rules_data_type_ms_products_plan extends rules_data_type {
  function load($pid) {
    return ms_products_load_product_plan($pid);
  }

  function get_identifier() {
    $product_plan = $this->get();
    return $product_plan->pid;
  }
}

/**
* Defines the rules pid data type.
*/
class rules_data_type_ms_products_pid extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    return array(
      '#type' => 'select',
      '#title' => $info['label'],
      '#description' => t('Select a Product Plan.'),
      '#options' => ms_products_get_plans(),
      '#required' => TRUE,
      '#default_value' => $value,
    );
  }
}

/**
* Defines the rules purchase_status data type.
*/
class rules_data_type_ms_purchase_status extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    return array(
      '#type' => 'select',
      '#title' => $info['label'],
      '#description' => t('Status'),
      '#options' => ms_products_get_purchase_statuses(),
      '#required' => TRUE,
      '#default_value' => $value,
    );
  }
}
