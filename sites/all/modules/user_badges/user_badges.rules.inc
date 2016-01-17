<?php
/**
* @file
* Provide better integration into the rules module
*/

/**
* Implementation of hook_rules_event_info().
*/
function user_badges_rules_event_info() {
 return array(
   'user_badges_badge_given' => array(
     'label' => t('User was given a badge'),
     'arguments' => array(
       'user' => array('type' => 'user', 'label' => t('User before receiving badge')),
       'badge_id' => array('type' => 'value', 'label' => t('The given Badge\'s ID')),
     ),
     'module' => 'User Badges',
   ),
   'user_badges_badge_removed' => array(
     'label' => t('User was removed a badge'),
     'arguments' => array(
       'user' => array('type' => 'user', 'label' => t('User before removing badge')),
       'badge_id' => array('type' => 'value', 'label' => t('The removed Badge\'s ID')),
     ),
     'module' => 'User Badges',
   ),
 );
}

/**
* Implementation of hook_rules_condition_info().
*/
function user_badges_rules_condition_info() {
 return array(
   'user_badges_has_badge' => array(
     'label' => t('User has badge'),
     'arguments' => array(
       'account' => array('type' => 'user', 'label' => t('User')),
     ),
     'module' => 'User Badges',
   ),
   'user_badges_receives_badge' => array(
     'label' => t('Badge user is receiving'),
     'arguments' => array(
       'badge_id' => array('type' => 'value', 'label' => t('The given Badge\'s ID')),
     ),
     'module' => 'User Badges',
   ),
   'user_badges_badge_count' => array(
     'label' => t('User badge count'),
     'arguments' => array(
       'account' => array('type' => 'user', 'label' => t('User')),
       'number' => array('type' => 'number', 'label' => t('Number')),
     ),
     'module' => 'User Badges',
   ),
   'user_badges_has_badge_by_name' => array(
     'label' => t('User has badge by name'),
     'arguments' => array(
       'account' => array('type' => 'user', 'label' => t('User')),
       'badgename' => array('type' => 'string', 'label' => t('Badge Name')),
     ),
     'module' => 'User Badges',
   ),
 );
}

/**
* Configuration form for user_badges_has_badge condition
*/
function user_badges_has_badge_form($settings = array(), &$form) {
  $form['badges'] = array('#tree' => TRUE);
  $badges = user_badges_get_badges('all');
  $settings['badges'] = is_array($settings['badges']) ? $settings['badges'] : array();
  
  $form['settings']['operator'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#options' => array(
      'any' => t('Has one of'),
      'all' => t('Has all of'),
    ),
    '#default_value' => $settings['operator'],
  );
  foreach ($badges as $badge) {
    $form['settings']['badges'][$badge->bid] = array(
      '#type' => 'checkbox',
      '#title' => theme('user_badge', $badge),
      '#return_value' => 1,
      '#default_value' => $settings['badges'][$badge->bid],
      '#description' => check_plain($badge->name),
    );
  }
  
  return $form;
}

/**
* Test the user_badges_has_badge condition
*/
function user_badges_has_badge($account, $settings) {
  $badges = user_badges_get_badges($account->uid);
  $result = $settings['operator'] == 'all' ? TRUE : FALSE;
  foreach ($settings['badges'] as $bid => $on) {
    if (!$on) {
      continue;
    }
    $has_badge = isset($badges[$bid]);
    if ($settings['operator'] == 'all') {
      $result = $result && $has_badge;
    }
    else {
      $result = $result || $has_badge;
    }
  }
  return $result;
}

/**
* Configuration form for user_badges_receives_badge condition
*/
function user_badges_receives_badge_form($settings = array(), &$form) {
  $form['badges'] = array('#tree' => TRUE);
  $badges = user_badges_get_badges('all');
  $settings['badges'] = is_array($settings['badges']) ? $settings['badges'] : array();
  
  foreach ($badges as $badge) {
    $form['settings']['badges'][$badge->bid] = array(
      '#type' => 'checkbox',
      '#title' => theme('user_badge', $badge),
      '#return_value' => 1,
      '#default_value' => $settings['badges'][$badge->bid],
      '#description' => check_plain($badge->name),
    );
  }
  
  return $form;
}

/**
* Test the user_badges_receives_badge condition
*/
function user_badges_receives_badge($badge_id, $settings) {
  $badge_bool = FALSE;
  foreach ($settings['badges'] as $bid => $on) {
    if ($bid == $badge_id && $on == 1) {
      $badge_bool = TRUE;
    }
  }
  return $badge_bool;
}

/**
* Configuration form for user_badges_badge_count condition
*/
function user_badges_badge_count_form($settings = array(), &$form) {
  $form['settings']['operation'] = array(
    '#type' => 'select',
    '#title' => t('User\'s number of badges is'),
    '#options' => array('greater' => t('Greater than'), 'equal' => t('Equal to'), 'less' => t('Less than')),
    '#default_value' => isset($settings['operation']) ? $settings['operation'] : 'equal',
  );
  $form['settings']['number']['#weight'] = 2;
}

/**
* Test the user_badges_badge_count condition
*/
function user_badges_badge_count($account, $number, $settings) {
  $badges = user_badges_get_badges($account->uid);
  $badges_count = count($badges);
  switch ($settings['operation']) {
    case 'greater':
      return $badges_count > $number;
  
    case 'equal':
      return $badges_count == $number;
  
    case 'less':
      return $badges_count < $number;
  }
}

/**
 * Test the user_badges_has_badge_by_name condition
 */
function user_badges_has_badge_by_name($account, $badgename, $settings) {
  $has_badge = FALSE;
  $user_badges = user_badges_get_badges($account->uid);
  if (count($user_badges)) {
    foreach ($user_badges as $bid => $badge) {
      if ($badgename == $badge->name) {
        $has_badge = TRUE;
        break;
      }
    }
  }
  
  return $has_badge;
}

/**
 * Implements hook_rules_action_info().
 */
function user_badges_rules_action_info() {
  return array(
    'user_badges_action_add_badge_by_name' => array(
      'label' => t('Add badge by name'),
      'arguments' => array(
      'account' => array('type' => 'user', 'label' => t('Acting User')),
    ),
      'module' => 'User',
      'eval input' => array('badge'),
    ),
    'user_badges_action_remove_badge_by_name' => array(
      'label' => t('Remove badge by name'),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Acting User')),
      ),
      'module' => 'User',
      'eval input' => array('badge'),
    ),
  );
}

/**
 * Implements a Rules action.
 * Adds a badge to the current user, by the textual name of the badge.
 */
function user_badges_action_add_badge_by_name($account, $settings) {
  if (isset($account->uid)) {
    $uid = $account->uid;
  }
  else {global $user;$uid = $user->uid;}

  $badges = user_badges_get_badges('all');
  foreach ($badges as $bid => $vals) {
    if ($settings['badge'] == $vals->name) {
      user_badges_user_add_badge($uid, $bid, 'user');
    }
  }
}

function user_badges_action_add_badge_by_name_form($settings, &$form) {
  $settings += array('badge' => '');
  $form['settings']['badge'] = array(
    '#type' => 'textfield',
    '#title' => t('Badge name'),
    '#default_value' => $settings['badge'],
    '#description' => t('The textual name of the badge as configured on the user badges admin page.'),
  );
}

/**
 * Implements a Rules action.
 * Removes a badge from the current user, by the textual name of the badge.
 */
function user_badges_action_remove_badge_by_name($account, $settings) {
  if (isset($account->uid)) {
    $uid = $account->uid;
  }
  else {global $user;$uid = $user->uid;}

  $badges = user_badges_get_badges('all');
  foreach ($badges as $bid => $vals) {
    if ($settings['badge'] == $vals->name) {
      user_badges_user_remove_badge($uid, $bid, 'user');
    }
  }
}

function user_badges_action_remove_badge_by_name_form($settings, &$form) {
   $settings += array('badge' => '');
   $form['settings']['badge'] = array(
     '#type' => 'textfield',
     '#title' => t('Badge name'),
     '#default_value' => $settings['badge'],
     '#description' => t('The textual name of the badge as configured on the user badges admin page.'),
   );
}