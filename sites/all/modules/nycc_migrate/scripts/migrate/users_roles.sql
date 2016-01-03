REPLACE INTO $targetdb`users_roles`
(`uid`,
`rid`)
SELECT `users_roles`.`uid`,
    `users_roles`.`rid`
FROM $sourcedb`users_roles`;
