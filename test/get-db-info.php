<?php

print_r(array(
	'country' => GeoIP::getDatabaseInfo(GeoIP::COUNTRY_EDITION),
	'city'    => GeoIP::getDatabaseInfo(GeoIP::CITY_EDITION),
	'isp'     => GeoIP::getDatabaseInfo(GeoIP::ISP_EDITION)
));

/*////////////////////////////////////////
// example output ////////////////////////

Medivh:php-geoipo bob$ php test/get-db-info.php 
Array
(
    [country] => GEO-106FREE 20110601 Build 1 Copyright (c) 2011 MaxMind Inc All Rights Reserved
    [city] => GEO-533LITE 20111004 Build 1 Copyright (c) 2011 MaxMind Inc All Rights Reserved
    [isp] => 
)

//////////////////////////////////////////
////////////////////////////////////////*/

?>