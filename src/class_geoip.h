
typedef struct _obj_geoip_s {
	zend_object std;
	zval *host;
} obj_geoip_s;

zend_object_value obj_geoip_new(zend_class_entry * TSRMLS_DC);
zend_object_value obj_geoip_mknew(zend_class_entry *, obj_geoip_s ** TSRMLS_DC);

extern zend_class_entry *class_geoip_ce;
static zend_function_entry class_geoip_methods[];
extern zend_object_handlers obj_geoip_handlers;

PHP_METHOD(GeoIP,__construct);

PHP_MINIT_FUNCTION(class_geoip);
