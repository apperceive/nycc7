REPLACE INTO $targetdb`role`
(`rid`,
`name`,
`weight`)
SELECT `role`.`rid`,
    `role`.`name`,
    0
FROM $sourcedb`role`;