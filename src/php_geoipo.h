#ifndef PHP_GEOIPO_H
#define PHP_GEOIPO_H 1

#include "php.h"

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_GEOIPO_VERSION "0.0.1"
#define PHP_GEOIPO_EXTNAME "geoipo"

#include <GeoIP.h>
#include <GeoIPCity.h>

// define the module entry point.
extern zend_module_entry geoipo_module_entry;
#define phpext_geoipo_ptr &geoipo_module entry

typedef struct _class_const_list {
	const char *cc; // class constant (for php)
	long lc;        // library constant (from geoip)
} class_const_list;

zval *geoipo_get_object_property(zval *, const char * TSRMLS_DC);
void geoipo_init(TSRMLS_D);
void geoipo_return_object_property(zval *, char *, void *, long TSRMLS_DC);

ZEND_BEGIN_MODULE_GLOBALS(geoipo)
int geoipo_has_initd;
ZEND_END_MODULE_GLOBALS(geoipo)
ZEND_EXTERN_MODULE_GLOBALS(geoipo)

#ifdef ZTS
#define GEOIPOG(v) TSRMG(geoipo_globals_id, zend_geoipo_globals *, v)
#else
#define GEOIPOG(v) (geoipo_globals.v)
#endif

#define GEOIPO_ERROR_NO_DATABASE "Unable to open database %s"
#define GEOIPO_ERROR_INVALID_DBID "Invalid database selection %ld"

#include "config.h"
#include "src/class_geoip.h"

#endif
