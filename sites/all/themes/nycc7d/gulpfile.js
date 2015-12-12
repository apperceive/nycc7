// gulpfile.js for nycc7d bootstrap subtheme

// Require the needed packages 
var gulp = require('gulp');
var haml = require('gulp-haml');
var less = require('gulp-less');
var prettify = require('gulp-prettify');
var mainBowerFiles = require('main-bower-files');

gulp.task('less', function () {
  gulp.src('./less/**/*.less')
    .pipe(less())
    .pipe(gulp.dest('./css/'));
});
 
 
gulp.task('haml', function () {
  gulp.src('./haml/**/*.haml')
    .pipe(haml({ext: ".php"}))
    .pipe(gulp.dest('./html/'));
});
 
 
gulp.task('prettify', ['haml'], function () {
  gulp.src('./html/**/*.php')
    .pipe(prettify({indent_size: 2}))
    .pipe(gulp.dest('./templates/'));
});
 
 
// see http://andy-carter.com/blog/a-beginners-guide-to-package-manager-bower-and-using-gulp-to-manage-components
gulp.task('bower', function() {
  return gulp.src(mainBowerFiles(), {
      base: 'bower_components'
    })
    .pipe(gulp.dest('public/lib'));
}); 
 
gulp.task('bootstrap:prepareLess', ['bower'], function() {
  return gulp.src('less/bootstrap/*.less')
    .pipe(gulp.dest('public/lib/bootstrap/less'));
});
gulp.task('bootstrap:compileLess', ['bootstrap:prepareLess'], function() {
  return gulp.src('public/lib/bootstrap/less/bootstrap.less')
    .pipe(less())
    .pipe(gulp.dest('public/lib/bootstrap/dist/css'));
});

gulp.task('font-awesome:prepareLess', ['bower'], function() {
  return gulp.src('less/font-awesome/*.less')
    .pipe(gulp.dest('public/lib/font-awesome/less'));
});
gulp.task('font-awesome:compileLess', ['font-awesome:prepareLess'], function() {
  return gulp.src('public/lib/font-awesome/less/font-awesome.less')
    .pipe(less())
    .pipe(gulp.dest('public/lib/font-awesome/dist/css'));
});


gulp.task('lessbow', function(){
  gulp.start(['font-awesome:compileLess', 'bootstrap:compileLess']); 
});



// extras
/*
gulp.task('fonts', function() {
  return gulp.src('node_modules/font-awesome/fonts/*')
    .pipe(gulp.dest('public/fonts'))
})
*/

gulp.task('watch', function() {
  gulp.watch(['less/bootstrap/variables.less'], 
      ['bootstrap:compileLess']);
});
 
 
 
 
 
 

//var opts = ['test': '123', 'rest': 'hello', 'base': '../nycc7/']
//  , deps = [];
//gulp.task('test', deps, function() {
  //console.log('in task');
  //gulp.src('../nycc7/templates/**/*.php', opts)
    //.pipe(console.log('in pipe 1'))
    //.pipe(???);  
//});


/*

gulp.task('test2', deps, function() {
  gulp.watch('./*', function(event) {
    console.log('File ' + event.path + ' was ' + event.type + ', running tasks...');
  });
});
*/


 
 
// Default gulp task to run 
gulp.task('default', function(){
  //gulp.watch('.haml/**/*.haml', ['haml']);
  gulp.start(['haml', 'prettify', 'less']);   // alternative to watch
});

