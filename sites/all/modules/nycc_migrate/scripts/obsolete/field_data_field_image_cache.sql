REPLACE INTO `field_data_field_image_cache`
(`entity_type`,
`bundle`,
`deleted`,
`entity_id`,
`revision_id`,
`language`,
`delta`,
`field_image_cache_fid`,
`field_image_cache_alt`,
`field_image_cache_title`,
`field_image_cache_width`,
`field_image_cache_height`)
SELECT 
  "node",
  node.type,
  0,
  `content_field_image_cache`.`nid`,
  `content_field_image_cache`.`vid`,
  "und",
  `content_field_image_cache`.`delta`,
  `content_field_image_cache`.`field_image_cache_fid`,
  "",
  "",
  NULL,
  NULL
FROM `d6test`.`content_field_image_cache` INNER JOIN `node` ON (`d6test`.`content_field_image_cache`.nid=node.nid AND `d6test`.`content_field_image_cache`.vid=node.vid);

