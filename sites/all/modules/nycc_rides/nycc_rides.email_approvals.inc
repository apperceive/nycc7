<?php
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//EMAIL APPROVAL RELATED FUNCTIONS
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

function nycc_rides_inbound_callback() {
	if (variable_get('nycc_rides_debug_to_log', '') == 1)
		$email_debug = true;
	else
		$email_debug = false;
  	
  	// Check signature.
	if(_nycc_rides_isValid_signature('nycc_rides/inbound')) {
		//iterate through the messages
    	foreach (json_decode($_POST['mandrill_events']) as $event) {
      		$args = array(
        		'from_email' => $event->msg->from_email ?: '',
        		'from_name' => $event->msg->from_name ?: '',
        		'to_mail' => $event->msg->email ?: '',
        		'subject' => $event->msg->subject ?: '',
        		'text' => array('value' => $event->msg->text ?: '', ),
        		'raw_msg' => array('value' => $event->msg->raw_msg ?: '', ),
      		);
      	// Add type & ID.
      	//$args += _nycc_rides_inbound_parse_address($args['to_mail']);
      	$token = _nycc_rides_parse_email($args['to_mail']);
      	
      	if ($email_debug ) watchdog('nycc_rides', "Ride Token is " .  $token, array(), WATCHDOG_NOTICE) ;
		$ride = nycc_rides_get_ride_from_token($token);
		if (isset($ride)) {
			if ($email_debug ) watchdog('nycc_rides', "Ride found is " .  $ride->title, array(), WATCHDOG_NOTICE) ;
			if(nycc_rides_can_approve_rides($args['from_email'])) {
				$ride_status = nycc_rides_get_ride_status($ride);
				if ($email_debug ) watchdog('nycc_rides', 'Ride Status for Ride: ' . $ride->nid . " is " .  $ride_status, array(), WATCHDOG_NOTICE) ;	
				if($ride_status == 'Revised' || $ride_status == 'Submitted') {
					if ($email_debug ) watchdog('nycc_rides', 'Ride: ' . $ride->nid . ' needs approval', array(), WATCHDOG_NOTICE) ;

					if (_nycc_rides_has_approval($args['text']['value'])) {
            nycc_rides_approve_ride($ride);
						watchdog('nycc_rides', "Ride: " . $ride->nid . " is Approved via email by: " . $args['from_email'], array(), WATCHDOG_NOTICE) ;

					} else { //is not approval
						if ($email_debug ) watchdog('nycc_rides', 'Not Approving ride: ' . $ride->nid . "Start of Message is: " . $startOfMsg , array(), WATCHDOG_NOTICE) ;
						nycc_rides_send_msg($args['from_email'], 
							'webmaster@nycc.org', 
							"NYCC Ride: " . $ride->title . " will not be approved" ,
							"Hello, The ride listed above will not be approved because we did not see the word Approved at the start of your reply. If you feel that this is a error, please email the NYCC Webmaster"
						);
						
					} //end if should be approved
				} else { //if ride isn't in a submitted or revised state
					if ($email_debug ) watchdog('nycc_rides', 'Skipping approval of ride: ' . $ride->nid . " because it's already approved", array(), WATCHDOG_WARNING) ;
						nycc_rides_send_msg($args['from_email'], 
							'webmaster@nycc.org', 
							"NYCC Ride: " . $ride->title . " was not be approved" ,
							"Hello, The ride listed above was not approved because the status is not Submitted or Revised. If you feel that this is a error, please email the NYCC Webmaster"
						);		
				} // end if ride is revised
			} else { //can not approve
				watchdog('nycc_rides', 'Unauthorised approval attempt from ' . $args['from_email'], array(), WATCHDOG_ERROR) ;
				nycc_rides_send_msg($args['from_email'], 
					'webmaster@nycc.org', 
					"Unable to process ride approval for Ride: " . $ride->title ,
					"Hello, We are unable to process your request to approve a ride because it was sent from an unauthorised email address. If you feel that this is a error, please email the NYCC Webmaster"
				);
					
			} // end if can approve
				
		} else { //unable to load ride from token
			watchdog('nycc_rides', 'Unable to load ride from token: ' . $token, array(), WATCHDOG_ERROR) ;
			nycc_rides_send_msg($args['from_email'], 
					'webmaster@nycc.org', 
					"Unable to process ride approval.",
					"Hello, We are unable to process your request to approve a ride because the ride could not be found. If you feel that this is a error, please email the NYCC Webmaster"
			);
		} //end if ride is loaded
    }//end for each
  }  else { // if email signature isn't valid.
    watchdog('nycc_rides', 'Failed signature validation calculated="%calculated", received="%received",
    md5(mandrill_events)="%md5", strlen(mandrill_events)=%length, post=<pre>@post</pre>', array(
      '%calculated' => print_r($calculated_signatures, TRUE),
      '%received' => $_SERVER['HTTP_X_MANDRILL_SIGNATURE'],
      '%length' => strlen($_POST['mandrill_events']),
      '%md5' => md5($_POST['mandrill_events']),
      '@post' => print_r($_POST, TRUE),
    ), WATCHDOG_WARNING);
  } //end if valid signature
}


function _nycc_rides_has_approval($text) {
	return strtolower(substr(ltrim($text),0,7)) == 'approve' ;
}



function nycc_rides_send_msg($to, $from, $subject, $body) {
	if (variable_get('nycc_rides_debug_to_log', '') == 1)
		$email_debug = true;
	else
		$email_debug = false;
    
	if ($email_debug ) watchdog('nycc_rides', 'nycc_rides_send_msg called', array(), WATCHDOG_NOTICE) ;
	$headers = array(
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
	);
	$msg = array(
			'id' => 'test_test',
			'from' => $from,
			'to' => $to,
			'subject' => $subject,
			'body' => $body,
			'headers' => $headers,
		);
	//watchdog('nycc_rides', '<pre>' .print_r( $msg, TRUE) . '</pre>');		
	drupal_mail_send($msg);
} 

function nycc_rides_can_approve_rides($sender='') {
	$approvers = explode(",",nycc_rides_get_ride_coordinator_emails("all"));
  array_push($approvers, "a-rides@nycc.org",  "b-rides@nycc.org",  "c-rides@nycc.org",  "vp-rides@nycc.org",  "webmaster@nycc.org");
  return in_array($sender, $approvers);
}


function nycc_rides_get_ride_from_token($token='') {
	if (variable_get('nycc_rides_debug_to_log', '') == 1)
		$email_debug = true;
	else
		$email_debug = false;
    
	if (empty($token)) {
    	return FALSE;
  	}
  $sql =" SELECT n.nid from node n inner join field_data_field_ride_token t on n.nid = t.entity_id and n.vid = t.revision_id where t.field_ride_token_value = '$token' limit 1";
  //watchdog('nycc_rides', 'SQL: ' .$sql);
  $q = db_query($sql);
  foreach($q as $row) {
    //watchdog('nycc_rides', 'Token: ' . $token . " returned: <pre>" . print_r($row) . '</pre>', array(), WATCHDOG_NOTICE) ;
		if ($email_debug ) watchdog('nycc_rides', 'Token: ' . $token . " returned node: " . $row->nid, array(), WATCHDOG_NOTICE) ;	
		$ride =  node_load($row->nid);
    if (is_object($ride)) {
      return $ride;
    } else {
      watchdog('nycc_rides', 'nycc_rides_get_ride_from_token: could not load ride:' . $row->nid . ' from token: ' . $token, array(), WATCHDOG_NOTICE) ;	
    }
  }    
  if ($email_debug ) watchdog('nycc_rides', 'Ride could not be found for Token: ' . $token, array(), WATCHDOG_NOTICE) ;	
}


function nycc_rides_email_alter(&$args) {
  // Cleanup message bellow. 

  // Delete signature.
  $text = explode('--', $args['text']['value']);
  $text = $text[0];

  // Delete quotes.
  $args['text']['value'] = preg_replace("/(?<=^|\n)>.*\r?\n|\r?\n>.*/", '', $text);
}



function _nycc_rides_parse_email($email = '') {
  $mail = explode('@', $email);
  return $mail[0] ?: '' ;
}


function _nycc_rides_isValid_signature($urlSuffix=''){
	if(!empty($urlSuffix) && !empty($_POST)) {
		$url = url($urlSuffix, array('absolute' => TRUE));	
		try {
			foreach (mandrill_get_api_object()->webhooks->getList() as $list) {
				if (($url == $list['url']) 
					&& (_nycc_rides_generate_signature($url, $list['auth_key'], $_POST) == $_SERVER['HTTP_X_MANDRILL_SIGNATURE']))
				{
					return true;
				}
			}
		} catch (Mandrill_Exception $e) {
			watchdog('nycc_rides', "_nycc_rides_isValid_signature generated exception: " . $e->getMessage(), array('url' => $url,'_POST' => $_POST),  WATCHDOG_WARNING) ;	
			return false;
		}
	} else {
		watchdog('nycc_rides', "_nycc_rides_isValid_signature. recieved invalid parameters " , array('url' => $url,'_POST' => $_POST),  WATCHDOG_WARNING) ;	
		return false;
	}
}


function _nycc_rides_generate_signature($url, $webhook_key, $params) {
/**
 * Generates a base64-encoded signature for a Mandrill webhook request.
 * Verbatim from http://help.mandrill.com/entries/23704122-Authenticating-webhook-requests
 *
 * @param string $url the webhook url
 * @param string $webhook_key the webhook's authentication key
 * @param array $params the request's POST parameters
 *
 * @return string
 */
	$signed_data = $url;
  	ksort($params);
  	foreach ($params as $key => $value) {
    	$signed_data .= $key;
    	$signed_data .= $value;
  	}
  	return base64_encode(hash_hmac('sha1', $signed_data, $webhook_key, TRUE));
}


function random_text( $type = 'alnum', $length = 60 )
{
	switch ( $type ) {
		case 'all':
			$pool = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_+-|}{][;:,<.>?';
			break;
		case 'alnum':
			$pool = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
			break;
		case 'alpha':
			$pool = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
			break;
		case 'hexdec':
			$pool = '0123456789abcdef';
			break;
		case 'numeric':
			$pool = '0123456789';
			break;
		case 'nozero':
			$pool = '123456789';
			break;
		case 'distinct':
			$pool = '2345679ACDEFHJKLMNPRSTUVWXYZ';
			break;
		case 'email':
			$pool = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!#$%&*+-=?^_{|}~';
			break;									

		default:
			$pool = (string) $type;
			break;
	}
	$crypto_rand_secure = function ( $min, $max ) {
		$range = $max - $min;
		if ( $range < 0 ) return $min; // not so random...
		$log    = log( $range, 2 );
		$bytes  = (int) ( $log / 8 ) + 1; // length in bytes
		$bits   = (int) $log + 1; // length in bits
		$filter = (int) ( 1 << $bits ) - 1; // set all lower bits to 1
		do {
			$rnd = hexdec( bin2hex( openssl_random_pseudo_bytes( $bytes ) ) );
			$rnd = $rnd & $filter; // discard irrelevant bits
		} while ( $rnd >= $range );
		return $min + $rnd;
	};

	$token = "";
	$max   = strlen( $pool );
	for ( $i = 0; $i < $length; $i++ ) {
		$token .= $pool[$crypto_rand_secure( 0, $max )];
	}
	return $token;
}
