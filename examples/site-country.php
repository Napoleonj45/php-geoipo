<?php

GeoIP::init();

if(!GeoIP::hasDatabase(GeoIP::COUNTRY_EDITION)) {
	echo("you need to run this on a system that has a geoip database installed.");
	exit(0);
}

$sites = array(
	'at.pool.ntp.org',
	'de.pool.ntp.org',
	'it.pool.ntp.org',
	'pl.pool.ntp.org',
	'au.pool.ntp.org',
	'nz.pool.ntp.org',
	'jp.pool.ntp.org',
	'th.pool.ntp.org',
	'us.pool.ntp.org',
	'ca.pool.ntp.org'
);

$geo = new GeoIP;
foreach($sites as $site) {
	
	$geo->host = $site;
	printf(
		"Site: %s (%s)".PHP_EOL.
		"=================================".PHP_EOL.
		"\tCountry: %s".PHP_EOL.
		"\tCodes: %s-%s".PHP_EOL.PHP_EOL,
		$geo->host,
		gethostbyname($geo->host),
		$geo->getCountryName(),
		$geo->getContinentCode(),	
		$geo->getCountryCode()
	);
}

?>