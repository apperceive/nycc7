
REPLACE INTO `field_data_field_carousel_order`
(`entity_type`,
`bundle`,
`deleted`,
`entity_id`,
`revision_id`,
`language`,
`delta`,
`field_carousel_order_value`)
SELECT 
  "node",
  node.type,
  0,
  `content_field_carousel_order`.`nid`,
  `content_field_carousel_order`.`vid`,
  "und",
  0,
  `content_field_carousel_order`.`field_carousel_order_value`
FROM `d6test`.`content_field_carousel_order` INNER JOIN `node` ON (`d6test`.content_field_carousel_order.nid=node.nid AND `d6test`.content_field_carousel_order.vid=node.vid);
