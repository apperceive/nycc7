<?php
// $Id: aef_jcarousel.jq.inc,v 1.1 2009/11/16 11:04:12 ndeschildre Exp $

/**
 * Implementation of hook_jq().
 * http://drupal.org/project/jq
 */
function aef_jcarousel_jq($op, $plugin = NULL) {
  if ($op == 'info') {
    $path = drupal_get_path('module', 'aef_jcarousel');
    return array(
      'jcarousel' => array(
        'name' => t('JCarousel'),
        'description' => t('jCarousel is a jQuery plugin for controlling a list of items in horizontal or vertical order. The items, which can be static HTML content or loaded with (or without) AJAX, can be scrolled back and forth (with or without animation).'),
        'version' => '1.2.3',
        'url' => 'http://sorgalla.com/jcarousel/',
        'files' => array(
          'js' => array(
            $path .'/jcarousel/lib/jquery.jcarousel.js',
          ),
          'css' => array(
            $path .'/jcarousel/lib/jquery.jcarousel.css'
          ),
        ),
        'invocation' => NULL,
      ),
    );
  }
}
