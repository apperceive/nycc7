<?php

/*
 * migrate user profile data
 *
 */

  
  watchdog('nycc_migrate', "Convert user profile data - start.");
  
  /* profile fields */
  $flds = array(
    'field_age_range gender',
    'field_registration_date_import',
    'field_riding_style',
    'field_ride_coordinator',
    'field_email_list_flag',
    'field_publish_email_flag',
    'field_publish_address_flag',
    'field_publish_phone_flag',
    'field_terms_of_use',
    'field_address',
    'field_city',
    'field_contact_name',
    'field_country',
    'field_emergency_contact_no',
    'field_first_name',
    'field_profile_last_eny_year',
    'field_last_name',
    'field_phone',
    'field_profile_extra',
    'field_review_last_date',
    'field_state',
    'field_waiver_last_date',
    'field_zip',
    'field_ride_reminders',
    'field_ride_rosters',
  );
  
  $sql = "SELECT uid FROM {users} WHERE uid > 0 AND uid = 3692";
  $q = db_query($sql);
  foreach ($q as $r) {
    $uid = $r->uid;
    drush_print("Notice: processing user: $uid");
    $account = user_load($uid);
    if (!$account) {
      drush_print("Error: unable to load user: $uid");
      continue;
    }
 
    $nid = 0;
    $pnode = NULL;
    $found = array();
    
    $sql2 = "SELECT nid FROM node WHERE uid = :uid AND type = 'profile' ORDER BY nid";
    $q2 = db_query($sql2, array(':uid' => $uid));
    
    if (!$q2) {
      
      // ***** TODO: no node found, create pnode, what defaults?
      drush_print("Error: no profile nodes found for uid: $uid");
      continue;
      //$nid = $newnode->nid;
      //$found[$uid] = $nid;
    }
    else { 
      foreach($q2 as $r2) {
        $nid = $r2->nid;
        drush_print("Notice: processing user: $uid, node: $nid");
        if (!isset($found[$uid])) {
          $pnode = node_load($nid);
          if ($pnode) {
            //$nid = $pnode->nid;
            $found[$uid] = $nid;
          }
          else {
            // ***** TODO: unable to load profile node - not expected
            drush_print("Error: unable to load profile node for user: $uid as node: $nid");
            continue;
          }
        }
        else {
          // ***** TODO: delete duplicate profile node
          drush_print("Error: found duplicate profile for user: $uid as node: $nid");
          continue;        
          // TODO: delete additional ones if they exist
          //$dsql = "DELETE FROM node WHERE type = 'profile' AND uid = :uid AND nid > :nid"
          //$q2 = db_query($dsql, array(':uid' => $uid, ':nid' => $nid));
        }
      }
    }
    
    $profile = profile2_load_by_user($account, 'profile');
    
    if (!$profile) {
      drush_print("Notice: creating profile2 for user: $uid");
      $params = array(
        'user' => $account, 
        'type' => 'profile',
        // 'label' => 'Profile'     // optional, how use this? 
      );
      $profile = profile2_create($params);
    }
    
    // copy custom fields (as arrays)
    foreach($flds as $fld) {
      $profile->{$fld} = isset($pnode->{$fld}) ? $pnode->{$fld} : array(LANGUAGE_NONE => array(0 => array('value' => 0)));
    }
    
    // drush_print(var_export($profile, 1));
    
    try {
      profile2_save($profile);
      $pid = isset($profile->pid) ? isset($profile->pid) : NULL;
      drush_print("Notice: saved profile2 for user: $uid and pid: $pid");
    } catch (Exception $e) {
      $emsg = $e->getMessage();
      drush_print("Error: unable to save profile2 for user: $uid, error: $emsg");
    }
    // ***** TODO: delete $pnode?
    
  }
  
  watchdog('nycc_migrate', "Convert user profile data - complete.");
  
  