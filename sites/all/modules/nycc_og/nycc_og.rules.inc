<?php
/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function nycc_og_rules_event_info() {
  return array(
    'nycc_og_member_confirmed' => array(
      'label' => t('Member is confirmed in group'),
      'module' => 'nycc_og',
      'group' => 'NYCC Organic Groups' ,
      'variables' => array(
        'group' => array('type' => 'node', 'label' => t('Group')),
        'user' => array('type' => 'user', 'label' => t('Member who was confirmed')),
      ),
    ),

    'nycc_og_member_waitlisted' => array(
      'label' => t('Member is watlisted for a group'),
      'module' => 'nycc_og',
      'group' => 'NYCC Organic Groups' ,
      'variables' => array(
        'group' => array('type' => 'node', 'label' => t('Group')),
        'user' => array('type' => 'user', 'label' => t('Member who was waitlisted')),
      ),
    ),
    
    'nycc_og_confirmed_member_withdraws' => array(
      'label' => t('Confirmed Member withdraws from a group'),
      'module' => 'nycc_og',
      'group' => 'NYCC Organic Groups' ,
      'variables' => array(
        'group' => array('type' => 'node', 'label' => t('Group')),
        'user' => array('type' => 'user', 'label' => t('Member who has withdrawn')),
      ),
    ),

    'nycc_og_waitlisted_member_withdraws' => array(
      'label' => t('Waitlisted Member withdraws from a group'),
      'module' => 'nycc_og',
      'group' => 'NYCC Organic Groups' ,
      'variables' => array(
        'group' => array('type' => 'node', 'label' => t('Group')),
        'user' => array('type' => 'user', 'label' => t('Member who has withdrawn')),
      ),
    ),
  );
}

/**
 * Implementation of hook_rules_condition_info().
 */
//function nycc_pic_otw_rules_condition_info() {
//  // Conditions, like actions, are described by an array with keys
//  // representing the machine name of the condition (which also by default is
//  // the name of the callback function).
//  $conditions['nycc_pic_otw_pic_is_approved'] = array(
//    'label' => t('Ride is Approved'),
//    'module' => 'nycc_rides',
//    'group' => t('NYCC Rides'),
//    // Parameters are described identically to how they work for actions.
//    'parameter' => array(
//      '$ride' => array(
//        'type' => 'node',
//        'label' => t('Ride to check'),
//      ),
//    ),
//  );
//  
//  $conditions['nycc_rides_signup_is_enabled'] = array(
//    'label' => t('Signup for Ride is Enabled'),
//    'module' => 'nycc_rides',
//    'group' => t('NYCC Rides'),
//    // Parameters are described identically to how they work for actions.
//    'parameter' => array(
//      '$ride' => array(
//        'type' => 'node',
//        'label' => t('Ride to check'),
//      ),
//    ),
//  );
//  return $conditions;
//}
?>