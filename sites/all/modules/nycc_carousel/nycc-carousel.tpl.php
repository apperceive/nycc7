<?php

// output bootstrap carousel from site images

// need array(s) of image/src, description/caption, alt (optional), link(optional?), date?, index/counter?

// add links to pages  http://nycc.org/sites/default/files/imagecache/home_page_top_banner/060515-iceCreamSocial.png
// how is date used

// need two loops, once for ol.carousel-indicators li and one for .item divs
$slides = array(
  array('title' => 'All-Class Ride 2015', 'src' => 'http://nycc.org/sites/default/files/imagecache/home_page_top_banner/071815-allClassRide.jpg', 'caption' => 'The NYCC 2015 All-Class Ride - Join us for the ride of the year!'),
  array('title' => 'Escape New York 2015', 'src' => 'http://nycc.org/sites/default/files/imagecache/home_page_top_banner/ENY-Logo2015-2.png', 'caption' => 'This fall classic offers four scenic, well marked routes  -   25, 50, 65 or 100 miles – all beginning and ending at Sakura Park in Manhattan.   All offer picture-postcard views of the Hudson River and Palisades as you cross the George Washington Bridge and ride the serene roads of Bergen and Rockland Counties.'),
  array('title' => 'Cycling Skills Active Workshop', 'src' => 'http://nycc.org/sites/default/files/imagecache/home_page_top_banner/nycc-manuel-bikes-banner.jpg', 'caption' => "Calling ALL NYCC cyclists! Come one, come all... A riders, B riders, C riders! Join us for a totally \"classless\" skills session. Improve your handing skills... practice navigating the GWB hairpin turn... have a go at \"bunny hopping\"... take part in flat changing clinic... join a \"race\" where the slowest rider wins... and much, much more! It will be a fun and informative morning. Bring everything you'd normally bring on a ride, tubes, inflation device, fluids, etc. "),
  array('title' => 'NYCC 2015 Ice Cream Social', 'src' => 'http://nycc.org/sites/default/files/imagecache/home_page_top_banner/060515-iceCreamSocial.png', 'caption' => 'Please join us for ice cream as we take in the beautiful NYC skyline with front row seats from Pier 1 at Brooklyn Bridge Park.  You bring the talk, The Brooklyn Ice Cream Factory will bring the ice cream and the city will bring the views!!  (and yes, I’ve already asked Mother Nature to bring the weather – the pleasant kind.  She’s thinking about it). '),
);

//dpm(get_defined_vars());

// use theme_items?
// simplfy active calculation?


?>
<div id="nycc-carousel" class="carousel slide" data-ride="carousel">
  <!-- Indicators -->
  <ol class="carousel-indicators">
  <?php foreach($slides as $ndx => $slide) { ?>
    <li data-target="#nycc-carousel" data-slide-to="<?php print $ndx; ?>" <?php ($ndx == 0) ? print 'class="active"' : print ''; ?> ></li>
  <?php }  ?>
  </ol>

  <!-- Wrapper for slides -->
  <div class="carousel-inner" role="listbox">
  <?php foreach($slides as $ndx => $slide) { ?>
    <div class="item <?php ($ndx == 0) ? print ' active' : print '';?> ">
      <img src="<?php print $slide['src']; ?>">
      <div class="carousel-caption"><h2><?php print $slide['title']; ?></h2><?php print $slide['caption']; ?></div>
    </div>
  <?php }  ?>
  </div>

  <!-- Controls -->
  <a class="left carousel-control" href="#nycc-carousel" role="button" data-slide="prev">
    <span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span>
    <span class="sr-only">Previous</span>
  </a>
  <a class="right carousel-control" href="#nycc-carousel" role="button" data-slide="next">
    <span class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
    <span class="sr-only">Next</span>
  </a>
</div>

