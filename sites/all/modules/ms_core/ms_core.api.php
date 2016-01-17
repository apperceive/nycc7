<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup ms_core_api MS Core API
 * @{
 * Hooks and functions that are used by MoneyScripts modules.
 *
 * This includes functions for adding gateways, products, dealing with orders,
 * carts, etc.
 * @}
 */

/**
 * Allows for adding fields to the shopping cart page.
 *
 * This can be used to display forms or other information on the cart page.
 *
 * @return array
 *   An array of fields that should be added to cart page, containing:
 *     id: A unique field id.
 *     title: The field name.
 *     html: The html that should be displayed.
 *     weight: The weight of the field.
 *
 * @ingroup ms_core_api
 */
function hook_ms_cart_fields() {
  $fields = array();
  $fields[] = array(
    'id' => 'unique_field_id',
    'title' => t('Field name'),
    'html' => drupal_get_form('my_form'),
    'weight' => 11,
  );

  return $fields;
}

/**
 * Allows for adding fields to the checkout page.
 *
 * This can be used to display forms or other information on the checkout page.
 *
 * @return array
 *   An array of fields that should be added to checkout page, containing:
 *     id: A unique field id.
 *     title: The field name.
 *     html: The html that should be displayed.
 *     weight: The weight of the field.
 *
 * @ingroup ms_core_api
 */
function hook_ms_checkout_fields() {
  $fields = array();
  $fields[] = array(
    'id' => 'unique_field_id',
    'title' => t('Field name'),
    'html' => drupal_get_form('my_form'),
    'weight' => 11,
  );

  return $fields;
}

/**
 * Called whenever a product id is changed.
 *
 * This can be used to update any stored product ids in a database or other
 * places, so that the product ids are not orphaned.
 *
 * @param string $old_id
 *   The old product id.
 * @param string $new_id
 *   The new product id.
 *
 * @ingroup ms_core_api
 */
function hook_ms_product_id_change($old_id, $new_id) {
  // No example.
}

/**
 * Adds a license check for the module.
 *
 * This is only used by MoneyScripts modules that have a license on
 * MoneyScripts.net.
 */
function hook_ms_license() {
  // No example.
}

/**
 * Adds override settings to product forms.
 *
 * Using '#ms_override' => TRUE, you can specify a form element to be shown
 * on product plan pages so that users can override the setting on a per-plan
 * basis.
 *
 * @return array
 *   An array of modules keyed by module id containing:
 *     title: The title of the module
 *     form: The form builder containing the overridable settings.
 *
 * @see ms_core_variable_get()
 * @ingroup ms_core_api
 */
function hook_ms_core_overrides() {

}

/**
 * Registers products with MS Core.
 *
 * This is used to register all of a module's products for use with select lists
 * and other places.
 *
 * @return array
 *   An array of MsProduct objects containing the following extra keys:
 *     owner: The user id of the product owner.
 *     module_title: The title of the module.
 *     type: The type of product (recurring or cart).
 *     data: An array of product data.
 *     edit_path: The url of the edit product page.
 *     purchase_path: The url to purchase the product.
 *
 * @ingroup ms_core_api
 */
function hook_ms_products() {
  // No example.
}

/**
 * Assigns a user to an order.
 *
 * @param string $type
 *   The type of order, recurring or cart.
 * @param MsProduct $product
 *   The product object.
 * @param MsOrder $order
 *   The order object.
 * @param MsPayment $payment
 *   The payment object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_order_assign_user($type, $product, $order, $payment) {
  // No example.
}

/**
 * Act on a payment.
 *
 * @param string $type
 *   The type of order, recurring or cart.
 * @param MsProduct $product
 *   The product object.
 * @param MsOrder $order
 *   The order object.
 * @param MsPayment $payment
 *   The payment object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_order_payment($type, $product, $order, $payment) {
  switch ($product->module) {
    case 'mymodule':
      // Logic here
      break;
  }
}

/**
 * Allows modules to modify which payment gateways are shown at checkout.
 *
 * This gets called just after the gateways are fetched via hook_payment_gateway
 * for display as options on the checkout page. Useful for removing gateways
 * depending on other criteria such as user location in a profile field.
 *
 * @param array $gateways
 *   An array of payment gateways that can be altered.
 * @param object $cart
 *   The cart object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_payment_gateway_alter($gateways, $cart) {
  // Hide the PayPal WPS option.
  if (!empty($gateways['paypal_wps'])) {
    unset($gateways['paypal_wps']);
  }
}

/**
 * Allows payment gateways to show their particular billing info form.
 *
 * This is used by some gateways that save payment profiles for users and allow
 * them to edit them, such as Stripe and Authorize.net.
 *
 * @param string $html
 *  A display array that can have elements added to it.
 * @param object $account
 *  The user account.
 *
 * @ingroup ms_core_api
 */
function hook_ms_core_billing_info_alter(&$html, $account) {
  $payment_profiles = ms_core_payment_profiles_load_by_user($account->uid);
  // Load the saved payment profiles and shown them with an Edit link that goes to an edit page
  foreach ($payment_profiles as $payment_profile) {
    $saved_card = (!empty($payment_profile->cc_num)) ? $payment_profile->cc_num : t('N/A');

    $billing_address = t('N/A');
    if ($payment_profile && isset($payment_profile->address)) {
      $billing_address = t("@address - @city , @state", array(
        '@address' => $payment_profile->address,
        '@city' => $payment_profile->city,
        '@state' => $payment_profile->state,
      ));
    }

    $html['GATEWAY_profile'][$payment_profile->id] = array(
      '#type' => 'fieldset',
      '#title' => t('Saved Profile - !edit', array('!edit' => l(t('Edit'), 'user/' . $payment_profile->uid . '/GATEWAY/billing/' . $payment_profile->id))),
    );
    $html['GATEWAY_profile'][$payment_profile->id]['card'] = array(
      '#type' => 'item',
      '#title' => t('Saved Card'),
      '#value' => $saved_card
    );
    $html['GATEWAY_profile'][$payment_profile->id]['billing_address'] = array(
      '#type' => 'item',
      '#title' => t('Billing Address'),
      '#value' => $billing_address,
    );
  }
}

/**
 * Allows modules to modify a product as it is added to cart.
 *
 * This is useful when you need to change something about a product when it is
 * added to the cart.
 *
 * @param MsProduct $product
 *   The product being added.
 * @param object $cart
 *   The cart object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_cart_added_product_alter($product, $cart) {
  // Change the product price.
  switch ($product->name) {
    case 'Silver Membership':
      $product->amount = 10.00;
      break;
  }
}

/**
 * React when a product is added to the cart.
 *
 * @param object $cart
 *   The cart object.
 * @param MsProduct $product
 *   The product that was added.
 *
 * @ingroup ms_core_api
 */
function hook_ms_cart_add($cart, $product) {
  // Add an adjustment to the cart for this product.
  $adjustment = new MsAdjustment();
  $adjustment->id = 'CUSTOM_FEE_1';
  $adjustment->product_id = $product->cart_product_id;
  $adjustment->display = "CUSTOM FEE";
  $adjustment->type = 'percentage';
  $adjustment->value = 10;
  $adjustment->weight = 1;
  $adjustment->scope = 'recurring';

  // Add the tax to the order.
  if ($adjustment->value) {
    ms_core_add_cart_adjustment($adjustment, TRUE);
  }
}

/**
 * Allows modules to act when a new order is created.
 *
 * @param MsOrder $order
 *   The order object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_order_new($order) {
  // No example.
}

/**
 * Allows modules to act when a new order is loaded.
 *
 * @param MsOrder $order
 *   The order object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_order_load($order) {
  // No example.
}
