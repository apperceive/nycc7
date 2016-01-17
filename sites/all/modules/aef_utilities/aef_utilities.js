// This function allows to update or create a content.
// A test AJAJ request is done first.
// If needed, the content is retrieved with a second AJAJ request.
//
// It takes as parameter a single hash table :
//
// * checking_url         The url called to retrieve checking data
// * updating_url         The url called to retrieve the content
// * checking_params      A Hash table used to generate the query string for checking request
// * updating_params      A Hash table used to generate the query string for content request
// * updating_field       The field used to get the content in the content request response
// * template             A string to encapsulate the content (i.e. '<div class="example">%CONTENT%</div>')
//                        %CONTENT% will be replaced by the content retrieved in the request
//
// * checking_callback    A user callback used to check what is to be done
//                        If 1 is returned, the content is retrieved and all continues
//                        If 0 is returned, nothing is done
//                        If -1 is returned, otherwise_callback is called
//
// * before_callback      A user callback called after the content request, just before content encapsulation
// * updating_callback    A user callback called after content encapsulation. It's in this callback that
//                        all the job will be done
//
// * otherwise_callback   A user callback called when the checking function returns -1

Drupal.aefUpdateIfNeeded = function (options) {
  $.getJSON(options.checking_url, options.checking_params, function(checking_json)
  {
    var check = options.checking_callback(checking_json);

    if (check > 0)
    {
      $.getJSON(options.updating_url, options.updating_params, function(updating_json)
      {
        options.before_callback(updating_json);

        var content = updating_json[options.updating_field];

        options.updating_callback(content);
      });
    }
    else if (check < 0)
    {
      options.otherwise_callback(checking_json);
    }
  });
};
