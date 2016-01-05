REPLACE INTO $targetdb`field_data_comment_body`
(`entity_type`,
`bundle`,
`deleted`,
`entity_id`,
`revision_id`,
`language`,
`delta`,
`comment_body_value`,
`comment_body_format`)
SELECT 
  'comment',
  CONCAT('comment_node_', n.type),
  c.status,
  c.nid, 
  n.vid, 
  'und', 
  0, 
  c.comment, 
  c.format 
FROM $sourcedb`comments` c INNER JOIN $sourcedb`node` n ON c.nid = n.nid;
