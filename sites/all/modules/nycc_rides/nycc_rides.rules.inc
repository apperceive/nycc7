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
      'variables' => array(
        'member' => array('type' => 'user', 'label' => t('Member Joining The Ride')),
        'ride' => array('type' => 'node', 'label' => t('Ride Being Joined')),
      ),
    ),
  
    'nycc_rides_member_joins_waitlist' => array(
      'label' => t('Member Joins a Waitlist'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'member' => array('type' => 'user', 'label' => t('Member Joining The Waitlist')),
        'ride' => array('type' => 'node', 'label' => t('Ride Being Waitlisted On')),
      ),
    ),

    
    'nycc_rides_member_joins_from_waitlist' => array(
      'label' => t('Member Joins From a Waitlist'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'member' => array('type' => 'user', 'label' => t('Member Joining The Ride')),
        'ride' => array('type' => 'node', 'label' => t('Ride Being Join')),
      ),
    ),    
    'nycc_rides_member_withdraws_from_ride' => array(
      'label' => t('Member Withdraws From a Ride'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'member' => array('type' => 'user', 'label' => t('Member Withdrawing From The Ride')),
        'ride' => array('type' => 'node', 'label' => t('Ride Withdrawing From')),
      ),
    ),    

    'nycc_rides_member_withdraws_from_waitlist' => array(
      'label' => t('Member Withdraws From a Waitlist'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'member' => array('type' => 'user', 'label' => t('Member Withdrawing From The Waitlist')),
        'ride' => array('type' => 'node', 'label' => t('Ride\'s Wiatlist Withdrawing From')),
      ),
    ), 

    'nycc_rides_ride_is_submitted' => array(
      'label' => t('Ride Is Submitted And Awaiting Approval'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'submitter' => array('type' => 'user', 'label' => t('Member Who Submitted The Ride')),
        'ride' => array('type' => 'node', 'label' => t('Ride Awaiting Approval')),
      ),
    ),

    'nycc_rides_ride_is_approved' => array(
      'label' => t('Ride Is Approved'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'approver' => array('type' => 'user', 'label' => t('Approver')),
        'ride' => array('type' => 'node', 'label' => t('Ride That Was Approved')),
      ),
    ),
    
    'nycc_rides_ride_is_cancelled' => array(
      'label' => t('Ride Is Cancelled'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'cancelled_by' => array('type' => 'user', 'label' => t('Cancelled By User')),
        'ride' => array('type' => 'node', 'label' => t('Ride That Was Cancelled')),
      ),
    ),    

    'nycc_rides_ride_is_revised' => array(
      'label' => t('Ride Is Revised'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'revised_by' => array('type' => 'user', 'label' => t('Revised By User')),
        'ride' => array('type' => 'node', 'label' => t('Revised Ride')),
        'original_ride' => array('type' => 'node', 'label' => t('Ride Prior To Revision')),
      ),
    ),
    
    'nycc_rides_ride_is_viewed' => array(
      'label' => t('Ride Is Viewed'),
      'module' => 'nycc_rides',
      'group' => 'NYCC Rides' ,
      'variables' => array(
        'viewed_by' => array('type' => 'user', 'label' => t('Viewing User')),
        'ride' => array('type' => 'node', 'label' => t('Ride That Was Viewed')),
      ),
    ),
  );
}

/**
 * Implementation of hook_rules_condition_info().
 */
function nycc_rides_rules_condition_info() {
  // Conditions, like actions, are described by an array with keys
  // representing the machine name of the condition (which also by default is
  // the name of the callback function).
  $conditions['nycc_rides_signup_is_enabled'] = array(
    'label' => t('Signup for Ride is Enabled'),
    'module' => 'nycc_rides',
    'group' => t('NYCC Rides'),
    // Parameters are described identically to how they work for actions.
    'parameter' => array(
      '$ride' => array(
        'type' => 'node',
        'label' => t('Ride to check'),
      ),
    ),
  );
  return $conditions;
}


/**
 * The callback for the condition 'nycc_rides_signup_is_enabled'.
 * @param <node> $ride
 
 * @return <boolean>
 *   TRUE or FALSE depending on if the given view has the given base table or
 *   not.
 */
function nycc_rides_signup_is_enabled($ride) {
  $ride_status = nycc_rides_ride_should_be_enabled($ride);

  // All conditions return TRUE or FALSE.
  return ($ride_status['value']);
}



?>