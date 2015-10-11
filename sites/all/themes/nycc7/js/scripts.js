
(function($) {
  // You pass-in jQuery and then alias it with the $-sign
  // So your internal code doesn't change
  
  // smartmenus
  $(function() {
    $('#main-menu').smartmenus();
  });
  
})(jQuery);


/*
jQuery.noConflict();
 jQuery( document ).ready(function( $ ) {
  $('#main-menu').smartmenus({
      xxxsubMenusSubOffsetX: 1,
      xxxsubMenusSubOffsetY: -8
  });
 });
 
 */