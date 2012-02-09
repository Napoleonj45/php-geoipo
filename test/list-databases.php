<?php

print_r(GeoIP::listDatabases());

/*////////////////////////////////////////
// example output ////////////////////////

Medivh:php-geoipo bob$ php test/list-databases.php 
Array
(
    [0] => stdClass Object
        (
            [DBID] => 1
            [Description] => GeoIP Country Edition
            [Filename] => /usr/local/share/GeoIP/GeoIP.dat
            [Available] => 1
        )

    [1] => stdClass Object
        (
            [DBID] => 2
            [Description] => GeoIP City Edition, Rev 1
            [Filename] => /usr/local/share/GeoIP/GeoIPCity.dat
            [Available] => 1
        )

    [2] => stdClass Object
        (
            [DBID] => 3
            [Description] => GeoIP Region Edition, Rev 1
            [Filename] => /usr/local/share/GeoIP/GeoIPRegion.dat
            [Available] => 
        )

...

//////////////////////////////////////////
////////////////////////////////////////*/

?>