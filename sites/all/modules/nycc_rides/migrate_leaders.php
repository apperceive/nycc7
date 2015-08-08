<?php
 
// Search for publication nodes of publication type "report".
$efq = new EntityFieldQuery();
$efq
  // Conditions on the entity - its type and its bundle ("sub-type")
  ->entityCondition('entity_type', 'node')
  ->entityCondition('bundle', 'rides')
  // Conditions on the entity's fields
  // * a "sub-sub-type" we use for publications.
  ->fieldCondition('field_ride_leaders', 'value', '', '!=');
 
// Execute, returning an array of arrays.
$result = $efq->execute();
 
// Ensure we've got some node results.
if (!isset($result['node'])) {
  drush_log("No nodes to process.", "ok");
  return;
}

drush_log("loaded " . count($result) . " nodes.");
return;

 
// Iterate over the result, loading each node at a time.
foreach($result['node'] as $nid => $stub_node) {
  // Load the full node and wrap it with entity_metadata_wrapper().
  $node = node_load($nid);
  $wrapped_node = entity_metadata_wrapper("node", $node);
 
  // If there's a full-text field_body, swap it into the summary;
  // then delete that full version, so it's blank, and save.
  $full_body = $wrapped_node->field_body->value();
  if ($full_body["value"]) {
    $full_body["summary"] = $full_body["value"];
    $full_body["value"] = "";
    $wrapped_node->field_body->set($full_body);
    $wrapped_node->save();
  }
 
  // Log our progress.
  drush_log("Processed nid={$node->nid}, title={$node->title}", "ok");
}