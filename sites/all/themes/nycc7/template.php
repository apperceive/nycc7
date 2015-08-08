<?php
/**
 * @file
 * template.php
 */
 
function nycc7_preprocess_block(&$variables) {
  if ($variables['block']->region != 'navigation') {
    $variables['classes_array'][] = 'well';
    $variables['classes_array'][] = 'well-lg';
  }
} // nycc7_preprocess_block
 
function nycc7_preprocess_page(&$variables) {

  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-6 col-md-5"';
  }
  elseif (!empty($variables['page']['sidebar_first']) || !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-9 col-md-8"';
  }
  else {
    $variables['content_column_class'] = ' class="col-sm-12"';
  }
  
  $search_form = drupal_get_form('search_form');
  $search_form_box = drupal_render($search_form);
  $variables['search_box'] = $search_form_box;
  $variables['settings_box'] = drupal_render($variables['tabs']['#primary']);
  // unset effect of drupal_render for now
  $variables['tabs']['#primary']['#printed'] = FALSE;
  $variables['menu_expanded'] = _nycc7_menu(menu_tree('main-menu'));
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

function xxnycc7_menu_local_tasks(&$variables) {
  if (isset($variables['primary'])) {
    foreach($variables['primary'] as $menu_item_key => $menu_attributes) {
      //$variables['primary'][$menu_item_key]['#link']['localized_options'] = array(      'attributes' => array('class' => array('xxx')),);
    }
  }
  //return theme_menu_local_tasks($variables);
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

