<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
 global $user;
 //dpm(get_defined_vars());
?>
<nav id="navbar" role="banner" class="<?php print $navbar_classes; ?> navbar-custom" data-sm-skip="true"  aria-expanded="false" aria-controls="navbar">
  <div class="<?php print $container_class; ?>">

    <div class="navbar-header">
      <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#navbar-nav-links" aria-expanded="true">
        <span class="sr-only">Toggle navigation</span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      
      <div id='navbar-nav-links' class='navbar-nav-links'>
        <?php print $navbar_nav_links ; ?>
      </div>
      
      <div class='navbar-brand'>
        <a class="navbar-link" title="<?php print t('NYCC Home'); ?>" href="<?php print $front_page; ?>"><em>NYCC</em></a>
      </div>
      
      <h1 class='navbar-text visible-md-inline visible-lg-inline'>New York Cycle Club</h1>
    </div> <!-- navbar-header -->

  </div> <!-- container -->

</nav>

<div class="main-container <?php print $container_class; ?>">

  <header role="banner" id="page-header">
    <?php if (!empty($site_slogan)): ?>
      <p class="lead"><?php print $site_slogan; ?></p>
    <?php endif; ?>

    <?php print render($page['header']); ?>
  </header> <!-- /#page-header -->
  
  <?php if (!empty($messages)): ?>
    <div class='container'>
      <div class="row row-messages">
        <div class='col-xs-12'>
          <?php print $messages; ?>
        </div>
      </div>
    </div>
  <?php endif; ?>
  
  <?php if (!empty($page['help'])): ?>
    <div class="row row-help">
       <div class='col-xs-12'>
          <?php print render($page['help']); ?>
      </div>
    </div>
  <?php endif; ?>

  <?php if (!empty($main_menu_expanded)): ?>
    <div class="row row-main-menu">
      <div class = 'col-xs-12'>
        <?php print $main_menu_expanded; ?>
      </div>
    </div>
  <?php endif; ?>

  <?php if (!empty($search_box)): ?>
    <div class="row row-search">
      <div class='col-xs-12'>
        <div id='search'>
          <?php print $search_box; ?>
        </div>
      </div>
    </div>
  <?php endif; ?>

  <?php if (!empty($page['highlighted'])): ?>
    <div class="row">
      <div class='col-xs-12 col-md-8'>
        <div class="highlighted jumbotron"><?php print render($page['highlighted']); ?></div>
      </div>
    </div>
  <?php endif; ?>

  <div class="row row-content-sidebars">
    <?php if (!empty($page['sidebar_first'])): ?>
      <aside class="col-xs-12 col-sm-4" role="complementary">
        <?php print render($page['sidebar_first']); ?>
      </aside>  <!-- /#sidebar-first -->
    <?php endif; ?>

    <section<?php print $content_column_class; ?>>
    
      <?php if (!empty($breadcrumb)): print $breadcrumb; endif;?>
      
      <a id="main-content"></a>
      
      <?php print render($title_prefix); ?>
      <?php if (!empty($title) && !$is_front): ?>
        <h1 class="page-header"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      
      <?php if (!empty($tabs) && !$is_front): ?>
        <?php print render($tabs); ?>
      <?php endif; ?>
            
      <?php if (!empty($action_links)): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      
      <?php print render($page['content_top']); ?>
      <?php print render($page['content']); ?>
      <?php print render($page['content_bottom']); ?>
      
      <?php if (!empty($page['content_aside'])): ?>
        <?php print render($page['content_aside']); ?>
      <?php endif; ?>
    </section>

    <?php if (!empty($page['sidebar_second'])): ?>
      <aside class="col-xs-12 col-sm-5 col-md-4" role="complementary">
        <?php print render($page['sidebar_second']); ?>
      </aside>  <!-- /#sidebar-second -->
    <?php endif; ?>

  </div> <!-- row -->

  <?php if ($is_front): ?>

  <div class='row row-upcoming'>
    <div class='col-xs-12 col-sm-4 '>
      <div class='rides'>
        <h2><a href='/upcoming-rides' title='Upcoming rides'>Rides</a></h2>
        <div>
          <p>Where winds have selectively removed low specific gravity minerals from sand, the grain size of the thin veneer or residue of heavy constituents that remains as blacksand is very much finer than that of light-colored surrounding sand on which the winnowing process is not far advanced.</p>
          <p>The relative frequencies of the heavy minerals present in a number of representative assemblages after removal of magnetite and ilmenite are shown in table 1 and, by and large, these data are averages of counts of at least 300 grains for each of several preparations from specific localities. <a href='/upcoming-rides' title='Upcoming rides'>More ...</a></p>
        </div>
      </div> <!-- rides -->
    </div> <!-- col -->
    <div class='col-xs-12 col-sm-4 '>
      <div class='groups'>
        <a href='/groups' title='NYCC groups'><h2>Groups</a></h2>
        <div>
          <p>The relative frequencies of the heavy minerals present in a number of representative assemblages after removal of magnetite and ilmenite are shown in table 1 and, by and large, these data are averages of counts of at least 300 grains for each of several preparations from specific localities.</p>
          <p>Where winds have selectively removed low specific gravity minerals from sand, the grain size of the thin veneer or residue of heavy constituents that remains as blacksand is very much finer than that of light-colored surrounding sand on which the winnowing process is not far advanced. </p>
          <p>Although the products of the winnowing action of prevailing on-shore winds are quite evident at many points on the coastline they are of small volume; production of larger quantities of black-sand by these means was observed particularly in the sand-dunes both to the north and south of the mouth of Pajaro River. <a href='/groups' title='NYCC groups'>More ...</a></p>
        </div>
      </div> <!-- groups -->
    </div> <!-- col -->
    <div class='col-xs-12 col-sm-4 '>
      <div class='events'>
        <h2><a href='/upcoming-events' title='Upcoming events'>Events</a></h2>
        <div>
          <p>Although the products of the winnowing action of prevailing on-shore winds are quite evident at many points on the coastline they are of small volume; production of larger quantities of black-sand by these means was observed particularly in the sand-dunes both to the north and south of the mouth of Pajaro River. <a href='/upcoming-events' title='Upcoming events'>More ...</a></p>
        </div>
      </div> <!-- events -->
    </div> <!-- col -->
  </div> <!-- row -->

   
  <div class='row row-join'>
    <div class='col-xs-12'>
      <a class='center-block btn btn-danger btn-lg' href='/join-nycc'>Join or Renew your NYCC Membership!</a>
    </div> <!-- col -->
  </div> <!-- row -->
  
  
  <div class='row row-community'>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-lg' href='/volunteer'>Volunteer</a>
    </div>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-lg' href='/eny15'>Escape NY 2015</a>
    </div>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-lg' href='/sponsorship'>Sponsorship</a>
    </div>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-lg' href='/resources'>Resources</a>
    </div>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-lg' href='/message-board/forums/message-board'>Message Board</a>
    </div>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-lg' href='/about'>About</a>
    </div>
  </div> <!-- row -->
  
  
  <div class='row row-navs'>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-primary btn-lg' href='/upcoming_rides'>Social Media Links</a>
    </div>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-primary btn-lg' href='/upcoming_events'>Top Level Menu</a>
    </div>
    <div class='col-xs-12 col-sm-4'>
      <a class='center-block btn btn-primary btn-lg' href='/eny'>Credits & Org Links</a>
    </div>
  </div> <!-- row --> 
  
  <?php endif; ?>
  
  <div class='row row-colophon'>
    <div class='col-xs-12'>
      <p>Copyright © 2015 - New York Cycle Club. No images, text, graphics or design may be reproduced without permission. All rights reserved.</p>
    </div>
  </div> <!-- row -->
  
   
</div> <!-- main-container -->

<?php if (!empty($page['footer'])): ?>
  <footer class="footer <?php print $container_class; ?>">
    <div class='row'>
      <?php print render($page['footer']); ?>
    </div>
  </footer>
<?php endif; ?>
