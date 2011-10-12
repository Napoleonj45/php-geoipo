<?php

// to set a custom directory, you must GeoIP::init(string) before you use any of
// the GeoIP functions, methods, or classes. It is a limitation of libgeoip that
// once it is set you have to forget.

GeoIP::init('/Users/bob/Temp');

// you do not have to have the databases there to see that the custom directory
// was successful. hasDatabase will tell you if the databases you need are there.

print_r(array(
	'countryfile'  => GeoIP::getDatabaseFile(GeoIP::COUNTRY_EDITION),
	'countryavail' => GeoIP::hasDatabase(GeoIP::COUNTRY_EDITION),
	
	'ispfile'      => GeoIP::getDatabaseFile(GeoIP::ISP_EDITION),
	'ispavail'     => GeoIP::hasDatabase(GeoIP::ISP_EDITION)
));

?>