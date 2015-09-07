<?php
function nycc_rides_action_info() {
  return array(
    'nycc_rides_approve_ride_action' => array(
      'label' => t('NYCC Rides - Approve a Ride'),
      'type' => 'node',
      'configurable' => FALSE,
      'triggers' => array('any'),
    ),
  );
}

function nycc_rides_approve_ride_action($node, $context) {
  if (!isset($node->nid) && isset($context['nid']))
    $node = $context['nid'];

  if (is_numeric($node))
    $node = node_load($node);

  global $user;
  if (is_object($node) && nycc_rides_can_approve($user)) {
    nycc_rides_update_ride_status($node, 'Approved');    

    drupal_set_message(check_plain('Ride' . $node->title . " has been approved!"));
  } else {
    drupal_set_message(check_plain('You are not authorized to approve this ride.', "error"));
  }
}