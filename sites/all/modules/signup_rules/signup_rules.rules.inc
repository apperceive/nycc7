<?php

/**
 * @file
 * Rules hook implementations for Signup Rules module.
 */

/**
 * Implements hook_rules_event_info().
 */
function signup_rules_rules_event_info() {
  $events = array();
  $events['signup_rules_event_insert'] = array(
    'label' => t('User signed up for content'),
    'module' => 'Signup',
    'arguments' => _signup_rules_event_arguments(),
    'help' => 'Triggered when a user first signs up to content.',
  );
  $events['signup_rules_event_update'] = array(
    'label' => t('Signup was changed'),
    'module' => 'Signup',
    'arguments' => _signup_rules_event_arguments(),
    'help' => 'Triggered when signup data has been updated.',
  );
  $events['signup_rules_event_cancel'] = array(
    'label' => t('Signup was cancelled'),
    'module' => 'Signup',
    'arguments' => _signup_rules_event_arguments(),
    'help' => 'Triggered when a user\'s signup has been cancelled.',
  );
  $events['signup_rules_event_open'] = array(
    'label' => t('Signups were opened for content'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content to which signups were opened')),
    ),
    'help' => 'Triggered when the status of signups for content is opened.',
  );
  $events['signup_rules_event_close'] = array(
    'label' => t('Signups were closed for content'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content to which signups were closed')),
    ),
    'help' => 'Triggered when the status of signups for content is closed.',
  );
  $events['signup_rules_event_display'] = array(
    'label' => t('Signup form is being displayed'),
    'module' => 'Signup',
    'arguments' => array(
      'user' => array('type' => 'user', 'label' => t('User viewing content')),
      'node' => array('type' => 'node', 'label' => t('Content')),
      'signup_suppress' => array('type' => 'signup_suppress', 'label' => t('Signup form display status')),
    ),
    'help' => 'Triggered when the signup form is being prepared for display.',
  );
  $events['signup_rules_event_signup_node'] = array(
    'label' => t('User signed up from content signup form'),
    'module' => 'Signup',
    'arguments' => _signup_rules_event_arguments(),
    'help' => 'Triggered when a user signs up from the signup form on content.',
  );
  $events['signup_rules_event_signup_register'] = array(
    'label' => t('User signed up from registration form'),
    'module' => 'Signup',
    'arguments' => _signup_rules_event_arguments(),
    'help' => 'Triggered when a user signs up from the user registration form.',
  );
  $events['signup_rules_event_attended'] = array(
    'label' => t('Signup was marked attended'),
    'module' => 'Signup',
    'arguments' => _signup_rules_event_arguments(),
    'help' => 'Triggered when a user\'s signup has been marked attended.',
  );
  $events['signup_rules_event_not_attended'] = array(
    'label' => t('Signup was marked not attended'),
    'module' => 'Signup',
    'arguments' => _signup_rules_event_arguments(),
    'help' => 'Triggered when a user\'s signup has been marked not attended.',
  );
  return $events;
}

/**
 * Helper function to provide common event arguments.
 */
function _signup_rules_event_arguments() {
  return array(
    'user' => array('type' => 'user', 'label' => t('User who signed up for content')),
    'node' => array('type' => 'node', 'label' => t('Content to which the user is signed up')),
    'signup' => array('type' => 'signup', 'label' => t('Signup data')),
  );
}

/**
 * Implements hook_rules_condition_info().
 *
 * Some of these conditions could have opposites implemented, but that
 * is not necessary since Rules was nice enough to provide an inverted
 * value option.
 * @TODO: possible condition, user signed up from registration form?
 */
function signup_rules_rules_condition_info() {
  $conditions = array();
  $conditions['signup_total'] = array(
    'label' => t('Content signup total has value'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Signup content')),
    ),
    'base' => 'signup_rules_condition_signup_total',
    'help' => 'Evaluates to TRUE if the signup total of the content is less than, greater than, or equal to the given numeric value.',
  );
  $conditions['user_is_signed_up'] = array(
    'label' => t('User is signed up to content'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
      'user' => array('type' => 'user', 'label' => t('User')),
    ),
    'base' => 'signup_rules_condition_user_is_signed_up',
    'help' => 'Eveluates to TRUE if the selected user is signed up to content.',
  );
  $conditions['signups_opened'] = array(
    'label' => t('Signups are open for content'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'base' => 'signup_rules_condition_signups_open',
    'help' => 'Evaluates to TRUE if signups are open for content.',
  );
  $conditions['signup_attended'] = array(
    'label' => t('Signup is marked attended'),
    'module' => 'Signup',
    'arguments' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_rules_condition_signup_attended',
    'help' => 'Evaluates to TRUE if a signup has been marked \'attended\'.',
  );
  $conditions['signup_not_attended'] = array(
    'label' => t('Signup is marked not attended'),
    'module' => 'Signup',
    'arguments' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_rules_condition_signup_not_attended',
    'help' => 'Evaluates to TRUE if a signup has been marked \'not attended\'.',
  );
  return $conditions;
}

/**
 * Condition: determines whether a signup total for a node has a specific value.
 *
 * @param $node
 *   The node being checked.
 * @param $settings
 *   An array of settings from the condition form. This array has values
 *   from our custom elements to use to determine operators and values
 *   against which we check the number of signups for the node.
 */
function signup_rules_condition_signup_total($node, $settings) {
  $total = db_result(db_query("SELECT COUNT(*) FROM {signup_log} WHERE nid = %d", $node->nid));
  switch ($settings['signup_rules_condition']) {
    case 'greater':
      if ($total > $settings['signup_rules_total']) {
        return TRUE;
      }
      return FALSE;
    case 'equal':
      if ($total == $settings['signup_rules_total']) {
        return TRUE;
      }
      return FALSE;
    case 'less':
      if ($total < $settings['signup_rules_total']) {
        return TRUE;
      }
      return FALSE;
  }
}

/**
 * Condition: checks if user is signed up for a node.
 */
function signup_rules_condition_user_is_signed_up($node, $user) {
  $result = db_result(db_query("SELECT uid FROM {signup_log} WHERE uid = %d AND nid = %d", $user->uid, $node->nid));
  return !empty($result);
}

/**
 * Condition: Signups are open for content.
 */
function signup_rules_condition_signups_open($node) {
  return $node->signup_status == 1;
}

/**
 * Condition: Signup is marked attended.
 */
function signup_rules_condition_signup_attended($signup) {
  return $signup->attended == 1;
}

/**
 * Condition: Signup is marked not attended.
 */
function signup_rules_condition_signup_not_attended($signup) {
  return $signup->attended == 0;
}

/**
 * Implements hook_rules_action_info().
 */
function signup_rules_rules_action_info() {
  $actions = array();
  $actions['load_signup'] = array(
    'label' => t('Load signup data'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
      'user' => array('type' => 'user', 'label' => t('User')),
    ),
    'new variables' => array(
      'signup_loaded' => array(
        'type' => 'signup',
        'label' => t('Loaded signup data'),
        'save' => FALSE,
      ),
    ),
    'base' => 'signup_rules_action_load_signup',
  );
  $actions['signup_user'] = array(
    'label' => t('Sign up user to content'),
    'module' => 'Signup',
    'arguments' => array(
      'user' => array('type' => 'user', 'label' => t('User')),
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'new variables' => array(
      'signup_created' => array(
        'type' => 'signup',
        'label' => t('Created signup data'),
        'save' => FALSE,
      ),
    ),
    'base' => 'signup_rules_action_signup_user',
    'help' => 'Signs up a user to content.',
  );
  $actions['cancel_node_signups'] = array(
    'label' => t('Cancel all signups for content'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'base' => 'signup_rules_action_cancel_node_signups',
    'help' => 'Cancels all signups for a piece of content.',
  );
  $actions['cancel_user_signups'] = array(
    'label' => t('Cancel all signups for user'),
    'module' => 'Signup',
    'arguments' => array(
      'user' => array('type' => 'user', 'label' => t('User')),
    ),
    'base' => 'signup_rules_action_cancel_user_signups',
    'help' => 'Cancels all signups for a given user.',
  );
  $actions['open_signups'] = array(
    'label' => t('Open signups for content'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'base' => 'signup_rules_action_open_signups',
    'help' => 'Opens signups for a piece of content.',
  );
  $actions['close_signups'] = array(
    'label' => t('Close signups for content'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'base' => 'signup_rules_action_close_signups',
    'help' => 'Closes signups for a piece of content.',
  );
  $actions['mark_attended'] = array(
    'label' => t('Mark signup as attended'),
    'module' => 'Signup',
    'arguments' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_mark_attended_action',
    'help' => 'Marks a signup as attended.',
  );
  $actions['mark_not_attended'] = array(
    'label' => t('Mark signup as not attended'),
    'module' => 'Signup',
    'arguments' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_mark_not_attended_action',
    'help' => 'Marks a signup as not attended.',
  );
  $actions['cancel_signup'] = array(
    'label' => t('Cancel signup'),
    'module' => 'Signup',
    'arguments' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_cancel_action',
    'help' => 'Cancels an individual signup.',
  );
  $actions['mail_to_signup_users'] = array(
    'label' => t('Send mail to signed up users'),
    'module' => 'Signup',
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'eval input' => array('subject', 'message'),
    'base' => 'signup_rules_action_mail_to_signup_users',
    'help' => 'Sends a mail to all users signed up for content.',
  );
  $actions['suppress_form'] = array(
    'label' => t('Prevent display of signup form'),
    'module' => 'Signup',
    'arguments' => array(
      'signup_suppress' => array('type' => 'signup_suppress', 'label' => t('Signup form display status')),
    ),
    'base' => 'signup_rules_action_suppress',
    'help' => 'Prevents the display of the signup form.',
  );
  return $actions;
}

/**
 * Action: Load signup data.
 */
function signup_rules_action_load_signup($node, $user) {
  $query = db_query('SELECT sl.*, n.title, u.name, u.mail FROM {signup_log} sl INNER JOIN {node} n ON sl.nid = n.nid INNER JOIN {users} u ON sl.uid = u.uid WHERE sl.nid = %d AND sl.uid = %d', $node->nid, $user->uid);
  if ($signup = db_fetch_object($result)) {
    return array('signup_loaded' => $signup);
  }
  return;
}

/**
 * Action: Sign up a user to content.
 */
function signup_rules_action_signup_user($user, $node, $settings) {
  // Node and user IDs must be stored in an array for the signup function.
  $data['nid'] = $node->nid;
  $data['uid'] = $user->uid;

  // Check if the node signup status should be ignored.
  if ($settings['ignore'] == 1) {
    signup_sign_up_user($data);
  }
  else {
    // Check if signups are enabled for the node.
    $status = db_result(db_query('SELECT status FROM {signup} WHERE nid = %d', $node->nid));
    if ($status == 1) {
      signup_sign_up_user($data);
    }
  }

  $signup = db_fetch_object(db_query('SELECT sl.*, n.title, u.name, u.mail FROM {signup_log} sl INNER JOIN {node} n ON sl.nid = n.nid INNER JOIN {users} u ON sl.uid = u.uid WHERE sl.nid = %d AND sl.uid = %d', $user->uid, $node->nid));
  return array('signup_created' => $signup);
}

/**
 * Action: Cancel all signups for a node.
 */
function signup_rules_action_cancel_node_signups($node, $settings) {
  $signups = signup_get_signups($node->nid);
  foreach ($signups as $signup) {
    signup_cancel_signup($signup, FALSE);
  }
  return;
}

/**
 * Action: Cancel all signups for a user.
 */
function signup_rules_action_cancel_user_signups($user) {
  $signups = signup_rules_get_user_signups($user->uid);
  foreach ($signups as $signup) {
    signup_cancel_signup($signup, FALSE);
  }
}

/**
 * Action: Close signups on a node.
 *
 * The conditional checks to see if signups are even enabled for
 * the node type. If they aren't then there is no attempt to close
 * them as they should be closed by Signup already.
 */
function signup_rules_action_close_signups($node) {
  if (variable_get('signup_node_default_state_'. $node->type, 'disabled') != 'disabled') {
    signup_close_signup($node->nid);
  }
}

/**
 * Action: Open signups on a node.
 *
 * The conditional checks to ensure that signups are enabled for
 * the node before programatically opening signups.
 */
function signup_rules_action_open_signups($node) {
  if (variable_get('signup_node_default_state_'. $node->type, 'disabled') != 'disabled') {
    signup_open_signup($node->nid);
  }
}

/**
 * Action: Send mail to all users signed up for content.
 */
function signup_rules_action_mail_to_signup_users($node, $settings) {
  $signups = array();

  // Build the proper query for selecting limited signups.
  $limit = $settings['limit'];
  if ($limit == 'attended') {
    $result = db_query('SELECT u.uid, u.name, u.mail, u.language, s_l.* FROM {signup_log} s_l INNER JOIN {users} u ON u.uid = s_l.uid WHERE s_l.nid = %d AND attended = 1', $node->nid);
  }
  elseif ($limit == 'not attended') {
    $result = db_query('SELECT u.uid, u.name, u.mail, u.language, s_l.* FROM {signup_log} s_l INNER JOIN {users} u ON u.uid = s_l.uid WHERE s_l.nid = %d AND attended = 0', $node->nid);
  }

  // If there is no limit then we leave the empty array to pass to Signup.
  if ($limit != 'all') {
    while ($signup = db_fetch_object($result)) {
      $signups[] = $signup;
    }
  }

  signup_rules_send_mail($node, $signups, $settings);
}

/**
 * Send mail to signed up users. This function is separated so other modules
 * can make use of it as is done in signup_status_rules.
 */
function signup_rules_send_mail($node, $signups, $settings) {
  $subject = str_replace(array("\r", "\n"), '', $settings['subject']);
  $body = $settings['message'];
  $from = !empty($settings['from']) ? str_replace(array("\r", "\n"), '', $settings['from']) : variable_get('site_mail', ini_get('sendmail_from'));

  // If limit was set to all then an empty array will be passed in $signups
  // and signup_set_broadcaster() will populate the array with all signups.
  module_load_include('inc', 'signup', 'includes/broadcast');
  signup_send_broadcast($node->nid, $subject, $body, $from, FALSE, $signups);
  watchdog('signup_rules', 'Successfully sent email to %limit users signed up for %title.', array('%limit' => $limit, '%title' => $node->title), WATCHDOG_NOTICE, l(t('view'), 'node/'. $node->nid));
}

/**
 * Action: Prevent display of signup form.
 */
function signup_rules_action_suppress(&$suppress) {
  $suppress = TRUE;
}

/**
 * Implements hook_rules_date_type_info().
 *
 * Tokens are made available by Signup module, so we make use of their
 * Token type. Also, our data type class makes use of save and load
 * functions, so this data type is saveable.
 */
function signup_rules_data_type_info() {
  return array(
    'signup' => array(
      'label' => t('Signup data'),
      'class' => 'rules_data_type_signup',
      'module' => 'Signup',
      'savable' => FALSE,
      'token type' => 'signup',
      'identifiable' => TRUE,
    ),
    'signup_suppress' => array(
      'label' => t('Signup form display status'),
      'module' => 'Signup',
      'savable' => FALSE,
      'identifiable' => TRUE,
    ),
  );
}

/**
 * Defines the signup data type.
 */
class rules_data_type_signup extends rules_data_type {

//  function save() {
//    $signup = &$this->get();
//    signup_save_signup($signup);
//    return TRUE;
//  }

  function load($sid) {
    return signup_load_signup($sid);
  }

  function get_identifier() {
    $signup = $this->get();
    return $signup->sid;
  }
}
