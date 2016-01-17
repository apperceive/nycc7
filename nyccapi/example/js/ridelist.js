localStorage['serviceURL'] = "http://nycc.org/";
var serviceURL = localStorage['serviceURL'];

var rides;

//$(window).load(function() {
$(document).bind('pageinit', function () {
  setTimeout(getRideList, 100);
});

// $(document).bind( "pagebeforechange", function(e, data) {
//   setTimeout(getRideList, 100);
//   e.preventDefault();
// });

// from http://stackoverflow.com/questions/12205041/jquery-mobile-pre-load-page
// 
// $(document).bind( "pagebeforechange", function( e, data ) {
//     if ( (typeof data.toPage === "string") && (data.toPage == "newPageURL")  ) {
//         e.preventDefault();    //to make sure we prevent standard page change
//         getPageFromAPI({success: function (newPage) {
//                 $.mobile.changePage(newPage)
//             }
//         });
//     }
// });


$(document).ajaxError(function(event, request, settings) {
  $('#busy').hide();
  alert("Error accessing the server");
});

function getRideList() {
  //$('#busy').show();
  $.mobile.showPageLoadingMsg();
  $.getJSON(serviceURL + 'current-rides/json?token=Fv8idn3bTh', function(data) {
    var $ridelist = $('#rideList');
    //$('#busy').hide();
    $.mobile.hidePageLoadingMsg();
    $ridelist.find('li').remove();
    rides = data.rides;
    $.each(rides, function(index, ride) {
      ride = ride.ride;
      $ridelist.append('<li>' +
          '<p>' + ride.Name + '</p>' +
          '<p>' + ride.Time + ' ' + ride.Date + ' - ' + ride.Level + ' - ' + ride.Distance + '</p>' +
          '<p>' + ride.Leaders.replace("<br/>", ", ") + '</p>' +
          '</li>');
    });
    $ridelist.listview('refresh');
  });
}