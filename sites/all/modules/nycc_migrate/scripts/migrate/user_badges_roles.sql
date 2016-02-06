REPLACE INTO $targetdb`user_badges_roles`
(`rid`,
`bid`)
SELECT `user_badges_roles`.`rid`,
    `user_badges_roles`.`bid`
FROM $sourcedb`user_badges_roles`;
