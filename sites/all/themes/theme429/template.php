<?php

function phptemplate_preprocess_node(&$vars) {
  //dsm($vars);
  //if ($vars['type'] == 'forum')
  //  $vars['date'] = theme429_format_date_for_forum($vars['date']);


  if(array_key_exists( 'joined', $vars))
     $vars['joined'] = $vars['joined'] . " Paid member";

  if ($vars['type'] == 'rides') {
     $ride = $vars['node'];

    /*if (is_array($vars['og_groups'])) {
      $vars['title'] = "GRP: ". $vars['title'];
    }*/

    // todo: make this a function call
    $vars['ride_day_date_time'] = date('l, F d, Y', strtotime($vars['field_date_ride_first'][0]['value'])) ." ". $vars['field_ride_start_time_hour'][0]['value'] .":". $vars['field_ride_start_time_min'][0]['value'] ." ". $vars['field_ride_start_time_select'][0]['value'];

    // todo: make this a function call
    $vars['ride_from'] = $vars['field_ride_from'][0]['value'] ? $vars['field_ride_from'][0]['value'] : $vars['field_ride_from_select'][0]['value'];

    // note: 4 is the Filtered HTML with media profile, FALSE for no access check
    $vars['nycc_ride_description'] = check_markup($vars['field_ride_description'][0]['value'], 4, FALSE);

    if (nycc_ride_has_images($vars['field_ride_image']))
      $vars['nycc_ride_images'] = "<div class='uploaded-images'><label>Uploaded Images: </label><div><small>Click for full sized image.</small></div>". $vars['field_ride_image_rendered'] ."</div>";

    if (nycc_ride_has_attachments($vars['field_ride_attachments']))
      $vars['nycc_ride_attachments'] = "<div class='uploaded-files'><label>Attachments: </label><div><small>Click to open or download. \"Right\" click to save.</small></div>". $vars['field_ride_attachments_rendered'] ."</div>";

    if (isset($vars['field_ride_cue_sheet'][0]['nid']) && ($vars['field_ride_cue_sheet'][0]['nid'] > 0)) {
      $csnid = $vars['field_ride_cue_sheet'][0]['nid'];
      $vars['nycc_ride_cue_sheet'] = "<div class='nycc-ride-cue-sheet'><label>Library Cue Sheet: </label><div></div><a href='/node/$csnid' target='_blank' title='View cue sheet details and PDF ...'>". $vars['field_ride_cue_sheet'][0]['safe']['title'] ."</a></div>";
    }

    if (drupal_strlen($vars['log']))
      $vars['revision_note'] = "<div class='last-revision'><label>Last revision:</label><span>". date('F, d Y', $vars['revision_timestamp']) ."</span></div>";

    $vars['revision_history'] = "<fieldset class='collapsible collapsed'><legend>Ride Revision History</legend>". views_embed_view('ride_revisions', 'default', $vars['nid']) ."</fieldset>";

    $vid = nycc_get_last_revision_id($vars['nid']);
    if ($vid)
      $vars['last_revision'] = "<div class='nycc-last-rev'>$vid</div>";
      //$lastrev = node_load(array('nid' => $vars['nid']), $vid);
      //$vars['last_revision_rendered'] = node_view($lastrev);
      //$lastrev->field_ride_description[0]['value'];

      // TODO: why is this inside this conditional block? move outside?
      // Is this working because there is always a revision?
     $vars['nycc_ride_buttons'] = (nycc_is_rider($ride) || nycc_is_waiting($ride)) ? nycc_withdraw_from_ride_form($ride) : nycc_join_ride_form($ride);

     //dsm($vars['nycc_ride_buttons']);
   }

}

function phptemplate_preprocess_page(&$vars) {
  // this does not work here: dsm($vars);
  global $user;
  $body_classes = array();
  $blks = array();

  $vars['primary_menu'] = menu_tree(variable_get('menu_primary_links_source', 'primary-links'));

  //$vars['tabs2'] = menu_secondary_local_tasks();
  if (!isset($vars['tabs2']))
    $vars['tabs2'] = "";

  if (isset($vars['node']) && $vars['node'] && arg(2) != 'edit' && $vars['node']->type == 'popup') {
    $vars['template_files'][] = 'page-popup';
  }

  $vars['messages'] = str_replace(array('<li>Volunteer <em>Volunteer -  </em> has been created.</li>'), array(''), $vars['messages']);


  //if (in_array('Paid Member', $user->roles))
  //$vars['joined'] = "Paid member";


  // $body_classes[] = ($vars['is_front']) ? 'front' : 'not-front';
  // $body_classes[] = ($vars['logged_in']) ? 'logged-in' : 'not-logged-in';
  $body_classes[] = (isset($vars['sidebar_left']) && $vars['sidebar_left']) ? 'sidebar-left' : '';
  $body_classes[] = (isset($vars['sidebar_right']) && $vars['sidebar_right']) ? 'sidebar-right' : '';
  $body_classes[] = (isset($vars['layout']) && $vars['layout']) ? 'layout-'.$vars['layout'] : '';
  $body_classes[] = arg(0);
  if (arg(1))
   $body_classes[] = arg(0) ."-". arg(1);
  if (arg(1) && arg(2))
    $body_classes[] = arg(0) ."-". arg(1) ."-". arg(2);

  if (isset($vars['node'])) {
    $body_classes[] = 'full-node';
    $body_classes[] = 'node-'. $vars['node']->type;
    $body_classes[] = (($vars['node']->type == 'forum') || (arg(0) == 'forum')) ? 'forum' : '';
    // if(arg(2) != "edit"){
    //   $body_classes[] = ($vars['node']->type) ? 'node-type-'. $vars['node']->type : '';
    // }
    if(arg(0) == "node" && arg(1) == "add"){
      $body_classes[] = ($vars['node']->type) ? 'node-add-type-'. arg(2) : '';
    }
    if(arg(2) == "edit"){
      $body_classes[] = ($vars['node']->type) ? 'node-edit-type-'. $vars['node']->type : '';
    }
    if ($vars['node']->type == 'rides') {
      $body_classes[] = "nycc-ride-status-". strtolower($vars['node']->field_ride_status[0]['value']);
    }
    //$body_classes[] = 'node-'. $vars['node']->nid;
    if ($vars['node']->type == 'group') {
      $vars['tabs'] = str_replace(array('>By term<'), array('><'), $vars['tabs']);
      $vars['tabs'] = str_replace(array('>Track<'), array('><'), $vars['tabs']);
    }
  }
  else {
    $body_classes[] = (arg(0) == 'forum') ? 'forum' : '';
  }

  $body_classes = array_filter($body_classes);  // Remove empty elements
  // todo: remove dups
  //$vars['body_classes'] = implode(' ', $body_classes);
  $vars['body_classes'] .= ' '. implode(' ', $body_classes);


  if (arg(0) == 'og') {
    //dsm($vars);
    // save items that need to be restored
    //$vars['content'] = str_replace("delete_admin", "delete_xxx", $vars['content']);
    // change
    $vars['content'] = str_replace("administrator", "Captain", $vars['content']);
    $vars['content'] = str_replace("Administrator", "Captain", $vars['content']);
    $vars['content'] = str_replace("admin<", "Captain<", $vars['content']);
    $vars['content'] = str_replace("Admin<", "Captain<", $vars['content']);
    $vars['content'] = str_replace("admin:", "Captain:", $vars['content']);
    $vars['content'] = str_replace("Admin:", "Captain:", $vars['content']);
    $vars['content'] = str_replace("admin ", "Captain ", $vars['content']);
    $vars['content'] = str_replace("Admin ", "Captain ", $vars['content']);
    $vars['title'] = str_replace("administrator", "Captain", $vars['title']);
    $vars['title'] = str_replace("admin ", "Captain ", $vars['title']);
    // restore
    //$vars['content'] = str_replace("delete_xxx", "delete_admin", $vars['content']);
  }


  if (arg(0) == 'user' && arg(1) > 0) {
    $account = user_load(array('uid' => arg(1)));
    if ($account) {
      $cp = content_profile_load('profile', $account->uid);
      if ($cp) {
        // replace user name with full name
        $vars['title'] = str_replace($account->name, $cp->title, $vars['title']);
        $vars['head_title'] = str_replace($account->name, $cp->title, $vars['head_title']);
        //$blks[] = nycc_output_block("", nycc_output_profile_buttons($account), "block-nycc-profile-buttons");
      }
      //$vars['tabs'] = str_replace(array('>Edit<'), array('>Edit Account<'), $vars['tabs']);
      $vars['tabs'] = str_replace(array('>Profile<'), array('>Profile<'), $vars['tabs']);
      $vars['tabs'] = str_replace(array('>Available<'), array('>Events<'), $vars['tabs']);
    }
    //$vars['tabs'] = "";
  }

  if (arg(0) =='node' && arg(1) == 'add' && arg(2) == 'rides') {
    $blks[] = nycc_output_block("", nycc_output_ride_buttons($vars['node']), "block-nycc-ride-buttons");
    //$blks[] = "<div class='block block-template block-nycc-buttons' id='block-template-1'>". nycc_output_buttons($vars['node']) ."</div>";
    $vars['head_title'] = "Submit A Ride";
    $vars['title'] = "";

    $blks[] = nycc_output_block("Have a Question?", nycc_output_ride_coordinators(), "block-nycc-rchelp");
    $blks[] = nycc_output_block("Ride Levels?", nycc_output_ride_classification(), "block-nycc-ride-levels");
    //$blks[] = nycc_output_block("", nycc_output_similar_rides($vars['node']), "block-nycc-similar");
    //$blks[] = "<div class='block block-template block-nycc-rchelp' id='block-template-3'><h2>Have a Question?</h2><div id='ride-coordinators-wrapper'>". nycc_output_ride_coordinators() ."</div></div>";
    //$blks[] = "<div class='block block-template block-nycc-rlhelp' id='block-template-4'><h2>Ride Levels?</h2><div id='ride-classification-wrapper'>". nycc_output_ride_classification() ."</div></div>";
    //$blks[] = "<div class='block block-template block-nycc-rlhelp' id='block-template-5'><div id='ride-similar-wrapper'>". nycc_output_similar_rides($vars['node']) ."</div></div>";
  }

  if (isset($vars['node']) && ($vars['node']->type == 'rides')) {
     // if (isset($vars['tabs']))
     //  if (nycc_can_modify_ride($vars['node'], $user))
     //    $vars['tabs'] = str_replace(array('>Edit<'), array('>Revise<'), $vars['tabs']);
     //  else {
     //    $vars['tabs'] = str_replace(array('>View<'), array('><'), $vars['tabs']);
     //    $vars['tabs'] = str_replace(array('>Edit<'), array('><'), $vars['tabs']);
     //  }

    if (arg(2) != 'devel')
      $vars['tabs'] = "";

    //$blks[] = "<div class='block block-template block-nycc-buttons' id='block-template-1'>". nycc_output_buttons($vars['node']) ."</div>";
    $blks[] = nycc_output_block("", nycc_output_ride_buttons($vars['node']), "block-nycc-buttons");

    if (!arg(2)) {
      //$blks[] = "<div class='block block-template block-nycc-riders' id='block-template-2'><h2>Participants</h2><div id='ride-participants-wrapper'>". nycc_output_ride_participants($vars['node']) ."</div></div>";
      $blks[] = nycc_output_block("Participants", nycc_output_ride_participants($vars['node']), "block-nycc-participants");
      if (nycc_count_waiters($vars['node']))
        $blks[] = nycc_output_block("Waiting List", nycc_output_ride_waitlist($vars['node']), "block-nycc-waitlist");
      $gp = og_determine_context();
      if ($gp)
        $blks[] = nycc_output_block("Withdrawals", nycc_output_group_ride_withdrawals_block($vars['node']), "block-nycc-withdrawals");
    }

    if (arg(2) == 'edit') {
      $vars['title'] = "";
      // $blks[] = "<div class='block block-template block-nycc-rchelp' id='block-template-3'><h2>Have a Question?</h2><div id='ride-coordinators-wrapper'>". nycc_output_ride_coordinators() ."</div></div>";
      // $blks[] = "<div class='block block-template block-nycc-rlhelp' id='block-template-4'><h2>Ride Levels?</h2><div id='ride-classification-wrapper'>". nycc_output_ride_classification() ."</div></div>";
      // $blks[] = "<div class='block block-template block-nycc-rlhelp' id='block-template-5'><div id='ride-similar-wrapper'>". nycc_output_similar_rides($vars['node']) ."</div></div>";
      $blks[] = nycc_output_block("Have a Question?", nycc_output_ride_coordinators(), "block-nycc-rchelp");
      $blks[] = nycc_output_block("Ride Levels?", nycc_output_ride_classification(), "block-nycc-ride-levels");
      $blks[] = nycc_output_block("", nycc_output_similar_rides($vars['node']), "block-nycc-similar");
    }

  }
  $vars['right'] = implode("\n", $blks) . (drupal_strlen(trim($vars['right'])) ? "\n". $vars['right'] : "");

  // Add Google Webmaster Tools verification to homepage.
  if(drupal_is_front_page()) {
    $vars['head'] = '<meta name="google-site-verification" content="dNFpFpf_79zFbAeEbzTNppj91ljQbgTb97LBcQ-MHtA" />'."\n". $vars['head'];
  }

  // support for privacy
  // as per http://drupal.org/node/918194#comment-3474618
  header('P3P: CP="CAO PSA OUR"');
  // alternative longer version:
  //header('P3P:CP="IDC DSP COR ADM DEVi TAIi PSA PSD IVAi IVDi CONi HIS OUR IND CNT"');
  // for use of xml file, see also http://drupal.org/node/454720
  // header("P3P: policyref=\"http://yoururl/p3p.xml\",CP=\"NON DSP COR CURa PSA PSD OUR BUS NAV STA\"");
  // not just and IE problem, but see http://msdn.microsoft.com/en-us/library/ms537343.aspx#_P3P%20and%20Compact%20Policies
}

function phptemplate_preprocess_content_profile_display_view(&$vars) {
  $account = user_load(array('uid' => $vars['uid']));
  $cp = $vars['node'];
  $vars['title'] = ""; //$cp->title;
  $output .= nycc_profile_output_user_profile($account, $cp);
  $vars['content'] = $output;
}

function phptemplate_node_form($form) {
  if((arg(0) == "node") && (arg(1) == "add")&& (arg(2) == "rides"))
    return nycc_output_ride_node_form($form, 'add');

  if ((arg(0) == 'node') && (arg(2) == 'edit')) {
    if($form['#node']->type == "rides")
      return nycc_output_ride_node_form($form, 'edit');
  }

  if((arg(0) == "node") && (arg(1) == "add")&& (arg(2) == "group"))
    return nycc_output_group_node_form($form, 'add');

  if ((arg(0) == 'node') && (arg(2) == 'edit')) {
    if($form['#node']->type == "group")
      return nycc_output_group_node_form($form, 'edit');
  }

  return theme_node_form($form);
}

function phptemplate_preprocess_comment(&$vars) {
  $node = $vars['node'];
  //if ($node && ($node->type == 'forum'))
  //  $vars['date'] = theme429_format_date_for_forum($vars['date']);
}

/**
 * Sets the body-tag class attribute.
 *
 * Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
 */
function phptemplate_body_class(/* $left, $right*/) {
  $node_type_rides = "";
  $node_view = "";
  $path = "";
  $node_type = "";
  if((arg(0) == 'node') && (arg(1) == "add") && (arg(2) ==  "rides")){
    $node_type_rides = ' node-rides';
  }
  if(arg(0) == 'ride-complete'){
    $node_view = ' node-rides-view';
  }
  else if(arg(0) == 'ride-detail'){
    $node_view = ' node-rides-view';
  }
  if(arg(0) == "forum" && arg(1) == "3"){
    $path =' message-board';
  }
  print trim($node_type_rides .$node_view .$node_type .$path);
}

function theme429_menu_local_task($link, $active = FALSE) {
  return '<li '. ($active ? 'class="active" ' : '') .'><span><span>'. $link ."</span></span></li>\n";
}

function theme429_l($text, $path, $options = array()) {
  $options += array(
      'attributes' => array(),
      'html' => TRUE,
    );
  if ($path == $_GET['q'] || ($path == '<front>' && drupal_is_front_page())) {
    if (isset($options['attributes']['class'])) {
      $options['attributes']['class'] .= ' active';
    }
    else {
      $options['attributes']['class'] = 'active';
    }
  }
  if (isset($options['attributes']['title']) && strpos($options['attributes']['title'], '<') !== FALSE) {
    $options['attributes']['title'] = strip_tags($options['attributes']['title']);
  }
  return '<a href="'. check_url(url($path, $options)) .'"'. drupal_attributes($options['attributes']) .'>'. ($options['html'] ? $text : check_plain($text)) .'</a>';
}

/*** Override theme_links to include <span> in list.*/
function theme429_links($links, $attributes = array('class' => 'links')) {
  $output = '';
  if (count($links) > 0) {
    $output = '<ul'. drupal_attributes($attributes) .'>';
    $num_links = count($links);
    $i = 1;
    foreach ($links as $key => $link) {
      $class = '';
      if (isset($link['attributes']) && isset($link['attributes']['class'])) {
        $link['attributes']['class'] .= ' ' . $key;
        $class = $key;
      }
      else {
        $link['attributes']['class'] = $key;
        $class = $key;
      }
      $extra_class = '';
      if ($i == 1) {
        $extra_class .= 'first ';
      }
      if ($i == $num_links) {
        $extra_class .= 'last ';
      }
      $current = '';
      if (strstr($class, 'active')) {
        $current = ' active';
      }
      $output .= '<li class="'. $extra_class . $class . $current .'">';
      $html = isset($link['html']) && $link['html'];
      $link['query'] = isset($link['query']) ? $link['query'] : NULL;
      $link['fragment'] = isset($link['fragment']) ? $link['fragment'] : NULL;
      if (isset($link['href'])) {
        $spanned_title = "<span ". drupal_attributes($link['attributes']) ."><span>".$link['title']."</span></span>";
       $output .= theme429_l($spanned_title, $link['href'], $link['attributes'], $link['query'], $link['fragment']);
      } else if ($link['title']) {
        if (!$html) {
          $link['title'] = check_plain($link['title']);
        }
        $output .= '<span'. drupal_attributes($link['attributes']) .'>'. $link['title'] .'</span>';
      }
      $i++;
      $output .= "</li>\n";
    }
    $output .= '</ul>';
  }
  return $output;
}

// NO LONGER USED - moved to nycc_form_alter
/*FOR USER LOGIN*/
function custom_login_block() {
  $form = array(
    '#action' => url($_GET['q'], array('query' => drupal_get_destination())),
    //'#id' => 'user-login-form',
    '#id' => 'user-login',
    '#validate' => user_login_default_validators(),
    '#submit' => array('user_login_submit'),
  );
  $form['name'] = array('#type' => 'textfield',
    '#title' => t('Username'),
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#size' => 15,
    '#required' => TRUE,
  );
  $form['pass'] = array('#type' => 'password',
    '#title' => t('Password'),
    '#maxlength' => 60,
    '#size' => 15,
    '#required' => TRUE,
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Login'), );
  $items = array();
  if (variable_get('user_register', 1)) {
    $items[] = l(t('create new account'), 'user/register', array('title' => t('Create a new user account.')));
  }
  $items[] = l(t('request new password'), 'user/password', array('title' => t('Request new password via e-mail.')));
  $form['links'] = array('#value' => theme('item_list', $items));
  return $form;
}

function theme429_user_bar() {
  global $user;
  $output = '';
  if (!$user->uid) {
    //$output .= drupal_get_form('custom_login_block');
    $output .= drupal_get_form('user_login_block');
  }
  else {
    $output .= t('<p class="user-info">Hi !user, welcome back.</p>', array('!user' => theme('username', $user)));
    $user_access = 0;
    $roles_data = $user->roles;
    //checking permissions
    foreach($roles_data as $role_name) {
      if (($role_name == "ride coordinator") || ($role_name == "vp of rides")) {
        $user_access = 1;
        break;
      }
    }

    /*
    //Checking User Access
    if($user_access == 1) {
      $output .= theme('item_list', array(
      l(t('Your account'), 'user/'.$user->uid, array('title' => t('Edit your account'))),
      l(t('Approve rides'), 'approve-rides', array('title' => t('Approve rides'))),
      l(t('Sign out'), 'logout')));
    }
    else{
      $output .= theme('item_list', array(
      l(t('Your account'), 'user/'.$user->uid, array('title' => t('Edit your account'))),
      l(t('Sign out'), 'logout')));
    }
    */

    $arr = array();
    $arr[] = l(t('Your account'), 'user/'.$user->uid, array('title' => t('Edit your account')));
    //Checking User Access
    if($user_access == 1)
       $arr[] = l(t('Approve rides'), 'approve-rides', array('title' => t('Approve rides')));

    $cnt = 0;
    if (is_array($user->og_groups))
      $cnt = count($user->og_groups);

    if ($cnt == 1) {
      foreach ($user->og_groups as $gid => $g)
        $arr[] = l($g['title'], "node/".$gid, array('title' => $g['title']));
    }
    if ($cnt > 1) {
      $arr[] = l('My groups', "og/my", array('title' => 'My groups'));
    }

    $arr[] = l(t('Sign out'), 'logout');
    $output .= theme('item_list', $arr);

 }
 $output = '<div id="user-bar">'.$output.'</div>';
 return $output;
}

function theme429_format_date_for_forum($date) {
  dsm(array($date, strtotime($date), date('m/d/y h:i:s A', strtotime($date))));
  //$fmt = variable_get('date_format_long', 'm/d/y h:i:s A');

  //return date($fmt, strtotime($date)) ." (". t('%time ago', array('%time' => format_interval(time() - strtotime($date)))) . ")";
  return date('m/d/y h:i:s A', strtotime($date)) ." (". t('%time ago', array('%time' => format_interval(time() - strtotime($date)))) . ")";
}

function theme429_preprocess_views_exposed_form(&$vars, $hook) {
  // only alter the search view exposed filter form
  if ($vars['form']['#id'] == 'views-exposed-form-cue-sheet-listing-block-1') {
    // Change the text on the submit button to Search
    $vars['form']['submit']['#value'] = t('Search');
    unset($vars['form']['submit']['#printed']);
    $vars['button'] = drupal_render($vars['form']['submit']);

    //Add autocomplete functionality to the keys textbox
    $vars['form']['keys']['#autocomplete_path'] = 'finder_autocomplete/autocomplete/2/1';
    unset($vars['form']['keys']['#printed']);
    $vars['widgets']['filter-keys']->widget = drupal_render($vars['form']['keys']);

    //Change the <any> choice to All options
    $vars['form']['field_cuesheet_region_nid']['#options']['All'] = t('Choose');
    unset($vars['form']['field_cuesheet_region_nid']['#printed']);
    $vars['widgets']['filter-field_cuesheet_region_nid']->widget = drupal_render($vars['form']['field_cuesheet_region_nid']);

    $vars['form']['field_cuesheet_level_value_many_to_one']['#options']['All'] = t('Choose');
    unset($vars['form']['field_cuesheet_level_value_many_to_one']['#printed']);
    $vars['widgets']['filter-field_cuesheet_level_value_many_to_one']->widget = drupal_render($vars['form']['field_cuesheet_level_value_many_to_one']);

    $vars['form']['field_cuesheet_rating_value_many_to_one']['#options']['All'] = t('Choose');
    unset($vars['form']['field_cuesheet_rating_value_many_to_one']['#printed']);
    $vars['widgets']['filter-field_cuesheet_rating_value_many_to_one']->widget = drupal_render($vars['form']['field_cuesheet_rating_value_many_to_one']);
  }
}

function theme429_uc_empty_cart() {
  $output = '<p class="uc-cart-empty">'. t('There are no products in your shopping cart.') .'</p>';
  $output .= '<p class="uc-cart-empty-help">'. t('Note: if your cart is unexpectedly empty, this may be due to your browser\'s security settings.  Please see <a href="/node/55331">this page</a> for more information.') .'</p>';

  return $output;
}

//function theme429_preprocess_block(&$variables) {
  // static $block_counter = array();
  // // All blocks get an independent counter for each region.
  // if (!isset($block_counter[$variables['block']->region])) {
  //   $block_counter[$variables['block']->region] = 1;
  // }
  // // Same with zebra striping.
  // $variables['block_zebra'] = ($block_counter[$variables['block']->region] % 2) ? 'odd' : 'even';
  // $variables['block_id'] = $block_counter[$variables['block']->region]++;
  //
  // $variables['template_files'][] = 'block-' . $variables['block']->region;
  // $variables['template_files'][] = 'block-' . $variables['block']->module;
  // $variables['template_files'][] = 'block-' . $variables['block']->module . '-' . $variables['block']->delta;

  //dsm($variables['block']);
//}


// function theme429_print_pdf_dompdf_footer($html){
//   if ( isset($pdf) ) {
//     //$pdf->setPrintFooter(true);
//     //$font = Font_Metrics::get_font("helvetica", "bold");
//     //$pdf->page_text(72, 18, "{PAGE_NUM} of {PAGE_COUNT}", $font, 6, array(0,0,0));
//   }
//   return $html;
// }

// function phptemplate_preprocess_user_profile(&$vars) {
//   // $vars['account']
//   //dsm($vars);
//   $account = $vars['account'];
//   if ($account) {
//     $cp = content_profile_load('profile', $account->uid);
//     if ($cp) {
//       //$vars['title'] = $cp->title;
//       //dsm($vars);
//     }
//   }
// }

function theme429_preprocess_author_pane(&$vars) {
  //var_dump($vars['account']->roles);
  $acct = $vars['account'];
  if (!$acct->uid || !is_array($acct->roles)) {
    //watchdog('mss', "roles not an array in theme429_preprocess_author_pane for account ". $acct->uid);
    return;
  }
  foreach ($acct->roles as $role) {
    if ($role == "paid member") {
      $vars['user_title'] = "Active member";
      continue;
    }
  }
}

drupal_add_js(drupal_get_path('theme', 'theme429') . '/js/dropdown-menu.js');
drupal_add_js(drupal_get_path('theme', 'theme429') . '/js/datepicker.js');
drupal_add_js(drupal_get_path('theme', 'theme429') . '/js/additional.js');
drupal_add_js(drupal_get_path('theme', 'theme429') . '/js/confirm.js');
drupal_add_js(drupal_get_path('theme', 'theme429') . '/js/jquery.simplemodal.js');
drupal_add_js('misc/collapse.js');

drupal_add_css(drupal_get_path('theme', 'theme429') . '/css/datepicker.css');
drupal_add_css(drupal_get_path('theme', 'theme429') . '/css/confirm.css');

?>