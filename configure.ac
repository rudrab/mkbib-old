AC_PREREQ(2.60)
AC_INIT([mkbib], [2.0], [bnrj.rudra@yahoo.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.9.6 dist-bzip2 subdir-objects -Wno-portability])
AC_PROG_CC

AC_CONFIG_MACRO_DIR([m4])
GNOME_DOC_INIT 

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
