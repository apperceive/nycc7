<?php

/*

// this is duplicate code was refactored but not checked in until after it had already been recreated
// real code in nycc_rides.pages.inc

//RIDE ROSTER RELATED FUNCTIONS
function nycc_rides_output_riders_roster_pdf($nid = NULL) {
  $output = "";
  $ride = node_load($nid);
  if ($ride) {
    // ASSERT: all rides have valid dates, titles, leaders and waitlist values
    $dateval = $ride->field_ride_timestamp[LANGUAGE_NONE][0]['value'];
    if ($dateval) {
      // set title
      $datestr = substr($dateval, 0, 16);
      drupal_set_title($ride->title ."-". $datestr);
      
      // first comes the ride details from a view
      $output .= views_embed_view('rides_detail_on_riders_list', 'block_1', $nid, $nid);
      
      // display waiver text from specific node
      $waiver = node_load('45438');  
      if ($waiver)
        $output .= $waiver->body[LANGUAGE_NONE][0]['value'];
      
      //TODO:CONFIRM THAT $leaders works! May need to wrap in nycc_field_get_value()
    
      // display ride participants using view blocks, passing uids as params
      $leaders = nycc_refs2ids(nycc_field_get_property_value($ride, 'field_ride_current_leaders'));
      $riders = nycc_refs2ids(nycc_field_get_property_value($ride, 'field_ride_current_riders'));
      $output .= views_embed_view('ride_participants', 'block_2', implode("+", array_merge($leaders, $riders)));
      $waiters = nycc_refs2ids(nycc_field_get_property_value($ride, 'field_ride_waitlist'));
      $output .= views_embed_view('ride_participants', 'block_3', implode("+", $waiters));
    } // dateval
  } else 
    $output = t("<h2 class='text-danger'>Invalid ride id: !id</h2>", array('!id' => $nid));

  return $output;
}
*/

// UTILITY FUNCTION, not callback, no UI
function nycc_rides_ride_roster_email($timestamp, $hrs) {
  //TODO FIX THIS SQL. Using Old Table Names
  //TODO FIX THIS SQL. Assumes leader is Node Reference. New field is user reference

//This function accepts a timestamp (generally REQUEST_TIME) and an hours interval and generates an email to the leaders of rides starting between those intervals
//NOTE: It has a hard coded action ID

  $rr_debug = TRUE;
  $action_id = 5;  // 5 is the tokenized email action for roster emails

  $sql=<<<EOS
  SELECT t.entity_id AS nid, field_ride_current_leaders_uid as uid
  FROM node n 
  join field_data_field_ride_timestamp t on n.nid = t.entity_id AND n.vid = t.revision_id
  join field_data_field_ride_current_leaders l on n.nid = l.entity_id AND n.vid = l.revision_id
  WHERE (t.field_ride_timestamp_value BETWEEN from_unixtime($timestamp) AND DATE_ADD(NOW(), INTERVAL $hrs HOUR))
EOS;
  if ($rr_debug ) watchdog("nycc-ride_roster", "SQL is " .  $sql, array(), WATCHDOG_NOTICE);
  $ride_count = 0;
  $q = db_query($sql);
    while ($row = db_fetch_object($q)) {
      $ride = node_load($row->nid);
      $acct = user_load($row->uid);

      if ( ($ride) && ($acct) ) {
        $ride_count++;
          $cp = profile2_load_by_user($acct->uid, "profile");
          if ($cp) {
            if ($cp->field_ride_rosters['und'][0]['value'] != "false") {
              $context = array('hook' => 'cron', 'op' => 'send', 'node' => $ride, 'user' => $acct);
              actions_do($action_id, $acct, $context);
        if ($rr_debug ) watchdog("nycc-ride_roster", $acct->name . " sent a roster for ride <a href='/node/". $ride->nid ."'>". $ride->title ."</a>");
            } else {
            if ($rr_debug ) watchdog("nycc-ride_roster", $acct->name . " skipping roster for ride <a href='/node/". $ride->nid ."'>". $ride->title ."</a>");
            }
          } else {// if cp
            watchdog("nycc-ride_roster", "Could not load profile for user: " . $acct->uid, array(), WATCHDOG_WARNING) ;
          }
      }  else  { //no ride or user
        watchdog("nycc-ride_roster", "Could not load Ride: " . $row->nid . " or User: " . $row->nid, array(), WATCHDOG_WARNING) ;
      }
    }  // while
    if($ride_count > 0) {
      if ($rr_debug ) watchdog("nycc-ride_roster", $ride_count . " rides found starting within " . $hrs . " hours" , array(), WATCHDOG_NOTICE) ;
    } else {
    if ($rr_debug ) watchdog("nycc-ride_roster", "No rides found starting within " . $hrs . " hours", array(), WATCHDOG_NOTICE);
    }
}

// NOT A CALLBACK, UI output function
function nycc_output_riders_roster_html($nid = NULL) {
//This function is used by a custom token to generate a roster to be emailed
//NOTE: It contains hardcoded view name and display IDs

  $rr_debug = TRUE;

   $view_name = "ride_participants";
  $display_name = "block_4";

  if (is_object($nid)) {
    $ride = $nid;
      $nid = $ride->nid;
  } elseif (is_numeric($nid) && ($nid > 0))  {
       $ride = node_load( $nid);
   }
    if ($ride) {
        $leaders = nycc_refs2ids($ride->field_ride_current_leaders);
        $riders = nycc_refs2ids($ride->field_ride_current_riders);
        $waiters = nycc_refs2ids($ride->field_ride_waitlist);
        $roster = array(implode("+", array_filter(array_merge($leaders, $riders, $waiters))));

//         if ($rr_debug ) watchdog("nycc-ride_roster", "Roster for ride: " . $nid . " is: " . implode("+", array_filter(array_merge($leaders, $riders, $waiters))) , array(), WATCHDOG_NOTICE);
 //        dpm(array_filter(array_merge($leaders, $riders, $waiters)));

      $view = views_get_view($view_name);
      if($view){

      global $user;
      $original_user = $user;
      $old_state = session_save_session();
      session_save_session(FALSE);
      $user = user_load(1);   // admin

        $view->hide_admin_links=TRUE;

        $output = $view->preview($display_name, $roster);

      $user = $original_user;
      session_save_session($old_state);
        return $output;
      } else { //coulnd't love view
        watchdog("nycc-ride_roster", "Could not load View: " . $view_name . " from function nycc_output_riders_roster_html", array(), WATCHDOG_ERROR) ;
      }

  } else { //no ride loaded
     watchdog("nycc-ride_roster", "Could not load Ride: " . $nid . " from function nycc_output_riders_roster_html", array(), WATCHDOG_ERROR) ;
   }
 }


//END RIDE ROSTER RELATED FUNCTIONS