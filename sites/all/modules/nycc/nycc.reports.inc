<?php

// status <> 0 on ride nodes


function nycc_rides_report_leaders() {
  $sql = array();
  $sql['query'] = "SELECT np.title AS name, np.nid, count(*) AS cnt FROM node np, content_type_rides r, content_field_ride_leaders l, node nr WHERE l.field_ride_leaders_nid = np.nid AND np.type='profile' AND l.nid = r.nid AND l.vid = r.vid AND np.status <> 0 AND nr.nid = r.nid AND nr.vid = r.vid AND nr.status <> 0 ";

  $sql['count'] = "SELECT COUNT(DISTINCT np.title) AS cnt FROM node np, content_type_rides r, content_field_ride_leaders l, node nr WHERE l.field_ride_leaders_nid = np.nid AND np.type='profile' AND l.nid = r.nid AND l.vid = r.vid AND np.status <> 0 AND nr.nid = r.nid AND nr.vid = r.vid AND nr.status <> 0 ";

  return nycc_rides_report("Ride Reports: Leaders", $sql, nycc_rides_report_rides_header_1(), 'nycc_rides_report_rides_process_row_1', 1);
}

function nycc_rides_report_riders() {
  $sql = array();
  $sql['query'] = "SELECT np.title AS name, np.nid, count(*) AS cnt FROM node np, content_type_rides r, content_field_ride_current_riders cr, node nr WHERE cr.field_ride_current_riders_uid = np.uid AND np.type='profile' AND cr.nid = r.nid AND cr.vid = r.vid AND np.status <> 0 AND nr.nid = r.nid AND nr.vid = r.vid AND nr.status <> 0";

  $sql['count'] = "SELECT COUNT(DISTINCT np.title) AS cnt FROM node np, content_type_rides r, content_field_ride_current_riders cr, node nr WHERE cr.field_ride_current_riders_uid = np.uid AND np.type='profile' AND cr.nid = r.nid AND cr.vid = r.vid AND np.status <> 0 AND nr.nid = r.nid AND nr.vid = r.vid AND nr.status <> 0 ";

  return nycc_rides_report("Ride Reports: Riders", $sql, nycc_rides_report_rides_header_1(), 'nycc_rides_report_rides_process_row_1', 1);
}

function nycc_rides_report_rides($account) {
  $uid = $account->uid;
  $sql = array();
  $sql['query'] = "SELECT r.field_date_ride_first_value AS date, nr.nid, nr.vid, nr.title, r.field_ride_speed_value AS speed, r.field_ride_select_level_value AS level, r.field_ride_type_value AS type, r.field_ride_distance_in_miles_value AS distance, r.field_ride_spots_value AS spots, r.field_ride_status_value AS status FROM content_type_rides r, content_field_ride_current_riders cr, node nr WHERE cr.nid = r.nid AND cr.vid = r.vid AND nr.nid = r.nid AND nr.vid = r.vid AND nr.status <> 0 AND cr.field_ride_current_riders_uid = $uid ";

  $sql['count'] = null;
  global $user;
  if ($user->uid == $account->uid)
    $title = "My Rides";
  else
    $title = $account->name ."'s Rides";
  return nycc_rides_report($title, $sql, nycc_rides_report_rides_header_2(), 'nycc_rides_report_rides_process_row_2');
}

function nycc_rides_report_waitlists($account) {
  $uid = $account->uid;
  $sql = array();
  $sql['query'] = "SELECT r.field_date_ride_first_value AS date, nr.nid, nr.vid, nr.title, r.field_ride_speed_value AS speed, r.field_ride_select_level_value AS level, r.field_ride_type_value AS type, r.field_ride_distance_in_miles_value AS distance, r.field_ride_spots_value AS spots, r.field_ride_status_value AS status FROM content_type_rides r, content_field_ride_waitlist cr, node nr WHERE cr.nid = r.nid AND cr.vid = r.vid AND nr.nid = r.nid AND nr.vid = r.vid AND nr.status <> 0 AND cr.field_ride_waitlist_uid = $uid ";

  $sql['count'] = null;
  global $user;
  if ($user->uid == $account->uid)
    $title = "My Waitlists";
  else
    $title = $account->name ."'s Waitlists";
  return nycc_rides_report($title, $sql, nycc_rides_report_rides_header_2(), 'nycc_rides_report_rides_process_row_2');
}

function nycc_rides_report_leads($account) {
  $uid = $account->uid;
  $cp = content_profile_load('profile', $uid);
  $nid = $cp->nid;

  $sql = array();
  $sql['query'] = "SELECT r.field_date_ride_first_value AS date, nr.nid, nr.vid, nr.title, r.field_ride_speed_value AS speed, r.field_ride_select_level_value AS level, r.field_ride_type_value AS type, r.field_ride_distance_in_miles_value AS distance, r.field_ride_spots_value AS spots, r.field_ride_status_value AS status FROM content_type_rides r, content_field_ride_leaders l, node nr WHERE l.nid = r.nid AND l.vid = r.vid AND nr.nid = r.nid AND nr.vid = r.vid AND nr.status <> 0 AND l.field_ride_leaders_nid = $nid ";

  $sql['count'] = null;
  global $user;
  if ($user->uid == $account->uid)
    $title = "My Leads";
  else
    $title = $account->name ."'s Leads";
  return nycc_rides_report($title, $sql, nycc_rides_report_rides_header_2(), 'nycc_rides_report_rides_process_row_2');
}

function nycc_rides_report_rides_process_row_1($r) {
  return array('data' =>
    array(
      // Cells
      //theme('username', $r),
      l($r->name, 'node/'. $r->nid),  // todo: link to user rides panel?
      $r->cnt,
    ),
    // Attributes for tr
    //'class' => "nycc-report-". preg_replace('/[^a-z]/i', '-', $r->type) .' '. $classes[$r->severity]
  );
}

function nycc_rides_report_rides_header_1() {
  return array(
    array('data' => t('Member'), 'field' => 'np.title'),
    array('data' => t('# Rides'), 'field' => 'cnt', 'sort' => 'desc'),
  );
}

function nycc_rides_report_rides_process_row_2($r) {
  return array('data' =>
    array(
      // Cells

      l($r->title, 'node/'. $r->nid),
      date('m-d-Y', strtotime($r->date)) ." ". ((strtotime($r->date) > time()) ? '*' : ''),
      $r->level ."-ride",
      $r->speed,
      $r->distance,
      $r->type,
      $r->spots,
      //$r->remaining,  // TODO
      $r->status,
    ),
    // Attributes for tr
    // TODO: set class for future dates
    //'class' => "nycc-report-". preg_replace('/[^a-z]/i', '-', $r->type) .' '. $classes[$r->severity]
    'class' => (strtotime($r->date) > time()) ? 'nycc-ride-upcoming' : ""
  );
}

function nycc_rides_report_rides_header_2() {
  return array(
    //array('data' => ''),
    array('data' => t('Title'), 'field' => 'nr.title'),
    array('data' => t('Date (* upcoming)'), 'field' => 'r.field_date_ride_first_value', 'sort' => 'desc'),
    array('data' => t('Level'), 'field' => 'r.field_ride_select_level_value'),
    array('data' => t('Speed'), 'field' => 'r.field_ride_speed_value'),
    array('data' => t('Distance'), 'field' => 'r.field_ride_distance_in_miles_value'),
    array('data' => t('Type'), 'field' => 'r.field_ride_type_value'),
    array('data' => t('Spots'), 'field' => 'r.field_ride_spots_value'),
    //array('data' => t('Spots Remaining'), 'field' => 'nr.title'),
    array('data' => t('Status'), 'field' => 'r.field_ride_status_value'),
  );
}

function nycc_rides_report($title, $sql, $header, $process, $group = null) {
  drupal_set_title($title);

  $filter = nycc_rides_report_build_filter_queries();
  $rows = array();

  $output = drupal_get_form('nycc_rides_report_filter_form');

  if ($group)
    $group = " GROUP BY ". $group;

  $tablesort = tablesort_sql($header);
  // global $user;
  // if ($user->uid == 1)
  //watchdog('dev', var_export(array('sql' => $sql["query"] ." AND ". $filter['where'] . $group . $tablesort, 'args' => $filter['args']),1));
  if (!empty($filter['where'])) {
    $result = pager_query($sql["query"] ." AND ". $filter['where'] . $group . $tablesort, 20, 0, ($group ? $sql['count'] ." AND ". $filter['where'] : null), $filter['args']);
  }
  else {
    $result = pager_query($sql["query"] . $group . $tablesort, 20, 0, $sql['count']);
  }

  while ($r = db_fetch_object($result)) {
    $rows[] = $process($r);
  }

  if (!$rows) {
    $rows[] = array(array('data' => t('No information available.'), 'colspan' => 6));
  }

  $output .= theme('table', $header, $rows, array('class' => 'nycc-rides-report'));
  $output .= theme('pager', NULL, 20, 0);

  return $output;
}

function nycc_rides_report_build_filter_queries() {
  if (empty($_SESSION['nycc_rides_report_filter'])) {
    return;
  }

  $filters = nycc_rides_report_filters();
  //dsm($_SESSION['nycc_rides_report_filter']);

  // Build query
  $where = $args = array();
  foreach ($_SESSION['nycc_rides_report_filter'] as $key => $filter) {
    $filter_where = array();
    if (is_array($filter)) {
      foreach ($filter as $value) {
        $filter_where[] = $filters[$key]['where'];
        $args[] = $value;
      }
    } else {
      if ($key == 'startdate' && drupal_strlen($filter)) {
        $filter_where[] = "field_date_ride_first_value >= '%s'";
        $args[] = nycc_get_date($filter);
      }
      if ($key == 'enddate' && drupal_strlen($filter)) {
        $filter_where[] = "field_date_ride_first_value <= '%s'";
        $args[] = nycc_get_date($filter);
      }
    }
    if (!empty($filter_where)) {
      $where[] = '('. implode(' OR ', $filter_where) .')';
    }
  }
  // remove empty where's ?
  //watchdog('dev', var_export(array('where' => $where),1));

  $where = !empty($where) ? implode(' AND ', $where) : '';

  return array(
    'where' => $where,
    'args' => $args,
  );
}

function nycc_rides_report_filters() {
  $filters = array();

  $filters['level'] = array(
    'title' => t('Level'),
    'where' => 'r.field_ride_select_level_value = "%s"',
    'options' => nycc_ride_level_options(),
  );

  // $filters['status'] = array(
  //   'title' => t('Status'),
  //   'where' => 'r.field_ride_status_value = "%s"',
  //   'options' => nycc_ride_status_options(),
  // );
  //
  $filters['type'] = array(
    'title' => t('Type'),
    'where' => 'r.field_ride_type_value = "%s"',
    'options' => nycc_ride_type_options(),
  );

  // $filters['source'] = array(
  //   'title' => t('Source'),
  //   'where' => '',
  //   'options' => array('Riders', 'Leaders'),
  // );

  return $filters;
}

function nycc_rides_report_filter_form() {
  $session = &$_SESSION['nycc_rides_report_filter'];
  $session = is_array($session) ? $session : array();
  $filters = nycc_rides_report_filters();

  //dsm($session);

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter report'),
    '#theme' => 'nycc_rides_report_filters',
    '#collapsible' => TRUE,
    '#collapsed' => empty($session),
  );

  $form['filters']['status']['startdate'] = array(
    '#title' => t("Start Date"),
    '#type' => 'textfield',
    '#size' => 20,
    '#attributes'=> array('class' => 'w8em format-m-d-y divider-slash'),
  );
  if (!empty($session['startdate'])) {
    $form['filters']['status']['startdate']['#default_value'] = $session['startdate'];
  }

  $form['filters']['status']['enddate'] = array(
    '#title' => t("End Date"),
    '#type' => 'textfield',
    '#size' => 20,
    '#attributes'=> array('class' => 'w8em format-m-d-y divider-slash'),
  );
  if (!empty($session['enddate'])) {
    $form['filters']['status']['enddate']['#default_value'] = $session['enddate'];
  }

  foreach ($filters as $key => $filter) {
    $form['filters']['status'][$key] = array(
      '#title' => $filter['title'],
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => count($filter['options']),
      '#options' => $filter['options'],
      '#default_value' => '-1',
    );
    if (!empty($session[$key])) {
      $form['filters']['status'][$key]['#default_value'] = $session[$key];
    }
  }

  $form['filters']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  if (!empty($session)) {
    $form['filters']['buttons']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset')
    );
  }

  return $form;
}

function nycc_rides_report_filter_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Filter') && empty($form_state['values']['type']) && empty($form_state['values']['level']) && empty($form_state['values']['status']) && empty($form_state['values']['startdate']) && empty($form_state['values']['enddate'])) {
    form_set_error('startdate', t('You must select something to filter by.'));
  }
}

function nycc_rides_report_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = nycc_rides_report_filters();
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name])) {
          $_SESSION['nycc_rides_report_filter'][$name] = $form_state['values'][$name];
        }
      }
      if (isset($form_state['values']['startdate'])) {
        $_SESSION['nycc_rides_report_filter']['startdate'] = $form_state['values']['startdate'];
      }
      if (isset($form_state['values']['enddate'])) {
        $_SESSION['nycc_rides_report_filter']['enddate'] = $form_state['values']['enddate'];
      }
      break;
    case t('Reset'):
      $_SESSION['nycc_rides_report_filter'] = array();
      break;
  }
  return 'nycc-rides-report';
}


/**
 * Theme administration filter selector.
 *
 * @ingroup themeable
 */
function theme_nycc_rides_report_filters($form) {
  $output = '';
  foreach (element_children($form['status']) as $key) {
    $output .= drupal_render($form['status'][$key]);
  }
  $output .= '<div id="nycc-rides-report-buttons">'. drupal_render($form['buttons']) .'</div>';
  return $output;
}









?>