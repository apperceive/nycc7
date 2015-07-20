<?php
// $Id: print.tpl.php,v 1.8.2.17 2010/08/18 00:33:34 jcnventura Exp $

/**
 * @file
 * Default print module template
 *
 * @ingroup print
 */
  //var_dump($print);
  if (arg(2)) {
    $ridenode = node_load(array('nid' => arg(2)));
    if ($ridenode) {
      //$pagetitle = $ridenode->title ."-". substr($ridenode->field_date_ride_first[0]['value'], 0, 10);
    }
  }
  $print['css'] = '/sites/all/themes/theme429/print.css';
  
  '<link rel="stylesheet" type="text/css" media="print" href="/sites/all/themes/theme429/print.css" />';
  
  $print['logo'] = "<img class='pdflogo' src='http://nycc.org/sites/all/themes/theme429/images/nycc-pdf-logo.gif' alt='logo'>";
  //$print['footer_message'] = t("Page !n of !total", array("!n" => "{PAGE_NUM}", "!total" => "{PAGE_COUNT}"));
 
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $print['language']; ?>" xml:lang="<?php print $print['language']; ?>">
  <head>
    <?php print $print['head']; ?>
    <?php print $print['base_href']; ?>
    <title><?php print $print['title']; ?></title>
    <?php print $print['scripts']; ?>
    <?php print $print['sendtoprinter']; ?>
    <?php print $print['robots_meta']; ?>
    <?php print $print['favicon']; ?>
  
    <style type="text/css">
  body {
    font-family: sans-serif, arial;
    font-size: 8pt;
    padding: 0 20px;
  }

  #pdf-costum-text h2 {
    text-align: center;
    width: 100%;
  }

  .views-admin-links,
  .statistics_counter {
    display: none;
  }

  #pdf-costum-text p {
    text-indent: -10px;
    margin-left: 10px;
    font-size: 8pt;
  }

  .view .view-content .views-table {
    width: 100%; 
  }

  .view .view-content .views-table,
  .view .view-content .views-table tr,
  .view .view-content .views-table td,
  .view .view-content .views-table th {
    border: solid 1px black;
    margin: 0;
    padding: 0;
    text-align: center;
    border-collapse: collapse;
  }

  .view .view-content .views-table td,
  .view .view-content .views-table th {
    padding: 3px;
    height: 25px;
  }

  .nycc_rae {
    padding-left: 45px; 
    font-size: 10pt;
    display: inline-block;
    margin-left: 480px;
    /*page-break-before: always;*/
  }

  .nycc_pdf_page_break {
    page-break-before: always;
  }
  
  .nycc-read-release-row {
    height: 500px;
  }

  .nycc-read-release {
    color: #ccc;
  }

  .view-rides-detail-on-riders-list .views-field-title,
  .view-rides-detail-on-riders-list .views-field-field-date-ride-first-value,
  .view-rides-detail-on-riders-list .views-field-field-ride-leaders-nid {
    display: inline-block;
    max-width: 120px;
    padding-right: 10px;
  }

  .view-rides-detail-on-riders-list .views-field-field-ride-leaders-nid {
    display: inline-block;
    /*max-width: 240px;*/
    padding-right: 0px;
    border: none;
    border-collapse: collapse;
  }

  .view-rides-detail-on-riders-list .views-field-field-ride-leaders-nid .field-content {
    display: inline;
  }

  .view-rides-detail-on-riders-list .views-field-field-ride-leaders-nid .field-item {
    display: inline;
    /*width: auto;*/
    margin: 0;
    padding: 0;
    padding-right: 10px;
  }

  .view-rides-detail-on-riders-list .field-content hr {
    border: 1px black thin;
  }

  .view-rides-detail-on-riders-list .field-content b {
    display: block;
    margin-top: 20px;
  }

  .print-logo {
    width: 100%;
    text-align: right;
    padding-right: 60px;
  }

  .print-logo img {
    margin: 0;
    padding: 0;
  }
    </style>
  </head>
  <body>
    <?php if (!empty($print['message'])) {
      print '<div class="print-message">'. $print['message'] .'</div><p />';
    } ?>
    <div class="print-logo"><?php print $print['logo']; ?></div>
    <div class="print-content"><?php print $print['content']; ?></div>
    <div class="print-footer"><?php print $print['footer_message']; ?></div>
  </body>
</html>
