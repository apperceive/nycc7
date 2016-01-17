<?php

// ======================================
// Classes
// ======================================

/**
 * MsOrder class
 *
 * @author Leighton
 */
class MsOrder
{
  // Property Declaration
  // We are using var instead of public to preserve PHP 4 compatibility
  var $oid = 0;
  var $uid = 0;
  var $order_key = '';
  var $order_type = 'cart';
  var $status = 'checkout';
  var $title = '';
  var $order_number = '';
  var $gateway = '';
  var $amount = 0;
  var $total = 0;
  var $currency;
  var $secured = 1;
  var $recurring_schedule = array(
    'total_occurrences' => 0,
    'main_amount' => 0,
    'main_length' => 0,
    'main_unit' => '',
    'has_trial' => FALSE,
    'trial_amount' => 0,
    'trial_length' => 0,
    'trial_unit' => '',
  );
  var $first_name = '';
  var $last_name = '';
  var $email_address = '';
  var $shipping_address = array(
    'street' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => '',
    'phone' => '',
  );
  var $billing_address = array(
    'street' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => '',
    'phone' => '',
  );
  var $data = array();
  var $products = array();
  var $payments = array();
  var $adjustments = array();
  var $history = array();
  var $created;
  var $modified;
  var $unique_key;

  /**
   * For PHP 4 Compatibility
   */
  function MsOrder() {
    // Call the constructor
    $argcv = func_get_args();
    call_user_func_array(array(&$this, '__construct'), $argcv);
  }

  /**
   * Constructor - set some additional defaults
   */
  function __construct() {
    // Set the default variables
    $this->created = time();
    $this->modified = time();
    $this->currency = variable_get('ms_core_default_currency', 'USD');
    $this->order_key = ms_core_generate_order_key($this);
    $this->unique_key = drupal_get_token(serialize($this));
  }

  /**
   * Loads an order from an $oid
   *
   * @param $oid
   *   The order id of the order to load
   */
  function load($oid) {
    // Load the order from the database
    $result = db_query("SELECT * FROM {ms_orders} WHERE oid = %d", $oid);

    if (!$row = db_fetch_object($result)) {
      return FALSE;
    }

    // Set the values
    $this->oid = $row->oid;
    $this->uid = $row->uid;
    $this->order_type = $row->order_type;
    $this->status = $row->status;
    $this->gateway = $row->gateway;
    $this->amount = round($row->amount, 2);
    $this->total = $row->total;
    $this->currency = $row->currency;
    $this->secured = $row->secured;
    $this->first_name = $row->first_name;
    $this->last_name = $row->last_name;
    $this->email_address = $row->email_address;
    $this->created = $row->created;
    $this->modified = $row->modified;
    $this->unique_key = $row->unique_key;
    $this->order_key = $row->order_key;

    // Get the Products for the Order
    $this->products = ms_core_get_order_products($row->oid);

    // Get the Adjustments for the Order
    $this->adjustments = ms_core_get_order_adjustments($row->oid);

    // Get the Payments for the Order
    $this->payments = ms_core_get_order_payments($row->oid);

    // Get the History for the Order
    $this->history = ms_core_get_order_history($row->oid);

    $this->recurring_schedule = unserialize($row->recurring_schedule);

    // Set the defaults
    if (!isset($this->recurring_schedule['trial_length'])) {$this->recurring_schedule['trial_length'] = '';}
    if (!isset($this->recurring_schedule['trial_unit'])) {$this->recurring_schedule['trial_unit'] = '';}
    if (!isset($this->recurring_schedule['trial_amount'])) {$this->recurring_schedule['trial_amount'] = '';}
    if (!isset($this->recurring_schedule['has_trial'])) {$this->recurring_schedule['has_trial'] = FALSE;}
    if (!isset($this->recurring_schedule['main_length'])) {$this->recurring_schedule['main_length'] = '';}
    if (!isset($this->recurring_schedule['main_unit'])) {$this->recurring_schedule['main_unit'] = '';}
    if (!isset($this->recurring_schedule['main_amount'])) {$this->recurring_schedule['main_amount'] = '';}

    // Apply adjustments to the recurring schedule main amount
    // Removed this so that trial periods will work
    if ($this->recurring_schedule['trial_length'] AND $this->recurring_schedule['trial_unit']) {
      $this->recurring_schedule['trial_amount'] = round(ms_core_get_product_adjusted_price($this->recurring_schedule['trial_amount'], $this->adjustments, 'all'), 2);
    }
    else {
      $this->recurring_schedule['main_amount'] = round(ms_core_get_product_adjusted_price($this->recurring_schedule['main_amount'], $this->adjustments, 'recurring'), 2);
    }

    $this->shipping_address = unserialize($row->shipping_address);
    $this->billing_address = unserialize($row->billing_address);

    // Set the defaults
    if (!isset($this->billing_address['street'])) {$this->billing_address['street'] = '';}
    if (!isset($this->billing_address['city'])) {$this->billing_address['city'] = '';}
    if (!isset($this->billing_address['state'])) {$this->billing_address['state'] = '';}
    if (!isset($this->billing_address['zip'])) {$this->billing_address['zip'] = '';}
    if (!isset($this->billing_address['country'])) {$this->billing_address['country'] = '';}
    if (!isset($this->billing_address['phone'])) {$this->billing_address['phone'] = '';}

    if (!isset($this->shipping_address['street'])) {$this->shipping_address['street'] = '';}
    if (!isset($this->shipping_address['city'])) {$this->shipping_address['city'] = '';}
    if (!isset($this->shipping_address['state'])) {$this->shipping_address['state'] = '';}
    if (!isset($this->shipping_address['zip'])) {$this->shipping_address['zip'] = '';}
    if (!isset($this->shipping_address['country'])) {$this->shipping_address['country'] = '';}
    if (!isset($this->shipping_address['phone'])) {$this->shipping_address['phone'] = '';}

    $this->data = unserialize($row->data);

    $this->title = ms_core_get_order_title($this, 127);
    $this->order_number = ms_core_order_number($this);

    // Calculate the totals
    $this->calculate_total();

    return TRUE;
  }

  /**
   * Saves the MsOrder object to the database
   *
   * @return
   *   Returns TRUE if the operation is successful, FALSE otherwise
   */
  function save() {
    // Calculate the totals
    $this->calculate_total();

    if ($this->oid > 0) {
      // Update into the database
      return drupal_write_record('ms_orders', $this, 'oid');
    }
    else {
      // Insert into Database
      if (drupal_write_record('ms_orders', $this)) {
        return TRUE;
      }
      return FALSE;
    }
  }

  /**
   * Calculates and sets the correct total based on the products
   */
  function calculate_total() {
    // Set the price and total again
    $this->amount = ms_core_get_final_price($this);
    $this->total = ms_core_get_order_total($this);

    if ($this->order_type == 'recurring') {
      foreach ($this->products as $product) {
        $this->recurring_schedule = $product->recurring_schedule;
        if ($product->recurring_schedule['has_trial']) {
          // Calculate the amount that should be paid for the initial payment from the trial_amount
          $this->recurring_schedule['trial_amount'] = round(ms_core_get_product_adjusted_price($product->recurring_schedule['trial_amount'], $this->adjustments, 'all'), 2);
        }
        else {
          // Set the trial length to the same as the main length
          $this->recurring_schedule['trial_length'] = $this->recurring_schedule['main_length'];
          $this->recurring_schedule['trial_unit'] = $this->recurring_schedule['main_unit'];
          // Calculate the amount that should be paid for the initial payment from the main_amount
          $this->recurring_schedule['trial_amount'] = round(ms_core_get_product_adjusted_price($product->recurring_schedule['main_amount'], $this->adjustments, 'all'), 2);
        }
        $this->recurring_schedule['main_amount'] = round(ms_core_get_product_adjusted_price($product->recurring_schedule['main_amount'], $this->adjustments, 'recurring'), 2);

        // If the initial payment is the same as the regular payment, don't do a trial
        if ($this->recurring_schedule['main_amount'] == $this->recurring_schedule['trial_amount']) {
          $this->recurring_schedule['has_trial'] = FALSE;
        }
        else {
          $this->recurring_schedule['has_trial'] = TRUE;
        }

        // Set the order amount to be the initial payment amount
        $this->amount = round($this->recurring_schedule['trial_amount'], 2);

        // We should only have 1 product in a recurring order, so break here
        break;
      }
    }
    else {
      // Just set the amount to what it should be for the initial payment (all adjustments)
      $this->amount = round(ms_core_get_final_price($this), 2);
    }
    // drupal_set_message(ms_core_print_r($this));
  }
}

/**
 * MsPayment class
 *
 * @author Leighton
 */
class MsPayment
{
  // Property Declaration
  // We are using var instead of public to preserve PHP 4 compatibility
  // Set the default members
  var $pid = 0;
  var $oid = 0;
  var $gateway = '';
  var $type = '';
  var $amount = 0;
  var $currency = '';

  var $transaction = '';
  var $recurring_id = '';
  var $data = array();

  var $recurring_schedule = array(
    'total_occurrences' => 0,
    'main_amount' => 0,
    'main_length' => 0,
    'main_unit' => '',
    'has_trial' => FALSE,
    'trial_amount' => 0,
    'trial_length' => 0,
    'trial_unit' => '',
  );

  var $shipping_address = array(
    'street' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => '',
    'phone' => '',
  );
  var $billing_address = array(
    'street' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => '',
    'phone' => '',
  );

  var $first_name = '';
  var $last_name = '';

  /**
   * For PHP 4 Compatibility
   */
  function MsPayment() {
    // Call the constructor
    $argcv = func_get_args();
    call_user_func_array(array(&$this, '__construct'), $argcv);
  }

  /**
   * Constructor
   */
  function __construct() {

  }

  /**
   * Loads a payment from the database
   *
   * @param $pid
   *   The id of the payment to load
   */
  function load($pid) {
    // Load the data from the database
    $result = db_query("SELECT * FROM {ms_payments} WHERE pid = %d", $pid);

    if (!$row = db_fetch_object($result)) {
      return FALSE;
    }

    // Set the values
    $this->pid = $row->pid;
    $this->oid = $row->oid;
    $this->type = $row->type;
    $this->transaction = $row->transaction;
    $this->recurring_id = $row->recurring_id;
    $this->gateway = $row->gateway;
    $this->amount = round($row->amount, 2);
    $this->currency = $row->currency;
    $this->created = $row->created;
    $this->modified = $row->modified;

    $this->recurring_schedule = unserialize($row->recurring_schedule);

    // Set the defaults
    if (!isset($this->recurring_schedule['trial_length'])) {$this->recurring_schedule['trial_length'] = '';}
    if (!isset($this->recurring_schedule['trial_unit'])) {$this->recurring_schedule['trial_unit'] = '';}
    if (!isset($this->recurring_schedule['trial_amount'])) {$this->recurring_schedule['trial_amount'] = '';}
    if (!isset($this->recurring_schedule['main_length'])) {$this->recurring_schedule['main_length'] = '';}
    if (!isset($this->recurring_schedule['main_unit'])) {$this->recurring_schedule['main_unit'] = '';}
    if (!isset($this->recurring_schedule['main_amount'])) {$this->recurring_schedule['main_amount'] = '';}

    $this->data = unserialize($row->data);

    return TRUE;
  }

  /**
   * Saves the object to the database
   *
   * @return
   *   Returns TRUE if the operation is successful, FALSE otherwise
   */
  function save() {
    // Ensure that the amount field is actually a decimal value
    if (!$this->amount) {
      $this->amount = 0;
    }
    if ($this->pid > 0) {
      // Update into the database
      return drupal_write_record('ms_payments', $this, 'pid');
    }
    else {
      // Insert into Database
      if (drupal_write_record('ms_payments', $this)) {
        return TRUE;
      }
      return FALSE;
    }
  }
}

/**
 * MsProduct class
 *
 * @author Leighton
 */
class MsProduct
{
  // Property Declaration
  // We are using var instead of public to preserve PHP 4 compatibility
  var $order_product_id = 0;
  var $oid = 0;
  var $type = 'cart';
  var $name = '';
  var $module = '';
  var $qty = 1;
  var $amount = '';
  var $id = '';
  var $owner = 0;
  var $edit_path = '';
  var $purchase_path = '';

  var $data = array();
  var $recurring_schedule = array(
    'total_occurrences' => 0,
    'main_amount' => 0,
    'main_length' => 0,
    'main_unit' => '',
    'has_trial' => FALSE,
    'trial_amount' => 0,
    'trial_length' => 0,
    'trial_unit' => '',
    );

  /**
   * For PHP 4 Compatibility
   */
  function MsProduct() {
    // Call the constructor
    $argcv = func_get_args();
    call_user_func_array(array(&$this, '__construct'), $argcv);
  }

  /**
   * Constructor
   */
  function __construct() {

  }

  /**
   * Loads the product from the database
   *
   * @param $id
   *   The id of the product to load
   */
  function load($id) {
    // Load the data from the database
    $result = db_query("SELECT * FROM {ms_order_products} WHERE order_product_id = %d", $id);

    if (!$row = db_fetch_object($result)) {
      return FALSE;
    }

    // Set the values
    $this->order_product_id = $row->order_product_id;
    $this->oid = $row->oid;
    $this->type = $row->type;
    $this->id = $row->id;
    $this->name = $row->name;
    $this->module = $row->module;
    $this->amount = round($row->amount, 2);
    $this->qty = $row->qty;

    $this->recurring_schedule = unserialize($row->recurring_schedule);

    // Set the defaults
    if (!isset($this->recurring_schedule['trial_length'])) {$this->recurring_schedule['trial_length'] = '';}
    if (!isset($this->recurring_schedule['trial_unit'])) {$this->recurring_schedule['trial_unit'] = '';}
    if (!isset($this->recurring_schedule['trial_amount'])) {$this->recurring_schedule['trial_amount'] = '';}
    if (!isset($this->recurring_schedule['main_length'])) {$this->recurring_schedule['main_length'] = '';}
    if (!isset($this->recurring_schedule['main_unit'])) {$this->recurring_schedule['main_unit'] = '';}
    if (!isset($this->recurring_schedule['main_amount'])) {$this->recurring_schedule['main_amount'] = '';}

    $this->data = unserialize($row->data);

    return TRUE;
  }

  /**
   * Saves the object to the database
   *
   * @return
   *   Returns TRUE if the operation is successful, FALSE otherwise
   */
  function save() {
    $this->name = substr($this->name, 0, 127);
    // Ensure that the amount field is actually a decimal value
    if (!$this->amount) {
      $this->amount = 0;
    }
    if ($this->order_product_id > 0) {
      // Update into the database
      return drupal_write_record('ms_order_products', $this, 'order_product_id');
    }
    else {
      // Insert into Database
      if (drupal_write_record('ms_order_products', $this)) {
        return TRUE;
      }
      return FALSE;
    }
  }

  /**
   * Initializes a product object with variables.
   *
   * This is mostly used for loading cart products into the class.
   *
   * @param $product
   *   The $product object from the database
   */
  function initialize($product) {
    foreach ($product as $key => $value) {
      $this->$key = $value;
    }
    $this->name = substr($this->name, 0, 127);
    $this->data = unserialize($this->data);
    $this->recurring_schedule = unserialize($this->recurring_schedule);
  }
}

/**
 * MsAdjustment class
 *
 * @author Leighton
 */
class MsAdjustment
{
  // Property Declaration
  // We are using var instead of public to preserve PHP 4 compatibility
  var $id = '';
  var $product_id = NULL;
  var $display = '';
  var $type = 'fixed';
  var $scope = 'recurring';
  var $value = 0;
  var $weight = 0;
  var $data = array();

  /**
   * For PHP 4 Compatibility
   */
  function MsAdjustment() {
    // Call the constructor
    $argcv = func_get_args();
    call_user_func_array(array(&$this, '__construct'), $argcv);
  }

  /**
   * Constructor
   *
   * @param $adjustment
   *   (Optional) The $adjustment object from the database to use for initializing values
   */
  function __construct($adjustment = NULL) {
    if (isset($adjustment)) {
      $this->initialize($adjustment);
    }
  }

  /**
   * Initializes an adjustment object with variables.
   *
   * This is mostly used for loading cart adjustments into the class.
   *
   * @param $adjustment
   *   The $adjustment object from the database
   */
  function initialize($adjustment) {
    foreach ($adjustment as $key => $value) {
      $this->$key = $value;
    }
    $this->data = unserialize($this->data);
  }
}
