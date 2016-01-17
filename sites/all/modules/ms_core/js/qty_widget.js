$(function(){
  $("#edit-qty-inc").show();
  $("#edit-qty-dec").show();
  $('#edit-qty-inc').click(function(e){
    e.preventDefault();
    $(':text[name="qty"]').val( Number($(':text[name="qty"]').val()) + 1 );
  });
  $('#edit-qty-dec').click(function(e){
    e.preventDefault();
    if (Number($(':text[name="qty"]').val()) > 1) {
      $(':text[name="qty"]').val( Number($(':text[name="qty"]').val()) - 1 );
    }
  });
});
