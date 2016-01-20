<?php

/*
 * migrate user profile data
 *
 */
 
 /*
 
 
      db_update('users')
        ->fields(array('pass' => $new_hash))
        ->condition('uid', $account->uid)
        ->execute();
 
 */
 
  watchdog('nycc_migrate', "Convert user profile data - start.");
  
  $result = db_query("SELECT uid FROM {users} WHERE uid > 0 ORDER by uid");
  foreach ($result as $account) {
    
    $uid = $account->uid;
    $nid = 0;
    $pnode = NULL;
    $found = array();
    
    $sql = "SELECT nid FROM node WHERE uid = :uid AND type = 'profile' ORDER BY uid";
    $q = db_query($sql, array(':uid' => $uid));
    
    if (!$q) {
      
      // ***** TODO: no node, create pnode, what defaults? or just report error
      
      $nid = $pnode->nid;
      $found[$uid] = $nid;
    }
    else { 
      if (!isset($found[$uid])) {
        $pnode = node_load($nid);
        if ($pnode) {
         
          $nid = $pnode->nid;
          $found[$uid] = $nid;

        }
        else {
          // ***** TODO: unable to load profile node - not expected - report problem
          drush_print("Error: unable to hash new password for user: " . $account->uid); 
        }
      }
      else {
        // ***** TODO: delete duplicate profile node or report it
        drush_print("Error: unable to hash new password for user: " . $account->uid); 
        // delete additional ones if they exist
        $dsql = "DELETE FROM node WHERE type = 'profile' AND uid = :uid AND nid > :nid"
        $q2 = db_query($dsql, array(':uid' => $uid, ':nid' => $nid));
      }
    }
    
    // ***** TODO: test for profile load, else create
    $profile = profile2_load_by_user($account, 'profile');
    if (!$profile) {
      $params = array(
        'user' => $account, 
        'type' => 'profile',
        // 'label' => 'Profile'           // optional
      );
      // ***** TODO: ? add fields and values as ->field_x => [LANGUAGE_NONE][0]['value']
      $profile = profile2_create($params);
    }
    
    // ***** TODO: foreach field in array list, copy, handle types? (value, tid?, other?)
    $flds = array(
      // list of field names
      // group by type?
      // src field name
      // dst field name
    );
    
    foreach($flds as $fld) {
      // ***** TODO: copy node field to profile field
      // $profile->field_x = $pnode->field_xx;
    }
    
    // ***** TODO: report errors
    profile2_save($profile);
    
    // ***** TODO: delete $pnode?
    
  }
  
  watchdog('nycc_migrate', "Convert user profile data - complete.");
  
  