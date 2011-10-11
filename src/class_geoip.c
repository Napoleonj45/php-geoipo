
#include "php_geoipo.h"

zend_class_entry *class_geoip_ce;

PHP_MINIT_FUNCTION(class_geoip) {
	zend_class_entry ce;

	//. register GeoIP.
	INIT_CLASS_ENTRY(ce, "GeoIP", class_geoip_methods);
	class_geoip_ce = zend_register_internal_class(&ce TSRMLS_CC);
	class_geoip_ce->create_object = obj_geoip_new;
	zend_declare_property_null(class_geoip_ce,"host",(sizeof("host")-1),ZEND_ACC_PUBLIC TSRMLS_CC);
	memcpy(&obj_geoip_handlers,zend_get_std_object_handlers(),sizeof(zend_object_handlers));
	
	return SUCCESS;
}

zend_object_value
obj_geoip_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value output;
	obj_geoip_s *object;
	
	//. allocate object space.
	object = ecalloc(1,sizeof(obj_geoip_s));
	object->std.ce = ce;
	object->host = NULL;
	
	ALLOC_HASHTABLE(object->std.properties);
	zend_hash_init(
		object->std.properties,
		0,
		NULL,
		ZVAL_PTR_DTOR,
		0
	);
	zend_hash_copy(
		object->std.properties,
		&ce->default_properties,
		(copy_ctor_func_t)zval_add_ref,
		NULL,
		sizeof(zval *)
	);
	
	output.handle = zend_objects_store_put(
		object,
		NULL,
		NULL,
		NULL TSRMLS_CC
	);
	
	output.handlers = &obj_geoip_handlers;
	return output;
}

PHP_METHOD(GeoIP, __construct) { 	
 	char *host;
	int host_len;

	zval *this = getThis();
	zval *member;
	zval *value;
	
 	
 	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &host, &host_len) == FAILURE) {
		return;
 	}
 
	MAKE_STD_ZVAL(member); ZVAL_STRING(member,"host",1);
	MAKE_STD_ZVAL(value); ZVAL_STRING(value,host,1);
	obj_geoip_handlers.write_property(this,member,value);
 	
 	return;
}

PHP_METHOD(GeoIP, getCountry) {
	zval *this = getThis();	
	zval *member;
	zval *host;
	
	MAKE_STD_ZVAL(member); ZVAL_STRING(member,"host",1)
	obj_geoip_handlers.read_property(this,member,BP_VAR_IS);
	
	if(Z_STRVAL_P(host) == NULL) {
		RETURN_FALSE;
	}
	
	puts(Z_STRVAL_P(host));

	return;
}
