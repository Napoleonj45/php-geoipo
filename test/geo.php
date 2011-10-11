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
$geo = new GeoIP('squirrelshaterobots.com');
var_dump(
	$geo,
	$geo->getCountryCode(),
	$geo->getCountryCode(2),
	$geo->getCountryCode(3),
	$geo->getCountryName(),
	$geo->getContinentCode(),
	$geo->getRecord()
);

?>