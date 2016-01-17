<?php // $Id: page.tpl.php,v 1.25 2008/01/24 09:42:53 Leo Exp $

//dsm(get_defined_vars());

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <title><?php print $head_title ?></title>
  <meta http-equiv="Content-Style-Type" content="text/css" />
  
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
  

  <!--script type="text/javascript" src="http://info.template-help.com/files/ie6_warning/ie6_script.js"></script-->
  
  <script type="text/javascript" src="<?php print base_path().path_to_theme() ?>/js/cufon-yui.js"></script>
  <script type="text/javascript" src="<?php print base_path().path_to_theme() ?>/js/cufon-replace.js"></script>
  
  <script type="text/javascript" src="<?php print base_path().path_to_theme() ?>/js/Arial_700.font.js"></script>
  
  <script type="text/javascript" src="<?php print base_path().path_to_theme() ?>/js/jquery.galleriffic.js"></script>
  <script type="text/javascript" src="<?php print base_path().path_to_theme() ?>/js/jquery.opacityrollover.js"></script>
  
  <script type="text/javascript">
  jQuery(document).ready(function($) {
    // We only want these styles applied when javascript is enabled
    $('#thumbs').css({'display' : 'block'});
    $('div.navigation').css({'float' : 'left'});
    $('div.content-gallery').css('display', 'block');

    // Initially set opacity on thumbs and add
    // additional styling for hover effect on thumbs
    var onMouseOutOpacity = 0.67;
    $('#thumbs ul.thumbs li').opacityrollover({
      mouseOutOpacity:   onMouseOutOpacity,
      mouseOverOpacity:  1.0,
      fadeSpeed:         'fast',
      exemptionSelector: '.selected'
    });
    if ($('#thumbs').length)
    {
    // Initialize Advanced Galleriffic Gallery
    var gallery = $('#thumbs').galleriffic({
      delay:                     5500,
      numThumbs:                 10,
      preloadAhead:              4,
      enableTopPager:            true,
      enableBottomPager:         true,
      maxPagesToShow:            7,
      imageContainerSel:         '#slideshow',
      controlsContainerSel:      '#controls',
      captionContainerSel:       '#caption',
      loadingContainerSel:       '#loading',
      renderSSControls:          true,
      renderNavControls:         true,
      playLinkText:              'Play Slideshow',
      pauseLinkText:             'Pause Slideshow',
      prevLinkText:              'previous',
      nextLinkText:              'next',
      nextPageLinkText:          'Next &rsaquo;',
      prevPageLinkText:          '&lsaquo; Prev',
      enableHistory:             false,
      autoStart:                 true,
      syncTransitions:           true,
      defaultTransitionDuration: 900,
      onSlideChange:             function(prevIndex, nextIndex) {
        // 'this' refers to the gallery, which is an extension of $('#thumbs')
        this.find('ul.thumbs').children()
          .eq(prevIndex).fadeTo('fast', onMouseOutOpacity).end()
          .eq(nextIndex).fadeTo('fast', 1.0);
      },
      onPageTransitionOut:       function(callback) {
        this.fadeTo('fast', 0.0, callback);
      },
      onPageTransitionIn:        function() {
        this.fadeTo('fast', 1.0);
      }
    });
    }
  });
</script>

</head>

<body id="body" class="<?php print $body_classes;?> <?php print phptemplate_body_class();?>">

  <div class="main">
    <div class="main-bg">
      <div class="main-width">
      
        <div class="header">
        
          <div class="login">
            <div class="corner-left">
              <div class="corner-right">
                <?php print theme429_user_bar(); ?>
              </div>
            </div>
          </div>
          
          <?php if ($messages != ""): ?>
            <?php print $messages ?>
          <?php endif; ?>
          
          
          <div class="logo">
            <div class="indent">
              <?php if ($logo) : ?>
                <h1><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" /></a></h1>
              <?php endif; ?>
              <?php if ($site_name) : ?>
                <h1 class="site-name"><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1>
              <?php endif; ?>
              <?php if ($mission != ""): ?>
                <div id="mission"><?php print $mission ?></div>
              <?php endif; ?>
            </div>
          </div>
          
          <div class="right">
            <div class="search">
              <div class="indent">
                <?php if ($search_box): print $search_box;  endif; ?>
              </div>
            </div>
            
            <?php if (isset($secondary_links)) : ?>
              <div class="sub-menu">
                <?php print theme('links', $secondary_links, array('class' => '')) ?>
              </div>
            <?php endif; ?>
          
          </div>
          
            
          <?php if (module_hook('yuimenu','menu') && ("tns" == variable_get('yuimenu_type','tns') || "tnm"==variable_get('yuimenu_type','tns')) ){?>
          
            <div class="main-menu">
              <div class="menu-bg">
                <div class="corner-left"><div class="corner-right"><?php print html_menu(variable_get('yuimenu_root','1') ); ?></div></div>
              </div>
            </div>
            
          <?php }?>
          
          
          
      </div>  <!-- .header -->
        
        
        
        
        <?php if ($is_front != ""): ?>
          <div class="content">
        <?php else: ?>  
          <div class="content mr-top">
        <?php endif; ?>  
        
        
        <div id="nav">
          <?php if ($nav): ?>
            <?php print $nav ?>
          <?php endif; ?>
  
          <?php if (!$nav): ?><!-- if block in $nav, removes default $primary and $secondary links -->
  
          <div id="primary">
            <div id="primary-inner">
                <?php print $primary_menu; ?>
            </div> <!-- /inner -->
          </div> <!-- /primary || superfish -->
  
          <?php endif; ?>
        </div> <!-- /#nav -->
        
         <!-- Block After Menu -->     
        <?php if ($main_top): ?>
          <div id="main-top">
            <?php print $main_top; ?>  
          </div>
        <?php endif; ?>

        
        
        
          <div class="content-pd">
                      <div class="content-bg">
                         <div class="content-top-edge"></div>
            <div class="corner-left-bot"><div class="corner-right-bot">
              <div class="content-top">
                           <?php if (isset($content_top)) : ?>
                  <?php print $content_top;?> 
                                    
                <?php endif; ?>
              
                <?php if (drupal_strlen(trim($right))): ?>
                  <div class="column-right">
                    <div class="corner-top"><div class="corner-bot">  
                          
                      <?php print $right?>
                    
                    </div></div>
                    
                  </div>
                <?php endif; ?>
            
                <div class="column-center">
                  
                  
                    <?php if ($is_front != ""): ?>
                      <div id="custom" class="custom-content">
                        <?php print $customcontent?>
                      </div>
                    <?php endif; ?>  
                    <?php print $back_to_home; ?>
                  
                    <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
                    <?php if ($title): print '<div id="bredcrum-def"><div class="bredrum-nodetitle"><div><h2 id="tabs-wrapper-title" '. ($tabs ? ' class="with-tabs title"' : '') .'>'. $title .'</h2></div></div>';  ?>
                      
                    <?php print '<div id="crumb-menu-def"><ul><li>' . '<a href="/home">Home</a>' .'</li><li class="last">' . $title . '</li></ul></div></div>'; endif;?>
                    
                    <?php if ($tabs): print $tabs .'</div>'; endif; ?>
                      
                    <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
                       
                    <?php if ($show_messages && $messages != ""): ?>
                    <?php /*print $messages*/ ?>
                    <?php endif; ?>
                    
                    <?php if ($help != ""): ?>
                      <div id="help"><?php print $help ?></div>
                    <?php endif; ?>
                    <!-- start main content -->
  
                    <?php print $content; ?>
                  
                    <?php if (isset($content_bottom)) : ?>
                      <div class="content-bottom">
                        <?php print $content_bottom;?> 
                      </div>
                    <?php endif ?>

                </div>
              
              </div>
            </div></div>
                       <div class="content-bottom-edge"></div>
          </div></div>
          
        </div>
        
        <div class="footer"><div class="footer-bg">
          <div class="width">
            <div class="indent">
              
              <div class='nycc-social-links'>
                <a href="https://www.twitter.com/NYCycleClub"><img src="https://twitter-badges.s3.amazonaws.com/follow_us-c.png" alt="Follow NYCycleClub on Twitter"/></a>
                <a href="https://www.facebook.com/groups/62503570990/"><img src="/sites/all/themes/theme429/images/facebook-group.gif" alt="Follow NYCycleClub on Facebook"/></a>
              </div>
              
              <?php if (isset($primary_links)) : ?>
                <div class="footer-menu">
                  <?php print theme('links', $primary_links, array('class' => '')) ?>
                </div>
              <?php endif; ?>
              
              <div class="footer-text">
                <?php if (isset($footer)) : ?>
                  <?php print $footer;?> 
                               
                <?php endif; ?>
              </div>
              
            </div>
          </div>
        </div></div>
        
      </div>
    </div>
    
  </div>
  
  <?php print $closure;?>
  
  <script type="text/javascript">Cufon.now();</script>
  
</body></html>
