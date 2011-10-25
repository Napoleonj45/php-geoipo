#ifndef PHP_GEOIPO_H
#define PHP_GEOIPO_H 1

#include "php.h"

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_GEOIPO_VERSION "1.0.0"
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

//////////////////////////////////////////
// geoip utility functions ///////////////

void geoipo_zval_set_or_false(zval *, void *, long);

//////////////////////////////////////////
// geoip record cache ////////////////////

typedef struct _geoipo_record_cache {
	zval *ContinentCode;
	zval *CountryCode;
	zval *CountryCode3;
	zval *CountryName;
	zval *RegionCode;
	zval *RegionName;
	zval *City;
	zval *PostalCode;
	zval *Latitude;
	zval *Longitude;
	zval *TimeZone;
} geoipo_record_cache;

void geoipo_record_cache_init(geoipo_record_cache **);
void geoipo_record_cache_destroy(geoipo_record_cache **);

//////////////////////////////////////////
// geoip region cache ////////////////////

typedef struct _geoipo_region_cache {
	zval *CountryCode;
	zval *RegionCode;
	zval *RegionName;
	zval *TimeZone;
} geoipo_region_cache;

void geoipo_region_cache_init(geoipo_region_cache **);
void geoipo_region_cache_destroy(geoipo_region_cache **);

//////////////////////////////////////////
// geoip data fetching ///////////////////

geoipo_record_cache *geoipo_record_get(char *);
geoipo_region_cache *geoipo_region_get(char *);

//////////////////////////////////////////
//////////////////////////////////////////

#include "config.h"
#include "class_geoip.h"

#endif
