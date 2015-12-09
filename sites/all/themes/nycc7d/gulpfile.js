// gulpfile.js 
// Require the needed packages 
var gulp = require('gulp');
var haml = require('gulp-haml');
var less = require('gulp-less');
var prettify = require('gulp-prettify');

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
 
 
// Default gulp task to run 
gulp.task('default', function(){
  //gulp.watch('.haml/**/*.haml', ['haml']);
  gulp.start(['haml', 'prettify', 'less']);   // alternative to watch
});
 
