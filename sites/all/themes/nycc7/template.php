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
    //$variables['classes_array'][] = 'well';
    //$variables['classes_array'][] = 'well-lg';
  } else {
    $variables['content'] = str_replace('Login</a>', '<button type="button" class="navbar-toggle" title="Sign In"><span class="glyphicon glyphicon-user"></span></button></a>', $variables['content']);   
  }
} // nycc7_preprocess_block

/**
 * Implements hook_preprocess_html().
 *
 * WHY IS THIS NECESSARY? copied from base theme
 */
function nycc7_preprocess_html(&$variables) {
  // Backport from Drupal 8 RDFa/HTML5 implementation.
  // @see https://www.drupal.org/node/1077566
  // @see https://www.drupal.org/node/1164926
  
  //dpm($variables);

  // NOTE: THIS IS OVERWRITING THE EXISTING ARRAY
  // HTML element attributes.
  $variables['html_attributes_array'] = array(
    'lang' => $variables['language']->language,
    'dir' => $variables['language']->dir,
  );

  // Override existing RDF namespaces to use RDFa 1.1 namespace prefix bindings.
  if (function_exists('rdf_get_namespaces')) {
    $rdf = array('prefix' => array());
    foreach (rdf_get_namespaces() as $prefix => $uri) {
      $rdf['prefix'][] = $prefix . ': ' . $uri;
    }
    if (!$rdf['prefix']) {
      $rdf = array();
    }
    $variables['rdf_namespaces'] = drupal_attributes($rdf);
  }

  // NOTE: THIS IS OVERWRITING THE EXISTING ARRAY

  // MSS: W3C check says no to document role 2015-10-28
  // BODY element attributes.
  $variables['body_attributes_array'] = array(
    //'role'  => 'document',
    'class' => $variables['classes_array'],
  );
  $variables['body_attributes_array'] += $variables['attributes_array'];

  // Navbar position.
  switch (bootstrap_setting('navbar_position')) {
    case 'fixed-top':
      $variables['body_attributes_array']['class'][] = 'navbar-is-fixed-top';
      break;

    case 'fixed-bottom':
      $variables['body_attributes_array']['class'][] = 'navbar-is-fixed-bottom';
      break;

    case 'static-top':
      $variables['body_attributes_array']['class'][] = 'navbar-is-static-top';
      break;
  }
} // nycc7_preprocess_html

/**
 * Implements hook_process_html().
 *
 * WHY IS THIS NECESSARY? copied from base theme
 */
function nycc7_process_html(&$variables) {
  $variables['html_attributes'] = drupal_attributes($variables['html_attributes_array']);
  $variables['body_attributes'] = drupal_attributes($variables['body_attributes_array']);
}

/**
 * Implements hook_preprocess_page().
 *
 */
 function nycc7_preprocess_page(&$variables) {
  global $user;
  
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
    "http://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css",
    "http://fonts.googleapis.com/css?family=Lato:400,700,400italic",
    "http://fonts.googleapis.com/css?family=Roboto+Slab:400,700,300",
    "sites/all/libraries/smartmenus/dist/css/sm-core-css.css",      
    "sites/all/libraries/smartmenus/dist/css/sm-blue/sm-blue.css",      
    // TODO: reinstate this?
    "sites/all/libraries/smartmenus/dist/addins/bootstrap/jquery.smartmenus.bootstrap.css ",      
  );
  foreach ($css as $file) {
    $type = (stripos($file, 'http') === 0) ? "external" : "file";
    $options = array('type' => $type, 'weight' => 100, 'group' => JS_THEME, );
    drupal_add_css($file, $options);
  }
 
  $search_form_box = "";
  if (arg(0) != 'search') {
    $search_form = drupal_get_form('search_form');
    $search_form_box = drupal_render($search_form);
  }
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
  
  $variables['navbar_nav_links'] = _nycc7_navbar_nav_links();

} // nycc7_preprocess_page


/**
 * Processes variables for the "page" theme hook.
 *
 * See template for list of available variables.
 *
 * @see page.tpl.php
 *
 * @ingroup theme_process
 *
 * WHY IS THIS NECESSARY? copied from base theme
 */
function nycc7_process_page(&$variables) {
  if (!array_key_exists('navbar_classes', $variables))
    $variables['navbar_classes'] = "";
  if (isset($variables['navbar_classes_array']) && is_array($variables['navbar_classes_array']))
    $variables['navbar_classes'] = implode(' ', $variables['navbar_classes_array']);
    
}

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
  
  return l($text, $path, array('attributes' => array('title' => $title, 'class' => $classes . ' btn btn-primary'))); 
}

// note: only handles two levels of menu
function _nycc7_menu($menuname, $flat = false, $class = "", $id = "", $front = true, $submenuclasses = "") {
  $menu = menu_tree($menuname);
  $id = $id ? $id : $menuname;
  $ulclasess = drupal_strlen($submenuclasses) ? " class='$submenuclasses'" : "";
  $tree = array();
  
  foreach ($menu as $key => $item) {
    $classes = array();
    if ($flat) $classes[] = 'top-item';
    if (!is_numeric($key)) continue;

    // note: one level processing here only
    $leaf = array();
    if (is_array($item['#below'])) {
      foreach ($item['#below'] as $child_key => $child) {
        if (!is_numeric($child_key)) continue;
        if (drupal_strlen(trim($child['#title']))) {
          $cl = l($child['#title'], $child['#href']);
          // TODO: set active?
          $leaf[] = "<li id='menu-item-$child_key'>$cl</li>";
        }
      }
    }
    $submenuul ="";
    if (count($leaf)) {
      $submenu = implode('', $leaf);
      if (drupal_strlen(trim($submenu))) $submenuul = "<ul>$submenu</ul>";
    }
    // TODO: set active?
    if ($item['#href'] == "<front>") $classes[] = 'home';
    $title = ($item['#href'] == "<front>") ? "&nbsp;" : $item['#title'];
       
    $attributes = array('class' => implode(' ', $classes));
    
    $l = l($title, $item['#href'], array('html' => true, 'attributes' => $attributes));
    // skip home link if $front is true, for footer menu
    if (!($item['#href'] == "<front>") || (($item['#href'] == "<front>") && $front))
      $tree[] = "<li id='menu-item-$key'$submenuclasses>$l$submenuul</li>";
    
  } // for
  $s = "<ul id='$id' class='$class'>" . implode('', $tree) . "</ul>";
  return $s;
} // _nycc7_menu

function _nycc7_navbar_nav_links() {
  global $user;
  $output = "";
  if (!$user->uid) {
    // TODO: make this a single line form - css done
    // placeholders too
    
    $form = drupal_get_form('user_login_block'); // TODO: check this in D7
    $output = drupal_render($form);
    
    /*if (module_exists('hybridauth') && !user_is_logged_in()) {
      $element['#type'] = 'hybridauth_widget';
      $output .= drupal_render($element);
    }*/
    
    // buttons: join, password, register, contact?
    // TODO: add social logins? also join
  }
  else {
    $uid = $user->uid;
    $user = user_load($uid); // load to get $user->realname
    $username = isset($user->realname) ? $user->realname : $user->name;
    $can_approve = function_exists('nycc_rides_can_approve') ? nycc_rides_can_approve() : false;
    $arr = array();  // build array of nav items
    
    $arr[] = _nycc7_navbar_button("Sign out as $username", 'fa-sign-out', '/user/logout');

    $arr[] = _nycc7_navbar_button("Account settings for $username", 'fa-user', '/user');
    
    srand();
    $rand = rand(-2,12);
    $badge = $rand > -1 ? $rand : "";
    $arr[] = _nycc7_navbar_button("You have $badge unread notifications", 'fa-envelope-o', "/user/$uid/messages", $badge);
        
    if ($can_approve) 
      $arr[] = _nycc7_navbar_button('Approve rides', 'fa-thumbs-o-up', '/approve-rides');       

    // groups
    if (isset($user->og_user_node) && is_array($user->og_user_node) && array_key_exists('und', $user->og_user_node) && is_array($user->og_user_node['und'])) {
      foreach($user->og_user_node['und'] as $ndx => $g) {
        $gid = $g['target_id'];
        $gp = node_load($gid);
        $gtitle = 'Group: ' . $gp->title;
        $icon = 'fa-users';
        //$icon = stripos($gtitle, 'group') ? 'fa-user-plus' : $icon;
        //$icon = stripos($gtitle, 'sts') ? 'fa-user-plus' : $icon;
        $icon = stripos($gtitle, 'volunteer') ? 'fa-heart' : $icon;
        $icon = stripos($gtitle, 'sig') ? 'fa-bicycle' : $icon;
        $icon = stripos($gtitle, 'sts') ? 'fa-cogs' : $icon;
        $arr[] = _nycc7_navbar_button($gtitle, $icon, "/node/$gid");
      } // for
    } // array
    
    $output = implode("\n", $arr);
  }
  return $output;
} // _nycc7_navbar_nav_links

function _nycc7_navbar_button($title, $icon, $href, $badge = '') {
  $button = "
    <a class='navbar-btn btn navbar-right' title='$title' onclick='location.href = \"$href\"; return false;'>
      <span class='sr-only'>$title</span>
      <span class='fa $icon fa-2x'></span>
      <span class='badge'>$badge</span>
    </a>";    
  return $button;
} // _nycc7_button

function _nycc_block_render($module, $block_id) {
  $block = block_load($module, $block_id);
  $block_content = _block_render_blocks(array($block));
  $build = _block_get_renderable_array($block_content);
  $block_rendered = drupal_render($build);
  return $block_rendered;
} // _nycc_block_render

/*

function xxxnycc7_menu_local_tasks(&$variables) {
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


// output timestamp as a small 'calendar page'
function _nycc7_output_datetime($date) {
  $parts = getdate(strtotime($date)); // 'Y-m-d H:i:s', 
  $year = $parts['year'];
  $yearclasses = ($year == date("Y")) ? "year thisyear" : "year";
  $yearspan ="<span class='$yearclasses'>$year</span>";
  $dow = $parts['weekday'];
  $month = $parts['month'];
  $day = $parts['mday'];
  if (!$parts['hours'] && !$parts['minutes'])
    $timediv = "";
  else {
    $time = ($parts['hours'] > 12 ? $parts['hours'] - 12 : $parts['hours']) . ':' . ($parts['minutes'] < 10 ? '0' : '') . $parts['minutes'] . ' ' . ($parts['hours'] > 12 ? "pm" : "am");
    $timediv = "<div class='time'>$time</div>";
  }
  $output =<<<EOS
  <div class="nycc7-cal-datetime">
    <!-- time datetime="$date" class="icon"></time -->
    <div class='month'>$month$yearspan</div>
    <div class='day'>$day</div>
    <div class='dow'>$dow</div>
    $timediv
  </div>
EOS;
  return $output;  
}  // _nycc7_output_datetime