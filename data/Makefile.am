gladedir = $(datadir)/mkbib/glade
dist_glade_DATA = \
	main-window.ui

desktop_in_data=mkbib.desktop.in
desktop_DATA=$(desktop_in_data:.desktop.in=.desktop)
desktopdir=$(datadir)/applications
@INTLTOOL_DESKTOP_RULE@

CLEANFILES=$(desktop_DATA)

EXTRA_DIST=mkbib.desktop.in
