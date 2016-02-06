REPLACE INTO $targetdb`user_badges_user`
(`uid`,
`bid`,
`type`,
`userweight`)
SELECT `user_badges_user`.`uid`,
    `user_badges_user`.`bid`,
    `user_badges_user`.`type`,
    `user_badges_user`.`userweight`
FROM  $sourcedb`user_badges_user`;
