
typedef struct _obj_geoip_s {
	zend_object  std;
	GeoIPRecord *recordcache;
	GeoIPRegion *regioncache;
} obj_geoip_s;

//////////////////////////////////////////
// class GeoIP ///////////////////////////

extern char * custom_directory;

// internals.
extern zend_class_entry     *class_geoip_ce;
static zend_object_handlers  obj_geoip_handlers;
zend_object_value            obj_geoip_new(zend_class_entry * TSRMLS_DC);
void                         obj_geoip_write_property(zval *, zval *, zval * TSRMLS_DC);
void                         obj_geoip_free(void * TSRMLS_DC);

// static methods.
PHP_METHOD(GeoIP, getDatabaseFile);
PHP_METHOD(GeoIP, getDatabaseInfo);
PHP_METHOD(GeoIP, hasDatabase);
PHP_METHOD(GeoIP, init);

// instance methods.
PHP_METHOD(GeoIP, __construct);
PHP_METHOD(GeoIP, getContinentCode);
PHP_METHOD(GeoIP, getCountryCode);
PHP_METHOD(GeoIP, getCountryName);
PHP_METHOD(GeoIP, getID);
PHP_METHOD(GeoIP, getRecord);
PHP_METHOD(GeoIP, getRegion);

// method catalogue.
static zend_function_entry   class_geoip_methods[] = {
	PHP_ME(GeoIP, getDatabaseFile,  NULL, (ZEND_ACC_PUBLIC|ZEND_ACC_ALLOW_STATIC))
	PHP_ME(GeoIP, getDatabaseInfo,  NULL, (ZEND_ACC_PUBLIC|ZEND_ACC_ALLOW_STATIC))
	PHP_ME(GeoIP, hasDatabase,      NULL, (ZEND_ACC_PUBLIC|ZEND_ACC_ALLOW_STATIC))
	PHP_ME(GeoIP, init,             NULL, (ZEND_ACC_PUBLIC|ZEND_ACC_ALLOW_STATIC))
	PHP_ME(GeoIP, __construct,      NULL, (ZEND_ACC_PUBLIC|ZEND_ACC_CTOR))
	PHP_ME(GeoIP, getContinentCode, NULL, (ZEND_ACC_PUBLIC))
	PHP_ME(GeoIP, getCountryCode,   NULL, (ZEND_ACC_PUBLIC))
	PHP_ME(GeoIP, getCountryName,   NULL, (ZEND_ACC_PUBLIC))
	PHP_ME(GeoIP, getID,            NULL, (ZEND_ACC_PUBLIC))
	PHP_ME(GeoIP, getRecord,        NULL, (ZEND_ACC_PUBLIC))
	PHP_ME(GeoIP, getRegion,        NULL, (ZEND_ACC_PUBLIC))
	{NULL,NULL,NULL}
};

// constants
static class_const_list      class_geoip_constants[] = {
	{ "ASNUM_EDITION",       GEOIP_ASNUM_EDITION },
	{ "CITY_EDITION_REV0",   GEOIP_CITY_EDITION_REV0 },
	{ "CITY_EDITION_REV1",   GEOIP_CITY_EDITION_REV1 },
	{ "CITY_EDITION",        GEOIP_CITY_EDITION_REV1 },	 
	{ "COUNTRY_EDITION",     GEOIP_COUNTRY_EDITION },
	{ "DOMAIN_EDITION",      GEOIP_DOMAIN_EDITION },
	{ "ISP_EDITION",         GEOIP_ISP_EDITION },
	{ "NETSPEED_EDITION",    GEOIP_NETSPEED_EDITION },
	{ "ORG_EDITION",         GEOIP_ORG_EDITION },
	{ "PROXY_EDITION",       GEOIP_PROXY_EDITION },
	{ "REGION_EDITION_REV0", GEOIP_REGION_EDITION_REV0 },
	{ "REGION_EDITION_REV1", GEOIP_REGION_EDITION_REV1 },
	{ "REGION_EDITION",      GEOIP_REGION_EDITION_REV1 },
	{ "CABLEDSL_SPEED",      GEOIP_CABLEDSL_SPEED },
	{ "CORPORATE_SPEED",     GEOIP_CORPORATE_SPEED },
	{ "DAILUP_SPEED",        GEOIP_DIALUP_SPEED },
	{ "UNKNOWN_SPEED",       GEOIP_UNKNOWN_SPEED },
	{ NULL,   0 }
};

//. module class functions
PHP_MINIT_FUNCTION(class_geoip);
