REPLACE INTO $targetdb`history`
(`uid`,
`nid`,
`timestamp`)
SELECT `history`.`uid`,
    `history`.`nid`,
    `history`.`timestamp`
FROM $sourcedb`history`;

