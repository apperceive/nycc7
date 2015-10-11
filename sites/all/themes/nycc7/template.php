<?php
/**
 * @file
 * template.php
 */


function nycc7_theme($existing, $type, $theme, $path) {
  return array(
    'rides_node_form' => array(
        'arguments' => array('form' => NULL),
        'template' => 'templates/node--rides_edit',
        'render element' => 'form',
        ),
  );
}

function nycc7_preprocess_block(&$variables) {
  if ($variables['block']->region != 'navigation') {
    $variables['classes_array'][] = 'well';
    $variables['classes_array'][] = 'well-lg';
  } else {
    $variables['content'] = str_replace('Login</a>', '<button type="button" class="navbar-toggle" title="Sign In"><span class="glyphicon glyphicon-user"></span></button></a>', $variables['content']);   
  }
} // nycc7_preprocess_block
 
function nycc7_preprocess_page(&$variables) {

  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-xs-12 col-sm-6 "';
  }
  elseif (!empty($variables['page']['sidebar_first']) || !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-xs-12 col-sm-7 col-md-8"';
  }
  else {
    $variables['content_column_class'] = ' class="col-xs-12 col-sm-12 "';
  }

  $css = array (
    //"http://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css",
    //"http://maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css",
    "sites/all/libraries/smartmenus/dist/css/sm-core-css.css",      
    "sites/all/libraries/smartmenus/dist/css/sm-blue/sm-blue.css",      
    "sites/all/libraries/smartmenus/dist/addins/bootstrap/jquery.smartmenus.bootstrap.css ",      
  );
  
  foreach ($css as $file) {
    $type = (stripos($file, 'http') === 0) ? "external" : "file";
    $options = array('type' => $type, 'weight' => 100, 'group' => JS_THEME, );
    drupal_add_css($file, $options);
  }
 
  
  $search_form = drupal_get_form('search_form');
  $search_form_box = drupal_render($search_form);
  $variables['search_box'] = $search_form_box;
  
  //$variables['settings_box'] = drupal_render($variables['tabs']['#primary']);
  // unset effect of drupal_render for now
  //$variables['tabs']['#primary']['#printed'] = FALSE;
  
  $variables['main_menu_expanded'] = _nycc7_menu('main-menu', false, 'sm sm-blue' );
  
  // make these drop downs? they only have one level of depth
  // add a title, bold but not a link?
  // how toggle menus off when another opened?
  $variables['user_menu_expanded'] = _nycc7_menu('user-menu', true);
  
  // navigation and other menus can have several levels e.g add content | type 
  $variables['navigation_menu_expanded'] = _nycc7_menu('navigation', true);
} // nycc7_preprocess_page

function nycc7_preprocess_node(&$variables) {
  // note: this is not called for add/edit forms, only for view
} //nycc_rides2_preprocess_node 

// can't we just add classes in preprocessor?
function nycc7_nycc_ride_link($variables) {  
  $title = check_plain($variables['title']);
  $path = $variables['path'];
  $classes = $variables['classes'];
  $text = $variables['text'];
  if ($path == "#")
    return "<a href='#' onclick='alert(\"$title\");' title='$title' class='btn btn-info $classes'>$text</a>";
  
  return l($text, $path, array('attributes' => array('title' => $title, 'class' => array('btn btn-primary ' . $classes)))); 
}

// note: only handles two levels of menu
function _nycc7_menu($menuname, $flat = false, $class = "", $id = "", $front = true, $ulclasses = "") {
  $menu = menu_tree($menuname);
  $id = $id ? $id : $menuname;
  $litopclass = $flat ? '' : 'top-item';
  $tree = array();
  
  foreach ($menu as $key => $item) {
    if (!is_numeric($key))
      continue;
    

    // note: one level processing here only
    $leaf = array();
    if (is_array($item['#below'])) {
      foreach ($item['#below'] as $child_key => $child) {
        if (!is_numeric($child_key))
          continue;
        if (drupal_strlen(trim($child['#title']))) {
          $cl = l($child['#title'], $child['#href']);
          $leaf[] = "<li id='menu-item-$child_key'>$cl</li>";
        }
      }
    }
    $submenuul ="";
    if (count($leaf)) {
      $submenu = implode('', $leaf);
      if (drupal_strlen(trim($submenu)))
        $submenuul = "<ul zzzclass='sub-menu'>$submenu</ul>";
    }
    //dpm($item['#href']);
    $homeclass = ($item['#href'] == "<front>") ? "home" : "";
    $title = ($item['#href'] == "<front>") ? "&nbsp;" : $item['#title'];
    $l = l($title, $item['#href'], array('html' => true, 'attributes' => array('class' => array($homeclass, $litopclass))));
    $ulclasess = drupal_strlen($ulclasses) ? "class='$ulclasses'" : "";
    $t = "<li id='menu-item-$key' $ulclasses>$l$submenuul</li>";
    if (!($item['#href'] == "<front>") || (($item['#href'] == "<front>") && $front))
      $tree[]  = $t;
    
  } // for
  $s = "<ul id='$id' class='$class'>" . implode('', $tree) . "</ul>";
 return $s;
} // _nycc7_menu


/*

function xxnycc7_menu_local_tasks(&$variables) {
  if (isset($variables['primary'])) {
    foreach($variables['primary'] as $menu_item_key => $menu_attributes) {
      //$variables['primary'][$menu_item_key]['#link']['localized_options'] = array(      'attributes' => array('class' => array('xxx')),);
    }
  }
  //return theme_menu_local_tasks($variables);
} // nycc7_menu_local_tasks


function nycc7_preprocess_image_style(&$variables) {
  $variables['attributes']['class'][] = 'img-responsive'; // http://getbootstrap.com/css/#overview-responsive-images
}

function nycc7_preprocess_field(&$variables) {
  if($variables['element']['#field_name'] == 'field_photo'){
    foreach($variables['items'] as $key => $item){
      $variables['items'][ $key ]['#item']['attributes']['class'][] = 'img-responsive'; // http://getbootstrap.com/css/#overview-responsive-images
      }
  }
}
*/
