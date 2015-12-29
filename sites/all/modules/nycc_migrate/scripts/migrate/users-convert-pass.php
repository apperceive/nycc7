<?php

/*
 * fix user passwords after Drupal 6 -> 7 users table migration
 *
 * run from drush
 *
 * from http://stackoverflow.com/questions/6205605/drupal-6-user-password-import-to-drupal-7
 * also http://drupal.stackexchange.com/questions/2279/how-do-i-migrate-users-passwords-from-drupal-6-to-drupal-7
 *
 * TODO: make this idempotent, use highwater mark?
 *
 */
 
  define('DRUPAL_ROOT', getcwd());
  include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');

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
  
  