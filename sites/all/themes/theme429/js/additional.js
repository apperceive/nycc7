// function nyccJoinRide(nid) {
//   nyccWorkingIcon('#nycc-ride-buttons');
//   $('.block-nycc-participants ul').html("")
//   $.post('/nycc-join-ride-js', { nid: nid }, function(data) {
//     if (data == "-3") {
//       location.href = "/nycc-update-contact-info/" + nid;
//     } else if (data == "-2") {
//       nyccUpdateParticipantsBlock(nid);
//       alert("You have already joined this ride.");
//     } else if (data == "-1") {
//       nyccUpdateSpots(0);
//       nyccUpdateParticipantsBlock(nid);
//       alert("Sorry! There are no more spots available.");
//     } else {
//       $('#nycc-ride-buttons').replaceWith(data);
//       nyccUpdateParticipantsBlock(nid);
//       nyccUpdateSpots(-1);
//     }
//   });
// }
//
// function nyccWithdrawFromRide(nid) {
//   nyccWorkingIcon('#nycc-ride-buttons');
//   $('.block-nycc-participants ul').html("")
//   $.post('/nycc-withdraw-from-ride-js', { nid: nid }, function(data) {
//     $('div#nycc-ride-buttons').replaceWith(data);
//     nyccUpdateParticipantsBlock(nid);
//     nyccUpdateSpots(1);
//   });
// }
//
// function nyccUpdateSpots(delta) {
//   var $spots = $("div.field-field-ride-spots div.field-items div.field-item");
//   var spots = $spots.html().trim();
//   var re = /([0-9]+) of ([0-9]+)/;
//   var result = re.exec(spots);
//   if (result[1] && result[2]) {
//     var x = new Number(result[1]);
//     if (!delta)
//       x = 0;
//     else
//       x += delta;
//     if (x < 0)
//       x = 0;
//     var y = result[2];
//     $spots.html(x.toString() + " of " + y);
//   }
// }

// function nyccUpdateParticipantsBlock(nid) {
//   nyccWorkingIcon('.block-nycc-participants ul');
//   $.post('/nycc-ride-participants-js', { nid: nid }, function(data) {
//     $('.block-nycc-participants ul').html(data);
//   });
// }

function nyccWorkingIcon(id) {
  $(id).html("<img src='/sites/all/themes/theme429/images/loading.gif'><img src='/sites/all/themes/theme429/images/loading.gif'>");
}

$(document).ready(function() {


  // disable quantity field on shopping cart pages
  // TODO: limit to memberships
  // no longer needed. now using uc_restrict_qty module
  //$('body.cart #cart-form-products td.qty div input').attr("readonly","true").css("border","none");

  //$('div.og-audience').parent('fieldset.collapsible').css('display','none');
  // og uses both checkboxes and selects under different situations
  $('div.og-audience, select.og-audience').parent().parent().parent().css('display','none');

  //$('body.no-sidebars #cke_edit-body').css('width', '920');

  //$('body.node-type-event .node .indent').prepend($('#signup-form #edit-submit').clone());

  // copy first additional date to field_date_ride_first
  $("select.gpstatus").change(function(){
    var $this = $(this),
        tid = $this.val(),      // selected tid
        id = $this.attr('id');
    //nyccWorkingIcon(id + 'busy');
    $(id + '-busy').show();
    var jqxhr = $.post('/nycc-update-group-user-status-js/', { id: id, tid: tid }, function(data) {
      //alert(data);
      $(id + '-busy').hide();
    });
  });



  // toggle all attendance checkboxes
  // todo: generalize to any set of cb's
  $('div.nycc-attendance-toggle a').click(function() {
    $('input.attendance').each(function() { this.checked = !this.checked; });
  });

  $('a.nycc-view-full-width').click(function() {
    $('div.column-right').toggle();
    return false;
  });


  $(".block-edit-link-custom").hide();

  $('div.block').mouseover(function() {
    $(this).find('.block-edit-link-custom').css('display', 'block');
  });

  $('div.block').mouseout(function() {
    $(this).find('.block-edit-link-custom').css('display', 'none');
  });

  // mss: hack to default wysiwyg link targets
  // TODO: is there a simple way to configure this in wysiwyg module instead?
  $(".forum-post .content p a[target='']").attr("target", "_blank");
  $(".node-rides .nycc-ride-content .description a[target='']").attr("target", "_blank");

  // CONTINUE BUTTON ON SUBMIT RIDE PAGE
  $("img#submit-ride-continue").click(function(){
    $("input#edit-submit").click();
  });

  /*
  ******  REGISTER EVENT BUTOON
  */
  $("img#register-event-button").click(function(){
    $("input.node-add-to-cart").click();
  });


  // todo: is this obsolete?
  /*
  ****** REMOVE A HERF FROM CLAENDER DISPLAY ON EVENTS PAGE
  */
  $(".mini-day-off a").removeAttr("href");
  $(".mini-day-on a").removeAttr("href");
  $(".date-heading a").removeAttr("href");


  if ($('body').hasClass('node-type-event')) {
    $("#nav ul.primary-links li.menu-642").addClass("active-trail active");
    $("#nav ul.primary-links li.menu-642 a").addClass("active");
  }


  // handle from switch between text input and select
  $('#edit-field-ride-from-0-value-wrapper').append('&nbsp;<small><a id="nycc-list-from-locations" href="#" title="Switch back to selection list...">list</a></small>');
  $('#edit-field-ride-from-0-value').attr('placeholder', 'Enter location where ride starts');

  if ($('#edit-field-ride-from-0-value').val())
    $('#edit-field-ride-from-select-value-wrapper').hide();
  else
    $('#edit-field-ride-from-0-value-wrapper').hide();

  $('#edit-field-ride-from-select-value').change(function() {
    var $this = $(this);
    if ($this.val() == "- Other...") {
      $('#edit-field-ride-from-select-value').val("");
      $('#edit-field-ride-from-select-value-wrapper').hide();
      $('#edit-field-ride-from-0-value-wrapper').show();
      $('#edit-field-ride-from-0-value').focus();
    }
  });

  $('#nycc-list-from-locations').click(function() {
    $('#edit-field-ride-from-select-value-wrapper').show();
    $('#edit-field-ride-from-0-value').val("");
    $('#edit-field-ride-from-0-value-wrapper').hide();
    return false;
  });

  // hide the submit button
  $('#nycc-submit-ride').hide();

  // create datepickers
  datePickerController.createDatePicker({
    formElements : {
      "edit-nycc-rides-datepicker" : "m-sl-d-sl-Y"
    },
    staticPos : true,
    statusFormat : "l-cc-sp-F-sp-d-cc-sp-Y",
    fillGrid : true,
    constrainSelection : false,
    higlightDays : [0,0,0,0,0,0,0],
    callbackFunctions : {
      "dateset" : [function(argObj) {
        // don't use argObj.date as it is parsed for wrong time zone
        var d = argObj.mm + "/" + argObj.dd + "/" + argObj.yyyy,
          $ad = $('#edit-field-ride-additional-dates-0-value');
        if ($('#edit-choose-calendar-target-yes').attr('checked') == true) {
          var v = $ad.val();
          // todo: test for duplicate
          if (v.indexOf(d) == -1) {
            v = v + (v.length ? ", " : "") + d;
            $ad.val(v);
          }
        }
        else
          $ad.val(d);
        $ad.change();
        nyccWorkingIcon('.existing-rides');
        $.post('/nycc-existing-rides-js', { date: d }, function(data) {
          $('.existing-rides').replaceWith(data);
        });
        return false;
      }]
    }
    //positioned : "nycc-rides-datepicker",
    //cursorDate : $("edit-dp-normal-1").val(),
    //hideInput : true,
  });

  // todo: this does not work. how do it?
  //fdLocale.firstDayOfWeek = 6;   // set first day to sunday, 0 = monday

  // copy first additional date to field_date_ride_first
  $("#edit-field-ride-additional-dates-0-value").change(function(){
    var d = $(this).val(),
        p = d.indexOf(",");
    if (p != -1)
      d = d.substr(0, p);
    $("#edit-field-date-ride-first-0-value-date").val(d);

  });

  var $form = $("body.current-rides #views-exposed-form-current-rides-list-page-1");
  $form.find("div").hide();
  var $cur = $("#edit-field-ride-select-level-value-many-to-one").val();
  var $newform = $("<div id='nycc-filter-ride-level'></div>").appendTo($form);
  $newform.append("<strong>Level:</strong>");
  $newform.append(nycc_make_level_filter_input("All", "All", $cur == "All"));
  $newform.append(nycc_make_level_filter_input("A-rides", "A", $cur == "A"));
  $newform.append(nycc_make_level_filter_input("B-rides", "B", $cur == "B"));
  $newform.append(nycc_make_level_filter_input("C-rides", "C", $cur == "C"));

  $cur = $("#edit-field-ride-type-value-many-to-one").val();
  $newform = $("<div id='nycc-filter-ride-type'></div>").appendTo($form);
  $newform.append("<strong>Type:</strong>");
  $newform.append(nycc_make_type_filter_input("All", "All", $cur == "All"));
  $newform.append(nycc_make_type_filter_input("Cue Sheet", "Cue Sheet Ride", $cur == "Cue Sheet Ride"));
  $newform.append(nycc_make_type_filter_input("Training", "Training Ride", $cur == "Training Ride"));

  $cur = $("#edit-field-ride-dow-value-many-to-one").val();
  $newform = $("<div id='nycc-filter-ride-dow'></div>").appendTo($form);
  $newform.append("<strong>&nbsp;&nbsp;Day:&nbsp;</strong>");
  $newform.append(nycc_make_dow_filter_input("All", "All", $cur == "All"));
  $newform.append(nycc_make_dow_filter_input("Mon", "Monday", $cur == "Monday"));
  $newform.append(nycc_make_dow_filter_input("Tue", "Tuesday", $cur == "Tuesday"));
  $newform.append(nycc_make_dow_filter_input("Wed", "Wednesday", $cur == "Wednesday"));
  $newform.append(nycc_make_dow_filter_input("Thu", "Thursday", $cur == "Thursday"));
  $newform.append(nycc_make_dow_filter_input("Fri", "Friday", $cur == "Friday"));
  $newform.append(nycc_make_dow_filter_input("Sat", "Saturday", $cur == "Saturday"));
  $newform.append(nycc_make_dow_filter_input("Sun", "Sunday", $cur == "Sunday"));
  $newform.append(nycc_make_dow_filter_input("Sat & Sun", "Saturday,Sunday", $cur == "Saturday,Sunday"));


  // todo: test for existance
  // CKEDITOR.config.bodyClass = 'content-center';
  // CKEDITOR.config.bodyId = 'body';

  if (Drupal && Drupal.wysiwyg && Drupal.wysiwyg.activeId) {
    var editor = CKEDITOR.instances[Drupal.wysiwyg.activeId];

    // The set of styles for the Styles combo (THIS REPLACES EXISTING!)
    CKEDITOR.stylesSet.add( 'default',
    [
        /* Block Styles */

          // These styles are already available in the "Format" combo, so they are
          // not needed here by default. You may enable them to avoid placing the
          // "Format" combo in the toolbar, maintaining the same features.
          /*
          { name : 'Paragraph'    , element : 'p' },
          { name : 'Heading 1'    , element : 'h1' },
          { name : 'Heading 2'    , element : 'h2' },
          { name : 'Heading 3'    , element : 'h3' },
          { name : 'Heading 4'    , element : 'h4' },
          { name : 'Heading 5'    , element : 'h5' },
          { name : 'Heading 6'    , element : 'h6' },
          { name : 'Preformatted Text', element : 'pre' },
          { name : 'Address'      , element : 'address' },
          */

          { name : 'Blue Title'    , element : 'h3', styles : { 'color' : 'Blue' } },
          { name : 'Red Title'    , element : 'h3', styles : { 'color' : 'Red' } },
          { name : 'Yellow Title'    , element : 'h3', styles : { 'color' : 'Yellow' } },

          /* Inline Styles */

          // These are core styles available as toolbar buttons. You may opt enabling
          // some of them in the Styles combo, removing them from the toolbar.
          /*
          { name : 'Strong'      , element : 'strong', overrides : 'b' },
          { name : 'Emphasis'      , element : 'em'  , overrides : 'i' },
          { name : 'Underline'    , element : 'u' },
          { name : 'Strikethrough'  , element : 'strike' },
          { name : 'Subscript'    , element : 'sub' },
          { name : 'Superscript'    , element : 'sup' },
          */

          { name : 'Marker: Green'  , element : 'span', styles : { 'background-color' : 'Green' } },
          { name : 'Marker: Red'  , element : 'span', styles : { 'background-color' : 'Red' } },
          { name : 'Marker: Yellow'  , element : 'span', styles : { 'background-color' : 'Yellow' } },

          { name : 'Big'        , element : 'big' },
          { name : 'Small'      , element : 'small' },
          { name : 'Typewriter'    , element : 'tt' },

          { name : 'Computer Code'  , element : 'code' },
          { name : 'Keyboard Phrase'  , element : 'kbd' },
          { name : 'Sample Text'    , element : 'samp' },
          { name : 'Variable'      , element : 'var' },

          { name : 'Deleted Text'    , element : 'del' },
          { name : 'Inserted Text'  , element : 'ins' },

          { name : 'Cited Work'    , element : 'cite' },
          { name : 'Inline Quotation'  , element : 'q' },

          { name : 'Language: RTL'  , element : 'span', attributes : { 'dir' : 'rtl' } },
          { name : 'Language: LTR'  , element : 'span', attributes : { 'dir' : 'ltr' } },

        /* Object Styles */

          {
            name : 'Image on Left',
            element : 'img',
            attributes :
            {
              'style' : 'padding: 5px; margin-right: 5px',
              'border' : '2',
              'align' : 'left'
            }
          },

          {
            name : 'Image on Right',
            element : 'img',
            attributes :
            {
              'style' : 'padding: 5px; margin-left: 5px',
              'border' : '2',
              'align' : 'right'
            }
          },

          { name : 'Borderless Table', element : 'table', styles: { 'border-style': 'hidden', 'background-color' : '#E6E6FA' } },
          // { name : 'Disc Bulleted List', element : 'ul', styles : { 'list-style-type' : 'disc' } },
          // { name : 'Circle Bulleted List', element : 'ul', styles : { 'list-style-type' : 'circle' } },
          // { name : 'Square Bulleted List', element : 'ul', styles : { 'list-style-type' : 'square' } },
          { name : 'Normal Bulleted List', element : 'ul', styles : { 'list-style-image' : 'none' } },
          { name : 'Asterisk Bulleted List', element : 'ul', styles : { 'list-style-image' : 'url(/sites/all/themes/theme429/images/ul_asterisk.gif)' } },
          { name : 'Chevron Bulleted List', element : 'ul', styles : { 'list-style-image' : 'url(/sites/all/themes/theme429/images/ul_chevron.gif)' } }

     ]);

    editor.on("afterCommandExec", function(e){
      if(e.data.name == 'maximize'){
          // maximized
          if(e.data.command.state == CKEDITOR.TRISTATE_ON){
            // add special css class to body(e.editor.document.getBody())
            $('#nav').hide();
            $('div.header').hide();
          } else {
          // minimized
          // remove special css from body
            $('#nav').show();
            $('div.header').show();
          }
      }
    });
  }
});

function nycc_make_level_filter_input(text, v, checked) {
  return "<label class='option'><input name='nycc_level' type='radio' onclick='nycc_filter_ride_level(\""+ v +"\")' "+ (checked ? "checked" : "") +"/>"+ text + "</label>";
}

function nycc_filter_ride_level(ridelevel) {
  $("#edit-field-ride-select-level-value-many-to-one").val(ridelevel);
  $("#edit-submit-current-rides-list").click();
}

function nycc_make_type_filter_input(text, v, checked) {
  return "<label class='option'><input name='nycc_type' type='radio' onclick='nycc_filter_ride_type(\""+ v +"\")' "+ (checked ? "checked" : "") +"/>"+ text + "</label>";
}

function nycc_filter_ride_type(ridetype) {
  $("#edit-field-ride-type-value-many-to-one").val(ridetype);
  $("#edit-submit-current-rides-list").click();
}

function nycc_make_dow_filter_input(text, v, checked) {
  return "<label class='option'><input name='nycc_dow' type='radio' onclick='nycc_filter_ride_dow(\""+ v +"\")' "+ (checked ? "checked" : "") +"/>"+ text + "</label>";
}

function nycc_filter_ride_dow(ridedow) {
  //$("#edit-field-ride-dow-value-many-to-one").val(ridedow);
  $("#edit-field-ride-dow-value-many-to-one").val(ridedow.split(","));
  $("#edit-submit-current-rides-list").click();
}

function nycc_show_ride_submit_confirm() {
  $("#nycc-submit-ride").show();
  $("#edit-submit").show();
}

function nycc_show_ride_submit_click() {
  $("#edit-submit").click();
}


