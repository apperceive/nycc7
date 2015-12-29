
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


/* for simple fields: */


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
  `content_type_WWWWW`.`nid`,
  `content_type_WWWWW`.`vid`,
  "und",
  0,
  `content_type_WWWWW`.`field_XXXXX_value`
FROM `d6test`.`content_type_WWWWW` INNER JOIN `node` ON (`d6test`.content_type_WWWWW.nid=node.nid AND `d6test`.content_type_WWWWW.vid=node.vid);
