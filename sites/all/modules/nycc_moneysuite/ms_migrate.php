<?php
// ======================================
// Updates FROM MS_CORE
// ======================================
/**
 * Add the scope field to the adjustments database
 */

drupal_set_message("running ms_core update 7100");
  if (!db_field_exists('ms_order_adjustments', 'scope')) {
    db_add_field('ms_order_adjustments', 'scope', array(
      'description' => 'Whether the adjustment should apply to whole order or just first payment',
      'type' => 'varchar',
      'length' => 32,
      'not null' => TRUE,
      'default' => 'recurring',
    ));
    db_add_field('ms_cart_adjustments', 'scope', array(
      'description' => 'Whether the adjustment should apply to whole order or just first payment',
      'type' => 'varchar',
      'length' => 32,
      'not null' => TRUE,
      'default' => 'recurring',
    ));
  }
  drupal_set_message (t('Successfully added a scope column to the cart and order adjustment database tables.'));


/**
 * Add the total field to the ms_orders database
 */

drupal_set_message("running ms_core update 7101");

  if (!db_field_exists('ms_orders', 'total')) {
    db_add_field('ms_orders', 'total', array(
      'type' => 'numeric',
      'description' => 'The total amount that has been paid for the order',
      'not null' => TRUE,
      'default' => 0,
      'precision' => '10',
      'scale' => '2',
    ));
  }
  drupal_set_message( t('Successfully added the total column to the ms_orders database table.'));


/**
 * Add the order_key field to the ms_orders database
 */
drupal_set_message("running ms_core update 7102");


  if (!db_field_exists('ms_orders', 'order_key')) {
    db_add_field('ms_orders', 'order_key', array(
      'type' => 'varchar',
      'description' => 'Key for this order',
      'length' => '32',
      'not null' => TRUE,
      'default' => '',
    ));

    // Populate the order_key of existing orders
    $result = db_query("SELECT oid FROM {ms_orders}");
    foreach ($result as $row) {
      $key = ms_core_generate_order_key();
      db_update('ms_orders')
        ->fields(array(
          'order_key' => $key,
        ))
        ->condition('oid', $row->oid)
        ->execute();
    }
  }
  drupal_set_message( t('Successfully added the order_key column to the ms_orders database table.'));



/**
 * Add the first_name, last_name, and email_address fields to the database
 */
drupal_set_message("running ms_core update 7103");
  // Add new fields to the database
  if (!db_field_exists('ms_orders', 'first_name')) {
    db_add_field('ms_orders', 'first_name', array(
      'type' => 'varchar',
      'description' => 'First Name of the Purchaser',
      'length' => '64',
      'not null' => TRUE,
      'default' => '',
    ));
    db_add_field('ms_orders', 'last_name', array(
      'type' => 'varchar',
      'description' => 'Last Name of the Purchaser',
      'length' => '64',
      'not null' => TRUE,
      'default' => '',
    ));
    db_add_field('ms_orders', 'email_address', array(
      'type' => 'varchar',
      'description' => 'Email Address of the Purchaser',
      'length' => '255',
      'not null' => TRUE,
      'default' => '',
    ));

    // Populate the first_name and last_name and email_address of existing orders
    $result = db_query("SELECT * FROM {ms_orders}");
    foreach ($result as $row) {
      $last_name = '';
      $email = '';

      $name = explode(' ', $row->full_name);
      $first_name = $name[0];
      unset($name[0]);

      if (is_array($name)) {
        $last_name = implode(' ', $name);
      }

      if ($row->uid) {
        $account = user_load($row->uid);
        $email = $account->mail;
      }

      db_update('ms_orders')
        ->fields(array(
          'first_name' => $first_name,
          'last_name' => $last_name,
          'email_address' => $email,
        ))
        ->condition('oid', $row->oid)
        ->execute();
    }
  }

  drupal_set_message( t('Successfully added the first_name, last_name, and email_address columns to the ms_orders database table.'));


/**
 * Remove the full_name field from the database
 */
drupal_set_message("running ms_core update 7104");
  db_drop_field('ms_orders', 'full_name');

  drupal_set_message( t('Successfully dropped the full_name column from the ms_orders database table.'));


/**
 * Add the notified field to the ms_recurring_schedules table
 */
drupal_set_message("running ms_core update 7105");
  if (!db_field_exists('ms_recurring_schedules', 'notified')) {
    db_add_field('ms_recurring_schedules', 'notified', array(
      'description' => t('Whether the payment notification has been sent.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
      'default' => 0,
    ));

    db_update('ms_recurring_schedules')
      ->fields(array(
        'notified' => 1,
      ))
      ->execute();
  }

  drupal_set_message( t('Successfully added the notified field to the ms_recurring_schedules database table.'));


/**
 * Change the product id tables to use varchar
 */
drupal_set_message("running ms_core update 7106");

  db_query("ALTER TABLE {ms_order_products} MODIFY id VARCHAR(100)");
  db_query("ALTER TABLE {ms_cart_products} MODIFY id VARCHAR(100)");

  drupal_set_message( t('Successfully changed the id field to varchar.'));


/**
 * Attempt to update the licenses automatically
 */
drupal_set_message("running ms_core update 7107");


  if (!variable_get('ms_core_public_key', '')) {
    $modules = array('ms_membership', 'ms_paypublish', 'ms_affiliates', 'ms_files');

    foreach ($modules as $module) {
      if ($public_key = variable_get($module . '_public_key', '')) {
        break;
      }
    }

    if (!empty($public_key)) {
      $rec_data = xmlrpc('http://www.moneyscripts.net/xmlrpc.php', array(
        'moneyscripts.get_core_keys' => array($public_key),
      ));

      if ($rec_data) {
        if ($rec_data['public_key']) {
          variable_set('ms_core_public_key', $rec_data['public_key']);
          variable_set('ms_core_private_key', $rec_data['private_key']);

          return t('The Licensing system has changed. You now only need 1 Public and Private key pair for all of the MS Modules.
            Your keys have been automatically updated. Please verify them here: !link.
            You can find the new keys in the My Software area of your account on MoneyScripts.net.',
            array('!link' => l(t('MS Core Settings'), 'admin/moneyscripts/ms_core')));
        }
      }
    }
    return t('The Licensing system has changed. You now only need 1 Public and Private key pair for all of the MS Modules.
      Please update your keys here: !link. You can find the keys in the My Software area of your account on MoneyScripts.net.',
      array('!link' => l(t('MS Core Settings'), 'admin/moneyscripts/ms_core')));
  }



/**
 * Add the uid field to the ms_cart_products table
 */
drupal_set_message("running ms_core update 7201");

  if (!db_field_exists('ms_cart_products', 'uid')) {
    db_add_field('ms_cart_products', 'uid', array(
      'description' => t('The user id who owns the products.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
      'default' => 1,
    ));
  }

  drupal_set_message( t('Successfully added the uid field to the ms_cart_products database table.'));


/**
 * Add the uid field to the ms_order_products table
 */
drupal_set_message("running ms_core update 7202");


  if (!db_field_exists('ms_order_products', 'uid')) {
    db_add_field('ms_order_products', 'uid', array(
      'description' => t('The user id who owns the products.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
      'default' => 1,
    ));
  }

  drupal_set_message( t('Successfully added the uid field to the ms_order_products database table.'));


/**
 * Change the product id tables to use varchar 128
 */
drupal_set_message("running ms_core update 7203");

function ms_core_update_7203(&$sandbox) {
  db_query("ALTER TABLE {ms_order_products} MODIFY id VARCHAR(128)");
  db_query("ALTER TABLE {ms_cart_products} MODIFY id VARCHAR(128)");

  drupal_set_message( t('Successfully changed the id field to varchar 128.'));


/**
 * Add the remote_id field to the ms_core_payment_profiles table
 */
drupal_set_message("running ms_core update 7204");


  if (!db_field_exists('ms_core_payment_profiles', 'remote_id')) {
    db_add_field('ms_core_payment_profiles', 'remote_id', array(
      'description' => 'Remote ID',
      'type' => 'varchar',
      'length' => 255,
      'default' => '',
    ));
  }

  drupal_set_message( t('Successfully added the remote_id field to the ms_core_payment_profiles database table.'));


/**
 * Update permissions to access checkout page
 */
drupal_set_message("running ms_core update 7205");


  $result = db_query("SELECT * FROM {role_permission} WHERE permission = :perm", array(':perm' => 'access content'));

  foreach ($result as $row) {
    db_merge('role_permission')
      ->key(array(
        'rid' => $row->rid,
        'permission' => 'access checkout page',
      ))
      ->fields(array(
        'module' => 'ms_core',
      ))
      ->execute();
  }

  // Clear the user access cache.
  drupal_static_reset('user_access');
  drupal_static_reset('user_role_permissions');

  drupal_set_message( t('Updated permissions to access checkout page.'));


/**
 * Add the product_id fields
 */
drupal_set_message("running ms_core update 7206");

  if (!db_field_exists('ms_order_adjustments', 'product_id')) {
    db_add_field('ms_order_adjustments', 'product_id', array(
      'description' => t('The product id associated with the adjustment.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
      'default' => NULL,
    ));
  }

  if (!db_field_exists('ms_cart_adjustments', 'product_id')) {
    db_add_field('ms_cart_adjustments', 'product_id', array(
      'description' => t('The product id associated with the adjustment.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
      'default' => NULL,
    ));
  }

  drupal_set_message( t('Added the product_id fields.'));


/**
 * Add the active and optional fields to adjustments.
 */
drupal_set_message("running ms_core update 7301");

  if (!db_field_exists('ms_order_adjustments', 'active')) {
    db_add_field('ms_order_adjustments', 'active', array(
      'description' => t('Whether the adjustment is active.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
      'default' => 1,
    ));
    db_add_field('ms_order_adjustments', 'optional', array(
      'description' => t('Whether the adjustment is optional.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
      'default' => 0,
    ));
  }

  if (!db_field_exists('ms_cart_adjustments', 'active')) {
    db_add_field('ms_cart_adjustments', 'active', array(
      'description' => t('Whether the adjustment is active.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
      'default' => 1,
    ));
    db_add_field('ms_cart_adjustments', 'optional', array(
      'description' => t('Whether the adjustment is optional.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
      'default' => 0,
    ));
  }

  drupal_set_message( t('Added the optional and active fields for adjustments.'));


/**
 * Create the new tables for countries and regions.
 */
drupal_set_message("running ms_core update 7302");
  $schema['ms_core_countries'] = array(
    'description' => 'Used to hold countries',
    'fields' => array(
      'iso' => array(
        'type' => 'char',
        'description' => 'The ISO code for the country',
        'not null' => TRUE,
        'length' => '2',
      ),
      'name' => array(
        'type' => 'varchar',
        'description' => 'The name of the country',
        'length' => '80',
      ),
      'numcode' => array(
        'type' => 'int',
        'description' => 'The Number Code for the Country',
        'disp_width' => '11',
      ),
      'currency' => array(
        'type' => 'varchar',
        'description' => 'The currency of the country',
        'length' => '3',
      ),
    ),
    'primary key' => array('iso'),
  );
  $schema['ms_core_regions'] = array(
    'description' => 'Used to hold regions',
    'fields' => array(
      'code' => array(
        'type' => 'char',
        'description' => 'The code for the region',
        'length' => '2',
      ),
      'name' => array(
        'type' => 'varchar',
        'description' => 'The name of the region',
        'length' => '80',
      ),
      'numcode' => array(
        'type' => 'int',
        'description' => 'The Number Code for the Country',
        'disp_width' => '11',
      ),
    ),
  );

  // First, drop the old table.
  db_drop_table('ms_core_countries');

  // Then create the new tables.
  if (!db_table_exists('ms_core_countries')) {
    db_create_table('ms_core_countries', $schema['ms_core_countries']);
  }
  if (!db_table_exists('ms_core_regions')) {
    db_create_table('ms_core_regions', $schema['ms_core_regions']);
  }


/**
 * Import the countries and regions.
 */
drupal_set_message("running ms_core update 7303");
  db_truncate('ms_core_countries')->execute();
  db_truncate('ms_core_regions')->execute();

  // Now import the data.
  module_load_include('inc', 'ms_core', 'ms_core.countries');
  $countries = _ms_core_get_countries_data();

  foreach ($countries as $raw_values) {
    $map = array('name', 'iso', 'numcode', 'currency');
    $country = array_combine($map, $raw_values);
    db_insert('ms_core_countries')
      ->fields($country)
      ->execute();
  }

  module_load_include('inc', 'ms_core', 'ms_core.regions');
  $regions = _ms_core_get_regions_data();

  foreach ($regions as $raw_values) {
    $map = array('name', 'code', 'numcode');
    $region = array_combine($map, $raw_values);
    db_insert('ms_core_regions')
      ->fields($region)
      ->execute();
  }


/**
 * Update payment types.
 */
	drupal_set_message("running ms_core update 7304");


  db_update('ms_payments')
    ->fields(array(
      'type' => 'refund',
    ))
    ->condition('type', 'rec_refund')
    ->execute();
  db_update('ms_payments')
    ->fields(array(
      'type' => 'failed',
    ))
    ->condition('type', 'rec_failed')
    ->execute();
  db_update('ms_payments')
    ->fields(array(
      'type' => 'reversal',
    ))
    ->condition('type', 'rec_reversal')
    ->execute();


/**
 * Add the pid field to ms_recurring_schedules table.
 */
drupal_set_message("running ms_core update 7305");

  if (!db_field_exists('ms_recurring_schedules', 'pid')) {
    db_add_field('ms_recurring_schedules', 'pid', array(
      'description' => 'The product ID.',
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
    ));
  }

  drupal_set_message( t('Added the pid field to the ms_recurring_schedules table.'));


/**
 * Remove the recurring_schedule and secured fields.
 */
drupal_set_message("running ms_core update 7306");
  if (db_field_exists('ms_payments', 'recurring_schedule')) {
    db_drop_field('ms_payments', 'recurring_schedule');
  }
  if (db_field_exists('ms_orders', 'secured')) {
    db_drop_field('ms_orders', 'secured');
  }

  drupal_set_message( t('Removed the recurring_schedule and secured fields.'));


/**
 * Remove duplicate recurring_schedule records.
 *
 * Due to a bug in the new multi_recurring feature, there is a chance there could be duplicate recurring_schedule
 * records for recurring orders. We remove any duplicates here.
 */
drupal_set_message("running ms_core update 7307");
  $result = db_query("SELECT * FROM {ms_recurring_schedules} a
INNER JOIN (SELECT oid FROM {ms_recurring_schedules}
GROUP BY oid HAVING count(id) > 1) dup ON a.oid = dup.oid WHERE a.status = :status", array(':status' => 'active'));

  $all = array();
  foreach ($result as $row) {
    $all[$row->oid][$row->id] = $row;
  }
  foreach ($all as $records) {
    array_pop($records);
    foreach ($records as $row) {
      db_update('ms_recurring_schedules')
        ->fields(array(
          'status' => 'inactive',
        ))
        ->condition('id', $row->id)
        ->execute();
    }
  }


/**
 * Add a notified field for payment profiles.
 */
drupal_set_message("running ms_core update 7308");
  db_add_field('ms_core_payment_profiles', 'notified', array(
    'description' => t('Whether the payment notification has been sent.'),
    'type' => 'int',
    'unsigned' => TRUE,
    'not null' => TRUE,
    'default' => 0,
  ));




/**
 * Make sure there aren't any old tokens from Drupal 6.
 */
drupal_set_message("running ms_core update 7309");
  if (function_exists('token_update_token_text')) {
    $replacements = array(
      'userName' => 'user:name',
    );
    $variables = array();
    $messages = array();

    $settings_functions = module_invoke_all('ms_core_overrides');

    if (is_array($settings_functions) && !empty($settings_functions)) {
      foreach ($settings_functions as $module => $info) {
        if (function_exists($module . '_token_info')) {
          $tokens_info = call_user_func($module . '_token_info');

          foreach ($tokens_info['tokens'] as $type => $tokens) {
            foreach ($tokens as $token_key => $token_info) {
              $replacements[$token_key] = $type . ':' . $token_key;
            }
          }
        }

        $t_form_state = form_state_defaults();
        if (!empty($info['params'])) {
          $t_form_state['build_info']['args'] = $info['params'];
        }
        $module_form = drupal_retrieve_form($info['form'], $t_form_state);

        $settings = array();
        _ms_core_extract_token_variables($module_form, $settings);

        if (!empty($settings)) {
          foreach ($settings as $key => $element) {
            $variables[] = $key;
          }
        }
      }
    }

    $variables_count = 0;

    foreach ($variables as $variable) {
      if ($string = variable_get($variable, '')) {
        $new_string = token_update_token_text($string, $replacements);
        if ($new_string != $string) {
          variable_set($variable, $new_string);
          $variables_count += 1;
        }
      }
    }

    $messages[] = t('Updated tokens in @count variables.', array('@count' => $variables_count));

    // Now we need to update all of the overridden settings in the orders.
    $result = db_select('ms_orders', 'o')
      ->fields('o')
      ->execute();

    $orders_count = 0;
    foreach ($result as $order) {
      $order->data = unserialize($order->data);
      $changed = FALSE;
      if (!empty($order->data['override_settings'])) {
        foreach ($order->data['override_settings'] as $key => &$string) {
          if (in_array($key, $variables)) {
            $new_string = token_update_token_text($string, $replacements);
            if ($new_string != $string) {
              $string = $new_string;
              $changed = TRUE;
            }
          }
        }
      }

      if ($changed) {
        db_update('ms_orders')
          ->fields(array(
            'data' => $order->data,
          ))
          ->condition('oid', $order->oid)
          ->execute();
        $orders_count += 1;
      }
    }

    $messages[] = t('Updated tokens in override settings of @count orders.', array('@count' => $orders_count));

    return implode('<br />', $messages);
  } else {
    throw new DrupalUpdateException('There was an error updating the tokens. You may need to perform the update manually.');
  }




// ======================================
// Updates FROM MS_PRODUCTS:
// ======================================

/**
 * Add the uid field to the product plans table
 */
drupal_set_message("running ms_products update 7101");

  if (!db_field_exists('ms_products_plans', 'uid')) {
    db_add_field('ms_products_plans', 'uid', array(
      'description' => t('The user id who owns the products.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
      'default' => 1,
    ));
  }

  drupal_set_message( t('Successfully added the uid field to the ms_products_plans database table.'));


/**
 * Add the allow_roles, deny_roles, and expire_when fields to the ms_products_plans table
 */
drupal_set_message("running ms_products update 7301");
  if (!db_field_exists('ms_products_plans', 'allow_roles')) {
    db_add_field('ms_products_plans', 'allow_roles', array(
      'type' => 'text',
      'description' => 'Which Roles can Purchase/Upgrade this role',
      'serialize' => TRUE,
    ));
  }
  if (!db_field_exists('ms_products_plans', 'deny_roles')) {
    db_add_field('ms_products_plans', 'deny_roles', array(
      'type' => 'text',
      'description' => 'Which Roles can not Purchase/Upgrade this role',
      'serialize' => TRUE,
    ));
  }
  if (!db_field_exists('ms_products_plans', 'expire_when')) {
    db_add_field('ms_products_plans', 'expire_when', array(
      'type' => 'varchar',
      'description' => 'When to remove the role',
      'length' => '32',
      'default' => 'subscr_eot',
    ));
  }

  // Set the default for all of the product plans for allow_roles and deny_roles and machine_name
  $result = db_query("SELECT * FROM {ms_products_plans}");
  foreach ($result as $plan) {
    $plan->allow_roles = array();
    $plan->deny_roles = array();
    db_update('ms_products_plans')
      ->fields(array(
        'allow_roles' => serialize($plan->allow_roles),
        'deny_roles' => serialize($plan->deny_roles),
      ))
      ->condition('pid', $plan->pid)
      ->execute();
  }
  drupal_set_message( t('Successfully added the new fields to the ms_products_plans database table.'));


/**
 * Make sure the arrays are serialized correctly.
 */
	drupal_set_message("running ms_products update 7302");
  $result = db_query("SELECT * FROM {ms_products_plans}");
  foreach ($result as $plan) {
    $plan->allow_roles = _ms_products_unserialize($plan->allow_roles);
    $plan->deny_roles = _ms_products_unserialize($plan->deny_roles);
    $plan->modify_options = _ms_products_unserialize($plan->modify_options);
    $plan->recurring_schedule = _ms_products_unserialize($plan->recurring_schedule);
    $plan->data = _ms_products_unserialize($plan->data);
    db_update('ms_products_plans')
      ->fields(array(
        'allow_roles' => serialize($plan->allow_roles),
        'deny_roles' => serialize($plan->deny_roles),
        'modify_options' => serialize($plan->modify_options),
        'recurring_schedule' => serialize($plan->recurring_schedule),
        'data' => serialize($plan->data),
      ))
      ->condition('pid', $plan->pid)
      ->execute();
  }


/**
 * Make sure sku is set and used for the purchases and add fixed date support.
 */
drupal_set_message("running ms_products update 7304");

  $schema['ms_product_purchases'] = array(
    'description' => 'Used to record all product purchases',
    'fields' => array(
      'id' => array(
        'type' => 'serial',
        'description' => 'The unique ID (primary)',
        'not null' => TRUE,
      ),
      'oid' => array(
        'type' => 'int',
        'description' => 'The unique order ID',
        'not null' => TRUE,
      ),
      'uid' => array(
        'type' => 'int',
        'description' => 'The uid of the user making the record',
        'not null' => TRUE,
      ),
      'bundle' => array(
        'type' => 'varchar',
        'description' => 'The type of the Product',
        'length' => '255',
        'not null' => TRUE,
        'default' => '',
      ),
      'sku' => array(
        'type' => 'varchar',
        'description' => 'The machine name of the product plan.',
        'length' => '255',
        'not null' => TRUE,
        'default' => '',
      ),
      'status' => array(
        'type' => 'varchar',
        'description' => 'The Status of the Purchase',
        'length' => '255',
      ),
      'expiration' => array(
        'type' => 'int',
        'description' => 'The Expiration of the Purchase',
        'not null' => TRUE,
        'default' => 0,
      ),
      'start_date' => array(
        'type' => 'int',
        'description' => 'Start Date for the Purchase',
        'not null' => TRUE,
        'default' => 0,
      ),
      'current_payments' => array(
        'type' => 'int',
        'description' => 'The current number of payments made',
        'not null' => TRUE,
        'default' => 0,
      ),
      'max_payments' => array(
        'type' => 'int',
        'description' => 'The max number of payments for the Purchase',
        'not null' => TRUE,
        'default' => 0,
      ),
      'data' => array(
        'type' => 'text',
        'description' => 'Serialized Array of Data for the purchase',
        'serialize' => TRUE,
      ),
    ),
    'primary key' => array('id'),
  );
  if (!db_table_exists('ms_product_purchases')) {
    db_create_table('ms_product_purchases', $schema['ms_product_purchases']);
  }

  // Rename table to ms_products_purchases.
  if (db_table_exists('ms_products_purchases')) {
    db_drop_table('ms_products_purchases');
  }
  db_rename_table('ms_product_purchases', 'ms_products_purchases');
  if (db_field_exists('ms_products_plans', 'type')) {
    db_change_field('ms_products_plans', 'type', 'cart_type', array(
      'type' => 'varchar',
      'description' => 'cart or recurring',
      'length' => '100',
      'not null' => TRUE,
    ));
    db_change_field('ms_products_plans', 'module', 'bundle', array(
      'type' => 'varchar',
      'description' => 'The type of the Product',
      'length' => '255',
      'not null' => TRUE,
      'default' => '',
    ));
    db_change_field('ms_products_purchases', 'pid', 'sku', array(
      'type' => 'varchar',
      'description' => 'The machine name of the product plan.',
      'length' => '255',
      'not null' => TRUE,
      'default' => '',
    ));
  }

  if (db_field_exists('ms_products_plans', 'machine_name')) {
    db_drop_field('ms_products_plans', 'machine_name');
  }

  if (!db_field_exists('ms_products_purchases', 'bundle')) {
    db_add_field('ms_products_purchases', 'bundle', array(
      'type' => 'varchar',
      'description' => 'The type of the Product',
      'length' => '255',
      'not null' => TRUE,
      'default' => '',
    ));
  }
  if (!db_field_exists('ms_products_purchases', 'data')) {
    db_add_field('ms_products_purchases', 'data', array(
      'type' => 'text',
      'description' => 'Serialized Array of Data for the purchase',
      'serialize' => TRUE,
    ));
  }

  // Add the new tables.
  $schema['ms_products_plan_options'] = array(
    'description' => 'Options associated with plans.',
    'fields' => array(
      'id' => array(
        'type' => 'serial',
        'description' => 'The unique ID (primary)',
        'not null' => TRUE,
      ),
      'sku' => array(
        'type' => 'varchar',
        'description' => 'The machine name of the product plan.',
        'length' => '255',
        'not null' => TRUE,
        'default' => '',
      ),
      'name' => array(
        'type' => 'varchar',
        'description' => 'The machine name of the option.',
        'length' => '255',
        'not null' => TRUE,
        'default' => '',
      ),
      'title' => array(
        'type' => 'varchar',
        'description' => 'The title of the option.',
        'length' => '255',
      ),
      'description' => array(
        'type' => 'varchar',
        'description' => 'The description of the option.',
        'length' => '255',
      ),
      'optional' => array(
        'type' => 'int',
        'description' => 'Whether or not users are required to purchase the option.',
        'not null' => TRUE,
        'default' => 1,
      ),
      'default_value' => array(
        'type' => 'int',
        'description' => 'Whether or not the option is selected by default.',
        'not null' => TRUE,
        'default' => 1,
      ),
      'amount' => array(
        'type' => 'numeric',
        'description' => 'The fee for the option.',
        'precision' => '10',
        'scale' => '2',
        'not null' => TRUE,
        'default' => 0.00,
      ),
      'expiration' => array(
        'type' => 'int',
        'description' => 'How long until the option expires.',
        'not null' => TRUE,
        'default' => 0,
      ),
    ),
    'primary key' => array('id'),
  );

  $schema['ms_products_purchase_options'] = array(
    'description' => 'Options associated with purchases.',
    'fields' => array(
      'id' => array(
        'type' => 'serial',
        'description' => 'The unique ID (primary)',
        'not null' => TRUE,
      ),
      'pid' => array(
        'type' => 'int',
        'description' => 'The ID of the purchase.',
        'not null' => TRUE,
      ),
      'name' => array(
        'type' => 'varchar',
        'description' => 'The machine name of the option.',
        'length' => '255',
        'not null' => TRUE,
        'default' => '',
      ),
      'status' => array(
        'type' => 'varchar',
        'description' => 'The status of the option.',
        'length' => '255',
      ),
      'expiration' => array(
        'type' => 'int',
        'description' => 'How long until the option expires.',
        'not null' => TRUE,
        'default' => 0,
      ),
    ),
    'primary key' => array('id'),
  );

  if (db_table_exists('ms_products_plan_options')) {
    db_drop_table('ms_products_plan_options');
  }
  if (db_table_exists('ms_products_purchase_options')) {
    db_drop_table('ms_products_purchase_options');
  }

  db_create_table('ms_products_plan_options', $schema['ms_products_plan_options']);
  db_create_table('ms_products_purchase_options', $schema['ms_products_purchase_options']);

  // Set the sku of the purchases table to the sku of the product plans table.
  $result = db_query("SELECT * FROM {ms_products_plans}");
  foreach ($result as $plan) {
    db_update('ms_products_purchases')
      ->fields(array(
        'sku' => $plan->sku,
        'bundle' => $plan->bundle,
      ))
      ->condition('sku', $plan->pid)
      ->execute();
  }

  $skus = array();
  module_load_include('inc', 'ms_products', 'ms_products.plans');
  foreach (ms_products_get_plans() as $plan) {
    $skus[$plan->pid] = $plan->sku;
    ms_core_product_id_change($plan->bundle . '-' . $plan->pid, $plan->bundle . '-' . $plan->sku);
    if (module_exists('i18n_string')) {
      module_load_install('i18n_string');
      i18n_string_install_update_context('ms_products_plan:plan:' . $plan->pid . ':*', 'ms_products_plan:plan:' . $plan->sku . ':*');
    }
  }

  // Migrate rules from pid to sku.
  if (module_exists('rules')) {
    // It's too hard to migrate rules because of dependencies, so just have
    // users manually migrate rules if there are any.
    // Unfortunately this can't be helped.
    //_ms_products_migrate_rules($skus);
  }

  // Clear the schema cache.
  cache_clear_all('schema', 'cache');

  drupal_set_message( t('Successfully changed the sku of purchases to point to sku.'));


/**
 * Migrate the modify options field.
 */
drupal_set_message("running ms_products update 7305");
  $result = db_query("SELECT * FROM {ms_products_plans}");
  foreach ($result as $plan) {
    $modify_options = _ms_products_unserialize($plan->modify_options);
    $plan->modify_options = array(
      'upgrade' => $modify_options,
      'downgrade' => array(),
    );
    db_update('ms_products_plans')
      ->fields(array(
        'modify_options' => serialize($plan->modify_options),
      ))
      ->condition('pid', $plan->pid)
      ->execute();
  }

  drupal_set_message( t('Successfully updated the modify options field.'));



/**
 * Add new fields for Configurable Plan Options.
 */
drupal_set_message("running ms_products update 7306");
  if (db_field_exists('ms_products_plan_options', 'widget')) {
    db_drop_field('ms_products_plan_options', 'widget');
  }
  if (!db_field_exists('ms_products_plan_options', 'widget')) {
    db_add_field('ms_products_plan_options', 'widget', array(
      'type' => 'varchar',
      'description' => 'The widget used.',
      'length' => '32',
      'not null' => TRUE,
      'default' => 'checkbox',
    ));
  }
  if (db_field_exists('ms_products_plan_options', 'sub_options')) {
    db_drop_field('ms_products_plan_options', 'sub_options');
  }

  if (!db_field_exists('ms_products_plan_options', 'sub_options')) {
    db_add_field('ms_products_plan_options', 'sub_options', array(
      'type' => 'varchar',
      'description' => 'The sub option choices.',
      'length' => '512',
      'not null' => TRUE,
      'default' => '',
    ));
  }
  if (db_field_exists('ms_products_purchase_options', 'sub_option')) {
    db_drop_field('ms_products_purchase_options', 'sub_option');
  }

  if (!db_field_exists('ms_products_purchase_options', 'sub_option')) {
    db_add_field('ms_products_purchase_options', 'sub_option', array(
      'type' => 'varchar',
      'description' => 'The value of the selected option.',
      'length' => '255',
      'not null' => TRUE,
      'default' => '',
    ));
  }

  drupal_set_message( t('Successfully added fields for configurable plan options.'));


/**
 * Add new pid field for Purchases.
 */
drupal_set_message("running ms_products update 7307");
  if (db_field_exists('ms_products_purchases', 'pid')) {
    db_drop_field('ms_products_purchases', 'pid');
  }
  if (!db_field_exists('ms_products_purchases', 'pid')) {
    db_add_field('ms_products_purchases', 'pid', array(
      'type' => 'int',
      'description' => 'The unique product ID (primary)',
      'not null' => TRUE,
      'default' => 0,
    ));
  }

  // Populate the existing purchases.
  $result = db_query("SELECT * FROM {ms_products_purchases}");
  foreach ($result as $purchase) {
    $plan = ms_products_plan_load($purchase->sku);
    if ($plan) {
      db_update('ms_products_purchases')
        ->fields(array(
          'pid' => $plan->pid,
        ))
        ->condition('id', $purchase->id)
        ->execute();
    }
  }

  drupal_set_message( t('Successfully added pid field for purchases.'));




// ======================================
// Updates: from ms_memberships
// ======================================

/**
 * Migrate Override Settings
 */
drupal_set_message("running ms_membership update 7701");

  $result = db_query("SELECT mpid, name FROM {ms_membership_plans} ORDER BY weight ASC");

  foreach ($result as $product) {
    $product->data = unserialize($product->data);

    if (isset($product->data['override_settings']) AND is_array($product->data['override_settings'])) {
      $old_override_settings = $product->data['override_settings'];
      $new_override_settings = array();

      foreach ($old_override_settings as $info) {
        if ($info['override']) {
          foreach ($info['settings'] as $key => $value) {
            if (is_array($value)) {
              foreach ($value as $child_key => $child_value) {
                if (!is_array($child_value)) {
                  if ($child_value != variable_get($child_key, '')) {
                    $new_override_settings[$child_key] = $child_value;
                  }
                }
              }
            }
            else {
              if ($value != variable_get($key, '')) {
                $new_override_settings[$key] = $value;
              }
            }
          }
        }
      }

      $product->data['override_settings'] = $new_override_settings;

      db_update('ms_membership_plans')
        ->fields(array(
          'data' => serialize($product->data),
        ))
        ->condition('mpid', $product->mpid)
        ->execute();
    }
  }


/**
 * Add the uid field to the ms_membership_plans table
 */
drupal_set_message("running ms_membership update 7702");
  if (!db_field_exists('ms_membership_plans', 'uid')) {
    db_add_field('ms_membership_plans', 'uid', array(
      'description' => t('The user id who owns the products.'),
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
      'default' => 1,
    ));
  }

  drupal_set_message( t('Successfully added the uid field to the ms_membership_plans database table.'));




/**
 * Add the deny_roles field to the ms_membership_plans table
 */
drupal_set_message("running ms_membership update 7703");
  if (!db_field_exists('ms_membership_plans', 'deny_roles')) {
    db_add_field('ms_membership_plans', 'deny_roles', array(
      'description' => t('Which Roles can not Purchase/Upgrade this role'),
      'type' => 'text',
      'serialize' => TRUE,
    ));

    // Set the default to an empty array
    db_update('ms_membership_plans')
      ->fields(array(
        'deny_roles' => serialize(array()),
      ))
      ->execute();
  }
  drupal_set_message( t('Successfully added the deny_roles field to the ms_membership_plans database table.'));


/**
 * Add the fixed_date field to the ms_membership_plans table
 */
drupal_set_message("running ms_membership update 7704");
  if (!db_field_exists('ms_membership_plans', 'fixed_date')) {
    db_add_field('ms_membership_plans', 'fixed_date', array(
      'type' => 'int',
      'description' => 'The Subscription is Fixed Date',
      'not null' => TRUE,
      'default' => 0,
    ));
    db_add_field('ms_membership_plans', 'fixed_date_discount', array(
      'type' => 'int',
      'description' => 'If a discount should be applied.',
      'not null' => TRUE,
      'default' => 1,
    ));
    db_add_field('ms_membership_plans', 'fixed_date_string', array(
      'type' => 'varchar',
      'description' => 'Fixed date string',
      'length' => '255',
      'default' => '',
    ));
    db_add_field('ms_membership_plans', 'fixed_date_type', array(
      'type' => 'varchar',
      'description' => 'Fixed date type',
      'length' => '255',
      'default' => '',
    ));
  }
  drupal_set_message( t('Successfully added the fixed_date field to the ms_membership_plans database table.'));


/**
 * Rebuild the memberships to fix the current payments counter
 */
drupal_set_message("running ms_membership update 7705");
  module_load_include('module', 'ms_core');
  module_load_include('inc', 'ms_core', 'ms_core.recurring');
  module_load_include('php', 'ms_core', 'ms_core.classes');
  // Update the current payments for all of the memberships to match the recurring schedules for the order
  $result = db_query("SELECT * FROM {ms_memberships}");
  foreach ($result as $membership) {
    if ($membership->oid AND $order = ms_core_order_load($membership->oid)
      AND $recurring_schedule = ms_core_load_recurring_schedule($order->oid)
      AND !empty($recurring_schedule->current_payments)
    ) {
      db_update('ms_memberships')
        ->fields(array(
          'current_payments' => $recurring_schedule->current_payments,
        ))
        ->condition('oid', $order->oid)
        ->execute();
    }
  }

  drupal_set_message( t('Rebuilt the memberships to fix the current payments counter.'));



// ======================================
// Updates: from Authorize.net Gateway
// ======================================

/**
 * Add the notified field to the payment profiles table.
 */
drupal_set_message("running ms_authorizenet update 7101");

  if (!db_field_exists('ms_authorizenet_payment_profiles', 'notified')) {
    db_add_field('ms_authorizenet_payment_profiles', 'notified', array(
      'type' => 'int',
      'description' => 'When the user was last notified of card expiration',
      'disp_width' => '11',
      'not null' => TRUE,
      'default' => 0,
    ));

    $notified = variable_get('ms_authorizenet_last_expiration_reminder', 0);
    db_query("UPDATE {ms_authorizenet_payment_profiles} SET notified = :notified", array(':notified' => $notified));
    variable_del('ms_authorizenet_last_expiration_reminder');
  }

  drupal_set_message( t('Successfully added the notified field to the ms_authorizenet_payment_profiles database table.'));




// ======================================
// Updates: from PayPal WPS Gateway
// ======================================
/**
 * Set the default language encoding to UTF-8
 */
	drupal_set_message("running ms_wps update 7200");


  variable_set('ms_paypal_wps_charset', 'UTF-8');

  drupal_set_message( t('Updated the language encoding variable for PayPal WPS.'));








//HELPER FUNCTIONS



/**
 * Attempts to fully unserialize a string that may have been serialized multiple times.
 */
function _ms_products_unserialize($object) {
  for ($i = 0; $i < 6; $i++) {
    if (is_array($object)) {
      return $object;
    }
    $object = unserialize($object);
  }

  return array();
}


/**
 * Recursively extracts the textarea form elements.
 */
function _ms_core_extract_token_variables($parent, &$settings) {
  foreach (element_children($parent) as $child_name) {
    if (!empty($parent[$child_name]['#type']) && $parent[$child_name]['#type'] == 'textarea') {
      $settings[$child_name] = $parent[$child_name];
    }
    _ms_core_extract_token_variables($parent[$child_name], $settings);
  }
}
