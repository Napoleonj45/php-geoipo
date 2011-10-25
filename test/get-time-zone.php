<?php

var_dump(array(
	GeoIP::getTimeZone('US','IN'),
	GeoIP::getTimeZone('US','TX'),
	GeoIP::getTimeZone('ABC','DEF')
));

/*////////////////////////////////////////
//// example output //////////////////////

[bob@mckay ~/source/php-geoipo]$ php test/get-time-zone.php   
array(3) {
  [0]=> string(20) "America/Indianapolis"
  [1]=> string(15) "America/Chicago"
  [2]=> bool(false)
}

//////////////////////////////////////////
////////////////////////////////////////*/

?>