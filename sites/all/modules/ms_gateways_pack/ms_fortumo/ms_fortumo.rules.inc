<?php
/**
 * @file
 */
// ======================================
// CONDITIONS
// ======================================

// ======================================
// ACTIONS
// ======================================

// ======================================
// TRIGGERS
// ======================================

/**
 * Implements  hook_rules_event_info()
 */
function ms_fortumo_rules_event_info() {
  $items = array(
    'ms_fortumo_event_widget_payment' => array(
      'label' => t('Fortumo Widget Payment Received'),
      'module' => 'MS Fortumo',
      'help' => t("Act when a widget payment has been received."),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('User Account')),
        'credits' => array('type' => 'number', 'label' => t('Credits')),
      ),
    ),
  );

  return $items;
}

// ======================================
// DATA TYPES
// ======================================

