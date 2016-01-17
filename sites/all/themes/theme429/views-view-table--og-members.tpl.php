<?php
/**
 * @file views-view-table.tpl.php
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $class: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 * @ingroup views_templates
 */
 //dsm(get_defined_vars());


 $groupid = $variables['view']->args[0];

?>
<table class="<?php print $class; ?>"<?php print $attributes; ?>>
  <?php if (!empty($title)) : ?>
    <caption><?php print $title; ?></caption>
  <?php endif; ?>
  <thead>
    <tr>
      <?php foreach ($header as $field => $label): ?>
        <th class="views-field views-field-<?php print $fields[$field]; ?>">
          <?php print $label; ?>
        </th>
      <?php endforeach; ?>
      <?php if (nycc_is_captain($groupid) || nycc_can_approve()) { ?>
        <th class="">Notes</th>
        <th class="">Status</th>
       <?php } ?>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($rows as $count => $row): ?>
      <tr class="<?php print implode(' ', $row_classes[$count]); ?>">
        <?php $flag = 0; ?>
        <?php foreach ($row as $field => $content): ?>
          <td class="views-field views-field-<?php print $fields[$field]; ?>">
            <?php print $content; ?>
          </td>
          <?php
            if (!$flag) {
              $flag = 1; // only process first column
              $temp = str_replace('<a href="/users/', '', $content);
              $uname = str_replace(strstr($temp, '"'), '', $temp);
              //$acct = user_load(array('name' => $uname));
              $acctid = str_replace("user/", "", drupal_lookup_path('source', "users/$uname"));
              $iscapt = nycc_is_captain($groupid, $acctid);
              //dpm(array($groupid, $acctid, $uname));
            }
          ?>
        <?php endforeach; ?>
        <?php if (nycc_is_captain($groupid) || nycc_can_approve()) { ?>
          <?php if (!$iscapt) { ?>
            <td class="">
              <a href='/nycc/group/notes/<?php print $groupid; ?>/<?php print $acctid; ?>'>Notes</a>
            </td>
            <td class="">
              <?php
                $title = '';
                $ignored = '';
                $tid = nycc_get_group_user_status($groupid, $acctid);
                //$term = taxonomy_get_term($tid);
                //$selected = $term->name;
                $selected = $tid;
                //dpm($selected);
                $vocabulary_id = 6;
                $description = '';
                $blank = NULL;
                $exclude = array();
                $multiple = FALSE;
                $t = _taxonomy_term_select($title, $ignored, $selected, $vocabulary_id, $description, $multiple, $blank, $exclude);
                //dsm($t, drupal_render($t));
                $t['#attributes']['id'] = "gpstatus-" . $groupid . "-" . $acctid;
                $t['#attributes']['class'] = "gpstatus";
                $t['#name'] = 'gpstatus';
                $t['#value'] = $selected;
                print drupal_render($t);
              ?>
              <span style='display:none;' id='gpstatus-<?php print $groupid . "-" . $acctid; ?>-busy'><img src='/sites/all/themes/theme429/images/loading.gif'><img src='/sites/all/themes/theme429/images/loading.gif'></span>
            </td>
           <?php } else { ?>
            <td class="">
              &nbsp;
            </td>
            <td class="">
              &nbsp;
            </td>
           <?php } ?>
         <?php } ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>