<?php


function nycc_og_action_info() {

  $actions = array();
  $actions['nycc_og_make_leader_action'] = array(
    'type' => 'og_membership',
    'label' => t('Make Leader'),
    'configurable' => FALSE,
  );
  
  $actions['nycc_og_make_captain_action'] = array(
    'type' => 'og_membership',
    'label' => t('Make Captain'),
    'configurable' => FALSE,
  );
  $actions['nycc_og_make_rider_action'] = array(
    'type' => 'og_membership',
    'label' => t('Make Rider'),
    'configurable' => FALSE,
  );
  $actions['nycc_og_make_waiter_action'] = array(
    'type' => 'og_membership',
    'label' => t('Make Waiter'),
    'configurable' => FALSE,
  );
  $actions['nycc_og_add_to_group_action'] = array(
    'type' => 'user',
    'label' => t('Add To Group'),
    'configurable' => FALSE,
  );  
  return $actions;
}
/**
 * @file
 * Action to set the state of a user in a group.
 */

//function og_membership_delete_action_info() {
//  return array('og_membership_delete_action' => array(
//    'type' => 'og_membership',
//    'label' => t('Remove from group'),
//    'configurable' => FALSE,
//  ));
//}
//
//function og_membership_delete_action($og_membership, $context) {
//  $group_type = $og_membership->group_type;
//  $gid = $og_membership->gid;
//
//  // Don't process the group manager, if exists.
//  $group = entity_load_single($group_type, $gid);
//  if (!empty($group->uid) && $group->uid == $og_membership->etid && $og_membership->entity_type == 'user') {
//    return;
//  }
//
//  $og_membership->delete();
//}



function nycc_og_add_to_group_action($user, $context) {
  dpm(get_defined_vars());
}
function nycc_og_make_leader_action($og_membership, $context) {
  dpm($context);
  $group_type = $og_membership->group_type;
  $gid = $og_membership->gid;
  $uid = $og_membership->etid;

  // Don't process the group manager, if exists.
  $group = entity_load_single($group_type, $gid);
  if ($og_membership->entity_type == 'user') {
    nycc_og_add_role_leader($gid, $uid);
  }
}


function nycc_og_make_captain_action($og_membership, $context) {
  $group_type = $og_membership->group_type;
  $gid = $og_membership->gid;
  $uid = $og_membership->etid;

  // Don't process the group manager, if exists.
  $group = entity_load_single($group_type, $gid);
  if ($og_membership->entity_type == 'user') {
    nycc_og_add_role_captain($gid, $uid, $context);
  }
}



function nycc_og_make_rider_action($og_membership, $context) {
  $group_type = $og_membership->group_type;
  $gid = $og_membership->gid;
  $uid = $og_membership->etid;

  // Don't process the group manager, if exists.
  $group = entity_load_single($group_type, $gid);
  if ($og_membership->entity_type == 'user') {
    nycc_og_add_role_rider($gid, $uid);
  }
}

function nycc_og_make_waiter_action($og_membership, $context) {
  $group_type = $og_membership->group_type;
  $gid = $og_membership->gid;
  $uid = $og_membership->etid;

  // Don't process the group manager, if exists.
  $group = entity_load_single($group_type, $gid);
  if ($og_membership->entity_type == 'user') {
    nycc_og_add_role_waiter($gid, $uid);
  }
}