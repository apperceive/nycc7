REPLACE INTO $targetdb`node`
(`nid`,
`vid`,
`type`,
`language`,
`title`,
`uid`,
`status`,
`created`,
`changed`,
`comment`,
`promote`,
`sticky`,
`tnid`,
`translate`)
SELECT `node`.`nid`,
    `node`.`vid`,
    `node`.`type`,
    'und',
    `node`.`title`,
    `node`.`uid`,
    `node`.`status`,
    `node`.`created`,
    `node`.`changed`,
    `node`.`comment`,
    `node`.`promote`,
    `node`.`sticky`,
    `node`.`tnid`,
    `node`.`translate`
FROM $sourcedb`node` WHERE LENGTH(TRIM(`node`.`type`)) > 0 AND NOT (`node`.`type` IN ('board_members', 'ride', 'product'))
