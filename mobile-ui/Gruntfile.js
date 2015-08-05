module.exports = function(grunt) {

	grunt.initConfig({
		watch: {
			sass: {
			  files: 'src/scss/*.*',
			  tasks: ['sass'],
			},
			options: {
				livereload: true
			}
			// all: {
			// 	files: ['src/**'],
			// 	options: {
			// 		livereload: true
			// 	},
			// }
			// connect: {
			// 	files: 'src/*.*',
			// 	options: {
			// 		livereload: true
			// 	}
			// }
		},
		sass: {
			dist: {
				files: {
					'src/css/main.css' : 'src/scss/main.scss',
				}
			}
		},
		connect: {
			options: {
				verbose: true,
				port: 4020,
				hostname: 'localhost',
				base: 'src/',
				// open: true,
				// livereload: 35728
			},
			livereload: {
				options: {
					open: true
				}
			}
		},
		// cssmin: {},
		// imagemin: {},
		// jshint: {},
		copy: {
			main: {
				files: [
					{
						expand: true, 
						cwd: 'src/',
						src: ['*.*', 'css/**', 'js/**', 'img/**'],
						dest: 'dist'
					},
					{
						expand: true, 
						cwd: 'dist/',
						src: ['**'],
						dest: '../CordovaApp/www/'
					},
				]
			}
		}
	});

	grunt.loadNpmTasks('grunt-contrib-connect');
	grunt.loadNpmTasks('grunt-contrib-sass');
	grunt.loadNpmTasks('grunt-contrib-cssmin');
	grunt.loadNpmTasks('grunt-contrib-imagemin');
	grunt.loadNpmTasks('grunt-contrib-jshint');
	grunt.loadNpmTasks('grunt-contrib-watch');
	grunt.loadNpmTasks('grunt-contrib-copy');

	grunt.registerTask('default', ['sass']);
	grunt.registerTask('build', ['copy']);
	grunt.registerTask('serve', ['connect', 'watch', 'sass']);

}