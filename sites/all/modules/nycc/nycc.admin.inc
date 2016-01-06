<?php
function nycc_admin() {
  $form = array();
  $form['html'] = array(
  '#type' => 'markup',
  '#markup' => '<h2>This module has no settings.</h2>',
  );
  return system_settings_form($form);
}

