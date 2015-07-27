<?php

/**
* Define this Export UI plugin.
*/
$plugin = array(
  'schema' => 'ms_products_plans',
  'access' => 'administer moneyscripts settings',
  'create access' => 'administer moneyscripts settings',
  'delete access' => 'administer moneyscripts settings',
  'menu' => array(
    'menu item' => 'ms_products',
    'menu title' => 'Products',
    'menu description' => 'Administer Products',
  ),
  'title singular' => t('ms_products_plan'),
  'title plural' => t('ms_products_plans'),
  'title singular proper' => t('Product Plan'),
  'title plural proper' => t('Product Plans'),
  'form' => array(
    'settings' => 'ms_products_plan_form',
    'validate' => 'ms_products_plan_form_validate',
    'submit' => 'ms_products_plan_form_submit',
  ),
  'strings' => array(
    'confirmation' => array(
      'add' => array(
        'success' => '',
      ),
      'edit' => array(
        'success' => '',
      ),
    ),
  ),
  'handler' => array(
    'class' => 'ms_products_export_ui',
    'parent' => 'ctools_export_ui',
  ),
);
