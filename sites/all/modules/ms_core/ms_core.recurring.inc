<?php

/**
 * @file Contains code specific to recurring payments and gateway helper code
 */

/**
 * Access callback that will check if the user can modify their order
 */
function ms_core_modify_test($oid, $id) {
  global $user;
  $order = ms_core_order_load($oid);
  if (user_access('administer moneyscripts orders') OR ($user->uid == $order->uid)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Access callback to see if the user can cancel their order
 */
function ms_core_cancel_test($oid) {
  global $user;
  $order = ms_core_order_load($oid);
  if ($user->uid == $order->uid OR user_access('administer moneyscripts orders')) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Get the Cancellation URL for a recurring order
 */
function ms_core_get_billing_helper_url($order) {
  return 'user/' . $order->uid . '/billing-info';
}

/**
 * Get the Modification URL for a recurring order
 */
function ms_core_get_modify_helper_url($order, $id) {
  return 'ms/modify/' . $order->oid . '/' . $id;
}

/**
 * Get the Cancellation URL for a recurring order
 */
function ms_core_get_cancel_helper_url($order) {
  return 'ms/cancel/' . $order->oid;
}

/**
 * Initiate a cancellation
 */
function ms_core_cancel_confirm(&$form_state, $oid) {
  $order = ms_core_order_load($oid);
  $form['#ms_core_oid'] = $order->oid;
  $redirect = (isset($_GET['destination'])) ? $_GET['destination'] : '';
  return confirm_form($form, t('Are you sure you want to cancel your order?'), $redirect, t('This action cannot be undone.'), t('Confirm'));
}

/**
 * Submit handler to cancel an order
 */
function ms_core_cancel_confirm_submit($form, &$form_state) {
  $order = ms_core_order_load($form['#ms_core_oid']);

  // Enter the rec_cancel payment
  $payment = ms_core_new_payment($order->oid, $order->gateway, 'rec_cancel');

  // Generate a Unique Transaction ID
  $payment->transaction = ms_core_generate_transaction_id(10);
  $payment->currency = $order->currency;
  $payment->recurring_schedule = $order->recurring_schedule;
  $payment->billing_address = $order->billing_address;
  $payment->shipping_address = $order->shipping_address;
  $payment->recurring_id = $order->oid;

  ms_core_enter_payment($payment, FALSE);

  $redirect = (isset($_GET['destination'])) ? $_GET['destination'] : '';
  $form_state['redirect'] = $redirect;
}

/**
 * Initiate an order modification
 */
function ms_core_modify_confirm(&$form_state, $oid, $id) {
  $order = ms_core_order_load($oid);
  $new_product = ms_core_load_module_product(ms_core_get_order_module($order), $id);

  // Show if there will be a charge
  $details_text = t('Your order will be modified immediately. You will not be charged.');
  if ($amount = ms_core_get_outstanding_modification_amount($new_product, $order)) {
    $details_text = t("You will be charged @amount if you proceed.",
      array('@amount' => ms_core_format_money($amount, $order->currency)));
  }

  $form['#ms_core_oid'] = $order->oid;
  $form['#ms_core_id'] = $id;
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : '';
  return confirm_form($form, t('Are you sure you want to change your order to %new_prod?',
    array('%new_prod' => $new_product->name)), $redirect, $details_text, t('Confirm'));
}

/**
 * Submit callback when modifying an order
 */
function ms_core_modify_confirm_submit($form, &$form_state) {
  $approved = TRUE;
  $amount = 0;

  // Load the order and products
  $order = ms_core_order_load($form['#ms_core_oid']);
  $account = user_load($order->uid);

  $new_product = ms_core_load_module_product(ms_core_get_order_module($order), $form['#ms_core_id']);

  // Remove trial periods if any from modifications
  $new_product->recurring_schedule['has_trial'] = FALSE;
  $new_product->recurring_schedule['trial_length'] = 0;

  // Remove any coupons from the order
  ms_core_remove_order_adjustments($order);

  if ($amount = ms_core_get_outstanding_modification_amount($new_product, $order)) {
    // Ask the gateway to charge the order
    $approved = module_invoke($order->gateway, 'ms_core_modification_charge', $amount, $order);
  }

  if ($approved) {
    $data['new_product'] = $new_product;

    $order = ms_core_add_data($order, $data);

    // Enter the rec_modify payment
    $payment = ms_core_new_payment($order->oid, $order->gateway, 'rec_modify');

    // Generate a Unique Transaction ID
    $payment->transaction = ms_core_generate_transaction_id(10);
    $payment->amount = $amount;
    $payment->currency = $order->currency;
    $payment->data = array('new_product' => $new_product);
    $payment->recurring_schedule = $new_product->recurring_schedule;
    $payment->billing_address = $order->billing_address;
    $payment->shipping_address = $order->shipping_address;
    $payment->recurring_id = $order->oid;

    ms_core_enter_payment($payment, FALSE);

    $redirect = (isset($_GET['destination'])) ? $_GET['destination'] : '';
    $form_state['redirect'] = $redirect;
  }
}

/**
 * Calculates the outstanding amount that should be paid for a modification
 *
 * @param $new_product
 *   The new product
 * @param $order
 *   The order object
 *
 * @return
 *   The amount that should be charged in order to proceed with the modification
 */
function ms_core_get_outstanding_modification_amount($new_product, $order) {
  if ($amount = $new_product->recurring_schedule['main_amount']) {
    if (isset($order->data['residual_credit']) AND $credit = $order->data['residual_credit']) {
      $amount -= $credit;
    }

    if ($amount > 0) {
      // Since most gateways don't support charges for under $1, let's set that as a base amount
      if ($amount < 1) {
        $amount = 1.00;
      }
      return $amount;
    }
  }

  return 0;
}

/**
 * Insert a new recurring schedule
 */
function ms_core_create_recurring_schedule($oid, $gateway, $module, $recurring_schedule, $created = NULL) {
  // If a recurring schedule already exists, just return it
  if ($existing = ms_core_load_recurring_schedule($oid)) {
    return $existing;
  }
  $created = ($created) ? $created : time();

  $order = ms_core_order_load($oid);

  $order->recurring_schedule = $recurring_schedule;

  $last_payment = $created;

  if ($lp = ms_core_get_last_payment($order)) {
    $last_payment = $lp->created;
  }

  $next_payment = ms_core_calculate_next_payment_date($order, $last_payment);

  $expiration = ms_core_calculate_expiration_date($recurring_schedule);

  // First delete any existing recurring_schedules for this order
  db_query("DELETE FROM {ms_recurring_schedules} WHERE oid = %d", $oid);

  // Set the additional values
  $recurring_schedule['oid'] = $oid;
  $recurring_schedule['status'] = 'active';
  $recurring_schedule['gateway'] = $gateway;
  $recurring_schedule['module'] = $module;
  $recurring_schedule['next_payment'] = $next_payment;
  $recurring_schedule['current_payments'] = 0;
  $recurring_schedule['created'] = $created;
  $recurring_schedule['expiration'] = $expiration;
  $recurring_schedule['modified'] = $created;

  // Insert the recurring schedule
  return drupal_write_record('ms_recurring_schedules', $recurring_schedule);
}

/**
 * Calculates the expiration based on the start date, trial period, etc
 *
 * @param $recurring_schedule
 *   The recurring schedule array
 *
 * @return
 *   The timestamp of the expiration string
 */
function ms_core_calculate_expiration_date($recurring_schedule) {

  if ($recurring_schedule['total_occurrences']) {
    // Add time based on number of payments multiplied by period length and unit
    $regular_time = strtotime(ms_core_get_string_time($recurring_schedule['main_length'], $recurring_schedule['main_unit']), 0);
    $date = $regular_time * $recurring_schedule['total_occurrences'];

    // Add Time for Trial Period 1
    if ($recurring_schedule['trial_length']) {
      $date += strtotime(ms_core_get_string_time($recurring_schedule['trial_length'], $recurring_schedule['trial_unit']), 0);
    }

    return $date;
  }

  return 0;
}

/**
 * Calculates the date of the next payment from a recurring schedule
 *
 * @param $order
 *   The order object, passed by reference.
 * @param $last_payment
 *   The timestamp of the last payment
 *
 * @return
 *   The timestamp of the next payment. If the recurring schedule has a trial period,
 *   then the total_occurrences is incremented by one if it is not unlimited
 */
function ms_core_calculate_next_payment_date(&$order, $last_payment) {
  $num_payments = count($order->payments);
  // If this is the second payment (first would always be 'rec_signup', and
  // there is a trial period that is not free, then the next payment is after
  // the trial period.
  if ($num_payments == 2 && $order->recurring_schedule['has_trial'] && $order->recurring_schedule['trial_amount']) {
    // Add a payment for the trial to total_occurrences if needed.
    if ($order->recurring_schedule['total_occurrences']) {
      $order->recurring_schedule['total_occurrences'] += 1;
    }
    return strtotime(ms_core_get_string_time($order->recurring_schedule['trial_length'], $order->recurring_schedule['trial_unit']), $last_payment);
  }
  else {
    return strtotime(ms_core_get_string_time($order->recurring_schedule['main_length'], $order->recurring_schedule['main_unit']), $last_payment);
  }
}

/**
 * Update new recurring schedule
 */
function ms_core_update_recurring_schedule($recurring_schedule) {
  return drupal_write_record('ms_recurring_schedules', $recurring_schedule, 'id');
}

/**
 * Load a recurring_schedule
 */
function ms_core_load_recurring_schedule($oid) {
  $result = db_query("SELECT * FROM {ms_recurring_schedules} WHERE oid=%d", $oid);
  if ($result) {
    $recurring_schedule = db_fetch_object($result);
    return $recurring_schedule;
  }
  return FALSE;
}

/**
 * Increment the recurring schedule and reset next payment
 */
function ms_core_increment_recurring_schedule($oid, $time = NULL) {
  $time = ($time) ? $time : time();

  $order = ms_core_order_load($oid);

  // Attempt to load the recurring schedule. If not found, attempt to create one.
  if (!$recurring_schedule = ms_core_load_recurring_schedule($oid)) {
    ms_core_create_recurring_schedule($order->oid, $order->gateway, ms_core_get_order_module($order), $order->recurring_schedule);
    $recurring_schedule = ms_core_load_recurring_schedule($oid);
  }
  if ($recurring_schedule) {
    // Calculate the next payment date
    $next_payment = ms_core_calculate_next_payment_date($order, $time);

    // Handle Failed Payments
    if ($recurring_schedule->failed_payments) {
      $next_payment -= 259200 * $recurring_schedule->failed_payments;

      // If the $next_payment is before $time, we need to keep some of the failed payments
      $failed_payments = 0;
      if (variable_get('ms_core_charge_back_payments', FALSE)) {
        if ($next_payment < $time) {
          $failed_payments = floor(($time - $next_payment) / 259200);
        }
      }

      // Clear the failed payments counter
      db_query("UPDATE {ms_recurring_schedules} SET failed_payments = %d WHERE id = %d", $failed_payments, $recurring_schedule->id);
    }

    // Update into Database
    $result = db_query("UPDATE {ms_recurring_schedules} SET current_payments = %d, next_payment = %d WHERE oid = %d",
      $recurring_schedule->current_payments + 1, $next_payment, $oid);

    // Reset the notified flag
    db_query("UPDATE {ms_recurring_schedules} SET notified = %d WHERE id = %d", 0, $recurring_schedule->id);

    return $next_payment;
  }
  else {
    ms_core_log_error('ms_core', 'Unable to load the recurring schedule for a rec_payment increment. Order #: !order',
      array('!order' => $oid));
    return FALSE;
  }
}

/**
 * Cancel the recurring schedule and set next payment and expiration
 */
function ms_core_cancel_recurring_schedule($oid, $date = NULL) {
  $date = ($date) ? $date : time();

  // Load the recurring schedule
  $recurring_schedule = ms_core_load_recurring_schedule($oid);

  // Calculate the Expiration Date
  $now = time();
  $trial1 = 0;

  // Calculate time based on number of payments multiplied by period length and unit
  $regular_time = strtotime(ms_core_get_string_time($recurring_schedule->main_length, $recurring_schedule->main_unit), 0);

  if ($recurring_schedule->current_payments) {
    $regular_time = $regular_time * $recurring_schedule->current_payments;
  }

  // Calculate Time for Trial Period 1
  if ($recurring_schedule->trial_unit) {
    $trial1 = strtotime(ms_core_get_string_time($recurring_schedule->trial_length, $recurring_schedule->trial_unit), 0);
  }

  if (($recurring_schedule->created + $trial1) > $now) {
    $expiration = $recurring_schedule->created + $trial1;
  }
  elseif (($recurring_schedule->created + $trial1 + $regular_time) > $now) {
    $expiration = $recurring_schedule->created + $trial1 + $regular_time;
  }
  else {
    $expiration = $now;
  }

  // Update into Database
  $result = db_query("UPDATE {ms_recurring_schedules} SET total_occurrences = %d, expiration = %d, status = '%s' WHERE oid = %d",
    $recurring_schedule->current_payments, $expiration, 'cancelled', $oid);

  return $expiration;
}

/**
 * Mark the recurring schedule as expiring
 */
function ms_core_change_recurring_schedule_status($oid, $status) {
  // Load the recurring schedule
  $recurring_schedule = ms_core_load_recurring_schedule($oid);

  // Update into Database
  $result = db_query("UPDATE {ms_recurring_schedules} SET status = %d WHERE oid = %d",
    $status, $oid);

  return $result;
}

/*
 * Calculate how much credit a user has left in a recurring order
 */
function ms_core_calculate_credit($oid, $expiration = 0) {
  // Load the order
  if ($order = ms_core_order_load($oid)) {
    $credit = 0;
    // Set some variables
    $last_payment = $order->created;
    $price = $order->amount;

    // See if there is a payment entered for this order
    if ($lp = ms_core_get_last_payment($order)) {
      $last_payment = $lp->created;
      $price = $lp->amount;
    }

    // Set some base variables
    $time_left = ($expiration) ? $expiration - time() : 0;
    $total_time = ($expiration) ? $expiration  - $last_payment : 0;

    // Change the values if it is recurring
    if ($order->order_type == 'recurring') {
      $recurring_schedule = ms_core_load_recurring_schedule($order->oid);
      if ($recurring_schedule->next_payment) {
        $time_left = $recurring_schedule->next_payment - time();
        $total_time = $recurring_schedule->next_payment - $last_payment;
      }
    }

  if ($total_time > 0) {
      $credit = ($time_left / $total_time) * $price;
    }
    else {
      $credit = $price;
    }

    return ($credit > 0) ? $credit : 0;
  }
  else {
    return 0;
  }
}

/**
 * Load a payment_profile from the database by id
 *
 * @param int $id
 *    The id of the payment profile
 *
 * @return
 *    Return the payment_profile object
 */
function ms_core_payment_profile_load($id) {
  if (is_null($id)) {
    return FALSE;
  }

  $result = db_query("SELECT * FROM {ms_core_payment_profiles} WHERE id = %d", $id);
  while ($profile = db_fetch_object($result)) {
    return $profile;
  }
  return FALSE;
}

/**
 * Load the first payment profile for a user
 *
 * @param int $uid
 *    The user id
 *
 * @return
 *    The first payment profile
 */
function ms_core_payment_profile_load_by_user($uid) {
  $profiles = ms_core_payment_profiles_load_by_user($uid);
  if (!empty($profiles)) {
    return $profiles[0];
  }
  return FALSE;
}

/**
 * Load a payment_profile from the database
 *
 * @param int $oid
 *    The order id or blank to load the session's order
 *
 * @return
 *    Return the payment_profile object
 */
function ms_core_payment_profile_load_by_oid($oid) {
  if (is_null($oid) || $oid < 1) {
    return FALSE;
  }

  return db_fetch_object(db_query("SELECT * FROM {ms_core_payment_profiles} WHERE oid=%d", $oid));
}

/**
 * Load the payment profiles for a user
 *
 * @param int $uid
 *    The user id
 *
 * @return
 *    Return an array of the payment_profile objects
 */
function ms_core_payment_profiles_load_by_user($uid) {
  if (is_null($uid) || $uid < 1) {
    return array();
  }

  $result = db_query("SELECT * FROM {ms_core_payment_profiles} WHERE uid = %d", array($uid));
  $profiles = array();
  while ($profile = db_fetch_object($result)) {
    $profiles[] = $profile;
  }

  return $profiles;
}

/**
 * Delete Payment Profile
 */
function ms_core_payment_profile_delete($oid) {
  db_query("DELETE FROM {ms_core_payment_profiles} WHERE oid = %d", $oid);
}

/**
 * Save the payment_profile object
 *
 * @param object $payment_profile
 *    The payment_profile object to save
 *
 * @return
 *    Return the DB Result (boolean true) or FALSE
 */
function ms_core_payment_profile_save($pp) {
  // Convert to object if it is an array
  if (is_array($pp)) {
    $pp = (object) $pp;
  }

  $pp->cc_num = ms_core_mask_cc($pp->cc_num);

  if (isset($pp->id)) {
    // Update the record
    drupal_write_record('ms_core_payment_profiles', $pp, 'id');
  }
  else {
    drupal_write_record('ms_core_payment_profiles', $pp);
  }

  return $pp;
}

/**
 * Get the last payment string for an order
 *
 * @param object $order
 *    The order object to get the last payment string for
 *
 * @return
 *    The date and amount of the last payment, or 'None'
 */
function ms_core_get_last_payment_string($order) {
  $lp = ms_core_get_last_payment($order);
  if ($lp->created) {
    return format_date($lp->created, 'small') ." - ". ms_core_format_money($lp->amount, $lp->currency);
  }
  else {
    return t('None');
  }
}

/**
 * Get the next payment string for an order
 *
 * @param object $order
 *    The order object to get the next payment string for
 *
 * @return
 *    The date and amount of the next payment, or 'None'
 */
function ms_core_get_next_payment_string($order) {
  $recurring_schedule = ms_core_load_recurring_schedule($order->oid);
  if ($recurring_schedule AND $recurring_schedule->status == 'active' AND $recurring_schedule->next_payment > time()) {
    return ms_core_get_next_payment_date($order) ." - ". ms_core_get_next_payment_amount($order);
  }
  else {
    return t('None');
  }
}

/**
 * Get the next payment date
 *
 * @param $order
 * @return string
 *   The next payment date
 */
function ms_core_get_next_payment_date($order) {
  $recurring_schedule = ms_core_load_recurring_schedule($order->oid);
  if ($recurring_schedule->status == 'active' AND $recurring_schedule->next_payment > time()) {
    return format_date($recurring_schedule->next_payment, 'small');
  }
  else {
    return t('N/A');
  }
}

/**
 * Get the next payment amount
 *
 * @param $order
 * @return string
 *   The next payment amount
 */
function ms_core_get_next_payment_amount($order) {
  $recurring_schedule = ms_core_load_recurring_schedule($order->oid);
  if ($recurring_schedule->status == 'active' AND $recurring_schedule->next_payment > time()) {
    return ms_core_format_money($recurring_schedule->main_amount, $order->currency);
  }
  else {
    return t('N/A');
  }
}
