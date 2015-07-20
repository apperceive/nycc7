<?php
// $Id: node.tpl.php,v 1.4 2007/08/07 08:39:36 goba Exp $
//dsm(get_defined_vars());
?>
  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?> <?php print $upcoming ?>">
    <div class="indent">
      <?php if ($page == 0): ?>
        <div class="title">
          <?php print $picture ?>
          
          <h1><a href="<?php print $node_url ?>"><?php print $title ?></a></h1>
          
          <div class="date"><?php print $submitted ?></div>  
          
        </div>
        <div class="taxonomy"><?php print $terms ?></div>

        <div class="text-box"><?php print $content ?></div>

        <?php if ($links): ?>
          <div class="post-links"><?php print $links ?></div>
        <?php endif; ?>

      <?php else: ?>
  
        <div>
          <ul class='nycc-ride-stats'>
            <li><?php print $field_ride_status_rendered ?></li>
            <li><?php print $field_ride_select_level_rendered ?></li>
            <li><?php print $field_ride_speed_rendered ?></li>
            <li><?php print $field_ride_spots_rendered ?></li>
            <li><?php print $field_ride_distance_in_miles_rendered ?></li>
            <li><?php print $field_ride_type_rendered ?></li>          
          </ul>
  
          <div class='nycc-ride-content'>
            <h2 class='datestr'><?php print $title ?></h2>
      
            <h3 class='title'><?php print $ride_day_date_time  ?></h3>
      
            <div class="description"><?php print $nycc_ride_description ?></div>
                    
            <div class="from"><label>Starting From: </label><span><?php print $ride_from ?></span></div>
                    
            <?php print $field_ride_leaders_rendered ?>            
          
            <?php print $nycc_ride_images ?>
            
            <?php print $nycc_ride_attachments ?>
          
            <?php print $nycc_ride_cue_sheet ?>
            
            <?php print $revision_note ?>
      
            <?php print $revision_history ?>

            <?php print $nycc_ride_buttons ?>

          </div>
        </div>
      <?php endif; ?>

    </div>
  </div>

        <div class="social-buttons">
          <a href="http://twitter.com/share" class="twitter-share-button" data-count="horizontal" data-url="<?php $curr_url = check_plain("http://" .$_SERVER['HTTP_HOST'] .$node_url); echo $curr_url; ?>" data-text="<?php echo $title; ?>" data-via="NYCycleClub">Tweet</a><script type="text/javascript" src="http://platform.twitter.com/widgets.js"></script>
        </div>
        

<br/><br/>
<iframe style="border: currentColor; width: 160px; height: 80px; overflow: hidden;" src="https://www.facebook.com/plugins/like.php?href=<?php $curr_url = check_plain("http://" .$_SERVER['HTTP_HOST'] .$node_url); echo $curr_url; ?>&amp;layout=standard&amp;show_faces=true&amp;width=450&amp;font=arial&amp;height=80&amp;action=like&amp;colorscheme=light&amp;locale=en_US&amp;send=false" frameBorder="0" allowTransparency="true" scrolling="no"></iframe>