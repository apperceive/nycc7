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
	if(nycc_mandrill_signature_is_valid('nycc_rides/inbound')) {
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
      	$token = nycc_mandrill_parse_email($args['to_mail']);
      	
      	if ($email_debug ) watchdog('nycc_rides', "Ride Token is " .  $token, array(), WATCHDOG_NOTICE) ;
		$ride = nycc_rides_get_ride_from_token($token);
		if (isset($ride)) {
			if ($email_debug ) watchdog('nycc_rides', "Ride found is " .  $ride->title, array(), WATCHDOG_NOTICE) ;
			if(nycc_rides_can_approve_rides($args['from_email'])) {
				$ride_status = nycc_rides_get_ride_status($ride);
				if ($email_debug ) watchdog('nycc_rides', 'Ride Status for Ride: ' . $ride->nid . " is " .  $ride_status,
																		array(), WATCHDOG_NOTICE) ;	
				if($ride_status == 'Revised' || $ride_status == 'Submitted') {
					if ($email_debug ) watchdog('nycc_rides', 'Ride: ' . $ride->nid . ' needs approval', array(), WATCHDOG_NOTICE) ;

					if (_nycc_rides_has_approval($args['text']['value'])) {
            nycc_rides_update_ride_status($ride, 'Approved'); 
						watchdog('nycc_rides', "Ride: " . $ride->nid . " is Approved via email by: " . $args['from_email'],
										 array(), WATCHDOG_NOTICE) ;

					} else { //is not approval
						if ($email_debug ) watchdog('nycc_rides', 'Not Approving ride: ' . $ride->nid . "Start of Message is: " . $startOfMsg ,
																				array(), WATCHDOG_NOTICE) ;
						nycc_mandrill_send_msg($args['from_email'], 
							'webmaster@nycc.org', 
							"NYCC Ride: " . $ride->title . " will not be approved" ,
							"Hello, The ride listed above will not be approved because we did not see the word Approved at the start of your reply. If you feel that this is a error, please email the NYCC Webmaster"
						);
						
					} //end if should be approved
				} else { //if ride isn't in a submitted or revised state
					if ($email_debug ) watchdog('nycc_rides', 'Skipping approval of ride: ' . $ride->nid . " because it's already approved",
																			array(), WATCHDOG_WARNING) ;
						nycc_mandrill_send_msg($args['from_email'], 
							'webmaster@nycc.org', 
							"NYCC Ride: " . $ride->title . " was not be approved" ,
							"Hello, The ride listed above was not approved because the status is not Submitted or Revised. If you feel that this is a error, please email the NYCC Webmaster"
						);		
				} // end if ride is revised
			} else { //can not approve
				watchdog('nycc_rides', 'Unauthorised approval attempt from ' . $args['from_email'], array(), WATCHDOG_ERROR) ;
				nycc_mandrill_send_msg($args['from_email'], 
					'webmaster@nycc.org', 
					"Unable to process ride approval for Ride: " . $ride->title ,
					"Hello, We are unable to process your request to approve a ride because it was sent from an unauthorised email address. If you feel that this is a error, please email the NYCC Webmaster"
				);
					
			} // end if can approve
				
		} else { //unable to load ride from token
			watchdog('nycc_rides', 'Unable to load ride from token: ' . $token, array(), WATCHDOG_ERROR) ;
			nycc_mandrill_send_msg($args['from_email'], 
					'webmaster@nycc.org', 
					"Unable to process ride approval.",
					"Hello, We are unable to process your request to approve a ride because the ride could not be found. If you feel that this is a error, please email the NYCC Webmaster"
			);
		} //end if ride is loaded
    }//end for each
  } //end if valid signature
}


function _nycc_rides_has_approval($text) {
	return strtolower(substr(ltrim($text),0,7)) == 'approve' ;
}


function nycc_rides_get_ride_token_email($nid) {
  $ride = nycc_rides_get_ride_object($nid);
  if (!$ride) {
    watchdog("nycc_rides", "nycc_rides_get_ride_token_email - no NID was passed. Cancelling function.", array(), WATCHDOG_WARNING);
    return "";
  }
	$token = nycc_field_get_text_value($ride->field_ride_token);
	if (empty($token)) {
    watchdog("nycc_rides", "nycc_rides_get_ride_token_email - No approval token specofied for Ride: !nid. Cancelling function.", array('!nid' => $ride->nid), WATCHDOG_WARNING);
    return "";
	}
	
	$approval_domain = variable_get('nycc_rides_approval_email_domain', '');
	if ($approval_domain =='') {
		watchdog("nycc_rides", "nycc_rides_get_ride_token_email - No approval domain specified in nycc rides configuration page. Cancelling function.", array(), WATCHDOG_WARNING);
		return "";			
	}
	
	return valid_email_address($token . '@' . $approval_domain) ? $token . '@' . $approval_domain: '';
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



function _nycc_rides_random_text( $type = 'alnum', $length = 60 )
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
