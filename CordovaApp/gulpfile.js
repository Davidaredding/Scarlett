var gulp = require('gulp');
var connect = require('gulp-connect');
var sass = require('gulp-sass');
var es6 = require('gulp-es6-transpiler');
var concat = require('gulp-concat');
var concatCSS = require('gulp-concat-css');
var uglify = require('gulp-uglify');
var inject = require('gulp-inject');
var clean = require('gulp-clean');
var runSequence = require('run-sequence');


// DEV TASKS
// serve to localhost:8080 with live reload
gulp.task('serve', function(){
	connect.server({
		root: './src',
		livereload: true
	});

	// gulp-inject will inject css/js files into the index.html
	var target = gulp.src('./src/index.html');
	var sources = gulp.src([
		'./src/css/*.css', 
		'./src/css/font-awesome-4.4.0/css/font-awesome.min.css',
		'./src/js/*.js',
		'./src/js/ng-app/**/*.js',
		'./src/js/node_modules/angular/angular.min.js',
		'./src/js/node_modules/angular-route/angular-route.min.js'
		], {});
	return target.pipe(inject(sources))
		.pipe(gulp.dest('./src'));
});

 // SASS runner
gulp.task('sass', function() {
	gulp.src('./src/scss/main.scss')
		.pipe(sass({
			includePaths: './src/scss/',
			outputStyle: 'compressed'
		}).on('error', sass.logError))
		.pipe(gulp.dest('./src/css'))
		.pipe(connect.reload()); // reload page
});

// ES6 Transpiler
gulp.task('es6', function() {
	// your stuff here, Dave!
});

// Watch task - watches for file changes
gulp.task('watch', function(){
	gulp.watch('./src/scss/**/*.scss', ['sass']);
	// gulp.watch('./files/to/watch.yes'), ['tasks']);
});

// BUILD TASKS

// Clean www folder
gulp.task('clean', function() {
	return gulp.src('./www/', {read: false})
		.pipe(clean());
});

// Consolidate Vendors by concatenating scripts, then uglify
gulp.task('uglify', function() {
	return gulp.src([
	  	'./src/js/*.js',
	  	'./src/js/node_modules/**/*.min.js',
	  	'./src/js/ng-app/**/*.js'
  	])
  	.pipe(concat('scripts.min.js'))
    .pipe(uglify())
    .pipe(gulp.dest('./www/js/'));
});

gulp.task('concatCSS', function () {
	return gulp.src(['./src/css/*.css', './src/css/font-awesome-4.4.0/css/font-awesome.min.css',])
		.pipe(concatCSS('main.css'))
		.pipe(gulp.dest('./www/css/'))
});

gulp.task('inject', function () {
	var target = gulp.src('./src/index.html');
	var sources = gulp.src([
		'./www/css/*.css', 
		'./www/js/*.js',
	], {});
	return target.pipe(inject(sources))
		.pipe(gulp.dest('./www/'));});

gulp.task('copy', function () {
	gulp.src('./src/img/**/*.*')
		// .pipe($.copy('./www/img/'));
		.pipe(gulp.dest('./www/img/'));
	gulp.src('./src/Views/**/*.html')
		.pipe(gulp.dest('./www/Views/'));
});

// TASK CLI
// just run 'gulp'
gulp.task('default', ['serve', 'watch']);
// 'gulp build'
gulp.task('build', function(callback) {
	runSequence('clean', ['concatCSS', 'uglify', 'copy'], 'inject', callback);
});
