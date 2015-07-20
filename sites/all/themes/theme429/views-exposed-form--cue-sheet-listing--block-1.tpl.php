<?php
// $Id: views-exposed-form.tpl.php,v 1.4.4.1 2009/11/18 20:37:58 merlinofchaos Exp $
/**
 * @file views-exposed-form.tpl.php
 *
 * This template handles the layout of the views exposed filter form.
 *
 * Variables available:
 * - $widgets: An array of exposed form widgets. Each widget contains:
 * - $widget->label: The visible label to print. May be optional.
 * - $widget->operator: The operator for the widget. May be optional.
 * - $widget->widget: The widget itself.
 * - $button: The submit button for the form.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($q)): ?>
  <?php
    // This ensures that, if clean URLs are off, the 'q' is added first so that
    // it shows up first in the URL.
    print $q;
  ?>
<?php endif; ?>
<div class="views-exposed-form">
  <div class="views-exposed-widgets clear-block">
    <?php foreach($widgets as $id => $widget): ?>
      <div class="views-exposed-widget">
        <?php if (!empty($widget->label)): ?>
          <label for="<?php print $widget->id; ?>">
            <?php print $widget->label; ?>
          </label>
        <?php endif; ?>
        <?php if (!empty($widget->operator)): ?>
          <div class="views-operator">
            <?php print $widget->operator; ?>
          </div>
        <?php endif; ?>
        <div class="views-widget">
		<?php
		if ( $widget->label == "Distance:") {
		$str = $widget->widget;
		
		
		$str1 = '<div class="form-item" id="edit-field-cuesheet-distance-value-min-wrapper">';
		$str2 = '<div class="form-item" id="edit-field-cuesheet-distance-value-min-wrapper"><div id="before-text">Between</div>';
 		         
		$str = str_replace($str1,$str2,$str);
		
		$str1 = '<div class="form-item" id="edit-field-cuesheet-distance-value-max-wrapper">';
		$str2 = '<div class="form-item" id="edit-field-cuesheet-distance-value-max-wrapper"><div id="after-text">and</div>';
 		         
		$str = str_replace($str1,$str2,$str);
		
		
		
		
		?>
          <?php //print $widget->widget; ?>
		  <?php //print(serialize(substr($str,1,50))); 
		  print $str . '<div id="after-text-max">miles</div>';
		  }
		  else{
		  print $widget->widget;
		  }
		  ?>
        </div>
      </div>
    <?php endforeach; ?>
    <div class="views-exposed-widget">
      <?php print $button ?>
    </div>
  </div>
</div>
<?php

