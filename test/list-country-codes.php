<?php

$clist2 = GeoIP::listCountryCodes();
$clist3 = GeoIP::listCountryCodes(3);

$iter = 0;
foreach($clist2 as $code => $country) {
	printf(
		"%s (%s) => %s%s",
		$code,
		key($clist3),
		$country,
		PHP_EOL
	);
	
	next($clist3);
}

?>