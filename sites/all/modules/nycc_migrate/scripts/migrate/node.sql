REPLACE INTO `node`
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
    `node`.`language`,
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
FROM `d6test`.`node`;

