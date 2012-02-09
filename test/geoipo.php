<?php

$geo = new GeoIP('microsoft.com');
//$geo = new GeoIP('squirrelshaterobots.com');
print_r(array(
	'geoipo'        => $geo,
	'countrycode'   => $geo->getCountryCode(),
	'countrycode2'  => $geo->getCountryCode(2),
	'countrycode3'  => $geo->getCountryCode(3),
	'countryname'   => $geo->getCountryName(),
	'continentcode' => $geo->getContinentCode(),
	'id'            => $geo->getID(),
	'record'        => $geo->getRecord(),
	'region'        => $geo->getRegion()
));

/*////////////////////////////////////////
// example output ////////////////////////

Medivh:php-geoipo bob$ php test/geoipo.php 
Array
(
    [geoipo] => GeoIP Object
        (
            [host] => squirrelshaterobots.com
        )

    [countrycode] => US
    [countrycode2] => US
    [countrycode3] => USA
    [countryname] => United States
    [continentcode] => NA
    [id] => 225
    [record] => stdClass Object
        (
            [ContinentCode] => NA
            [CountryCode] => US
            [CountryCode3] => USA
            [CountryName] => United States
            [RegionCode] => MI
            [RegionName] => Michigan
            [City] => East Lansing
            [PostalCode] => 48823
            [Latitude] => 42.762699127197
            [Longitude] => -84.44270324707
            [MetroCode] => 551
            [AreaCode] => 517
            [TimeZone] => America/New_York
        )

    [region] => stdClass Object
        (
            [DBID] => 2
            [CountryCode] => US
            [RegionCode] => MI
            [RegionName] => Michigan
            [TimeZone] => America/New_York
        )

)

//////////////////////////////////////////
////////////////////////////////////////*/

?>