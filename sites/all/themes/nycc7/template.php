<?php
/**
 * @file
 * template.php
 */
 
function nycc7_preprocess_page(&$vars) {
  $search_form = drupal_get_form('search_form');
  $search_form_box = drupal_render($search_form);
  $vars['search_box'] = $search_form_box;
  //dsm($vars);
  $vars['settings_box'] = drupal_render($vars['tabs']['#primary']);
  // unset effect of drupal_render for now
  $vars['tabs']['#primary']['#printed'] = FALSE;
  $vars['menu_expanded'] = _nycc7_menu(menu_tree('main-menu'));
} // nycc7_preprocess_node

function _nycc7_menu($menu) {
  $tree = array();
  $i = 0;
  foreach ($menu as $key => $item) {
    if (!is_numeric($key)){
      break;
    }
    $leaf = array();
    $leaf[] = '<li class="top-item">' . l($item['#title'], $item['#href']) . '</li>';
    foreach ($item['#below'] as $child_key => $child) {
      if (!is_numeric($child_key)){
        break;
      }
      $leaf[] = '<li>' . l($child['#title'], $child['#href']) . '</li>';
    }
    $class = 'col-xs-12 col-sm-6 col-md-3 col-lg-2 ';
    $tree[] = '<ul class="' . $class . '">' . implode('', $leaf) . '</ul>';
    $i++;
  }
  return implode('', $tree);
} // _nycc7_menu

function xxnycc7_menu_local_tasks(&$vars) {
  if (isset($vars['primary'])) {
    foreach($vars['primary'] as $menu_item_key => $menu_attributes) {
      //$vars['primary'][$menu_item_key]['#link']['localized_options'] = array(      'attributes' => array('class' => array('xxx')),);
    }
  }
  //return theme_menu_local_tasks($vars);
} // nycc7_menu_local_tasks

function nycc7_theme($existing, $type, $theme, $path) {
  return array(
    'rides_node_form' => array(
        'arguments' => array('form' => NULL),
        'template' => 'templates/node--rides--edit',
        'render element' => 'form',
        ),
  );
}

