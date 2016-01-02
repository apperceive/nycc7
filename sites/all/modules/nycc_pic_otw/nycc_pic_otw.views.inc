<?php

function nycc_pic_otw_views_default_views() {
$view = new view();
$view->name = 'picture_of_the_week';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Picture of the Week';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'views_bootstrap_carousel_plugin_style';
$handler->display->display_options['style_options']['interval'] = '3000';
$handler->display->display_options['style_options']['navigation'] = 1;
$handler->display->display_options['style_options']['indicators'] = 1;
$handler->display->display_options['style_options']['pause'] = 1;
$handler->display->display_options['row_plugin'] = 'views_bootstrap_carousel_plugin_rows';
$handler->display->display_options['row_options']['image'] = 'field_pic_otw_picture';
$handler->display->display_options['row_options']['title'] = 'title';
$handler->display->display_options['row_options']['description'] = 'nothing';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Picture */
$handler->display->display_options['fields']['field_pic_otw_picture']['id'] = 'field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['table'] = 'field_data_field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['field'] = 'field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['label'] = '';
$handler->display->display_options['fields']['field_pic_otw_picture']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_pic_otw_picture']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['field_pic_otw_picture']['type'] = 'colorbox';
$handler->display->display_options['fields']['field_pic_otw_picture']['settings'] = array(
  'colorbox_node_style' => 'large',
  'colorbox_node_style_first' => 'large',
  'colorbox_image_style' => 'large',
  'colorbox_gallery' => 'post',
  'colorbox_gallery_custom' => '',
  'colorbox_caption' => 'node_title',
  'colorbox_caption_custom' => '',
);
/* Field: Content: Body */
$handler->display->display_options['fields']['body']['id'] = 'body';
$handler->display->display_options['fields']['body']['table'] = 'field_data_body';
$handler->display->display_options['fields']['body']['field'] = 'body';
$handler->display->display_options['fields']['body']['label'] = '';
$handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
/* Field: Content: Who's In It */
$handler->display->display_options['fields']['field_pic_otw_members']['id'] = 'field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['table'] = 'field_data_field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['field'] = 'field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['delta_offset'] = '0';
/* Field: Global: Custom text */
$handler->display->display_options['fields']['nothing']['id'] = 'nothing';
$handler->display->display_options['fields']['nothing']['table'] = 'views';
$handler->display->display_options['fields']['nothing']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing']['label'] = '';
$handler->display->display_options['fields']['nothing']['alter']['text'] = '[body]
[field_pic_otw_members]';
$handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
/* Field: Content: Status */
$handler->display->display_options['fields']['field_pic_otw_status']['id'] = 'field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status']['field'] = 'field_pic_otw_status';
/* Sort criterion: Content revision: Updated date */
$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['table'] = 'node_revision';
$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['granularity'] = 'minute';
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'pic_otw' => 'pic_otw',
);
/* Filter criterion: Content: Status (field_pic_otw_status) */
$handler->display->display_options['filters']['field_pic_otw_status_value']['id'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['filters']['field_pic_otw_status_value']['field'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['value'] = array(
  'published' => 'published',
  'promoted' => 'promoted',
);

/* Display: Pic of the week Page */
$handler = $view->new_display('page', 'Pic of the week Page', 'page_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'views_bootstrap_grid_plugin_style';
$handler->display->display_options['style_options']['columns'] = '2';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'views_bootstrap_carousel_plugin_rows';
$handler->display->display_options['row_options']['image'] = 'field_pic_otw_picture';
$handler->display->display_options['row_options']['title'] = 'title';
$handler->display->display_options['row_options']['description'] = 'nothing';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['path'] = 'potw';

/* Display: Block for Front Page */
$handler = $view->new_display('block', 'Block for Front Page', 'block_1');
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'pic_otw' => 'pic_otw',
);
/* Filter criterion: Content: Status (field_pic_otw_status) */
$handler->display->display_options['filters']['field_pic_otw_status_value']['id'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['filters']['field_pic_otw_status_value']['field'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['value'] = array(
  'promoted' => 'promoted',
);

/* Display: Approver Page */
$handler = $view->new_display('page', 'Approver Page', 'page_3');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'editableviews_table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_pic_otw_picture' => 'field_pic_otw_picture',
  'body' => 'body',
  'field_pic_otw_members' => 'field_pic_otw_members',
  'field_pic_otw_status_editable' => 'field_pic_otw_status_editable',
  'field_pic_otw_rejected_reason_editable' => 'field_pic_otw_rejected_reason_editable',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_picture' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'body' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_members' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_status_editable' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_rejected_reason_editable' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['sticky'] = TRUE;
$handler->display->display_options['style_options']['batch_size'] = '10';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Picture */
$handler->display->display_options['fields']['field_pic_otw_picture']['id'] = 'field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['table'] = 'field_data_field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['field'] = 'field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['label'] = '';
$handler->display->display_options['fields']['field_pic_otw_picture']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_pic_otw_picture']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['field_pic_otw_picture']['type'] = 'colorbox';
$handler->display->display_options['fields']['field_pic_otw_picture']['settings'] = array(
  'colorbox_node_style' => '',
  'colorbox_node_style_first' => 'large',
  'colorbox_image_style' => '',
  'colorbox_gallery' => 'post',
  'colorbox_gallery_custom' => '',
  'colorbox_caption' => 'node_title',
  'colorbox_caption_custom' => '',
);
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Content: Body */
$handler->display->display_options['fields']['body']['id'] = 'body';
$handler->display->display_options['fields']['body']['table'] = 'field_data_body';
$handler->display->display_options['fields']['body']['field'] = 'body';
$handler->display->display_options['fields']['body']['label'] = 'Caption';
$handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['body']['type'] = 'text_plain';
/* Field: Content: Who's In It */
$handler->display->display_options['fields']['field_pic_otw_members']['id'] = 'field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['table'] = 'field_data_field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['field'] = 'field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['delta_offset'] = '0';
/* Field: Content: Status (editable) */
$handler->display->display_options['fields']['field_pic_otw_status_editable']['id'] = 'field_pic_otw_status_editable';
$handler->display->display_options['fields']['field_pic_otw_status_editable']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status_editable']['field'] = 'field_pic_otw_status_editable';
$handler->display->display_options['fields']['field_pic_otw_status_editable']['label'] = 'Status';
$handler->display->display_options['fields']['field_pic_otw_status_editable']['widget_type'] = 'options_select';
$handler->display->display_options['fields']['field_pic_otw_status_editable']['suppress_label'] = 1;
$handler->display->display_options['fields']['field_pic_otw_status_editable']['suppress_description'] = 1;
/* Field: Content: Rejected Reason (editable) */
$handler->display->display_options['fields']['field_pic_otw_rejected_reason_editable']['id'] = 'field_pic_otw_rejected_reason_editable';
$handler->display->display_options['fields']['field_pic_otw_rejected_reason_editable']['table'] = 'field_data_field_pic_otw_rejected_reason';
$handler->display->display_options['fields']['field_pic_otw_rejected_reason_editable']['field'] = 'field_pic_otw_rejected_reason_editable';
$handler->display->display_options['fields']['field_pic_otw_rejected_reason_editable']['label'] = 'Reason (if Rejecting)';
$handler->display->display_options['fields']['field_pic_otw_rejected_reason_editable']['type'] = 'text_plain';
$handler->display->display_options['fields']['field_pic_otw_rejected_reason_editable']['widget_type'] = 'text_textarea';
$handler->display->display_options['fields']['field_pic_otw_rejected_reason_editable']['suppress_label'] = 1;
$handler->display->display_options['fields']['field_pic_otw_rejected_reason_editable']['suppress_description'] = 1;
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content revision: Updated date */
$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['table'] = 'node_revision';
$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['granularity'] = 'minute';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'pic_otw' => 'pic_otw',
);
/* Filter criterion: Content: Status (field_pic_otw_status) */
$handler->display->display_options['filters']['field_pic_otw_status_value']['id'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['filters']['field_pic_otw_status_value']['field'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['value'] = array(
  'all' => 'all',
  'submitted' => 'submitted',
  'published' => 'published',
  'promoted' => 'promoted',
  'rejected' => 'rejected',
);
$handler->display->display_options['filters']['field_pic_otw_status_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_pic_otw_status_value']['expose']['operator_id'] = 'field_pic_otw_status_value_op';
$handler->display->display_options['filters']['field_pic_otw_status_value']['expose']['label'] = 'Status';
$handler->display->display_options['filters']['field_pic_otw_status_value']['expose']['operator'] = 'field_pic_otw_status_value_op';
$handler->display->display_options['filters']['field_pic_otw_status_value']['expose']['identifier'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['expose']['remember'] = TRUE;
$handler->display->display_options['filters']['field_pic_otw_status_value']['expose']['multiple'] = TRUE;
$handler->display->display_options['filters']['field_pic_otw_status_value']['expose']['remember_roles'] = array(
  2 => '2',
  36 => 0,
  6 => 0,
  1 => 0,
  38 => 0,
  9 => 0,
  7 => 0,
  13 => 0,
  16 => 0,
  37 => 0,
  19 => 0,
  32 => 0,
  14 => 0,
  11 => 0,
  27 => 0,
  39 => 0,
  10 => 0,
  12 => 0,
  25 => 0,
  3 => 0,
  21 => 0,
  29 => 0,
  31 => 0,
  5 => 0,
  40 => 0,
  18 => 0,
  4 => 0,
  24 => 0,
  35 => 0,
  15 => 0,
  23 => 0,
);
$handler->display->display_options['filters']['field_pic_otw_status_value']['reduce_duplicates'] = TRUE;
$handler->display->display_options['path'] = 'potw_approve';

/* Display: My Pics of the Week */
$handler = $view->new_display('page', 'My Pics of the Week', 'page_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'My Pics of the Week';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'views_bootstrap_table_plugin_style';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_pic_otw_picture' => 'field_pic_otw_picture',
  'body' => 'body',
  'field_pic_otw_members' => 'field_pic_otw_members',
  'nothing' => 'nothing',
  'field_pic_otw_status' => 'field_pic_otw_status',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_picture' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'body' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_members' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nothing' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_status' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['bootstrap_styles'] = array(
  'striped' => 'striped',
  'bordered' => 'bordered',
  'hover' => 'hover',
  'condensed' => 'condensed',
);
$handler->display->display_options['style_options']['responsive'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Picture */
$handler->display->display_options['fields']['field_pic_otw_picture']['id'] = 'field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['table'] = 'field_data_field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['field'] = 'field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['label'] = '';
$handler->display->display_options['fields']['field_pic_otw_picture']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_pic_otw_picture']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['field_pic_otw_picture']['type'] = 'colorbox';
$handler->display->display_options['fields']['field_pic_otw_picture']['settings'] = array(
  'colorbox_node_style' => 'large',
  'colorbox_node_style_first' => 'large',
  'colorbox_image_style' => 'large',
  'colorbox_gallery' => 'post',
  'colorbox_gallery_custom' => '',
  'colorbox_caption' => 'node_title',
  'colorbox_caption_custom' => '',
);
/* Field: Content: Body */
$handler->display->display_options['fields']['body']['id'] = 'body';
$handler->display->display_options['fields']['body']['table'] = 'field_data_body';
$handler->display->display_options['fields']['body']['field'] = 'body';
$handler->display->display_options['fields']['body']['label'] = '';
$handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
/* Field: Content: Who's In It */
$handler->display->display_options['fields']['field_pic_otw_members']['id'] = 'field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['table'] = 'field_data_field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['field'] = 'field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['delta_offset'] = '0';
/* Field: Content: Status */
$handler->display->display_options['fields']['field_pic_otw_status']['id'] = 'field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status']['field'] = 'field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status']['alter']['alter_text'] = TRUE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Author uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'node';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'default';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
$handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'pic_otw' => 'pic_otw',
);
/* Filter criterion: Content: Status (field_pic_otw_status) */
$handler->display->display_options['filters']['field_pic_otw_status_value']['id'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['filters']['field_pic_otw_status_value']['field'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['value'] = array(
  'submitted' => 'submitted',
  'published' => 'published',
  'promoted' => 'promoted',
  'rejected' => 'rejected',
);
$handler->display->display_options['path'] = 'user/%/my-pics_otw';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'My Pics of the Week';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: Tagged In Pics */
$handler = $view->new_display('page', 'Tagged In Pics', 'page_4');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Pics I\'m Tagged In';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'views_bootstrap_table_plugin_style';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_pic_otw_picture' => 'field_pic_otw_picture',
  'body' => 'body',
  'field_pic_otw_members' => 'field_pic_otw_members',
  'nothing' => 'nothing',
  'field_pic_otw_status' => 'field_pic_otw_status',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_picture' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'body' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_members' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nothing' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_pic_otw_status' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['bootstrap_styles'] = array(
  'striped' => 'striped',
  'bordered' => 'bordered',
  'hover' => 'hover',
  'condensed' => 'condensed',
);
$handler->display->display_options['style_options']['responsive'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Picture */
$handler->display->display_options['fields']['field_pic_otw_picture']['id'] = 'field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['table'] = 'field_data_field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['field'] = 'field_pic_otw_picture';
$handler->display->display_options['fields']['field_pic_otw_picture']['label'] = '';
$handler->display->display_options['fields']['field_pic_otw_picture']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_pic_otw_picture']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['field_pic_otw_picture']['type'] = 'colorbox';
$handler->display->display_options['fields']['field_pic_otw_picture']['settings'] = array(
  'colorbox_node_style' => 'large',
  'colorbox_node_style_first' => 'large',
  'colorbox_image_style' => 'large',
  'colorbox_gallery' => 'post',
  'colorbox_gallery_custom' => '',
  'colorbox_caption' => 'node_title',
  'colorbox_caption_custom' => '',
);
/* Field: Content: Body */
$handler->display->display_options['fields']['body']['id'] = 'body';
$handler->display->display_options['fields']['body']['table'] = 'field_data_body';
$handler->display->display_options['fields']['body']['field'] = 'body';
$handler->display->display_options['fields']['body']['label'] = '';
$handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
/* Field: Content: Who's In It */
$handler->display->display_options['fields']['field_pic_otw_members']['id'] = 'field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['table'] = 'field_data_field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['field'] = 'field_pic_otw_members';
$handler->display->display_options['fields']['field_pic_otw_members']['delta_offset'] = '0';
/* Field: Content: Status */
$handler->display->display_options['fields']['field_pic_otw_status']['id'] = 'field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status']['field'] = 'field_pic_otw_status';
$handler->display->display_options['fields']['field_pic_otw_status']['alter']['alter_text'] = TRUE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Who&#039;s In It (field_pic_otw_members) */
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['id'] = 'field_pic_otw_members_uid';
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['table'] = 'field_data_field_pic_otw_members';
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['field'] = 'field_pic_otw_members_uid';
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['default_action'] = 'default';
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['default_argument_type'] = 'user';
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['default_argument_options']['user'] = FALSE;
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['field_pic_otw_members_uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'pic_otw' => 'pic_otw',
);
/* Filter criterion: Content: Status (field_pic_otw_status) */
$handler->display->display_options['filters']['field_pic_otw_status_value']['id'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['table'] = 'field_data_field_pic_otw_status';
$handler->display->display_options['filters']['field_pic_otw_status_value']['field'] = 'field_pic_otw_status_value';
$handler->display->display_options['filters']['field_pic_otw_status_value']['value'] = array(
  'submitted' => 'submitted',
  'published' => 'published',
  'promoted' => 'promoted',
  'rejected' => 'rejected',
);
$handler->display->display_options['path'] = 'user/%/tagged-pics_otw';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Pics I\'m Tagged In';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;



 
  $views[$view->name] = $view;
  return $views;
}