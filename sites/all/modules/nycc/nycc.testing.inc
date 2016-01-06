<?php
function nycc_get_test_data($lang_code='und') {
  $return_objects = array();

//FALSE CONDITIONS  
  $return_objects[] = (object) ['desc' => 'Field is null' ,
            'to_test' =>null,
            'expected_return'=>FALSE,
            'expected_return_value'=>array(0=>array('value'=>'')),
            'expected_return_text_value'=>'',
            ];
            
  $return_objects[] = (object) ['desc' => 'Field is empty array' ,
                                'to_test' => array(),
                                'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',                                
                                ];
  
  $return_objects[] = (object) ['desc' => 'Field is array but has no #value, #default_value, or und elements' ,
                                'to_test' => array('test'=>1),
                                'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with empty und element' ,
                                'to_test' => array($lang_code=>null),
                                'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  
  $return_objects[] = (object) ['desc' => 'Field is array with und element but null for index 0' ,
                                 'to_test' => array($lang_code=>array(0=>null)),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with und element but empty array for index 0' ,
                                 'to_test' => array($lang_code=>array(0=>array())),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with und element but no value element in index 0' ,
                                 'to_test' => array($lang_code=>array(0=>array('test'=>1))),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with und element but an empty array for value' ,
                                 'to_test' => array($lang_code=>array(0=>array())),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with und element but value=>null' ,
                                 'to_test' => array($lang_code=>array(0=>array('value'=>null))),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with und element and value=> empty array' ,
                                 'to_test' => array($lang_code=>array(0=>array('value'=>array()))),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with string' ,
                                 'to_test' => array('uid'=>'1'),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with int' ,
                                 'to_test' => array('uid'=>1),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with object' ,
                                 'to_test' => array('obj'=> (object) ['property' => 'Here we go']),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>'')),
                                'expected_return_text_value'=>'',  
                                ];  
  //TRUE CONDITIONS
  $return_objects[] = (object) ['desc' => 'Field is  object' ,
                                 'to_test' => (object) ['property' => 'Here we go'],
                                 'expected_return' => TRUE,
                                'expected_return_value'=>array(0=>array('value'=> (object) ['property' => 'Here we go'])),
                                'expected_return_text_value'=>'',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is string' ,
                                 'to_test' => 'This is a string',
                                 'expected_return' => TRUE,
                                'expected_return_value'=>array(0=>array('value'=>'This is a string')),
                                'expected_return_text_value'=>'This is a string',  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is int' ,
                                 'to_test' => '12345',
                                 'expected_return' => TRUE,
                                'expected_return_value'=>array(0=>array('value'=>12345)),
                                'expected_return_text_value'=>12345,  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is single int value stored in und[0][value]' ,
                                 'to_test' => array($lang_code=>array(0=>array('value'=>12345))),
                                 'expected_return' => TRUE,
                                'expected_return_value'=>array(0=>array('value'=>12345)),
                                'expected_return_text_value'=>12345,  
                                ]; 
  $return_objects[] = (object) ['desc' => 'Field is single string value stored in und[0][value]' ,
                                 'to_test' => array($lang_code=>array(0=>array('value'=>'this is a string'))),
                                 'expected_return' => TRUE,
                                'expected_return_value'=>array(0=>array('value'=>'this is a string')),
                                'expected_return_text_value'=>'this is a string',  
                                ]; 
  $return_objects[] = (object) ['desc' => 'Field is multi string value stored in und[0][value]' ,
                                 'to_test' => array($lang_code=>array(0=>array('value'=>'this is a string'), 1=>array('value'=>'this is another string'), 2=>array('value'=>'this is yet another string'))),
                                 'expected_return' => TRUE,
                                'expected_return_value'=>array(0=>array('value'=>'this is a string'), 1=>array('value'=>'this is another string'), 2=>array('value'=>'this is yet another string')),
                                'expected_return_text_value'=>'',  
                                ]; 
  $return_objects[] = (object) ['desc' => 'Field is array with #value key and int value' ,
                                 'to_test' => array('#value'=>1),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>1)),
                                'expected_return_text_value'=>1,  
                                ];
  $return_objects[] = (object) ['desc' => 'Field is array with #default_value key and int value' ,
                                 'to_test' => array('#default_value'=>1),
                                 'expected_return' => FALSE,
                                'expected_return_value'=>array(0=>array('value'=>1)),
                                'expected_return_text_value'=>1,  
                                ];
    
    
  return $return_objects;
}
function nycc_field_has_value_test($verbose = FALSE) {
 //function to test all combinations of values to ensure it's correctly trapping all situations
 $test_data = nycc_get_test_data();
 
 $final_value = true;
 foreach($test_data as $test){
  $test_name = $test->desc;
  if(nycc_field_has_value($test->to_test) == $test->expected_return) {
    if ($verbose) dpm("Testing $test_name ... passed");  
  } else {
    $final_value = false;
    if ($verbose)  dpm("ERROR: Testing $test_name ... failed");  
  }
 }
 if ($verbose) {
  if($final_value) {
    dpm("Testing complete. returned: True");
  } else {
    dpm("Testing complete. returned: False");
  }
 }
 return $final_value;
}

function nycc_field_get_text_value_test($verbose = FALSE) {
 //function to test all combinations of values to ensure it's correctly trapping all situations
 $test_data = nycc_get_test_data();
 
 $final_value = true;
 foreach($test_data as $test){
  $test_name = $test->desc;
  if(nycc_field_get_text_value($test->to_test) == $test->expected_return_text_value) {
    if ($verbose) dpm("Testing $test_name ... passed");  
  } else {
    $final_value = false;
    if ($verbose)  dpm("ERROR: Testing $test_name ... failed");  
  }
 }
 if ($verbose) {
  if($final_value) {
    dpm("Testing complete. returned: True");
  } else {
    dpm("Testing complete. returned: False");
  }
 }
 return $final_value;
}


function nycc_field_get_value_test($verbose = FALSE) {
 //function to test all combinations of values to ensure it's correctly trapping all situations
 $test_data = nycc_get_test_data();
 
 $final_value = true;
 foreach($test_data as $test){
  $test_name = $test->desc;
  if(nycc_field_get_value($test->to_test) == $test->expected_return_value) {
    if ($verbose) dpm("Testing $test_name ... passed");  
  } else {
    $final_value = false;
    if ($verbose)  dpm("ERROR: Testing $test_name ... failed");  
  }
 }
 if ($verbose) {
  if($final_value) {
    dpm("Testing complete. returned: True");
  } else {
    dpm("Testing complete. returned: False");
  }
 }
 return $final_value;
}


