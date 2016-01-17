<?php
// $Id: aef_jcarousel_views.views.inc,v 1.1 2009/11/16 11:06:33 ndeschildre Exp $
/**
 * @file
 *  Provides the views plugin information.
 */

/**
  * Implementation of hook_views_plugin().
  */
function aef_jcarousel_views_views_plugins() {
  return array(
    'module' => 'aef_jcarousel_views',
    'style' => array(
      'aef_jcarousel_views' => array(
        'title' => t('AEF Carousel View'),
        'theme' => 'aef_jcarousel_view',
        'help' => t('Display rows in a Carousel via jCarousel.'),
        'handler' => 'aef_jcarousel_views_style_plugin',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
