AC_PREREQ(2.69)
AC_INIT([mkbib], [2.1], [rudra.banerjee@aol.co.uk])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.9.6 dist-bzip2 subdir-objects foreign])

# Checks for programs.
AC_PROG_CC

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

LT_PREREQ([2.2.6])
LT_INIT
AC_CONFIG_MACRO_DIR([m4])
YELP_HELP_INIT

# Compiling sources with per-target flags requires AM_PROG_CC_C_O
AM_PROG_CC_C_O
AC_PROG_INSTALL

#AC_PROG_LIBTOOL
PKG_CHECK_MODULES(LIBSOUP, libsoup-2.4 >= 2.26)
AC_SUBST(LIBSOUP_CFLAGS)
AC_SUBST(LIBSOUP_LIBS)

AM_PATH_GTK_3_0([3.4.0],,AC_MSG_ERROR([Gtk+ 3.0.0 or higher required.]))

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
		 Makefile
		 help/Makefile
		 ])
AC_OUTPUT
