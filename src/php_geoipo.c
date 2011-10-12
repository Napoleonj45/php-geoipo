#include "php_geoipo.h"

ZEND_DECLARE_MODULE_GLOBALS(geoipo)

static void geoipo_init_globals(zend_geoipo_globals *geoipo_globals) {
	geoipo_globals->geoipo_has_initd = 0;
	return;
}


PHP_MINIT_FUNCTION(geoipo) {
	ZEND_INIT_MODULE_GLOBALS(geoipo, geoipo_init_globals, NULL);
	
	PHP_MINIT(class_geoip)(INIT_FUNC_ARGS_PASSTHRU);
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(geoipo) {
	return SUCCESS;
}

PHP_MINFO_FUNCTION(geoipo) {
	php_info_print_table_start();
	php_info_print_table_row(2,"GeoIP OOP","enabled");
	php_info_print_table_end();
}

//. stand alone functions.
static function_entry geoipo_functions[] = {
	{NULL,NULL,NULL}
};

zend_module_entry geoipo_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_GEOIPO_EXTNAME,
	geoipo_functions,
	PHP_MINIT(geoipo),
	PHP_MSHUTDOWN(geoipo),
	NULL,
	NULL,
	PHP_MINFO(geoipo),
	PHP_GEOIPO_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_GEOIPO
ZEND_GET_MODULE(geoipo)
#endif

/* zval *geoipo_get_object_property(zval *object, string property);
 * given and object and a property name, return that property from the
 * object. */

zval *
geoipo_get_object_property(zval *object, const char *property) {
	zval *member;
	zval *value;
	zval *output;

	MAKE_STD_ZVAL(member); ZVAL_STRING(member,property,1);
	value = Z_OBJ_HT_P(object)->read_property(object,member,BP_VAR_IS TSRMLS_CC);

	MAKE_STD_ZVAL(output);
	ZVAL_STRING(output,Z_STRVAL_P(value),1);
	return output;
}

void
geoipo_init() {
	_GeoIP_setup_dbfilename();
	GEOIPOG(geoipo_has_initd) = 1;
	return;
}

/* void geoipo_return_object_property(zval *object, char *key, void *value, long type)
 * This is used mostly on return_value from wrapped methods. It will set the specified
 * object property to the specified value UNLESS the value is NULL. when that is the
 * case then it will set the object property to boolean false. This eliminates pages
 * of:
 * if(!NULL) add_property_string
 * else add_property_boolean
 * if(!NULL) add_property_string
 * else add_property_boolean
 * if(!NULL) add_property_string
 * else add_property_boolean
 */

void
geoipo_return_object_property(zval *object, char *key, void *value, long type) {

	switch(type) {
		case IS_DOUBLE: {
			if(value != NULL) add_property_double(object,key,*(double*)value);
			else add_property_bool(object,key,0);
			break;		
		}
		case IS_LONG: {
			if(value != NULL) add_property_long(object,key,*(long*)value);
			else add_property_bool(object,key,0);
			break;		
		}
		case IS_STRING: {
			if(value != NULL) add_property_string(object,key,value,1);
			else add_property_bool(object,key,0);
			break;
		}
	}

}
