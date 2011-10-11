<?php

GeoIP::init();

$reflect = new ReflectionClass('GeoIP');

echo "Constants", PHP_EOL;
echo "---------", PHP_EOL;
foreach($reflect->getConstants() as $const => $value)
	echo " * GeoIP::{$const} = {$value}", PHP_EOL;
echo PHP_EOL, PHP_EOL;

echo "Use", PHP_EOL;
echo "---------", PHP_EOL;
$geo = new GeoIP('64.85.165.162');
var_dump(
	$geo,
	$geo->getCountry(),
	$geo->getCountry(2),
	$geo->getCountry(3),
	$geo->getContinent()
);

?>