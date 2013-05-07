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
BUILT_SOURCES = bib.h
SUBDIRS=help
pixmapdir=$(datadir)/images
pixmap_DATA =  images/ast_bib1.png   images/ast_gog.png \
	       images/ast_smp.png    images/mkbib.png \
	       images/ast_entry.png  images/ast_slct.png\
	       images/icon.png       images/icon.svg
AM_CFLAGS = @GTK_CFLAGS@ @LIBSOUP_CFLAGS@
AM_CPPFLAGS= -DPIXMAP=\"$(datadir)/images\"
bin_PROGRAMS = mkbib
mkbib_SOURCES = src/bib.y src/scan.l src/caution.c src/search.c \
		src/help.c  src/mkbib.c  src/update_file.c \
		src/biblst src/main.h src/view.c src/file_util.c
AM_YFLAGS = -d
AM_LFLAGS = -i
mkbib_LDADD = @GTK_LIBS@ @LIBSOUP_LIBS@ @LEXLIB@
EXTRA_DIST=$(pixmap_DATA) gnome-doc-utils.make m4 
CLEANFILES = src/bib.c src/bib.h *~
#DISTCLEANFILES=gnome-doc-utils.make
DISTCHECK_CONFIGURE_FLAGS=--disable-scrollkeeper
