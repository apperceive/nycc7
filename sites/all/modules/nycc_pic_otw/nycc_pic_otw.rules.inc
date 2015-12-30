<?php
/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function nycc_pic_otw_rules_event_info() {
  return array(

    'nycc_pic_otw_pic_is_submitted' => array(
      'label' => t('Picture Is Submitted And Awaiting Approval'),
      'module' => 'nycc_pic_otw',
      'group' => 'NYCC Pic of the Week' ,
      'variables' => array(
        'submitter' => array('type' => 'user', 'label' => t('Member Who Submitted The Picture')),
        'picture' => array('type' => 'node', 'label' => t('Picture Awaiting Approval')),
      ),
    ),

    'nycc_pic_otw_pic_is_approved' => array(
      'label' => t('Picture Is Approved'),
      'module' => 'nycc_pic_otw',
      'group' => 'NYCC Pic of the Week' ,
      'variables' => array(
        'approver' => array('type' => 'user', 'label' => t('Approver')),
        'picture' => array('type' => 'node', 'label' => t('Ride That Was Approved')),
      ),
    ),
    
    'nycc_pic_otw_pic_is_rejected' => array(
      'label' => t('Picture Is Rejected'),
      'module' => 'nycc_pic_otw',
      'group' => 'NYCC Pic of the Week' ,
      'variables' => array(
        'rejected_by' => array('type' => 'user', 'label' => t('Rejected By User')),
        'picture' => array('type' => 'node', 'label' => t('Picture That Was Rejected')),
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