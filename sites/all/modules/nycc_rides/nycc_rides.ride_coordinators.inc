<?php
/*
TODO:
1) Need to fix Profile table since 2 profiles exit - _nycc_rides_get_ride_coordinator_info()
2) //TODO need to fix this QUERY. Are we pulling the right profile? - nycc_rides_ride_coordinators
*/
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//Ride Coordinator RELATED FUNCTIONS
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
function nycc_rides_get_ride_coordinator_email($ride) {
  $ride = nycc_rides_get_ride_object($ride);
  if (isset($ride)){
    $level = nycc_field_get_property_text_value($ride, 'field_ride_select_level');
    if ($level)
      return strtolower($level) . '-rides@nycc.org';
  }
}

function nycc_rides_get_ride_coordinator_emails($data) {
  return _nycc_rides_get_ride_coordinator_info($data, 'mail');
}


function nycc_rides_get_ride_coordinator_names($data) {
  return _nycc_rides_get_ride_coordinator_info($data, 'realname');
}

//function _nycc_rides_get_ride_coordinator_info($data, $field_to_get) {
//  if(is_string($data) && strtolower($data) == "all") {
//    $level = "%";
//  } else {  
//    if (is_object($data) && $data->type && ($data->type == 'rides')) {
//      $level_array = nycc_field_get_value($data->field_ride_select_level);
//      $level=$level_array[0]['value'];
//    }
//    else if (is_numeric($data)) {
//      $ride = node_load($data);
//    
//      if ($ride){
//        $level_array = nycc_field_get_value($ride->field_ride_select_level);
//        $level=$level_array[0]['value'];
//      }
//    }
//    else if (strlen($data) == 1 && in_array(strtolower($data), array('a', 'b', 'c'))) {
//      $level = $data;
//    } else {
//      return "";
//    }
//   $level .= "-ride" ;
//  }
//
//  //TODO: Need to fix Profile table since 2 profiles exist or each user. bringing back old values
//  $sql = "SELECT DISTINCT u.mail, rn.realname,c.field_ride_coordinator_value  FROM {users} u  join {profile} p on p.uid=u.uid join {field_data_field_ride_coordinator} c on c.entity_id = p.pid join {realname} rn on u.uid=rn.uid WHERE  c.field_ride_coordinator_value like :level";
//
//  $q = db_query($sql, array(':level'=>$level));
//  $ret="";
//  foreach($q as $r) {
//    if (drupal_strlen($ret))
//      $ret .= ",";
//      $ret .= $r->{$field_to_get};
//
//  }
//  return $ret;
//}


function _nycc_rides_get_ride_coordinator_info($data, $field_to_get) {
  if(is_string($data) && strtolower($data) == "all") {
    $level = "all";

  } else {  
    if (is_object($data) && $data->type && ($data->type == 'rides')) {
      $level = nycc_field_get_property_text_value($data, 'field_ride_select_level');
    }
    else if (is_numeric($data)) {
      $ride = node_load($data);
    
      if ($ride){
        $level = nycc_field_get_property_text_value($data, 'field_ride_select_level');
      }
    }
    else if (strlen($data) == 1 && in_array(strtolower($data), array('a', 'b', 'c'))) {
      $level = $data;
    } else {
      return "";
    }
    
  }
  $ret = "";  
  $all_uids = array_unique(call_user_func_array('array_merge', _nycc_rides_get_ride_coordinator_uids($level)));;
  foreach(user_load_multiple($all_uids) as $user) {
    if (drupal_strlen($ret)) {
      $ret .= ",";
    }
    
    $ret .= $user->{$field_to_get};    
  }
  return $ret;
}

function _nycc_rides_get_ride_coordinator_uids($level = 'all') {
  if ($level == 'all') {
    return array('a'=> array_filter( array_values(variable_get('nycc_rides_a-ride-coordinators'))),
                 'b'=> array_filter( array_values(variable_get('nycc_rides_b-ride-coordinators'))),
                 'c'=> array_filter( array_values(variable_get('nycc_rides_c-ride-coordinators'))),
                );
    
  } elseif (in_array(strtolower($level), array('a', 'b', 'c'))) {
    $var_name = 'nycc_rides_' . $level . '-ride-coordinators';
    return array($level => array_filter( array_values(variable_get($var_name))));
  }
  return array();
}



function nycc_rides_ride_coordinators() {
  $coordinators = array();
  
  //$all_uids = array_unique(call_user_func_array('array_merge', ));

  foreach(_nycc_rides_get_ride_coordinator_uids() as $level => $level_rc) {
    foreach(user_load_multiple($level_rc) as $user) {
      $coordinators[] = array(
                           'level'=>$level,
                           'uid'=>$user->uid,
                           'name'=>nycc_get_user_full_name($user),
                           'mail'=>$user->mail,
                           'phone'=>nycc_format_phone_no(nycc_get_user_phone($user, false))
                           );
       
    }
  }

  
  //$sql = 'SELECT c.field_ride_coordinator_value AS level, n.title as name, p.field_phone_value as phone, u.mail, u.uid FROM {content_field_ride_coordinator} c, {content_type_profile} p, {node} n, {users} u WHERE n.nid=c.nid AND p.nid=n.nid AND u.uid=n.uid AND field_ride_coordinator_value IS NOT NULL ORDER BY 1';
  
  //TODO need to fix this QUERY. Are we pulling the right profile?
//  $sql =<<<EOS
//SELECT DISTINCT u.uid, u.mail, rn.realname as name,c.field_ride_coordinator_value as level, p.field_phone_value AS phone
//FROM {users} u  
//join {profile} pr on pr.uid=u.uid 
//join {field_data_field_ride_coordinator} c on c.entity_id = pr.pid 
//join {realname} rn on u.uid=rn.uid
//join {field_data_field_phone} p on p.entity_id = pr.pid
//ORDER BY 1;
//EOS;
//  $results = db_query($sql);
//  foreach ($results as $coordinator) {
//    $coordinator->level = strtoupper(substr($coordinator->level,0,1)); // lcfirst($coordinator->level);
//    $coordinators[] = $coordinator;
//  }
  return $coordinators;
}

function nycc_rides_output_ride_coordinators() {
  $output = "<div class='ride-coordinators'>";
  $output .= "<label>". t("Ride Coordinators:") ."</label>";
  $coordinators = nycc_rides_ride_coordinators();
  foreach ($coordinators as $coordinator) {
    $level = $coordinator['level'];
    $uid = $coordinator['uid'];
    $name = $coordinator['name'];
    $phone = $coordinator['phone'];
    
    $output .= "<div class='coordinator-$level-rides'>$level-rides: <a href='/user/$uid' title='View profile in another tab or window...' target=_blank'>$name</a>,<br/>" .
      "<a href='tel:$phone'>$phone</a><br/>" .
      "<a title='Send email...' href=mailto:$level-rides@nycc.org>$level-rides@nycc.org</a></div>";
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
  	$arg1 = "leaders";

	$arg2 = arg(2);
	if (drupal_strlen($arg2) == 0)
  	$arg2 = "all";

	if (! user_access('emails riders and leaders', $user)){
    	// access denied
    	drupal_set_message("You must be an have the permission to 'email riders and leaders' to view this page.");
			return $form;
  }
	    	//nycc_turn_off_journal_form('nycc_ride_coordinators_email');
	$badge_names = array();
	$q = db_query("select 0 as bid, '' as name UNION  select bid, name from {user_badges_badges} order by name;");
  foreach($q as $row) {
		$badge_names[$row->bid] = t($row->name);
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
		'#options' => array(
                        "leaders" => "Leaders",
                        "riders" => "Riders",
                        "riders_and_leaders" => "Riders & Leaders",
                        "by_badge" => "By Badge"),
		'#description' => "Send e-mail to ride leaders or riders (over the past 12 months) or based on assigned badge.",
	);

	$form['level'] = array(
		'#type' => 'select',
		'#required' => true,
		'#default_value' => $arg2,
		'#title' => t("Ride Level"),
		'#options' => array("all" => "All", "A" => "A", "B" => "B", "C" => "C"),
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
		'#type' => 'text_format',
		'#required' => true,
    '#format' => '1',
		'#title' => t("Body"),
		'#rows' => 20,
		'#description' => "Enter the text of your message here. Required",
	);

	$form['button'] = array(
		'#type' => 'submit',
		'#value' => t('Send'),
	);
	return($form);
}

function nycc_rides_ride_coordinators_email_validate($form, &$form_state) {
  dpm($form_state);
	if (!valid_email_address($form_state['values']['from']))
		form_set_error('from', "Please enter a valid e-mail address.");
		
	if ($form_state['values']['target'] == 'by_badge' &&  $form_state['values']['badge'] == 0)
		form_set_error('badge', "you must select a badge if you want to target by badge");

		
	if ($form_state['values']['target'] != 'by_badge' &&  $form_state['values']['badge'] != 0)
		form_set_error('target', "you must select 'By Badge' as your target group if you want to target by a badge");
  
}

function nycc_rides_ride_coordinators_email_submit($form, &$form_state) {
  dpm($form_state);

  $target = strtolower($form_state['values']['target']);
  if (drupal_strlen($target) == 0)
    $target = "leaders";
  	
	$level = strtolower($form_state['values']['level']);
	if (drupal_strlen($level) == 0)
  	$level = "all";
  	
  $badge_id = strtolower($form_state['values']['badge']);
  
  $arg1 =  ($target == 'by_badge' ) ? $badge_id : $target;

  $subject = $form_state['values']['subject'];
  $body = $form_state['values']['message']['value'];
  $reply_to = $form_state['values']['from'];
  $from = "no-reply@nycc.org";

  $target_emails  = array_values(nycc_rides_get_members_by_activity($arg1, $level));

  $msg = array(
               'to'=>  nycc_mandrill_generate_recipient_list( $target_emails, 'bcc') ,
               'subject' => $subject,
               'html' => $body, 
               'from_email' => $from,
               'headers' => array('Reply-To' => $reply_to ),
      );
  try {
    if (nycc_mandrill_send_email($msg) ) {
      drupal_set_message(t('Your message has been sent to @count recipients', array('@count' => count($target_emails))));
    }
  } catch (Exception $e) {
    drupal_set_message(t('Error sending message: @msg', array('@msg' => $e->getMessage())));
  }
}

function nycc_rides_output_ride_coordinators_email_block() {
  $output = "";
  $output .= "<p>Send an email blast to ride leaders and club members based on riding level.</p>";
  $output .= "<ul>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/Leaders/All' class='nycc-button-wrapper'>A/B/C-Level Leaders</a></li>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/Leaders/A' class='nycc-button-wrapper'>A-Level Leaders</a></li>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/Leaders/B' class='nycc-button-wrapper'>B-Level Leaders</a></li>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/Leaders/C' class='nycc-button-wrapper'>C-Level Leaders</a></li>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/Riders/All' class='nycc-button-wrapper'>A/B/C-Level Riders</a></li>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/Riders/A' class='nycc-button-wrapper'>A-Level Riders</a></li>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/Riders/B' class='nycc-button-wrapper'>B-Level Riders</a></li>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/Riders/C' class='nycc-button-wrapper'>C-Level Riders</a></li>";
  $output .= "<li><a href='/nycc-ride-coordinators-email/By_Badge' class='nycc-button-wrapper'>By Assigned Badge</a></li>";  
  $output .= "</ul>";
  return $output;
}

/**
 * Implementation of hook_block().
 */
function nycc_rides_block_info() {
  //VALIDATED
  $blocks[0]['info'] = t('Ride Coordinator Email');
  $blocks[0]['cache'] = DRUPAL_NO_CACHE;
 
  return $blocks;
} 


function nycc_rides_block_view($delta = 0) {
  if ($delta == 0) {
    $block['subject'] = t('Ride Coordinators Email');
    $block['content'] = nycc_rides_output_ride_coordinators_email_block();
  }
  return $block;
}

