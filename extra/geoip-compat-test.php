<?php

require(sprintf('%s/geoip-compat.php',dirname(__FILE__)));

print_r(array(
	geoipo_record_by_name('burgerking.com'),
	geoipo_region_name_by_code('US','TX'),
	geoipo_time_zone_by_country_and_region('US','TX')
));

?>