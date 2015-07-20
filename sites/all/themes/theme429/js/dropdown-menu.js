 $(document).ready(function(){
   $("#primary-innerul li").hover(function(event){
      $(this).find("ul").addClass("show"),
	  $(this).find("ul").removeClass("hide")}

	  ,function(event){
      $(this).find("ul").addClass("hide");
	  $(this).find("ul").removeClass("show");
 });
   
  $("#primary-inner ul li").hover(function(event){
      $(this).children("div.yuimenu").show()}
	  ,function(event){
	 $(this).children("div.yuimenu").hide();
 });
   
  $("#primary-inner ul li").hover(function(event){
      $(this).children("div.yuimenu").show(),
	  $(this).find("ul").slideDown(100)}
	  ,function(event){
      $(this).find("ul").slideUp(100);
	 $(this).children("div.yuimenu").hide();
 });
  

  $("#primary-inner ul li ul li").click(function(event){
    $(this).parent().hide();
 });
});



/* $(document).ready(function(){
   $(".main-menu ul li").hover(function(event){
      $(this).children("div").slideDown(1000)}
	  ,function(event){
     $(this).children("div").hide(10);
 });

});

*/

