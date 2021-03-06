# Process this file with autoconf to produce a configure script.
AC_INIT([twiin], [0.1.0])
AM_INIT_AUTOMAKE([twiin], [0.1.0])

# Check for options.
AC_ARG_ENABLE(gtkmm, AC_HELP_STRING([--enable-gtkmm],
   [Compile GTKMM support (default=yes)]), , enable_gtkmm=yes)
AM_CONDITIONAL(ENABLE_GTKMM, test x$enable_gtkmm = xyes)

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_C_BIGENDIAN
AC_LANG(C++)
AC_CHECK_LIBM

# Checks for libraries.
PKG_CHECK_MODULES([DENISE], [denise])
PKG_CHECK_MODULES([ANDREA], [andrea])
TWIIN_REQUIRES="$TWIIN_REQUIRES denise andrea"
TWIIN_CFLAGS="$TWIIN_CFLAGS $DENISE_CFLAGS $ANDREA_CFLAGS"
TWIIN_LIBS="$TWIIN_LIBS $DENISE_LIBS $ANDREA_LIBS"

if test x"$enable_gtkmm" = xyes
then
   PKG_CHECK_MODULES([GTKMM_3_0], [gtkmm-3.0])
   TWIIN_REQUIRES="$TWIIN_REQUIRES gtkmm-3.0"
   TWIIN_CFLAGS="$GTKMM_3_0_CFLAGS $TWIIN_CFLAGS -DENABLE_GTKMM"
   TWIIN_LIBS="$GTKMM_3_0_LIBS $TWIIN_LIBS"
fi

# Checks for header files.
AC_CHECK_HEADERS(getopt.h)

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

CPPFLAGS="$TWIIN_CFLAGS $CPPFLAGS"
LIBS="$TWIIN_LIBS $LIBS"

AC_SUBST(TWIIN_REQUIRES)
AC_SUBST(TWIIN_CFLAGS)
AC_SUBST(TWIIN_LIBS)

AC_CONFIG_FILES([twiin.pc Makefile src/Makefile])
AC_OUTPUT

