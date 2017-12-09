$(document).ready(function() {
  $('#menu').append('\
  <nav class="navbar navbar-expand-lg navbar-light bg-light">\
    <div class="container">\
      <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">\
    <span class="navbar-toggler-icon"></span>\
      </button>\
      <div class="collapse navbar-collapse" id="navbarNav">\
        <ul class="navbar-nav" id="navbar">\
        </ul>\
      </div>\
    </div>\
  </nav>\
   ');

  $.getJSON("menu.json", function(data) {
    $.each(data, function(key, val) {
      $('#navbar').append('<li class="nav-item"><a class="nav-link" href="' + val.link + '">' + val.title + '</a></li>');
    });
  });
});
