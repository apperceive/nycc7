<?php

/*
 * fix user passwords after Drupal 6 -> 7 users table migration
 */


  // Lower than DRUPAL_HASH_COUNT to make the update run at a reasonable speed.
  $hash_count_log2 = 11;


  $result = db_query("SELECT uid, pass FROM {users} WHERE uid > 0");
  foreach ($result as $account) {
    $new_hash = user_hash_password($account->pass, $hash_count_log2);
    if ($new_hash) {
      // Indicate an updated password.
      $new_hash  = 'U' . $new_hash;
      db_update('users')
        ->fields(array('pass' => $new_hash))
        ->condition('uid', $account->uid)
        ->execute();
    }
  }
  
  