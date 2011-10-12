PHP_ARG_ENABLE(
	geoipo,
	[Whether to enable the "geoipo" extension],
	[  --enable-geoipo      Enable "vehicles" extension support],
	yes
)

AC_DEFINE(
	[WITH_GEOIPO],
	1,
	[if GeoIP OOP should be built]
)

AC_MSG_CHECKING([for GeoIP.h])
for i in $PHP_GEOIPO /usr/local /usr; do
	if test -r $i/include/GeoIP.h; then
		GEOIPO_DIR=$i
		AC_DEFINE(HAVE_GEOIP_H,1,[Whether you have GeoIP.h])
		PHP_ADD_INCLUDE($i/include)
	fi
done

if test -z "$GEOIPO_DIR"; then
	AC_MSG_RESULT([not found])
	AC_MSG_ERROR([Please reinstall the GeoIP C API including dev files])
else
	AC_MSG_RESULT([found])
fi

PHP_SUBST(GEOIPO_SHARED_LIBADD)

AC_CHECK_LIB(geoip, GeoIP_new, [PHP_ADD_LIBRARY(geoip,,GEOIPO_SHARED_LIBADD)], [AC_MSG_ERROR([libgeoip not found])] )

PHP_NEW_EXTENSION(
	[geoipo],
	[src/php_geoipo.c src/class_geoip.c],
	[$ext_shared],
	[]
)
