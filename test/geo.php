<?php

$geo = new GeoIP();
$geo->host = '64.85.165.162';
var_dump($geo,$geo->getCountry());

?>