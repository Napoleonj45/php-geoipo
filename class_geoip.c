
#include "php_geoipo.h"

zend_class_entry *class_geoip_ce;

#pragma mark PHP_MINIT_FUNCTION
PHP_MINIT_FUNCTION(class_geoip) {
	zend_class_entry ce;

	//. register GeoIP.
	INIT_CLASS_ENTRY(ce, "GeoIP", class_geoip_methods);
	class_geoip_ce = zend_register_internal_class(&ce TSRMLS_CC);
	class_geoip_ce->create_object = obj_geoip_new;
	zend_declare_property_null(class_geoip_ce,"host",(sizeof("host")-1),ZEND_ACC_PUBLIC TSRMLS_CC);
	memcpy(&obj_geoip_handlers,zend_get_std_object_handlers(),sizeof(zend_object_handlers));
	obj_geoip_handlers.write_property = obj_geoip_write_property;

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
	
	zend_declare_class_constant_string(
		class_geoip_ce,
		"LIBRARY_VERSION", strlen("LIBRARY_VERSION"),
		GeoIP_lib_version() TSRMLS_CC
	);	

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
	object->recordcache = NULL;
	object->regioncache = NULL;
	
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
		(zend_objects_free_object_storage_t)obj_geoip_free,
		NULL TSRMLS_CC
	);
	
	output.handlers = &obj_geoip_handlers;
	return output;
}

void
obj_geoip_write_property(zval *object, zval *member, zval *value TSRMLS_DC) {
	obj_geoip_s *this = (obj_geoip_s *)zend_objects_get_address(object TSRMLS_CC);

	//. if the host property of this object is changed then all caches should 
	//. be invalidated.
	if(!strncmp(Z_STRVAL_P(member),"host",4)) {
		//php_printf("dumping cache due to host change\n");
		
		if(this->recordcache != NULL) {
			geoipo_record_cache_destroy(&this->recordcache);
			this->recordcache = NULL;
		}
	
		if(this->regioncache != NULL) {
			geoipo_region_cache_destroy(&this->regioncache);
			this->regioncache = NULL;
		}
	}

	//. and the default write behaviour should continue.
	zend_get_std_object_handlers()->write_property(
		object,
		member,
		value TSRMLS_CC
	);
	
	return;
}

void
obj_geoip_free(void *obj TSRMLS_DC) {
	obj_geoip_s *object = (obj_geoip_s *)obj;

	if(object->recordcache != NULL)
	geoipo_record_cache_destroy(&object->recordcache);

	if(object->regioncache != NULL)
	geoipo_region_cache_destroy(&object->regioncache);

	efree(obj);
	object = NULL;
	return;
}

#pragma mark string GeoIP::getDatabaseFilename(int GeoIP::CONSTANT);
/* returns the file path to the specified database. The database may
or may not actually exist on disk. */

PHP_METHOD(GeoIP, getDatabaseFile) {
	long dbid = 0;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &dbid) == FAILURE) {
		RETURN_FALSE;
	}
	
	if(GEOIPOG(geoipo_has_initd) == 0) geoipo_init(TSRMLS_C);
	
	if(dbid < 0 || dbid >= NUM_DB_TYPES) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_INVALID_DBID,
			dbid
		); RETURN_FALSE;
	}

	RETURN_STRING(GeoIPDBFileName[dbid],1);
}

#pragma mark string GeoIP::getDatabaseInfo(int GeoIP::CONSTANT);
/* returns whatever info libGeoIP wants to return about for the
specified database. Generally this will end up telling you about
the database version number and copyright info. */

PHP_METHOD(GeoIP, getDatabaseInfo) {
	long dbid = 0;
	char *info;
	GeoIP *geo;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &dbid) == FAILURE) {
		RETURN_FALSE;
	}
	
	if(GEOIPOG(geoipo_has_initd) == 0) geoipo_init(TSRMLS_C);
	
	if(dbid < 0 || dbid >= NUM_DB_TYPES) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_INVALID_DBID,
			dbid
		); RETURN_FALSE;
	}
	
	if(!GeoIP_db_avail(dbid)) {
		RETURN_FALSE;
	}
	
	geo = GeoIP_open_type(dbid,GEOIP_STANDARD);
	info = GeoIP_database_info(geo); // malloc'd
	GeoIP_delete(geo);

	RETVAL_STRING(info,1);
	free(info);
	return;
}

#pragma mark string GeoIP::getRegionName(string rcode);
/* return the name of the region by the region code. */

PHP_METHOD(GeoIP, getRegionName) {
	char *ccode;
	int ccodelen;
	char *rcode;
	int rcodelen;
	const char *rname;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &ccode, &ccodelen, &rcode, &rcodelen) == FAILURE) {
		RETURN_FALSE;
	}
	
	rname = GeoIP_region_name_by_code(ccode,rcode);

	if(rname == NULL) {
		RETURN_BOOL(0);
	} else {
		RETURN_STRING(rname,1);
	}
	
}

#pragma mark string GeoIP::getTimeZone(string ccode, string rcode);
/* return the timezone of a specific country region, by country and
region. */

PHP_METHOD(GeoIP, getTimeZone) {
	char *ccode;
	int ccodelen;
	char *rcode;
	int rcodelen;
	const char *timezone;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &ccode, &ccodelen, &rcode, &rcodelen) == FAILURE) {
		RETURN_FALSE;
	}
	
	timezone = GeoIP_time_zone_by_country_and_region(ccode,rcode);
	
	if(timezone == NULL) {
		RETURN_BOOL(0);
	} else {
		RETURN_STRING(timezone,1);
	}

	return;
}

#pragma mark boolean GeoIP::hasDatabase(int GeoIP::CONSTANT);
/* returns a boolean value describing if the database you specified
actually exists on disk for reals or not. */

PHP_METHOD(GeoIP, hasDatabase) {
	long dbid = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &dbid) == FAILURE) {
		RETURN_FALSE;
	}

	if(GEOIPOG(geoipo_has_initd) == 0) geoipo_init(TSRMLS_C);

	if(dbid < 0 || dbid >= NUM_DB_TYPES) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_INVALID_DBID,
			dbid
		); RETURN_FALSE;
	}
	
	if(!GeoIP_db_avail(dbid)) { RETURN_FALSE; }
	else { RETURN_TRUE; }

	return;
}

#pragma mark void GeoIP::init(optional string directory);
/* initialize the GeoIP library. this is done automatically for you
if you try to use a GeoIP method without having done it already.
however if you do it yourself before you do anything else with GeoIP
you can specify a custom directory for the library to find the
databases instead of the default systemwide ones. */

PHP_METHOD(GeoIP, init) {
	char *dir = NULL;
	int dir_len = 0;

	if(GEOIPOG(geoipo_has_initd) == 1) {
	
		//. as of today (libgeoip 1.4.8) this function appears to be safe to
		//. use as many times as wanted to flush the database paths and
		//. regenerate them. If this is true then this method in its current
		//. state can be renamed to setDatabaseDirectory.
		GeoIP_cleanup();
		
	}

 	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &dir, &dir_len);
 
 	//. init the custom directory
	GeoIP_setup_custom_directory(dir);
	
	//. init the database files in the library.
	geoipo_init(TSRMLS_C);
 
 	return;
}

#pragma mark array GeoIP::listCountryCodes(optional int length default 2);
/* return an assoc array of all the country codes that are statically
defined in the GeoIP library. The array keys will be the code and the
value will be the country name. Valid values for the length are 2 or 3. */

PHP_METHOD(GeoIP, listCountryCodes) {
	int a = 0;
	unsigned int cnum = GeoIP_num_countries();
	long abbrlen = 0;
	
 	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &abbrlen);
 	if(abbrlen != 3) abbrlen = 2;	
	
	array_init(return_value);
	
	for(a = 0; a < cnum; a++) {
		add_assoc_string(
			return_value,
			((abbrlen==3)?(GeoIP_code3_by_id(a)):(GeoIP_code_by_id(a))),
			(char*)GeoIP_name_by_id(a), 1
		);
	}

	return;
}

#pragma mark array GeoIP::listDatabases(void);
/* list all the databases that GeoIP could use. this does not mean they
are really there, check the property for that. but if they were there
then let me tell you... */

PHP_METHOD(GeoIP, listDatabases) {
	int a = 0;
	zval *obj;
	
	if(GEOIPOG(geoipo_has_initd) == 0) geoipo_init(TSRMLS_C);	
	
	array_init(return_value);
	for(a = 0; a < NUM_DB_TYPES; a++) {
		if(GeoIPDBDescription[a] == NULL) continue;
		if(GeoIPDBFileName[a] == NULL) continue;

		ALLOC_INIT_ZVAL(obj);
		object_init(obj);
		add_property_long(  obj, "DBID",        a                              );
		add_property_string(obj, "Description", (char*)GeoIPDBDescription[a], 1);
		add_property_string(obj, "Filename",    (char*)GeoIPDBFileName[a],    1);
		add_property_bool(  obj, "Available",   GeoIP_db_avail(a)              );
	
		add_next_index_zval(return_value, obj);
	}

	return;
}

#pragma mark void GeoIP->__construct(optional string host);
/* this is your default object constructor for when an object is
created. if an optional string is given then that is the host name
that will be set default for you in the public host property. */
 
PHP_METHOD(GeoIP, __construct) { 	
 	char *host;
	int host_len;

	zval *this = getThis();
	zval *member;
	zval *value;

	//. if the paths have not yet been initalized it should be done right
	//. now before an object tries to read from the databases.
	if(GEOIPOG(geoipo_has_initd) == 0) geoipo_init(TSRMLS_C);
 	
 	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &host, &host_len) == FAILURE) {
		return;
 	}
 
	MAKE_STD_ZVAL(member); ZVAL_STRING(member,"host",1);
	MAKE_STD_ZVAL(value); ZVAL_STRING(value,host,1);
	obj_geoip_handlers.write_property(this,member,value TSRMLS_CC);
 	
 	return;
}

#pragma mark string GeoIP->getContinent(void);
/* this will return the continent code for the host that is currently
specified on the object. */

PHP_METHOD(GeoIP, getContinentCode) {
	GeoIP *geo;
	int    ccode;
	zval  *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);

	if(!GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_NO_DATABASE,
			GeoIPDBFileName[GEOIP_COUNTRY_EDITION]
		); RETURN_FALSE;
	}
	
	geo = GeoIP_open_type(GEOIP_COUNTRY_EDITION,GEOIP_STANDARD);
	ccode = GeoIP_id_by_name(geo,Z_STRVAL_P(host));
	GeoIP_delete(geo);
	
	if(ccode == 0) { RETURN_FALSE; }
	else { RETURN_STRING(GeoIP_country_continent[ccode],1); }
}

#pragma mark string GeoIP->getCountryCode(optional int length default 2)
/* return the country code for the host that is currently specified on
the object. by default it will return the 2 character code, however if
you request the 3 character code using the optional argument then that
is what you will get. valid values for the optional argument are
limited to the value 2 or 3. */
 
PHP_METHOD(GeoIP, getCountryCode) {
	GeoIP       *geo;
	const char  *country;
	long         abbrlen = 0;
	zval        *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);

 	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &abbrlen);
 	if(abbrlen != 3) abbrlen = 2;
 	
	if(!GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_NO_DATABASE,
			GeoIPDBFileName[GEOIP_COUNTRY_EDITION]
		); RETURN_FALSE;
	}

	geo = GeoIP_open_type(GEOIP_COUNTRY_EDITION,GEOIP_STANDARD);
	if(abbrlen == 3) country = GeoIP_country_code3_by_name(geo,Z_STRVAL_P(host));
	else             country = GeoIP_country_code_by_name(geo,Z_STRVAL_P(host));
	GeoIP_delete(geo);

	if(country == NULL) { RETURN_FALSE; }
	else { RETURN_STRING(country,1); }
}

#pragma mark string GeoIP->getCountryName(void);
/* return the country name for the host that is currently specified
on the object. */

PHP_METHOD(GeoIP, getCountryName) {

	GeoIP       *geo;
	const char  *country;
	zval        *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);
 	
	if(!GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_NO_DATABASE,
			GeoIPDBFileName[GEOIP_COUNTRY_EDITION]
		); RETURN_FALSE;
	}

	geo = GeoIP_open_type(GEOIP_COUNTRY_EDITION,GEOIP_STANDARD);
	country = GeoIP_country_name_by_name(geo,Z_STRVAL_P(host));
	GeoIP_delete(geo);

	if(country == NULL) { RETURN_FALSE; }
	else { RETURN_STRING(country,1); }
}

#pragma mark int GeoIP->getID(void);
/* return the "id" for the host that is currently specified on the
object. it seems that the id is a unique integer standing for the
country. */

PHP_METHOD(GeoIP, getID) {
	
	GeoIP *geo;
	int    id;
	zval  *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);

	if(!GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_NO_DATABASE,
			GeoIPDBFileName[GEOIP_COUNTRY_EDITION]
		); RETURN_FALSE;
	}

	geo = GeoIP_open_type(GEOIP_COUNTRY_EDITION,GEOIP_STANDARD);
	id = GeoIP_id_by_name(geo,Z_STRVAL_P(host));
	GeoIP_delete(geo);

	if(id == 0) { RETURN_FALSE; }
	else { RETURN_LONG(id); }
}

PHP_METHOD(GeoIP, getISP) {

	GeoIP *geo;
	char  *isp;
	zval  *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);
	
	if(!GeoIP_db_avail(GEOIP_ISP_EDITION)) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_NO_DATABASE,
			GeoIPDBFileName[GEOIP_ISP_EDITION]
		); RETURN_FALSE;
	}
	
	geo = GeoIP_open_type(GEOIP_ISP_EDITION,GEOIP_STANDARD);
	isp = GeoIP_name_by_name(geo,Z_STRVAL_P(host)); // malloc'd
	GeoIP_delete(geo);
	
	if(isp == NULL) {
		RETURN_FALSE;
	}
	
	RETVAL_STRING(isp,1);
	free(isp);
}

#pragma mark object GeoIP->getRecord(void)
/* returns an object of values for the host currently specified on the
object. this object is cached in the object instance so that multiple
calls to this pulling field names out will not cause database lookups
for the same data over and over. if you need most all of the
information at once, this is your best method to use. */

/* returned object properties:
 * string ContinentCode
 * string CountryCode
 * string CountryCode3
 * string CountryName
 * string RegionCode
 * string RegionName
 * string City
 * string PostalCode
 * double Latitude
 * double Longitude
 * string TimeZone
 */

PHP_METHOD(GeoIP, getRecord) {
	zval                *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);
	obj_geoip_s         *this = (obj_geoip_s *)zend_objects_get_address(getThis() TSRMLS_CC);
	geoipo_record_cache *cache = NULL;

	if(!GeoIP_db_avail(GEOIP_CITY_EDITION_REV1) && !GeoIP_db_avail(GEOIP_CITY_EDITION_REV0)) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_NO_DATABASE,
			GeoIPDBFileName[GEOIP_CITY_EDITION_REV1]
		); RETURN_FALSE;
	}

	if(this->recordcache == NULL) {
		//php_printf("record seek %s\n",Z_STRVAL_P(host));
		cache = geoipo_record_get(Z_STRVAL_P(host));
		if(cache == NULL) {
			RETURN_FALSE;
		} else {
			this->recordcache = cache;
		}
	} else {
		//php_printf("record reuse %s\n",Z_STRVAL_P(host));
		cache = this->recordcache;
	}
	
	if(cache == NULL) {
		RETURN_FALSE;
	}
	
	//. build the return object.
	object_init(return_value);
	geoipo_property_set_or_false_from_zval(return_value, "ContinentCode", cache->ContinentCode, IS_STRING TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "CountryCode",   cache->CountryCode,   IS_STRING TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "CountryCode3",  cache->CountryCode3,  IS_STRING TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "CountryName",   cache->CountryName,   IS_STRING TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "RegionCode",    cache->RegionCode,    IS_STRING TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "RegionName",    cache->RegionName,    IS_STRING TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "City",          cache->City,          IS_STRING TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "PostalCode",    cache->PostalCode,    IS_STRING TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "Latitude",      cache->Latitude,      IS_DOUBLE TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "Longitude",     cache->Longitude,     IS_DOUBLE TSRMLS_CC);
	geoipo_property_set_or_false_from_zval(return_value, "TimeZone",      cache->TimeZone,      IS_STRING TSRMLS_CC);

	return;
}

#pragma mark object GeoIP->getRegion(void);
/* returns an object of values for the host currently specified on the
object. this result is cached in the same way as getRecord. if the
official region database exists it will attempt to use it because it
will be smaller ergo faster to scan. else it will use the city
database which contains the same information. if you only need basic
country info about a host, this is your best method to use. */

/* returned object properties:
 * string CountryCode
 * string RegionCode
 * string RegionName
 * string TimeZone
 */

PHP_METHOD(GeoIP, getRegion) {
	zval                *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);
	obj_geoip_s         *this = (obj_geoip_s *)zend_objects_get_address(getThis() TSRMLS_CC);

	//. use the region database if it exists.
	if(GeoIP_db_avail(GEOIP_REGION_EDITION_REV1) || GeoIP_db_avail(GEOIP_REGION_EDITION_REV0)) {
		geoipo_region_cache *cache;
	
		if(this->regioncache == NULL) {
			cache = geoipo_region_get(Z_STRVAL_P(host));
			if(cache == NULL) {
				RETURN_FALSE;
			} else {
				this->regioncache = cache;
			}
		} else {
			cache = this->regioncache;
		}

		if(cache == NULL) {
			RETURN_FALSE;
		}
		
		object_init(return_value);
		geoipo_property_set_or_false_from_zval(return_value, "CountryCode",   cache->CountryCode,   IS_STRING TSRMLS_CC);
		geoipo_property_set_or_false_from_zval(return_value, "RegionCode",    cache->RegionCode,    IS_STRING TSRMLS_CC);
		geoipo_property_set_or_false_from_zval(return_value, "RegionName",    cache->RegionName,    IS_STRING TSRMLS_CC);
		geoipo_property_set_or_false_from_zval(return_value, "TimeZone",      cache->TimeZone,      IS_STRING TSRMLS_CC);
		return;
	}
	
	//. else use the city database.
	else if(GeoIP_db_avail(GEOIP_CITY_EDITION_REV1) || GeoIP_db_avail(GEOIP_CITY_EDITION_REV0)) {
		geoipo_record_cache *cache;
		
		if(this->recordcache == NULL) {
			//php_printf("record seek %s\n",Z_STRVAL_P(host));
			cache = geoipo_record_get(Z_STRVAL_P(host));
			if(cache == NULL) {
				RETURN_FALSE;
			} else {
				this->recordcache = cache;
			}
		} else {
			//php_printf("record reuse %s\n",Z_STRVAL_P(host));
			cache = this->recordcache;
		}

		if(cache == NULL) {
			RETURN_FALSE;
		}

		object_init(return_value);
		geoipo_property_set_or_false_from_zval(return_value, "CountryCode",   cache->CountryCode,   IS_STRING TSRMLS_CC);
		geoipo_property_set_or_false_from_zval(return_value, "RegionCode",    cache->RegionCode,    IS_STRING TSRMLS_CC);
		geoipo_property_set_or_false_from_zval(return_value, "RegionName",    cache->RegionName,    IS_STRING TSRMLS_CC);
		geoipo_property_set_or_false_from_zval(return_value, "TimeZone",      cache->TimeZone,      IS_STRING TSRMLS_CC);
		return;
	}
	
	else {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_NO_DATABASE,
			GeoIPDBFileName[GEOIP_CITY_EDITION_REV1]
		); RETURN_FALSE;
	}

}
