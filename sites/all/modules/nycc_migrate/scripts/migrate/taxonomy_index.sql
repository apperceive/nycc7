REPLACE INTO $targetdb`taxonomy_index`
(`nid`,
`tid`,
`sticky`,
`created`)
SELECT `term_node`.`nid`,
    `term_node`.`tid`,
    0,
    UNIX_TIMESTAMP()
FROM $sourcedb`term_node`;
