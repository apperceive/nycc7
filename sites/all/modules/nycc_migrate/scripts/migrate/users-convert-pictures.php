<?php
  
  $debug = drush_get_option(array('sql'), FALSE);
  /*
  // don't do this, check for valid data instead - it appears that none of the 
  // clear out all user pictures values (fids)
  // old school: $q = db_query("UPDATE users SET picture = ''");
  if (!$debug)
    db_update('users')->fields(array('picture' => ''))->execute();
  */
  
  $no = drush_get_option(array('no'), FALSE);
  $filesdir = drush_get_option(array('filesdir'), 'sites/default/files');
  $subdir = drush_get_option(array('subdir'), 'pictures');
  // TODO: simplify this down to a single regex if possible
  $glob = drush_get_option(array('glob'), 'picture-');
  $extensions = drush_get_option(array('extensions'), 'jpg|png|gif|jpeg');
  $pattern = drush_get_option(array('pattern'), "~picture\-[0-9]+\.(jpg|png|gif|jpeg)~");
  
  $files = glob("$filesdir/$subdir/$glob");
  
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
    $pos = strpos($filepath, "/$subdir/");        // note: by defn of glob pos > 0
    $filename = substr($filepath, $pos+drupal_strlen("/$subdir/"));
    $uri = "public://$subdir/$filename";
    $fmuri = "$filesdir/$subdir/$filename";
    
    if (!preg_match($pattern, $filename)) {  // check file matches pattern 
      drush_print("Notice: file skipped because it does not match pattern - rm $filename");
      $rejected++;
      continue;
    }
    
    $pos = strrpos($filename, '.');
    if (!($pos >0)) {
      drush_print("Notice: file skipped because it has no extension or is 'dot' file - rm $filename");
      $rejected++;
      continue;
      
    }
    $patlen = drupal_strlen($pattern);
    $len = drupal_strlen($filename) - $patlen - $pos - 2;
    $uid = substr($filename, $patlen, $len);
    $acct = user_load($uid);
    
    if (!$acct) {
      drush_print("Notice: orphan file with no matching account - rm $filepath");
      $orphans++;
      continue;
    }
    
    if (!$acct->status) {
      drush_print("Notice: file matches blocked user - rm $filepath");
      $blocked++;
       continue;
    } // blocked
   
    $filesprocessed++;
    list($width, $height, $type, $attr) = getimagesize($filepath);
    $filesize = filesize($filepath);
    $imgtype = image_type_to_mime_type($type);
    $fid = 0;
    
    //drush_print("PROCESSING filename: $filename, uri: $uri, type: $type, imgtype: $imgtype, filesize: $filesize, attr: $attr");
    
    // is this file managed?  if so, get the first one - TODO: consider case where two rows ref same file
    // note that file_managed to filename is many to one in general, but not for user pics in markus dev d7
    $q = db_select('file_managed', 'fm')->fields('fm', array('uid', 'fid'))->condition('uri', $fmuri)->execute();
    //var_dump($r);
    // if file is managed, check for correct uid
    foreach ($q as $r) {
      $fid = $r->fid;
      if ($fid && ($r->uid !== $uid)) {
        //drush_print("UPDATE FILE_MANAGED: update file_managed SET uid = $uid WHERE fid = $fid;");
        if (!$no) 
          db_update('file_managed')->fields(array('uid' => $uid))->condition('fid', $fid)->execute();
        $fmupdated++;
      } // bad uid 
      $fmexisting++;
    } // $r
    
    if (!$fid) {
      // if file is not managed, make it so, insert file_managed record, assigned to acct 
      $time = time();
      //drush_print("INSERT FILE_MANAGED: uid: $uid, filename: $filename, uri: $uri, filemime: $imgtype, filesize: $filesize, timestamp: $time");
      if (!$no) {
        // test for existing primary key (uri)
        $q = db_select('file_managed', 'fm')->fields('fm', array('uid', 'fid'))->condition('uri', $fmuri)->execute();
        if (!$q)
          $fid = db_insert('file_managed')->fields(array('uid' => $uid, 'filename' => $filename, 'uri' => $uri, 'filemime' => $imgtype, 'filesize' => $filesize, 'timestamp' => $time))->execute();
        else
          $fid = $q->fetchField(1);
      }
      if ($no && !$fid) $fid = "TBD";  // for testing only
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
      //drush_print("UPDATE USER: $acct->name ($uid) with new picture: $fid : $filename because user.picture.fid: $acctfid while fid: $fid");
      if (!$no) {
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
  if ($debug) drush_print(var_export(get_defined_vars(),1));
?>
