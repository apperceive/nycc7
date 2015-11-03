<?php
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//Ride Coordinator RELATED FUNCTIONS
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
function nycc_rides_get_ride_coordinator_email($ride) {
  $ride = nycc_rides_get_ride_object($ride);
  if (isset($ride)){
    return nycc_field_get_text_value($ride->field_ride_select_level) . '-rides@nycc.org';
  }
}

function nycc_rides_get_ride_coordinator_emails($data) {
  return _nycc_rides_get_ride_coordinator_info($data, 'mail');
}


function nycc_rides_get_ride_coordinator_names($data) {
  return _nycc_rides_get_ride_coordinator_info($data, 'title');
}

function _nycc_rides_get_ride_coordinator_info($data, $field_to_get) {
  if(is_string($data) && strtolower($data) == "all") {
    $level = "%";
  } else {  
    if (is_object($data) && $data->type && ($data->type == 'rides')) {
      $level_array = nycc_field_get_value($data->field_ride_select_level);
      $level=$level_array[0]['value'];
    }
    else if (is_numeric($data)) {
      $ride = node_load($data);
    
      if ($ride){
        $level_array = nycc_field_get_value($ride->field_ride_select_level);
        $level=$level_array[0]['value'];
      }
    }
    else if (strlen($data) == 1 && in_array(strtolower($data), array('a', 'b', 'c'))) {
      $level = $data;
    } else {
      return "";
    }
   $level .= "-ride" ;
  }

  
  $sql = "SELECT u.mail, n.title FROM {users} u, {node} n, {field_data_field_ride_coordinator} c WHERE n.uid = u.uid AND c.entity_id = n.nid AND c.revision_id = n.vid AND c.field_ride_coordinator_value like :level";

  $q = db_query($sql, array(':level'=>$level));
  $ret="";
  foreach($q as $r) {
    if (drupal_strlen($ret))
      $ret .= ",";

      $ret .= $r->{$field_to_get};

  }
  return $ret;
}


function nycc_rides_ride_coordinators() {
  $coordinators = array();
  //$sql = 'SELECT c.field_ride_coordinator_value AS level, n.title as name, p.field_phone_value as phone, u.mail, u.uid FROM {content_field_ride_coordinator} c, {content_type_profile} p, {node} n, {users} u WHERE n.nid=c.nid AND p.nid=n.nid AND u.uid=n.uid AND field_ride_coordinator_value IS NOT NULL ORDER BY 1';
  $sql =<<<EOS
  SELECT 
  c.field_ride_coordinator_value AS level,
  n.title AS name,
  p.field_phone_value AS phone,
  u.mail,
  u.uid
  FROM 
  {node} n,
  {users} u,
  {field_data_field_phone} p,
  {field_data_field_ride_coordinator} c
  WHERE
  u.uid = n.uid AND 
  p.entity_id = n.nid AND
  c.entity_id = n.nid AND
  p.revision_id = n.vid AND
  c.revision_id = n.vid AND
  c.field_ride_coordinator_value IS NOT NULL 
  ORDER BY 1;
EOS;
  $results = db_query($sql);
  foreach ($results as $coordinator) {
    $coordinator->level = strtoupper(substr($coordinator->level,0,1)); // lcfirst($coordinator->level);
    $coordinators[] = $coordinator;
  }
  return $coordinators;
}

function nycc_rides_output_ride_coordinators() {
  $output = "<div class='ride-coordinators'>";
  $output .= "<label>". t("Ride Coordinators:") ."</label>";
  $coordinators = nycc_rides_ride_coordinators();
  foreach ($coordinators as $coordinator) {
    if (strtoupper($coordinator->level) == "A")
      $output .= "<div class='coordinator-a-rides'>A-rides: <a href='/user/" .
      $coordinator->uid . "' title='View profile in another tab or window...' target=_blank'>" .
      $coordinator->name . "</a>,<br/>" .
      $coordinator->phone . ",<br/><a title='Send email...' href=mailto:a-rides@nycc.org>a-rides@nycc.org</a></br></br></div>";

    if (strtoupper($coordinator->level) == "B")
      $output .= "<div class='coordinator-b-rides'>B-rides: <a href='/user/" .
      $coordinator->uid . "' title='View profile in another tab or window...' target=_blank'>" .
      $coordinator->name . "</a>,<br/>" .
      $coordinator->phone . ",<br/><a title='Send email...' href=mailto:b-rides@nycc.org>b-rides@nycc.org</a></br></br></div>";

    if (strtoupper($coordinator->level) == "C")
      $output .= "<div class='coordinator-c-rides'>C-rides: <a href='/user/" .
      $coordinator->uid . "' title='View profile in another tab or window...' target=_blank'>" .
      $coordinator->name . "</a>,<br/>" .
      $coordinator->phone . ",<br/><a title='Send email...' href=mailto:c-rides@nycc.org>c-rides@nycc.org</a></div>";
  }
  $output .= "</div>";
  return $output;
}



//RIDE COORDINATOR EMAIL FORM
function nycc_rides_ride_coordinators_email($form, &$form_state) {
  global $user;
	$form = array();

	$arg1 = arg(1);
	if (drupal_strlen($arg1) == 0)
  	$arg1 = "Leaders";

	$arg2 = arg(2);
	if (drupal_strlen($arg2) == 0)
  	$arg2 = "All";

	if (! user_access('emails riders and leaders', $user)){
    	// access denied
    	drupal_set_message("You must be an have the permission to 'email riders and leaders' to view this page.");
			return $form;
  }
	    	//nycc_turn_off_journal_form('nycc_ride_coordinators_email');
	$badge_names = array();
	$q = db_query("select '' as name UNION ALL select name from {user_badges_badges} order by name;");
  foreach($q as $row) {
		$badge_names[t($row->name)] = t($row->name);
  }	

	$form['description'] = array(
		'#prefix' => '<div>',
	  '#value' => t("Use this for to send an e-mail blast to the selected members."),
	  '#suffix' => '</div>',
	);

	$form['target'] = array(
		'#type' => 'select',
		'#required' => true,
		'#default_value' => $arg1,
		'#title' => t("Target Group"),
		'#options' => array("Leaders" => "Leaders", "Riders" => "Riders","By_Badge" => "By Badge"),
		'#description' => "Send e-mail to ride leaders or riders (over the past 12 months) or based on assigned badge.",
	);

	$form['level'] = array(
		'#type' => 'select',
		'#required' => true,
		'#default_value' => $arg2,
		'#title' => t("Ride Level"),
		'#options' => array("All" => "All", "A" => "A", "B" => "B", "C" => "C"),
		'#description' => "Choose the riding level of the riders you wish to e-mail.",
	);

	
	$form['badge'] = array(
		'#type' => 'select',
		'#required' => false,
		'#default_value' => $badge_names[''],
		'#title' => t("Target Members By Badge"),
		'#options' => $badge_names,
		'#description' => "Only applies if you are targeting members by Badge",
	);
	

	$form['from'] = array(
		'#type' => 'textfield',
		'#required' => true,
		'#title' => t("From"),
		'#description' => "Enter your email address. Required.",
		//'#default_value' => $cp->field_first_name[0]['value'] . " " . $cp->field_last_name[0]['value'],
		'#default_value' => $user->mail,
	);

	$form['subject'] = array(
		'#type' => 'textfield',
		'#required' => true,
		'#title' => t("Subject"),
		'#description' => "Enter the subject for the e-mail message. Required.",
	);

	$form['message'] = array(
		'#type' => 'textarea',
		'#required' => true,
		'#title' => t("Body"),
		'#rows' => 20,
		'#description' => "Enter the text of your message here. Required <br/>(Note: You can past html content into this box. Use an editor like <a href='http://www.quackit.com/html/online-html-editor/' target='_blank'>http://www.quackit.com/html/online-html-editor/</a> to generate HTML content.)",
	);

	$form['button'] = array(
		'#type' => 'submit',
		'#value' => t('Send'),
	);
	return($form);
}

function nycc_rides_ride_coordinators_email_validate($form, &$form_state) {
	if (!valid_email_address($form_state['values']['from']))
		form_set_error('from', "Please enter a valid e-mail address.");
		
	if ($form_state['values']['target'] == 'By Badge' &&  $form_state['values']['badge'] == '')
		form_set_error('badge', "you must select a badge if you want to target by badge");

		
	if ($form_state['values']['target'] != 'By Badge' &&  $form_state['values']['badge'] != '')
		form_set_error('target', "you must select 'By Badge' as your target group if you want to target by a badge");
    	
}

function nycc_rides_ride_coordinators_email_submit($form, &$form_state) {

  	$arg1 = strtolower($form_state['values']['target']);
  	if (drupal_strlen($arg1) == 0)
    	$arg1 = "Leaders";
  	
  	$arg2 = strtolower($form_state['values']['level']);
  	if (drupal_strlen($arg2) == 0)
    	$arg2 = "All";
  	
	$arg3 = strtolower($form_state['values']['badge']);

  	$subject = $form_state['values']['subject'];
  	$body = $form_state['values']['message'];
	//  $from = $form_state['values']['from'];  - 11/25/2015 changing to reduce bounced emails whend sender is aol.com or yahoo.com
  	$replyTo = $form_state['values']['from'];
  	$from = "no-reply@nycc.org";


  	// todo: remove
  	//drupal_set_message("No emails sent yet. This feature is still under construction. Coming soon!");
  	//drupal_set_message("Note that messages will be sent to " . $form_state['values']['level'] . " level $arg1");


  	$r2 = time();
  	$r1 = $r2 - 365 * 24 * 60 * 60;

  	if ($arg1 == 'riders') {
    	$sql =<<<EOS

			SELECT DISTINCT mail FROM users u, users_roles ur, node p, node r, content_type_rides ctr, content_field_ride_current_riders cfrcr, content_type_profile cp  
      		WHERE u.uid = cfrcr.field_ride_current_riders_uid AND ur.uid = u.uid AND ur.rid = 3 AND ctr.nid = cfrcr.nid AND ctr.vid = cfrcr.vid 
      		AND r.type = 'rides' AND cfrcr.nid = r.nid AND cfrcr.vid = r.vid 
        	AND p.uid=u.uid AND p.type = 'profile'  
            AND cp.nid=p.nid AND cp.vid=p.vid AND cp.field_email_list_flag_value = 'Receive Club Emails'
      		AND	UNIX_TIMESTAMP(CONCAT(DATE_FORMAT(ctr.field_date_ride_first_value, "%Y-%m-%%d"), "T",
        		If(ctr.field_ride_start_time_select_value = "AM", ctr.field_ride_start_time_hour_value, 
        		IF(ctr.field_ride_start_time_hour_value < 12, ctr.field_ride_start_time_hour_value+12, ctr.field_ride_start_time_hour_value-12)),":",
        		ctr.field_ride_start_time_min_value,":00"))
      		BETWEEN $r1 AND $r2
EOS;

	} 
	else if ($arg1 == 'leaders'){

    	$sql =<<<EOS2

		SELECT DISTINCT mail FROM users u, node p, node r, content_field_ride_leaders cfrl, content_type_rides ctr, content_type_profile cp 
		WHERE u.uid = p.uid  AND cfrl.field_ride_leaders_nid = p.nid AND p.type = 'profile' AND r.type = 'rides' AND ctr.nid = r.nid 
		AND ctr.vid = r.vid AND r.nid = cfrl.nid AND r.vid = cfrl.vid AND  ctr.field_ride_status_value = 'Approved'
        AND cp.nid=p.nid AND cp.vid=p.vid AND cp.field_email_list_flag_value = 'Receive Club Emails'
		AND UNIX_TIMESTAMP(CONCAT( DATE_FORMAT(ctr.field_date_ride_first_value, "%Y-%m-%%d"), "T",
		If(ctr.field_ride_start_time_select_value = "AM", ctr.field_ride_start_time_hour_value, 
		IF(ctr.field_ride_start_time_hour_value < 12, ctr.field_ride_start_time_hour_value+12, ctr.field_ride_start_time_hour_value-12)),
			":", ctr.field_ride_start_time_min_value, ":00"))
		BETWEEN $r1 AND $r2
EOS2;

	} else {
  		$sql ="select distinct u.mail as mail 
  			from users u join user_badges_user ub on u.uid=ub.uid 
  			join user_badges_badges b on ub.bid = b.bid 
  			join node n on n.uid=u.uid 
  			join content_type_profile cp on cp.nid=n.nid AND cp.vid=n.vid	
  			where cp.field_email_list_flag_value = 'Receive Club Emails' and b.name='" . $arg3  . "';";  	
	}	

// add filter
	if ($arg2 != "all" && $arg1 != "by badge")
    	$sql .= " AND ctr.field_ride_select_level_value = '$arg2' ";
	else if ($arg1 != "by badge")
    	$sql .= " AND ctr.field_ride_select_level_value IN ('a', 'b', 'c') ";

//  dpm(array('sql' => $sql));
  // for testing and logging
  $emails = array();

  $q = db_query($sql);
  $cnt = 0;
  while ($row = db_fetch_object($q)) {
    $emails[] = t($row->mail);
    $message = array(
      'to' => t($row->mail),
      'subject' => t($subject),
      'body' => $body,
      'headers' => array('From' => t($from), 'Reply-To' => t($replyTo)),
    );
    //dpm($message);
    drupal_mail_send($message);
    $cnt++;
  }
  drupal_set_message("$cnt emails sent.");
  watchdog('nycc', "%cnt emails sent with subject '%subject' from '%from' with body '%body' to: %emails", array('%cnt' => $cnt, '%subject' => t($subject), '%from' => t($from), '%body' => t($body), '%emails' => implode(', ', $emails)), WATCHDOG_INFO);
  //dpm($emails);

}
//
//function nycc_rides_output_ride_coordinators_email_block() {
//	//TODO: add SIG Graduate Links
//  $output = "";
//  $output .= "<p>Send an email blast to ride leaders and club members based on riding level.</p>";
//  $output .= "<ul>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/Leaders/All' class='nycc-button-wrapper'>A/B/C-Level Leaders</a></li>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/Leaders/A' class='nycc-button-wrapper'>A-Level Leaders</a></li>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/Leaders/B' class='nycc-button-wrapper'>B-Level Leaders</a></li>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/Leaders/C' class='nycc-button-wrapper'>C-Level Leaders</a></li>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/Riders/All' class='nycc-button-wrapper'>A/B/C-Level Riders</a></li>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/Riders/A' class='nycc-button-wrapper'>A-Level Riders</a></li>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/Riders/B' class='nycc-button-wrapper'>B-Level Riders</a></li>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/Riders/C' class='nycc-button-wrapper'>C-Level Riders</a></li>";
//  $output .= "<li><a href='/nycc-ride-coordinators-email/By_Badge' class='nycc-button-wrapper'>By Assigned Badge</a></li>";  
//  $output .= "</ul>";
//  return $output;
//}
//
///**
// * Implementation of hook_block().
// */
//function nycc_rides_block($op = 'list', $delta = 0, $edit = array()) {
//  switch ($op) {
//    case 'list':
//       $blocks[0]['info'] = t('Ride Coordinators Email');
//       $blocks[0]['cache'] = BLOCK_CACHE_PER_PAGE;
//      return $blocks;
//    case 'configure':
//      return $form;
//    case 'save':
//      break;
//    case 'view':
//       if ($delta == 0) {
//         $block['subject'] = t('Ride Coordinators Email');
//         $block['content'] = nycc_rc_output_ride_coordinators_email_block();
//       }
//      return $block;
//  }
//}
