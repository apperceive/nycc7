<?php

function nycc_email_views_default_views() {

	$view = new view;
	$view->name = 'add_token_to_ride';
	$view->description = 'Add token to rides';
	$view->tag = '';
	$view->base_table = 'node';
	$view->core = 6;
	$view->api_version = '2';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	$handler = $view->new_display('default', 'Defaults', 'default');
	$handler->override_option('fields', array(
	  'nid' => array(
		'label' => 'Nid',
		'alter' => array(
		  'alter_text' => 0,
		  'text' => '',
		  'make_link' => 0,
		  'path' => '',
		  'absolute' => 0,
		  'link_class' => '',
		  'alt' => '',
		  'rel' => '',
		  'prefix' => '',
		  'suffix' => '',
		  'target' => '',
		  'help' => '',
		  'trim' => 0,
		  'max_length' => '',
		  'word_boundary' => 1,
		  'ellipsis' => 1,
		  'html' => 0,
		  'strip_tags' => 0,
		),
		'empty' => '',
		'hide_empty' => 0,
		'empty_zero' => 0,
		'hide_alter_empty' => 1,
		'link_to_node' => 0,
		'exclude' => 0,
		'id' => 'nid',
		'table' => 'node',
		'field' => 'nid',
		'relationship' => 'none',
	  ),
	  'title' => array(
		'label' => 'Title',
		'alter' => array(
		  'alter_text' => 0,
		  'text' => '',
		  'make_link' => 0,
		  'path' => '',
		  'absolute' => 0,
		  'link_class' => '',
		  'alt' => '',
		  'rel' => '',
		  'prefix' => '',
		  'suffix' => '',
		  'target' => '',
		  'help' => '',
		  'trim' => 0,
		  'max_length' => '',
		  'word_boundary' => 1,
		  'ellipsis' => 1,
		  'html' => 0,
		  'strip_tags' => 0,
		),
		'empty' => '',
		'hide_empty' => 0,
		'empty_zero' => 0,
		'hide_alter_empty' => 1,
		'link_to_node' => 1,
		'exclude' => 0,
		'id' => 'title',
		'table' => 'node',
		'field' => 'title',
		'relationship' => 'none',
	  ),
	  'field_ride_token_value' => array(
		'label' => 'approval token',
		'alter' => array(
		  'alter_text' => 0,
		  'text' => '',
		  'make_link' => 0,
		  'path' => '',
		  'absolute' => 0,
		  'link_class' => '',
		  'alt' => '',
		  'rel' => '',
		  'prefix' => '',
		  'suffix' => '',
		  'target' => '',
		  'help' => '',
		  'trim' => 0,
		  'max_length' => '',
		  'word_boundary' => 1,
		  'ellipsis' => 1,
		  'html' => 0,
		  'strip_tags' => 0,
		),
		'empty' => '',
		'hide_empty' => 0,
		'empty_zero' => 0,
		'hide_alter_empty' => 1,
		'link_to_node' => 0,
		'label_type' => 'widget',
		'format' => 'plain',
		'multiple' => array(
		  'group' => TRUE,
		  'multiple_number' => '',
		  'multiple_from' => '',
		  'multiple_reversed' => FALSE,
		),
		'exclude' => 0,
		'id' => 'field_ride_token_value',
		'table' => 'node_data_field_ride_token',
		'field' => 'field_ride_token_value',
		'relationship' => 'none',
	  ),
	  'field_ride_status_value' => array(
		'label' => 'Ride Status',
		'alter' => array(
		  'alter_text' => 0,
		  'text' => '',
		  'make_link' => 0,
		  'path' => '',
		  'absolute' => 0,
		  'link_class' => '',
		  'alt' => '',
		  'rel' => '',
		  'prefix' => '',
		  'suffix' => '',
		  'target' => '',
		  'help' => '',
		  'trim' => 0,
		  'max_length' => '',
		  'word_boundary' => 1,
		  'ellipsis' => 1,
		  'html' => 0,
		  'strip_tags' => 0,
		),
		'empty' => '',
		'hide_empty' => 0,
		'empty_zero' => 0,
		'hide_alter_empty' => 1,
		'link_to_node' => 0,
		'label_type' => 'widget',
		'format' => 'default',
		'multiple' => array(
		  'group' => TRUE,
		  'multiple_number' => '',
		  'multiple_from' => '',
		  'multiple_reversed' => FALSE,
		),
		'exclude' => 0,
		'id' => 'field_ride_status_value',
		'table' => 'node_data_field_ride_status',
		'field' => 'field_ride_status_value',
		'relationship' => 'none',
	  ),
	));
	$handler->override_option('filters', array(
	  'type' => array(
		'operator' => 'in',
		'value' => array(
		  'rides' => 'rides',
		),
		'group' => '0',
		'exposed' => FALSE,
		'expose' => array(
		  'operator' => FALSE,
		  'label' => '',
		),
		'id' => 'type',
		'table' => 'node',
		'field' => 'type',
		'relationship' => 'none',
	  ),
	  'field_ride_status_value_many_to_one' => array(
		'operator' => 'or',
		'value' => array(
		  'Approved' => 'Approved',
		  'Revised' => 'Revised',
		  'Submitted' => 'Submitted',
		),
		'group' => '0',
		'exposed' => FALSE,
		'expose' => array(
		  'operator' => FALSE,
		  'label' => '',
		),
		'id' => 'field_ride_status_value_many_to_one',
		'table' => 'node_data_field_ride_status',
		'field' => 'field_ride_status_value_many_to_one',
		'relationship' => 'none',
		'reduce_duplicates' => 1,
	  ),
	  'views_or_begin' => array(
		'id' => 'views_or_begin',
		'table' => 'views_or',
		'field' => 'views_or_begin',
	  ),
	  'field_ride_token_value' => array(
		'operator' => '=',
		'value' => '[nycc_email_ride-auth-token]',
		'group' => '0',
		'exposed' => FALSE,
		'expose' => array(
		  'operator' => FALSE,
		  'label' => '',
		  'optional' => FALSE,
		),
		'case' => 1,
		'id' => 'field_ride_token_value',
		'table' => 'node_data_field_ride_token',
		'field' => 'field_ride_token_value',
		'relationship' => 'none',
	  ),
	  'views_or_next' => array(
		'id' => 'views_or_next',
		'table' => 'views_or',
		'field' => 'views_or_next',
	  ),
	  'field_ride_token_value_1' => array(
		'operator' => 'empty',
		'value' => '',
		'group' => '0',
		'exposed' => FALSE,
		'expose' => array(
		  'operator' => FALSE,
		  'label' => '',
		  'optional' => FALSE,
		),
		'case' => 1,
		'id' => 'field_ride_token_value_1',
		'table' => 'node_data_field_ride_token',
		'field' => 'field_ride_token_value',
		'relationship' => 'none',
	  ),
	  'views_or_end' => array(
		'operator' => '=',
		'value' => '',
		'group' => '0',
		'exposed' => FALSE,
		'expose' => array(
		  'operator' => FALSE,
		  'label' => '',
		),
		'id' => 'views_or_end',
		'table' => 'views_or',
		'field' => 'views_or_end',
		'relationship' => 'none',
	  ),
	  'field_ride_timestamp_value' => array(
		'operator' => '>',
		'value' => array(
		  'value' => array(
			'date' => '',
		  ),
		  'min' => array(
			'date' => '',
		  ),
		  'max' => array(
			'date' => '',
		  ),
		  'default_date' => 'now',
		  'default_to_date' => '',
		),
		'group' => '0',
		'exposed' => FALSE,
		'expose' => array(
		  'operator' => FALSE,
		  'label' => '',
		),
		'date_fields' => array(
		  'node_data_field_ride_timestamp.field_ride_timestamp_value' => 'node_data_field_ride_timestamp.field_ride_timestamp_value',
		),
		'date_method' => 'OR',
		'granularity' => 'day',
		'form_type' => 'date_text',
		'default_date' => 'now',
		'default_to_date' => '',
		'year_range' => '-3:+3',
		'id' => 'field_ride_timestamp_value',
		'table' => 'node_data_field_ride_timestamp',
		'field' => 'field_ride_timestamp_value',
		'relationship' => 'none',
	  ),
	));
	$handler->override_option('access', array(
	  'type' => 'none',
	));
	$handler->override_option('cache', array(
	  'type' => 'none',
	));
	$handler->override_option('use_pager', '1');
	$handler->override_option('style_plugin', 'bulk');
	$handler->override_option('style_options', array(
	  'grouping' => '',
	  'override' => 1,
	  'sticky' => 0,
	  'order' => 'asc',
	  'summary' => '',
	  'columns' => array(
		'nid' => 'nid',
		'title' => 'title',
		'field_ride_token_value' => 'field_ride_token_value',
	  ),
	  'info' => array(
		'nid' => array(
		  'sortable' => 0,
		  'separator' => '',
		),
		'title' => array(
		  'sortable' => 0,
		  'separator' => '',
		),
		'field_ride_token_value' => array(
		  'sortable' => 0,
		  'separator' => '',
		),
	  ),
	  'default' => '-1',
	  'execution_type' => '2',
	  'max_performance' => 1,
	  'display_type' => '0',
	  'hide_selector' => 0,
	  'preserve_selection' => 1,
	  'display_result' => 1,
	  'merge_single_action' => 1,
	  'operations' => array(
		'og_add_group_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_ruleset_action_rules_add_user_to_event' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_nodes-1980f2a3af0b5c3577850d4ba2600405' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_comment_setting_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_assign_owner_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_delete_node_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_mass_update-a27b9efabcd054685a549378b174ad11' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'system_message_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'token_actions_message_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'token_actions_send_email_action-9a018ed1c46c6601b1d3240132bec298' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_ruleset_action_rules_set_1' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_script_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_node_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'imagecache_flush_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'imagecache_generate_all_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'imagecache_generate_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'og_make_private_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'og_make_public_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_make_sticky_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_make_unsticky_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_mass_update-c4d3b28efb86fd703619a50b74d43794' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_nodes-d2f8fcfac885d3fb2f708ab01379cef6' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_content_taxonomy_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_fields_action' => array(
		  'selected' => 1,
		  'skip_confirmation' => 0,
		  'label' => '',
		  'settings' => array(
			'php_code' => 0,
			'show_tokens' => 1,
			'display_fields' => array(
			  'field_ride_token' => 'field_ride_token',
			),
			'_error_element_base' => 'style_options][operations][views_bulk_operations_fields_action][settings][',
		  ),
		),
		'views_bulk_operations_taxonomy_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'views_bulk_operations_argument_selector_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_promote_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_mass_update-14de7d028b4bffdf2b4a266562ca18ac' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_mass_update-9c585624b9b3af0b4687d5f97f35e047' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_publish_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'token_actions_goto_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'system_goto_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'og_remove_groups_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_unpromote_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'og_remove_group_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_mass_update-8ce21b08bb8e773d10018b484fe4815e' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_nodes-d01fd34cabb3dc7f502f837fd4312e8e' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'mollom_action_unpublish_node' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'token_actions_send_email_action-a28083a5b36701a87a1a37fa6fca6bd5' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'token_actions_send_email_action-d1f1bcbbc669a1a6d1954851bbc648c2' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_save_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'mimemail_send_mail_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'system_send_email_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'token_actions_send_email_action-23096a7395def9a51c6719960853a884' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'token_actions_send_email_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_nodes-92c03865ec445bf6f4562ae354fd9354' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_nodes-76e4c513dfadc798e2eb84ad0a677401' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_nodes-c27a48571d3f8d04c8197b80eb126857' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_mass_update-0ccad85c1ebe4c9ceada1aa64293b080' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_unpublish_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'node_unpublish_by_keyword_action' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_nodes-a97f13a5c62f568e87f02eb8932ec742' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'auto_nodetitle_operations_update' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'pathauto_node_update_alias_multiple-620e193b20ba9caa374fea9ca0ad38f0' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
		'flag_nodes-a8087eaa5701dbc775a16a149d8a8e03' => array(
		  'selected' => 0,
		  'skip_confirmation' => 0,
		  'label' => '',
		),
	  ),
	));
	$handler = $view->new_display('page', 'Page', 'page_1');
	$handler->override_option('path', 'update_rides_token');
	$handler->override_option('menu', array(
	  'type' => 'none',
	  'title' => '',
	  'description' => '',
	  'weight' => 0,
	  'name' => 'primary-links',
	));
	$handler->override_option('tab_options', array(
	  'type' => 'none',
	  'title' => '',
	  'description' => '',
	  'weight' => 0,
	  'name' => 'navigation',
	));


  $views[$view->name] = $view;

  return $views;
}