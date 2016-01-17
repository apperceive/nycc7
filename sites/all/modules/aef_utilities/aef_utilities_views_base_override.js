/**
 * Helper function to parse a querystring.
 * Ovverriding the function in views/js/base.js !
 */

Drupal.Views = Drupal.Views || {};

Drupal.Views.parseQueryString = function (query) {
  var args = {};
  var pos = query.indexOf('?');
  if (pos != -1) {
    query = query.substring(pos + 1);
  }
  var pairs = query.split('&');
  for(var i in pairs) {
    var pair = pairs[i].split('=');
    // Ignore the 'q' path argument, if present.
    // if (pair[0] != 'q' && pair[1]) {   <--   original views code
    if (pair[0] != 'q') {
      args[pair[0]] = decodeURIComponent(pair[1].replace(/\+/g, ' '));
    }
  }
  return args;
};
