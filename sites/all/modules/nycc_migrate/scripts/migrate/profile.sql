REPLACE INTO $targetdb`profile` (`type`,`uid`,`label`,`created`,`changed`)
SELECT 'profile', `users`.`uid`, 'Profile', `users`.`created`, `users`.`changed`) FROM $sourcedb`users` WHERE uid > 0; 
