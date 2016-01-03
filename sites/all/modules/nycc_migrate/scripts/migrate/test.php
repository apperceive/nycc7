<?php

// used for testing drush commands

drush_print("drush test - simple test to load user 1 and display name");

$u = user_load(1);

if ($u)
  drush_print("user/1: " . $u->name);
else
  drush_print("unable to user_load(1)");

// environment exports: 
//$sourcedb = $_ENV["sourcedb"];

//drush_print("sourcedb: $sourcedb");

