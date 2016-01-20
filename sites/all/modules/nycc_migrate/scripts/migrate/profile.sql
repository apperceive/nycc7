REPLACE INTO $targetdb`profile` (`type`,`uid`,`label`,`created`,`changed`)
SELECT 'profile', `users`.`uid`, 'Profile', `node`.`created`, `node`.`changed` FROM $sourcedb`users` INNER JOIN $sourcedb`node` ON `node`.uid = `users`.uid WHERE uid > 0 AND `node`.type = 'profile'; 
