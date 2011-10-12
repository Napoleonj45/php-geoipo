<?php

GeoIP::init('/Users/bob/Temp');
print_r(array(
	'countryfile'  => GeoIP::getDatabaseFile(GeoIP::COUNTRY_EDITION),
	'countryavail' => GeoIP::hasDatabase(GeoIP::COUNTRY_EDITION),
	
	'ispfile'      => GeoIP::getDatabaseFile(GeoIP::ISP_EDITION),
	'ispavail'     => GeoIP::hasDatabase(GeoIP::ISP_EDITION)
));

?>