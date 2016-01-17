
/**
 * Containers of a Carousel may want to take control of a carousel navigation.
 * To do so, it must define the aef-jcarousel-arrows-$items_number attribute, where can be found
 * the class of the element that will contains the navigation.
 */
function aef_jcarousel_load_custom_navigation(carousel){
  var element = $(carousel.container);
  var match_attribute = 'aef-jcarousel-arrows-' + carousel.options.scroll;

  while(element.parent().size() > 0 && element.parent().filter('[' + match_attribute + ']').size() <= 0)
  {
    element = element.parent();
  }

  if(element.parent().size() > 0)
  {
    var target_element_class = element.parent().attr(match_attribute);
    $(carousel.container).children('.jcarousel-prev').hide();
    $(carousel.container).children('.jcarousel-next').hide();
    var target_element = $(element.parent()).find('.' + target_element_class);
    target_element.append('<div class="jcarousel-prev aef-jcarousel-arrow aef-jcarousel-arrows-prev"></div>');
    target_element.append('<div class="jcarousel-next aef-jcarousel-arrow aef-jcarousel-arrows-next"></div>');

    carousel.buttonPrev = jQuery('.aef-jcarousel-arrows-prev', target_element);
    carousel.buttonNext = jQuery('.aef-jcarousel-arrows-next', target_element);
  }
}

/**
 * Fallback Drupal behavior to add Jcarousel behavior.
 * By default, jcarousel is added directly during the load.
 * If it couldn't happen (right now, the only case is during the AHAH preview, when the JS is executed
 * before the HTML is inserted :( ), this behavior will do it
 */
Drupal.behaviors.aefjcarousel = function() {
  // Iterate through each selector and add the carousel.
  if(Drupal.settings.aefjcarousel != null)
  {
    jQuery.each(Drupal.settings.aefjcarousel, function(selector, options) {

      // Convert any callback arguments from strings to function calls.
      var callbacks = ['initCallback', 'itemLoadCallback', 'itemFirstInCallback', 'itemFirstOutCallback', 'itemLastOutCallback', 'itemLastInCallback', 'itemVisibleInCallback', 'itemVisibleOutCallback', 'buttonNextCallback', 'buttonPrevCallback'];
      for (callback in callbacks) {
        var callbackname = callbacks[callback];
        // The callback depends on its type.
        if (typeof(options[callbackname]) == 'string') {
          // Strings are evaluated as functions.
          options[callbackname] = eval(options[callbackname]);
        }
        else if (typeof(options[callbackname]) == 'object' && (options[callbackname] instanceof Array)) {
          // Arrays are evaluated as a list of callback registrations. This is because callbacks
          // like itemVisibleInCallback can either be a function call back, or an array of callbacks
          // consisting of both onBeforeAnimation and onAfterAnimation.
          for (subcallback in options[callbackname]) {
            var name = options[callbackname][subcallback];
            options[callbackname][name] = eval(options[callbackname][name]);
          }
        }
      }

      // Create the countdown element on non-processed elements.
      $(selector + ':not(.jcarousel-list)').jcarousel(options);
    });
  }
};
