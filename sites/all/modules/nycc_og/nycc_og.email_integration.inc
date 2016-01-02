<?php
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//EMAIL Integration RELATED FUNCTIONS
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


//OG Email Related Fuctions
function nycc_og_group_email_callback() {
  $bcc_webmaster = TRUE;
  if (variable_get('nycc_og_debug_to_log', '1') == 1) {
    $email_debug = TRUE;
  }
  else {
    $email_debug = FALSE;
  }

  if (! nycc_mandrill_signature_is_valid(variable_get('nycc_og_inbound_url', ''), $_POST)) {
    if ($email_debug ) watchdog('nycc_og', 'Invalid signature detected. Message will be discarded.', array(), WATCHDOG_NOTICE) ;
    return;
  }
  
  //iterate through the messages
  foreach (json_decode($_POST['mandrill_events']) as $event) {
    //watchdog('nycc_og', 'received event: <pre>@event</pre>',  array('@event' => print_r($event, TRUE) ), WATCHDOG_NOTICE) ;
    
    $args = nycc_mandrill_get_msg_details($event);
    watchdog('nycc_og', 'Args Are: <pre>@args</pre>',  array('@args' => print_r($args, TRUE) ), WATCHDOG_NOTICE) ;    
    
    if ( strcasecmp($args['to_mail'] , $args['from_email']) == 0) {
      //prevent loopbacks
      if ($email_debug ) watchdog('nycc_og',
                                  'Dicarding message. sender is same as recipient: @user_email to @grp_email',
                                  array('@user_email' => $args['from_email'], '@grp_email' => $args['to_mail']),
                                  WATCHDOG_NOTICE) ;
      return;
    }
    
    $grp_members = nycc_og_get_group_node_by_email($args['to_mail']);
      
    if (!isset($grp_members)) {
      if ($email_debug ) watchdog('nycc_og',
                                  'Dicarding message. Could not find group with email address: @grp_email', 
                                  array('@grp_email' => $args['to_mail']), WATCHDOG_WARNING) ;
                
      $msg = array(
          'to'=> mandrill_get_to($args['from_email']),
          'subject' => "Unable to process your Group Email sent to: " . $args['to_mail'],
          'html' => "Hello,<br/>We are unable to process your request to email this group because we could not find a group with that email address. <br/><br/>If you feel you're receiving this message in error, please contact the group captain.<br/>"
      );
      if ($bcc_webmaster) $msg['bcc_address'] = 'webmaster@nycc.org';
      nycc_mandrill_send_email($msg);
      return;
    }
    
    if (! nycc_og_can_email_group($args['to_mail'] , $args['from_email'])) {
      if ($email_debug ) watchdog('nycc_og', 'Dicarding message. @user_email can not email @grp_email' , 
                    array('@user_email' => $args['from_email'], '@grp_email' => $args['to_mail']), WATCHDOG_WARNING) ;
                    
      $msg = array(
          'to'=> mandrill_get_to($args['from_email']),
          'subject' => 'Unable to process your Group Email sent to: ' . $args['to_mail'],
          'html' => "Hello,<br/> We are unable to process your request to email this group because you are not a member of this group.<br/><br/>If you feel you're receiving this message in error, please contact the group captain.<br/>"
      );
      if ($bcc_webmaster) $msg['bcc_address'] = 'webmaster@nycc.org';
      nycc_mandrill_send_email($msg);                   
      return;      
    }
    
    $send_to = nycc_og_get_group_member_emails($args['to_mail'], $args['to_cc_list']);

    watchdog('nycc_og',
                    'Sending email with subject @subject to: <pre>@to</pre>', 
                    array('@to' => print_r($send_to, TRUE), '@subject' => $args['subject']), WATCHDOG_NOTICE) ;
    
    $message = array(
          'html' => $args['html'], 
          'subject' => $args['subject'],
          'from_email' => $args['to_mail'],
          'from_name' => $args['from_name'] . ' (via ' . variable_get('nycc_og_signing_domain', '') . ')',
          'to' => $send_to,
          'headers' => array('Reply-To' => $args['from_email']) ,
          'attachments' => $args['attachments'],
          'images' => $args['images']
    );    
    if(! nycc_mandrill_send_email ($message )) {
      watchdog('nycc_og',
                    'Error sedning email with subject @subject to: <pre>@to</pre>', 
                    array('@to' => print_r($send_to, TRUE), '@subject' => $args['subject'] ), WATCHDOG_WARNING) ;    
    }
  } // end for each
}



function nycc_og_can_email_group($grp_email, $sender_email) {
  return nycc_og_get_group_info_by_email(strtolower($grp_email), 'can_send', strtolower($sender_email));
}

function nycc_og_get_group_node_by_email($grp_email) {
  return nycc_og_get_group_info_by_email(strtolower($grp_email), 'group_id');
}

function nycc_og_get_group_member_emails($grp_email, $recipients = array()) {
  $addresses =  nycc_og_get_group_info_by_email(strtolower($grp_email), 'email_addresses');
  
  if(!empty($addresses)) {
    foreach($addresses as $ndx => $addr) {
      
      //watchdog('nycc_og', 'nycc_og_get_group_member_emails - comparing ' . $addr['email'] . ' to ' . $grp_email, array(),  WATCHDOG_NOTICE) ;
      
      if( strtolower($grp_email) != strtolower($addr['email'])
         && (in_array(strtolower($addr['email']), $recipients))) {
        unset($addresses[$ndx]);
        watchdog('nycc_og', 'nycc_og_get_group_member_emails - skipping ' .
                 $addr['email'] . ' because it was in the recipient list', array(),  WATCHDOG_NOTICE) ;
      } 
    }
  } else {
    dpm("no addresses returned");
  }
  return $addresses;
}

function nycc_og_allows_external_email($grp_email) {
  return nycc_og_get_group_info_by_email(strtolower($grp_email), 'allow_external');
}

function nycc_og_get_group_info_by_email($grp_email, $query_type, $sender_email=NULL) {
  $debug = 1;
  if (empty($grp_email)) {
    return null;
  }
  
  //$user_email = (isset($sender_email)) ? $sender_email : '%';
  
$sql = <<<EOS
select
  m.gid as gid,
  m.state as grp_state,
  n.title as grp_name,
  n.nid nid,
  n.vid as vid,
  r.name as role_name,
  u.name as user_name,
  rn.realname as realname,
  u.mail as mail,
  ge.field_group_email_value as grp_email,
  gle.field_group_leaders_email_value as leader_email,
  ae.field_group_accept_ext_email_value as accept_external_email
from og_membership m 
  join node n on n.nid= m.gid
  join og_users_roles ur on m.etid=ur.uid  AND ur.gid=m.gid
  join og_role r on r.rid=ur.rid
  join users u on u.uid=ur.uid
  join realname rn on u.uid=rn.uid
  LEFT join field_data_field_group_email ge on ge.entity_id=n.nid and ge.revision_id=n.vid 
  LEFT join field_data_field_group_leaders_email gle on gle.entity_id=n.nid and gle.revision_id=n.vid 
  LEFT join field_data_field_group_accept_ext_email ae on ae.entity_id=n.nid and ae.revision_id=n.vid 
where
  m.state=1
  
  AND (
    ge.field_group_email_value=:email
    OR gle.field_group_leaders_email_value=:email
  ) 
EOS;
 //and (u.mail like ':user_email' OR field_group_accept_ext_email_value=1)
 
 
  //$q = db_query($sql,  array(':email' => $grp_email, ':user_email' => $user_email));
  $q = db_query($sql,  array(':email' => $grp_email));
  
  if($q->rowCount() ==0) {
    watchdog('nycc_og', 'nycc_og_get_group_info_by_email sql  returned no data of group with email @email',
           array( '@email' => $grp_email), WATCHDOG_NOTICE) ;
    return null;
  }
  
  watchdog('nycc_og', 'nycc_og_get_group_info_by_email sql  returned @rows  rows for group with email @email',
           array('@rows' => $q->rowCount(), '@email' => $grp_email), WATCHDOG_NOTICE) ;
  
  switch ($query_type) {
    case "allow_external":
      watchdog('nycc_og', 'nycc_og_get_group_info_by_email - checking if group: @email accepts external emails',
           array('@email' => $grp_email), WATCHDOG_NOTICE) ;
      foreach($q as $records) {
      if ($debug >1)  watchdog('nycc_og', 'nycc_og_get_group_info_by_email - checking if group: @email accepts external emails. Value of accept_external_email is:@val',
           array('@email' => $grp_email, '@val' => $records->accept_external_email), WATCHDOG_NOTICE) ;
        if($records->accept_external_email==1) {
          return TRUE;
        } else {
          return FALSE;
        }
      }
      return FALSE;
      break;
      
    case "group_id":
      if ($debug >1) watchdog('nycc_og', 'nycc_og_get_group_info_by_email - Returning GID for group: @email',
           array('@email' => $grp_email), WATCHDOG_NOTICE) ;      
      foreach($q as $records) {
        if ($debug >1) watchdog('nycc_og', 'nycc_og_get_group_info_by_email -  GID for group: @email is : @gid',
           array('@email' => $grp_email, '@gid' => $records->gid), WATCHDOG_NOTICE) ;         
        return $records->gid;
      }
      return NULL;
      break;
    
    case "can_send":
      if ($debug >1) watchdog('nycc_og', 'nycc_og_get_group_info_by_email - Checking if user @user can email group: @email',
           array('@user' => $sender_email, '@email' => $grp_email), WATCHDOG_NOTICE) ;        
      //if the sender email wasn't specified then return false
      if(!isset($sender_email)) {
        return FALSE;
      }
            
      //if the group accepts external email then anyone can send
      foreach($q as $records) {
        if ($debug >1) watchdog('nycc_og', 'nycc_og_get_group_info_by_email - accept_external_email is: @allow and member email is: @mail',
           array('@allow' => $records->accept_external_email, '@mail' => $records->mail), WATCHDOG_NOTICE) ; 
        if($records->accept_external_email==1 || strcasecmp($records->mail, $sender_email) ==0) return TRUE;
      }
      return FALSE;
      break;
    
    
    case "email_addresses":
      $emails = array();
        if ($debug >1) watchdog('nycc_og', 'nycc_og_get_group_info_by_email - Returning email addresses for Group: @email.',
           array( '@email' => $grp_email), WATCHDOG_NOTICE) ;       
      foreach($q as $records) {
        $to_insert = array(
                          'email' => $records->mail,
                          'name' => $records->realname,
                          'type' => 'bcc'
                          );           
        //If email was sent to leaders then check if member is leader or captain. Otherwise, if email was sent to group address then return the member address
        if (strcasecmp($records->leader_email, $grp_email) == 0) {
          if ($debug >1) watchdog('nycc_og',
                   'nycc_og_get_group_info_by_email - @email is leader address. Checking if member @user is a leader or captain',
           array( '@email' => $grp_email, '@user' => $records->user_name), WATCHDOG_NOTICE) ;
          
          if (in_array($records->role_name, array('leader', 'captain'))) {
            if ($debug >1) watchdog('nycc_og',
                     'nycc_og_get_group_info_by_email - @user is a leader or captain in group @group',
                     array( '@group' => $records->grp_name, '@user' => $records->user_name), WATCHDOG_NOTICE) ;
            
            if ($debug >1) watchdog('nycc_og',
                     'nycc_og_get_group_info_by_email - <pre>@record</pre>',
                     array( '@record' => print_r($records, TRUE)), WATCHDOG_NOTICE) ;
            
            if (!in_array($to_insert, $emails)) {
              $emails[] =   $to_insert;
            }                     
          }
          else { //if 
            if ($debug >1) watchdog('nycc_og',
                     'nycc_og_get_group_info_by_email - Skipping @user. Not leader or captain in group @group',
                     array( '@group' => $records->grp_name, '@user' => $records->user_name), WATCHDOG_NOTICE) ;            
          }
        }
        elseif (strcasecmp($records->grp_email, $grp_email) ==0) {
          if ($debug >1) watchdog('nycc_og',
                   'nycc_og_get_group_info_by_email - @email is group address. adding @user to email',
                   array( '@email' => $grp_email, '@user' => $records->user_name), WATCHDOG_NOTICE) ;          
          
          if (!in_array($to_insert, $emails)) {
            $emails[] =   $to_insert;
          }
        }
      }
      
      
      $to_add = array(
                      'email' => $grp_email,
                      'name' => $grp_email,
                      'type' => 'to'
                      );
      //watchdog('nycc_og',
      //               'nycc_og_get_group_info_by_email - Adding <pre>@record</pre>',
      //               array( '@record' => print_r($to_add, TRUE)), WATCHDOG_NOTICE) ;
      $emails[] =$to_add;
      return $emails;
    
      break;
  }
}


//END OG Email Related functions
