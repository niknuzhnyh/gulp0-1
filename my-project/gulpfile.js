const { src, dest, watch, parallel, series } = require('gulp');

const scss         = require('gulp-sass');
const concat       = require('gulp-concat');
const browserSync  = require('browser-sync').create();
const uglify       = require('gulp-uglify-es').default;
const autoprefixer = require('gulp-autoprefixer');
const imagemin     = require('gulp-imagemin');
const del          = require('del');

function styles() {
   return src('app/scss/style.scss')
      .pipe(scss({ outputStyle: 'compressed' }))
      .pipe(autoprefixer({
         overrideBrowserslist: ['last 10 version'],
         grid: true
      }))
      .pipe(concat('style.min.css'))
      .pipe(dest("app/css"))
      .pipe(browserSync.stream())

}

function images() {
   return src('app/images/**/*')
      .pipe(imagemin({
         interlaced: true,
         progressive: true,
         optimizationLevel: 5,
         svgoPlugins: [
            {
               removeViewBox: true
            }
         ]
      }))
      .pipe(dest('dist/images'))

}

function browsersync() {
   browserSync.init({
      server: {
         baseDir: "app/"
      }
   });
}

function scripts() {
   return src([
      'node_modules/jquery/dist/jquery.js',
      'app/js/main.js'
   ])
      .pipe(concat('main.min.js'))
      .pipe(uglify())
      .pipe(dest('app/js'))
      .pipe(browserSync.stream())
}

function cleanDist() {
   return del('dist')
}


function build() {
   return src([
      'app/css/style.min.css',
      'app/js/main.min.js',
      'app/fonts/**/*',
      'app/*.html'
   ], { base: 'app' })
      .pipe(dest('dist'))
}

function watching() {
   watch(['app/js/**/*.js', '!app/js/main.min.js'], scripts);
   watch(['app/scss/**/*.scss'], styles);
   watch(['app/*.html']).on('change', browserSync.reload);
}

exports.styles      = styles;
exports.scripts     = scripts;
exports.watching    = watching;
exports.browsersync = browsersync;
exports.images      = images;
exports.cleanDist   = cleanDist;
exports.build       = series(cleanDist, images, build);
exports.default     = parallel(styles, scripts, browsersync, watching);



