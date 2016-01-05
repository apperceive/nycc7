REPLACE INTO $targetdb`users` (
`uid`,
`name`,
`pass`,
`mail`,
`theme`,
`signature`,
`signature_format`,
`created`,
`access`,
`login`,
`status`,
`timezone`,
`language`,
`init`,
`data`,
`picture`
)
SELECT 
`users`.`uid`,
`users`.`name`,
`users`.`pass`,
`users`.`mail`,
`users`.`theme`,
`users`.`signature`,
`users`.`signature_format`,
`users`.`created`,
`users`.`access`,
`users`.`login`,
`users`.`status`,
`users`.`timezone_name`,
`users`.`language`,
`users`.`init`,
`users`.`data`,
0
FROM $sourcedb`users`
WHERE (status > 0) AND (uid > 1) /*AND ((UNIX_TIMESTAMP()-access) > 5*365*24*60*60)*/
;
