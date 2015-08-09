<?php
/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function nycc_rides_rules_event_info() {
  return array(
    'nycc_rides_member_joins_ride' => array(
      'label' => t('Member Joins a Ride'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'arguments' => array(
        'member' => array('type' => 'user', 'label' => t('Member Joining The Ride')),
        'ride' => array('type' => 'node', 'label' => t('Ride Being Joined')),
      ),
    ),
    
    'nycc_rides_member_joins_waitlist' => array(
      'label' => t('Member Joins a Waitlist'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'arguments' => array(
        'member' => array('type' => 'user', 'label' => t('Member Joining The Waitlist')),
        'ride' => array('type' => 'node', 'label' => t('Ride Being Waitlisted On')),
      ),
    ),

    'nycc_rides_member_withdraws_from_ride' => array(
      'label' => t('Member Withdraws From a Ride'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'arguments' => array(
        'member' => array('type' => 'user', 'label' => t('Member Withdrawing From The Ride')),
        'ride' => array('type' => 'node', 'label' => t('Ride Withdrawing From')),
      ),
    ),    

    'nycc_rides_member_withdraws_from_waitlist' => array(
      'label' => t('Member Withdraws From a Waitlist'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'arguments' => array(
        'member' => array('type' => 'user', 'label' => t('Member Withdrawing From The Waitlist')),
        'ride' => array('type' => 'node', 'label' => t('Ride\'s Wiatlist Withdrawing From')),
      ),
    ), 

    'nycc_rides_ride_submitted' => array(
      'label' => t('Ride Is Submitted And Awaiting Approval'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'arguments' => array(
        'submitter' => array('type' => 'user', 'label' => t('Member Who Submitted The Ride')),
        'ride' => array('type' => 'node', 'label' => t('Ride Awaiting Approval')),
      ),
    ),

    'nycc_rides_ride_approved' => array(
      'label' => t('Ride Is Approved'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'arguments' => array(
        'member' => array('type' => 'user', 'label' => t('Approver')),
        'ride' => array('type' => 'node', 'label' => t('Ride That Was Approved')),
      ),
    ),    
  );
}


?>