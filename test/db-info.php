<?php

print_r(array(
	'country' => GeoIP::getDatabaseInfo(GeoIP::COUNTRY_EDITION),
	'city'    => GeoIP::getDatabaseInfo(GeoIP::CITY_EDITION),
	'iso'     => GeoIP::getDatabaseInfo(GeoIP::ISP_EDITION)
));

?>