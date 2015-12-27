REPLACE INTO `role`
(`rid`,
`name`,
`weight`)
SELECT `role`.`rid`,
    `role`.`name`,
    0
FROM `d6test`.`role`;