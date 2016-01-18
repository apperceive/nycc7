REPLACE INTO $targetdb`file_managed`
(
`uid`,
`filename`,
`uri`)
SELECT
    `users`.`uid`,
    REPLACE(`users`.`picture`,'sites/default/files/',''),
    REPLACE(`users`.`picture`,'sites/default/files/','public://') 
FROM $sourcedb`users` WHERE picture LIKE 'sites/default/files/%'
