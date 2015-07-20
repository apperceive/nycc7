<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */

//print $fields->title;
?>
 


<?php foreach ($fields as $id => $field): ?>
	<?php 
	print $field->class . "=". $field->raw;
		if ($field->class=='field-image-cache-fid') {
	 	print '<a class="thumb" name="leaf" href="'.$field->raw.'" title="Title">';
		}
	 ?>

<div class="thumb-content">
<p class="fleft">
<? 
	if ($field->class=='field-image-cache-fid-1') 
	{
		print '<img alt="" src="'.$field->raw.'" />';
	}
?>
</p>
<div class="thumb-date">
<? 
	if ($field->class=='field-date-value') 
	{
		print $field->raw;
	}
?>
</div>
<p>long</p>
</div>
</a>
<div class="caption">
<?php 
		print '<h1>'.$fields['title']->raw.'</h1>';
	
?>
<a class="more" href="#">Read More <span>&gt;&gt;</span></a>
										<p>Ryan Lochte competes in the Men's 200m Backstroke Final during</p>
										
										
									</div>

      
<?php endforeach; ?>
