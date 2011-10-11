#include "php_geoipo.h"

PHP_MINIT_FUNCTION(geoipo) {

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
