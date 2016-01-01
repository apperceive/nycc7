REPLACE INTO `field_data_field_date`
(`entity_type`,
`bundle`,
`deleted`,
`entity_id`,
`revision_id`,
`language`,
`delta`,
`field_date_value`)
SELECT 
  "node",
  node.type,
  0,
  `content_field_date`.`nid`,
  `content_field_date`.`vid`,
  "und",
  `content_field_date`.`delta`,
  `content_field_date`.`field_date_value`
FROM `d6test`.`content_field_date` INNER JOIN `node` ON (`d6test`.`content_field_date`.nid=node.nid AND `d6test`.`content_field_date`.vid=node.vid);


