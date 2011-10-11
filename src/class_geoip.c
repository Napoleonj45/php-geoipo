
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

	//. register GeoIP constants
	int a;
	class_const_list item;
	for(a = 0; a < sizeof(class_geoip_constants); a++) {	
		item = (class_const_list)class_geoip_constants[a];
		if(item.cc == NULL) break;

		zend_declare_class_constant_long(
			class_geoip_ce,
			item.cc, strlen(item.cc),
			(long)item.lc TSRMLS_CC
		);		
	}

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

/* void GeoIP::init(optional string custom_directory)
 * Initalize the GeoIP library for use. This is a user function so that
 * a custom directory can be used for the databases. (GeoIP C library is
 * a set and you-have-to-forget onetimesetall). */

PHP_METHOD(GeoIP, init) {
	char *dir;
	int dir_len;

 	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &dir, &dir_len);
 
 	//. init the custom directory
	if(dir_len) {
		GeoIP_setup_custom_directory(dir);
	}
	
	//. init the database files in the library.
	_GeoIP_setup_dbfilename();
 
 	return;
}

/* void GeoIP::__construct(optional string host)
 * Create an instance of the GeoIP object. If the hostname is given then
 * store that in the public host property. */
 
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

/* string GeoIP->getCountry(void);
 * Get the 2 character country code for the currently stored host value
 * from the GeoIP database. */
 
PHP_METHOD(GeoIP, getCountry) {
	GeoIP *geo;
	char *country;
	
	zval *this = getThis();	
	zval *host = geoipo_get_object_property(this,"host");
	
	if(!GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING,
			"Unable to find database `%s`",
			GeoIPDBFileName[GEOIP_COUNTRY_EDITION]
		);	
		RETURN_FALSE;
	}

	geo = GeoIP_open_type(GEOIP_COUNTRY_EDITION,GEOIP_STANDARD);
	country = GeoIP_country_code_by_name(geo,Z_STRVAL_P(host));
	GeoIP_delete(geo);

	if(country == NULL) return;
	RETURN_STRING(country, 1);		
}
