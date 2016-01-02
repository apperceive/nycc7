<?php

  $debug = false;
  
  /*
  // don't do this, check for valid data instead - it appears that none of the 
  // clear out all user pictures values (fids)
  // old school: $q = db_query("UPDATE users SET picture = ''");
  if (!$debug)
    db_update('users')->fields(array('picture' => ''))->execute();
  */
  
  
  $current = getcwd ();
  $pos = strpos($current, "/sites/");
  $has_sites_files = file_exists('sites/default/files');
  if ($pos === false && !$has_sites_files)
    die("You must be in a Drupal site");
  $base = $has_sites_files ? $current : substr($current, 0, $pos);
  $pattern = "$base/sites/default/files/pictures/picture*";
  $files = glob($pattern);
  // counters
  $filesfound = 0;
  $filesprocessed = 0;
  $usersupdated = 0;
  $fmupdated = 0;
  $fminserted = 0;
  $userspicok = 0;
  $rejected = 0;
  $orphans = 0;
  $blocked = 0;
  $fmexisting = 0;
  foreach ($files as $filepath) {
    $filesfound++;
    $filename = str_replace($base . '/sites/default/files/pictures/', '', $filepath);
    $uri = "public://pictures/$filename";
    $fmuri = "sites/default/files/pictures/$filename";
    if (!preg_match('~picture\-[0-9]+\.(jpg|png|gif)~', $filename)) {  // check file matches pattern 
      $rejected++;
      continue;
    }
      
    $uid = str_replace(array('picture-', '.jpg', '.png', '.gif'), '', $filename);
    $acct = user_load($uid);
    
    if (!$acct) {
      drush_print("ORPHAN FILE: no account - rm $filepath");
      $orphans++;
      continue;
    }
    
    if (!$acct->status) {
      drush_print("BLOCKED USER: rm $filepath");
      $blocked++;
       continue;
    } // blocked
   
    $filesprocessed++;
    list($width, $height, $type, $attr) = getimagesize($filepath);
    $filesize = filesize($filepath);
    $imgtype = image_type_to_mime_type($type);
    drush_print("PROCESSING filename: $filename, uri: $uri, type: $type, imgtype: $imgtype, filesize: $filesize, attr: $attr");
    $fid = 0;
    
    // is this file managed?  if so, get the first one - TODO: consider case where two rows ref same file
    // note that file_managed to filename is many to one in general, but not for user pics in markus dev d7
    $q = db_select('file_managed', 'fm')->fields('fm', array('uid', 'fid'))->condition('uri', $fmuri)->execute();
    //var_dump($r);
    // if file is managed, check for correct uid
    foreach ($q as $r) {
      $fid = $r->fid;
      if ($r->uid !== $uid) {
        drush_print("UPDATE FILE_MANAGED: update file_managed SET uid = $uid WHERE fid = $fid;");
        if (!$debug) 
          db_update('file_managed')->fields(array('uid' => $uid))->condition('fid', $fid)->execute();
        $fmupdated++;
      } // bad uid 
      $fmexisting++;
    } // $r
    
    if (!$fid) {
      // if file is not managed, make it so, insert file_managed record, assigned to acct 
      $time = time();
      drush_print("INSERT FILE_MANAGED: uid: $uid, filename: $filename, uri: $uri, filemime: $imgtype, filesize: $filesize, timestamp: $time");
      if (!$debug) {
        // test for existing primary key (uri)
        $q = db_select('file_managed', 'fm')->fields('fm', array('uid', 'fid'))->condition('uri', $fmuri)->execute();
        if (!$q)
          $fid = db_insert('file_managed')->fields(array('uid' => $uid, 'filename' => $filename, 'uri' => $uri, 'filemime' => $imgtype, 'filesize' => $filesize, 'timestamp' => $time))->execute();
      }
      if ($debug)
        $fid = "TBD";
      $fminserted++;
    }
    
    // insert failed?
    if (!$fid) {
      drush_print("ERROR: no fid!!! insert failed?");      // not expecting this, but let's test just in case
      continue;
    } // !$fid
    
    // ensure users picture is correct
    // reminder: while in the db, users.picture is an fid; in $acct user object, picture is an object of file_managed properties
    if (isset($acct->picture) && $fid && ($acct->picture->fid != $fid)) {
      $acctfid = $acct->picture->fid;
      drush_print("UPDATE USER: $acct->name ($uid) with new picture: $fid : $filename because user.picture.fid: $acctfid while fid: $fid");
      if (!$debug) {
        db_update('users')->fields(array('picture' => $fid))->condition('uid', $uid)->execute();
        user_load($uid);  // reload for changed picture data
        //user_save($acct);
      }
      $usersupdated++;
    } // update required
    else {
      $userspicok++;
    } // diff pic/fid
 
    
  } // files
    
  drush_print("STATS: Processed $filesprocessed out of $filesfound, rejected: $rejected, blocked: $blocked, orphans: $orphans.");
  drush_print("STATS: file_managed inserts: $fminserted, file_managed updates: $fmupdated, file_managed existing: $fmexisting.");
  drush_print("STATS: Users with correct picture fids: $userspicok, users updated: $usersupdated.");
  drush_print("STATS: Working Drupal site root was $base");
  drush_print("STATS: Debug mode is " . ($debug ? "ON, so no changes made!" : "OFF, all actions were executed."));
?>
