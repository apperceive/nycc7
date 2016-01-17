<?php
// $Id: page-node-formatter_notheme.tpl.php,v 1.1 2009/11/25 14:07:25 ndeschildre Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
  </head>
  <body class="<?php print $body_classes; ?>">

            <?php print $content ?>

    <?php print $closure ?>
  </body>
</html>
