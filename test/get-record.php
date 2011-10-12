<?php

$geo = new GeoIP('squirrelshaterobots.com');
print_r($geo->getRecord());

/*////////////////////////////////////////
// example output ////////////////////////
 
Medivh:php-geoipo bob$ php test/get-record.php 
stdClass Object
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


//////////////////////////////////////////
////////////////////////////////////////*/

?>