<?php


function nycc_rides_token_info() { 
    $info['tokens']['node']['nycc_rides_leader_emails_csv'] = array(
      'name' => t('nycc_rides_leader_emails_csv'),
      'description' => t('Returns all leader emails as comma separated string'),
    );
  
    $info['tokens']['node']['nycc_rides_participants_emails_csv'] = array(
      'name' => t('nycc_rides_participants_emails_csv'),
      'description' => t('Returns all participants emails (riders, leaders, waiters) as comma separate string'),
    );

    $info['tokens']['node']['nycc_rides_ride_date_and_time'] = array(
      'name' => t('nycc_rides_ride_date_and_time'),
      'description' => t('Returns Date and time of a ride'),
    );
    $info['tokens']['node']['nycc_rides_leaders_names_as_links'] = array(
      'name' => t('nycc_rides_leaders_names_as_links'),
      'description' => t('Returns leaders Names as clickable mailto:links'),
    );
    $info['tokens']['node']['nycc_rides_leaders_names'] = array(
      'name' => t('nycc_rides_leaders_names'),
      'description' => t('Returns leaders Names separated by commas'),
    );    
    $info['tokens']['node']['nycc_rides_ride_location'] = array(
      'name' => t('nycc_rides_ride_location'),
      'description' => t('Returns location of Ride Start'),
    );
    $info['tokens']['node']['nycc_rides_ride_coordinator_name'] = array(
      'name' => t('nycc_rides_ride_coordinator_name'),
      'description' => t('Returns the name of Ride Coordinator based on the ride level'),
    );         
    $info['tokens']['node']['nycc_rides_ride_coordinator_email'] = array(
      'name' => t('nycc_rides_ride_coordinator_email'),
      'description' => t('Returns the email address of Ride Coordinator based on the ride level'),
    );
    $info['tokens']['node']['nycc_rides_ride_token_address'] = array(
      'name' => t('nycc_rides_ride_token_address'),
      'description' => t('Returns the email address of the ride for email approvals'),
    );
    $info['tokens']['node']['nycc_rides_ride_description_formatted'] = array(
      'name' => t('nycc_rides_ride_description_formatted'),
      'description' => t('Returns the ride description in html format'),
    );
    $info['tokens']['node']['nycc_rides_ride_enable_time_formatted'] = array(
      'name' => t('nycc_rides_ride_enable_time_formatted'),
      'description' => t('Returns the date & time a ride will be enabled'),
    );
    $info['tokens']['node']['nycc_ride_roster_html'] = array(
        'name' => t("nycc_ride_roster_html"),
        'description' => t("Ride Roster with contact info for Leaders"),
  );    
  return $info;
}


function nycc_rides_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);
  
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        
        case 'nycc_rides_ride_date_and_time':

          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_time_formatted($node)) : nycc_rides_get_ride_time_formatted($node) ;
          break;
                                           
        case 'nycc_rides_participants_emails_csv':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_all_rider_emails($node)) : nycc_rides_get_all_rider_emails($node) ;
          break;
        
        case 'nycc_rides_leader_emails_csv':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_all_leader_emails($node)) : nycc_rides_get_all_leader_emails($node) ;
          break;
        
        case 'nycc_rides_leaders_names':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_all_leader_names($node)) : nycc_rides_get_all_leader_names($node) ;
          break;
        
        case 'nycc_rides_leaders_names_as_links':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_all_leader_names_emails($node)) : nycc_rides_get_all_leader_names_emails($node) ;
          break;        
        case 'nycc_rides_ride_location':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_location($node)) : nycc_rides_get_ride_location($node) ;
          break;
        
        case 'nycc_rides_ride_coordinator_name':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_coordinator_names($node)) : nycc_rides_get_ride_coordinator_names($node) ;
          break;  
        case 'nycc_rides_ride_coordinator_email':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_coordinator_email($node)) : nycc_rides_get_ride_coordinator_email($node) ;
          break;
        case 'nycc_rides_ride_token_address':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_token_email($node)) : nycc_rides_get_ride_token_email($node) ;
          break;
        case 'nycc_rides_ride_description_formatted':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_get_ride_description_formatted($node)) : nycc_rides_get_ride_description_formatted($node) ;
          break;
        
        case 'nycc_rides_ride_enable_time_formatted':
          $replacements[$original] = $sanitize ? filter_xss(nycc_rides_ride_enable_time($node, TRUE)) : nycc_rides_ride_enable_time($node, TRUE) ;
          break;
        
        case 'nycc_ride_roster_html':
            $replacements[$original] = $sanitize ? filter_xss(nycc_output_riders_roster_html($node)) : nycc_output_riders_roster_html($node);
        break;        
        
      } //switch
    }//for each
  }//if $node
  return $replacements;
}
