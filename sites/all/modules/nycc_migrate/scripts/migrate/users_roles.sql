REPLACE INTO `users_roles`
(`uid`,
`rid`)
SELECT `users_roles`.`uid`,
    `users_roles`.`rid`
FROM `d6test`.`users_roles`;
