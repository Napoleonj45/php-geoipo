<?php

/* how to pull up a region listing with timezone using php-geoip
 * using only the two databases maxmind release for free. */

// this is an example of the original geoip api. (not this module)

$who = 'squirrelshaterobots.com';
$record = geoip_record_by_name($who);
$regionname = geoip_region_name_by_code($record['country_code'],$record['region']);
$timezone = geoip_time_zone_by_country_and_region($record['country_code'],$record['region']);

echo "Who: {$who}", PHP_EOL;
echo " * Country: {$record['country_code']}", PHP_EOL;
echo " * Region: {$regionname}", PHP_EOL;
echo " * Time Zone: {$timezone}", PHP_EOL;

/*////////////////////////////////////////
// example output ////////////////////////

Medivh:php-geoipo bob$ php examples/cmp-region-geoip.php 
Who: squirrelshaterobots.com
 * Country: US
 * Region: Michigan
 * Time Zone: America/New_York
 
//////////////////////////////////////////
////////////////////////////////////////*/

?>