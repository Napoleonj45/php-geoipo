
#include "php_geoipo.h"

//. class and object handlers.
zend_class_entry *class_geoip_ce;
zend_object_handlers obj_geoip_handlers;

//. class method index.
static zend_function_entry
class_geoip_methods[] = {
	PHP_ME(GeoIP, __construct, NULL, (ZEND_ACC_PUBLIC | ZEND_ACC_CTOR))
	{NULL,NULL,NULL}
};

PHP_MINIT_FUNCTION(class_geoip) {
	zend_class_entry ce;
	
	//. register GeoIP.
	INIT_CLASS_ENTRY(ce, "GeoIP", class_geoip_methods);
	class_geoip_ce = zend_register_internal_class(&ce TSRMLS_CC);
	memcpy(&obj_geoip_handlers,zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	return SUCCESS;
}

zend_object_value
obj_geoip_new(zend_class_entry *ce TSRMLS_DC)
{
	obj_geoip_s *tmp;
	return obj_geoip_mknew(ce, &tmp TSRMLS_CC);	
}

zend_object_value
obj_geoip_mknew(zend_class_entry *ce, obj_geoip_s **obj TSRMLS_DC)
{
	zend_object_value output;
	obj_geoip_s *iobj;
	
	//. allocate object space.
	iobj = emalloc(sizeof(obj_geoip_s));
	memset(iobj,0,sizeof(obj_geoip_s));
	
	//. define type.
	iobj->std.ce = ce;
	*obj = iobj;
	
	ALLOC_HASHTABLE(iobj->std.properties);
	zend_hash_init(
		iobj->std.properties,
		0,
		NULL,
		ZVAL_PTR_DTOR,
		0
	);
	zend_hash_copy(
		iobj->std.properties,
		&ce->default_properties,
		(copy_ctor_func_t)zval_add_ref,
		NULL,
		sizeof(zval *)
	);
	
	output.handle = zend_objects_store_put(
		iobj,
		NULL,
		NULL,
		NULL
	);
	
	output.handlers = &obj_geoip_handlers;
	return output;
}

PHP_METHOD(GeoIP, __construct) {
 	obj_geoip_s *this = (obj_geoip_s *)zend_object_store_get_object(getThis() TSRMLS_CC);
 	
 	//. maybe do stuff later here.
 	
 	return;
}
