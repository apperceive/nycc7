<?php

/**
 * @file
 * Default theme implementation for displaying a single search result.
 *
 * This template renders a single search result and is collected into
 * search-results.tpl.php. This and the parent template are
 * dependent to one another sharing the markup for definition lists.
 *
 * Available variables:
 * - $url: URL of the result.
 * - $title: Title of the result.
 * - $snippet: A small preview of the result. Does not apply to user searches.
 * - $info: String of all the meta information ready for print. Does not apply
 *   to user searches.
 * - $info_split: Contains same data as $info, split into a keyed array.
 * - $module: The machine-readable name of the module (tab) being searched, such
 *   as "node" or "user".
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Default keys within $info_split:
 * - $info_split['module']: The module that implemented the search query.
 * - $info_split['user']: Author of the node linked to users profile. Depends
 *   on permission.
 * - $info_split['date']: Last update of the node. Short formatted.
 * - $info_split['comment']: Number of comments output as "% comments", %
 *   being the count. Depends on comment.module.
 *
 * Other variables:
 * - $classes_array: Array of HTML class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $title_attributes_array: Array of HTML attributes for the title. It is
 *   flattened into a string within the variable $title_attributes.
 * - $content_attributes_array: Array of HTML attributes for the content. It is
 *   flattened into a string within the variable $content_attributes.
 *
 * Since $info_split is keyed, a direct print of the item is possible.
 * This array does not apply to user searches so it is recommended to check
 * for its existence before printing. The default keys of 'type', 'user' and
 * 'date' always exist for node searches. Modules may provide other data.
 * @code
 *   <?php if (isset($info_split['comment'])): ?>
 *     <span class="info-comment">
 *       <?php print $info_split['comment']; ?>
 *     </span>
 *   <?php endif; ?>
 * @endcode
 *
 * To check for all available data within $info_split, use the code below.
 * @code
 *   <?php print '<pre>'. check_plain(print_r($info_split, 1)) .'</pre>'; ?>
 * @endcode
 *
 * @see template_preprocess()
 * @see template_preprocess_search_result()
 * @see template_process()
 *
 * @ingroup themeable
 */
 
  // hide info
  $info = "";
 
  $icons = array(
    'forum' => 'commenting-o',   // 'bullhorn',
    'rides' => 'bicycle',
    'event' => 'calendar',   
    'page' => 'file-o',   
    'story' => 'file-text',   
    'group' => 'users',   
    'gpforumpost' => 'list-alt',
    'picture' => 'image',   
    'cue_sheet' => 'map-signs',   
    'region' => 'map-o',   
    'pm' => 'legal',                   // presidents message
    'obride' => 'globe',   
    'profile' => 'user',   
    'volunteer' => 'heart',   
    'webform' => 'check-square',   
    'webforms' => 'check-square-o',   
    'poll' => 'pie-chart',   
    'poll2' => 'bar-chart',   
    'mass_contact' => 'envelope-o',   
    'unknown' => 'question',
    'missing' => 'question-circle',
  );

  //$temp = split('/', $url); // deprecated
  //$temp = preg_split('~\/~', $url);
  //$type = strtolower($temp[4]);
  
  $type = array_key_exists('node', $result) ? $result['node']->type : 'unknown';
  if (!in_array($type, array_keys($icons)))
    $type = 'missing';
  
  //dpm(array($result, $result['node']->type, $type));
  
  // handle subtypes if present
  //if ($type == 'resources')
  //  $type = strtolower($temp[5]);

  $icon = $icons[$type];
  $fa = "<span class='fa fa-$icon fa-lg'></span>";
  
  // todo: add flag for language if snippet starts with "Language XXX"
  // use $temp[3] as language code
 
?>
<li class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <h3 class="title"<?php print $title_attributes; ?>>
    &nbsp; <?php print $fa; ?> &nbsp; <a href="<?php print $url; ?>" title="<?php print $type; ?>"><?php print $title; ?></a>
  </h3>
  <?php print render($title_suffix); ?>
  <div class="search-snippet-info">
    <?php if ($snippet): ?>
      <p class="search-snippet"<?php print $content_attributes; ?>><?php print $snippet; ?></p>
    <?php endif; ?>
    <?php if ($info): ?>
      <p class="search-info"><?php print $info; ?></p>
    <?php endif; ?>
  </div>
</li>
