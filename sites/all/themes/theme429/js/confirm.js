/*
 * SimpleModal Confirm Modal Dialog
 * http://www.ericmmartin.com/projects/simplemodal/
 * http://code.google.com/p/simplemodal/
 *
 * Copyright (c) 2010 Eric Martin - http://ericmmartin.com
 *
 * Licensed under the MIT license:
 *   http://www.opensource.org/licenses/mit-license.php
 *
 * Revision: $Id: confirm.js 238 2010-03-11 05:56:57Z emartin24 $
 *
 */

// jQuery(function ($) {
//   $('#confirm-dialog input.confirm, #confirm-dialog a.confirm').click(function (e) {
//     e.preventDefault();
// 
//     // example of calling the confirm function
//     // you must use a callback function to perform the "yes" action
//     confirm("I agree to the terms", function () {
//       window.location.href = '';
//     });
//   });
// });

function confirm(message, callback) {
  $('#confirm').modal({
    closeHTML: "<a href='#' title='Close' class='modal-close'>x</a>",
    position: ["20%",],
    overlayId: 'confirm-overlay',
    containerId: 'confirm-container', 
    onShow: function (dialog) {
      $('.message', dialog.data[0]).append('<a href="/sites/default/files/waiver.pdf" target="_new">'+message+'</a>');

      // if the user clicks "yes"
      $('.yes', dialog.data[0]).click(function () {
        // call the callback
        if ($.isFunction(callback)) {
          callback.apply();
        }
        // close the dialog
        $.modal.close();
      });
    }
  });
}

function move() {
  window.location.href=window.location.href;
}

$(document).ready(function(){
   // $("input#withdraw-from-ride-button").hide();
   // $("input#join-a-ride-button").hide();
   $("input#submit-button").hide();
   $("input#not-submit-button").show();
   $("input#confirm-checkbox").click(function(){
     $("input#submit-button").toggle();
     $("input#not-submit-button").toggle();
   });
   
   // $("input#submit-button").click(function(){
   //   $.modal.close();
   //   $("input#join-a-ride-button").click();
   // });
   
  //  $("input#join-a-ride-button").click(function(){
  //    var nid = $(this).attr("name");
  //    //$(".confirm").hide();
  //    $.post('/nycc-join-ride-js', { nid: nid}, function(data) {
  //      $('div#confirm-dialog').html(data);
  //    });
  //    //setTimeout('move()', 2000);
  //  });
  //  
  // $("input#withdraw-from-ride-button").click(function(){
  //   var nid = $(this).attr("name");
  //   //$(".confirm").hide();
  //   $.post('/nycc-withdraw-from-ride-js', { nid: nid}, function(data) {
  //     $('div#confirm-dialog').html(data);
  //   });
  // });
  
  
 });

