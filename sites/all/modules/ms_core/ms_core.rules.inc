<?php

// ======================================
// CONDITIONS
// ======================================

/*
 * Conditions:
 * + Check the Status of an order
 * + Check the payment type
 * + Check the payment amount greater than
 * + Check the product
 * + Check the gateway chosen
 * +Check the payment number
 */

/**
* Implementation of hook_rules_condition_info().
* @ingroup rules
*/
function ms_core_rules_condition_info() {
  return array(
    'ms_core_condition_order_status' => array(
      'label' => t('Check the status of the order'),
      'arguments' => array(
        'order' => array('type' => 'ms_order', 'label' => t('Order')),
        'status' => array('type' => 'ms_order_status', 'label' => t('Status'))
      ),
      'module' => 'MS Core',
    ),
    'ms_core_condition_payment_type' => array(
      'label' => t('Check the type of payment'),
      'arguments' => array(
        'payment' => array('type' => 'ms_payment', 'label' => t('Payment')),
        'type' => array('type' => 'ms_payment_type', 'label' => t('Payment Type'))
      ),
      'module' => 'MS Core',
    ),
    'ms_core_condition_gateway' => array(
      'label' => t('Check the chosen Payment Gateway'),
      'arguments' => array(
        'gateway' => array('type' => 'ms_gateway', 'label' => t('Gateway'))
      ),
      'module' => 'MS Core',
    ),
    'ms_core_condition_payment_number' => array(
      'label' => t('Check the payment number'),
      'arguments' => array(
        'order' => array('type' => 'ms_order', 'label' => t('Order')),
        'payment' => array('type' => 'ms_payment', 'label' => t('Payment')),
      ),
      'module' => 'MS Core',
    ),
    'ms_core_condition_payment_amount_greater_than' => array(
      'label' => t('Check if payment amount is greater than X'),
      'arguments' => array(
        'payment' => array('type' => 'ms_payment', 'label' => t('Payment')),
        'amount' => array('type' => 'number', 'label' => t('Amount'))
      ),
      'module' => 'MS Core',
    ),
    'ms_core_condition_product_id' => array(
      'label' => t('Check the product'),
      'arguments' => array(
        'product' => array('type' => 'ms_product', 'label' => t('Product')),
        'id' => array('type' => 'ms_product_id', 'label' => t('Product'))
      ),
      'module' => 'MS Core',
    ),
  );
}

/**
 * Condition: Check the gateway
 */
function ms_core_condition_gateway($gateway) {
  return $_SESSION['ms_gateway'] == $gateway;
}

/**
 * Condition: Check the payment type
 */
function ms_core_condition_payment_type($payment, $type) {
  return $payment->type == $type;
}

/**
 * Condition: Check the payment amount is greater than X
 */
function ms_core_condition_payment_amount_greater_than($payment, $amount) {
  return $payment->amount > $amount;
}

/**
 * Condition: Check the payment number
 */
function ms_core_condition_payment_number($order, $payment) {
  return array_search($payment, $order);
}

/**
 * Condition: Check the product id
 */
function ms_core_condition_product_id($product, $id) {
  $product_id = $product->module .'-'. $product->id;
  return $product_id == $id;
}

/**
 * Condition: Check the value of a field.
 */
function ms_core_condition_order_status($order, $status) {
  if ($order->status == $status) {
    return TRUE;
  }
  return FALSE;
}

// ======================================
// ACTIONS
// ======================================

/*
 * Actions:
 * + Change Order Status (Active, Cancelled, EOT, etc)
 * + Add Adjustment to Cart
 * + Remove Adjustment from Cart
 */

/**
 * Implementation of hook_rules_action_info().
 * @ingroup rules
 */
function ms_core_rules_action_info() {
  return array(
    'ms_core_action_change_order_status' => array(
      'label' => t('Change the Status of an Order'),
      'arguments' => array(
        'order' => array('type' => 'ms_order', 'label' => t('Order')),
        'status' => array('type' => 'ms_order_status', 'label' => t('Status')),
      ),
      'module' => 'MS Core',
    ),
    'ms_core_action_add_cart_adjustment' => array(
      'label' => t('Add an Adjustment to a Cart'),
      'arguments' => array(
        'adjustment' => array('type' => 'ms_cart_adjustment', 'label' => t('Adjustment')),
      ),
      'module' => 'MS Core',
    ),
    'ms_core_action_remove_cart_adjustment' => array(
      'label' => t('Remove an adjustment from a Cart'),
      'arguments' => array(
        'adjustment' => array('type' => 'ms_cart_adjustment', 'label' => t('Adjustment')),
      ),
      'module' => 'MS Core',
    ),
  );
}

/**
 * Action: Add an Adjustment to a Cart
 */
function ms_core_action_add_cart_adjustment($values) {
  $adjustment = ms_core_rules_create_adjustment($values['adjustment']);
  ms_core_add_cart_adjustment($adjustment);
}

/**
 * Action: Remove an Adjustment from a Cart
 */
function ms_core_action_remove_cart_adjustment($values) {
  $adjustment = ms_core_rules_create_adjustment($values['adjustment']);
  return ms_core_remove_cart_adjustment($adjustment);
}

/**
 * Action: Change the status of an Order
 */
function ms_core_action_change_order_status($order, $status) {
  // Change the Order Status
  return ms_core_set_order_status($order, $status);
}

// ======================================
// TRIGGERS
// ======================================

/*
 * Triggers:
 * + Order status is changed
 * + Payment is received for an order
 * + Product added to cart
 * + Cart moves to checkout
 * + Gateway chosen for cart
 */

/**
 * Implementation of hook_rules_event_info()
 */
function ms_core_rules_event_info() {
  $items = array(
    'ms_core_event_order_status_changed' => array(
      'label' => t('Order Status Changed'),
      'module' => 'MS Core',
      'help' => t("Act when an order is marked as completed."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('User Account')),
        'order' => array('type' => 'ms_order', 'label' => t('Order')),
        'old_status' => array('type' => 'ms_order_status', 'label' => t('Old Order Status')),
        'new_status' => array('type' => 'ms_order_status', 'label' => t('New Order Status')),
      ),
    ),
    'ms_core_event_order_payment_received' => array(
      'label' => t('Order Payment Received'),
      'module' => 'MS Core',
      'help' => t("Act when a payment is received for an order"),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('User Account')),
        'order' => array('type' => 'ms_order', 'label' => t('Order')),
        'payment' => array('type' => 'ms_payment', 'label' => t('Payment')),
      ),
    ),
    'ms_core_event_cart_checkout' => array(
      'label' => t('User begins Checkout'),
      'module' => 'MS Core',
      'help' => t("Gets triggered when the user moves to the Checkout Page"),
      'arguments' => array(
        'cart' => array('type' => 'ms_cart', 'label' => t('Cart')),
      ),
    ),
    'ms_core_event_product_add_to_cart' => array(
      'label' => t('Product Added to Cart'),
      'module' => 'MS Core',
      'help' => t("Triggered when a Product is added to a cart"),
      'arguments' => array(
        'product' => array('type' => 'ms_product', 'label' => t('Product')),
        'cart' => array('type' => 'ms_cart', 'label' => t('Cart')),
      ),
    ),
    'ms_core_event_gateway_checkout' => array(
      'label' => t('Payment Gateway Chosen'),
      'module' => 'MS Core',
      'help' => t("Triggered when a Payment Gateway is Chosen for Checkout"),
      'arguments' => array(
      ),
    ),
  );

  return $items;
}

/**
 * Creates an adjustment object from the parameters specified
 */
function ms_core_rules_create_adjustment($values) {
  $adjustment = new MsAdjustment();
  $adjustment->id = 'ms_core_rules_adjustment';
  $adjustment->display = $values['name'];
  $adjustment->type = $values['type'];
  $adjustment->value = $values['value'];
  $adjustment->weight = isset($values['weight']) ? $values['weight'] : 0;
  $adjustment->scope = $values['scope'];
  return $adjustment;
}

// ======================================
// DATA TYPES
// ======================================

/**
 * Implementation of hook_rules_data_type_info().
 */
function ms_core_rules_data_type_info() {
  return array(
    'ms_order_status' => array(
      'label' => t('Order Status'),
      'class' => 'rules_data_type_ms_order_status',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => TRUE,
      'module' => 'MS Core',
    ),
    'ms_product_id' => array(
      'label' => t('Product'),
      'class' => 'rules_data_type_ms_product_id',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => TRUE,
      'module' => 'MS Core',
    ),
    'ms_payment_type' => array(
      'label' => t('Payment Type'),
      'class' => 'rules_data_type_ms_payment_type',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => TRUE,
      'module' => 'MS Core',
    ),
    'ms_gateway' => array(
      'label' => t('Gateway'),
      'class' => 'rules_data_type_ms_gateway',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => TRUE,
      'module' => 'MS Core',
      'hidden' => TRUE,
    ),
    'ms_cart_adjustment' => array(
      'label' => t('Cart Adjustment'),
      'class' => 'rules_data_type_ms_adjustment',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => TRUE,
      'module' => 'MS Core',
    ),
    'ms_order' => array(
      'label' => t('Order'),
      'class' => 'rules_data_type_ms_order',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'uses_input_form' => FALSE,
      'module' => 'MS Core',
    ),
    'ms_cart' => array(
      'label' => t('Cart'),
      'class' => 'rules_data_type_ms_cart',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => FALSE,
      'module' => 'MS Core',
    ),
    'ms_product' => array(
      'label' => t('Product'),
      'class' => 'rules_data_type_ms_product',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'uses_input_form' => FALSE,
      'module' => 'MS Core',
    ),
    'ms_payment' => array(
      'label' => t('Payment'),
      'class' => 'rules_data_type_ms_payment',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'uses_input_form' => FALSE,
      'module' => 'MS Core',
    ),
  );
}

/**
 * Defines the rules ms_payment data type.
 */
class rules_data_type_ms_payment extends rules_data_type {
  function load($pid) {
    return ms_core_load_payment($pid);
  }

  function get_identifier() {
    $payment = &$this->get();
    return $payment->pid;
  }
}

/**
 * Defines the rules ms_product data type.
 */
class rules_data_type_ms_product extends rules_data_type {
  function load($id) {
    return ms_core_load_payment($id);
  }

  function get_identifier() {
    $product = &$this->get();
    return $product->pid;
  }
}

/**
 * Defines the rules ms_order data type.
 */
class rules_data_type_ms_order extends rules_data_type {
  function load($oid) {
    return ms_core_order_load($oid);
  }

  function get_identifier() {
    $order = &$this->get();
    return $order->oid;
  }
}

/**
 * Defines the rules ms_cart data type.
 */
class rules_data_type_ms_cart extends rules_data_type {
  function load() {
    return ms_core_get_cart();
  }
}

/**
 * Defines the rules order_status data type.
 */
class rules_data_type_ms_order_status extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    return array(
      '#type' => 'select',
      '#title' => $info['label'],
      '#description' => t('Status'),
      '#options' => ms_core_get_order_statuses(),
      '#required' => TRUE,
      '#default_value' => $value,
    );
  }
}

/**
 * Defines the rules ms_product_id data type.
 */
class rules_data_type_ms_product_id extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    return array(
      '#type' => 'select',
      '#title' => $info['label'],
      '#description' => t('Product'),
      '#options' => ms_core_get_module_products_list(),
      '#required' => TRUE,
      '#default_value' => $value,
    );
  }
}

/**
 * Defines the rules ms_payment_type data type.
 */
class rules_data_type_ms_payment_type extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    return array(
      '#type' => 'select',
      '#title' => $info['label'],
      '#description' => t('Payment Type'),
      '#options' => ms_core_get_payment_types(),
      '#required' => TRUE,
      '#default_value' => $value,
    );
  }
}

/**
 * Defines the rules ms_gateway data type.
 */
class rules_data_type_ms_gateway extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    return array(
      '#type' => 'select',
      '#title' => $info['label'],
      '#description' => t('Gateway'),
      '#options' => ms_core_get_payment_gateways_list(),
      '#required' => TRUE,
      '#default_value' => $value,
    );
  }
}

/**
 * Defines the rules ms_adjustment data type.
 */
class rules_data_type_ms_adjustment extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    $form = array();
    $form['adjustment'] = array(
      '#type' => 'fieldset',
      '#title' => t('Adjustment Settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['adjustment']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Display Text for the adjustment'),
      '#size' => 32,
      '#maxlength' => 64,
      '#default_value' => isset($value['adjustment']['name']) ? $value['adjustment']['name'] : '',
      '#description' => t("This is what will appear next to the adjustment amount on orders."),
      '#required' => TRUE,
    );
    $form['adjustment']['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Adjustment value'),
      '#size' => 32,
      '#maxlength' => 64,
      '#default_value' => isset($value['adjustment']['value']) ? $value['adjustment']['value'] : 0,
      '#description' => t("The value of the adjustment. Negative numbers will give discounts."),
      '#required' => TRUE,
    );
    $form['adjustment']['weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Adjustment weight'),
      '#size' => 32,
      '#maxlength' => 64,
      '#default_value' => isset($settings['adjustment']['weight']) ? $settings['adjustment']['weight'] : 0,
      '#description' => t("The weight of the adjustment. Specifies which order the adjustments are calculated in. Can be negative."),
      '#required' => TRUE,
    );
    $form['adjustment']['type'] = array(
      '#type' => 'radios',
      '#title' => t('Percentage or Fixed Amount?'),
      '#default_value' => isset($value['adjustment']['type']) ? $value['adjustment']['type'] : 'percentage',
      '#options' => array('percentage' => t('Percentage'), 'fixed' => t('Fixed')),
      '#required' => TRUE,
    );
    $form['adjustment']['scope'] = array(
      '#type' => 'radios',
      '#title' => t('Scope'),
      '#default_value' => isset($value['adjustment']['scope']) ? $value['adjustment']['scope'] : 'recurring',
      '#options' => ms_core_get_scopes(),
      '#required' => TRUE,
      '#description' => t('Whether the adjustment should affect the first payment only or all payments. Only applies for Recurring Payments.'),
    );
    return $form;
  }
}
