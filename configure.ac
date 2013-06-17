AC_PREREQ(2.69)
AC_INIT([mkbib], [2.1],,[mkbib])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.9.6 dist-bzip2 subdir-objects foreign])

#GETTEXT_PACKAGE=AC_PACKAGE_NAME	
#AC_SUBST(GETTEXT_PACKAGE)
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[The name of the gettext domain])
IT_PROG_INTLTOOL(0.40.0)
#AC_SUBST(GETTEXT_PACKAGE, mkbib)
PKG_PROG_PKG_CONFIG([0.22])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

AC_PROG_YACC
if test x"$YACC" = "xyacc"; then
  AC_CHECK_PROG([YACC_EXISTS], [yacc], [yes], [no])
  if test x"$YACC_EXISTS" != xyes; then
    AC_MSG_ERROR([[bison/byacc/yacc not found.
		  Please install bison]])
  fi
fi

AM_PROG_LEX
if test "x$LEX" != xflex; then
  AC_MSG_ERROR([Please install flex before installing.])
fi
if test "x$LEXLIB" != "x-lfl"; then
  if test "x$LEXLIB" != "x-ll" ; then
    AC_MSG_ERROR([Please install flex library before installing.])
  fi
fi



PKG_CHECK_MODULES(LIBSOUP, libsoup-2.4 >= 2.26)
AC_SUBST(LIBSOUP_CFLAGS)
AC_SUBST(LIBSOUP_LIBS)

AM_PATH_GTK_3_0([3.4.0],,AC_MSG_ERROR([Gtk+ 3.0.0 or higher required.]))

GLIB_GSETTINGS
YELP_HELP_INIT

AC_CONFIG_FILES([
		 Makefile
		 help/Makefile
		 data/Makefile
		 po/Makefile.in
		 ])
AC_OUTPUT
