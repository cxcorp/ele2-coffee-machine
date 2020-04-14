#ifndef COFFEE_SENSOR_CONFIG_WEB_ASSETS_H
#define COFFEE_SENSOR_CONFIG_WEB_ASSETS_H

#include <Arduino.h>

namespace ConfigWebAssets {

// Pre-gzipped bootstrap.min.css for styling
// minimal size achieved with Google's zopfli utility
extern const size_t bootstrap_css_gz_len;
extern PGM_P bootstrap_css_gz;

extern PGM_P index_min_html;

}

#endif
