#ifndef PHP_GEOIPO_H
#define PHP_GEOIPO_H 1

#include "php.h"

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_GEOIPO_VERSION "0.0.1"
#define PHP_GEOIPO_EXTNAME "geoipo"

#include <GeoIP.h>

// define the module entry point.
extern zend_module_entry geoipo_module_entry;
#define phpext_geoipo_ptr &geoipo_module entry

typedef struct _class_const_list {
	const char *cc; // class constant (for php)
	long lc;        // library constant (from geoip)
} class_const_list;

zval *geoipo_get_object_property(zval *, const char *);

#define GEOIPO_ERROR_NO_DATABASE "Unable to open database `%s`."
#define GEOIPO_ERROR_INVALID_DBID "Invalid database selection `%d`."

#include "config.h"
#include "src/class_geoip.h"

#endif
