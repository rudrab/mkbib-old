# Copyright (C) 2012 Rudra Banerjee*/
#
#
#    This file is part of mkBIB.
#
#    mkBIB is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    mkBIB is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with mkBIB.  If not, see <http://www.gnu.org/licenses/>.
#
#
BUILT_SOURCES = src/bib.h
AM_YFLAGS = -d 
AM_LFLAGS = -i
# desktop file
# (we can't use INTLTOOL_DESKTOP_RULE here due to lp#605826)
%.desktop: %.desktop.in
#	$(MKDIR_P) $(dir $@); LC_ALL=C $(INTLTOOL_MERGE) -d -u -c $(top_builddir)/po/.intltool-merge-cache $(top_srcdir)/po $< $@
applicationsdir = $(datadir)/applications
applications_DATA = data/mkbib.desktop
dist_noinst_DATA = data/mkbib.desktop.in

# gsettings
#gsettings_SCHEMAS = data/org.mkbib.gschema.xml
##@INTLTOOL_XML_NOMERGE_RULE@
##@GSETTINGS_RULES@

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
SUBDIRS= data help #po
pixmapdir=$(datadir)/images
pixmap_DATA = \
	      images/mkbib.png \
	      images/mkbib.svg \
	      images/icon.png  \
	      images/icon.svg
AM_CFLAGS =  @GTK_CFLAGS@ @LIBSOUP_CFLAGS@
AM_CPPFLAGS= -DPIXMAP=\"$(datadir)/images\"
bin_PROGRAMS = mkbib
mkbib_SOURCES = \ 
                src/bib.y src/scan.l src/caution.c src/search.c \
		src/help.c  src/mkbib.c  src/update_file.c \
		src/biblst src/main.h src/view.c src/file_util.c

mkbib_LDADD = @GTK_LIBS@ @LIBSOUP_LIBS@ @LEXLIB@
EXTRA_DIST=$(pixmap_DATA) m4
CLEANFILES = src/bib.c src/bib.h *~
#DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc
