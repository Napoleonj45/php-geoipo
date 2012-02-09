<?php

$geo = new GeoIP;
foreach(array('google.com','microsoft.com','opsat.net','att.com') as $host) {
	$geo->host = $host;
	var_dump(array(
		'isp' => $geo->getISP()
	));

}

?>