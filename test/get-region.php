<?php

$geo = new GeoIP('squirrelshaterobots.com');
print_r($geo->getRegion());

/*////////////////////////////////////////
// example output ////////////////////////
 
Medivh:php-geoipo bob$ php test/get-region.php 
stdClass Object
(
    [DBID] => 2
    [CountryCode] => US
    [RegionCode] => MI
    [RegionName] => Michigan
    [TimeZone] => America/New_York
)

//////////////////////////////////////////
////////////////////////////////////////*/

?>