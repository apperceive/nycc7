replace INTO `node_revision`
(`nid`,
`vid`,
`uid`,
`title`,
`log`,
`timestamp`,
`status`,
`comment`,
`promote`,
`sticky`)
SELECT `node_revisions`.`nid`,
    `node_revisions`.`vid`,
    `node_revisions`.`uid`,
    `node_revisions`.`title`,
    `node_revisions`.`log`,
    `node_revisions`.`timestamp`,
    `node`.`status`,
    `node`.`comment`,
    `node`.`promote`,
    `node`.`sticky`
FROM `d6test`.`node_revisions` INNER JOIN `node` ON node_revisions.nid=node.nid AND node_revisions.vid=node.vid;


