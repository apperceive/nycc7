// gulpfile.js 
// Require the needed packages 
var gulp = require('gulp');
var haml = require('gulp-haml');
var less = require('gulp-less');

gulp.task('less', function () {
  gulp.src('./less/**/*.less')
    .pipe(less())
    .pipe(gulp.dest('./css/'));
});
 
 
gulp.task('haml', function () {
  gulp.src('./haml/**/*.haml')
    .pipe(haml({ext: ".php"}))
    .pipe(gulp.dest('./templates/'));
});
 
 
// Default gulp task to run 
gulp.task('default', function(){
  //gulp.watch('.haml/**/*.haml', ['haml']);
  gulp.start(['haml', 'less']);   // alternative to watch
});
 
