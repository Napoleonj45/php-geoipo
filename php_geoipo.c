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

	GeoIP_cleanup();

	return SUCCESS;
}

PHP_MINFO_FUNCTION(geoipo) {
	php_info_print_table_start();
	php_info_print_table_row(2,"GeoIP OOP","enabled");
	php_info_print_table_row(2,"libgeoip Version",GeoIP_lib_version());
	php_info_print_table_end();
}

//. stand alone functions.
static zend_function_entry geoipo_functions[] = {
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
geoipo_get_object_property(zval *object, const char *property TSRMLS_DC) {
	zval *member;
	zval *value;
	zval *output;

	MAKE_STD_ZVAL(member); ZVAL_STRING(member,property,1);
	value = Z_OBJ_HT_P(object)->read_property(object,member,BP_VAR_IS,0 TSRMLS_CC);

	MAKE_STD_ZVAL(output);
	ZVAL_STRING(output,Z_STRVAL_P(value),1);
	return output;
}

void
geoipo_init(TSRMLS_D) {
	_GeoIP_setup_dbfilename();
	GEOIPOG(geoipo_has_initd) = 1;
	return;
}

/* given a zval, a value, and the type you are aiming for attempt
to set the zval to that value with that type. however if the value
was NULL then instead set the zval to boolean false. */

void
geoipo_zval_set_or_false(zval *z, void *value, long type) {
	switch(type) {
		case IS_DOUBLE: {
			if(value == NULL) { ZVAL_BOOL(z,0); } 
			else { ZVAL_DOUBLE(z,*(double*)value); }
			break;
		}
		case IS_STRING: {
			if(value == NULL) { ZVAL_BOOL(z,0); } 
			else { ZVAL_STRING(z,(char*)value,1); }
			break;
		}
	}
	
	return;
}

/* given an object, property, value, set the property to the value if
it is the type we wanted. else set the type to boolean false. this is
to force strings to copy for our result objects. */

void
geoipo_property_set_or_false_from_zval(zval *z, char *key, zval *value, long type TSRMLS_DC) {
	if(value == NULL) {
		add_property_bool(z,key,0);
		return;
	}

	switch(type) {
		case IS_DOUBLE: {
			if(Z_TYPE_P(value) != IS_DOUBLE) add_property_bool(z,key,0);
			else add_property_double(z,key,Z_DVAL_P(value));
			break;
		}
		case IS_STRING: {
			if(Z_TYPE_P(value) != IS_STRING) add_property_bool(z,key,0);
			else add_property_string(z,key,Z_STRVAL_P(value),1);
			break;
		}
	}
	
	return;
}

//////////////////////////////////////////
// geoip record cache ////////////////////

/* allocate space in memory for my cache object and initate all the
zvals in it. returns the pointer to the allocated structure. */
void
geoipo_record_cache_init(geoipo_record_cache **input) {
	*input = ecalloc(sizeof(geoipo_record_cache),1);

	MAKE_STD_ZVAL((*input)->ContinentCode);
	MAKE_STD_ZVAL((*input)->CountryCode);
	MAKE_STD_ZVAL((*input)->CountryCode3);
	MAKE_STD_ZVAL((*input)->CountryName);
	MAKE_STD_ZVAL((*input)->RegionCode);
	MAKE_STD_ZVAL((*input)->RegionName);
	MAKE_STD_ZVAL((*input)->City);
	MAKE_STD_ZVAL((*input)->PostalCode);
	MAKE_STD_ZVAL((*input)->Latitude);
	MAKE_STD_ZVAL((*input)->Longitude);
	MAKE_STD_ZVAL((*input)->TimeZone);

	return;
}

/* destroy an allocated cache structure. destroy all the zvals in it
and then free the ram that was assigned to the structure */

void
geoipo_record_cache_destroy(geoipo_record_cache **input) {

	zval_dtor((*input)->ContinentCode);
	zval_dtor((*input)->CountryCode);
	zval_dtor((*input)->CountryCode3);
	zval_dtor((*input)->CountryName);
	zval_dtor((*input)->RegionCode);
	zval_dtor((*input)->RegionName);
	zval_dtor((*input)->City);
	zval_dtor((*input)->PostalCode);
	zval_dtor((*input)->Latitude);
	zval_dtor((*input)->Longitude);
	zval_dtor((*input)->TimeZone);
	
	efree(*input);
	*input = NULL;

	return;
}

//////////////////////////////////////////
// geoip region cache ////////////////////

void
geoipo_region_cache_init(geoipo_region_cache **input) {
	*input = ecalloc(sizeof(geoipo_region_cache),1);

	MAKE_STD_ZVAL((*input)->CountryCode);
	MAKE_STD_ZVAL((*input)->RegionCode);
	MAKE_STD_ZVAL((*input)->RegionName);
	MAKE_STD_ZVAL((*input)->TimeZone);

	return;
}

void
geoipo_region_cache_destroy(geoipo_region_cache **input) {

	zval_dtor((*input)->CountryCode);
	zval_dtor((*input)->RegionCode);
	zval_dtor((*input)->RegionName);
	zval_dtor((*input)->TimeZone);
	
	efree((*input));
	(*input) = NULL;

	return;
}

//////////////////////////////////////////
// geoip record get //////////////////////

geoipo_record_cache *
geoipo_record_get(char *host) {
	GeoIP               *geo;
	GeoIPRecord         *record;
	geoipo_record_cache *out;
	const char *regionname;
	const char *timezone;
	double dlong,dlat;

	// try to get the proper database open.
	if(GeoIP_db_avail(GEOIP_CITY_EDITION_REV1))
	geo = GeoIP_open_type(GEOIP_CITY_EDITION_REV1,GEOIP_STANDARD);
	else if(GeoIP_db_avail(GEOIP_CITY_EDITION_REV1))
	geo = GeoIP_open_type(GEOIP_CITY_EDITION_REV0,GEOIP_STANDARD);
	else return NULL;
	
	// read out the record.
	record = GeoIP_record_by_name(geo,host);
	GeoIP_delete(geo);
	
	// no record quit, else allocate the space for it.
	if(record == NULL) return NULL;
	else geoipo_record_cache_init(&out);

	// i want to extend the record info a little.
	regionname = GeoIP_region_name_by_code(record->country_code,record->region);
	timezone = GeoIP_time_zone_by_country_and_region(record->country_code,record->region);
	dlat = (double)record->latitude;
	dlong = (double)record->longitude;

	// plop in the data struct.
	geoipo_zval_set_or_false(out->ContinentCode, record->continent_code, IS_STRING);
	geoipo_zval_set_or_false(out->CountryCode,   record->country_code,   IS_STRING);
	geoipo_zval_set_or_false(out->CountryCode3,  record->country_code3,  IS_STRING);
	geoipo_zval_set_or_false(out->CountryName,   record->country_name,   IS_STRING);
	geoipo_zval_set_or_false(out->RegionCode,    record->region,         IS_STRING);
	geoipo_zval_set_or_false(out->RegionName,    (char*)regionname,      IS_STRING);
	geoipo_zval_set_or_false(out->City,          record->city,           IS_STRING);
	geoipo_zval_set_or_false(out->PostalCode,    record->postal_code,    IS_STRING);
	geoipo_zval_set_or_false(out->Latitude,      &dlat,                  IS_DOUBLE);
	geoipo_zval_set_or_false(out->Longitude,     &dlong,                 IS_DOUBLE);
	geoipo_zval_set_or_false(out->TimeZone,      (char*)timezone,        IS_STRING);
	
	GeoIPRecord_delete(record);
	return out;
}

geoipo_region_cache *
geoipo_region_get(char *host) {
	GeoIP               *geo;
	GeoIPRegion         *region;
	geoipo_region_cache *out;
	const char          *regionname;
	const char          *timezone;
	
	// open the database
	if(GeoIP_db_avail(GEOIP_REGION_EDITION_REV1))
	geo = GeoIP_open_type(GEOIP_REGION_EDITION_REV1,GEOIP_STANDARD);
	else if(GeoIP_db_avail(GEOIP_REGION_EDITION_REV0))
	geo = GeoIP_open_type(GEOIP_REGION_EDITION_REV0,GEOIP_STANDARD);
	else return NULL;
	
	// read out the region
	region = GeoIP_region_by_name(geo,host);
	GeoIP_delete(geo);

	// no region quit, else allocate space.
	if(region == NULL) return NULL;
	else geoipo_region_cache_init(&out);

	regionname = GeoIP_region_name_by_code(region->country_code,region->region);
	timezone = GeoIP_time_zone_by_country_and_region(region->country_code,region->region);

	// plop in the data struct.
	geoipo_zval_set_or_false(out->CountryCode,   region->country_code,   IS_STRING);
	geoipo_zval_set_or_false(out->RegionCode,    region->region,         IS_STRING);
	geoipo_zval_set_or_false(out->RegionName,    (char*)regionname,      IS_STRING);
	geoipo_zval_set_or_false(out->TimeZone,      (char*)timezone,        IS_STRING);
	
	GeoIPRegion_delete(region);
	return out;
}

//////////////////////////////////////////
//////////////////////////////////////////
