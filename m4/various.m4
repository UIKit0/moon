AC_DEFUN([MOONLIGHT_CHECK_ZLIB],
[
	AC_CHECK_HEADERS(zlib.h)
	AC_CHECK_LIB(z, inflate, ZLIB="-lz")
])

AC_DEFUN([MOONLIGHT_CHECK_EXECINFO],
[
	AC_CHECK_HEADERS(execinfo.h)
])
