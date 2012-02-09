<?php

var_dump(array(
	GeoIP::getRegionName('US','IN'),
	GeoIP::getRegionName('US','TX'),
	GeoIP::getRegionName('ABC','DEF')
));

/*////////////////////////////////////////
//// example output //////////////////////

[bob@mckay ~/source/php-geoipo]$ php test/get-region-name.php 
array(3) {
  [0]=> string(7) "Indiana"
  [1]=> string(5) "Texas"
  [2]=> bool(false)
}

//////////////////////////////////////////
////////////////////////////////////////*/

?>