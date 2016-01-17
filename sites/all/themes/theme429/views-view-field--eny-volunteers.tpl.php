<?php
 /**
  * This template is used to print a single field in a view. It is not
  * actually used in default Views, as this is registered as a theme
  * function which has better performance. For single overrides, the
  * template is perfectly okay.
  *
  * Variables available:
  * - $view: The view object
  * - $field: The field handler object that can process the input
  * - $row: The raw SQL result that can be used
  * - $output: The processed output that will normally be used.
  *
  * When fetching output from the $row, this construct should be used:
  * $data = $row->{$field->field_alias}
  *
  * The above will guarantee that you'll always get the correct data,
  * regardless of any changes in the aliasing that might happen if
  * the view is modified.
  */
  $arr = array("field_vol_car", "field_vol_available_after", "field_vol_available_before", "field_vol_marshalls", "field_vol_provisions_friday", "field_vol_provisions_van_return", "field_vol_registration", "field_vol_rest_stop", "field_vol_route_demarking", "field_vol_route_marking", "field_vol_sag", "field_vol_sakura_close", "field_vol_sakura_food_prep", "field_vol_sakura_traffic");

//var_export($field->content_field['field_name']);

  if (in_array($field->content_field['field_name'], $arr)) {
    if ($output == "no")
      $output = "";
    else
      $output = "1";
  }
?>
<?php print $output; ?>