<?php

/*////////////////////////////////////////
//// geoip -> geoipo compat layer ////////

// v1.0.0 - 2011-10-25
// Bob Majdak Jr <bob@squirrelshaterobots.com>
// Licensed under the Simplified BSD (2-Clause) license just like the
// rest of this codebase.

it was requested that i build this compatability layer between geoipo
oop and the old original proceedural geoip extention so that you could
update an old project with minimal effort until you had time to do it
properly.

eventually i want to do this in the C side of the extention, but that
still requires more research for me on how to properly implement it.

//////////////////////////////////////////
////////////////////////////////////////*/

function geoipo_continent_code_by_name($host) {
	$geo = new GeoIP($host);
	return $geo->getContinentCode();
}

function geoipo_country_code_by_name($host) {
	$geo = new GeoIP($host);
	return $geo->getCountryCode(2);
}

function geoipo_country_code3_by_name($host) {
	$geo = new GeoIP($host);
	return $geo->getCountryCode(3);
}

function geoipo_country_name_by_name($host) {
	$geoip = new GeoIP($host);
	return $geo->getCountryName();
}

function geoipo_database_info($db) {
	return GeoIP::getDatabaseInfo($db);
}

function geoipo_db_avail($db) {
	return GeoIP::hasDatabase($db);
}

function geoipo_get_all_info() {
	return GeoIP::listDatabases();
}

function geoipo_id_by_name($host) {
	$geo = new GeoIP($host);
	return $geo->getID();
}

function geoipo_isp_by_name($host) {
	$geo = new GeoIP($host);
	return $geo->getISP();
}

function geoipo_org_by_name($host) {
	//. not implemented by geoipo (yet)
	return null;
}

function geoipo_record_by_name($host) {
	$geo = new GeoIP($host);
	return geoipo_falcon_punch_object(clone($geo->getRecord()));
}

function geoipo_region_by_name($host) {
	$geo = new GeoIP($host);
	return geoipo_falcon_punch_object($geo->getRegion());
}

function geoipo_region_name_by_code($rcode) {
	// geoipo is too smart to implement this at the moment.
}

function geoipo_time_zone_by_country_and_region($ccode,$rcode) {
	// geoipo is too smart to implement this at the moment.
}

//////////////////////////////////////////
// utilitarrrr ///////////////////////////

function geoipo_falcon_punch_object($input) {
	$output = array();
	foreach($input as $key => $value) {
		$key = trim(strtolower(preg_replace('/([A-Z])/','_\1',$key)),'_');
		$output[$key] = $value;
	}
	
	return $output;
}

//////////////////////////////////////////
//////////////////////////////////////////

?>