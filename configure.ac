AC_INIT([evolution-go-to-folder], [0.1.0], [https://github.com/nhinds/evolution-go-to-folder])
AC_CONFIG_SRCDIR(src)

AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR([m4])

AM_MAINTAINER_MODE

AC_SUBST(ACLOCAL_AMFLAGS, "\${ACLOCAL_FLAGS}")

GLIB_GSETTINGS

AC_PROG_CC
AC_ISC_POSIX
AC_HEADER_STDC
AM_DISABLE_STATIC
AM_PROG_LIBTOOL

GNOME_COMPILE_WARNINGS
EVOLUTION_INIT

dnl base packages and versions
LIBGTK_REQUIRED=3.6.0
EVOLUTION_REQUIRED=3.13.0

AC_SUBST(LIBGTK_REQUIRED)
AC_SUBST(EVOLUTION_REQUIRED)

LIBGCONFBRIDGE_CFLAGS=
LIBGCONFBRIDGE_LIBS=
PKG_CHECK_MODULES([LIBGCONFBRIDGE], libgconf-bridge, [HAVE_LIBGCONFBRIDGE="yes"], [HAVE_LIBGCONFBRIDGE="no"])
if test "x$HAVE_LIBGCONFBRIDGE" == "xyes"; then
	AC_DEFINE(HAVE_LIBGCONFBRIDGE, 1, [libgconf-bridge available])
	AC_SUBST(HAVE_LIBGCONFBRIDGE)
fi
AC_SUBST(LIBGCONFBRIDGE_CFLAGS)
AC_SUBST(LIBGCONFBRIDGE_LIBS)

PKG_CHECK_MODULES(MODULE,
  [gtk+-3.0 >= $LIBGTK_REQUIRED dnl
   evolution-shell$EVOLUTION_BASE_VERSION_S >= $EVOLUTION_REQUIRED dnl
])


dnl ***********
dnl GConf stuff
dnl ***********
AC_PATH_PROG(GCONFTOOL, gconftool-2, no)
AM_GCONF_SOURCE_2

# Generated files from automake
AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT

echo ""
echo "Evolution go-to-folder plugin successfully configured"
echo ""
echo "    Installation path: $MODULE_INSTALL_DIR"
echo ""
