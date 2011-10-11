
typedef struct _obj_geoip_s {
	zend_object  std;
	zval *       host;
} obj_geoip_s;

//. class methods
PHP_METHOD(GeoIP, __construct);
PHP_METHOD(GeoIP, getCountry);

static zend_object_handlers  obj_geoip_handlers;
extern zend_class_entry     *class_geoip_ce;
static zend_function_entry   class_geoip_methods[] = {
	PHP_ME(GeoIP, __construct, NULL, (ZEND_ACC_PUBLIC|ZEND_ACC_CTOR))
	PHP_ME(GeoIP, getCountry,  NULL, ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

//. php class pwners
zend_object_value obj_geoip_new(zend_class_entry * TSRMLS_DC);

//. module functions
PHP_MINIT_FUNCTION(class_geoip);
