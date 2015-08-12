<?php

// output bootstrap carousel from site images

// need array(s) of image/src, description/caption, alt (optional), link, index/counter?
// use theme_items?
// simplfy active calculation?
// use fontawesum

//dpm(get_defined_vars());

// Variables:
// $slides - array of slide arrays with keys: title, date, body, imgpath, thumbpath, nodepath,

?>
<div id="nycc-carousel" class="carousel slide" data-ride="carousel">
  <!-- Indicators -->
  <ol class="carousel-indicators row">
  <?php foreach($slides as $ndx => $slide) { ?>
    <li data-target="#nycc-carousel" data-slide-to="<?php print $ndx; ?>" <?php ($ndx == 0) ? print 'class="active"' : print ''; ?> ></li>
  <?php }  ?>
  </ol>

  <!-- Wrapper for slides -->
  <div class="carousel-inner" role="listbox">
  <?php foreach($slides as $ndx => $slide) { ?>
    <div class="item <?php ($ndx == 0) ? print ' active' : print '';?> ">
      <img src="<?php print $slide['imgpath']; ?>">
      <div class="carousel-caption"><h2><?php print $slide['title']; ?></h2><?php print $slide['body']; ?></div>
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

