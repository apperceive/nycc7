TRUNCATE $targetdb`field_data_body`;

REPLACE INTO $targetdb`field_data_body` (entity_type, bundle, deleted, entity_id, revision_id, language, delta, body_value, body_format) SELECT 'node', IF(LENGTH(TRIM(node.type)) > 0, node.type, 'page'), 0, node.nid, node.vid, 'und', 0, body, 5 FROM $sourcedb`node_revisions` INNER JOIN $sourcedb`node` ON ($sourcedb`node_revisions`.nid=node.nid AND $sourcedb`node_revisions`.vid=node.vid);

TRUNCATE $targetdb`field_revision_body`;

REPLACE INTO $targetdb`field_revision_body` (entity_type, bundle, deleted, entity_id, revision_id, language, delta, body_value, body_format) SELECT 'node', IF(LENGTH(TRIM(node.type)) > 0, node.type, 'page'), 0, node.nid, node.vid, 'und', 0, body, 5 FROM $sourcedb`node_revisions` INNER JOIN $sourcedb`node` ON ($sourcedb`node_revisions`.nid=node.nid AND $sourcedb`node_revisions`.vid=node.vid);
