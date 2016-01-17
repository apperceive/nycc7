<?php

// ======================================
// TRIGGERS
// ======================================

/*
* Triggers:
* + Gift card is activated
* + Gift card is redeemed
*/

/**
 * Implementation of hook_rules_event_info()
 */
function ms_gift_cards_rules_event_info() {
  $items = array(
    'ms_gift_cards_event_activation' => array(
      'label' => t('Gift Card Activated'),
      'module' => 'MS Gift Cards',
      'help' => t("Triggers when a user activated a gift card."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Account')),
        'gift_card' => array('type' => 'ms_gift_card', 'label' => t('Membership')),
        'order' => array('type' => 'ms_order', 'label' => t('Order')),
        'product' => array('type' => 'ms_product', 'label' => t('Product')),
        'payment' => array('type' => 'ms_payment', 'label' => t('Payment')),
      ),
    ),
    'ms_gift_cards_event_redemption' => array(
      'label' => t('Gift Card Redeemed'),
      'module' => 'MS Gift Cards',
      'help' => t("Triggers when a user redeems their gift card."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Account')),
        'gift_card' => array('type' => 'ms_gift_card', 'label' => t('Membership')),
        'order' => array('type' => 'ms_order', 'label' => t('Order')),
        'product' => array('type' => 'ms_product', 'label' => t('Product')),
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
function ms_gift_cards_rules_data_type_info() {
  return array(
    'ms_gift_card' => array(
      'label' => t('Gift Card'),
      'class' => 'rules_data_type_ms_gift_card',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'uses_input_form' => FALSE,
      'module' => 'MS Gift Cards',
    ),
  );
}

/**
* Defines the rules membership data type.
*/
class rules_data_type_ms_gift_card extends rules_data_type {
  function load($id) {
    return ms_gift_cards_load_gift_card($id);
  }

  function get_identifier() {
    $gift_card = &$this->get();
    return $gift_card->id;
  }
}
