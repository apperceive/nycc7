
REPLACE INTO `field_data_field_XXXXX`
(`entity_type`,
`bundle`,
`deleted`,
`entity_id`,
`revision_id`,
`language`,
`delta`,
`field_XXXXX_value`)
SELECT 
  "node",
  node.type,
  0,
  `content_field_XXXXX`.`nid`,
  `content_field_XXXXX`.`vid`,
  "und",
  0,
  `content_field_XXXXX`.`field_XXXXX_value`
FROM `d6test`.`content_field_XXXXX` INNER JOIN `node` ON (`d6test`.content_field_XXXXX.nid=node.nid AND `d6test`.content_field_XXXXX.vid=node.vid);
