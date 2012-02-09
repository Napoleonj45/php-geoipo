PHP GeoIP OOP (geoipo)
==============================

An object oriented PHP module for GeoIP from Maxmind.

------------------------------
------------------------------

Not seeing any code on Github? Then you may be looking at the master
branch. PHP GeoIP supports both PHP 5.3 and 5.4 in separate branches.
Check out the branch for the version of PHP you need.

PHP 5.4 Support
------------------------------

PHP 5.4.0 and Up: https://github.com/bobmajdakjr/php-geoipo/tree/php-5.4.0

PHP 5.3 Support
------------------------------

PHP 5.3.4 and Up: https://github.com/bobmajdakjr/php-geoipo/tree/php-5.3.4

------------------------------
------------------------------


Project Details
------------------------------

This project has been built to solve two problems of the current
GeoIP module for PHP.

	1. Be Object Oriented
	2. Allow custom directories to be set outside of PHP.INI.

To do this the original would have required too much refactoring,
so I have I have built this from the ground up.


Documentation
------------------------------
 * http://squirrelshaterobots.com/projects/php-geoipo/

Quick How-To
------------------------------
 * http://squirrelshaterobots.com/programming/php/geoip-in-php-with-the-new-php-geoipo-extension/

Installing with PECL/PEAR
------------------------------
 1. pecl channel-discover pecl.squirrelshaterobots.com
 2. pecl install shr/geoipo


Authors
------------------------------
 * Bob Majdak Jr (@bobmajdakjr)


Completed API
------------------------------

### Static Methods
 * string  GeoIP::getDatabaseFile(int dbid);
 * string  GeoIP::getDatabaseInfo(int dbid);
 * string  GeoIP::getRegionName(string ccode, string rcode);
 * string  GeoIP::getTimeZone(string ccode, string rcode);
 * boolean GeoIP::hasDatabase(int dbid);
 * void    GeoIP::init(optional string directory);
 * array   GeoIP::listCountryCodes(optional int len default 2);
 * array   GeoIP::listDatabases(void);

### Instance Properties
 * string  GeoIP->host

### Instance Methods
 * object  GeoIP->__construct(optional string host);
 * string  GeoIP->getContinentCode(void);
 * string  GeoIP->getCountryCode(optional int len default 2);
 * string  GeoIP->getCountryName(void);
 * int     GeoIP->getID(void);
 * string  GeoIP->getISP(void);
 * object  GeoIP->getRecord(void);
 * object  GeoIP->getRegion(void);


Requirements
------------------------------

 * GeoIP C API Library from MaxMind (http://www.maxmind.com/app/c)
 * Installed GeoIP Databases (if you actually want results...)
	
Really Important Notes
------------------------------

It used to be important that both geoip and geoipo were not loaded at
the same time, but I believes that no longer matters due to advances
this module has made.
	
Other Important Notes
------------------------------
	
I have no Windows machines. There is no Windows build script nor do I
know how to write one or how to Windows Windows Windows. If you are
able to provide build scripts for Windows or whatever its needed to
build it I love you long time.


Tested On
------------------------------

 * OS X Lion with the PHP it comes with and libGeoIP from source.

 * Ubuntu 11.04 with PHP from the repo and libGeoIP from source.
   While the package version number is correct something is wrong with
   Ubuntu's package for libGeoIP that it seems to be missing a core
   function in the library. Compiling the library from MaxMind's
   official source with --prefix=/usr fixes the problem.

 * FreeBSD 8 with PHP (ZTS) and libGeoIP from ports.

Databases
------------------------------

On UNIX style systems, the GeoIP database are usually found in
/usr/share/GeoIP, /usr/local/share/GeoIP, or some other path similar to
that. You will need to create that directory, make sure it has read
permissions, and extract your MaxMind databases into it.
	
MaxMind provides two free databases that are updated only once a month,
but will probably be more than good enough to get you rolling:
	
 1. http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz
 2. gunzip it and put it where it needs to be.

 1. http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz
 2. gunzip it, rename it to GeoIPCity.dat, and put it where it needs to be.


Build Instructions
------------------------------

This compiles just like any other module for PHP. Therefore you will need the
PHP dev packages for your system installed if you did not compile your own PHP
from source.

 1. install libGeoIP.
    * FreeBSD: /usr/ports/net/GeoIP
    * Ubuntu: apt-get install libgeoip1 libgeoip-dev
    * Else: http://www.maxmind.com/app/c

 2. install php-geoipo
    1. phpize && ./configure
    2. make
    3. make install

 3. add geoipo.so to your php.ini. and of course reload the webserver to
    reload the changes to the config if needed.


License Stuff
------------------------------

This project and all its files (except for specific listed components) are
licensed under the New BSD License. Here it is...

--------

Copyright (c) 2010-2011
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

     * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

--------
