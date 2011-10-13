
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
			GeoIPRecord_delete(this->recordcache);
			this->recordcache = NULL;
		}
	
		if(this->regioncache != NULL) {
			GeoIPRegion_delete(this->regioncache);
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
	GeoIPRecord_delete(object->recordcache);

	if(object->regioncache != NULL)
	GeoIPRegion_delete(object->regioncache);

	efree(obj);
	object = NULL;
	return;
}

#pragma mark GeoIP::getDatabaseFilename
/* string GeoIP::getDatabaseFilename(int GeoIP::CONSTANT);
 * Return the path to the filename of the selected database. */

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

#pragma mark GeoIP::getDatabaseInfo
/* string GeoIP::getDatabaseInfo(int GeoIP::CONSTANT);
 * Return an info string about the selected database. */

PHP_METHOD(GeoIP, getDatabaseInfo) {
	long   dbid = 0;
	char  *info;
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

	RETVAL_STRING(info,1); free(info);
	return;
}

#pragma mark GeoIP::hasDatabase
/* boolean GeoIP::hasDatabase(int GeoIP::CONSTANT);
 * Returns the boolean value of if a specified database is available on
 * the system. */

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

#pragma mark GeoIP::init
/* void GeoIP::init(optional string custom_directory)
 * Initalize the GeoIP library for use. This is a user function so that
 * a custom directory can be used for the databases. (GeoIP C library is
 * a set and you-have-to-forget onetimesetall). */

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

#pragma mark GeoIP->__construct
/* void GeoIP::__construct(optional string host)
 * Create an instance of the GeoIP object. If the hostname is given then
 * store that in the public host property. */
 
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

#pragma mark GeoIP->getContinent
/* string GeoIP->getContinent(void);
 * Get the contientnet code for wherever the country of the currently
 * stored host value is. */

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
	
	if(ccode == 0) return;
	RETURN_STRING(GeoIP_country_continent[ccode],1)
}

#pragma mark GeoIP->getCountry
/* string GeoIP->getCountry(optional int length);
 * Get the country code for the currently stored host value from the
 * GeoIP database. The optional int length may be 2 or 3, for selecting
 * two or three character abbreviations from GeoIP. The default is two
 * characters. */
 
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

	if(country == NULL) return;
	RETURN_STRING(country,1);		
}

#pragma mark GeoIP->getCountryName
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

	if(country == NULL) return;
	RETURN_STRING(country,1);	
}

#pragma mark GeoIP->getID
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

	RETURN_LONG(id);
}

#pragma mark GeoIP->getRecord
/* object GeoIP->getRecord(void);
 * Returns an object of the record details for the currently specified host.
 */

PHP_METHOD(GeoIP, getRecord) {

	GeoIP       *geo;
	GeoIPRecord *rec;
	char        *regname;
	char        *timezone;
	double       clon;
	double       clat;
	zval        *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);
	obj_geoip_s *this = (obj_geoip_s *)zend_objects_get_address(getThis() TSRMLS_CC);

	if(!GeoIP_db_avail(GEOIP_CITY_EDITION_REV1) && !GeoIP_db_avail(GEOIP_CITY_EDITION_REV0)) {
		php_error_docref(
			NULL TSRMLS_CC,
			E_WARNING, GEOIPO_ERROR_NO_DATABASE,
			GeoIPDBFileName[GEOIP_CITY_EDITION_REV1]
		); RETURN_FALSE;
	}

	if(this->recordcache == NULL) {
		//php_printf("record lookup %s\n",Z_STRVAL_P(host));
		if(GeoIP_db_avail(GEOIP_CITY_EDITION_REV1))
			geo = GeoIP_open_type(GEOIP_CITY_EDITION_REV1,GEOIP_STANDARD);
		else
			geo = GeoIP_open_type(GEOIP_CITY_EDITION_REV0,GEOIP_STANDARD);
		
		rec = GeoIP_record_by_name(geo,Z_STRVAL_P(host));
		GeoIP_delete(geo);
	} else {
		//php_printf("record reuse %s\n",Z_STRVAL_P(host));
		rec = this->recordcache;
	}
	
	if(rec == NULL) {
		RETURN_FALSE;
	}
	
	regname = (char*)GeoIP_region_name_by_code(rec->country_code,rec->region);
	timezone = (char*)GeoIP_time_zone_by_country_and_region(rec->country_code,rec->region);
	
	// float to double.
	clon = rec->longitude;
	clat = rec->latitude;
	
	//. build the return object.
	object_init(return_value);
	geoipo_return_object_property(return_value, "ContinentCode", rec->continent_code, IS_STRING TSRMLS_CC);
	geoipo_return_object_property(return_value, "CountryCode",   rec->country_code,   IS_STRING TSRMLS_CC);
	geoipo_return_object_property(return_value, "CountryCode3",  rec->country_code3,  IS_STRING TSRMLS_CC);
	geoipo_return_object_property(return_value, "CountryName",   rec->country_name,   IS_STRING TSRMLS_CC);
	geoipo_return_object_property(return_value, "RegionCode",    rec->region,         IS_STRING TSRMLS_CC);
	geoipo_return_object_property(return_value, "RegionName",    regname,             IS_STRING TSRMLS_CC);
	geoipo_return_object_property(return_value, "City",          rec->city,           IS_STRING TSRMLS_CC);
	geoipo_return_object_property(return_value, "PostalCode",    rec->postal_code,    IS_STRING TSRMLS_CC);
	geoipo_return_object_property(return_value, "Latitude",      &clon,               IS_DOUBLE TSRMLS_CC);
	geoipo_return_object_property(return_value, "Longitude",     &clat,               IS_DOUBLE TSRMLS_CC);
	geoipo_return_object_property(return_value, "TimeZone",      timezone,            IS_STRING TSRMLS_CC);
	
	// GeoIPRecord_delete(rec);
	// keep around the record object instead, this way if an app does something like:
	// $geo->getRecord()->CountryName; $geo->getRecord()->RegionName;
	// it is not actually looking up the library/database twice. we will then invalidate
	// the cache when the host property is changed.
	if(this->recordcache == NULL) this->recordcache = rec;

	return;
}

#pragma mark GeoIP->getRegion
/* object GeoIP->getRegion(void);
 * Returns an object of the region (country code, region code, region name). If the specific
 * region database exists it will use that, else it will fall back to the (potentally)
 * free city database which also has those details. One of the properties will be the
 * database id that you can check against the EDITION constants if you needed to know
 * which one it used. */

PHP_METHOD(GeoIP, getRegion) {

	GeoIP       *geo;
	long         dbid;
	char        *regname;
	char        *timezone;
	zval        *host = geoipo_get_object_property(getThis(),"host" TSRMLS_CC);
	obj_geoip_s *this = (obj_geoip_s *)zend_objects_get_address(getThis() TSRMLS_CC);

	//. use the region database if it exists.
	if(GeoIP_db_avail(GEOIP_REGION_EDITION_REV1) || GeoIP_db_avail(GEOIP_REGION_EDITION_REV0)) {
		GeoIPRegion *reg;

		if(this->regioncache == NULL) {
			//php_printf("region lookup %s\n",Z_STRVAL_P(host));
			if(GeoIP_db_avail(GEOIP_REGION_EDITION_REV1)) {
				geo = GeoIP_open_type(GEOIP_REGION_EDITION_REV1,GEOIP_STANDARD);
				dbid = GEOIP_REGION_EDITION_REV1;
			} else {
				geo = GeoIP_open_type(GEOIP_REGION_EDITION_REV0,GEOIP_STANDARD);
				dbid = GEOIP_REGION_EDITION_REV0;
			}
				
			reg = GeoIP_region_by_name(geo,Z_STRVAL_P(host));
			GeoIP_delete(geo);
		} else {
			//php_printf("region reuse %s\n",Z_STRVAL_P(host));
			reg = this->regioncache;
		}

		if(reg == NULL) {
			RETURN_FALSE;
		}
		
		regname = (char*)GeoIP_region_name_by_code(reg->country_code,reg->region);
		timezone = (char*)GeoIP_time_zone_by_country_and_region(reg->country_code,reg->region);
		
		object_init(return_value);
		geoipo_return_object_property(return_value, "DBID",        &dbid,             IS_LONG   TSRMLS_CC);
		geoipo_return_object_property(return_value, "CountryCode", reg->country_code, IS_STRING TSRMLS_CC);
		geoipo_return_object_property(return_value, "RegionCode",  reg->region,       IS_STRING TSRMLS_CC);
		geoipo_return_object_property(return_value, "RegionName",  regname,           IS_STRING TSRMLS_CC);
		geoipo_return_object_property(return_value, "TimeZone",    timezone,          IS_STRING TSRMLS_CC);

		//GeoIPRegion_delete(reg);
		if(this->regioncache == NULL) this->regioncache = reg;
		
		return;		
	}
	
	//. else use the city database.
	else if(GeoIP_db_avail(GEOIP_CITY_EDITION_REV1) || GeoIP_db_avail(GEOIP_CITY_EDITION_REV0)) {
		GeoIPRecord *rec;

		if(this->recordcache == NULL) {
			//php_printf("record lookup %s\n",Z_STRVAL_P(host));
			if(GeoIP_db_avail(GEOIP_CITY_EDITION_REV1)) {
				geo = GeoIP_open_type(GEOIP_CITY_EDITION_REV1,GEOIP_STANDARD);
				dbid = GEOIP_CITY_EDITION_REV1;
			} else {
				geo = GeoIP_open_type(GEOIP_CITY_EDITION_REV0,GEOIP_STANDARD);
				dbid = GEOIP_CITY_EDITION_REV0;
			}
	
			rec = GeoIP_record_by_name(geo,Z_STRVAL_P(host));
			GeoIP_delete(geo);
		} else {
			//php_printf("record reuse %s\n",Z_STRVAL_P(host));
			rec = this->recordcache;
		}
		
		if(rec == NULL) {
			RETURN_FALSE;
		}
		
		regname = (char*)GeoIP_region_name_by_code(rec->country_code,rec->region);
		timezone = (char*)GeoIP_time_zone_by_country_and_region(rec->country_code,rec->region);

		object_init(return_value);
		geoipo_return_object_property(return_value, "DBID",        &dbid,             IS_LONG   TSRMLS_CC);
		geoipo_return_object_property(return_value, "CountryCode", rec->country_code, IS_STRING TSRMLS_CC);
		geoipo_return_object_property(return_value, "RegionCode",  rec->region,       IS_STRING TSRMLS_CC);
		geoipo_return_object_property(return_value, "RegionName",  regname,           IS_STRING TSRMLS_CC);
		geoipo_return_object_property(return_value, "TimeZone",    timezone,          IS_STRING TSRMLS_CC);	
			
		//GeoIPRecord_delete(rec);
		if(this->recordcache == NULL) this->recordcache = rec;
		
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
