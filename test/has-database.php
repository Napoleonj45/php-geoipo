<?php

GeoIP::init();
var_dump(array(
	'country'     => GeoIP::hasDatabase(GeoIP::COUNTRY_EDITION),
	'countryfile' => GeoIP::getDatabaseFile(GeoIP::COUNTRY_EDITION),
	
	'isp'         => GeoIP::hasDatabase(GeoIP::ISP_EDITION),
	'ispfile'     => GeoIP::getDatabaseFile(GeoIP::ISP_EDITION),
	
	'invalid'     => GeoIP::hasDatabase(9001),
	'invalidfile' => GeoIP::getDatabaseFile(9001),
));

//. throwing the warnings was quite intentional.

?>