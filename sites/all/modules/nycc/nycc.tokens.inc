<?php

function nycc_token_info() { 

  //  $info['tokens']['node']['nycc_rides_ride_coordinator_email'] = array(
  //    'name' => t('nycc_rides_ride_coordinator_email'),
  //    'description' => t('Returns the email address of Ride Coordinator based on the ride level'),
  //  );  
  //return $info;
}


function nycc_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);
  
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        //
        //case 'nycc_rides_ride_date_and_time':
        //
        //  $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_time_formatted($node)) : nycc_rides_get_ride_time_formatted($node) ;
        //  break;
        //                                   
        //case 'nycc_rides_participants_emails_csv':
        //  $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_all_rider_emails($node)) : nycc_rides_get_all_rider_emails($node) ;
        //  break;
        //
        //case 'nycc_rides_leader_emails_csv':
        //  $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_all_leader_emails($node)) : nycc_rides_get_all_leader_emails($node) ;
        //  break;
        //
        //case 'nycc_rides_leaders_names_as_links':
        //  $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_all_leader_names_emails($node)) : nycc_rides_get_all_leader_names_emails($node) ;
        //  break;
        //
        //case 'nycc_rides_ride_location':
        //  $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_location($node)) : nycc_rides_get_ride_location($node) ;
        //  break;
        //
        //case 'nycc_rides_ride_coordinator_name':
        //  $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_coordinator_names($node)) : nycc_rides_get_ride_coordinator_names($node) ;
        //  break;  
        //case 'nycc_rides_ride_coordinator_email':
        //  $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_coordinator_meails($node)) : nycc_rides_get_ride_coordinator_meails($node) ;
        //  break;  
      } //switch
    }//for each
  }//if $node
  return $replacements;
}
