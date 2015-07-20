<?php
// $Id: block.tpl.php,v 1.2 2007/08/07 08:39:36 goba Exp $
?>

<div class="<?php print "block block-$block->module" ?>" id="<?php print "block-$block->module-$block->delta"; ?>">
	<div class="bgr"><div class="block-bg">
	<?php  if($block->subject != ""){ ?>
		<div class="title">
			<div><div>
				<h3><?php print $block->subject ?></h3>
			</div></div>
		</div>
	<?php }  ?>
	
<?php
$restrict_block_array = array(1 => 1, 24 => 24, 44 => 44, 52 => 52, 46 => 46, 36 => 36);
$restrict_block =  array_search($block->delta, $restrict_block_array);
 if($restrict_block != $block->delta) { 
  if ($block->module == 'block'):
    if (user_access('administer blocks')) : ?>
    <div class="block-edit-link-custom">
      <ul class="links-custom">
        <li>
          <a href='<?php print check_url(base_path()) ?>admin/build/block/configure/<?php print $block->module;?>/<?php print $block->delta.'?'.drupal_get_destination();?>'>Edit</a>
        </li>
      </ul>
    </div>
    <?php
    endif;
  endif;
  }
?>
		<div class="indent">
			<?php print $block->content ?>
		</div>
	
	</div></div>
</div>
