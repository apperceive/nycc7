<?php
// $Id: views-view-unformatted.tpl.php,v 1.6 2008/10/01 20:52:11 merlinofchaos Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<div class="custom-top"><div class="custom-indent">
<div id="gallery" class="content-gallery">
<div class="slideshow-container">
<div id="slideshow" class="slideshow"></div>
<div id="caption" class="caption-container"></div>							
</div>
</div>
					
<div id="thumbs" class="navigation">
<ul class="thumbs noscript">

<?php foreach ($rows as $id => $row): ?>
<li>
 
  <?php print($row)?>
								</li>

<?php endforeach; ?>
							
								
							
							</ul>
						</div>
						 
					</div></div>
					
