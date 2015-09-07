<?
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//Ride Coordinator RELATED FUNCTIONS
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

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
