REPLACE INTO $targetdb`profile` (`pid`, `type`, `uid`, `label`, `created`, `changed`)
SELECT  `node`.`nid`, 'profile', `node`.`uid`, 'Profile', `node`.`created`, `node`.`changed` FROM $sourcedb`users` INNER JOIN $sourcedb`node` ON `node`.uid = `users`.uid WHERE `users`.uid > 0 AND `node`.type = 'profile'; 
