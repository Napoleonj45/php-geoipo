<?php

/* how to pull up a region listing with timezone using php-geoipo
 * using only the two databases maxmind release for free. */

// this is an example of the new geoipo api. (this module)

$geo = new GeoIP('squirrelshaterobots.com');
$record = $geo->getRecord();

echo "Who: {$geo->host}", PHP_EOL;
echo " * Country: {$record->CountryCode}", PHP_EOL;
echo " * Region: {$record->RegionName}", PHP_EOL;
echo " * Time Zone: {$record->TimeZone}", PHP_EOL;

/*////////////////////////////////////////
// example output ////////////////////////

Medivh:php-geoipo bob$ php examples/cmp-region-geoipo.php 
Who: squirrelshaterobots.com
 * Country: US
 * Region: Michigan
 * Time Zone: America/New_York
 
//////////////////////////////////////////
////////////////////////////////////////*/

?>