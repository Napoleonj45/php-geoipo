<?php

GeoIP::init();
$geo = new GeoIP('squirrelshaterobots.com');
print_r(array(
	'host'          => $geo->host,
	'countrycode'   => $geo->getCountryCode(),
	'countrycode2'  => $geo->getCountryCode(2),
	'countrycode3'  => $geo->getCountryCode(3),
	'countryname'   => $geo->getCountryName(),
	'continentcode' => $geo->getContinentCode()
));

?>