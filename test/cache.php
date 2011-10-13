<?php

$geo = new GeoIP('squirrelshaterobots.com');
print_r(array(
	$geo->getRecord()->CountryCode,
	$geo->getRecord()->CountryName,
	$geo->getRecord()->RegionCode,
	$geo->getRecord()->RegionName
));

$geo->host = 'opsat.net';
print_r(array(
	$geo->getRecord()->CountryCode,
	$geo->getRecord()->CountryName,
	$geo->getRecord()->RegionCode,
	$geo->getRecord()->RegionName
));

/*////////////////////////////////////////
// example output ////////////////////////

// this example output is only when i had
// the debugging statements in. you will
// not see those messages in normal ops.

Medivh:php-geoipo bob$ php test/cache.php 
dumping cache due to host change
database lookup squirrelshaterobots.com
cache reuse squirrelshaterobots.com
cache reuse squirrelshaterobots.com
cache reuse squirrelshaterobots.com
Array
(
    [0] => US
    [1] => United States
    [2] => MI
    [3] => Michigan
)
dumping cache due to host change
database lookup opsat.net
cache reuse opsat.net
cache reuse opsat.net
cache reuse opsat.net
Array
(
    [0] => US
    [1] => United States
    [2] => MI
    [3] => Michigan
)

//////////////////////////////////////////
////////////////////////////////////////*/

?>