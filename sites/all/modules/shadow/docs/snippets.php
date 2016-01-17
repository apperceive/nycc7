<?php

// Test ShadowQuery
_shadow_load_classes();
$sql = <<<SQL
SELECT node_revisions.vid AS vid,
   node_revisions.title AS node_revisions_title,
   node_revisions.nid AS node_revisions_nid,
   node.created AS node_created
 FROM {node_revisions} node_revisions 
 LEFT JOIN {node} node ON node_revisions.nid = node.nid
 WHERE (node_revisions.vid>node.vid OR (node.status=0 AND (SELECT COUNT(vid) FROM {node_revisions} WHERE nid=node.nid)=1))
   ORDER BY node_created ASC
SQL;
$query = new ShadowQuery($sql);
dsm($query->getBaseTable());
dsm($query->getFilterFields());
dsm($query->getFilters());
dsm($query->getSortFields());



_shadow_load_classes();
$sql = 'SELECT *
FROM node n
JOIN term_node tn ON n.vid = tn.vid
JOIN term_data td ON tn.tid = td.tid
JOIN term_node tn2 ON n.vid = tn2.vid
WHERE td.name = \'test\'';
try {
  $query = new ShadowQuery($sql);
  dsm($query->getBaseTable());
  dsm($query->getJoins());
  dsm($query->getFilterFields());
  dsm($query->getFilterTypes());
  dsm($query->getFilters());
  dsm($query->getSortFields());
}
catch (Exception $e) {
  dsm($e->getMessage());
}


_shadow_load_classes();
$column = 'node.vid=term_node.vid{tn},term_node.tid=term_data.tid{td},term_data.name';
try {
  $column = new ShadowColumn($column);
  dsm($column->getTableName());
  dsm($column->getFieldName());
  dsm($column->isInverted());
  dsm($column->getJoins());
}
catch (Exception $e) {
  dsm($e->getMessage());
}


_shadow_load_classes();
$join = 'node.vid=term_node.vid{tn},term_node.tid=term_data.tid{td}';
try {
  $join = new ShadowJoin($join);
  dsm($join->getJoins());
}
catch (Exception $e) {
  dsm($e->getMessage());
}
