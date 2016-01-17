<?php
// $Id: node.tpl.php,v 1.4 2007/08/07 08:39:36 goba Exp $
?>
  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
    <div class="indent">
      <?php if ($page == 0): ?>
        <div class="title">
          <?php print $picture ?>
          
          <h1><a href="<?php print $node_url ?>"><?php print $title ?></a></h1>
          
          <div class="date"><?php print $submitted ?></div> 
          
        </div>
      <?php endif; ?>
  
      <div class="taxonomy"><?php print $terms ?></div>
    
      <div class="text-box"><?php print $content ?></div>
      
      <?php if ($links): ?>
        <div class="post-links"><?php print $links ?></div>
      <?php endif; ?>
      
      <?php if ($type == "event"): ?>
        <div class="social-buttons">
          <a href="http://twitter.com/share" class="twitter-share-button" data-count="horizontal" data-url="<?php $curr_url = check_plain("http://" .$_SERVER['HTTP_HOST'] .$node_url); echo $curr_url; ?>" data-text="<?php echo $title; ?>" data-via="NYCycleClub">Tweet</a><script type="text/javascript" src="http://platform.twitter.com/widgets.js"></script>
        </div>
     <?php endif; ?>
      
        
    </div>
  </div>
