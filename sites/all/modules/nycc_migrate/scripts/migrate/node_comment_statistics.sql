REPLACE INTO $targetdb`node_comment_statistics`
(`nid`,
`last_comment_timestamp`,
`last_comment_name`,
`last_comment_uid`,
`comment_count`,
`cid`)
SELECT `node_comment_statistics`.`nid`,
    `node_comment_statistics`.`last_comment_timestamp`,
    `node_comment_statistics`.`last_comment_name`,
    `node_comment_statistics`.`last_comment_uid`,
    `node_comment_statistics`.`comment_count`,
    0
FROM $sourcedb`node_comment_statistics`;
